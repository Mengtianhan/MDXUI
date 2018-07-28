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
