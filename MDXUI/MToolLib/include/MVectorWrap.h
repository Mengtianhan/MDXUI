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
#include <algorithm>
#include <vector>
#include <numeric>
#include "MString.h"

template<class T, class A, template<class T1, class A1> class C, class AllocatorType, class Fun>
MTypeWrap<C<T, A>, AllocatorType> OperatorMVector(const MTypeWrap<C<T, A>, AllocatorType>& left, MTypeWrap<C<T, A>, AllocatorType>& right, Fun fun);

//
// 针对容器进行特化
// 使用const版本的话效率会更高
// 就算不使用const版本单想要操作底层对象对象使用引用直接引用底层数据
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
class MTypeWrap<C<T, A>, AllocatorType> : public AllocatorType{
private:
	class Proxiy{
	public:
		Proxiy(MTypeWrap<C<T, A>, AllocatorType>* data, std::size_t index);
		Proxiy(const Proxiy& other);
		Proxiy& operator=(const Proxiy& val);
		Proxiy& operator=(const T& val);
		operator T()const;
		operator T();
		T* operator&();
		const T* operator&() const;
		T toT() const;
		T& ref();
		const T& ref() const;
		friend std::ostream& operator<<(std::ostream& os, const Proxiy& proxiy){
			os << proxiy.toT();
			return os;
		}
		friend std::wostream& operator<<(std::wostream& os, const Proxiy& proxiy){
			os << proxiy.toT();
			return os;
		}
	private:
		MTypeWrap<C<T, A>, AllocatorType>*  mData;
		std::size_t mIndex;
	};

public:
	typedef C<T, A> Type;
	typedef typename MConstParamWrape<Type>::type const_reference_type;
	typedef typename MParamWrape<Type>::type reference_type;
	typedef typename MConstParamWrape<T>::type const_value_reference_type;
	typedef typename MParamWrape<T>::type value_reference_type;
	typedef T value_type;
	typedef Proxiy reference;
	typedef Proxiy const_reference;

	class const_iterator{
	public:
		typedef std::random_access_iterator_tag iterator_category;
		typedef Proxiy value_type;
		typedef std::ptrdiff_t difference_type;
		typedef std::ptrdiff_t distance_type;
		typedef Proxiy* pointer;
		typedef Proxiy  reference;
		typedef const Proxiy const_reference;

		const_iterator(MTypeWrap<C<T, A>, AllocatorType>& ptr, size_t num, size_t cnum = 0) :
			mData(ptr),
			mNum(num),
			mCurrentNum(cnum)
		{}

		const_iterator(const const_iterator& other) :
			mData(other.mData),
			mNum(other.mNum),
			mCurrentNum(other.mCurrentNum)
		{}

		const_iterator& operator=(const const_iterator& other){
			mData = other.mData;
			mNum = other.mNum;
			mCurrentNum = other.mCurrentNum;
			return *this;
		}

		const value_type* operator->(){
			if (mCurrentNum < 0 || mCurrentNum >= mNum){
				throw std::out_of_range("index out of range ..... ");
			}
			return &mData[mCurrentNum];
		}

		const value_type* operator->() const{
			if (mCurrentNum < 0 || mCurrentNum >= mNum){
				throw std::out_of_range("index out of range ..... ");
			}
			return &mData[mCurrentNum];
		}

		const_reference operator*(){
			if (mCurrentNum < 0 || mCurrentNum >= mNum){
				throw std::out_of_range("index out of range ..... ");
			}
			return mData[mCurrentNum];
		}

		const_reference operator*() const{
			if (mCurrentNum < 0 || mCurrentNum >= mNum){
				throw std::out_of_range("index out of range ..... ");
			}
			return mData[mCurrentNum];
		}

		const_iterator operator++(int){
			const_iterator temp(*this);
			++mCurrentNum;
			return temp;
		}

		const_iterator& operator++(){
			++mCurrentNum;
			return *this;
		}

		const_iterator operator--(int){
			const_iterator temp(*this);
			--mCurrentNum;
			return temp;
		}

		const_iterator& operator--(){
			--mCurrentNum;
			return *this;
		}

		const_iterator& operator+(size_t index){
			mCurrentNum += index;
			return *this;
		}

		const_iterator& operator-(size_t index){
			mCurrentNum -= index;
			return *this;
		}

		const_iterator& operator+=(size_t index){
			mCurrentNum += index;
			return *this;
		}

		const_iterator& operator-=(size_t index){
			mCurrentNum -= index;
			return *this;
		}

		friend size_t operator-(const_iterator first, const_iterator second){
			return first.mCurrentNum - second.mCurrentNum;
		}

		friend bool operator<(const_iterator first, const_iterator second){
			if (first.mData.empty() && second.mData.empty()){
				return false;
			}
			if (first.mNum != second.mNum || &(first.mData.at(0)) != &(second.mData.at(0))){
				throw std::runtime_error("Error ..... ");
			}
			return first.mCurrentNum < second.mCurrentNum;
		}

		friend bool operator>(const_iterator first, const_iterator second){
			if (first.mData.empty() && second.mData.empty()){
				return false;
			}
			if (first.mNum != second.mNum || &(first.mData.at(0)) != &(second.mData.at(0))){
				throw std::runtime_error("Error ..... ");
			}
			return first.mCurrentNum > second.mCurrentNum;
		}

		friend bool operator==(const_iterator first, const_iterator second){
			if (first.mData.empty() && second.mData.empty()){
				return true;
			}
			if (first.mNum != second.mNum || &(first.mData.at(0)) != &(second.mData.at(0))){
				throw std::runtime_error("Error ..... ");
			}
			return  first.mCurrentNum == second.mCurrentNum;
		}

		friend bool operator!=(const_iterator first, const_iterator second){
			if (first.mData.empty() && second.mData.empty()){
				return false;
			}
			if (first.mNum != second.mNum || &(first.mData.at(0)) != &(second.mData.at(0))){
				throw std::runtime_error("Error ..... ");
			}
			return  first.mCurrentNum != second.mCurrentNum;
		}

		size_t CurrentIndex() const{
			return mCurrentNum;
		}

		const MTypeWrap<C<T, A>, AllocatorType>&  CurrentData(){
			return mData;
		}
	protected:
		MTypeWrap<C<T, A>, AllocatorType>& mData;
		size_t							   mNum;
		size_t							   mCurrentNum;
	};

	class iterator : public const_iterator{
	public:
		iterator(MTypeWrap<C<T, A>, AllocatorType>& ptr, size_t num, size_t cnum = 0) :
			const_iterator(ptr, num, cnum)
		{}

		iterator(const iterator& other) :
			const_iterator(other)
		{}

		iterator& operator=(const iterator& other){
			const_iterator::operator=(other);
			return *this;
		}

		value_type* operator->(){
			if (mCurrentNum < 0 || mCurrentNum >= mNum){
				throw std::out_of_range("index out of range ..... ");
			}
			return &mData[mCurrentNum];
		}

		const value_type* operator->() const{
			if (mCurrentNum < 0 || mCurrentNum >= mNum){
				throw std::out_of_range("index out of range ..... ");
			}
			return &mData[mCurrentNum];
		}

		reference operator*(){
			if (mCurrentNum < 0 || mCurrentNum >= mNum){
				throw std::out_of_range("index out of range ..... ");
			}
			return mData[mCurrentNum];
		}

		const reference operator*() const{
			if (mCurrentNum < 0 || mCurrentNum >= mNum){
				throw std::out_of_range("index out of range ..... ");
			}
			return mData[mCurrentNum];
		}

		iterator operator++(int){
			iterator temp(*this);
			++mCurrentNum;
			return temp;
		}

		iterator& operator++(){
			++mCurrentNum;
			return *this;
		}

		iterator operator--(int){
			iterator temp(*this);
			--mCurrentNum;
			return temp;
		}

		iterator& operator--(){
			--mCurrentNum;
			return *this;
		}

		iterator& operator+(size_t index){
			mCurrentNum += index;
			return *this;
		}

		iterator& operator-(size_t index){
			mCurrentNum -= index;
			return *this;
		}

		iterator& operator+=(size_t index){
			mCurrentNum += index;
			return *this;
		}

		iterator& operator-=(size_t index){
			mCurrentNum -= index;
			return *this;
		}

		friend size_t operator-(iterator first, iterator second){
			return first.mCurrentNum - second.mCurrentNum;
		}

		friend bool operator<(iterator first, iterator second){
			if (first.mData.size() == 0 && second.mData.size() == 0){
				return false;
			}
			if (first.mNum != second.mNum || &(first.mData.at(0)) != &(second.mData.at(0))){
				throw std::runtime_error("Error ..... ");
			}
			return first.mCurrentNum < second.mCurrentNum;
		}

		friend bool operator>(iterator first, iterator second){
			if (first.mData.size() == 0 && second.mData.size() == 0){
				return false;
			}
			if (first.mNum != second.mNum || &(first.mData.at(0)) != &(second.mData.at(0))){
				throw std::runtime_error("Error ..... ");
			}
			return first.mCurrentNum > second.mCurrentNum;
		}

		friend bool operator==(iterator first, iterator second){
			if (first.mData.size() == 0 && second.mData.size() == 0){
				return true;
			}
			if (first.mNum != second.mNum || &(first.mData.at(0)) != &(second.mData.at(0))){
				throw std::runtime_error("Error ..... ");
			}
			return  first.mCurrentNum == second.mCurrentNum;
		}

		friend bool operator!=(iterator first, iterator second){
			if (first.mData.size() == 0 && second.mData.size() == 0){
				return false;
			}
			if (first.mNum != second.mNum || &(first.mData.at(0)) != &(second.mData.at(0))){
				throw std::runtime_error("Error ..... ");
			}
			return  first.mCurrentNum != second.mCurrentNum;
		}

		size_t CurrentIndex() const{
			return mCurrentNum;
		}

		MTypeWrap<C<T, A>, AllocatorType>&  CurrentData(){
			return mData;
		}
	};

public:
	MTypeWrap(const_reference_type val = Type());
	MTypeWrap& operator =(const_reference_type val);

	//
	// 泛型构造
	//
	MTypeWrap(C<T, A>&& other);
	MTypeWrap(T* ptr, unsigned num);
	MTypeWrap(const std::initializer_list<T>& list);
	template<class U>
	MTypeWrap(const std::initializer_list<U>& list);

	MTypeWrap& operator=(C<T, A>&& other);

	MTypeWrap& operator=(const std::initializer_list<T>& list);
	template<class U>
	MTypeWrap& operator=(const std::initializer_list<U>& list);


	template<class U>
	MTypeWrap(const std::vector<U>& other);

	template<class U,class A2,template<class,class>class C2>
    MTypeWrap(const C2<U, A2>& other);

	template<class U, class A2>
	MTypeWrap(const MTypeWrap<C<U, A2>, AllocatorType>& other);




	template<class U>
	MTypeWrap& operator=(const std::vector<U>& other);

	template<class U, class A2, template<class, class>class C2>
	MTypeWrap& operator=(const C2<U, A2>& other);

	template<class U,class A2>
	MTypeWrap& operator=(const MTypeWrap<C<U, A2>, AllocatorType>& other);


	//
	// 将对象锁定不可共享
	//
	void lock();

	//
	// 解锁可共享
	//
	void unlock();

	//
	// 查询是否锁定不共享
	//
	bool islock() const;

	//
	// 实现可隐式转换类型
	//
	operator Type();

	//
	// 如果共享非const版本将会有一个重新构建的过程，并且新对象将会被标记为不可共享
	//
	reference_type toType();
	const_reference_type toType() const;

	//
	// 克隆一个新对象出来，新新对象被标记为不共享
	//
	MTypeWrap clone();
	MTypeWrap clone() const;

	//
	// 直接引用原始数据
	//
	const_reference_type reference_obj();
	const_reference_type reference_obj() const;

	const_reference_type ref(){ return reference_obj(); }
	const_reference_type ref() const{ return reference_obj(); }


	//
	// 直接引用原始数据，可能会破坏数据
	//
	reference_type unsafe_ref();

	void* Address() const;

	//
	// 隐式转换
	//
	template<class A2, template<class, class> class C2>
	operator C2<T, A2>() const;

	template<class A2, template<class, class> class C2>
	operator C2<T, A2>();

	template<template<class>class C2>
	operator C2<T>() const;

	template<template<class>class C2>
	operator C2<T>();

	template<class U, class A2, template<class, class> class C2>
	operator C2<U, A2>() const;

	template<class U, class A2, template<class, class> class C2>
	operator C2<U, A2>();

	template<class U, template<class>class C2>
	operator C2<U>() const;

	template<class U, template<class>class C2>
	operator C2<U>();


	/***************************下面是针对容器进行特化的操作****************************/

	//
	// 使用非标准类型对其进行初始化
	//
	template<class T1, template<class T2> class C1>
	void from_other(const C1<T1>& other_container);

	template<class T1, class A1, class CMP, template<class T2, class A2, class CMP2> class S>
	void from_other(const S<T1, A1, CMP>& other_set);

	template<class T1, class A1, template<class, class> class C1>
	void from_other(const C1<T1, A1>& other_container);

	//
	// 通过一个指针进行填充向量
	//
	void from_other(T* ptr, unsigned num);

	//
	// 管理一个指针
	// 该对象将不共享
	//
	void attach(T*, unsigned num);
	T* detach();

	//
	// range函数可以生成一个系列
	//
	Type range(const_value_reference_type start, const_value_reference_type stop, const_value_reference_type step) const;

	//
	// 切片数据返回一个列表
	// 当step为负数时方向切片
	// 当step为正向时正向切片
	//
	Type slice(std::size_t start, std::size_t stop, int step = 1) const;

	//
	// 使用迭代器赋值
	//
	template<class iter>
	void assign(iter __First, iter __Last);

	//
	// 添加元素
	//
	MTypeWrap& push_back(const_value_reference_type val);
	MTypeWrap& operator<<(const_value_reference_type val);
	MTypeWrap& append(const_value_reference_type val);

	//
	// 追加一个容器
	//
	MTypeWrap& push_back(const MTypeWrap& other);
	MTypeWrap& operator<<(const MTypeWrap& other);
	MTypeWrap& append(const MTypeWrap& other);

	MTypeWrap& push_front(const_value_reference_type val);
	MTypeWrap& push_front(const MTypeWrap& other);

	//
	// 不安全追加
	//
	MTypeWrap& unsafe_push_back(const_value_reference_type val);
	MTypeWrap& unsafe_push_back(const MTypeWrap& other);
	MTypeWrap& unsafe_push_back(const std::initializer_list<T>& other);

	//
	// 填充元素
	//
	void fill(std::size_t num, const_value_reference_type val);
	void fill(iterator pos1, iterator pos2, const_value_reference_type val);

	//
	// 索引元素
	//
	reference operator[](unsigned index);
	const_reference operator[](unsigned index) const;

	reference at(unsigned index);
	const_reference at(unsigned index) const;

	reference operator()(unsigned index);
	const_reference operator()(unsigned index) const;

	//
	// 不安全操作 
	// 直接操作底层数据 不执行共享检查 不执行边界检查
	//
	T& unsafe_ref(unsigned index);
	const T& unsafe_ref(unsigned index) const;   


	//
	// 不安全操作 
	// 直接操作底层数据 不执行共享检查 执行边界检查
	//
	T& unsafe_at(unsigned index);
	const T& unsafe_at(unsigned index) const;



	//
	// 返回最后一个元素
	//
	reference back();
	const_reference back() const;

	//
	// 返回第一个元素
	//
	reference front();
	const_reference front() const;

	//
	// 迭代器 开始位置
	//
	iterator begin();
	const_iterator begin() const;

	//
	// 迭代器 结束位置
	//
	iterator end();
	const_iterator end() const;


	//
	// 复制一段数据出来
	//
	MTypeWrap copy(int first, int last) const;

	//
	// 查找
	//
	iterator find(const_value_reference_type val);
	const_iterator find(const_value_reference_type val) const;
	template<class Fun>
	iterator find_if(const_value_reference_type val, Fun prd);
	template<class Fun>
	const_iterator find_if(const_value_reference_type val, Fun prd) const;
	template<class Fun>
	unsigned find_count(const_value_reference_type val, Fun prd) const;
	//
	// 查找范围，范围最后一个小于minval的索引和第一个大于maxval的索引
	//
	std::pair<unsigned, unsigned> find_range(const_value_reference_type minval, const_value_reference_type maxval) const;

	template<class Fun = std::equal_to<T>>
	unsigned find_lowindex(const_value_reference_type val, Fun prd = Fun()) const;

	template<class Fun = std::equal_to<T>>
	unsigned find_upindex(const_value_reference_type val, Fun prd = Fun()) const;

	//
	// 查找索引
	//
	template<class cmp = std::equal_to<T>>
	std::size_t find_index(const_value_reference_type val, cmp prd = cmp()) const;

	template<class cmp = std::equal_to<T>>
	MTypeWrap find_index(const MTypeWrap& vals, cmp prd = cmp()) const;


	template<class cmp>
	MTypeWrap find_index(cmp fun) const;

	//
	// 获取一个交集
	//
	template<class cmp = std::equal_to<T>>
	MTypeWrap intersection(const MTypeWrap& other, cmp prd = cmp()) const;

	template<template<class> class C2, class cmp = std::equal_to<T>>
	MTypeWrap intersection(const C2<T>& other, cmp prd = cmp()) const;

	template<template<class, class> class C2, class cmp = std::equal_to<T>>
	MTypeWrap intersection(const C2<T, A>& other, cmp prd = cmp()) const;

	//
	// 检查是否包含
	//
	template<class cmp = std::equal_to<T>>
	bool include(const MTypeWrap& other, cmp prd = cmp()) const;

	template<template<class> class C2, class cmp = std::equal_to<T>>
	bool include(const C2<T>& other, cmp prd = cmp()) const;

	template<template<class, class> class C2, class cmp = std::equal_to<T>>
	bool include(const C2<T, A>& other, cmp prd = cmp()) const;

	//
	// 求一个集合的补集
	//
	template<class cmp = std::equal_to<T>>
	MTypeWrap complement(const MTypeWrap& other, cmp prd = cmp()) const;

	//
	// 检查两个容器是否相等
	//
	template<class cmp = std::equal_to<T>>
	bool equal(const MTypeWrap& other, cmp prd = cmp()) const;

	template<template<class> class C2, class cmp = std::equal_to<T>>
	bool equal(const C2<T>& other, cmp __Prd = cmp()) const;

	template<template<class, class> class C2, class cmp = std::equal_to<T>>
	bool equal(const C2<T, A>& other, cmp __Prd = cmp()) const;

	bool less_than(const MTypeWrap& other) const;

	template<template<class> class C2>
	bool less_than(const C2<T>& other) const;

	template<template<class, class> class C2>
	bool less_than(const C2<T, A>& other) const;



	//
	// 大于
	//
	bool greater_than(const MTypeWrap& other) const;

	template<template<class> class C2>
	bool greater_than(const C2<T>& other) const;

	template<template<class, class> class C2>
	bool greater_than(const C2<T, A>& other) const;

	//
	// 小于等于
	//
	bool less_equal(const MTypeWrap& other) const;

	template<template<class> class C2>
	bool less_equal(const C2<T>& other) const;

	template<template<class, class> class C2>
	bool less_equal(const C2<T, A>& other) const;

	//
	// 大于等于
	//
	bool greater_equal(const MTypeWrap& other) const;

	template<template<class> class C2>
	bool greater_equal(const C2<T>& other) const;

	template<template<class, class> class C2>
	bool greater_equal(const C2<T, A>& other) const;

	//
	// 映射一个区间
	//
	MTypeWrap map(unsigned lowindex, unsigned upindex) const;

	//
	// 检查是否包含
	//
	template<class cmp = std::equal_to<T>>
	bool contains(const_value_reference_type val, cmp prd = cmp()) const;

	//
	// 搜索元素
	//
	template<class T1, class A1, template<class T2, class A2> class C1, class cmp = std::equal_to<T>>
	C1<T1, A1>& search(const_value_reference_type val, C1<T1, A1>& out, cmp prd = cmp()) const;
	template<class T1, template<class T2> class C1, class cmp = std::equal_to<T>>
	C1<T1>& search(const_value_reference_type val, C1<T1>& out, cmp prd = cmp()) const;


	//
	// 查找最靠近的指定值的值
	//
	bool near_search(const_value_reference_type val, T& out);
	bool near_search(const_value_reference_type val, T& out) const;

	bool near_search_left(const_value_reference_type val, T& out);
	bool near_search_left(const_value_reference_type val, T& out) const;

	bool near_search_right(const_value_reference_type val, T& out);
	bool near_search_right(const_value_reference_type val, T& out) const;

	//
	// 迭代器 最后一个元素位置
	//
	iterator last();
	const_iterator last() const;

	//
	// 插入元素
	//
	void insert(iterator pos, const_value_reference_type val);
	void insert(iterator pos, std::size_t count, const_value_reference_type val);
	template<class iter>
	void insert(iterator pos, iter __first, iter __last); // 插入 一段数据

	//
	// 删除元素
	//
	iterator erase(iterator pos);
	void erase(iterator pos1, iterator pos2);
	template<class cmp = std::equal_to<T>>
	void erase(const_value_reference_type  val, cmp prd = cmp());


	//
	// 插入元素 索引版本
	//
	void insert_index(int pos, const_value_reference_type val);
	void insert_index(int pos, std::size_t count, const_value_reference_type val);
	template<class iter>
	void insert_index(int pos, iter __first, iter __last); // 插入 一段数据

	//
	// 删除元素 索引版本
	//
	void erase_index(int pos);
	void erase_index(int pos1, int pos2);

	//
	// 弹出最后和开始位置的元素
	//
	void pop_back();
	void pop_front();

	//
	// 查看容器大小
	//
	std::size_t size();
	std::size_t size() const;

	//
	// 重新修改容器大小
	//
	void resize(std::size_t size);

	//
	// 预分配空间
	//
	void reserve(std::size_t size);

	//
	// 清空容器
	//
	void clear();

	//
	// 容器是否为空
	//
	bool empty() const;

	//
	// 类型转换
	//
	template < class T1, class A1, template < class T2, class A2> class C1>
	C1<T1, A1>& convert_type(C1<T1, A1>& out);

	template < class T1, template < class T2> class C1>
	C1<T1>& convert_type(C1<T1>& out);

	//
	// 带信息的打印
	//
	void print(const char* msg = "", std::ostream& os = std::cout) const;

	//
	// 将数据写进流获取是从流中读取
	//
	void ostream(std::ostream& os, const char* spliter = " ");
	void istream(std::istream& is);

	//
	// 将数据写进文件或者是从文件中读取
	//
	void save(const char* filename, const char* spliter = "\t");

	//
	// 数据的分隔可以使空格分好制表符逗号
	//
	void load(const char* filename);

	//
	// 排序
	//
	template<class cmp = std::less<T>>
	void sort(cmp prd = cmp());

	//
	// 按照给定条件移除元素
	//
	template<class cmp = std::equal_to<T>>
	void remove(const_value_reference_type val, cmp prd = cmp());

	//
	// 去掉重复的元素
	//
	template<class cmp = std::equal_to<T>, class cmp2 = std::less<T>>
	void unique(cmp prd = cmp(), cmp2 prd2 = cmp2());

	//
	// 排序一个区间
	//
	template<class cmp = std::less<T>>
	void sort(iterator first, iterator last, cmp prd = cmp());

	//
	// 针对一个区间进行元素的移除
	//
	template<class cmp = std::equal_to<T>>
	void remove(iterator first, iterator last, const_value_reference_type val, cmp prd = cmp());

	//
	// 针对一个区间进行元素的不重复
	//
	template<class cmp = std::equal_to<T>>
	void unique(iterator first, iterator last, cmp prd = cmp());

	//
	// 翻转元素
	//
	void reverse();
	void reverse(iterator first, iterator last);


	/*********下面是提供一些方便统计的数学计算,所以只适应于标量类型*************/

	//
	// 计算最大值
	//
	T max_element() const;

	template<class cmp>
	T max_element(cmp prd) const;

	template<class cmp = std::equal_to<T>>
	T max_element(const_value_reference_type excepty, cmp prd = cmp()) const;


	//
	// 计算最小值
	//
	T min_element() const;

	template<class cmp>
	T min_element(cmp prd) const;

	template<class cmp = std::equal_to<T>>
	T min_element(const_value_reference_type excepty, cmp prd = cmp()) const;

	//
	// 迭代器
	//
	const_iterator max_iterator() const;

	template<class cmp = std::equal_to<T>>
	const_iterator max_iterator(const_value_reference_type excepty, cmp prd = cmp()) const;

	const_iterator min_iterator() const;

	template<class cmp = std::equal_to<T>>
	const_iterator min_iterator(const_value_reference_type excepty, cmp prd = cmp()) const;

	//
	// 计算平均值
	//
	T aval_value() const;

	template<class cmp = std::equal_to<T>>
	T aval_value(const_value_reference_type excepty, cmp prd = cmp()) const;

	//
	// 计算总和
	//
	T sum_value() const;

	template<class cmp = std::equal_to<T>>
	T sum_value(const_value_reference_type excepty, cmp prd = cmp()) const;

	//
	// 计算最小间距
	//
	T min_span(unsigned step = 1) const;

	//
	// 计算最大间距
	//
	T max_span(unsigned step = 1) const;

	//
	// 间距的系列
	//
	Type span_array(unsigned step = 1) const;

	//
	// 计算最大公约数
	//
	T gcd() const;

	//
	// 计算峰峰值 或者 谷谷值
	//
	template<class cmp = std::greater<T>>
	Type p2p(cmp prd = cmp()) const;

	//
	// 计算标准差
	//
	T standard_deviation() const;
	T standard_deviation(double val) const;
	template<class cmp = std::equal_to<T>>
	T standard_deviation(const_value_reference_type excepty, cmp prd = cmp()) const;
	template<class cmp = std::equal_to<T>>
	T standard_deviation(double val, const_value_reference_type excepty, cmp prd = cmp()) const;

	//
	// 计算方差
	//
	T square_deviation() const;
	T square_deviation(double val) const;
	template<class cmp = std::equal_to<T>>
	T square_deviation(const_value_reference_type excepty, cmp prd = cmp()) const;
	template<class cmp = std::equal_to<T>>
	T square_deviation(double val, const_value_reference_type excepty, cmp prd = cmp()) const;

	//
	// 按照给定条件计算总和
	//
	template<class In>
	struct AccuFun : public std::binary_function<In, In, In>{
		In operator()(In in1, In in2){
			return in1 + in2;
		}
	};
	template<class Fun = AccuFun<T>>
	T accumulate(const_value_reference_type val, Fun fun = Fun()) const;

	//
	// 调整数据
	// 例如将数据调整到+-180之间
	// adjust(360,-180,180)
	//
	template<class Less = std::less<T>, class Greater = std::greater<T>, class Equal_to = std::equal_to<T>>
	void adjust(const_value_reference_type adjustvalue, const_value_reference_type __minvalue,
		const_value_reference_type __maxvalue, const_value_reference_type excepty,
		Less __lessfun = Less(),
		Greater __greaterfun = Greater(),
		Equal_to __equalfun = Equal_to());


	//
	// 调整数据，直接将满足某些条件的数据替换为指定数
	//
	template<class cmp = std::less<T>>
	void replace(const_value_reference_type replaced, const_value_reference_type value, cmp prd = cmp());

	//
	// 所有数据 + 上一个数
	//
	MTypeWrap& plus(const_value_reference_type val);

	template<class cmp = std::equal_to<T>>
	MTypeWrap& plus(const_value_reference_type val, const_value_reference_type excepty, cmp prd = cmp());

	//
	// 所有数据 - 一个数
	//
	MTypeWrap& minus(const_value_reference_type val);

	template<class cmp = std::equal_to<T>>
	MTypeWrap& minus(const_value_reference_type val, const_value_reference_type excepty, cmp prd = cmp());

	//
	// 所有数据 * 一个数
	//
	MTypeWrap& mult(const_value_reference_type val);

	template<class cmp = std::equal_to<T>>
	MTypeWrap& mult(const_value_reference_type val, const_value_reference_type excepty, cmp prd = cmp());

	//
	// 所有数 / 一个数
	//
	MTypeWrap& division(const_value_reference_type val);

	template<class cmp = std::equal_to<T>>
	MTypeWrap& division(const_value_reference_type val, const_value_reference_type excepty, cmp prd = cmp());

	//
	// +-*/ 的操作符重载
	//
	MTypeWrap& operator+=(const_value_reference_type other);
	MTypeWrap& operator-=(const_value_reference_type other);
	MTypeWrap& operator*=(const_value_reference_type other);
	MTypeWrap& operator/=(const_value_reference_type other);

	//
	// 一个系列加上一个系列
	//
	MTypeWrap& operator+=(const MTypeWrap& other);
	MTypeWrap& operator-=(const MTypeWrap& other);

	//
	// 一些数学变换
	//
	MTypeWrap sin() const;
	MTypeWrap cos() const;
	MTypeWrap asin() const;
	MTypeWrap acos() const;
	MTypeWrap log() const;
	MTypeWrap log2() const;
	MTypeWrap log10() const;
	MTypeWrap square() const;
	MTypeWrap power(double k) const;
	MTypeWrap sqrt() const;
	MTypeWrap abs() const;

	template<class F>
	MTypeWrap tranform_copy(F fun) const;

	template<class F>
	MTypeWrap& transform(F fun);

	//
	// 操作两个系列
	//
	template<class T1, class A1, template<class, class> class C1, class AllocatorType1, class Fun>
	friend MTypeWrap<C1<T1, A1>, AllocatorType1> OperatorMVector(const MTypeWrap<C1<T1, A1>, AllocatorType1>& left, MTypeWrap<C1<T1, A1>, AllocatorType1>& right, Fun fun);

	//
	// 两个系列之和等于一个系列
	//
	template<class T1, class A1, template<class, class> class C1, class AllocatorType1>
	friend MTypeWrap<C1<T1, A1>, AllocatorType1> operator+(const MTypeWrap<C1<T1, A1>, AllocatorType1>& left, const MTypeWrap<C1<T1, A1>, AllocatorType1>& right);

	//
	// 两个系列之差等于一个系列
	//
	template<class T1, class A1, template<class, class> class C1, class AllocatorType1>
	friend MTypeWrap<C1<T1, A1>, AllocatorType1> operator-(const MTypeWrap<C1<T1, A1>, AllocatorType1>& left, const MTypeWrap<C1<T1, A1>, AllocatorType1>& right);

	//
	// 实现流输出
	//
	template<class T1, class A1, template<class, class> class C1, class AllocatorType1>
	friend std::ostream& operator<<(std::ostream& os, const MTypeWrap<C1<T1, A1>, AllocatorType1>& other);


	friend bool operator==(const MTypeWrap& left, const MTypeWrap& right){
		return left.equal(right);
	}

	friend bool operator!=(const MTypeWrap& left, const MTypeWrap& right){
		return !left.equal(right);
	}

	friend bool operator<(const MTypeWrap& left, const MTypeWrap& right){
		return left.less_than(right);
	}

	friend bool operator<=(const MTypeWrap& left, const MTypeWrap& right){
		return left.less_equal(right);
	}

	friend bool operator>(const MTypeWrap& left, const MTypeWrap& right){
		return left.greater_than(right);
	}

	friend bool operator>=(const MTypeWrap& left, const MTypeWrap& right){
		return left.greater_equal(right);
	}
private:
	class MContentType : public MRefImpl<AllocatorType>
	{
	public:
		MContentType(const_reference_type val = Type());
		MContentType(const MContentType& other);
		MContentType& operator=(const MContentType& other);
		MContentType& operator =(const_reference_type val);
		void updatevalue(std::size_t index, const_value_reference_type val);
		Type   mVal;
	};
	MRefWrap<MContentType> mVal;
};

/******************************************下面是实现细节没兴趣可以不用关心*************************************************************************/
/*-----------------------------------------------------------------------------------------------*/
/*   实现
*
*
*
*
*
*
*
*
*
*
/*-----------------------------------------------------------------------------------------------
*
*
*
*
*/
/*---------------------------------------------华丽的分割线---------------------------------------*/
//
// 代理的实现
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>::Proxiy::Proxiy(MTypeWrap< C<T, A>, AllocatorType>* data, std::size_t index) :
mData(data),
mIndex(index)
{
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>::Proxiy::Proxiy(const typename MTypeWrap< C<T, A>, AllocatorType>::Proxiy& other) :
mData(other.mData),
mIndex(other.mIndex)
{
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::Proxiy& MTypeWrap< C<T, A>, AllocatorType>::Proxiy::operator=(
	const typename MTypeWrap< C<T, A>, AllocatorType>::Proxiy& val)
{
	if (mData->mVal->IsShared()){
		mData->mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*(mData->mVal));
	}
	mData->mVal->updatevalue(mIndex, val);
	return *this;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::Proxiy& MTypeWrap< C<T, A>, AllocatorType>::Proxiy::operator=(const T& val){
	if (mData->mVal->IsShared()){
		mData->mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*(mData->mVal));
	}
	mData->mVal->updatevalue(mIndex, val);
	return *this;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>::Proxiy::operator T() const{
	auto beg = mData->mVal->mVal.begin();
	std::advance(beg, mIndex);
	return *beg;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>::Proxiy::operator T(){
	auto beg = mData->mVal->mVal.begin();
	std::advance(beg, mIndex);
	return *beg;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
T MTypeWrap< C<T, A>, AllocatorType>::Proxiy::toT() const{
	auto beg = mData->mVal->mVal.begin();
	std::advance(beg, mIndex);
	return *beg;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
T& MTypeWrap< C<T, A>, AllocatorType>::Proxiy::ref(){
	auto beg = mData->mVal->mVal.begin();
	std::advance(beg, mIndex);
	return *beg;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
const T& MTypeWrap< C<T, A>, AllocatorType>::Proxiy::ref() const{
	return const_cast<MTypeWrap< C<T, A>, AllocatorType>::Proxiy*>(this)->ref();
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
T* MTypeWrap< C<T, A>, AllocatorType>::Proxiy::operator&(){
	auto beg = mData->mVal->mVal.begin();
	std::advance(beg, mIndex);
	return &(*beg);
}

//-------------------------------------------------------------------//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>::MContentType::MContentType(typename MTypeWrap< C<T, A>, AllocatorType>::const_reference_type val) :
MRefImpl<AllocatorType>(),
mVal(val)
{
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>::MContentType::MContentType(const typename MTypeWrap< C<T, A>, AllocatorType>::MContentType& other) :
MRefImpl<AllocatorType>(),
mVal(other.mVal)
{
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::MContentType& MTypeWrap< C<T, A>, AllocatorType>::MContentType::operator = (
	const typename MTypeWrap< C<T, A>, AllocatorType>::MContentType& other)
{
	MRefImpl<Allocator>::operator=(other);
	mVal = other.mVal;
	return *this;
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::MContentType& MTypeWrap< C<T, A>, AllocatorType>::MContentType::operator = (
	typename MTypeWrap< C<T, A>, AllocatorType>::const_reference_type val)
{
	MRefImpl<Allocator>::operator=(*this);
	mVal = val;
	return *this;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::MContentType::updatevalue(std::size_t index,
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val)
{
	if (index >= mVal.size()){
		return;
	}
	auto beg = mVal.begin();
	std::advance(beg, index);
	*beg = val;
}

//--------------------------------------------------------------------------
//
//  包装类的实现
//
//---------------------------------------------------------------------------


///
/// 常规构造函数
///
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>::MTypeWrap(typename MTypeWrap< C<T, A>, AllocatorType>::const_reference_type val) :
mVal(new MTypeWrap< C<T, A>, AllocatorType>::MContentType(val))
{
}

///
/// 常规赋值操作
///
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>&  MTypeWrap< C<T, A>, AllocatorType>::operator =(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_reference_type val)
{
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(val);
	}
	else{
		mVal->mVal = val;
	}
	return *this;
}

//
// 泛型构造
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>::MTypeWrap(C<T, A>&& other) :
mVal(new MTypeWrap< C<T, A>, AllocatorType>::MContentType(other))
{
	other.clear();
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>::MTypeWrap(T* ptr, unsigned num) :
mVal(new MTypeWrap< C<T, A>, AllocatorType>::MContentType())
{
	mVal->mVal.assign(ptr, ptr + num);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>::MTypeWrap(const std::initializer_list<T>& list) :
mVal(new MTypeWrap< C<T, A>, AllocatorType>::MContentType(list))
{
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class U>
MTypeWrap< C<T, A>, AllocatorType>::MTypeWrap(const std::initializer_list<U>& list) :
mVal(new MTypeWrap< C<T, A>, AllocatorType>::MContentType())
{
	for (auto& v : list){
		T val;
		mj::copyvalue(val, v);
		mVal->mVal.push_back(val);
	}
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::operator=(C<T, A>&& other){
	if (mVal->IsShared()){
		mVal = new typename MTypeWrap< C<T, A>, AllocatorType>::MContentType(other);
	}
	else{
		mVal->mVal = other;
	}
	other.clear();
	return *this;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::operator=(const std::initializer_list<T>& list){
	if (mVal->IsShared()){
		mVal = new typename MTypeWrap< C<T, A>, AllocatorType>::MContentType(list);
	}
	else{
		mVal->mVal = list;
	}
	return *this;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class U>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::operator=(const std::initializer_list<U>& list)
{
	if (mVal->IsShared()){
		mVal = new typename MTypeWrap< C<T, A>, AllocatorType>::MContentType();
	}
	for (auto& v : list){
		T val;
		mj::copyvalue(val, v);
		mVal->mVal.push_back(val);
	}
	return *this;
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class U>
MTypeWrap< C<T, A>, AllocatorType>::MTypeWrap(const std::vector<U>& other):
mVal(new MTypeWrap< C<T, A>, AllocatorType>::MContentType())
{
	for (auto& v : other){
		T val;
		mj::copyvalue(val, v);
		mVal->mVal.push_back(val);
	}
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class U, class A2, template<class, class>class C2>
MTypeWrap< C<T, A>, AllocatorType>::MTypeWrap(const C2<U, A2>& other):
mVal(new MTypeWrap< C<T, A>, AllocatorType>::MContentType())
{
	for (auto& v : other){
		T val;
		mj::copyvalue(val, v);
		mVal->mVal.push_back(val);
	}
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class U, class A2>
MTypeWrap< C<T, A>, AllocatorType>::MTypeWrap(const MTypeWrap<C<U, A2>, AllocatorType>& other):
mVal(new MTypeWrap< C<T, A>, AllocatorType>::MContentType())
{
	for (auto& v : other){
		T val;
		mj::copyvalue(val, v.ref());
		mVal->mVal.push_back(val);
	}
}



template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class U>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::operator=(const std::vector<U>& other)
{
	clear();
	for (auto& v : other){
		T val;
		mj::copyvalue(val, v);
		mVal->mVal.push_back(val);
	}
	return *this;
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class U, class A2, template<class, class>class C2>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::operator=(const C2<U, A2>& other)
{
	clear();
	for (auto& v : other){
		T val;
		mj::copyvalue(val, v);
		mVal->mVal.push_back(val);
	}
	return *this;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class U, class A2>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::operator=(const MTypeWrap<C<U, A2>, AllocatorType>& other)
{
	clear();
	for (auto& v : other){
		T val;
		mj::copyvalue(val, v.ref());
		mVal->mVal.push_back(val);
	}
	return *this;
}

//
// 隐式转换为原始类型
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>::operator typename MTypeWrap< C<T, A>, AllocatorType>::Type(){
	return mVal->mVal;
}

//
// 锁定不可共享
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::lock(){
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	mVal->MarkUnshareable();
}

//
// 解锁共享
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::unlock(){
	mVal->MarkShareable();
}

///
/// 是否可共享
///
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
bool MTypeWrap< C<T, A>, AllocatorType>::islock() const{
	return mVal->IsShared();
}

///
/// 获取数据地址
///
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void* MTypeWrap< C<T, A>, AllocatorType>::Address() const{
	return &(mVal->mVal);
}

///
/// 显示转换为原始类型
///
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::reference_type MTypeWrap< C<T, A>, AllocatorType>::toType(){
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	mVal->MarkUnshareable();
	return mVal->mVal;
}

///
/// 显示转换为原始类型
///
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::const_reference_type MTypeWrap< C<T, A>, AllocatorType>::toType() const{
	return mVal->mVal;
}

///
/// 引用原始对象
///
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::const_reference_type MTypeWrap< C<T, A>, AllocatorType>::reference_obj(){
	return mVal->mVal;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::const_reference_type MTypeWrap< C<T, A>, AllocatorType>::reference_obj() const{
	return mVal->mVal;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::reference_type MTypeWrap< C<T, A>, AllocatorType>::unsafe_ref(){
	return mVal->mVal;
}

//
// 隐式转换
//
template<class T, class A, template<class, class> class C, class AllocatorType>
template<class A2, template<class, class> class C2>
MTypeWrap< C<T, A>, AllocatorType>::operator C2<T, A2>() const{
	C2<T, A2> out;
	out.resize(size());
	for (int i = 0; i < size(); ++i){
		out[i] = this->at(i);
	}
	return out;
}

template<class T, class A, template<class, class> class C, class AllocatorType>
template<class A2, template<class, class> class C2>
MTypeWrap< C<T, A>, AllocatorType>::operator C2<T, A2>(){
	C2<T, A2> out;
	out.resize(size());
	for (int i = 0; i < size(); ++i){
		out[i] = this->at(i);
	}
	return out;
}

template<class T, class A, template<class, class> class C, class AllocatorType>
template<template<class>class C2>
MTypeWrap< C<T, A>, AllocatorType>::operator C2<T>() const{
	C2<T> out;
	out.resize(size());
	for (int i = 0; i < size(); ++i){
		out[i] = this->at(i);
	}
	return out;
}

template<class T, class A, template<class, class> class C, class AllocatorType>
template<template<class>class C2>
MTypeWrap< C<T, A>, AllocatorType>::operator C2<T>(){
	C2<T> out;
	out.resize(size());
	for (int i = 0; i < size(); ++i){
		out[i] = this->at(i);
	}
	return out;
}

template<class T, class A, template<class, class> class C, class AllocatorType>
template<class U, class A2, template<class, class> class C2>
MTypeWrap< C<T, A>, AllocatorType>::operator C2<U, A2>() const{
	C2<U, A2> out;
	out.resize(size());
	for (int i = 0; i < size(); ++i){
		mj::copyvalue(out[i], this->at(i));
	}
	return out;
}

template<class T, class A, template<class, class> class C, class AllocatorType>
template<class U, class A2, template<class, class> class C2>
MTypeWrap< C<T, A>, AllocatorType>::operator C2<U, A2>(){
	C2<U, A2> out;
	out.resize(size());
	for (int i = 0; i < size(); ++i){
		mj::copyvalue(out[i], this->at(i));
	}
	return out;
}

template<class T, class A, template<class, class> class C, class AllocatorType>
template<class U, template<class>class C2>
MTypeWrap< C<T, A>, AllocatorType>::operator C2<U>() const{
	C2<U> out;
	out.resize(size());
	for (int i = 0; i < size(); ++i){
		mj::copyvalue(out[i], this->at(i));
	}
	return out;
}


template<class T, class A, template<class, class> class C, class AllocatorType>
template<class U, template<class>class C2>
MTypeWrap< C<T, A>, AllocatorType>::operator C2<U>(){
	C2<U> out;
	out.resize(size());
	for (int i = 0; i < size(); ++i){
		mj::copyvalue(out[i], this->at(i));
	}
	return out;
}


//
// 克隆出一个新对象
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::clone(){
	MTypeWrap< C<T, A>, AllocatorType> newobj;
	newobj.mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	newobj.mVal->MarkUnshareable();
	return newobj;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::clone() const{
	return const_cast<MTypeWrap< C<T, A>, AllocatorType>*>(this)->clone();
}

//
// 使用其他类型的容器对其进行填充
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class T1, template<class T2> class C1>
void MTypeWrap< C<T, A>, AllocatorType>::from_other(const C1<T1>& other_container){
	clear();
	for (auto& v : other_container){
		typename MTypeWrap< C<T, A>, AllocatorType>::value_type val;
		mj::copyvalue(val, v);
		mVal->mVal.push_back(val);
	}
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class T1, class A1, class CMP, template<class T2, class A2, class CMP2> class S>
void MTypeWrap< C<T, A>, AllocatorType>::from_other(const S<T1, A1, CMP>& other_set){
	clear();
	for (auto& v : other_set){
		typename MTypeWrap< C<T, A>, AllocatorType>::value_type val;
		mj::copyvalue(val, v);
		mVal->mVal.push_back(val);
	}
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class T1, class A1, template<class, class> class C1>
void MTypeWrap< C<T, A>, AllocatorType>::from_other(const C1<T1, A1>& other_container){
	clear();
	for (auto& v : other_container){
		typename MTypeWrap< C<T, A>, AllocatorType>::value_type val;
		mj::copyvalue(val, v);
		mVal->mVal.push_back(val);
	}
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::from_other(T* ptr, unsigned num){
	clear();
	mVal->mVal.assign(ptr, ptr + num);
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::attach(T* ptr, unsigned num){
	clear();
	mVal->mVal.assign(ptr, ptr + num);
	mVal->MarkUnshareable();
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
T* MTypeWrap< C<T, A>, AllocatorType>::detach(){
	return &(mVal->mVal[0]);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::Type MTypeWrap< C<T, A>, AllocatorType>::range(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type start,
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type stop,
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type step) const
{
	typename MTypeWrap< C<T, A>, AllocatorType>::Type result;
	T __start = start;
	if (std::abs(step) < 1e-12){
		result.push_back(__start);
		return result;
	}
	double val1 = step / 2;
	for (; __start <= stop + val1; __start += step){
		result.push_back(__start);
	}
	return result;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::Type MTypeWrap< C<T, A>, AllocatorType>::slice(
	std::size_t start, std::size_t stop, int step) const
{
	typename MTypeWrap< C<T, A>, AllocatorType>::Type result;
	auto beg = mVal->mVal.begin();
	auto __end = mVal->mVal.begin();
	std::advance(beg, start);
	std::advance(__end, stop);
	if (step < 0){
		if (start < stop){
			return result;
		}
		for (; beg >= __end;){
			if (beg != mVal->mVal.end()){
				result.push_back(*beg);
				start += step;
			}
			if (start >= 0){
				try{
					std::advance(beg, step);
				}
				catch (...){
					beg = __end;
				}
			}
			else{
				return result;
			}		
		}
	}
	else{
		if (start > stop){
			return result;
		}
		for (; beg < __end;){
			if (beg != mVal->mVal.end()){
				result.push_back(*beg);
				start += step;
			}
			if (start  <= this->size()){
				try{
					std::advance(beg, step);
				}
				catch (...){
					beg = __end;
				}
			}
			else{
				return result;
			}
		}
	}
	return result;
}

//
// 常用的迭代器接口
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class iter>
void MTypeWrap< C<T, A>, AllocatorType>::assign(iter __First, iter __Last){
	clear();
	for (; __First != __Last; ++__First){
		T val;
		mj::copyvalue(val, *__First);
		mVal->mVal.push_back(val);
	}
}

//
// 追加元素
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::push_back(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val)
{
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	mVal->mVal.push_back(val);
	return *this;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::operator<<(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val)
{
	return push_back(val);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::append(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val)
{
	return push_back(val);
}

//
// 追加一个容器
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::push_back(const MTypeWrap< C<T, A>, AllocatorType>& other)
{
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	std::copy(other.mVal->mVal.begin(), other.mVal->mVal.end(),
		std::back_inserter(mVal->mVal));
	return *this;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::operator<<(const MTypeWrap< C<T, A>, AllocatorType>& other)
{
	return push_back(other);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::append(const MTypeWrap< C<T, A>, AllocatorType>& other){
	return push_back(other);
}


//
// 不安全追加
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::unsafe_push_back(const_value_reference_type val){
	mVal->mVal.push_back(val);
	return *this;
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::unsafe_push_back(const MTypeWrap< C<T, A>, AllocatorType>& other){
	std::copy(other.mVal->mVal.begin(), other.mVal->mVal.end(),
		std::back_inserter(mVal->mVal));
	return *this;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::unsafe_push_back(const std::initializer_list<T>& other){
	std::copy(other.begin(), other.end(),
		std::back_inserter(mVal->mVal));
	return *this;
}

//
// 往前追加
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::push_front(const_value_reference_type val)
{
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	mVal->mVal.insert(this->mVal->mVal.begin(), val);
	return *this;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::push_front(const MTypeWrap< C<T, A>, AllocatorType>& other)
{
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	C<T, A> temp = other;
	std::copy(mVal->mVal.begin(), mVal->mVal.end(),
		std::back_inserter(temp));
	mVal->mVal = temp;
	return *this;
}

//
// 填充元素
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::fill(std::size_t num, typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val)
{
	resize(num);
	std::fill(mVal->mVal.begin(), mVal->mVal.end(), val);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::fill(typename MTypeWrap< C<T, A>, AllocatorType>::iterator pos1,
	typename MTypeWrap< C<T, A>, AllocatorType>::iterator pos2,
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val)
{
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	auto beg1 = mVal->mVal.begin();
	std::advance(beg1, pos1.CurrentIndex());
	std::size_t num = pos2.CurrentIndex() - pos1.CurrentIndex();
	std::fill_n(beg1, num, val);
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::reference MTypeWrap< C<T, A>, AllocatorType>::operator[](unsigned index){
	return  typename MTypeWrap< C<T, A>, AllocatorType>::Proxiy(this, index);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::const_reference MTypeWrap< C<T, A>, AllocatorType>::operator[](unsigned index) const
{
	return  typename MTypeWrap< C<T, A>, AllocatorType>::Proxiy(const_cast<MTypeWrap< C<T, A>, AllocatorType>*>(this), index);
}


//
// 不安全操作
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
T& MTypeWrap< C<T, A>, AllocatorType>::unsafe_ref(unsigned index){
	return mVal->mVal[index];
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
const T& MTypeWrap< C<T, A>, AllocatorType>::unsafe_ref(unsigned index) const{
	return mVal->mVal[index];
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
T& MTypeWrap< C<T, A>, AllocatorType>::unsafe_at(unsigned index){
	return mVal->mVal.at(index);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
const T& MTypeWrap< C<T, A>, AllocatorType>::unsafe_at(unsigned index) const{
	return mVal->mVal.at(index);
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::reference MTypeWrap< C<T, A>, AllocatorType>::at(unsigned index){
	return  typename MTypeWrap< C<T, A>, AllocatorType>::Proxiy(this, index);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::const_reference MTypeWrap< C<T, A>, AllocatorType>::at(unsigned index) const{
	return  typename MTypeWrap< C<T, A>, AllocatorType>::Proxiy(const_cast<MTypeWrap< C<T, A>, AllocatorType>*>(this), index);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::reference MTypeWrap< C<T, A>, AllocatorType>::operator()(unsigned index){
	return  typename MTypeWrap< C<T, A>, AllocatorType>::Proxiy(this, index);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::const_reference MTypeWrap< C<T, A>, AllocatorType>::operator()(unsigned index) const{
	return  typename MTypeWrap< C<T, A>, AllocatorType>::Proxiy(const_cast<MTypeWrap< C<T, A>, AllocatorType>*>(this), index);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::reference MTypeWrap< C<T, A>, AllocatorType>::back(){
	if (mVal->mVal.empty()){
		throw std::out_of_range("container is empty");
	}
	return  MTypeWrap< C<T, A>, AllocatorType>::Proxiy(this, mVal->mVal.size() - 1);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::const_reference MTypeWrap< C<T, A>, AllocatorType>::back() const{
	if (mVal->mVal.empty()){
		throw std::out_of_range("container is empty");
	}
	return  typename MTypeWrap< C<T, A>, AllocatorType>::Proxiy(this, mVal->mVal.size() - 1);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::reference MTypeWrap< C<T, A>, AllocatorType>::front(){
	if (mVal->mVal.empty()){
		throw std::out_of_range("container is empty");
	}
	return  typename MTypeWrap< C<T, A>, AllocatorType>::Proxiy(this, 0);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::const_reference MTypeWrap< C<T, A>, AllocatorType>::front() const{
	if (mVal->mVal.empty()){
		throw std::out_of_range("container is empty");
	}
	return  MTypeWrap< C<T, A>, AllocatorType>::Proxiy(this, 0);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::iterator MTypeWrap< C<T, A>, AllocatorType>::begin(){
	return MTypeWrap< C<T, A>, AllocatorType>::iterator(*this, mVal->mVal.size());
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::const_iterator MTypeWrap< C<T, A>, AllocatorType>::begin() const{
	return MTypeWrap< C<T, A>, AllocatorType>::const_iterator(const_cast<MTypeWrap< C<T, A>, AllocatorType>&>(*this), mVal->mVal.size());
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::iterator MTypeWrap< C<T, A>, AllocatorType>::end(){
	return MTypeWrap< C<T, A>, AllocatorType>::iterator(*this, mVal->mVal.size(), mVal->mVal.size());
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::const_iterator MTypeWrap< C<T, A>, AllocatorType>::end() const{
	return MTypeWrap< C<T, A>, AllocatorType>::const_iterator(const_cast<MTypeWrap< C<T, A>, AllocatorType>&>(*this), mVal->mVal.size(), mVal->mVal.size());
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::iterator MTypeWrap< C<T, A>, AllocatorType>::find(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val)
{
	auto it = std::find(mVal->mVal.begin(), mVal->mVal.end(), val);
	auto beg = mVal->mVal.begin();
	int num = std::distance(beg, it);
	return MTypeWrap< C<T, A>, AllocatorType>::iterator(*this, mVal->mVal.size(), num);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::const_iterator MTypeWrap< C<T, A>, AllocatorType>::find(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val) const
{
	auto it = std::find(mVal->mVal.begin(), mVal->mVal.end(), val);
	auto beg = mVal->mVal.begin();
	int num = std::distance(beg, it);
	return MTypeWrap< C<T, A>, AllocatorType>::const_iterator(const_cast<MTypeWrap< C<T, A>, AllocatorType>&>(*this), mVal->mVal.size(), num);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class Fun>
typename MTypeWrap< C<T, A>, AllocatorType>::iterator MTypeWrap< C<T, A>, AllocatorType>::find_if(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val, Fun prd)
{
	auto it = std::find_if(mVal->mVal.begin(), mVal->mVal.end(), std::bind(prd, val, std::_Ph<1>()));
	auto beg = mVal->mVal.begin();
	int num = std::distance(beg, it);
	return MTypeWrap< C<T, A>, AllocatorType>::iterator(*this, mVal->mVal.size(), num);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class Fun>
typename MTypeWrap< C<T, A>, AllocatorType>::const_iterator MTypeWrap< C<T, A>, AllocatorType>::find_if(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val, Fun prd) const
{
	auto it = std::find_if(mVal->mVal.begin(), mVal->mVal.end(), std::bind(prd, val, std::_Ph<1>()));
	auto beg = mVal->mVal.begin();
	int num = std::distance(beg, it);
	return MTypeWrap< C<T, A>, AllocatorType>::const_iterator(const_cast<MTypeWrap< C<T, A>, AllocatorType>&>(*this), mVal->mVal.size(), num);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class Fun>
unsigned MTypeWrap< C<T, A>, AllocatorType>::find_count(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val,
	Fun prd) const
{
	unsigned res_count = 0;
	auto it = std::find_if(mVal->mVal.begin(), mVal->mVal.end(), std::bind(prd, val, std::_Ph<1>()));
	while (it != mVal->mVal.end()){
		++res_count;
		it = std::find_if(++it, mVal->mVal.end(), std::bind(prd, val, std::_Ph<1>()));
	}
	return res_count;
}

//
// 排序的事就交给外部来排了
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
std::pair<unsigned, unsigned> MTypeWrap< C<T, A>, AllocatorType>::find_range(
	typename  MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type minval,
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type maxval) const
{
	unsigned firstindex = -1;
	unsigned lastindex = -1;
	for (int i = 0; i < mVal->mVal.size() - 1; ++i){
		if (firstindex == -1){
			if (i == 0){
				if (minval <= mVal->mVal[i]){
					firstindex = 0;
					continue;
				}
			}
			if (mVal->mVal[i] < minval && mVal->mVal[i + 1] >= minval){
				firstindex = i;
			}
		}

		if (maxval >= mVal->mVal[mVal->mVal.size() - 1]){
			maxval = lastindex = this->size();
			break;
		}

		if (mVal->mVal[i] <= maxval && mVal->mVal[i + 1] > maxval){

			lastindex = i;
			break;
		}
	}
	return std::make_pair(firstindex, lastindex);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class Fun>
unsigned MTypeWrap< C<T, A>, AllocatorType>::find_lowindex(typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val, Fun prd) const
{
	unsigned index = -1;
	for (int i = 0; i < mVal->mVal.size(); ++i){
		if (prd(mVal->mVal[i], val)){
			index = i;
			break;
		}
	}
	return index;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class Fun>
unsigned MTypeWrap< C<T, A>, AllocatorType>::find_upindex(typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val, Fun prd) const{
	unsigned index = -1;
	for (int i = mVal->mVal.size() - 1; i >= 0; --i){
		if (prd(mVal->mVal[i], val)){
			index = i;
			break;
		}
	}
	return index;
}

//
// 查找索引
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
std::size_t MTypeWrap< C<T, A>, AllocatorType>::find_index(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val, cmp prd) const
{
	auto it = std::find_if(mVal->mVal.begin(), mVal->mVal.end(), std::bind(prd, val, std::_Ph<1>()));
	if (it != mVal->mVal.end()){
		return std::distance(mVal->mVal.begin(), it);
	}
	else{
		return -1;
	}
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::find_index(const MTypeWrap< C<T, A>, AllocatorType>& vals, cmp prd) const
{
	MTypeWrap< C<T, A>, AllocatorType> indexs;
	for (auto& val : vals.mVal->mVal){
		indexs.push_back(find_index(val, prd));
	}
	return indexs;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::find_index(cmp fun) const
{
	MTypeWrap< C<T, A>, AllocatorType> indexs;
	int index = 0;
	for (auto& __val : mVal->mVal){
		if (fun(__val)){
			indexs.push_back(index);
		}
		++index;
	}
	return indexs;
}

//
// 获取一个交集
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::intersection(const MTypeWrap< C<T, A>, AllocatorType>& other, cmp prd) const
{
	std::vector<T> out;
	std::set_intersection(
		this->begin(),
		this->end(),
		other.begin(),
		other.end(),
		std::back_inserter(out)
		);
	MTypeWrap< C<T, A>, AllocatorType> outwrap;
	outwrap.from_other(out);
	return outwrap;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<template<class> class C2, class cmp>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::intersection(const C2<T>& other, cmp prd) const
{
	std::vector<T> out;
	std::set_intersection(
		this->mVal->mVal.begin(),
		this->mVal->mVal.end(),
		other.begin(),
		other.end(),
		std::back_inserter(out)
		);
	MTypeWrap< C<T, A>, AllocatorType> outwrap;
	outwrap.from_other(out);
	return outwrap;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<template<class, class> class C2, class cmp>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::intersection(const C2<T, A>& other, cmp prd) const
{
	std::vector<T> out;
	std::set_intersection(
		this->mVal->mVal.begin(),
		this->mVal->mVal.end(),
		other.begin(),
		other.end(),
		std::back_inserter(out)
		);
	MTypeWrap< C<T, A>, AllocatorType> outwrap;
	outwrap.from_other(out);
	return outwrap;
}

//
// 检查是否包含
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
bool MTypeWrap< C<T, A>, AllocatorType>::include(const MTypeWrap< C<T, A>, AllocatorType>& other, cmp prd) const
{
	return std::includes(
		this->mVal->mVal.begin(),
		this->mVal->mVal.end(),
		other.mVal->mVal.begin(),
		other.mVal->mVal.end()
		);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<template<class> class C2, class cmp>
bool MTypeWrap< C<T, A>, AllocatorType>::include(const C2<T>& other, cmp prd) const
{
	return std::includes(
		this->mVal->mVal.begin(),
		this->mVal->mVal.end(),
		other.begin(),
		other.end()
		);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<template<class, class> class C2, class cmp>
bool MTypeWrap< C<T, A>, AllocatorType>::include(const C2<T, A>& other, cmp prd) const
{
	return std::includes(this->mVal->mVal.begin(), this->mVal->mVal.end(), other.begin(), other.end());
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::complement(const MTypeWrap< C<T, A>, AllocatorType>& other,cmp prd) const
{
    MTypeWrap< C<T, A>, AllocatorType> out;
    if(this->empty())
        return out;
    if(other.empty()){
        return *this;
    }
    std::vector<double> vs;
	auto _First1 = mVal->mVal.begin();
	auto _Last1 = mVal->mVal.end();

	auto _First2 = other.mVal->mVal.begin();
	auto _Last2 = other.mVal->mVal.end();

	for (; _First1 != _Last1 && _First2 != _Last2;){
		if (prd(*_First1, *_First2)){
			++_First1;
			++_First2;
		}
		else{
			if (*_First1 < *_First2){
				vs.push_back(*_First1);
				++_First1;
			}
			else if (*_First1 > *_First2){
				++_First2;
			}
		}
	}

	for (; _First1 != _Last1;){
		vs.push_back(*_First1++);
	}
	
    out.from_other(vs);
    return out;
}

//
// 检查两个容器是否相等
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
bool MTypeWrap< C<T, A>, AllocatorType>::equal(const MTypeWrap< C<T, A>, AllocatorType>& other, cmp prd) const
{
	if (this->size() != other.size())
		return false;

	for (int i = 0; i < this->size(); ++i){
		if (!prd((*this)(i), other(i))){
			return false;
		}
	}
	return true;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<template<class> class C2, class cmp>
bool MTypeWrap< C<T, A>, AllocatorType>::equal(const C2<T>& other, cmp __Prd) const
{
	if (this->size() != other.size())
		return false;
	auto beg = other.begin();
	for (int i = 0; i < this->size(); ++i){
		if (!prd((*this)(i), *(beg++))){
			return false;
		}
	}
	return true;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<template<class, class> class C2, class cmp>
bool MTypeWrap< C<T, A>, AllocatorType>::equal(const C2<T, A>& other, cmp __Prd) const
{
	if (this->size() != other.size())
		return false;
	auto beg = other.begin();
	for (int i = 0; i < this->size(); ++i){
		if (!prd((*this)(i), *(beg++))){
			return false;
		}
	}
	return true;
}

//
// 小于
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
bool MTypeWrap< C<T, A>, AllocatorType>::less_than(const MTypeWrap< C<T, A>, AllocatorType>& other) const
{
	if (this->size() < other.size()){
		return true;
	}
	else if (this->size() > other.size()){
		return false;
	}

	for (int i = 0; i < this->size(); ++i){
		if ((*this)(i) >= other(i)){
			return false;
		}
	}
	return true;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<template<class> class C2>
bool MTypeWrap< C<T, A>, AllocatorType>::less_than(const C2<T>& other) const
{
	if (this->size() < other.size()){
		return true;
	}
	else if (this->size() > other.size()){
		return false;
	}

	auto beg = other.begin();
	for (int i = 0; i < this->size(); ++i){
		if ((*this)(i) >= *(beg++)){
			return false;
		}
	}
	return true;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<template<class, class> class C2>
bool MTypeWrap< C<T, A>, AllocatorType>::less_than(const C2<T, A>& other) const
{
	if (this->size() < other.size()){
		return true;
	}
	else if (this->size() > other.size()){
		return false;
	}

	auto beg = other.begin();
	for (int i = 0; i < this->size(); ++i){
		if ((*this)(i) >= *(beg++)){
			return false;
		}
	}
	return true;
}

//
// 大于
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
bool MTypeWrap< C<T, A>, AllocatorType>::greater_than(const MTypeWrap< C<T, A>, AllocatorType>& other) const
{
	if (this->size() > other.size()){
		return true;
	}
	else if (this->size() < other.size()){
		return false;
	}

	for (int i = 0; i < this->size(); ++i){
		if ((*this)(i) <= other(i)){
			return false;
		}
	}
	return true;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<template<class> class C2>
bool MTypeWrap< C<T, A>, AllocatorType>::greater_than(const C2<T>& other) const
{
	if (this->size() > other.size()){
		return true;
	}
	else if (this->size() < other.size()){
		return false;
	}

	auto beg = other.begin();
	for (int i = 0; i < this->size(); ++i){
		if ((*this)(i) <= *(beg++)){
			return false;
		}
	}
	return true;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<template<class, class> class C2>
bool MTypeWrap< C<T, A>, AllocatorType>::greater_than(const C2<T, A>& other) const
{
	if (this->size() > other.size()){
		return true;
	}
	else if (this->size() < other.size()){
		return false;
	}

	auto beg = other.begin();
	for (int i = 0; i < this->size(); ++i){
		if ((*this)(i) <= *(beg++)){
			return false;
		}
	}
	return true;

}

//
// 小于等于
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
bool MTypeWrap< C<T, A>, AllocatorType>::less_equal(const MTypeWrap< C<T, A>, AllocatorType>& other) const
{
	if (this->size() < other.size()){
		return true;
	}
	else if (this->size() > other.size()){
		return false;
	}

	for (int i = 0; i < this->size(); ++i){
		if ((*this)(i) > other(i)){
			return false;
		}
	}
	return true;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<template<class> class C2>
bool MTypeWrap< C<T, A>, AllocatorType>::less_equal(const C2<T>& other) const
{
	if (this->size() < other.size()){
		return true;
	}
	else if (this->size() > other.size()){
		return false;
	}

	auto beg = other.begin();
	for (int i = 0; i < this->size(); ++i){
		if ((*this)(i) > *(beg++)){
			return false;
		}
	}
	return true;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<template<class, class> class C2>
bool MTypeWrap< C<T, A>, AllocatorType>::less_equal(const C2<T, A>& other) const
{
	if (this->size() < other.size()){
		return true;
	}
	else if (this->size() > other.size()){
		return false;
	}

	auto beg = other.begin();
	for (int i = 0; i < this->size(); ++i){
		if ((*this)(i) > *(beg++)){
			return false;
		}
	}
	return true;
}

//
// 大于等于
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
bool MTypeWrap< C<T, A>, AllocatorType>::greater_equal(const MTypeWrap< C<T, A>, AllocatorType>& other) const
{
	if (this->size() > other.size()){
		return true;
	}
	else if (this->size() < other.size()){
		return false;
	}

	for (int i = 0; i < this->size(); ++i){
		if ((*this)(i) < other(i)){
			return false;
		}
	}
	return true;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<template<class> class C2>
bool MTypeWrap< C<T, A>, AllocatorType>::greater_equal(const C2<T>& other) const
{
	if (this->size() > other.size()){
		return true;
	}
	else if (this->size() < other.size()){
		return false;
	}

	auto beg = other.begin();
	for (int i = 0; i < this->size(); ++i){
		if ((*this)(i) < *(beg++)){
			return false;
		}
	}
	return true;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<template<class, class> class C2>
bool MTypeWrap< C<T, A>, AllocatorType>::greater_equal(const C2<T, A>& other) const
{
	if (this->size() > other.size()){
		return true;
	}
	else if (this->size() < other.size()){
		return false;
	}

	auto beg = other.begin();
	for (int i = 0; i < this->size(); ++i){
		if ((*this)(i) < *(beg++)){
			return false;
		}
	}
	return true;
}

//
// 映射一个区间
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::map(unsigned lowindex, unsigned upindex) const{
	MTypeWrap< C<T, A>, AllocatorType> res;
	for (int i = lowindex; i < upindex; ++i){
		res.push_back(this->at(i));
	}
	return res;
}

//
// 复制一段数据出来
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::copy(int first, int last) const{
	int num = last - first;
	MTypeWrap< C<T, A>, AllocatorType> res;
	if (num <= 0 || this->size() < num || last > this->size() || first < 0){
		return res;
	}
	res.resize(num);
	for (int i = first; i < last; ++i){
		res.unsafe_ref(i - first) = this->mVal->mVal[i];
	}
	return res;
}

//
// 检查是否包含某个值
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
bool MTypeWrap< C<T, A>, AllocatorType>::contains(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val, cmp prd) const
{
	auto it = std::find_if(mVal->mVal.begin(), mVal->mVal.end(), std::bind(prd, val, std::_Ph<1>()));
	if (it != mVal->mVal.end()){
		return true;
	}
	else{
		return false;
	}
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class T1, class A1, template<class T2, class A2> class C1, class cmp>
C1<T1, A1>& MTypeWrap< C<T, A>, AllocatorType>::search(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val, C1<T1, A1>& out, cmp prd) const
{
	out.clear();
	for (auto& v : mVal->mVal){
		if (prd(v, val)){
			T1 outval;
			mj::copyvalue(outval, v);
			out.push_back(outval);
		}
	}
	return out;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class T1, template<class T2> class C1, class cmp>
C1<T1>& MTypeWrap< C<T, A>, AllocatorType>::search(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val, C1<T1>& out, cmp prd) const
{
	out.clear();
	for (auto& v : mVal->mVal){
		if (prd(v, val)){
			T1 outval;
			mj::copyvalue(outval, v);
			out.push_back(outval);
		}
	}
	return out;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
bool MTypeWrap< C<T, A>, AllocatorType>::near_search(typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val, T& out)
{
	if (this->empty()){
		return false;
	}
	MTypeWrap< C<T, A>, AllocatorType> temp = *this;
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
		else if (temp[midindex] > val){
			temp.erase(temp.begin() + midindex + 1, temp.end());
		}
	}
	return false;
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
bool MTypeWrap< C<T, A>, AllocatorType>::near_search(typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val, T& out) const
{
	return const_cast<MTypeWrap< C<T, A>, AllocatorType>*>(this)->near_search(val, out);
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
bool MTypeWrap< C<T, A>, AllocatorType>::near_search_left(typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val, T& out)
{
	if (this->empty()){
		return false;
	}
	MTypeWrap< C<T, A>, AllocatorType> temp = *this;
	temp.sort();
	if (val < temp.front() && temp.size() == 1){
		out = temp.front();
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
			out = temp[midindex];
			return true;
		}
		else if (temp[midindex] > val){
			temp.erase(temp.begin() + midindex + 1, temp.end());
		}
	}
	return false;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
bool MTypeWrap< C<T, A>, AllocatorType>::near_search_left(typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val, T& out) const
{
	return const_cast<MTypeWrap< C<T, A>, AllocatorType>*>(this)->near_search_left(val, out);
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
bool MTypeWrap< C<T, A>, AllocatorType>::near_search_right(typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val, T& out)
{
	if (this->empty()){
		return false;
	}
	MTypeWrap< C<T, A>, AllocatorType> temp = *this;
	temp.sort();
	if (val < temp.front()){
		out = temp[0];
		return true;
	}

	if (val >= temp.back()){
		return false;
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
			out = temp[midindex + 1];
			return true;
		}
		else if (temp[midindex] > val){
			temp.erase(temp.begin() + midindex + 1, temp.end());
		}
	}
	return false;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
bool MTypeWrap< C<T, A>, AllocatorType>::near_search_right(typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val, T& out) const
{
	return const_cast<MTypeWrap< C<T, A>, AllocatorType>*>(this)->near_search_right(val, out);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::iterator MTypeWrap< C<T, A>, AllocatorType>::last(){
	if (mVal->mVal.size() == 0){
		throw std::out_of_range("container is empty");
	}
	MTypeWrap< C<T, A>, AllocatorType>::iterator cit(*this, mVal->mVal.size(), mVal->mVal.size());
	--cit;
	return cit;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::const_iterator MTypeWrap< C<T, A>, AllocatorType>::last() const{
	if (mVal->mVal.size() == 0){
		throw std::out_of_range("container is empty");
	}
	MTypeWrap< C<T, A>, AllocatorType>::const_iterator cit(*this, mVal->mVal.size(), mVal->mVal.size());
	--cit;
	return cit;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::insert(iterator pos, const_value_reference_type val){
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}

	MTypeWrap< C<T, A>, AllocatorType>::iterator beg(*this, mVal->mVal.size());
	int num = pos - beg;
	auto __beg = mVal->mVal.begin();
	std::advance(__beg, num);
	mVal->mVal.insert(__beg, val);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::insert(iterator pos, std::size_t count,
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val)
{
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	MTypeWrap< C<T, A>, AllocatorType>::iterator beg(*this, mVal->mVal.size());
	int num = pos - beg;
	auto __beg = mVal->mVal.begin();
	std::advance(__beg, num);
	mVal->mVal.insert(__beg, count, val);
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class iter>
void MTypeWrap< C<T, A>, AllocatorType>::insert(iterator pos, iter __first, iter __last){
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	MTypeWrap< C<T, A>, AllocatorType>::iterator beg(*this, mVal->mVal.size());
	int num = pos - beg;
	auto __beg = mVal->mVal.begin();
	std::advance(__beg, num);
	mVal->mVal.insert(__beg, __first, __last);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::iterator MTypeWrap< C<T, A>, AllocatorType>::erase(typename MTypeWrap< C<T, A>, AllocatorType>::iterator pos){
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	MTypeWrap< C<T, A>, AllocatorType>::iterator beg(*this, mVal->mVal.size());
	int num = pos - beg;
	auto __beg = mVal->mVal.begin();
	std::advance(__beg, num);
	mVal->mVal.erase(__beg);
	return MTypeWrap< C<T, A>, AllocatorType>::iterator(*this, mVal->mVal.size(), num);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::erase(typename MTypeWrap< C<T, A>, AllocatorType>::iterator pos1,
	typename MTypeWrap< C<T, A>, AllocatorType>::iterator pos2)
{
	if (mVal->IsShared() && mVal->Count() > 1){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	MTypeWrap< C<T, A>, AllocatorType>::iterator beg(*this, mVal->mVal.size());
	int num = pos1 - beg;
	int num2 = pos2 - beg;
	auto __beg = mVal->mVal.begin();
	std::advance(__beg, num);
	auto __beg2 = mVal->mVal.begin();
	std::advance(__beg2, num2);
	mVal->mVal.erase(__beg, __beg2);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
void MTypeWrap< C<T, A>, AllocatorType>::erase(typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type  val, cmp prd){
	remove(val, prd);
}


//
// 插入元素 索引版本
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::insert_index(int pos, typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val){
	auto begit = begin();
	begit += pos;
	insert(begit, val);
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::insert_index(int pos, std::size_t count, typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val){
	auto begit = begin();
	begit += pos;
	insert(begit, count, val);
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class iter>
void MTypeWrap< C<T, A>, AllocatorType>::insert_index(int pos, iter __first, iter __last){
	auto begit = begin();
	begit += pos;
	insert(begit, __first, __last);
}

//
// 删除元素 索引版本
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::erase_index(int pos){
	auto begit = begin();
	begit += pos;
	erase(begit);
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::erase_index(int pos1, int pos2){
	auto begit = begin();
	auto endit = begin();
	begit += pos1;
	endit += pos2;
	erase(begit, endit);
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::pop_back(){
	if (mVal->IsShared() && mVal->Count() > 1){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	mVal->mVal.pop_back();
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::pop_front(){
	if (mVal->IsShared() && mVal->Count() > 1){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	mVal->mVal.erase(mVal->mVal.begin());
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template < class T1, class A1, template < class T2, class A2> class C1>
C1<T1, A1>& MTypeWrap< C<T, A>, AllocatorType>::convert_type(C1<T1, A1>& out){
	out.clear();
	for (auto& v : mVal->mVal){
		typename C1<T1, A1>::value_type val;
		mj::copyvalue(val, v);
		out.push_back(val);
	}
	return out;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template < class T1, template < class T2> class C1>
C1<T1>& MTypeWrap< C<T, A>, AllocatorType>::convert_type(C1<T1>& out){
	out.clear();
	for (auto& v : mVal->mVal){
		T1 val;
		mj::copyvalue(val, v);
		out.push_back(val);
	}
	return out;
}

//
// 带信息的打印
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::print(const char* msg, std::ostream& os) const{
	os << msg << *this << std::endl;;
}

//
// 将数据写进流获取是从流中读取
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::ostream(std::ostream& os, const char* spliter){
	if (mVal->mVal.empty())
		return;
	os << "Number : " << size() << std::endl;
	std::copy(mVal->mVal.begin(), mVal->mVal.end(), std::ostream_iterator<T>(os, spliter));
	os << std::endl;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::istream(std::istream& is){
	std::string str;
	while (!is.eof() && std::getline(is, str)){
		if (str.empty()){
			continue;
		}
		mj::MString mstr;
		mstr = str;
		if (mstr.istart_with("Number")){
			int num = 0;
			bool isok = mstr.reg_find("(\\d+)", num);
			if (isok == false)
				return;
			if (mVal->IsShared()){
				mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
			}
			std::getline(is, str);
			mstr = str;
			mstr.split(" \t,;、", mVal->mVal);
			if (mVal->mVal.size() < num){
				int len = mVal->mVal.size();
				for (int i = 0; i < num - len; ++i){
					mVal->mVal.push_back(T());
				}
			}
			return;
		}
	}
}

//
// 将数据写进文件或者是从文件中读取
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::save(const char* filename, const char* spliter){
	if (mVal->mVal.empty())
		return;
	std::ofstream out(filename);
	if (out.fail()){
		return;
	}
	ostream(out, spliter);
	out.close();
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::load(const char* filename){
	std::ifstream in(filename);
	if (in.fail()){
		return;
	}
	istream(in);
	in.close();
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
std::size_t MTypeWrap< C<T, A>, AllocatorType>::size(){
	return mVal->mVal.size();
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
std::size_t MTypeWrap< C<T, A>, AllocatorType>::size() const{
	return mVal->mVal.size();
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::resize(std::size_t size){
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	mVal->mVal.resize(size);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::reserve(std::size_t size){
	mVal->mVal.reserve(size);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::clear(){
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	mVal->mVal.clear();
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
bool MTypeWrap< C<T, A>, AllocatorType>::empty() const{
	return mVal->mVal.empty();
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
void MTypeWrap< C<T, A>, AllocatorType>::sort(cmp prd){
	if (mVal->IsShared() && mVal->Count() > 1){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	std::sort(mVal->mVal.begin(), mVal->mVal.end(), prd);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
void MTypeWrap< C<T, A>, AllocatorType>::remove(typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val, cmp prd){
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	mVal->mVal.erase(std::remove_if(mVal->mVal.begin(), mVal->mVal.end(), std::bind(prd, val, std::_Ph<1>())),
		mVal->mVal.end());
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp, class cmp2>
void MTypeWrap< C<T, A>, AllocatorType>::unique(cmp prd, cmp2 prd2){
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	std::sort(mVal->mVal.begin(), mVal->mVal.end(), prd2);
	mVal->mVal.erase(std::unique(mVal->mVal.begin(), mVal->mVal.end(), prd), mVal->mVal.end());
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
void MTypeWrap< C<T, A>, AllocatorType>::sort(typename MTypeWrap< C<T, A>, AllocatorType>::iterator first,
	typename MTypeWrap< C<T, A>, AllocatorType>::iterator last, cmp prd)
{
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	auto beg1 = mVal->mVal.begin();
	std::advance(beg1, first.CurrentIndex());
	auto beg2 = mVal->mVal.begin();
	std::advance(beg2, last.CurrentIndex());
	std::sort(beg1, beg2, prd);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
void MTypeWrap< C<T, A>, AllocatorType>::remove(typename MTypeWrap< C<T, A>, AllocatorType>::iterator first,
	typename MTypeWrap< C<T, A>, AllocatorType>::iterator last,
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val, cmp prd)
{
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	auto beg1 = mVal->mVal.begin();
	std::advance(beg1, first.CurrentIndex());
	auto beg2 = mVal->mVal.begin();
	std::advance(beg2, last.CurrentIndex());
	auto beg3 = std::remove_if(beg1, beg2, std::bind(prd, val, std::_Ph<1>()));
	auto beg4 = mVal->mVal.begin();
	std::advance(beg4, last.CurrentIndex());
	mVal->mVal.erase(beg3, beg4);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
void MTypeWrap< C<T, A>, AllocatorType>::unique(typename MTypeWrap< C<T, A>, AllocatorType>::iterator first,
	typename MTypeWrap< C<T, A>, AllocatorType>::iterator last, cmp prd)
{
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	auto beg1 = mVal->mVal.begin();
	std::advance(beg1, first.CurrentIndex());
	auto beg2 = mVal->mVal.begin();
	std::advance(beg2, last.CurrentIndex());
	std::sort(beg1, beg2);
	beg1 = mVal->mVal.begin();
	std::advance(beg1, first.CurrentIndex());
	beg2 = mVal->mVal.begin();
	std::advance(beg2, last.CurrentIndex());
	auto beg3 = std::unique(beg1, beg2, prd);
	auto beg4 = mVal->mVal.begin();
	std::advance(beg4, last.CurrentIndex());
	mVal->mVal.erase(beg3, beg4);
}

//
// 翻转元素
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::reverse(){
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	std::reverse(mVal->mVal.begin(), mVal->mVal.end());
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
void MTypeWrap< C<T, A>, AllocatorType>::reverse(typename MTypeWrap< C<T, A>, AllocatorType>::iterator first,
	typename MTypeWrap< C<T, A>, AllocatorType>::iterator last)
{
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	auto beg1 = mVal->mVal.begin();
	std::advance(beg1, first.CurrentIndex());
	auto beg2 = mVal->mVal.begin();
	std::advance(beg2, last.CurrentIndex());
	std::reverse(beg1, beg2);
}

//
// 计算最大值
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
T MTypeWrap< C<T, A>, AllocatorType>::max_element() const
{
	if (mVal->mVal.empty())
		return T();
	return *(std::max_element(mVal->mVal.begin(), mVal->mVal.end()));
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
T MTypeWrap< C<T, A>, AllocatorType>::max_element(cmp prd) const{
	if (mVal->mVal.empty())
		return T();
	return *(std::max_element(mVal->mVal.begin(), mVal->mVal.end(), prd));
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
T MTypeWrap< C<T, A>, AllocatorType>::max_element(typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type excepty, cmp prd) const{
	std::vector<T> vals;
	this->convert_type(vals);
	vals.erase(std::remove_if(vals.begin(), vals.end(), [&](T v){return prd(v, excepty); }));
	return *(std::max_element(vals.begin(), vals.end()));
}

//
// 计算最小值
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
T MTypeWrap< C<T, A>, AllocatorType>::min_element() const{
	if (mVal->mVal.empty())
		return T();
	return *(std::min_element(mVal->mVal.begin(), mVal->mVal.end()));
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
T MTypeWrap< C<T, A>, AllocatorType>::min_element(cmp prd) const{
	if (mVal->mVal.empty())
		return T();
	return *(std::min_element(mVal->mVal.begin(), mVal->mVal.end(), prd));
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
T MTypeWrap< C<T, A>, AllocatorType>::min_element(typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type excepty, cmp prd) const{
	std::vector<T> vals;
	this->convert_type(vals);
	if (vals.empty())
		return T();

	vals.erase(std::remove_if(vals.begin(), vals.end(), [&](T v){return prd(v, excepty); }));
	return *(std::max_element(vals.begin(), vals.end()));
}

//
// 获取迭代器信息
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::const_iterator MTypeWrap< C<T, A>, AllocatorType>::max_iterator() const
{
	return std::max_element(begin(), end());
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
typename MTypeWrap< C<T, A>, AllocatorType>::const_iterator MTypeWrap< C<T, A>, AllocatorType>::max_iterator(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type excepty, cmp prd) const
{
	std::vector<T> vals;
	this->convert_type(vals);
	vals.erase(std::remove_if(vals.begin(), vals.end(), [&](T v){return prd(v, excepty); }));
	auto it = std::max_element(vals.begin(), vals.end());
	int num = std::distance(vals.begin(), it);
	return const_iterator(*this, size(), num);
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::const_iterator MTypeWrap< C<T, A>, AllocatorType>::min_iterator() const
{
	return std::min_element(begin(), end());
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
typename MTypeWrap< C<T, A>, AllocatorType>::const_iterator MTypeWrap< C<T, A>, AllocatorType>::min_iterator(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type excepty, cmp prd) const
{
	std::vector<T> vals;
	this->convert_type(vals);
	vals.erase(std::remove_if(vals.begin(), vals.end(), [&](T v){return prd(v, excepty); }));
	auto it = std::min_element(vals.begin(), vals.end());
	int num = std::distance(vals.begin(), it);
	return const_iterator(*this, size(), num);
}

//
// 计算平均值
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
T MTypeWrap< C<T, A>, AllocatorType>::aval_value() const{
	T sum = std::accumulate(mVal->mVal.begin(), mVal->mVal.end(), T(0.0));
	return sum / this->size();
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
T MTypeWrap< C<T, A>, AllocatorType>::aval_value(typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type excepty, cmp prd) const{
	std::vector<T> vals;
	this->convert_type(vals);
	if (vals.empty())
		return T();

	vals.erase(std::remove_if(vals.begin(), vals.end(), [&](T v){return prd(v, excepty); }));
	T sum = std::accumulate(vals.begin(), vals.end(), T(0.0));
	return sum_value(excepty, prd) / vals.size();
}

//
// 计算总和
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
T MTypeWrap< C<T, A>, AllocatorType>::sum_value() const{
	T sum = std::accumulate(mVal->mVal.begin(), mVal->mVal.end(), T(0.0));
	return sum;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
T MTypeWrap< C<T, A>, AllocatorType>::sum_value(typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type excepty, cmp prd) const{
	std::vector<T> vals;
	this->convert_type(vals);
	if (vals.empty())
		return T();

	vals.erase(std::remove_if(vals.begin(), vals.end(), [&](T v){return prd(v, excepty); }));
	T sum = std::accumulate(vals.begin(), vals.end(), T(0.0));
	return sum;
}

//
// 计算最小间距
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
T MTypeWrap< C<T, A>, AllocatorType>::min_span(unsigned step) const{
	if (empty())
		return T();

	T __minspan = INT_MAX;
	auto beg = this->begin();
	auto end = this->end();
	for (; beg != end;){
		auto temp = beg;
		int dis = std::distance(beg, end);
		if (dis < step)
			break;
		std::advance(beg, step);
		if (beg != end){
			T tempv = std::abs(*temp - *beg);
			if (__minspan > tempv){
				__minspan = tempv;
			}
		}
	}
	return __minspan;
}

//
// 计算最大间距
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
T MTypeWrap< C<T, A>, AllocatorType>::max_span(unsigned step) const{
	if (empty())
		return T();

	T __maxspan = INT_MIN;
	auto beg = this->begin();
	auto end = this->end();
	for (; beg != end;){
		auto temp = beg;
		int dis = std::distance(beg, end);
		if (dis < step)
			break;
		std::advance(beg, step);
		if (beg != end){
			T tempv = std::abs(*temp - *beg);
			if (__maxspan < tempv){
				__maxspan = tempv;
			}
		}
	}
	return __maxspan;
}

//
// 间距的系列
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
typename MTypeWrap< C<T, A>, AllocatorType>::Type MTypeWrap< C<T, A>, AllocatorType>::span_array(unsigned step) const{
	typename MTypeWrap< C<T, A>, AllocatorType>::Type out;
	if (empty())
		return out;
	auto beg = this->begin();
	auto end = this->end();
	for (; beg != end;){
		auto temp = beg;
		int dis = std::distance(beg, end);
		if (dis < step)
			break;
		std::advance(beg, step);
		if (beg != end){
			T tempv = std::abs(*temp - *beg);
			out.push_back(tempv);
		}
	}
	return out;
}

//
// 计算最大公约数
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
T MTypeWrap< C<T, A>, AllocatorType>::gcd() const{
	if (empty())
		return T();

	auto fun = [](T x, T y)->T{
		if (x == 1e-10)
			return y;
		if (y == 1e-10)
			return x;
		if (x > y)
			std::swap(x, y);
		double c;
		for (c = std::fmod(x, y); c > 1e-10; c = std::fmod(x, y)){
			x = y;
			y = c;
		}
		return y;
	};
	auto beg = this->begin();
	auto end = this->end();

	T __cd = *(beg);

	for (; beg != end;){
		double val1 = __cd;
		double val2 = *beg;
		__cd = fun(val1, val2);
		++beg;
	}
	return __cd;
}

//
// 计算峰峰值 或者 谷谷值
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
typename MTypeWrap< C<T, A>, AllocatorType>::Type MTypeWrap< C<T, A>, AllocatorType>::p2p(cmp prd) const{
	typename MTypeWrap< C<T, A>, AllocatorType>::Type out;
	if (empty())
		return out;
	if (size() < 2){
		out.push_back(c[0]);
		return;
	}

	int num = size();

	for (int i = 1; i < num - 1; i++){
		if (i == 1){
			if (prd(at(i - 1), at(i))){
				out.push_back(at(0));
			}
		}
		if (i == num - 2){
			if (!prd(at(i), at(i + 1))){
				out.push_back(at(i + 1));
				return;
			}
		}
		if (!prd(at(i - 1), at(i)) && prd(at(i), at(i + 1))){
			out.push_back(at(i));
		}
	}
	return out;
}

//
// 计算标准差
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
T MTypeWrap< C<T, A>, AllocatorType>::standard_deviation() const{
	T val = this->aval_value();
	T sum = std::accumulate(mVal->mVal.begin(), mVal->mVal.end(), T(0.0), [&](T v1, T v){
		return v1 + (v - val)*(v - val);
	});
	T diff = std::sqrt(sum / mVal->mVal.size());
	return diff;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
T MTypeWrap< C<T, A>, AllocatorType>::standard_deviation(double val) const{
	T sum = std::accumulate(mVal->mVal.begin(), mVal->mVal.end(), T(0.0), [&](T v1, T v){
		return v1 + (v - val)*(v - val);
	});
	T diff = std::sqrt(sum / mVal->mVal.size());
	return diff;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
T MTypeWrap< C<T, A>, AllocatorType>::standard_deviation(typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type excepty, cmp prd) const{
	std::vector<T> vals;
	this->convert_type(vals);
	if (vals.empty())
		return T();

	vals.erase(std::remove_if(vals.begin(), vals.end(), [&](T v){return prd(v, excepty); }));
	T sum = std::accumulate(vals.begin(), vals.end(), T(0.0));
	T val = sum / vals.size();
	sum = std::accumulate(vals.begin(), vals.end(), T(0.0), [&](T v1, T v){
		return v1 + (v - val)*(v - val);
	});
	T diff = std::sqrt(sum / vals.size());
	return diff;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
T MTypeWrap< C<T, A>, AllocatorType>::standard_deviation(double val,
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type excepty, cmp prd) const
{
	std::vector<T> vals;
	this->convert_type(vals);
	if (vals.empty())
		return T();

	vals.erase(std::remove_if(vals.begin(), vals.end(), [&](T v){return prd(v, excepty); }));
	T sum = std::accumulate(vals.begin(), vals.end(), T(0.0), [&](T v1, T v){
		return v1 + (v - val)*(v - val);
	});
	T diff = std::sqrt(sum / vals.size());
	return diff;
}

//
// 计算方差
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
T MTypeWrap< C<T, A>, AllocatorType>::square_deviation() const{
	T val = this->aval_value();
	T sum = std::accumulate(mVal->mVal.begin(), mVal->mVal.end(), T(0.0), [&](T v1, T v){
		return v1 + (v - val)*(v - val);
	});
	return sum / mVal->mVal.size();
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
T MTypeWrap< C<T, A>, AllocatorType>::square_deviation(double val) const{
	T sum = std::accumulate(mVal->mVal.begin(), mVal->mVal.end(), T(0.0), [&](double v1, T v){
		return v1 + (v - val)*(v - val);
	});
	return sum / mVal->mVal.size();
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
T MTypeWrap< C<T, A>, AllocatorType>::square_deviation(typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type excepty, cmp prd) const{
	std::vector<T> vals;
	this->convert_type(vals);
	if (vals.empty())
		return T();

	vals.erase(std::remove_if(vals.begin(), vals.end(), [&](T v){return prd(v, excepty); }));
	T sum = std::accumulate(vals.begin(), vals.end(), T(0.0));
	T val = sum / vals.size();
	sum = std::accumulate(vals.begin(), vals.end(), T(0.0), [&](T v1, T v){
		return v1 + (v - val)*(v - val);
	});
	return sum / vals.size();
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
T MTypeWrap< C<T, A>, AllocatorType>::square_deviation(double val,
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type excepty, cmp prd) const
{
	std::vector<T> vals;
	this->convert_type(vals);
	if (vals.empty())
		return T();

	vals.erase(std::remove_if(vals.begin(), vals.end(), [&](T v){return prd(v, excepty); }));
	T sum = std::accumulate(vals.begin(), vals.end(), T(0.0), [&](T v1, T v){
		return v1 + (v - val)*(v - val);
	});
	return sum / vals.size();
}
//
// 按照给定条件计算总和
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class Fun>
T MTypeWrap< C<T, A>, AllocatorType>::accumulate(typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val, Fun fun) const{
	T sum = std::accumulate(mVal->mVal.begin(), mVal->mVal.end(), val, fun);
	return sum;
}

//
// 调整数据
// 例如将数据调整到+-180之间
// adjust(360,-180,180)
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class Less, class Greater, class Equal_to>
void MTypeWrap< C<T, A>, AllocatorType>::adjust(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type adjustvalue,
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type __minvalue,
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type __maxvalue,
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type excepty,
	Less __lessfun,
	Greater __greaterfun,
	Equal_to __equalfun)
{
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	int num = mVal->mVal.size();
	for (int i = 0; i < num; ++i){
		if (__equalfun(at(i), excepty))
			continue;
		if (__lessfun(at(i), __minvalue)){
			T val = at(i);
			val = int(val) % int(adjustvalue);
			val = val - int((val + 0.5*std::abs(std::abs(__maxvalue) - std::abs(__minvalue))) / (0.5*adjustvalue))*adjustvalue;
			mVal->updatevalue(i, val);
		}
		else if (__greaterfun(at(i), __maxvalue)){
			T val = at(i);
			val = int(val) % int(adjustvalue);
			val = val - int((val + 0.5*std::abs(std::abs(__maxvalue) - std::abs(__minvalue))) / (0.5*adjustvalue))*adjustvalue;
			mVal->updatevalue(i, val);
		}
	}
}


//
// 调整数据，直接将满足某些条件的数据替换为指定数
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
void MTypeWrap< C<T, A>, AllocatorType>::replace(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type replaced,
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type value, cmp prd)
{
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	int num = mVal->mVal.size();
	for (int i = 0; i < num; ++i){
		if (prd(at(i), replaced)){
			mVal->updatevalue(i, value);
		}
	}
}

//
// 所有数据 + 上一个数
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::plus(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val)
{
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	int num = mVal->mVal.size();
	for (int i = 0; i < num; ++i){
		mVal->updatevalue(i, at(i) + val);
	}
	return *this;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::plus(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val,
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type excepty, cmp prd)
{
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	int num = mVal->mVal.size();
	for (int i = 0; i < num; ++i){
		if (__lessfun(at(i), excepty)){
			if (prd(at(i), excepty))
				continue;
			mVal->updatevalue(i, at(i) + val);
		}
	}
	return *this;
}

//
// 所有数据 - 一个数
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::minus(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val)
{
	return plus(-1.0*val);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::minus(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val,
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type excepty,
	cmp prd)
{
	return plus(-1.0*val, excepty, prd);
}

//
// 所有数据 * 一个数
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::mult(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val)
{
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	int num = mVal->mVal.size();
	for (int i = 0; i < num; ++i){
		mVal->updatevalue(i, at(i) * val);
	}
	return *this;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::mult(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val,
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type excepty,
	cmp prd)
{
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	int num = mVal->mVal.size();
	for (int i = 0; i < num; ++i){
		if (__lessfun(at(i), excepty)){
			if (prd(at(i), excepty))
				continue;
			mVal->updatevalue(i, at(i)*val);
		}
	}
	return *this;
}

//
// 所有数 / 一个数
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::division(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val)
{
	return mult(1.0 / val);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class cmp>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::division(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type val,
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type excepty,
	cmp prd)
{
	return mult(1.0 / val, excepty, prd);
}

//
// +-*/ 的操作符重载
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::operator+=(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type other)
{
	return plus(other);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::operator-=(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type other)
{
	return plus(-1.0*other);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::operator*=(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type other)
{
	return mult(other);
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::operator/=(
	typename MTypeWrap< C<T, A>, AllocatorType>::const_value_reference_type other)
{
	return mult(1.0 / other);
}

//
// 一个系列加上一个系列
//
template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::operator+=(const MTypeWrap< C<T, A>, AllocatorType>& other)
{
	if (other.size() != size())
		return *this;
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	for (int i = 0; i < size(); ++i){
                this->at(i).ref() += other.at(i).ref();
	}
	return *this;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::operator-=(const MTypeWrap< C<T, A>, AllocatorType>& other)
{
	if (other.size() != size())
		return *this;
	if (mVal->IsShared()){
		mVal = new MTypeWrap< C<T, A>, AllocatorType>::MContentType(*mVal);
	}
	for (int i = 0; i < size(); ++i){
                this->at(i).ref() -= other.at(i).ref();
	}
	return *this;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::sin() const
{
	MTypeWrap< C<T, A>, AllocatorType> out = this->clone();
	if (out.empty())
		return out;
	for (auto& c : out){
		c = std::sin(c);
	}
	return out;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::cos() const
{
	MTypeWrap< C<T, A>, AllocatorType> out = this->clone();
	if (out.empty())
		return out;
	for (auto& c : out){
		c = std::cos(c);
	}
	return out;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::asin() const
{
	MTypeWrap< C<T, A>, AllocatorType> out = this->clone();
	if (out.empty())
		return out;
	for (auto& c : out){
		c = std::asin(c);
	}
	return out;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::acos() const
{
	MTypeWrap< C<T, A>, AllocatorType> out = this->clone();
	if (out.empty())
		return out;
	for (auto& c : out){
		c = std::acos(c);
	}
	return out;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::log() const
{
	MTypeWrap< C<T, A>, AllocatorType> out = this->clone();
	if (out.empty())
		return out;
	for (auto& c : out){
		c = std::log(c);
	}
	return out;
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::log2() const
{
	MTypeWrap< C<T, A>, AllocatorType> out = this->clone();
	if (out.empty())
		return out;
	for (auto& c : out){
		c = std::log2(c);
	}
	return out;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::log10() const
{
	MTypeWrap< C<T, A>, AllocatorType> out = this->clone();
	if (out.empty())
		return out;
	for (auto& c : out){
		c = std::log10(c);
	}
	return out;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::square() const{
	MTypeWrap< C<T, A>, AllocatorType> out = this->clone();
	if (out.empty())
		return out;
	for (auto& c : out){
		c = std::pow(c, 2);
	}
	return out;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::power(double k) const{
	MTypeWrap< C<T, A>, AllocatorType> out = this->clone();
	if (out.empty())
		return out;
	for (auto& c : out){
		c = std::pow(c,k);
	}
	return out;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::sqrt() const
{
	MTypeWrap< C<T, A>, AllocatorType> out = this->clone();
	if (out.empty())
		return out;
	for (auto& c : out){
		c = std::sqrt(c);
	}
	return out;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::abs() const
{
	MTypeWrap< C<T, A>, AllocatorType> out = this->clone();
	if (out.empty())
		return out;
	for (auto& c : out){
		c = std::abs(c);
	}
	return out;
}

template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class F>
MTypeWrap< C<T, A>, AllocatorType> MTypeWrap< C<T, A>, AllocatorType>::tranform_copy(F fun) const
{
	MTypeWrap< C<T, A>, AllocatorType> out = this->clone();
	if (out.empty())
		return out;
	for (auto& c : out){
		c = fun(c);
	}
	return out;
}


template<class T, class A, template<class T1, class A1> class C, class AllocatorType>
template<class F>
MTypeWrap< C<T, A>, AllocatorType>& MTypeWrap< C<T, A>, AllocatorType>::transform(F fun)
{
	if (this->empty())
		return *this;
	for (auto& c : *this){
		c = fun(c);
	}
	return *this;
}


//
// 操作两个系列
//
template<class T1, class A1, template<class, class> class C1, class AllocatorType1, class Fun>
MTypeWrap<C1<T1, A1>, AllocatorType1> OperatorMVector(const MTypeWrap<C1<T1, A1>, AllocatorType1>& left, MTypeWrap<C1<T1, A1>, AllocatorType1>& right, Fun fun){
	MTypeWrap<C1<T1, A1>, AllocatorType1> __result;
	if (left.size() != right.size()){
		return __result;
	}
	__result.resize(left.size());
	for (int i = 0; i < left.size(); ++i){
		T1 val1 = T1(left[i]);
		T1 val2 = T1(right[i]);
		__result[i] = fun(val1, val2);
	}
	return __result;
}

//
// 两个系列之和等于一个系列
//
template<class T1, class A1, template<class, class> class C1, class AllocatorType1>
MTypeWrap<C1<T1, A1>, AllocatorType1> operator+(const MTypeWrap<C1<T1, A1>, AllocatorType1>& left, const MTypeWrap<C1<T1, A1>, AllocatorType1>& right){
	MTypeWrap<C1<T1, A1>, AllocatorType1> result;
	if (left.size() != right.size())
		return result;
	auto beg1 = left.mVal->mVal.begin();
	auto end1 = left.mVal->mVal.end();
	auto beg2 = right.mVal->mVal.begin();
	auto end2 = right.mVal->mVal.end();
	for (; beg1 != end1; ++beg1, ++beg2){
		result.push_back(*beg1 + *beg2);
	}
	return result;
}

//
// 两个系列之差等于一个系列
//
template<class T1, class A1, template<class, class> class C1, class AllocatorType1>
MTypeWrap<C1<T1, A1>, AllocatorType1> operator-(const MTypeWrap<C1<T1, A1>, AllocatorType1>& left, const MTypeWrap<C1<T1, A1>, AllocatorType1>& right){
	MTypeWrap<C1<T1, A1>, AllocatorType1> result;
	if (left.size() != right.size())
		return result;
	auto beg1 = left.mVal->mVal.begin();
	auto end1 = left.mVal->mVal.end();
	auto beg2 = right.mVal->mVal.begin();
	auto end2 = right.mVal->mVal.end();
	for (; beg1 != end1; ++beg1, ++beg2){
		result.push_back(*beg1 - *beg2);
	}
	return result;
}

//
// 实现流输出
//
template<class T1, class A1, template<class, class> class C1, class AllocatorType1>
std::ostream& operator<<(std::ostream& os, const MTypeWrap<C1<T1, A1>, AllocatorType1>& other){
	if (other.mVal->mVal.empty())
		return os;
	os << "(";
	auto __end = other.mVal->mVal.end();
	std::advance(__end, -1);
	std::copy(other.mVal->mVal.begin(), __end, std::ostream_iterator<T1>(os, ","));
	os << *(__end);
	os << ")";
	return os;
}
