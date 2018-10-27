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
#include <MAny.h>
struct MNullType;

struct MEmptyType{};

template<class>
struct MEmptyTemp{};


//
// Any
//
using mj::MAny;
struct MAnyHolder{
public:
	MAnyHolder(){}
	MAnyHolder(const MAny& __any) :mAny(__any){}
	MAnyHolder& operator=(const MAny& __any){
		mAny = __any;
		return *this;
	}

	MAnyHolder(const MAnyHolder& other) :mAny(other.mAny){}
	MAnyHolder& operator=(const MAnyHolder& other){
		mAny = other.mAny;
		return *this;
	}

	MAny  toAny(){ return mAny; }
	MAny  toAny() const{ return mAny; }

private:
	MAny mAny;
};