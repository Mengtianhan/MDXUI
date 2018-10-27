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
#include "MContainerAllocator.h"
#include "MAllocator.h"
#include "MParamWrap.h"


template<class Type>
class MFasWrap : public MAllocator<MPoolAllocator<Type>>
{
public:
	typedef typename MConstParamWrape<Type>::type const_reference_type;// 也许是引用,也许不是，比如标量类型
	typedef typename MParamWrape<Type>::type reference_type;     // 也许是引用,也许不是，比如标量类型
public:
	MFasWrap(const_reference_type val = Type()) :mObj(val){}

	MFasWrap(const MFasWrap& other) :mObj(other.mObj){}
	MFasWrap(MFasWrap&& other){
		mObj = other.mObj;
		other.mObj.~Type();
	}
	MFasWrap& operator =(const_reference_type val){
		mObj = val;
		return *this;
	}
	MFasWrap& operator =(Type&& val){
		mObj = val;
		val.~Type();
		return *this;
	}
	MFasWrap& operator =(const MFasWrap& other){
		mObj = other.mObj;
		return *this;
	}
	MFasWrap& operator =(MFasWrap&& other){
		mObj = other.mObj;
		other.mObj.~Type();
		return *this;
	}

	operator Type(){
		return mObj;
	}
	reference_type toType(){
		return mObj;
	}
	const_reference_type toType() const{
		return mObj;
	}


	friend std::ostream& operator<<(std::ostream& os, const MFasWrap& other){
		os << other.mObj;
		return os;
	}
	friend std::istream& operator>>(std::istream& is, MFasWrap& other){
		is >> other.mObj;
		return is;
	}
private:
	Type mObj;
};