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
#include <iterator>
#include "MCopyValue.h"
#include "MEmptyType.h"

///
/// MList
///

template<class T, class Allocator = MEmptyType>
class MList{
public:
	//
	// 数据类型，双向链表
	//
	template<class U>
	struct MLink : public Allocator{
		MLink* pHeader{ nullptr };
		MLink* pNext{ nullptr };
		U  mValue;
	};
	typedef T value_type;

public:

	struct const_iterator{
		typedef std::bidirectional_iterator_tag  iterator_category;
		typedef __int32 difference_type;
		typedef T value_type;
		typedef T *pointer;
		typedef T &reference;

		MLink<T>* it;
		MLink<T>* EndFlagLink{ nullptr }; // 标记结束
		const_iterator() :it(nullptr){}
		const_iterator(MLink<T>* node) :it(node){}
		const_iterator(const const_iterator& other) :it(other.it), EndFlagLink(other.EndFlagLink){}
		inline const T &operator*() const { return it->mValue; }
		inline const T *operator->() const { return &it->mValue; }
		inline bool operator==(const const_iterator & other) const { return it == other.it; }
		inline bool operator!=(const const_iterator & other) const { return it != other.it; }
		inline bool operator<(const const_iterator& other) const { return it < other.it; }
		inline bool operator<=(const const_iterator& other) const { return it <= other.it; }
		inline bool operator>(const const_iterator& other) const { return it > other.it; }
		inline bool operator>=(const const_iterator& other) const { return it >= other.it; }
		inline const_iterator &operator++() {if (it->pNext){EndFlagLink = it;}it = it->pNext;return *this;}
		inline const_iterator operator++(int) { MLink<T> *n = it; it = it->pNext; if (it == nullptr){EndFlagLink = n;}return n;}
		inline const_iterator &operator--() {if (it == nullptr){it = EndFlagLink;}else{it = it->pHeader;}return *this;}
		inline const_iterator operator--(int) { MLink<T> *n = it; if (it == nullptr){it = EndFlagLink;}else{it = it->pHeader;}return n;}
	};

	struct iterator{
		typedef std::bidirectional_iterator_tag  iterator_category;
		typedef __int32 difference_type;
		typedef T value_type;
		typedef T *pointer;
		typedef T &reference;

		MLink<T>* it;
		MLink<T>* EndFlagLink{ nullptr }; // 标记结束
		iterator() :it(nullptr){}
		iterator(MLink<T>* node) :it(node){}
		iterator(const iterator& other) :it(other.it), EndFlagLink(other.EndFlagLink){}
		inline T &operator*() const { return it->mValue; }
		inline T *operator->() const { return &it->mValue; }
		inline bool operator==(const iterator & other) const { return it == other.it; }
		inline bool operator!=(const iterator & other) const { return it != other.it; }
		inline bool operator<(const iterator& other) const { return it < other.it; }
		inline bool operator<=(const iterator& other) const { return it <= other.it; }
		inline bool operator>(const iterator& other) const { return it > other.it; }
		inline bool operator>=(const iterator& other) const { return it >= other.it; }
		inline iterator &operator++() { if (it->pNext){ EndFlagLink = it; }it = it->pNext; return *this; }
		inline iterator operator++(int) { MLink<T> *n = it; it = it->pNext; if (it == nullptr){ EndFlagLink = n; }return n; }
		inline iterator &operator--() { if (it == nullptr){ it = EndFlagLink; } else{ it = it->pHeader; }return *this; }
		inline iterator operator--(int) { MLink<T> *n = it; if (it == nullptr){ it = EndFlagLink; } else{ it = it->pHeader; }return n; }
	};

private:
	//
	// 数据链表
	//
	MLink<T>* pLink{ nullptr };
	unsigned __int64 mLength{ 0ull };
	//
	// 查找
	//
	template<class U>
	MLink<T>* PrivateFind(const U& val){
		if (pLink == nullptr)
			return nullptr;

		MLink<T>* __TempLink = pLink;
		while (__TempLink->pHeader){
			__TempLink = __TempLink->pHeader;
		}

		T __val;
		mj::copyvalue(__val, val);
		if (__TempLink->mValue == __val){
			return pLink;
		}

		while (__TempLink->pNext){
			__TempLink = __TempLink->pNext;
			if (__TempLink->mValue == __val){
				return __TempLink;
			}
		}
		return nullptr;
	}

	//
	// 追加
	//
	template<class U>
	void PrivateAppend(const U& val){
		if (pLink == nullptr){
			pLink = new MLink<T>;
			mj::copyvalue(pLink->mValue, val);
			return;
		}

		while (pLink->pNext){
			pLink = pLink->pNext;
		}

		MLink<T>* __TempLink = nullptr;
		__TempLink = new MLink<T>;		 
		mj::copyvalue(__TempLink->mValue, val);
		__TempLink->pHeader = pLink;
		pLink->pNext = __TempLink;
		pLink = __TempLink;
	}

	//
	// 追加一个链表
	//
	void PrivateAppend(MLink<T>* val){
		if (val == nullptr)
			return;

		if (pLink == nullptr){
			pLink = val;
			return;
		}

		while (pLink->pNext){
			pLink = pLink->pNext;
		}

		if (val->pHeader){
			val->pHeader->pNext = nullptr;
		}

		val->pHeader = pLink;
		pLink->pNext = val;
	}

	void PrivateInit(T* beg, T* end){
		for (; beg != end; ++beg){
			PrivateAppend(*beg);
		}
	}

	void PrivateInit(const_iterator beg, const_iterator end){
		for (; beg != end; ++beg){
			PrivateAppend(*beg);
		}
	}

	void PrivateInit(iterator beg, iterator end){
		for (; beg != end; ++beg){
			PrivateAppend(*beg);
		}
	}

	template<class U>
	void PrivateInit(U* beg, U* end){
		for (; beg != end; ++beg){
			T val;
			mj::copyvalue(val, *beg);
			PrivateAppend(val);
		}
	}

	//
	// 前面添加
	//
	template<class U>
	void PrivateFrontAppend(const U& val){
		MLink<T>* __TempLink = new MLink<T>;
		mj::copyvalue(__TempLink->mValue, val);

		if (pLink == nullptr){
			pLink = __TempLink;
			return;
		}

		while (pLink->pHeader){
			pLink = pLink->pHeader;
		}
		pLink->pHeader = __TempLink;
		__TempLink->pNext = pLink;
	}

	//
	// 前面添加一个链表
	//
	void PrivateFrontAppend(MLink<T>* val){
		if (val == nullptr)
			return;

		val->pHeader = nullptr;

		if (pLink == nullptr){
			pLink = val;
			return;
		}

		while (val->pNext){
			val = val->pNext;
		}

		while (pLink->pHeader){
			pLink = pLink->pHeader;
		}

		val->pNext = pLink;
		pLink->pHeader = val;
	}

	//
	// 插入
	//
	template<class U>
	void PrivateInsert(MLink<T>* link, const U& val){
		if (link == nullptr){
			PrivateAppend(val);
			return;
		}

		MLink<T>* __TempLink = new MLink<T>;
		mj::copyvalue(__TempLink->mValue, val);

		MLink<T>* __TempLink2 = link->pNext;
		link->pNext = __TempLink;
		__TempLink->pHeader = link;
		__TempLink->pNext = __TempLink2;
		if (__TempLink2 != nullptr)
			__TempLink2->pHeader = __TempLink;
	}

	void PrivateInsert(MLink<T>* link, MLink<T>* val){
		if (val == nullptr){
			return;
		}
		if (link == nullptr){
			link = val;
			return;
		}
		MLink<T>* __TempLink = val;
		MLink<T>* __TempLink2 = link->pNext;
		link->pNext = __TempLink;
		__TempLink->pHeader = link;
		while (__TempLink->pNext){
			__TempLink = __TempLink->pNext;
		}
		__TempLink->pNext = __TempLink2;
		if (__TempLink2 != nullptr){
			__TempLink2->pHeader = __TempLink;
		}
	}

	//
	// 删除
	//
	template<class U>
	void PrivateErase(const U& val){
		MLink<T>* __TempLink = PrivateFind(val);
		if (__TempLink == nullptr)
			return;

		if (__TempLink->pHeader){
			__TempLink->pHeader->pNext = __TempLink->pNext;
		}
		if (__TempLink->pNext){
			__TempLink->pNext->pHeader = __TempLink->pHeader;
		}
		__TempLink->mValue.~T();
		delete __TempLink;
	}

	void PrivateErase(MLink<T>*& link){
		if (link == nullptr || pLink == nullptr){
			return;
		}
		MLink<T>* Header = link->pHeader;
		MLink<T>* Next = link->pNext;
		if (Header && Next){
			Header->pNext = Next;
			Next->pHeader = Header;
		}
		else if (Header){
			Header->pNext = Next;
		}
		else if (Next){
			Next->pHeader = Header;
		}
		link->mValue.~T();
		delete link;
		link = nullptr;
	}

	void PrivateErase(MLink<T>* beg, MLink<T>* end){
		if (beg == nullptr || pLink == nullptr)
			return;

		while (pLink->pHeader){
			pLink = pLink->pHeader;
			if (pLink == beg){
				break;
			}
		}

		while (pLink != beg){
			pLink = pLink->pNext;
			if (pLink == beg){
				break;
			}
		}

		if (pLink->pNext == nullptr){
			return;
		}

		while (pLink != end){
			pLink->mValue.~T();
			MLink<T>* __TempLink = pLink->pNext;
			delete pLink;
			pLink = __TempLink;
		}
	}

	//
	// 大小
	//
	unsigned __int64 PrivateSize(){
		unsigned __int64 __size = 0ull;

		if (pLink == nullptr)
			return __size;

		while (pLink->pNext){
			pLink = pLink->pNext;
		}

		MLink<T>* __TempLink = pLink;
		while (__TempLink->pHeader){
			++__size;
			__TempLink = __TempLink->pHeader;
		}
		return __size;
	}

	void PrivateClear(){
		if (pLink == nullptr){
			return;
		}
		while (pLink->pHeader){
			pLink = pLink->pHeader;
		}
		while (pLink){
			MLink<T>* Next = pLink->pNext;
			pLink->mValue.~T();
			delete pLink;
			pLink = Next;
		}
	}

	MLink<T>* PrivateHeader(){
		if (pLink == nullptr)
			return nullptr;
		MLink<T>* TemplLink = pLink;
		while (TemplLink->pHeader){
			TemplLink = TemplLink->pHeader;
		}
		return TemplLink;
	}

	void PrivateStream(std::ostream& os){
		MLink<T>* header = PrivateHeader();
		if (header == nullptr){
			return;
		}
		os << "MList:(";
		while (header){
			os << header->mValue << ",";
			header = header->pNext;
		}
		os << ")";
	}


public:

	//
	// 默认构造函数
	//
	MList() :mLength(0ull){}

	//
	// 使用初始化列表进行初始化
	//
	MList(const std::initializer_list<T>& l){
		mLength = l.size();
		PrivateInit(l.begin(), l.end());
	}

	//
	// 使用标准库类型容器进行构造
	//
	template<class A, template<class T1, class A1> class C>
	MList(const C<T, A>& c){
		mLength = c.size();
		PrivateInit(l.begin(), l.end());
	}

	template<class T, class A, template<class T1, class A1> class C>
	MList(const C<T, A>& c){
		mLength = c.size();
		PrivateInit(c.begin(), c.end());
	}

	//
	// 使用Qt之类容器进行构造
	//
	template<template<class T1> class C>
	MList(const C<T>& c){
		mLength = c.size();
		PrivateInit(l.begin(), l.end());
	}

	template<class U, template<class T1> class C>
	MList(const C<U>& c){
		mLength = c.size();
		PrivateInit(c.begin(), c.end());
	}

	//
	// copy构造
	//
	MList(const MList& other){
		mLength = other.mLength;
		PrivateInit(other.begin(), other.end());
	}

	template<class U>
	MList(const MList<U>& other){
		mLength = other.mLength;
		PrivateInit(other.begin(), other.end());
	}

	//
	// 幅值操作
	//
	MList& operator=(const MList& other){
		if (this == &other){
			return *this;
		}
		clear();
		mLength = other.mLength;
		PrivateInit(other.begin(), other.end());
		return *this;
	}

	template<class U>
	MList& operator=(const MList& other){
		clear();
		mLength = other.mLength;
		PrivateInit(other.begin(), other.end());
		return *this;
	}

	template<class A, template<class T1, class A1> class C>
	MList& operator=(const C<T, A>& c){
		clear();
		mLength = c.size();
		PrivateInit(c.begin(), c.end());
		return *this;
	}

	template<class U, class A, template<class T1, class A1> class C>
	MList& operator=(const C<U, A>& c){
		clear();
		mLength = c.size();
		PrivateInit(c.begin(), c.end());
		return *this;
	}

	template<template<class T1> class C>
	MList& operator=(const C<T>& c){
		clear();
		mLength = c.size();
		PrivateInit(c.begin(), c.end());
		return *this;
	}

	template<class U, template<class T1> class C>
	MList& operator=(const C<U>& c){
		clear();
		mLength = c.size();
		PrivateInit(c.begin(), c.end());
		return *this;
	}

	template<class iter>
	MList& assign(iter beg, iter end){
		clear();
		PrivateInit(beg, beg);
		mLength = PrivateSize();
		return *this;
	}

	MList& assign(iterator beg, iterator end){
		clear();
		PrivateInit(beg, beg);
		mLength = PrivateSize();
		return *this;
	}
	//
	// 转换到其他类型
	//
	template<class A, template<class T1, class A1> class C>
	operator C<T, A>(){
		C<T, A> c(begin(), c.end());
		return c;
	}

	template<class A, template<class T1, class A1> class C>
	operator C<T, A>() const{
		C<T, A> c(begin(), c.end());
		return c;
	}

	template<class U, class A, template<class T1, class A1> class C>
	operator C<U, A>(){
		C<U, A> c;
		for (T t : *this){
			U val;
			mj::copyvalue(val, c);
			c.push_back(val);
		}
		return c;
	}

	template<class U, class A, template<class T1, class A1> class C>
	operator C<U, A>() const{
		C<U, A> c;
		for (T t : *this){
			U val;
			mj::copyvalue(val, c);
			c.push_back(val);
		}
		return c;
	}

	template<template<class T1>class C>
	operator C<T>(){
		C<T> c(begin(), c.end());
		return c;
	}

	template<template<class T1>class C>
	operator C<T>() const{
		C<T> c(begin(), c.end());
		return c;
	}

	template<class U, template<class T1> class C>
	operator C<U>(){
		C<U> c;
		for (T t : *this){
			U val;
			mj::copyvalue(val, c);
			c.push_back(val);
		}
		return c;
	}

	template<class U, template<class T1> class C>
	operator C<U>() const{
		C<U> c;
		for (T t : *this){
			U val;
			mj::copyvalue(val, c);
			c.push_back(val);
		}
		return c;
	}

	//
	// clear
	//
	void clear(){
		PrivateClear();
	}

	bool empty(){
		return mLength == 0ull;
	}

	bool empty() const{
		return mLength == 0ull;
	}

	MList& push_back(const T& val){
		PrivateAppend(val);
		mLength++;
		return *this;
	}

	MList& append(const T& val){
		PrivateAppend(val);
		mLength++;
		return *this;
	}

	template<class U>
	MList& push_back(const U& val){
		T __val;
		mj::copyvalue(__val, val);
		PrivateAppend(__val);
		++mLength;
		return *this;
	}

	template<class U>
	MList& append(const U& val){
		T __val;
		mj::copyvalue(__val, val);
		PrivateAppend(__val);
		++mLength;
		return *this;
	}

	template<class U>
	MList& operator<<(const U& val){
		return append(val);
	}

	MList& push_front(const T& val){
		this->PrivateFrontAppend(val);
		++mLength;
		return *this;
	}

	T back(){
		if (!empty()){
			return *(--end());
		}
		else{
			return T();
		}
	}

	T back() const{
		if (!empty()){
			return *(--end());
		}
		else{
			return T();
		}
	}

	T front(){
		if (!empty()){
			return *(begin());
		}
		else{
			return T();
		}
	}

	T front() const{
		if (!empty()){
			return *(begin());
		}
		else{
			return T();
		}
	}

	MList& insert(iterator pos, const T& val){
		PrivateInsert(static_cast<MLink<T>*>(pos), val);
		++mLength;
		return *this;
	}

	MList& insert(iterator pos, iterator val){
		PrivateInsert(static_cast<MLink<T>*>(pos), static_cast<MLink<T>*>(val));
		++mLength;
		return *this;
	}

	size_t size(){
		return mLength;
	}

	size_t size() const{
		return mLength;
	}

	size_t counts(){
		return size();
	}

	size_t counts() const{
		return size();
	}

	//
	// 查找
	//
	iterator find(const T& val){
		MLink<T>* link = PrivateFind(val);
		return iterator(link);
	}

	const_iterator find(const T& val) const{
		MLink<T>* link = PrivateFind(val);
		return const_iterator(link);
	}

	//
	// 删除
	//
	void erase(iterator pos){
		PrivateErase(static_cast<MLink<T>*>(pos));
		--mLength;
	}

	void erase(const T& val){
		PrivateErase(val);
		--mLength;
	}

	void erase(iterator beg, iterator end){
		PrivateErase(static_cast<MLink<T>*>(beg), static_cast<MLink<T>*>(end));
		mLength = PrivateSize();
	}

	T& at(unsigned int index){
		auto beg = begin();
		std::advance(beg, index);
		if (beg != end()){
			return *beg;
		}
		else{
			throw std::out_of_range("MList at out of range");
		}
	}

	const T& at(unsigned int index) const{
		return const_cast<MLink<T>*>(this)->at(index);
	}

	T& operator[](unsigned int index){
		return at(index);
	}

	const T& operator[](unsigned int index) const{
		return at(index);
	}

	//
	// 迭代器
	//
	iterator begin(){
		MLink<T>* link = PrivateHeader();
		return iterator(link);
	}

	iterator end(){
		iterator iter;
		iter.EndFlagLink = pLink;
		while (pLink && iter.EndFlagLink->pNext){
			iter.EndFlagLink = iter.EndFlagLink->pNext;
		}
		return iter;
	}

	const_iterator begin() const{
		 MLink<T>* link = const_cast<MList*>(this)->PrivateHeader();
		return const_iterator(link);
	}

	const_iterator end() const{
		const_iterator iter;
		iter.EndFlagLink = pLink;
		while (pLink && iter.EndFlagLink->pNext){
			iter.EndFlagLink = iter.EndFlagLink->pNext;
		}
		return iter;
	}

	const_iterator cbegin(){
		MLink<T>* link = PrivateHeader();
		return const_iterator(link);
	}

	const_iterator cend(){
		return const_iterator();
	}

	//
	// 对流的支持
	//
	friend std::ostream& operator<<(std::ostream& os, const MList& l){
		const_cast<MList<T>&>(l).PrivateStream(os);
		return os;
	}

	//
	// 比较操作
	//
	friend bool operator==(const MList& left, const MList& right){
		if (left.size() != right.size()){
			return false;
		}
		auto leftbeg = left.begin();
		auto rightbeg = right.begin();
		for (; leftbeg != left.end(), rightbeg != right.end(); ++leftbeg, ++rightbeg){
			if (*leftbeg != *rightbeg){
				return false;
			}
		}
		return true;
	}

	friend bool operator!=(const MList& left, const MList& right){
		return !(left == right);
	}

	friend bool operator<(const MList& left, const MList& right){
		if (left.size() < right.size()){
			return true;
		}
		else if (left.size() > right.size()){
			return false;
		}

		auto leftbeg = left.begin();
		auto rightbeg = right.begin();
		for (; leftbeg != left.end(), rightbeg != right.end(); ++leftbeg, ++rightbeg){
			if (*leftbeg >= *rightbeg){
				return false;
			}
		}
		return true;
	}

	friend bool operator>(const MList& left, const MList& right){
		if (left.size() > right.size()){
			return true;
		}
		else if (left.size() < right.size()){
			return false;
		}

		auto leftbeg = left.begin();
		auto rightbeg = right.begin();
		for (; leftbeg != left.end(), rightbeg != right.end(); ++leftbeg, ++rightbeg){
			if (*leftbeg <= *rightbeg){
				return false;
			}
		}
		return true;
	}
};

	