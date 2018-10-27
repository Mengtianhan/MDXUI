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

#include <MTypeTool.h>
#include <future>
#include <MAny.h>
#include <MNoCopy.h>
#include "MTLConfig.h"
#include <HContainer.h>
#include <MTLTracePtr.h>


//
// 属性绑定第二版,该版本计划实现多槽连接,同时支持同步异步的选择
// 要实现同步和异步对于带有GUI操作的东西来说将是一个非常复杂的过程
// 由于不可能重新封装一个消息循环
// 所以在和线程不安全的GUI对象同步的时候需要自己注意
//

#ifndef msignals
#define msignals public
#endif


#ifndef mevent
#define mevent public
#endif

namespace TL{
	class MTLObject;
	class  MTLProperty
	{
		friend class MTLDataBind;
		template<class T>
		friend class MConnect;
	public:
		//
		// 函数名,对象跟踪器,响应函数,同步还是异步
		//
		typedef MTypeWrap<MTLTracePtr<MTLObject, MTLDataBind>> ObjTraceType;
		typedef TL::MSNoSqlDB<MString, ObjTraceType, mj::MAny, bool> SLOTTYPE;

		MTLProperty(){}
		~MTLProperty(){ mSlots.clear(); }


		//
		// 当连接多个槽对象的时候使用该函数进行通知
		// 如果只连接一个槽对象那么使用MProperty的通知
		//
		template<class T, class...Args>
		bool NotifyAll(
			const MString& funName,
			void(T::*func)(Args...),
			Args...agrs
			)
		{
			auto funs = MGetGroupFun<Args...>(funName, this);
			bool isok = false;
			for (auto& m : funs){
				if (m.first){
					isok = true;
					if (m.second){
						std::async(std::launch::async, [=](){m.first(agrs...); });
					}
					else{
						m.first(agrs...);
					}
				}
			}
			return isok;
		}

		//
		// 就算是子类也禁止访问
		//
	private:

		//
		// 获取槽函数
		//
		TL::MSNoSqlDB<mj::MAny,bool> GetGroupFun(const MString &funName){
			if (mSlots.empty()){
				return TL::MSNoSqlDB<mj::MAny, bool>();
			}

			mSlots.erase_if<1>([](const ObjTraceType& obj){
				return !obj.reference_obj().isAccepted();
			});

			TL::MSNoSqlDB<mj::MAny, bool> Slos = mSlots.colum_db<2, 3, 0,mj::MStrEquals<MString>>(funName);
			return Slos;
		}

		template<class...Args>
		void BindGroupFun(const MString& funName, const ObjTraceType& traceobj, const mj::MAny& fun, bool isAsyc){
			int num = mSlots.counts<0, 1, mj::MStrEquals<MString>>(funName,traceobj);
			if (num == 0){
				mSlots.push_back(funName, traceobj, fun, isAsyc);
				ObjTraceType& traceobj = mSlots.at<1>(mSlots.rows() - 1);
				traceobj.reference_obj().attach();
			}
			else{
				return;
			}
		}

		//
		// 断开相关槽函数
		//
		void RemoveGroupFun(const MString& funName, const ObjTraceType& traceobj){
			if (mSlots.empty()){
				return;
			}
			mSlots.eraseall<MString, 0, ObjTraceType, 1, mj::MStrEquals<MString>>(funName, traceobj);
		}

		//
		// 获取多参数函数
		//
		template<class...Args>
		std::vector<std::pair<std::function<void(Args...)>, bool>> MGetGroupFun(
			const MString& funName,
			const MTLProperty* obj)
		{
			typedef std::function<void(Args...)> funType;
			std::vector<std::pair<funType, bool>> funs;
			if (obj == nullptr)
				return funs;
			TL::MSNoSqlDB<mj::MAny, bool> slotsfun = const_cast<MTLProperty*>(obj)->GetGroupFun(funName);
			for (auto& v : slotsfun){
				mj::MAny any = TL::get<0>(v);
				bool isAsyc = TL::get<1>(v);
				if (any.empty())
					continue;
				try{
					funType fun = mj::any_cast<funType>(any);
					if (fun)
						funs.push_back(std::make_pair(fun, isAsyc));
				}
				catch (...){
					;
				}
			}
			return funs;
		}

	private:
		SLOTTYPE mSlots;
	};
}
