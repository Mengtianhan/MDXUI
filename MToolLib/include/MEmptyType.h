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