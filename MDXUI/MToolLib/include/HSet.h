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
