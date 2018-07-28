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