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
