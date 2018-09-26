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

#include "MParamWrap.h"

//
// 抽象工厂函数
//
namespace mj{

	template<class T>
	struct TypeDispatch{};

	//
	// 默认工厂函数
	//
	template<class T>
	struct MDefaultAFHolder{
		virtual T* DoCreate(TypeDispatch<T>){
			return new T();
		}
		virtual void DoDestroy(T*& ptr, TypeDispatch<T>){
			if (ptr){
				delete ptr;
				ptr = nullptr;
			}
		}
		virtual ~MDefaultAFHolder(){}
	};



	template<class T, template<class> class H = MDefaultAFHolder>
	class MAbstractFactory : public mjTL::MTLClass<T, H>{
	public:
		//
		// 创建
		//
		template<class R>
		R* Create(){
			H<R>& abf = *this;
			return abf.DoCreate(TypeDispatch<R>());
		}

		//
		// 销毁
		//
		template<class R>
		void Destroy(R*& ptr){
			H<R>& abf = *this;
			abf.DoDestroy(ptr,TypeDispatch<R>());
		}
	};
}