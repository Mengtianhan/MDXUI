#pragma once
#include <iostream>
#include <assert.h>
#include <mutex>
#include <new>

#include <HVector.h>
#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) > (b)) ? (b) : (a))
#endif

//----------------------------------------
//  针对浮点数的比较判断
//----------------------------------------
#ifndef __MINFLOATVALUE
#define __MINFLOATVALUE 0.000005
#endif

//
// 定义一个pair,该pair可以和标准库互换
//
template<class Key,class Value>
struct __MPair{
        __MPair():first(Key()),second(Value()){}
	__MPair(Key key, Value value) :first(key), second(value){}
	__MPair(const __MPair& other) :first(other.first), second(other.second){}
        __MPair(const std::pair<Key,Value>& stdPair):first(stdPair.first),second(stdPair.second){}
	__MPair& operator=(const __MPair& other){
		if (this == &other)
			return *this;
		first = other.first;
		second = other.second;
		return *this;
	}

        __MPair& operator=(const std::pair<Key,Value>& stdPair){
            first  = stdPair.first;
            second =stdPair.second;
            return *this;
        }

        operator std::pair<Key,Value>(){
            return std::make_pair(first,second);
        }

        operator std::pair<Key,Value>() const{
            return std::make_pair(first,second);
        }

	Key first;
	Value second;
        friend std::ostream& operator<<(std::ostream& os, const __MPair& p){
                os << "("<<p.first << " : " << p.second <<")"<< std::endl;
                return os;
        }
};

template<class K,class V>
__MPair<K, V> __makepair(const K& key, const V& value){
	return __MPair<K, V>(key, value);
}

//-----------------------------------------
// 定义一个默认比较器
//-----------------------------------------

template<class T>
struct __MLess{
	bool operator()(T t1, T t2){
		return t1 < t2;
	}
};

template<>
struct __MLess<float>{
	bool operator()(float t1, float t2){
		return t1 < t2 && std::abs(t1 - t2)> __MINFLOATVALUE;
	}
};


template<>
struct __MLess<double>{
	bool operator()(double t1, double t2){
		return t1 < t2 && std::abs(t1 - t2)> __MINFLOATVALUE;
	}
};

template<class T,class Cmp>
struct __CmpFun{
    bool operator()(T p1, T p2){
            return Cmp()(p1, p2);
    }
};

//------------------------------------
// 针对实现map的pair进行特化
//------------------------------------
template<class K, class V, class Cmp>
struct __CmpFun<__MPair<K, V>, Cmp>{
	bool operator()(__MPair<K, V> p1, __MPair<K, V> p2){
		return Cmp()(p1.first, p2.first);
	}
};


//----------------------------------------
// 针对Pair的小于特化
//----------------------------------------
template<class T>
struct __MPairLess;

template<class K, class V>
struct __MPairLess<__MPair<K, V>>{
	bool operator()(__MPair<K, V> p1, __MPair<K, V> p2){
		return p1.first< p2.first;
	}
};

//----------------------------------------
// 定义一个平衡二叉树数据结构
//----------------------------------------
template<class T>
struct AvlNode{
	T  mValue;
	AvlNode* Left;
	AvlNode* Right;
	int Hight;
	AvlNode(T value, AvlNode* left = nullptr, AvlNode* right = nullptr, int h = 0) :
		mValue(value), Left(left), Right(right), Hight(h)
	{}
};

//---------------------------------------
// 查看是否包含某项
//---------------------------------------
template<class T, class Cmp>
bool __contains(const T& value, AvlNode<T>* val, Cmp __cmp){
	if (val == nullptr)
		return false;
	if (__cmp(value,val->mValue)){
		return __contains(value, val->Left,__cmp);
	}
	else if (__cmp(val->mValue, value)){
		return __contains(value, val->Right,__cmp);
	}
	else{
		return true;
	}
}

template<class T,class Cmp>
AvlNode<T>* __find(const T& value, AvlNode<T>* val, Cmp __cmp){
	if (val == nullptr)
		return nullptr;
	if (__cmp(value, val->mValue)){
		return __find(value, val->Left, __cmp);
	}
	else if (__cmp(val->mValue, value)){
		return __find(value, val->Right, __cmp);
	}
	else{
		return val;
	}
}

//--------------------------------------
// 获取树的深度
//--------------------------------------
template<class T>
inline int Height(AvlNode<T>* val){
	return  val == nullptr ? -1 : val->Hight;
}

template<class T>
inline void RotateWithLeftChild(AvlNode<T>*& val){
	AvlNode<T>* k1 = val->Left;
	val->Left = k1->Right;
	k1->Right = val;
	val->Hight = max(Height(val->Left), Height(val->Right)) + 1;
	k1->Hight = max(Height(k1->Left), val->Hight) + 1;
	val = k1;
}

template<class T>
inline void RotateWithRightChild(AvlNode<T>*& val){
	AvlNode<T>* k1 = val->Right;
	val->Right = k1->Left;
	k1->Left = val;
	val->Hight = max(Height(val->Left), Height(val->Right)) + 1;
	k1->Hight = max(Height(k1->Right), val->Hight) + 1;
	val = k1;
}

template<class T>
inline void DoubleRotateWithLeftChild(AvlNode<T>*& val){
	RotateWithRightChild(val->Left);
	RotateWithLeftChild(val);
}

template<class T>
inline void DoubleRotateWithRightChild(AvlNode<T>*& val){
	RotateWithLeftChild(val->Right);
	RotateWithRightChild(val);
}

//---------------------------------------------
// 插入一个数
//--------------------------------------------
template<class T,class Cmp>
inline void __insert(const T& value, AvlNode<T>*& val, Cmp __cmp){
	if (val == nullptr){
		val = new AvlNode<T>(value);
	}
	else if (__cmp(value, val->mValue)){
		__insert(value, val->Left, __cmp);
		if (Height(val->Left) - Height(val->Right) == 2){
			if (__cmp(value, val->Left->mValue)){
				RotateWithLeftChild(val);
			}
			else{
				DoubleRotateWithLeftChild(val);
			}
		}
	}
	else if (__cmp(val->mValue,value)){
		__insert(value, val->Right, __cmp);
		if (Height(val->Left) - Height(val->Right) == -2){
			if (__cmp(val->Right->mValue,value)){
				RotateWithRightChild(val);
			}
			else{
				DoubleRotateWithRightChild(val);
			}
		}
	}
	val->Hight = max(Height(val->Left), Height(val->Right)) + 1;
}

template<class T>
AvlNode<T>* FinMin(AvlNode<T>* val){
	if (val == nullptr)
		return nullptr;
	if (val->Left == nullptr)
		return val;
	else
		return FinMin(val->Left);
}

template<class T>
AvlNode<T>* FinMax(AvlNode<T>* val){
	if (val == nullptr)
		return nullptr;
	if (val->Right == nullptr)
		return val;
	else
		return FinMax(val->Right);
}

template<class T,class Cmp>
void deleteMin(const T& value, AvlNode<T>*& val, Cmp __cmp){
	if (val == nullptr)
		return;
	if (__cmp(value ,val->mValue)){
		deleteMin(value, val->Left, __cmp);
	}
	else if (__cmp(val->mValue, value)){
		deleteMin(value, val->Right, __cmp);		
	}
	else{
		AvlNode<T>* __temp = val;
		val = (val->Left) ? val->Left : val->Right;
		delete __temp;
		__temp = nullptr;
	}
	if (val){
		if (Height(val->Left) - Height(val->Right) == 2){
			DoubleRotateWithLeftChild(val);
		}
		else if (Height(val->Left) - Height(val->Right) == -2){
			DoubleRotateWithRightChild(val);
		}
		val->Hight = max(Height(val->Left), Height(val->Right)) + 1;
	}
}

template<class T, class Cmp>
void deleteMax(const T& value, AvlNode<T>*& val, Cmp __cmp){
	if (val == nullptr)
		return;
	if (__cmp(val->mValue,value)){
		deleteMax(value, val->Right, __cmp);
	}
	else if (__cmp(value, val->mValue)){
		deleteMax(value, val->Left, __cmp);
	}
	else{
		AvlNode<T>* __temp = val;
		val = (val->Left) ? val->Left : val->Right;
		delete __temp;
		__temp = nullptr;
	}
	if (val){
		if (Height(val->Left) - Height(val->Right) == 2){
			DoubleRotateWithLeftChild(val);
		}
		else if (Height(val->Left) - Height(val->Right) == -2){
			DoubleRotateWithRightChild(val);
		}
		val->Hight = max(Height(val->Left), Height(val->Right)) + 1;
	}
}

template<class T,class Cmp>
inline void __remove(const T& value, AvlNode<T>*& val, Cmp __cmp){
	if (val == nullptr)
		return;

	if (__cmp(value , val->mValue)){
		__remove<T>(value, val->Left, __cmp);
		if (Height(val->Left) - Height(val->Right) == -2){
			RotateWithRightChild(val);
		}
	}
	else if (__cmp(val->mValue, value)){
		__remove<T>(value, val->Right, __cmp);
		if (Height(val->Left) - Height(val->Right) == 2){		
			RotateWithLeftChild(val);
		}
	}
	else if ((val->Left != nullptr) && (val->Right != nullptr)){
		if (__MPairLess<T>()(val->Left->mValue, val->mValue) && __cmp(val->Left->mValue, val->mValue)){
			AvlNode<T>* __min = FinMin(val->Right);
			val->mValue = __min->mValue;
			deleteMin(__min->mValue, val->Right, __cmp);
		}
		else{
			AvlNode<T>* __max = FinMax(val->Left);
			val->mValue = __max->mValue;
			deleteMax(__max->mValue, val->Left, __cmp);
		}
		
	}
	else{
		AvlNode<T>* __temp = val;
		val = (val->Left) ? val->Left : val->Right;
		delete __temp;
		__temp = nullptr;
	}
	
	if (val){
		if (Height(val->Left) - Height(val->Right) == 2){
			DoubleRotateWithLeftChild(val);
		}
		else if (Height(val->Left) - Height(val->Right) == -2){
			DoubleRotateWithRightChild(val);
		}
		val->Hight = max(Height(val->Left), Height(val->Right)) + 1;
	}
}

template<class T>
void __show(AvlNode<T>* val, std::ostream& os){
	if (val){
		if (val->Left){	
			__show(val->Left,os);
		}
		os << val->mValue;
		if (val->Right){
			__show(val->Right,os);
		}
	}
	else{
		return;
	}
}

template<class T, class F>
void __serial(AvlNode<T>* val, F& os){
	if (val){
		if (val->Left){
			__serial(val->Left, os);
		}
                os<<(&(val->mValue));
		if (val->Right){
			__serial(val->Right, os);
		}
	}
	else{
		return;
	}
}


template<class T>
void __clear(AvlNode<T>*& val){
	if (val){
		__clear(val->Left);
		__clear(val->Right);
		delete val;
	}
	val = nullptr;
}

template<class T>
void __clone(AvlNode<T>*& dest, AvlNode<T>* src){
        if(src == nullptr)
            return;
	if (dest == nullptr){
                dest = new AvlNode<T>(src->mValue);
	}
	dest->Hight = src->Hight;
	dest->mValue = src->mValue;
	__clone(dest->Left, src->Left);
	__clone(dest->Right, src->Right);	
}

//---------------------------------------------------
// 在begin和end的时候如果遍历失败会抛出此移除
// 通常这个异常是不会抛的，除非数据量很大已经无法获取内存
//---------------------------------------------------
class bad_realloc : public std::exception{
public:
	bad_realloc(const char* msg) :exception(msg){}
};
template<class T>
class MapIterHelp{
public:
	MapIterHelp() :mData(nullptr), mSize(0), mMaxSize(0)
	{}

	~MapIterHelp(){
		SetSize(0);
	}

	MapIterHelp(const MapIterHelp& other){
		for (int i = 0; i < other.mSize; ++i){
			Add(other.mData[i]);
		}
	}

	MapIterHelp& operator=(const MapIterHelp& other){
		if (&other == this)
			return *this;
		if (mData){
			free(mData);
		}
		for (int i = 0; i < other.mSize; ++i){
			Add(other.mData[i]);
		}
		return *this;
	}
	
	T& at(__int64 index) const{
		if (index >= mSize){
			throw std::out_of_range("index out of range");
		}
		return mData[index];
	}

	__int64 size(){
		return mSize;
	}

	

	MapIterHelp& operator<<(const T& value){
		Add(value);
		return *this;
	}

	bool resize(__int64 nNewMaxSize){
		if (nNewMaxSize < 0 || (nNewMaxSize > LLONG_MAX / sizeof(T))){
			return false;
		}
		if (nNewMaxSize == 0){
			if (mData){
				free(mData);
				mData = nullptr;
			}
			mMaxSize = 0;
			mSize = 0;
		}
		else if (mData == nullptr || nNewMaxSize > mMaxSize){
			unsigned nGrowBy = (mMaxSize == 0) ? 16 : mMaxSize + 16;
			if (mMaxSize + nGrowBy > (unsigned)LLONG_MAX)
				nGrowBy = LLONG_MAX - mMaxSize;

			nNewMaxSize = __max(nNewMaxSize, mMaxSize + nGrowBy);
			if (sizeof(T) > LLONG_MAX / nNewMaxSize){
				return false;
			}

			T* pDataNew = (T*)realloc(mData, nNewMaxSize * sizeof(T));
			if (pDataNew == nullptr){
				throw bad_realloc("realoc fail");
			}
			mData = pDataNew;
			mMaxSize = nNewMaxSize;
		}
		return true;
	}

	bool Add(const T& value){
		if (!resize(mSize + 1)){
			return false;
		}
		::new (&mData[mSize]) T;
		mData[mSize] = value;
		++mSize;
                return true;
	}

	bool SetSize(__int64 nNewMaxSize){
		int nOldSize = mSize;
		if (nOldSize > nNewMaxSize)
		{
			assert(mData);
			if (mData){
				for (int i = nNewMaxSize; i < nOldSize; ++i)
					mData[i].~T();
			}
		}

		bool __result = resize(nNewMaxSize);

		if (nOldSize < nNewMaxSize)
		{
			assert(mData);
			if (mData){
				for (int i = nOldSize; i < nNewMaxSize; ++i)
					new(&mData[i])T;
			}
		}
		return __result;
	}

	T* begin(){
		assert(mData);
		return &mData[0];
	}

	T* end(){
		return mData + mSize;
	}
private:
	T*		 mData{ nullptr };
        __int64          mSize{ 0ll };
        __int64          mMaxSize{ 0ll };
};

//---------------------------------------------------
// 定义一套Map迭代器
// 没有针const的版本
// 后续补充
// 其实也就在非const上面修改一下就好
//---------------------------------------------------
template<class T>
class MMapIter{
	template<bool, class T>
	struct __CType{
		typedef typename const T::reference referencetype;
		typedef typename const T::pointer pointertype;
	};

	template<class T>
	struct __CType<false, T>{
		typedef typename T::reference referencetype;
		typedef typename T::pointer pointertype;
	};
public:
	typedef typename T::type type;
	typedef typename T::iterator_category iterator_category;
	typedef typename T::value_type value_type;
	typedef typename T::difference_type difference_type;
	typedef typename T::distance_type distance_type;
	typedef typename T::pointer pointer;
	typedef typename T::reference reference;
	typedef typename T::CMP Cmp;
	typedef typename __CType<std::is_const<T>::value, T>::referencetype rtype;
	typedef typename __CType<std::is_const<T>::value, T>::pointertype ptype;


public:
	MMapIter(MapIterHelp<type*>* FirstNode,unsigned size) :
		mData(FirstNode), mCurrentSize(size)
	{}

	MMapIter(const MMapIter& other){
		mData = other.mData;
		mCurrentSize = other.mCurrentSize;
	}

	ptype  operator->(){
		if (mData == nullptr){
			throw std::out_of_range("iterator out of range");
		}
		return mData->at(mCurrentSize);
	}

	ptype  operator->() const{
		if (mData == nullptr){
			throw std::out_of_range("iterator out of range");
		}
		return mData->at(mCurrentSize);
	}

	rtype operator*(){
		if (mData == nullptr){
			throw std::out_of_range("iterator out of range");
		}
		return *(mData->at(mCurrentSize));
	}

	rtype operator*() const{
		if (mData == nullptr){
			throw std::out_of_range("iterator out of range");
		}
		return *(mData->at(mCurrentSize));
	}

	MMapIter& operator++(){
		if (mData == nullptr || mData->begin() == nullptr){
			throw std::out_of_range("iterator out of range");
		}
		++mCurrentSize;
		return *this;
	}

	MMapIter operator++(int){
		if (mData == nullptr || mData->begin() == nullptr){
			throw std::out_of_range("iterator out of range");
		}
		MMapIter Temp(*this);
		++mCurrentSize;
		return Temp;
	}

	MMapIter& operator--(){
		if (mData == nullptr || mData->begin() == nullptr){
			throw std::out_of_range("iterator out of range");
		}
		--mCurrentSize;
		return *this;
	}

	MMapIter& operator--(int){
		if (mData == nullptr || mData->begin() == nullptr){
			throw std::out_of_range("iterator out of range");
		}
		MMapIter Temp(*this);
		--mCurrentSize;
		return Temp;
	}

	MMapIter& operator+(size_t index){
		mCurrentSize += index;
		return *this;
	}

	MMapIter& operator-(size_t index){
		mCurrentSize -= index;
		return *this;
	}

	MMapIter& operator+=(size_t index){
		mCurrentSize += index;
		return *this;
	}

	MMapIter& operator-=(size_t index){
		mCurrentSize -= index;
		return *this;
	}

	friend bool operator<(MMapIter first, MMapIter second){
       //         static_assert((first.mData == second.mData),"different type not comp");
		return first.mCurrentSize < second.mCurrentSize;
	}

	friend bool operator>(MMapIter first, MMapIter second){
      //          static_assert((first.mData == second.mData),"different type not comp");
		return first.mCurrentSize > second.mCurrentSize;
	}

	friend bool operator==(MMapIter first, MMapIter second){
      //          static_assert((first.mData == second.mData),"different type not comp");
		return first.mCurrentSize == second.mCurrentSize;
	}

	friend bool operator!=(MMapIter first, MMapIter second){
     //           static_assert((&first.mData == &second.mData),"different type not comp");
		return first.mCurrentSize != second.mCurrentSize;
	}

private:
	unsigned mCurrentSize;
	MapIterHelp<type*>* mData;
};

template<class K, class V, class Cmp = __MLess<K>>
class MMap
{
public:
	typedef __MPair<K, V> type;
	typedef MMapIter<MMap> iterator;
	typedef MMapIter<const MMap> const_iterator;
	typedef std::random_access_iterator_tag iterator_category;
	typedef __MPair<K,V> value_type;
	typedef std::ptrdiff_t difference_type;
	typedef std::ptrdiff_t distance_type;
	typedef __MPair<K, V>* pointer;
	typedef __MPair<K, V>& reference;
	typedef __CmpFun<__MPair<K, V>, Cmp> CMP;
	typedef MapIterHelp<type*> IteratorHelp;



public:
	MMap() :mSize(0), __end(nullptr), __beg(nullptr)
	{
	}

        MMap(const MMap& other){
            __clone(mData, other.mData);
            mSize = other.mSize;
            __end = nullptr;
            __beg = nullptr;
        }

        //
        // 使用QMap来对其进行构造
        //
        template<class __Key,class __Value,template<class __K1,class __V1> class QM>
        MMap(const QM<__Key,__Value>& other){
            __end = nullptr;
            __beg = nullptr;
            for(auto& __m : other){
               (*this)[other.key(__m)] = value;
            }
        }

	MMap(const K& key, const V& value){
            __end = nullptr;
            __beg = nullptr;
            (*this)[key] = value;
	}

	MMap(const std::initializer_list<__MPair<K, V>>& l){
                __end = nullptr;
                __beg = nullptr;
		for (auto __l : l){
			insert(__l);
		}
	}

	template<class A,template<class K1,class V1,class C2,class A2>class M>
	MMap(const M<K, V, Cmp, A>& m){
                __end = nullptr;
                __beg = nullptr;
		clone(m);
	}

        MMap& operator=(const MMap& other){
		if (this == &other)
			return *this;
		__clear(mData);
		__clone(mData, other.mData);
		__end = nullptr;
		__beg = nullptr;
                mSize = other.mSize;
		return *this;
	}

        MMap& operator=(MMap&& other){
		if (this == &other)
			return *this;
		__clear(mData);
		mData = other.mData;
                mSize = other.mSize;
		other.mData = nullptr;
                other.mSize = 0;
		__end = nullptr;
		__beg = nullptr;
		return *this;
	}

	template<class Cmp2,class A, template<class K1, class V1, class C2, class A2>class M>
        MMap& operator=(const M<K, V, Cmp2, A>& m){
		clone(m);
                __end = nullptr;
                __beg = nullptr;
		return *this;
	}

	template<class Cmp2, class A, template<class K1, class V1, class C2, class A2>class M>
        operator M<K, V, Cmp2, A>(){
		M<K, V, Cmp2, A> __m;
		for (auto& m : *this){
			__m[m.first] = m.second;
		}
		return __m;
	}

	template<class Cmp2, class A, template<class K1, class V1, class C2, class A2>class M>
        operator M<K, V, Cmp2, A>() const{
		M<K, V, Cmp2, A> __m;
		for (auto& m : *this){
			__m[m.first] = m.second;
		}
		return __m;
	}

	template<class Cmp2, class A, template<class K1, class V1, class C2, class A2>class M>
        M<K, V, Cmp2, A> tomap(){
		M<K, V, Cmp2, A> __m;
		for (auto& m : *this){
			__m[m.first] = m.second;
		}
		return __m;
	}

	template<class Cmp2, class A, template<class K1, class V1, class C2, class A2>class M>
        M<K, V, Cmp2, A> tomap() const{
		M<K, V, Cmp2, A> __m;
		for (auto& m : *this){
			__m[m.first] = m.second;
		}
		return __m;
	}

        //
        // 隐式转换为QMap
        //
        template<template<class K1,class V1> class M>
        operator M<K,V>(){
            M<K, V> __m;
            for (auto& m : *this){
                    __m[m.first] = m.second;
            }
            return __m;
        }


        template<template<class K1,class V1> class M>
        operator M<K,V>() const{
            M<K, V> __m;
            for (auto& m : *this){
                    __m[m.first] = m.second;
            }
            return __m;
        }

        //
        // 显示转换为QMap
        //
        template<template<class K1,class V1> class M>
        M<K,V> qMap(){
            M<K, V> __m;
            for (auto& m : *this){
                    __m[m.first] = m.second;
            }
            return __m;
        }

        template<template<class K1,class V1> class M>
        M<K,V> qMap() const{
            M<K, V> __m;
            for (auto& m : *this){
                    __m[m.first] = m.second;
            }
            return __m;
        }

	~MMap(){
		__clear(mData);
		if (__beg){
			delete __beg;
			__beg = nullptr;
		}
		if (__end){
			delete __end;
			__end = nullptr;
		}
	}

        void insert(const __MPair<K,V>& value){
		if (__contains(value, mData, mCmpFun))
			return;
		__insert(value, mData, mCmpFun);
		++mSize;
	}

        void remove(const __MPair<K, V>& value){
		if (!__contains(value, mData, mCmpFun))
			return;
		__remove(value, mData, mCmpFun);
		--mSize;
	}

        void erase(const K& key){
		remove(__makepair(key, V()));
	}

        size_t counts(){
            return mSize;
        }

        size_t counts() const{
            return mSize;
        }

        size_t count(const K& key){
                if (__contains(__makepair(key, V()), mData, mCmpFun)){
			return 1;
		}
		else{
			return 0;
		}
	}

        size_t count(const K& key) const{
                return const_cast<MMap*>(this)->count(key);
        }

        bool contains(const K& key){
            return count(key);
        }

        bool contains(const K& key) const{
            return count(key);
        }

        bool containsvalue(const V& value){
            for(auto& m : *this){
                if(m.second == value){
                    return true;
                }
            }
            return false;
        }

        bool containsvalue(const V& value) const{
            return const_cast<MMap*>(this)->containsvalue(value);
        }

        K& value_key(const V& value){
            for(auto& m : *this){
                if(m.second == value){
                    return m.first;
                }
            }
            throw std::runtime_error("Not Find Value");
        }

        K& value_key(const V& value) const{
            return const_cast<MMap*>(this)->value_key(value);
        }

        V& key_value(const K& key){
            return at(key);
        }

        V& key_value(const K &key) const{
            return at(key);
        }

        V& at(const K& key){
		AvlNode<__MPair<K, V>>* it = __find(__makepair(key, V()), mData, mCmpFun);
		if (it){
			return it->mValue.second;
		}
		else{
                        throw std::out_of_range("Index Out of range");
		}
	}

        V& at(const K& key) const{
                return const_cast<MMap*>(this)->at(key);
	}

        V& operator[](const K& key){
		AvlNode<__MPair<K, V>>* it = __find(__makepair(key, V()), mData, mCmpFun);
		if (it){
			return it->mValue.second;
		}
		else{
			insert(__makepair(key, V()));
			it = __find(__makepair(key, V()), mData, mCmpFun);
			return it->mValue.second;
		}
	}

	const V& operator[](const K& key) const{
                return const_cast<MMap*>(this)->operator [](key);
	}

        V& operator[](const K& key) volatile{
                AvlNode<__MPair<K, V>>* it = __find(__makepair(key, V()), mData, mCmpFun);
                if (it){
                        return it->mValue.second;
                }
                else{
                        insert(__makepair(key, V()));
                        it = __find(__makepair(key, V()), mData, mCmpFun);
                        return it->mValue.second;
                }
        }

        const V& operator[](const K& key) const volatile{
                return const_cast<volatile MMap*>(this)->operator [](key);
        }


        size_t size(){
		return mSize;
	}

	size_t size() const{
                return const_cast<MMap*>(this)->size();
	}

        void clear(){
		__clear(mData);
		mSize = 0;
	}

        bool empty(){
		return mSize == 0 ? true : false;
	}

	bool empty() const{
                return const_cast<MMap*>(this)->empty();
	}

	iterator begin(){
		if (__beg){
			delete __beg;
			__beg = nullptr;
		}
		__beg = new IteratorHelp;
		__beg->resize(mSize);
		__serial(mData, *__beg);
		return iterator(__beg, 0);
	}

        iterator begin() const{
                return const_cast<MMap*>(this)->begin();
        }

	iterator end(){
		if (__end){
			delete __end;
			__end = nullptr;
		}
		__end = new IteratorHelp;
		__end->resize(mSize);
		__serial(mData, *__end);
		return iterator(__end, __end->size());
	}

        iterator end() const{
                return const_cast<MMap*>(this)->end();
        }

//        friend std::ostream& operator<<(std::ostream& os, const MMap& __map){
//                __show(__map.mData, os);
//                return os;
//        }

        void show(std::ostream& os = std::cout){
            __show(mData, os);
        }

        MMap& operator()(const K& key, const V& value){
		(*this)[key] = value;
		return *this;
	}

protected:
	template<class Cmp2,class A, template<class K1, class V1, class C2, class A2>class M>
	void clone(const M<K, V, Cmp2, A>& m){
		__clear(mData);
		for (auto& __m : m){
			insert(__makepair(__m.first, __m.second));
		}
		__end = nullptr;
		__beg = nullptr;
	}

private:
	AvlNode<__MPair<K, V>>*      mData{ nullptr };
	__CmpFun<__MPair<K, V>, Cmp> mCmpFun;
        int			 mSize{ 0 };
	IteratorHelp*		 __end{ nullptr };
	IteratorHelp*		 __beg{ nullptr };
};
