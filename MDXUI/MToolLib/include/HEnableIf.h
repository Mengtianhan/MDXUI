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

namespace{
	struct NA
	{
		typedef NA type;
		enum { value = 0 };
	};

#define MJ_NA_PARAM(param) param = NA

	template<
		bool C
		, typename T1
		, typename T2
	>
	struct if_c
	{
		typedef T1 type;
	};

	template<
		typename T1
		, typename T2
	>
	struct if_c<false, T1, T2>
	{
		typedef T2 type;
	};

	template<
		typename MJ_NA_PARAM(T1)
		, typename MJ_NA_PARAM(T2)
		, typename MJ_NA_PARAM(T3)
	>
	struct if_
	{
	private:
		typedef if_c<
			static_cast<bool>(T1::value)
			, T2
			, T3
		> almost_type_;

	public:
		typedef typename almost_type_::type type;
	};

	template <bool B, class T = void>
	struct disable_if_c {
		typedef T type;
	};

	template <class T>
	struct disable_if_c<true, T> {};

	template <class Cond, class T = void>
	struct disable_if : public disable_if_c<Cond::value, T> {};

	template <bool B, class T = void>
	struct enable_if_c {
		typedef T type;
	};

	template <class T>
	struct enable_if_c<false, T> {};

	template <class Cond, class T = void>
	struct enable_if : public enable_if_c<Cond::value, T> {};
}
