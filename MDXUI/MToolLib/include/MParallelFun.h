///////////////////////////////////////////////////////////////////////////////
//
// MToolLib ---- Mengjin Tool Library
// Copyright (C) 2014-2018 by Mengjin (sh06155@hotmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <MThreadMode.h>
#include <MParamWrap.h>
#include <MScopeLock.h>
#include <omp.h>
//
// 使用omp并行加速
// MSingleThreadMode 将使执行速度提高
//
template<class ThreadMode = MSingleThreadMode>
class MParallelFun : public ThreadMode{
private:
	template<bool>
	struct Apply{
		template<class...Args>
		static void Run(MParallelFun* ptr, void(*Fun) (int,Args...), Args... args){
#pragma omp parallel for 
			for (int i = 0; i < ptr->mThreadNum; ++i){
			//	MScopeLock<MThreadFun> LockForThread(ptr);
				Fun(i,args...);
			}
		}

		template<class T, class...Args>
		static void Run(MParallelFun* ptr, void(T::*Fun)(int,Args...), T* obj, Args...args){
#pragma omp parallel for 
                        for (int i = 0; i < ptr->mThreadNum; ++i){
				(obj->*Fun)(i,args...);
			}
		}

                template<class...Args>
                static void Run(MParallelFun* ptr, std::function<void(int,Args...)> fun, Args...args){
#pragma omp parallel for
                        for (int i = 0; i < ptr->mThreadNum; ++i){
                                fun(i,args...);
                        }
                }
	};

	template<>
	struct Apply<false>{
		template<class R,class...Args>
		static R Run(MParallelFun* ptr, R(*Fun) (int,Args...), Args... args){
                        R res = R();
#pragma omp parallel for reduction(+:res)
                        for (int i = 0; i < ptr->mThreadNum; ++i){
				res += Fun(i, args...);
                        }
			return res;
		}

		template<class R,class T, class...Args>
		static R Run(MParallelFun* ptr, R(T::*Fun)(int,Args...), T* obj, Args...args){
                        R res = R();
#pragma omp parallel for reduction(+:res)
                        for (int i = 0; i < ptr->mThreadNum; ++i){
				res += (obj->*Fun)(i,args...);
                        }
			return res;
		}

                template<class R,class...Args>
                static R Run(MParallelFun* ptr, std::function<R(int,Args...)> fun, Args...args){
                        R res = R();
#pragma omp parallel for  reduction(+:res)
                        for (int i = 0; i < ptr->mThreadNum; ++i){
                                res += fun(i,args...);
                        }
                        return res;
                }
	};

public:
	MParallelFun(unsigned loopNum = 1) :mThreadNum(loopNum){}

	template<class R,class...Args>
	R Run(R(*Fun) (int,Args...), Args... args){
		return Apply<MIs_Same<R, void>::value > ::Run(this,Fun,args...);
	}

	template<class R,class T,class...Args>
	R Run(R(T::*Fun)(int,Args...), T* obj,Args...args){
		return Apply<MIs_Same<R, void>::value > ::Run(this, Fun, obj,args...);
	}

        template<class R,class...Args>
        R Run(std::function<R(int,Args...)> fun,Args...args){
                return Apply<MIs_Same<R, void>::value > ::Run(this, fun, args...);
        }

private:
	unsigned  mThreadNum{ 1 };
};
