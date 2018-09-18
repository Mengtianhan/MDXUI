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
