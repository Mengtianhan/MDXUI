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
#include "MEmptyType.h"

template<typename T>
class MRefWrap;
//
//  继承自该类的类可以实现写时复制技术
//
template<class Allocator = MEmptyType>
class MRefImpl :public Allocator{
public:
	friend class MRefWrap<MRefImpl<Allocator>>;
	void AddRefCount();
	void DecRefCount();
	void RemRefCount();
	void MarkUnshareable();
	void MarkShareable();
	bool IsShareable();
	bool IsShared();
	unsigned Count();
private:
	int   mCount;
	bool  bIsShared;
protected:
	MRefImpl();
	MRefImpl(const MRefImpl&);
	MRefImpl& operator=(const MRefImpl&);
	virtual ~MRefImpl(){}
};


//
// 管理引用计数
//
template<typename T>
class MRefWrap {
public:
	MRefWrap(T* t);
	MRefWrap(const MRefWrap& wrap);
	MRefWrap& operator=(const MRefWrap& p);
	~MRefWrap();
	T* operator->() const;
	T& operator*() const;
private:
	T* mDataPtr;
	void init();
};

/*-----------------------------------------------------------------------------------------------*/
/*   实现  
/*-----------------------------------------------------------------------------------------------*/
template<class Allocator>
MRefImpl<Allocator>::MRefImpl()
:mCount(0), bIsShared(true)
{
}

template<class Allocator>
MRefImpl<Allocator>::MRefImpl(const MRefImpl<Allocator>&)
:mCount(0), bIsShared(true)
{
}

template<class Allocator>
MRefImpl<Allocator>& MRefImpl<Allocator>::operator=(const MRefImpl<Allocator>&){
	return *this;
}

template<class Allocator>
void MRefImpl<Allocator>::AddRefCount(){
	++mCount;
}

template<class Allocator>
void MRefImpl<Allocator>::DecRefCount(){
	--mCount;
}

template<class Allocator>
void MRefImpl<Allocator>::RemRefCount(){
	if (--mCount == 0)
		delete this;
}

template<class Allocator>
void MRefImpl<Allocator>::MarkUnshareable(){
	bIsShared = false;
}

template<class Allocator>
void MRefImpl<Allocator>::MarkShareable(){
	bIsShared = true;
}

template<class Allocator>
bool MRefImpl<Allocator>::IsShareable(){
	return bIsShared;
}

template<class Allocator>
bool MRefImpl<Allocator>::IsShared(){
	return bIsShared && mCount > 1;
}

template<class Allocator>
unsigned MRefImpl<Allocator>::Count(){
	return mCount;
}

//
// 引用计数
//
template<class T>
MRefWrap<T>::MRefWrap(T* t) :mDataPtr(t){
	init();
}

template<class T>
MRefWrap<T>::MRefWrap(const MRefWrap<T>& wrap) : mDataPtr(wrap.mDataPtr){
	init();
}

template<class T>
MRefWrap<T>& MRefWrap<T>::operator=(const MRefWrap<T>& p){
	if (mDataPtr != p.mDataPtr){
		if (mDataPtr){
			mDataPtr->RemRefCount();
			mDataPtr = p.mDataPtr;
			init();
		}
	}
	return *this;
}

template<class T>
void MRefWrap<T>::init(){
	if (!mDataPtr){
		return;
	}
	if (!(mDataPtr->IsShareable())){
		mDataPtr = new T(*mDataPtr);
	}
	mDataPtr->AddRefCount();
}

template<class T>
MRefWrap<T>::~MRefWrap(){
	if (mDataPtr){
		mDataPtr->RemRefCount();
	}
}

template<class T>
T* MRefWrap<T>::operator->() const{
	return mDataPtr;
}

template<class T>
T& MRefWrap<T>::operator*() const{
	return *mDataPtr;
}