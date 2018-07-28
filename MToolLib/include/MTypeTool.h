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
