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
#include <string>
#include <MCopyValue.h>
#include <MEmptyType.h>
#include <MParamWrap.h>
#include <MContainerAllocator.h>

template<class ,class >
class MVector;

template<class T,class A>
std::ostream& operator<<(std::ostream& os, const MVector<T,A>& vec);
template<class T, class A>
std::istream& operator>>(std::istream& is, MVector<T, A>& vec);
template<class T, class A>
MVector<T, A> operator+(const  MVector<T, A>& left, const  MVector<T, A>& right);
template<class T, class A>
MVector<T, A> operator-(const  MVector<T, A>& left, const  MVector<T, A>& right);
template<class T, class A>
bool operator==(const  MVector<T, A>& left, const  MVector<T, A>& right);
template<class T, class A>
bool operator!=(const  MVector<T, A>& left, const  MVector<T, A>& right);
template<class T, class A>
bool operator>(const  MVector<T, A>& left, const  MVector<T, A>& right);
template<class T, class A>
bool operator<(const  MVector<T, A>& left, const  MVector<T, A>& right);

template<class T, class A>
bool operator>=(const  MVector<T, A>& left, const  MVector<T, A>& right);
template<class T, class A>
bool operator<=(const  MVector<T, A>& left, const  MVector<T, A>& right);

//------------------------------------------------------------------------
//
// 下面类声明
//
//-------------------------------------------------------------------------
template<class T, class Allocator = std::allocator<T>>
class MVector{
public:
	typedef T* iterator;
	typedef std::random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef std::ptrdiff_t difference_type;
	typedef std::ptrdiff_t distance_type;
	typedef T* pointer;
	typedef const T* const_iterator;
	typedef T& reference;
	typedef const T& const_reference;

	//
	// 参数类型
	//
	typedef typename MConstParamWrape<T>::type const_reference_type;// 也许是引用,也许不是，比如标量类型
	typedef typename MParamWrape<T>::type reference_type;

public:
	//
	// 默认构造
	//
	MVector();
	~MVector();

	//
	// 复制构造
	//
	MVector(const MVector& other);
	MVector(MVector&& other);
	MVector(const std::initializer_list<T>& list);
	MVector(std::initializer_list<T>&& list);
	MVector(const_reference_type val, unsigned num);

	template<class U>
	MVector(const MVector<U, Allocator>& other);

	template<class U,class A,template<class,class> class C>
	MVector(const C<U, A>& other);

	template<class U,template<class> class C>
	MVector(const C<U>& other);

	template<class iter>
	MVector(iter First, iter Last);

	//
	// 幅值构造
	//
	MVector& operator=(const MVector& other);
	MVector& operator=(MVector&& other);
	MVector& operator=(const std::initializer_list<T>& list);

	template<class U>
	MVector& operator=(const MVector<U,Allocator>& other);

	template<class U, class A, template<class, class> class C>
	MVector& operator=(const C<U, A>& other);

	template<class U, template<class> class C>
	MVector& operator=(const C<U>& other);

	template<class iter>
	MVector& assign(iter First, iter Last);

	//
	// 隐式转换
	//
	template<class A,template<class,class> class C>
	operator C<T, A>() const;

	template<class A, template<class, class> class C>
	operator C<T, A>();

	template<template<class>class C>
	operator C<T>() const;

	template<template<class>class C>
	operator C<T>();

	template<class U,class A, template<class, class> class C>
	operator C<U, A>() const;

	template<class U, class A, template<class, class> class C>
	operator C<U, A>();

	template<class U,template<class>class C>
	operator C<U>() const;

	template<class U, template<class>class C>
	operator C<U>();

	//
	// 显示转换
	//
	template<typename A, template<class, class> class C>
	C<T, A>& toVector(C<T, A>& __c);

	template<typename A, template<class, class> class C>
	C<T, A>& toVector(C<T, A>& __c) const;

	template<typename U, typename A, template<class, class> class C>
	C<U, A>& toVector(C<U, A>& __c);

	template<typename U, typename A, template<class, class> class C>
	C<U, A>& toVector(C<U, A>& __c) const;

	template<class U,class A>
	MVector<U, A> ToOtherType();

	template<class U,class A>
	MVector<U,A> ToOtherType() const;

	template<template<class > class C>
	C<T>& ToOtherType(C<T>& out);

	template<template<class> class C>
	C<T>& ToOtherType(C<T>& out) const;

	template<class U ,template<class > class C>
	C<U>& ToOtherType(C<U>& out);

	template<class U,template<class> class C>
	C<U>& ToOtherType(C<U>& out) const;
	

	//
	// 追加元素
	//
	MVector& push_back(const_reference_type val);
	MVector& operator<<(const_reference_type val);
	MVector& append(const_reference_type val);

	//
	// 泛型追加
	//
	template<class U>
	MVector& push_back(U val);

	template<class U>
	MVector& operator<<(U val);

	template<class U>
	MVector& append(U val);

	//
	// 效率低下
	//
	MVector& push_front(const_reference_type val);

	//
	// 操作整个系列
	//
	MVector& operator+=(const_reference_type val);
	MVector& operator-=(const_reference_type val);
	MVector& operator*=(const_reference_type val);
	MVector& operator/=(const_reference_type val);

	//
	// 打印
	//
	void print(const char* msg = "",std::ostream& os = std::cout);

	template<class U,class A>
	friend std::ostream& operator<<(std::ostream& os, const MVector<U,A>& vec);
	template<class U, class A>
	friend std::istream& operator>>(std::istream& is, MVector<U, A>& vec);
	template<class U, class A>
	friend MVector<U, A> operator+(const MVector<U, A>& left, const MVector<U, A>& right);
	template<class U, class A>
	friend MVector<U, A> operator-(const MVector<U, A>& left, const MVector<U, A>& right);
	template<class U, class A>
	friend bool operator==(const MVector<U, A>& left, const MVector<U, A>& right);
	template<class U, class A>
	friend bool operator!=(const MVector<U, A>& left, const MVector<U, A>& right);
	template<class U, class A>
	friend bool operator>(const MVector<U, A>& left, const MVector<U, A>& right);
	template<class U, class A>
	friend bool operator<(const MVector<U, A>& left, const MVector<U, A>& right);

	template<class T, class A>
	friend bool operator>=(const  MVector<T, A>& left, const  MVector<T, A>& right);
	template<class T, class A>
	friend bool operator<=(const  MVector<T, A>& left, const  MVector<T, A>& right);

	//
	// 直接追加一个数组
	//
	MVector& push_back(const MVector& other);

	void resize(std::size_t size);
	void reserve(std::size_t size);

	std::size_t size() const;

	void clear();

	bool empty() const;

	void pop_back();
	void pop_front();

	reference back();
	const_reference back() const;

	reference front();
	const_reference front() const;

	reference at(std::size_t index);
	const_reference at(std::size_t index) const;

	reference operator[](std::size_t index);
	const_reference operator[](std::size_t index) const;

	reference operator()(std::size_t index);
	const_reference operator()(std::size_t index) const;

	//
	// 迭代器
	//
	iterator begin();
	const_iterator begin() const;

	iterator end();
	const_iterator end() const;

	iterator last();
	const_iterator last() const;

	//
	// 插入
	//
	void insert(iterator pos, const_reference_type val);
	void insert_index(unsigned index, const_reference_type val);



	//
	// 查找 第一项
	//
	template<class cmp = std::equal_to<T>>
	iterator find(const_reference_type val, cmp prd = cmp());

	template<class cmp = std::equal_to<T>>
	const_iterator find(const_reference_type val, cmp prd = cmp()) const;

	//
	// 返回索引
	// 如果没有发现返回
	//
	template<class cmp = std::equal_to<T>>
	std::size_t find_index(const_reference_type val, cmp prd = cmp()) const;

	//
	// 给 定一系列数据返回此数据的索引位置
	//
	template<class cmp = std::equal_to<T>>
	MVector<std::size_t> find_index(const MVector<T>& other, cmp prd = cmp());

	//
	// 查看有多少个值
	//
	template<class cmp = std::equal_to<T>>
	std::size_t count(const_reference_type val, cmp prd = cmp()) const;



	//
	// 查找 最后一项
	//
	template<class cmp = std::equal_to<T>>
	iterator find_last(const_reference_type val, cmp prd = cmp());

	template<class cmp = std::equal_to<T>>
	const_iterator find_last(const_reference_type val, cmp prd = cmp()) const;

	//
	// 查找最靠近的指定值的值
	//
	bool near_search(const_reference_type val,T& out);
	bool near_search(const_reference_type val,T& out) const;

	//
	// 排序
	//
	template<class cmp = std::less<T>>
	void sort(cmp prd = cmp());

	template<class cmp = std::less<T>>
	void sort(iterator First, iterator Last, cmp prd = cmp());

	//
	// 删除
	//
        unsigned erase(iterator pos);
	void erase(iterator First, iterator Last);
        unsigned erase_index(unsigned index);

	//
	// 是否包含
	//
	bool contains(const_reference_type val) const;

	//
	//  移除某个值
	//
	template<class cmp = std::equal_to<T>>
	bool remove(const_reference_type val, cmp prd = cmp());
	
	//
	// 操作系列
	//
	template<class TransFun>
	void transform(TransFun fun);

	//
	// 判断两个序列是否相等
	//
	template<class U,template<class> class C, class cmp = std::equal_to<T>>
	bool equal(const C<U>& other, cmp __Prd = cmp()) const;

	template<class U,class A, template<class,class> class C, class cmp = std::equal_to<T>>
	bool equal(const C<U,A>& other, cmp __Prd = cmp()) const;

	//
	// 获取一个交集
	//
	template<template<class> class C, class cmp = std::equal_to<T>>
	MVector intersection(const C<T>& other, cmp prd = cmp()) const;

	template<class A, template<class, class> class C, class cmp = std::equal_to<T>>
	MVector intersection(const C<T, A>& other, cmp prd = cmp()) const;

	//
	// 检查是否包含
	//
	template<template<class> class C, class cmp = std::equal_to<T>>
	bool include(const C<T>& other, cmp prd = cmp()) const;

	template<class A, template<class, class> class C, class cmp = std::equal_to<T>>
	bool include(const C<T, A>& other, cmp prd = cmp()) const;

	//
	// 交换两个系列
	//
	void swap(MVector& other);

	//
	// 获取最大最小
	//
	T max_value() const;
	T min_value() const;

	//
	// 文件流操作
	//
	void WriteToFile(const std::string& fileName) const;
	bool ReadFromFile(const std::string& fileName);
	__int64 WriteBinaryToFile(const std::string& fileName) const;
	bool ReadBinaryFile(const std::string& fileName, __int64 offset = 0ll);
	__int64 WriteToStream(std::ostream& os, bool isbinary = true) const;
	void ReadFromStream(std::istream& is,bool isbinary = true);

	//
	// 保留HArry的接口
	//
	void RemoveValue(const_reference_type val);
	void Insert(std::size_t index,const_reference_type val);
	std::size_t GetSize();
	std::size_t IndexOf(const_reference_type val);
	const_reference_type GetAt(std::size_t index) const;
	reference_type GetAt(std::size_t index);
private:
	void __resize(std::size_t num);
	void __append(const_reference_type val);
	Allocator	mAlloc; // 分配器
	T*			mData{ nullptr };
	std::size_t mSize{ 0 };  // 当前大小
	std::size_t mAvailabeNum{ 0 }; // 当前可用内存
};

/*------------------------------------------------------------------------------
*
*
*
*   类实现
*
*
*
*--------------------------------------------------------------------------------*/

//
// 默认构造
//
template<class T, class Allocator>
MVector<T, Allocator>::MVector(){
}

template<class T, class Allocator>
MVector<T, Allocator>::~MVector(){
	if (mAvailabeNum > 0){
		for (int i = 0; i < mSize; ++i){
			mData[i].~T();
		}
		mAlloc.deallocate(mData,mAvailabeNum);
	}
}

//
// 复制构造
//
template<class T, class Allocator>
MVector<T, Allocator>::MVector(const MVector<T, Allocator>& other){
	if (!other.empty()){
		__resize(other.size());
		memcpy(mData, other.mData, other.size()*sizeof(T));
		mSize = other.size();
	}
}

template<class T, class Allocator>
MVector<T, Allocator>::MVector(MVector<T, Allocator>&& other){
	if (!other.empty()){
		__resize(other.size());
		memcpy(mData, other.mData, other.size()*sizeof(T));
		mSize = other.size();
		other.clear();
	}
}

template<class T, class Allocator>
MVector<T, Allocator>::MVector(const std::initializer_list<T>& list){
	if (list.size() != 0){
		__resize(list.size());
		memcpy(mData, &(*(list.begin())), list.size()*sizeof(T));
		mSize = list.size();
	}
}

template<class T, class Allocator>
MVector<T, Allocator>::MVector(std::initializer_list<T>&& list){
	if (list.size() != 0){
		__resize(list.size());
		memcpy(mData, &(*(list.begin())), list.size()*sizeof(T));
		mSize = list.size();
	}
}

template<class T, class Allocator>
MVector<T, Allocator>::MVector(const_reference_type val, unsigned num){
	__resize(num);
	for (int i = 0; i < num; ++i){
		mData[i] = val;
	}
	mSize = num;
}

template<class T, class Allocator>
template<class U>
MVector<T, Allocator>::MVector(const MVector<U, Allocator>& other){
	if (!other.empty()){
		__resize(other.size());
		for (int i = 0; i < other.size(); ++i){
			T res;
			mj::copyvalue(res, other.at(i));
			new ((void*)(&mData[i])) T(res);
		}
		mSize = other.size();
	}
}

template<class T, class Allocator>
template<class U, class A, template<class, class> class C>
MVector<T, Allocator>::MVector(const C<U, A>& other){
	if (!other.empty()){
		__resize(other.size());
		for (int i = 0; i < other.size(); ++i){
			T res;
			mj::copyvalue(res, other.at(i));
			new ((void*)(&mData[i])) T(res);
		}
		mSize = other.size();
	}
}

template<class T, class Allocator>
template<class U, template<class> class C>
MVector<T, Allocator>::MVector(const C<U>& other){
	if (!other.empty()){
		__resize(other.size());
		for (int i = 0; i < other.size(); ++i){
			T res;
			mj::copyvalue(res, other.at(i));
			new ((void*)(&mData[i])) T(res);
		}
		mSize = other.size();
	}
}


template<class T, class Allocator>
template<class iter>
MVector<T, Allocator>::MVector(iter First, iter Last){
	int num = std::distance(First, Last);
	if (num > 0){
		__resize(num);
		while (First != Last){
			T res;
			mj::copyvalue(res, *First);
			new ((void*)(&mData[mSize])) T(res);
			++mSize;
			++First;
		}
	}
}

//
// 幅值构造
//
template<class T, class Allocator>
MVector<T, Allocator>& MVector<T, Allocator>::operator=(const MVector<T, Allocator>& other){
	if (other.empty() || this == &other)
		return *this;
	clear();
	__resize(other.size());
	memcpy(mData, other.mData, sizeof(T)*other.size());
	mSize = other.size();
	return *this;
}

template<class T, class Allocator>
MVector<T, Allocator>& MVector<T, Allocator>::operator=(MVector<T, Allocator>&& other){
	clear();
	mData = other.mData;
	mSize = other.size();
	mAvailabeNum = other.mAvailabeNum;
	mAlloc   = other.mAlloc;

	other.mData = nullptr;
	other.mSize = 0;
	other.mAvailabeNum = 0;

	return *this;
}

template<class T, class Allocator>
MVector<T, Allocator>& MVector<T, Allocator>::operator=(const std::initializer_list<T>& list){
	clear();
	__resize(list.size());
	int index = 0;
	for (auto& v : list){
		::new((void*)(&mData[index])) T(v);
		++index;
	}
	mSize = list.size();
	return *this;
}

template<class T, class Allocator>
template<class U>
MVector<T, Allocator>& MVector<T, Allocator>::operator=(const MVector<U, Allocator>& other){
	clear();
	if (mAvailabeNum < other.size())
		__resize(other.size());
	for (int i = 0; i < mother.size()Size; ++i){
		value_type val;
		mj::copyvalue(val, other[i]);
		::new ((void*)(&mData[i])) T(val);
	}
	mSize = other.size();
	return *this;
}

template<class T, class Allocator>
template<class U, class A, template<class, class> class C>
MVector<T, Allocator>& MVector<T, Allocator>::operator=(const C<U, A>& other){
	clear();
	if (mAvailabeNum < other.size())
		__resize(other.size());
	for (int i = 0; i < other.size(); ++i){
		value_type val;
		mj::copyvalue(val, other[i]);
		::new ((void*)(&mData[i])) T(val);
	}
	mSize = other.size();
	return *this;
}

template<class T, class Allocator>
template<class U, template<class> class C>
MVector<T, Allocator>& MVector<T, Allocator>::operator=(const C<U>& other){
	clear();
	if (mAvailabeNum < other.size())
		__resize(other.size());
	for (int i = 0; i < other.size(); ++i){
		value_type val;
		mj::copyvalue(val, other[i]);
		::new ((void*)(&mData[i])) T(val);
	}
	mSize = other.size();
	return *this;
}

template<class T, class Allocator>
template<class iter>
MVector<T, Allocator>& MVector<T, Allocator>::assign(iter First, iter Last){
	clear();
	int num = std::distance(First, Last);
	if (mAvailabeNum < num)
		__resize(num);
	int i = 0;
	while (First != Last){
		value_type val;
		mj::copyvalue(val, *First);
		::new ((void*)(&mData[i])) T(val);
		++First;
	}
	mSize = num;
	return *this;
}

//
// 隐式转换
//
template<class T, class Allocator>
template<class A, template<class, class> class C>
MVector<T, Allocator>::operator C<T, A>() const{
	C<T, A> result;
	for (int i = 0; i < mSize; ++i){
		result.push_back(mData[i]);
	}
	return result;
}

template<class T, class Allocator>
template<class A, template<class, class> class C>
MVector<T, Allocator>::operator C<T, A>(){
	C<T, A> result;
	for (int i = 0; i < mSize; ++i){
		result.push_back(mData[i]);
	}
	return result;
}


template<class T, class Allocator>
template<template<class>class C>
MVector<T, Allocator>::operator C<T>() const{
	C<T> result;
	for (int i = 0; i < mSize; ++i){
		result.push_back(mData[i]);
	}
	return result;
}

template<class T, class Allocator>
template<template<class>class C>
MVector<T, Allocator>::operator C<T>(){
	C<T> result;
	for (int i = 0; i < mSize; ++i){
		result.push_back(mData[i]);
	}
	return result;
}


template<class T, class Allocator>
template<class U, class A, template<class, class> class C>
MVector<T, Allocator>::operator C<U, A>() const{
	C<U, A> result;
	for (int i = 0; i < mSize; ++i){
		U val;
		mj::copyvalue(val, mData[i]);
		result.push_back(val);
	}
	return result;
}


template<class T, class Allocator>
template<class U, class A, template<class, class> class C>
MVector<T, Allocator>::operator C<U, A>(){
	C<U, A> result;
	for (int i = 0; i < mSize; ++i){
		U val;
		mj::copyvalue(val, mData[i]);
		result.push_back(val);
	}
	return result;
}

template<class T, class Allocator>
template<class U, template<class>class C>
MVector<T, Allocator>::operator C<U>() const{
	C<U> result;
	for (int i = 0; i < mSize; ++i){
		U val;
		mj::copyvalue(val, mData[i]);
		result.push_back(val);
	}
	return result;
}

template<class T, class Allocator>
template<class U, template<class>class C>
MVector<T, Allocator>::operator C<U>(){
	C<U> result;
	for (int i = 0; i < mSize; ++i){
		U val;
		mj::copyvalue(val, mData[i]);
		result.push_back(val);
	}
	return result;
}

//
// 显示转换
//
template<class T, class Allocator>
template<typename A, template<class, class> class C>
C<T, A>& MVector<T, Allocator>::toVector(C<T, A>& __c){
	__c.clear();
	for (int i = 0; i < mSize; ++i){
		__c.push_back(mData[i]);
	}
	return __c;	
}

template<class T, class Allocator>
template<typename A, template<class, class> class C>
C<T, A>& MVector<T, Allocator>::toVector(C<T, A>& __c) const{
	return const_cast<MVector<T, Allocator>*>(this)->toVector(__c);
}

template<class T, class Allocator>
template<typename U, typename A, template<class, class> class C>
C<U, A>& MVector<T, Allocator>::toVector(C<U, A>& __c){
	__c.clear();
	for (int i = 0; i < mSize; ++i){
		U val;
		mj::copyvalue(val, mData[i]);
		__c.push_back(val);
	}
	return __c;
}

template<class T, class Allocator>
template<typename U, typename A, template<class, class> class C>
C<U, A>& MVector<T, Allocator>::toVector(C<U, A>& __c) const{
	__c.clear();
	for (int i = 0; i < mSize; ++i){
		U val;
		mj::copyvalue(val, mData[i]);
		__c.push_back(val);
	}
	return __c;
}

template<class T, class Allocator>
template<class U,class A>
MVector<U, A> MVector<T, Allocator>::ToOtherType(){
	MVector<U, A> out;
	out.reserve(mSize);
	for (int i = 0; i < mSize; ++i){
		U val;
		mj::copyvalue(val, mData[i]);
		out << val;
	}
	return out;
	
}

template<class T, class Allocator>
template<class U,class A>
MVector<U, A> MVector<T, Allocator>::ToOtherType() const{
	return const_cast<MVector<T, Allocator>*>(this)->ToOtherType();
}

template<class T, class Allocator>
template<template<class> class C>
C<T>& MVector<T, Allocator>::ToOtherType(C<T>& out){
	out.clear();
	for (int i = 0; i < mSize; ++i){
		out.push_back(mData[i]);
	}
	return out;
	
}

template<class T, class Allocator>
template<template<class> class C>
C<T>& MVector<T, Allocator>::ToOtherType(C<T>& out) const{
	return const_cast<MVector<T, Allocator>*>(this)->ToOtherType(out);
}

template<class T, class Allocator>
template<class U, template<class > class C>
C<U>& MVector<T, Allocator>::ToOtherType(C<U>& out){
	out.clear();
	for (int i = 0; i < mSize; ++i){
		U val;
		mj::copyvalue(val, mData[i]);
		out.push_back(val);
	}
	return out;
}

template<class T, class Allocator>
template<class U, template<class> class C>
C<U>& MVector<T, Allocator>::ToOtherType(C<U>& out) const{
	return const_cast<MVector<T, Allocator>*>(this)->ToOtherType(out);
}

//
// 追加元素
//
template<class T, class Allocator>
MVector<T, Allocator>& MVector<T, Allocator>::push_back(typename MVector<T, Allocator>::const_reference_type val){
	__append(val);
	return *this;
}

template<class T, class Allocator>
MVector<T, Allocator>& MVector<T, Allocator>::operator<<(typename MVector<T, Allocator>::const_reference_type val){
	__append(val);
	return *this;
}

template<class T, class Allocator>
MVector<T, Allocator>& MVector<T, Allocator>::append(typename MVector<T, Allocator>::const_reference_type val){
	__append(val);
	return *this;
}

//
// 泛型追加
//
template<class T, class Allocator>
template<class U>
MVector<T, Allocator>& MVector<T, Allocator>::push_back(U val){
	T __val;
	mj::copyvalue(__val, val);
	__append(__val);
	return *this;
}

template<class T, class Allocator>
template<class U>
MVector<T, Allocator>& MVector<T, Allocator>::operator<<(U val){
	T __val;
	mj::copyvalue(__val, val);
	__append(__val);
	return *this;
}

template<class T, class Allocator>
template<class U>
MVector<T, Allocator>& MVector<T, Allocator>::append(U val){
	T __val;
	mj::copyvalue(__val, val);
	__append(__val);
	return *this;
}

template<class T, class Allocator>
MVector<T, Allocator>& MVector<T, Allocator>::push_front(typename MVector<T, Allocator>::const_reference_type val){
	insert(begin(), val);
	return *this;
}


template<class T, class Allocator>
MVector<T, Allocator>& MVector<T, Allocator>::operator+=(typename MVector<T, Allocator>::const_reference_type val){
	for (auto& v : *this){
		v += val;
	}
	return *this;
}

template<class T, class Allocator>
MVector<T, Allocator>& MVector<T, Allocator>::operator-= (typename MVector<T, Allocator>::const_reference_type val){
	*this += -1 * val;
	return *this;
}

template<class T, class Allocator>
MVector<T, Allocator>& MVector<T, Allocator>::operator*=(typename MVector<T, Allocator>::const_reference_type val){
	for (auto& v : *this){
		v *= val;
	}
	return *this;
}

template<class T, class Allocator>
MVector<T, Allocator>& MVector<T, Allocator>::operator/=(typename MVector<T, Allocator>::const_reference_type val){
	for (auto& v : *this){
		v /= val;
	}
	return *this;
}

template<class T, class Allocator>
void MVector<T, Allocator>::print(const char* msg , std::ostream& os){
	os << msg  ;
	if (mSize == 0){
		os << "[ empty ]";
		return;
	}	
	os << "[";
	std::copy(mData, mData + mSize - 1, std::ostream_iterator<T>(os, ","));
	os << mData[mSize-1]<<"]";
	os << std::endl;
}


//
// 友元函数
//
template<class T, class Allocator>
std::ostream& operator<<(std::ostream& os, const MVector<T, Allocator>& vec){
	if (vec.empty())
		return os;
	os << "MVector (";
	std::copy(vec.begin(), (vec.end() - 1), std::ostream_iterator<T>(os, ","));
	os << vec.back() << ")";
	return os;
}

template<class T, class Allocator>
std::istream& operator>>(std::istream& is, MVector<T, Allocator>& vec){
	std::copy((std::istreambuf_iterator<T>(is)),
		std::istreambuf_iterator<T>(),
		vec);
	return is;
}

template<class T, class Allocator>
MVector<T, Allocator> operator+(const MVector<T, Allocator>& left, const MVector<T, Allocator>& right)
{
	MVector<T, Allocator> result;
	int minnum = min(left.mSize, right.mSize);
	result.__resize(minnum);
	for (int i = 0; i < minnum; ++i){
		::new ((void*)(result.mData + i)) T(left.mData[i] + right.mData[i]);
	}
	result.mSize = minnum;
	return result;
}

template<class T, class Allocator>
MVector<T, Allocator> operator-(const MVector<T, Allocator>& left, const MVector<T, Allocator>& right)
{
	MVector<T, Allocator> result;
	int minnum = min(left.mSize, right.mSize);
	result.__resize(minnum);
	for (int i = 0; i < minnum; ++i){
		::new ((void*)(result.mData + i)) T(left.mData[i] - right.mData[i]);
	}
	result.mSize = minnum;
	return result;
}

template<class T, class Allocator>
bool operator==(const MVector<T, Allocator>& left, const MVector<T, Allocator>& right)
{
	if (left.mSize != right.mSize)
		return false;

	for (int i = 0; i<left.mSize; ++i){
		if (left.mData[i] != right.mData[i]){
			return false;
		}
	}
	return true;
}

template<class T, class Allocator>
bool operator!=(const MVector<T, Allocator>& left, const MVector<T, Allocator>& right)
{
	return !(left == right);
}

template<class T, class Allocator>
bool  operator>(const MVector<T, Allocator>& left, const MVector<T, Allocator>& right){
	if (left.mSize > right.mSize)
		return true;
	else if (left.mSize < right.mSize)
		return false;
	for (int i = 0; i<left.m_nSize; ++i){
		if (left.mData[i] <= right.mData[i]){
			return false;
		}
	}
	return true;
}

template<class T, class Allocator>
bool operator<(const MVector<T, Allocator>& left, const MVector<T, Allocator>& right){
	if (left.mSize < right.mSize)
		return true;
	else if (left.mSize > right.mSize)
		return false;
	for (int i = 0; i<left.mSize; ++i){
		if (left.mData[i] >= right.mData[i]){
			return false;
		}
	}
	return true;
}

template<class T, class Allocator>
bool operator>=(const MVector<T, Allocator>& left, const MVector<T, Allocator>& right)
{
	if (left.mSize > right.mSize)
		return true;
	else if (left.mSize < right.mSize)
		return false;
	for (int i = 0; i<left.m_nSize; ++i){
		if (left.mData[i] < right.mData[i]){
			return false;
		}
	}
	return true;
}

template<class T, class Allocator>
bool operator<=(const MVector<T, Allocator>& left, const MVector<T, Allocator>& right)
{
	if (left.mSize < right.mSize)
		return true;
	else if (left.mSize > right.mSize)
		return false;
	for (int i = 0; i<left.mSize; ++i){
		if (left.mData[i] > right.mData[i]){
			return false;
		}
	}
	return true;
}

//
// 直接追加一个数组
//
template<class T, class Allocator>
MVector<T, Allocator>& MVector<T, Allocator>::push_back(const MVector<T, Allocator>& other){
	if (mAvailabeNum < other.size() + mSize()){
		__resize(other.size() + mSize());
	}
	memcpy(mData + mSize, other.mData, other.size()*sizeof(T));
	return *this;
}

template<class T, class Allocator>
void MVector<T, Allocator>::resize(std::size_t size){
	if (size <= mSize){
		for (int i = size; i < mSize; ++i){
			mData[i].~T();
		}
		mSize = size;
	}
	else{
		__resize(size);
		for (int i = mSize; i < size; ++i){
			::new ((void*)(&mData[i])) T();
		}
		mSize = size ;
	}

}

template<class T, class Allocator>
void MVector<T, Allocator>::reserve(std::size_t size){
	__resize(size);
}

template<class T, class Allocator>
std::size_t MVector<T, Allocator>::size() const{
	return mSize;
}

template<class T, class Allocator>
void MVector<T, Allocator>::clear(){
	if (mSize == 0)
		return;
	memset(mData, 0, mSize*sizeof(T));
	mSize = 0;	
	if (mAvailabeNum > 4096){
		mAlloc.deallocate(mData, mAvailabeNum);
		mAvailabeNum = 0;
	}
}

template<class T, class Allocator>
bool MVector<T, Allocator>::empty() const{
	return mSize == 0 ? true : false;
}

template<class T, class Allocator>
void MVector<T, Allocator>::pop_back(){
	if (mSize == 0)
		return;
	mData[mSize - 1].~T();
	--mSize;
}

template<class T, class Allocator>
void MVector<T, Allocator>::pop_front(){
	if (mSize == 0)
		return;
	mData[0].~T();
	memmove(mData, mData + 1, sizeof(T)*(mSize - 1));
	--mSize;
}

template<class T, class Allocator>
typename MVector<T, Allocator>::reference MVector<T, Allocator>::back(){
	if (mSize == 0)
		throw std::out_of_range("MVector<T> out of range");
	return mData[mSize - 1];
	
}

template<class T, class Allocator>
typename MVector<T, Allocator>::const_reference MVector<T, Allocator>::back() const{
	return const_cast<MVector<T, Allocator>*>(this)->back();
}

template<class T, class Allocator>
typename MVector<T, Allocator>::reference MVector<T, Allocator>::front(){
	if (mSize == 0)
		throw std::out_of_range("MVector<T> out of range");
	return mData[0];
}

template<class T, class Allocator>
typename MVector<T, Allocator>::const_reference MVector<T, Allocator>::front() const{
	return const_cast<MVector<T, Allocator>*>(this)->front();
}

template<class T, class Allocator>
typename MVector<T, Allocator>::reference MVector<T, Allocator>::at(std::size_t index){
	if (index >= mSize)
		throw std::out_of_range("MVector<T> out of range");
	return mData[index];
}

template<class T, class Allocator>
typename MVector<T, Allocator>::const_reference MVector<T, Allocator>::at(std::size_t index) const{
	return const_cast<MVector<T, Allocator>*>(this)->at(index);
}

template<class T, class Allocator>
typename MVector<T, Allocator>::reference MVector<T, Allocator>::operator[](std::size_t index){
	return at(index);
}

template<class T, class Allocator>
typename MVector<T, Allocator>::const_reference MVector<T, Allocator>::operator[](std::size_t index) const{
	return at(index);
}

template<class T, class Allocator>
typename MVector<T, Allocator>::reference MVector<T, Allocator>::operator()(std::size_t index){
	return at(index);
}

template<class T, class Allocator>
typename MVector<T, Allocator>::const_reference MVector<T, Allocator>::operator()(std::size_t index) const{
	return at(index);
}

//
// 迭代器
//
template<class T, class Allocator>
typename MVector<T, Allocator>::iterator MVector<T, Allocator>::begin(){
	return mData;
}

template<class T, class Allocator>
typename MVector<T, Allocator>::const_iterator MVector<T, Allocator>::begin() const{
	return mData;
}

template<class T, class Allocator>
typename MVector<T, Allocator>::iterator MVector<T, Allocator>::end(){
	return mData + mSize;
}

template<class T, class Allocator>
typename MVector<T, Allocator>::const_iterator MVector<T, Allocator>::end() const{
	return mData + mSize;
}


template<class T, class Allocator>
typename MVector<T, Allocator>::iterator MVector<T, Allocator>::last(){
	return mData + mSize - 1;
}

template<class T, class Allocator>
typename MVector<T, Allocator>::const_iterator MVector<T, Allocator>::last() const{
	return mData + mSize - 1;
}

template<class T, class Allocator>
void MVector<T, Allocator>::insert(
	typename MVector<T, Allocator>::iterator pos, typename MVector<T, Allocator>::const_reference_type val)
{
	if (mSize + 1 > mAvailabeNum)
		__resize(mSize + 1 + 1.5*mSize);

	int num = pos - begin();
	memcpy(pos + 1, pos, sizeof(T)*(mSize - num));
	::new ((void*)(&mData[num])) T(val);
	++mSize;
}

template<class T, class Allocator>
void MVector<T, Allocator>::insert_index(unsigned index, typename MVector<T, Allocator>::const_reference_type val){
	insert(mData + index, val);
}

//
// 查找 第一项
//
template<class T, class Allocator>
template<class cmp>
typename MVector<T, Allocator>::iterator MVector<T, Allocator>::find(
	typename MVector<T, Allocator>::const_reference_type val, cmp prd)
{
	return std::find_if(begin(), end(), std::bind(prd, val,std::_Ph<1>()));
}

template<class T, class Allocator>
template<class cmp>
typename MVector<T, Allocator>::const_iterator MVector<T, Allocator>::find(
	typename MVector<T, Allocator>::const_reference_type val, cmp prd) const
{
	return std::find_if(begin(), end(), std::bind(prd, val, std::_Ph<1>()));
}


//
// 查找 最后一项
//
template<class T, class Allocator>
template<class cmp>
typename MVector<T, Allocator>::iterator MVector<T, Allocator>::find_last(
	typename MVector<T, Allocator>::const_reference_type val, cmp prd)
{
	auto it = find(val, prd);
	if (it == end())
		return it;

	auto it2 = return std::find_if(it, end(), std::bind(prd, val, std::_Ph<1>()));
	while (it2 != end()){
		it = it2;
		it2 = return std::find_if(it, end(), std::bind(prd, val, std::_Ph<1>()));
	}
	return it2;
	
}

template<class T, class Allocator>
template<class cmp>
typename MVector<T, Allocator>::const_iterator MVector<T, Allocator>::find_last(
	typename MVector<T, Allocator>::const_reference_type val, cmp prd) const
{
	return const_cast<MVector<T, Allocator>*>(this)->find_last(val, prd);
}

//
// 返回索引
// 如果没有发现返回
//
template<class T, class Allocator>
template<class cmp>
std::size_t MVector<T, Allocator>::find_index(typename MVector<T, Allocator>::const_reference_type val, cmp prd) const{
	auto it = find(val, prd);
	if (it == end())
		return -1;
	else
		return it - begin();
}


//
// 返回索引
// 如果没有发现返回
//
template<class T, class Allocator>
template<class cmp>
MVector<std::size_t> MVector<T, Allocator>::find_index(const MVector<T>& other, cmp prd){
	MVector<std::size_t> __result;
	if (other.empty())
		return __result;

	T First = other[0];
	T Last = other.back();
	std::size_t __size = other.size();
	auto beg = find(First, prd);
	auto it = find(Last, prd);
	if (beg == end() || it == end()){
		return __result;
	}

	int num = it - beg + 1;

	if (num == __size){
		//
		// 最好的情况
		//
		num = beg - begin();
		for (int i = 0; i < __size; ++i){
			__result << (num + i);
		}
		return __result;
	}
	else if (num < __size){
		//
		// 数据错误情况
		//
		return __result;
	}
	else{
		//
		// 最坏的情况
		//
		std::size_t index = 0;
		for (auto& d : other){
			auto in = find_index(d, prd);
			if (in == -1){
				in = index;
			}
			else{
				index = in;
			}
			__result << in;
		}
		return __result;
	}
}

//
// 最 临近查找
//
template<class T, class Allocator>
bool MVector<T, Allocator>::near_search(typename MVector<T, Allocator>::const_reference_type val,T& out){
	if (this->empty()){
		return false;
	}
	MVector<T> temp = *this;
	temp.sort();
	if (val < temp.front()){
		out = temp[0];
		return true;
	}

	if (val >= temp.back()){
		out = temp.back();
		return true;
	}

	int midindex = temp.size() / 2;
	while (true){
		if (temp.empty()){
			return false;
		}
		if (temp.size() == 1){
			out = temp[0];
			return true;
		}
		midindex = temp.size() / 2;
		if (temp.size() % 2 == 0){
			midindex -= 1;
		}
		if (temp[midindex] < val && temp[midindex + 1] < val){
			temp.erase(temp.begin(), temp.begin() + midindex);
		}
		else if (temp[midindex] <= val && temp[midindex + 1] >= val){
			double diff = val - temp[midindex];
			double diff2 = temp[midindex + 1] - val;
			if (diff > diff2){
				out = temp[midindex + 1];
				return true;
			}
			else{
				out = temp[midindex];
				return true;
			}
		}
		else if (temp[midindex] > val ){
			temp.erase(temp.begin() + midindex + 1, temp.end());
		}
	}
	return false;
}

template<class T, class Allocator>
bool MVector<T, Allocator>::near_search(typename MVector<T, Allocator>::const_reference_type val, T& out) const{
	return const_cast<MVector<T, Allocator>*>(this)->near_search(val, out);
}

//
// 查看有多少个值
//
template<class T, class Allocator>
template<class cmp>
std::size_t MVector<T, Allocator>::count(typename MVector<T, Allocator>::const_reference_type val, cmp prd) const{
	return std::count_if(begin(), end(), std::bind(prd, val, std::_Ph<1>()));
}

//
// 排序
//
template<class T, class Allocator>
template<class cmp>
void MVector<T, Allocator>::sort(cmp prd){
	std::sort(begin(), end(), prd);
}

template<class T, class Allocator>
template<class cmp >
void MVector<T, Allocator>::sort(typename MVector<T, Allocator>::iterator First, 
	typename MVector<T, Allocator>::iterator Last, cmp prd)
{
	std::sort(First, Last, prd);
}

//
// 删除
//
template<class T, class Allocator>
unsigned MVector<T, Allocator>::erase(typename MVector<T, Allocator>::iterator pos){
	int num = pos - mData;
	mData[num].~T();
	if (mSize - num - 1 > 0){
		memcpy(mData + num, mData + num + 1, (mSize - num - 1)*sizeof(T));
	}
	--mSize;
     return num;
}

template<class T, class Allocator>
void MVector<T, Allocator>::erase(typename MVector<T, Allocator>::iterator First, 
	typename MVector<T, Allocator>::iterator Last)
{
	int num = Last - First;
	int beg = First - begin();
	int __end = end() - Last;
	for (int i = beg; i < beg + num; ++i){
		mData[i].~T();
	}
	memcpy(First, Last, sizeof(T)*__end);
	mSize -= num;
}

template<class T, class Allocator>
unsigned MVector<T, Allocator>::erase_index(unsigned index){
	if (index < 0 || index >= size())
                return -1;
        return erase(begin() + index);
}
//
// 是否包含
//
template<class T, class Allocator>
bool MVector<T, Allocator>::contains(typename MVector<T, Allocator>::const_reference_type val) const{
	if (find(val) == end())
		return false;
	else
		return true;
}

//
// 获取一个交集
//
template<class T, class Allocator>
template<template<class> class C, class cmp>
MVector<T, Allocator> MVector<T, Allocator>::intersection(const C<T>& other, cmp prd) const
{
	MVector<T, Allocator> out;
	std::set_intersection(this->begin(), this->end(),
		other.begin(), other.end(),
		std::back_inserter(out));
	return out;
}

template<class T, class Allocator>
template<class A, template<class, class> class C, class cmp>
MVector<T, Allocator> MVector<T, Allocator>::intersection(const C<T, A>& other, cmp prd) const
{
	MVector<T, Allocator> out;
	std::set_intersection(this->begin(), this->end(),
		other.begin(), other.end(),
		std::back_inserter(out));
	return out;
}

//
// 检查是否包含
//
template<class T, class Allocator>
template<template<class> class C, class cmp>
bool MVector<T, Allocator>::include(const C<T>& other, cmp prd) const
{
	return std::includes(this->begin(), this->end(), other.begin(), other.end());
}

template<class T, class Allocator>
template<class A, template<class, class> class C, class cmp>
bool MVector<T, Allocator>::include(const C<T, A>& other, cmp prd) const
{
	return std::includes(this->begin(), this->end(), other.begin(), other.end());
}

//
//  移除某个值
//
template<class T, class Allocator>
template<class cmp>
bool MVector<T, Allocator>::remove(typename MVector<T, Allocator>::const_reference_type val, cmp prd){
	if (find(val, prd) == end())
		return false;
	auto it = std::remove_if(begin(), end(), std::bind(prd, val, std::_Ph<1>()));
	erase(it, end());
	return true;
}

//
// 操作系列
//
template<class T, class Allocator>
template<class TransFun>
void MVector<T, Allocator>::transform(TransFun fun){
	for (auto& v : *this){
		v = fun(v);
	}
}

//
// 判断两个序列是否相等
//
template<class T, class Allocator>
template<class U, template<class> class C, class cmp>
bool MVector<T, Allocator>::equal(const C<U>& other, cmp prd) const{
	if (mSize != other.size())
		return false;
	for (int i = 0; i < mSize; ++i){
		if (!prd(mData[i], other[i]))
			return false;
	}
	return true;
}

template<class T, class Allocator>
template<class U, class A, template<class, class> class C, class cmp>
bool MVector<T, Allocator>::equal(const C<U, A>& other, cmp prd) const{
	if (mSize != other.size())
		return false;
	for (int i = 0; i < mSize; ++i){
		if (!prd(mData[i], other[i]))
			return false;
	}
	return true;
}


//
// 交换两个系列
//
template<class T, class Allocator>
void MVector<T, Allocator>::swap(MVector<T, Allocator>& other){
	std::swap(mData, other.mData);
	std::swap(mSize, other.mSize);
	std::swap(mAvailabeNum, other.mAvailabeNum);
	std::swap(mAlloc, other.mAlloc);
}

template<class T, class Allocator>
T MVector<T, Allocator>::max_value() const{
	if (empty())
		return T();
	return *(std::max_element(mData, mData + mSize));
}

template<class T, class Allocator>
T MVector<T, Allocator>::min_value() const{
	if (empty())
		return T();
	return *(std::min_element(mData, mData + mSize));
}

//
// 文件流操作
//
template<class T, class Allocator>
void MVector<T, Allocator>::WriteToFile(const std::string& fileName) const{
	std::ofstream outFile(fileName);
	if (outFile.fail())
		return;
	WriteToStream(outFile);
	outFile.close();
}

template<class T, class Allocator>
bool MVector<T, Allocator>::ReadFromFile(const std::string& fileName){
	clear();
	std::ifstream inFile(fileName);
	if (inFile.fail())
		return false;
	ReadFromStream(inFile);
	inFile.close();
	return true;
}

template<class T, class Allocator>
__int64 MVector<T, Allocator>::WriteBinaryToFile(const std::string& fileName) const{
	if (mData == nullptr || mSize == 0)
		return 0ll;
	std::ofstream outFile(fileName, std::ios::app | std::ios::binary);
	if (outFile.fail())
		return 0ll;
	int count = this->size();
	__int64 len = 0ll;
	outFile.write((char*)(&count), sizeof(count));
	len += sizeof(count);
	__int64 __len = sizeof(T)*size();
	outFile.write((const char*)(&mData[0]), __len);
	outFile.close();
	len += __len;
	return len;
}

template<class T, class Allocator>
bool MVector<T, Allocator>::ReadBinaryFile(const std::string& fileName, __int64 offset){
	clear();
	std::ifstream inFile(fileName, std::ios::in | std::ios::binary);
	if (inFile.fail()){
		return false;
	}
	inFile.seekg(offset, std::ios::beg);
	int count = 0;

	inFile.read((char*)(&count), sizeof(count));
	if (mAvailabeNum < count)
		__resize(count);
	mSize = count;
	__int64 lenth = count*sizeof(T);
	inFile.read((char*)mData, lenth);
	inFile.close();
	return true;
}

template<class T, class Allocator>
__int64 MVector<T, Allocator>::WriteToStream(std::ostream& os, bool isbinary) const{
	if (empty()){
		return 0ll;
	}	
	if (isbinary){
		__int64 len = sizeof(mSize);
		os.write((char*)(&mSize), sizeof(mSize));
		os.write((char*)(&mData[0]), sizeof(T)*mSize);
	}
	else{
		os << size() << std::endl;
		for (auto& v : *this){
			os << v << " ";
		}
		os << std::endl;
	}
}

template<class T, class Allocator>
void MVector<T, Allocator>::ReadFromStream(std::istream& is, bool isbinary){
	clear();
	if (is.eof())
		return;
	if (isbinary){
		is.read((char*)(&mSize), sizeof(mSize));
		__resize(mSize);
		is.read((char*)(&mData[0]), sizeof(T)*mSize);
	}
	else{
		int count = 0;
		is >> count;
		for (int i = 0; i<count; ++i){
			if (!is.eof()){
				T v;
				is >> v;
				push_back(v);
			}
			else{
				break;
			}
		}
	}
}

template<class T, class Allocator>
void MVector<T, Allocator>::__resize(std::size_t num){
	if (mAvailabeNum >= num){
		return;
	}
	T* __data = mAlloc.allocate(num);
	mAvailabeNum = num;
	if (mSize == 0 || mData == nullptr)
	{
		mData = __data;
		mSize = 0;
		return;
	}
	if (__data == mData)
		return;

	//
	// 如果是重新分配的内存需要将当前内存释放
	//
	memcpy(__data, mData, sizeof(T)*mSize);
	for (int i = 0; i < mSize; ++i){
		mData[i].~T();
	}
	mAlloc.deallocate(mData,mSize);
	mData = __data;
}

template<class T, class Allocator>
void MVector<T, Allocator>::__append(typename MVector<T, Allocator>::const_reference_type val){
	if (mAvailabeNum == 0)
		__resize(16);
	if (mSize < mAvailabeNum - 1){
		::new ((void*)(&mData[mSize])) T(val);
	}
	else{
		int num = 4096 * 255;
		if (mAvailabeNum < num){
			__resize(2 * mAvailabeNum);
		}
		else{
			__resize(mAvailabeNum + 2*num);
		}
		::new ((void*)(&mData[mSize])) T(val);
	}
	++mSize;
}

//
// 保留HArray的接口
//

template<class T, class Allocator>
void MVector<T, Allocator>::RemoveValue(typename MVector<T, Allocator>::const_reference_type val){
	remove(val);
}

template<class T, class Allocator>
void MVector<T, Allocator>::Insert(std::size_t index,typename MVector<T, Allocator>::const_reference_type val){
	insert(begin()+index,val);
}

template<class T, class Allocator>
std::size_t MVector<T, Allocator>::GetSize(){
	return mSize;
}

template<class T, class Allocator>
std::size_t MVector<T, Allocator>::IndexOf(typename MVector<T, Allocator>::const_reference_type val){
	auto it = find(val);
	return it - begin();
}

template<class T, class Allocator>
typename MVector<T, Allocator>::const_reference_type MVector<T, Allocator>::GetAt(std::size_t index) const{
	return at(index);
}

template<class T, class Allocator>
typename MVector<T, Allocator>::reference_type MVector<T, Allocator>::GetAt(std::size_t index){
	return at(index);
}
