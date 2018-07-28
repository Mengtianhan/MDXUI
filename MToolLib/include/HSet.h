// HSet.h 主要用于协助标准库的set的操作
#include <iterator>
#include <iostream>


namespace mj{
	template<typename T, typename C, typename A, template<typename T1, typename C1, typename A1>class S>
	inline S<T, C, A>& operator<<(S<T, C, A>& s, const T& value){
		s.insert(value);
		return s;
	}

	template<typename T, typename C, typename A, template<typename T1, typename C1, typename A1>class S>
	inline S<T, C, A>& operator>>(S<T, C, A>& s, T& value){
		if (s.empty()){
			value = T();
		}
		else{
			auto it = --s.end();
			value = *it;
			s.erase(it);
		}
		return s;
	}

	template<typename T, typename C, typename A, template<typename T1, typename C1, typename A1>class S>
	inline std::ostream& operator<<(std::ostream& os, const S<T, C, A>& s){
		if (s.empty()){
			return os;
		}

		std::copy(s.begin(), s.end(), std::ostream_iterator<T>(os, ","));
		os << *(--s.end());
		return os;
	}

	template<typename T, typename C, typename A, template<typename T1, typename C1, typename A1>class S>
	inline std::istream& operator>>(std::istream& is, S<T, C, A>& s){
		s.clear();
		std::vector<T> temp((std::istream_iterator<T>(is)), std::istream_iterator<T>());
		for (auto& t : temp){
			s.insert(t);
		}
		return is;
	}
}
