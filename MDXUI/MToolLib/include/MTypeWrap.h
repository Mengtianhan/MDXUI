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
#include <iostream>
#include "MEmptyType.h"
#include "MParamWrap.h"
#include "MAllocator.h"
#include "MRefWrap.h"
#include "MContainerAllocator.h"

//
// 对类型进行包装
//
template<class Type, class Allocator>
class MTypeWrap;

template<class Type, class Allocator>
std::ostream& operator<<(std::ostream& os, const MTypeWrap<Type, Allocator>& other);

template<class Type, class Allocator = MEmptyType>
std::istream& operator>>(std::istream& is, MTypeWrap<Type, Allocator>& other);



//
// 包装类
//
template<class Type, class Allocator = MEmptyType>
class MTypeWrap : public Allocator
{
public:
	typedef typename MConstParamWrape<Type>::type const_reference_type;// 也许是引用,也许不是，比如标量类型
	typedef typename MParamWrape<Type>::type reference_type;     // 也许是引用,也许不是，比如标量类型
	typedef typename MAddReference<Type>::type reference_type2;  // 真正的引用


public:
	MTypeWrap(const_reference_type val = Type());
	MTypeWrap& operator =(const_reference_type val);
	
	operator Type();
	reference_type toType();
	const_reference_type toType() const;

	reference_type2 reference_obj();
	const reference_type2 reference_obj() const;

	MTypeWrap clone();
	MTypeWrap clone() const;

	void lock();
	void unlock();
	bool islock() const;

	const std::type_info & type() const;
	void* Address() const;

	void print(const char* msg = "", std::ostream& os = std::cout) const;

	template<class A,class B>
	friend std::ostream& operator<<(std::ostream& os, const MTypeWrap<A, B>& other);
	template<class A, class B>
	friend std::istream& operator>>(std::istream& is, MTypeWrap<A, B>& other);

private:
	class MContentType : public MRefImpl<MAllocator<MPoolAllocator<MContentType>>>
	{
	public:
		MContentType(const_reference_type val = Type());
		MContentType(const MContentType& other);
		MContentType& operator=(const MContentType& other);
		MContentType& operator =(const_reference_type val);
		operator Type();
		reference_type toType();
		const_reference_type toType() const;
		const std::type_info & type() const;
		Type   mVal;
	};
	MRefWrap<MContentType> mVal;
};

/*-----------------------------------------------------------------------------------------------*/
/*   实现
/*-----------------------------------------------------------------------------------------------*/

template<class Type, class Allocator>
MTypeWrap<Type, Allocator>::MContentType::MContentType(typename MTypeWrap<Type, Allocator>::const_reference_type val) :
MRefImpl<MAllocator<MPoolAllocator<MContentType>>>(),
mVal(val)
{
}

template<class Type, class Allocator>
MTypeWrap<Type, Allocator>::MContentType::MContentType(const typename MTypeWrap<Type, Allocator>::MContentType& other) :
MRefImpl<MAllocator<MPoolAllocator<MContentType>>>(other),
mVal(other.mVal)
{
}

template<class Type, class Allocator>
typename MTypeWrap<Type, Allocator>::MContentType& MTypeWrap<Type, Allocator>::MContentType::operator=(const typename MTypeWrap<Type, Allocator>::MContentType& other)
{
	MRefImpl<MAllocator<MPoolAllocator<MContentType>>>::operator=(other);
	mVal = other.mVal;
	return *this;
}

template<class Type, class Allocator>
typename MTypeWrap<Type, Allocator>::MContentType& MTypeWrap<Type, Allocator>::MContentType::operator =(typename MTypeWrap<Type, Allocator>::const_reference_type val){
	MRefImpl<MAllocator<MPoolAllocator<MContentType>>>::operator=(*this);
	mVal = val;
	return *this;
}

template<class Type, class Allocator>
MTypeWrap<Type, Allocator>::MContentType::operator Type(){
	return mVal;
}

template<class Type, class Allocator>
typename MTypeWrap<Type, Allocator>::reference_type MTypeWrap<Type, Allocator>::MContentType::toType(){
	return mVal;
}

template<class Type, class Allocator>
typename MTypeWrap<Type, Allocator>::const_reference_type MTypeWrap<Type, Allocator>::MContentType::toType() const{
	return mVal;
}


template<class Type, class Allocator>
const std::type_info & MTypeWrap<Type, Allocator>::MContentType::type() const{
	return typeid(mVal);
}


//---------------------------华丽的分割线-----------------------------------------------------------//

template<class Type, class Allocator>
MTypeWrap<Type, Allocator>::MTypeWrap(typename MTypeWrap<Type, Allocator>::const_reference_type val) :
mVal(new MTypeWrap<Type, Allocator>::MContentType(val))
{
}

template<class Type, class Allocator>
MTypeWrap<Type, Allocator>& MTypeWrap<Type, Allocator>::operator =(typename MTypeWrap<Type, Allocator>::const_reference_type val){
	mVal = new MTypeWrap<Type, Allocator>::MContentType(val);
	return *this;
}

template<class Type, class Allocator>
MTypeWrap<Type, Allocator>::operator Type(){
	return *mVal;
}

template<class Type, class Allocator>
typename MTypeWrap<Type, Allocator>::reference_type MTypeWrap<Type, Allocator>::toType(){
	if (mVal->IsShared() && mVal->Count() > 1){
		mVal = new MTypeWrap<Type, Allocator>::MContentType(*mVal);
	}
	return mVal->toType();
}

template<class Type, class Allocator>
typename MTypeWrap<Type, Allocator>::const_reference_type MTypeWrap<Type, Allocator>::toType() const{
	return mVal->toType();
}

template<class Type, class Allocator>
typename MTypeWrap<Type, Allocator>::reference_type2 MTypeWrap<Type, Allocator>::reference_obj(){
	return mVal->mVal;
}

template<class Type, class Allocator>
const typename MTypeWrap<Type, Allocator>::reference_type2 MTypeWrap<Type, Allocator>::reference_obj() const{
	return mVal->mVal;
}

template<class Type, class Allocator>
MTypeWrap<Type, Allocator> MTypeWrap<Type, Allocator>::clone(){
	MTypeWrap<Type, Allocator> newobj;
	newobj.mVal = new MTypeWrap< Type, Allocator>::MContentType(*mVal);
	newobj.mVal->MarkUnshareable();
	return newobj;
}

template<class Type, class Allocator>
MTypeWrap<Type, Allocator> MTypeWrap<Type, Allocator>::clone() const{
	return const_cast<MTypeWrap<Type, Allocator>*>(this)->clone();
}

template<class Type, class Allocator>
void MTypeWrap<Type, Allocator>::lock(){
	if (mVal->IsShared()){
		mVal = new MTypeWrap<Type, Allocator>::MContentType(*mVal);
	}
	mVal->MarkUnshareable();
}

template<class Type, class Allocator>
void MTypeWrap<Type, Allocator>::unlock(){
	mVal->MarkShareable();
}

template<class Type, class Allocator>
bool MTypeWrap<Type, Allocator>::islock() const{
	return mVal->IsShared();
}

template<class Type, class Allocator>
const std::type_info & MTypeWrap<Type, Allocator>::type() const{
	return mVal->type();
}

template<class Type, class Allocator>
void* MTypeWrap<Type, Allocator>::Address() const{
	return &(mVal->mVal);
}

template<class Type, class Allocator>
void MTypeWrap<Type, Allocator>::print(const char* msg, std::ostream& os) const{
	os << msg << mVal->mVal << std::endl;
}

template<class Type, class Allocator>
std::ostream& operator<<(std::ostream& os, const MTypeWrap<Type, Allocator>& other){
	return os << other.mVal->mVal;
}

template<class Type, class Allocator>
std::istream& operator>>(std::istream& is, MTypeWrap<Type, Allocator>& other){
	is >> other.mVal->mVal;
	return is;
}


///
// 引入两个特化类
//
#include "MVectorWrap.h"
#include "MVectorWrapNoAllocator.h"
#include "MFastWrap.h"
#include "MMapWrap.h"
