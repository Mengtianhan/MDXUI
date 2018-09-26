
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
#include <MTLProperty.h>
#include <MTLConnect.h>

namespace TL{
	//
	// 在操作 MDataBind::Connect 的时候 应该使用辅助宏和一个辅助函数
	// Connect 只负责单边绑定
	// MSIGNAL 所指定的是源 目前一个源只能对应一个slot,但是一个slot可以对应多个源
	// MSLOT 所指定的为接收对象的函数
	// 所以  MDataBind::Connect 调用操作应该为:
	// MDataBind::Connect(
	//                  MSIGNAL(ClassB,SetValue,pBobj),
	//                  MSLOT(&ClassA::SetValue,pAobj)
	//                  );
	//
	// MSLOT 和 MSIGNAL 的包装显得有些别扭,不同一样的操作是为让
	//
	// 但对于自由函数来说,只需要使用MSLOT直接包装函数指针即可
	// MDataBind::Connect(
	//                  MSIGNAL(ClassB,SetValue,pBobj),
	//                  MSLOT(freeFunPtr)
	//                  );
	// freeFunPtr 是一个函数指针 在MSLOT的包装下就可以作为槽函数通信
	//
	//

	//
	// 使用函数对象来对属性进行绑定
	//
	class MTLDataBind
	{
	public:
		MTLDataBind(){}
		virtual ~MTLDataBind(){}

		//
		// 第二类Connect函数
		//
		template<class T, class...Args>
		static std::shared_ptr<MConnect<T>> Connect(
			const MString& funName,
			void(T::*fun1)(Args...),
			T* obj1,
			std::function<void(Args...)> eventfun,
			const MTLProperty::ObjTraceType& tracePtr = MTLProperty::ObjTraceType(),
			bool isAsyc = false
			)
		{
			MAddPropertChangedFun(
				funName,
				fun1,
				obj1,
				eventfun,
				tracePtr,
				isAsyc);
			std::shared_ptr<MConnect<T>> connectObj(new MConnect<T>(
				funName,
				obj1,
				eventfun,
				tracePtr,
				isAsyc));
			return connectObj;
		}

		//
		// 第二类断开连接
		//
		template<class T, class...Args>
		static void DisConnect(
			const MString& funName,
			void(T::*fun1)(Args...),
			T* obj1,
			std::function<void(Args...)> eventfun,
			const MTLProperty::ObjTraceType& tracePtr = MTLProperty::ObjTraceType()
			)
		{
			MRemovePropertChangedFun(funName, obj1, tracePtr);
		}

	private:


		//
		// 添加属性改变函数
		//
		template<class T, class ...Args>
		static void MAddPropertChangedFun(const MString& funName,
			void(T::*fun)(Args...),
			T* obj,
			std::function<void(Args...)> eventfun,
			const MTLProperty::ObjTraceType& tracePtr,
			bool isAsyc)
		{
			if (obj == nullptr)
				return;
			(obj->BindGroupFun)(funName, tracePtr,eventfun, isAsyc);
		}

		//
		// 断开连接
		//
		template<class T>
		static void MRemovePropertChangedFun(const MString& funName,
			T* obj,
			const MTLProperty::ObjTraceType& tracePtr)
		{
			if (obj == nullptr)
				return;
			(obj->RemoveGroupFun)(funName, tracePtr);
		}
	};
}