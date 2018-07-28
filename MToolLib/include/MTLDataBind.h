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