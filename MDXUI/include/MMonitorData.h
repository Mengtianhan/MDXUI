

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma  once
#include <iostream>

//==========================================================
// 单元测试数据类型
//==========================================================
struct MMonitorUnitData{
    unsigned mId;
    double    mX;
    double    mY;
	double    mZ;
};
//==========================================================
// 监控数据类型
//==========================================================
struct MMonitorData{
    double mX,mY,mData;
    MMonitorData():
        mX(0),mY(0),mData(0)
    {

    }

	MMonitorData(double _x, double _y, double _data) :
        mX(_x),mY(_y),mData(_data)
    {

    }

    MMonitorData(const MMonitorData& other):
        mX(other.mX),mY(other.mY),mData(other.mData)
    {

    }

    MMonitorData& operator=(const MMonitorData& other)
    {
        mX = other.mX;
        mY = other.mY;
        mData = other.mData;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out,const MMonitorData& other){
        out<<"x = " <<other.mX <<"  y = "<<other.mY<<"  Value = "<<other.mData<<std::endl;
        return out;
    }
};


//T实际上也就只能指定为了 ,为set提供的一个比较器
template<class T = MMonitorData>
class MMonitorDataCMPY : public std::binary_function<T,T,bool>{
public:
   MMonitorDataCMPY(){}
   bool operator()(T t1,T t2){
       return t1.mY<t2.mY;
   }
};

template<class T = MMonitorData>
class MMonitorDataCMPX : public std::binary_function<T,T,bool>{
public:
   MMonitorDataCMPX(){}
   bool operator()(T t1,T t2){
       return t1.mX<t2.mX;
   }
};

template<class T = MMonitorData>
class MMonitorDataCMPV : public std::binary_function<T,T,bool>{
public:
   MMonitorDataCMPV(){}
   bool operator()(T t1,T t2){
       return t1.mData<t2.mData;
   }
};
//========================================================================
// 针对带三种类型的数据
//========================================================================
template<class T1,class T2,class V>
struct MDataForThree{
    T1 mX;
    T2 mY;
    V mValue;

    MDataForThree():mX(),y(),mValue(){}

    MDataForThree(T1 __x,T2 __y, V tvalue):
        mX(__x),mY(__y),mValue(tvalue){}

    MDataForThree(const MDataForThree& other):
        mX(other.mX),mY(other.mY),mValue(other.mValue)
    {
    }

    MDataForThree& operator=(const MDataForThree& other){
        mX = other.mX;
        mY = other.mY;
        mValue = other.mValue;
        return *this;
    }

    bool operator<(const MDataForThree& other){
        return mValue < other.mValue;
    }

    friend std::ostream& operator<<(std::ostream& out,const MDataForThree& other){

        out<<other.mX <<"\t\t"<<other.mY<<"\t\t"<<other.mValue<<std::endl;
        return out;
    }

};

template<class T1,class T2,class V>
inline bool operator<(const MDataForThree<T1,T2,V>& other,const MDataForThree<T1,T2,V>& another){
    return other.operator <(another);
}

