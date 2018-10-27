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

#include "MTypeWrap.h"
#include "MMemDB.h"

namespace TL{
	template<class T>
	using List = MTypeWrap<std::vector<T>>;

	template<class T>
	using Vector = MTypeWrap<std::vector<T>>;

	template<class K, class V>
	using Map = MTypeWrap<std::map<K, V>>;

	//
	// 用于初始化 Map
	//
	template<class K, class V>
	Map<K, V> map_list(const K& k, const V& v){
		return Map<K, V>()(k, v);
	}

	//
	// 只适应于单线程
	//
	template<class T, class...Args>
	using MSNoSqlDB = MNoSqlDB<MSingleThreadMode, T, Args...>;

	//
	// 可用于多线程操作
	//
	template<class T, class...Args>
	using MMNoSqlDB = MNoSqlDB<MMultThreadMode, T, Args...>;

	//
	// 一个包含MString的矩阵
	//
	typedef mj::MMatrixSheet<MString> smat;
}
