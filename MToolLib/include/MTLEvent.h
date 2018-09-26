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
//+----------------------
//
// 事件基类
//
//+----------------------
namespace TL{

	template<class R,class...Args>
	class MTLEvent;

	//+-----------------------------------
	//
	// 添加 和 移除 函数实现
	//
	//+------------------------------------
	template<class FunType>
	class EventBaseImpl{
	public:
		EventBaseImpl(){}
		virtual ~EventBaseImpl(){ m_Funs.clear(); }
		template<class Fun>
		int operator+=(Fun fun){
			int id = 0;
			while (m_Funs.count(id)){
				++id;
			}
			m_Funs[id] = fun;
			return id;
		}


		void operator-=(int id){
			if (m_Funs.count(id)){
				m_Funs.erase(id);
			}
		}


		template<class Fun>
		EventBaseImpl& operator=(Fun fun){
			m_Funs.clear();
			if(fun == nullptr)
				return *this;
			m_Funs[-1] = fun;
			return *this;
		}

		void clear(){
			m_Funs.clear();
		}

		std::vector<int> fun_ids(){
			std::vector<int> ids;
			for (auto& m : m_Funs){
				ids.push_back(m.first);
			}
			return ids;
		}

	protected:
		std::map<int,FunType>  m_Funs;

	private:
		EventBaseImpl(const EventBaseImpl&) = delete;
		EventBaseImpl& operator=(EventBaseImpl&) = delete;
	};



	//+------------------------------------------------------------------
	//
	// 实现任意参数
	//
	//+------------------------------------------------------------------
	template<class R,class...Args>
	class MTLEvent : public EventBaseImpl<std::function<R(Args...)>>{
	public:
		MTLEvent(){}
		virtual ~MTLEvent(){}
		void operator()(const Args&... arg){
			for (auto& m : m_Funs){
				if (m.second){
					m.second(arg...);
				}
			}
		}
	};

	//
	// 特殊返回值为void
	//
	template<class...Args>
	class MTLVoidEvent: public MTLEvent<void,Args...>{
	public:
		MTLVoidEvent(){}
		virtual ~MTLVoidEvent(){}
	};
}
