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