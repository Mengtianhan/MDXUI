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

#include <vector>
#include <bitset>
#include "MThreadMode.h"
#include "MSingleton.h"
#include <deque>



template<int N = 1044480>
struct MChunck
{
	typedef unsigned char* buffer_type;
	typedef unsigned long size_type;

	MChunck();
	~MChunck();
	void InitBuffer(size_type size);
	void* Allocator(size_type counts = 1);
	bool Deallocator(void* ptr, size_type counts = 1);
	bool DealReleaseBuffer();
	bool Unused();
	size_type   mTypeSize{ 1 };
	buffer_type mBuffer;
	size_type mCounts{ N };
	size_type mAvailabelIndex{ 0 };   // 可用索引
	size_type mAvailabelCounts{ N };  // 可用buffer
	std::bitset<N> mStatusSet;
};

//
// 每一次都是分配固定大小内存
//
template<int N = 1044480>
struct MFixMemroy{
	typedef unsigned long size_type;
	size_type mTypeSize; // 类型大小
	size_type mCurrentIndex{ 0 };
	MChunck<N>*  pCurrentChunck{ nullptr };
	std::vector<MChunck<N>*>  mChunckVector;


	MFixMemroy(size_type typesize);
	~MFixMemroy();
	void* Allocator();
	bool Deallocator(void* ptr);
};

//
// 普通分配器
//
template<class Type, int N = 1044480, class ThreadMode = MSingleThreadMode>
class MPoolAllocator : public ThreadMode, public MSingleton<MPoolAllocator<Type, N,ThreadMode>>
{
public:
	enum{ value = sizeof(Type) };
	void* Allocator(std::size_t size);
	void Deallocator(void* p, std::size_t size);

	template<class OtherType>
	struct rebind
	{
		typedef MPoolAllocator<OtherType, N, ThreadMode> AllocatorType;
	};
private:
	std::vector<MFixMemroy<N>*> mPools;
	MFixMemroy<N>*   pAllocator{ nullptr };
	std::deque<void*> mFastCache;

	template<class CableLock>
	class UniquiLock{
	public:
		UniquiLock(CableLock* obj) :pObj(obj){
			pObj->Lock();
		}
		~UniquiLock(){
			pObj->UnLock();
		}
	private:
		CableLock*  pObj{ nullptr };
	};
};

//
// 容器专用分配器
//
template<class T>
class MContainerAllocator{
public:
	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef unsigned long size_type;
	typedef char* buffer_type;

	enum{ value = sizeof(T) };

	MContainerAllocator<T> select_on_container_copy_construction() const;
	pointer address(reference _Val);
	const_pointer address(const_reference _Val) const;
	MContainerAllocator();
	MContainerAllocator(const MContainerAllocator<T>&);
	MContainerAllocator(MContainerAllocator<T>&&);
	MContainerAllocator& operator=(const MContainerAllocator<T>&);
	MContainerAllocator& operator=(MContainerAllocator<T>&&);
	~MContainerAllocator();
	template<class _Other>
	MContainerAllocator(const MContainerAllocator<_Other>&);
	void deallocate(pointer _Ptr, size_type);
	pointer allocate(size_type _Count);
	pointer allocate(size_type _Count, const void *);
	void construct(T *_Ptr);
	void construct(T *_Ptr, const T& _Val);
	template<class U,class... Args>
	void construct(U *_Ptr, Args&&... _Args);
	template<class U>
	void destroy(U *_Ptr);
	unsigned long max_size() const;
private:
	T*				mBuffer{ nullptr };
	size_type		mCounts;
};

//
// 普通分配器
//
template<class Type, int N,class ThreadMode>
void* MPoolAllocator<Type, N,ThreadMode>::Allocator(std::size_t size){
	if (size > value)
		return ::operator new(size);

	MPoolAllocator<Type, N, ThreadMode>::UniquiLock<MPoolAllocator<Type, N,ThreadMode>> lock(this);
	if (!mFastCache.empty()){
		void* p = mFastCache.front();
		mFastCache.pop_front();
		return p;
	}
	if (mPools.empty()){
		MFixMemroy<N>* pAlloc = new  MFixMemroy<N>(value);
		mPools.push_back(pAlloc);
		pAllocator = pAlloc;
	}
	void* Ptr = nullptr;
	if (pAllocator ){
		Ptr = pAllocator->Allocator();
		if (Ptr){
			return Ptr;
		}
		
	}
	else{
		MFixMemroy<N>* pAlloc = new  MFixMemroy<N>(value);
		mPools.push_back(pAlloc);
		pAllocator = pAlloc;
		Ptr = pAllocator->Allocator();
		return Ptr;
	}
	return nullptr;
}

template<class Type, int N,class ThreadMode>
void  MPoolAllocator<Type, N,ThreadMode>::Deallocator(void* p, std::size_t size){
	if (size > value){
		::operator delete(p);
		return;
	}	
	MPoolAllocator<Type, N, ThreadMode>::UniquiLock<MPoolAllocator<Type, N, ThreadMode>> lock(this);
	if (mFastCache.size() < 1024){
		mFastCache.push_back(p);
		return;
	}
	static MFixMemroy<N>* pDeallocatorPtr{ nullptr };
	for (auto& m : mPools){
		if (pDeallocatorPtr && pDeallocatorPtr->Deallocator(p)){
			break;
		}
		if (m->Deallocator(p)){
			pDeallocatorPtr = m;
			break;
		}
	}
}

//=================
//
// 容器专用分配器
//
//=================
template<class T>
MContainerAllocator<T> MContainerAllocator<T>::select_on_container_copy_construction() const{
	return (*this);
}

template<class T>
typename MContainerAllocator<T>::pointer MContainerAllocator<T>::address(typename MContainerAllocator<T>::reference _Val){
	return &_Val;
}

template<class T>
typename MContainerAllocator<T>::const_pointer MContainerAllocator<T>::address(
	typename MContainerAllocator<T>::const_reference _Val) const
{
	return &_Val;
}

template<class T>
MContainerAllocator<T>::MContainerAllocator()
{	
}

template<class T>
MContainerAllocator<T>::MContainerAllocator(const MContainerAllocator<T>& other){
}

template<class T>
MContainerAllocator<T>::MContainerAllocator(MContainerAllocator<T>&& other){
}

template<class T>
MContainerAllocator<T>& MContainerAllocator<T>::operator=(const MContainerAllocator<T>& other){
    return *this;
}

template<class T>
MContainerAllocator<T>& MContainerAllocator<T>::operator=(MContainerAllocator<T>&& other){
    return *this;
}

template<class T>
MContainerAllocator<T>::~MContainerAllocator(){
}


template<class T>
void MContainerAllocator<T>::deallocate(typename MContainerAllocator<T>::pointer _Ptr, 
	typename MContainerAllocator<T>::size_type size)
{
	if (_Ptr == nullptr)
		return;
	if (_Ptr == mBuffer){
		mCounts = 0;
	}
	if (_Ptr != nullptr){
		free(_Ptr);
		_Ptr = nullptr;
	}
	if (mCounts == 0){
		mBuffer = nullptr;
	}
}

template<class T>
typename MContainerAllocator<T>::pointer MContainerAllocator<T>::allocate(
	typename MContainerAllocator<T>::size_type size)
{
	int fixnum = 256 * 256; // 4096
	if (mBuffer == nullptr){	
		if (mCounts < fixnum && size < fixnum){
			mBuffer = (T*)malloc(fixnum*value);
			mCounts = fixnum;
			if (mBuffer == nullptr){
				throw std::runtime_error("MContainerAllocator<T> bad alloc");
			}
			return mBuffer;
		}
		else{
			mCounts = size + fixnum;
			mBuffer = (T*)malloc(mCounts*value);
			if (mBuffer == nullptr){
				throw std::runtime_error("MContainerAllocator<T> bad alloc");
			}
			return mBuffer;
		}
	}
	else{
		if (size > mCounts){
			mCounts = size + fixnum;
			mBuffer = (T*)malloc(mCounts*value);
			if (mBuffer == nullptr){
				throw std::runtime_error("MContainerAllocator<T> bad alloc");
			}
			return mBuffer;
		}
		else{
			return  mBuffer;
		}
	}
}

template<class T>
typename MContainerAllocator<T>::pointer MContainerAllocator<T>::allocate(
	typename MContainerAllocator<T>::size_type _Count, const void *)
{
	return allocate(_Count);
}

template<class T>
void MContainerAllocator<T>::construct(T *_Ptr){
	::new(void*(_Ptr)) T();
}

template<class T>
void MContainerAllocator<T>::construct(T *_Ptr, const T& _Val){
	::new(void*(_Ptr)) T(_Val);
}

template<class T>
template<class U,class...Args>
void MContainerAllocator<T>::construct(U *_Ptr, Args&&... _Args){
	::new(void*(_Ptr)) U(std::forward<Args...>(_Args...));
}

template<class T>
template<class U>
void MContainerAllocator<T>::destroy(U *_Ptr){
	_Ptr->~U();
}

template<class T>
unsigned long MContainerAllocator<T>::max_size() const{
	return ((typename MContainerAllocator<T>::size_type)(-1)) / value;
}

//
//  MFixMemroy
//
template<int N>
MFixMemroy<N>::MFixMemroy(typename MFixMemroy<N>::size_type typesize) :
mTypeSize(typesize)
{
	mChunckVector.clear();
	pCurrentChunck = nullptr;
}

template<int N>
MFixMemroy<N>::~MFixMemroy(){
	for (auto& m : mChunckVector){
		if (m){
			delete m;
			m = nullptr;
		}
	}
	mChunckVector.clear();
}

template<int N>
void* MFixMemroy<N>::Allocator(){

	void* Ptr = nullptr;
	if (pCurrentChunck){
		Ptr = pCurrentChunck->Allocator();
		if (Ptr){
			return Ptr;
		}
	}

	if (Ptr == nullptr){
		auto beg = mChunckVector.begin();
		auto end = mChunckVector.end();
		do{
			if (beg == mChunckVector.end()){
				MChunck<N>* pChunck = new MChunck<N>();
				pChunck->InitBuffer(mTypeSize);
				pCurrentChunck = pChunck;
				mChunckVector.push_back(pChunck);
				break;
			}

			if ((*beg)->mAvailabelCounts > 0 && (*beg)->mAvailabelCounts <= N){
				pCurrentChunck = *beg;
				break;
			}
			++beg;
		} while (1);
		
	}
	
	Ptr = pCurrentChunck->Allocator();
	return Ptr;
}

template<int N>
bool MFixMemroy<N>::Deallocator(void* ptr){
	MChunck<N>* Ptr = nullptr;
	int index = 0;
	for (auto& m : mChunckVector){
		Ptr = m;
		if (Ptr == nullptr){
			++index;
			continue;
		}
		if (Ptr->Deallocator(ptr)){
			if (Ptr->mAvailabelCounts >= N){
				delete Ptr;
				mChunckVector.erase(mChunckVector.begin() + index);
			}
			return true;
		}
		++index;
	}
	return false;
}

//
// MChunck
//
template<int N>
MChunck<N>::MChunck(){

}

template<int N>
MChunck<N>::~MChunck(){
	if (mBuffer){
		delete[] mBuffer;
		mBuffer = nullptr;
	}
}

template<int N>
void MChunck<N>::InitBuffer(typename MChunck<N>::size_type size){
	mTypeSize = size;
	mBuffer = new unsigned char[N*size];
	mAvailabelIndex = 0;
	mAvailabelCounts = N;
	for (int i = 0; i < N; ++i){
		mStatusSet[i] = 1;
	}
}

template<int N>
void* MChunck<N>::Allocator(typename MChunck<N>::size_type counts){
	if (mAvailabelCounts > mCounts || mAvailabelCounts <=0)
		return nullptr;
	typename MChunck<N>::buffer_type pResult{ nullptr };
	if (mStatusSet[mAvailabelIndex] == 0){
		for (int i = 0; i < N; ++i){
			if (mStatusSet[i] == 1){
				mAvailabelIndex = i;
				break;
			}
		}
	}
	pResult = mBuffer + (mTypeSize*mAvailabelIndex);
	mStatusSet[mAvailabelIndex] = 0;
	++mAvailabelIndex;
	--mAvailabelCounts;
	return pResult;
}

template<int N>
bool MChunck<N>::Deallocator(void* ptr, typename MChunck<N>::size_type counts){
	if (ptr < mBuffer || ptr >= mBuffer + N*mTypeSize){
		return false;
	}
	typename MChunck<N>::buffer_type releasebuffer = static_cast<typename MChunck<N>::buffer_type>(ptr);
	typename MChunck<N>::size_type num = releasebuffer - mBuffer;
	if (num % mTypeSize != 0){
		return false;
	}
	mAvailabelIndex = num / mTypeSize;
	if (mAvailabelIndex >= N){
		std::cout << ptr << "\t" << (void*)(mBuffer) << "\t" << mAvailabelIndex <<"\t"<<N<< std::endl;
		return false;
	}
	mStatusSet[mAvailabelIndex] = 1;
	++mAvailabelCounts;
	return true;
}

template<int N>
bool MChunck<N>::Unused(){
	return mAvailabelCounts == mCounts;
}
