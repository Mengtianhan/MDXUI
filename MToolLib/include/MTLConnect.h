#pragma once
#include <MAny.h>
#include "MString.h"
#include <MNoCopy.h>
#include <MParamWrap.h>
#include <MTLTracePtr.h>
#include <MTLDataBind.h>


namespace TL{
	typedef MTypeWrap<MTLTracePtr<MTLObject, MTLDataBind>>  MTraceObjectType;

	//
	// MScopeConnect 会自动断开连接操作
	// 模板参数为源类型
	//
	template<class T>
	class MConnect : public MNoCopy
	{
		friend class MTLDataBind;

	private:
		MConnect(
			const MString& funName, 
			T* obj, 
			const mj::MAny& fun,
			const MTraceObjectType& traceobj,
			bool isAsyc = false
			) :
			mFunName(funName), 
			mObj(obj), 
			mFun(fun),
			mTraceObj(traceobj),
			bIsAsyc(isAsyc)
		{
		}

	public:
		~MConnect()
		{
		}


		template<bool>
		struct ApplyImpl{
			static void Connect(MConnect<T>* ptr){
				ptr->mObj->BindPropertChangedFun(ptr->mFunName, ptr->mFun);
			}

			static void DisConnect(MConnect<T>* ptr){
				std::function<void()> fun{ nullptr };
				ptr->mObj->BindPropertChangedFun(ptr->mFunName, fun);
			}
		};

		template<>
		struct ApplyImpl<true>{
			static void Connect(MConnect<T>* ptr){
				ptr->mObj->BindGroupFun(ptr->mFunName, ptr->mTraceObj, ptr->mFun, ptr->bIsAsyc);
			}

			static void DisConnect(MConnect<T>* ptr){
				ptr->mObj->RemoveGroupFun(ptr->mFunName, ptr->mTraceObj);
			}
		};

		void Connect()
		{
			if (mTraceObj.reference_obj().isAccepted() == false)
				return;

			if (mObj && !mFunName.empty()){
				try{
					ApplyImpl<MIsBDClass(MTLProperty, T)>::Connect(this);
				}
				catch (...){
					;
				}
			}
		}
		void DisConnect()
		{
			if (mTraceObj.reference_obj().isAccepted() == false)
				return;

			if (mObj && !mFunName.empty()){
				try{
					ApplyImpl<MIsBDClass(MTLProperty, T)>::DisConnect(this);
				}
				catch (...){
					;
				}
			}
		}
		void Block(bool isblock)
		{
			if (isblock){
				DisConnect();
			}
			else{
				Connect();
			}
		}

	private:
		T*						mObj{ nullptr };
		mj::MAny				mFun{ nullptr };
		mj::MString				mFunName;
		MTraceObjectType		mTraceObj;
		bool					bIsAsyc{ false };
	};
}