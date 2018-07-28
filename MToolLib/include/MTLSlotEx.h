#pragma once
#include <MTLSlot.h>

namespace TL2{
	//
	// MSLOT的扩展
	// 最多支持四个额外参数的包装
	//
	template<size_t N>
	struct MPropertyFunHelpEx{
		template<class T, class F, class K, class...Args>
		static inline auto Apply(T t, F fun, K obj, Args...args)->
			decltype(
			MPropertyFunHelpEx<N - 1>::Apply
			(
			t, fun, obj,
			std::get<N - 1>(t),
			args...)
			)
		{
			return MPropertyFunHelpEx<N - 1>::Apply
				(
				t, fun, obj,
				std::get<N - 1>(t),
				args...
				);
		}

		template<class T, class F, class...Args>
		static inline auto Apply2(T t, F fun, Args...args)->
			decltype(
			MPropertyFunHelpEx<N - 1>::Apply2
			(
			t, fun, 
			std::get<N - 1>(t),
			args...)
			)
		{
			return MPropertyFunHelpEx<N - 1>::Apply2
				(
				t, fun, 
				std::get<N - 1>(t),
				args...
				);
		}
	};


	//
	// 递归终止
	//
	template<>
	struct MPropertyFunHelpEx<0>{
		template<class T, class F, class K, class...Args>
		static inline auto Apply(T t, F fun, K obj, Args...args)->decltype(
			std::bind(fun, obj, args...)
			)
		{
			return std::bind(fun, obj, args...);
		}

		template<class T, class F, class...Args>
		static inline auto Apply2(T t, F fun, Args...args)->decltype(
			std::bind(fun, args...)
			)
		{
			return std::bind(fun, args...);
		}
	};

	template<int N, int M,int M2, class R>
	struct ToFunEx{
		template<class T, class F, class K, class...Args>
		static std::function<R(Args...)> Apply(T t, F fun, K obj){
			auto __t = std::tuple_cat(t, std::make_tuple(std::_Ph<N + 1>()));
			return ToFunEx<N + 1, M, M2, R>::Apply<decltype(__t), F, K, Args...>
				(__t, fun, obj);
		}

		template<class T, class F, class...Args>
		static std::function<R(Args...)> Apply2(T t, F fun){
			auto __t = std::tuple_cat(t, std::make_tuple(std::_Ph<N + 1>()));
			return ToFunEx<N + 1, M, M2, R>::Apply2<decltype(__t), F, Args...>
				(__t, fun);
		}
	};


	//
	// 递归终止
	//
	template<int N, int M2,class R>
	struct ToFunEx<N, N, M2, R>{
		template<class T, class F, class K, class...Args>
		static std::function<R(Args...)> Apply(T t, F fun, K obj){
			return MPropertyFunHelpEx<sizeof...(Args)+M2>::Apply(t, fun, obj);
		}

		template<class T, class F, class...Args>
		static std::function<R(Args...)> Apply2(T t, F fun){
			return MPropertyFunHelpEx<sizeof...(Args)+M2>::Apply2(t, fun);
		}
	};

	//
	// 包装成员函数
	//
	template<class T, class R, class A, class...Args>
	std::function<R(Args...)> MSLOT(R(T::*fun)(A, Args...), T* obj, const A& obja){
		auto t = std::make_tuple(obja);
		return ToFunEx<0, sizeof...(Args), 1, R>::Apply<decltype(t), decltype(fun), T*, Args...>
			(
			t, fun, obj
			);
	}

	template<class T, class R,class A,class B, class...Args>
	std::function<R(Args...)> MSLOT(R(T::*fun)(A,B, Args...), T* obj, const A& obja,const B& objb){
		auto t = std::make_tuple(obja,objb);
		return ToFunEx<0, sizeof...(Args), 2, R>::Apply<decltype(t), decltype(fun), T*, Args...>
			(
			t, fun, obj
			);
	}

	template<class T, class R, class A, class B, class C,class...Args>
	std::function<R(Args...)> MSLOT(R(T::*fun)(A, B, C,Args...), T* obj, const A& obja, const B& objb,const C& objc){
		auto t = std::make_tuple(obja, objb, objc);
		return ToFunEx<0, sizeof...(Args), 3, R>::Apply<decltype(t), decltype(fun), T*, Args...>
			(
			t, fun, obj
			);
	}


	template<class T, class R, class A, class B, class C,class D, class...Args>
	std::function<R(Args...)> MSLOT(R(T::*fun)(A, B, C,D, Args...), T* obj, const A& obja, const B& objb, const C& objc,const D& objd){
		auto t = std::make_tuple(obja, objb, objc, objd);
		return ToFunEx<0, sizeof...(Args), 4, R>::Apply<decltype(t), decltype(fun), T*, Args...>
			(
			t, fun, obj
			);
	}

	//
	// 包装额外参数的自由函数
	//
	template<class R, class A, class...Args>
	std::function<R(Args...)> MSLOT(R(*fun)(A, Args...), const A& obja){
		auto t = std::make_tuple(obja);
		return ToFunEx<0, sizeof...(Args), 1, R>::Apply2<decltype(t), decltype(fun), Args...>
			(
			t, fun
			);
	}

	template<class R, class A, class B, class...Args>
	std::function<R(Args...)> MSLOT(R(*fun)(A, B, Args...), const A& obj1, const B& objb){
		auto t = std::make_tuple(obj1, objb);
		return ToFunEx<0, sizeof...(Args), 2, R>::Apply2<decltype(t), decltype(fun), Args...>
			(
			t, fun
			);
	}

	template<class R, class A, class B, class C,class...Args>
	std::function<R(Args...)> MSLOT(R(*fun)(A, B, C,Args...), const A& obja, const B& objb,const C& objc){
		auto t = std::make_tuple(obja, objb, objc);
		return ToFunEx<0, sizeof...(Args), 3, R>::Apply2<decltype(t), decltype(fun), Args...>
			(
			t, fun
			);
	}

	template<class R, class A, class B, class C, class D,class...Args>
	std::function<R(Args...)> MSLOT(R(*fun)(A, B, C, Args...), const A& obja, const B& objb, const C& objc,const D& objd){
		auto t = std::make_tuple(obja, objb, objc, objd);
		return ToFunEx<0, sizeof...(Args), 4, R>::Apply2<decltype(t), decltype(fun), Args...>
			(
			t, fun
			);
	}
}