// HMap.h 主要用于协助标准库的map的操作
#include <iterator>
#include <iostream>

namespace mj{

	template<typename K, typename V, typename C, typename A, template<typename K1, typename V1, typename C1, typename A1>class M>
	inline M<K, V, C, A>& operator<<(M<K, V, C, A>& m, const std::pair<K, V>& p){
		m[p.first] = p.second;
		return m;
	}

	template<typename K, typename V, typename C, typename A, template<typename K1, typename V1, typename C1, typename A1>class M>
	inline M<K, V, C, A>& operator>>(M<K, V, C, A>& m, std::pair<K, V>& p){
		if (m.empty()){
			p = std::make_pair(K(), V());
		}
		else{
			auto it = --m.end();
			p = *it;
			m.erase(it);
		}
		return m;
	}


	template<typename K, typename V, typename C, typename A, template<typename K1, typename V1, typename C1, typename A1>class M>
	inline std::ostream& operator<<(std::ostream& os, const M<K, V, C, A>& m){
		if (m.empty())
			return os;
		for (auto& p : m){
			os << p.first << " : " << p.second << std::endl;
		}
		return os;
	}

	template<typename K, typename V, typename C, typename A, template<typename K1, typename V1, typename C1, typename A1>class M>
	inline std::istream& operator>>(std::istream& is, M<K, V, C, A>& m){
		m.clear();
		K k;
		V v;
		while (!is.eof()){
			is >> k >> v;
			m[k] = v;
		}
		return is;
	}
}