#pragma once
#include <functional>
#include <unordered_map>
#include <map>
#include <tuple>

namespace mj{
	template<class R, class...Args>
	inline std::function<R(Args...)> FunCache(R(*fun)(Args...args)){
		auto result_map = std::make_shared<std::map<std::tuple<Args...>, R>>();
		return ([=](Args... args){
			std::tuple<Args...> t(args...);
			if (result_map->find(t) == result_map->end())
				(*result_map)[t] = fun(args...);
			return (*result_map)[t];
		});
	}

	template<class R, class...Args>
	inline std::function<R(Args...)> FunSugar(R(*fun)(Args...), bool isneedclear = false){
		using fun_type = std::function<R(Args...)>;
		static std::unordered_map<decltype(fun), fun_type> fun_map;
		if (isneedclear)
			return fun_map[fun] = FunCache(fun);
		if (fun_map.find(fun) == fun_map.end())
			fun_map[fun] = FunCache(fun);
		return fun_map[fun];
	}
}
