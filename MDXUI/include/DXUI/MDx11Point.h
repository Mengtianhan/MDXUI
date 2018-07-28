#pragma once

#include <MCopyValue.h>
#include <algorithm>
#include <iostream>
#include "DxConfig.h"
namespace MDx11{
template<class T>
struct MDx11Point{
    T mX;
    T mY;
    T mZ;
    T mW;

    MDx11Point():mX(0),mY(0),mZ(0),mW(1)
    {
    }

    MDx11Point(T x,T y,T z,T w = 1):mX(x),mY(y),mZ(z),mW(w)
    {
    }

    MDx11Point(const MDx11Point& other):
        mX(other.mX),
        mY(other.mY),
        mZ(other.mZ),
        mW(other.mW)
    {
    }

    MDx11Point& operator=(const MDx11Point& other){
        mX = other.mX;
        mY = other.mY;
        mZ = other.mZ;
        mW = other.mW;
        return *this;
    }

    template<class U>
    MDx11Point<U>& ToOtherType(MDx11Point<U>& other){
        U x,y,z,w;
        mj::copyvalue(x,mX);
        mj::copyvalue(y,mY);
        mj::copyvalue(z,mZ);
        mj::copyvalue(w,mW);
        other.mX = x;
        other.mY = y;
        other.mZ = z;
        other.mW = w;
        return other;
    }

    bool equals(MDx11Point other){
        return std::abs(mX - other.mX) < 1e-5 &&
                std::abs(mY - other.mY) < 1e-5 &&
                std::abs(mZ - other.mZ) < 1e-5;
    }

    friend std::ostream& operator<<(std::ostream& os,const MDx11Point<T>& __data){
        os<<"("<<__data.mX<<","<<__data.mY<<","<<__data.mZ<<","<<__data.mW<<")"<<std::endl;
        return os;
    }

};

typedef MDx11Point<float>  MDx11PointF;
typedef MDx11Point<float>  MDx11ColorF;



struct DXUnitPointType{
	unsigned mId;
	float mX;
	float mY;
	float mZ;
	friend std::ostream& operator<<(std::ostream& os, const DXUnitPointType& s){
		os << s.mId << "\t" << s.mX << "\t" << s.mY << "\t" << s.mZ;
		return os;
	}
};

//
// 下面的辅助函数仅仅只是为了更好的生成面，不适用于其他几何图形
// 至于其他几何图形有更好的方式
//
template<class A,class A2,template<class T1,class A1> class C>
inline void GenIndexesBufferData(C<std::pair<MDx11PointF,MDx11ColorF>,A>& points,C<unsigned,A2>& indexs){
    if(points.size()<4)
        return;
    typedef std::pair<MDx11PointF,MDx11ColorF> valuetype;
    indexs.clear();
    //
    // 按照Z轴排序
    //
    std::sort(points.begin(),points.end(),[&](
                valuetype p1,valuetype p2
              ){return p1.first.mZ > p2.first.mZ;});

    typedef C<valuetype,A> VValueType;
    typedef C<VValueType,std::allocator<VValueType>> MatrixType;
    VValueType TempV;
    MatrixType sheet;

    for(int i=0;i<points.size()-1;++i){

        MDx11PointF p1 = points[i].first;
        MDx11PointF p2 = points[i+1].first;
        if(i == points.size()-2){
            TempV.push_back(points[i]);
            TempV.push_back(points[i+1]);
            sheet.push_back(TempV);
            TempV.clear();
            break;
        }
        if(std::abs(p1.mZ - p2.mZ)<1e-4){
            TempV.push_back(points[i]);
        }
        else{
            TempV.push_back(points[i]);
            sheet.push_back(TempV);
            TempV.clear();
        }
    }

    //
    // 生成索引数据
    //
    for(int i=0;i<sheet.size()-1;++i){
        VValueType& element1(sheet.at(i));
        VValueType& element2(sheet.at(i+1));
        if(element1.size() != element2.size()){
            std::cerr<<"element1.size() = "<<element1.size()<<std::endl;
            std::cerr<<"element2.size() = "<<element2.size()<<std::endl;
            throw std::runtime_error("Invalid Data Length");
        }
        std::sort(element1.begin(),element1.end(),[&](valuetype p1,valuetype p2)
        {
            return p1.first.mX < p2.first.mX;
        });

        std::sort(element2.begin(),element2.end(),[&](valuetype p1,valuetype p2)
        {
            return p1.first.mX < p2.first.mX;
        });

        int num = element1.size();
        for(int j=0;j<num-1;++j){
            indexs.push_back(i*num+j);
            indexs.push_back(i*num+j+1);
            indexs.push_back((i+1)*num+j);
            indexs.push_back((i+1)*num+j);
            indexs.push_back(i*num+j+1);
            indexs.push_back((i+1)*num+j+1);
        }
    }

    //
    // 重新设置数据
    //

    points.clear();
    for(auto& v : sheet){
        for(auto& _v : v){
            points.push_back(_v);
        }
    }
}

template<class A,class A2,template<class T1,class A1> class C>
inline void GenIndexesBufferData(C<MDx11PointF,A>& points,C<unsigned,A2>& indexs){

    if(points.size()<4)
        return;

    typedef std::pair<MDx11PointF,MDx11ColorF> valuetype;
    typedef C<valuetype,std::allocator<valuetype>> VValueType;
    VValueType tempV;

    for(int i=0;i<points.size();++i){
        tempV.push_back(std::make_pair(points[i],MDx11ColorF()));
    }
    GenIndexesBufferData(tempV,indexs);
    points.clear();
    for(auto& v : tempV){
        points.push_back(v.first);
    }
}


inline void GenIndexesBufferData(std::vector<DxUI::BasicPCT>& points,std::vector<unsigned>& indexs){
	if (points.size()<4)
		return;

	typedef std::pair<MDx11PointF, MDx11ColorF> valuetype;
	typedef std::vector<valuetype> VValueType;
	VValueType tempV;
	tempV.resize(points.size());
	for (int i = 0; i<points.size(); ++i){
		DxUI::BasicPCT& pos = points[i];
		tempV[i] = std::make_pair(MDx11PointF(pos.Pos.x, pos.Pos.y, pos.Pos.z), MDx11ColorF(pos.Color.x, pos.Color.y, pos.Color.z));
	}

	GenIndexesBufferData(tempV, indexs);
	points.clear();
	points.resize(tempV.size());
	for (int i = 0; i<tempV.size(); ++i){
		valuetype& pos = tempV[i];
		points[i] = DxUI::BasicPCT(pos.first.mX, pos.first.mY, pos.first.mZ, pos.second.mX, pos.second.mY, pos.second.mZ, 1.f, 0.f, 0.f);
	}
}

//
// 生成一个四边形
//
template<class A,class A2,template<class T1,class A1> class C>
inline void GenIndexesBufferForQual(C<MDx11PointF,A>& points,C<unsigned,A2>& indexs){
    size_t num = points.size();
    if(num%4!=0){
        points.clear();
        return;
    }
    typedef C<MDx11PointF,A> TYPE1;
    TYPE1 __v1,__v2,__v3,__v4;
    for(int i=0;i<num/4;++i){
        __v1.push_back(points[i]);
        __v2.push_back(points[num/4+i]);
        __v3.push_back(points[2*num/4+i]);
        __v4.push_back(points[3*num/4+i]);
    }

    num = __v1.size();

    //
    // 计算索引数据
    //
    for(int i=0;i<num-1;++i){
        //
        // side1
        //
        indexs.push_back(i);
        indexs.push_back(i+1);
        indexs.push_back(num+i);

        indexs.push_back(num+i);
        indexs.push_back(i+1);
        indexs.push_back(num+i+1);

        //
        // side2
        //
        indexs.push_back(num+i);
        indexs.push_back(num+i+1);
        indexs.push_back(2*num+i);

        indexs.push_back(2*num+i);
        indexs.push_back(num+i+1);
        indexs.push_back(2*num+i+1);

        //
        // side3
        //
        indexs.push_back(2*num+i);
        indexs.push_back(2*num+i+1);
        indexs.push_back(3*num+i);

        indexs.push_back(3*num+i);
        indexs.push_back(2*num+i+1);
        indexs.push_back(3*num+i+1);

        //
        // side4
        //
        indexs.push_back(3*num+i);
        indexs.push_back(3*num+i+1);
        indexs.push_back(i);

        indexs.push_back(i);
        indexs.push_back(3*num+i+1);
        indexs.push_back(i+1);
    }
}

//
// 变换Y轴线条数据
//
template<class A,class A2,template<class T1,class A1> class C>
inline void GenIndexesBufferForYAxis(C<MDx11PointF,A>& points,C<unsigned,A2>& indexs){
    size_t num = points.size();
    if(num%4!=0){
        points.clear();
        return;
    }
    typedef C<MDx11PointF,A> TYPE1;
    TYPE1 __v1,__v2,__v3,__v4;
    for(int i=0;i<num/4;++i){
        __v1.push_back(points[i]);
        __v2.push_back(points[num/4+i]);
        __v3.push_back(points[2*num/4+i]);
        __v4.push_back(points[3*num/4+i]);
    }

    num = __v1.size();

    //
    // 计算索引数据
    //
    for(int i=0;i<num-1;++i){
        //
        // side1
        //
        indexs.push_back(i);
        indexs.push_back(num+i);
        indexs.push_back(i+1);

        indexs.push_back(i+1);
        indexs.push_back(num+i);
        indexs.push_back(num+i+1);

        //
        // side2
        //
        indexs.push_back(num+i);
        indexs.push_back(2*num+i);
        indexs.push_back(num+i+1);

        indexs.push_back(num+i+1);
        indexs.push_back(2*num+i);
        indexs.push_back(2*num+i+1);

        //
        // side3
        //
        indexs.push_back(2*num+i);
        indexs.push_back(3*num+i);
        indexs.push_back(2*num+i+1);

        indexs.push_back(2*num+i+1);
        indexs.push_back(3*num+i);
        indexs.push_back(3*num+i+1);

        //
        // side4
        //
        indexs.push_back(3*num+i);
        indexs.push_back(i);
        indexs.push_back(3*num+i+1);

        indexs.push_back(3*num+i+1);
        indexs.push_back(i);
        indexs.push_back(i+1);
    }
}

//
// 变换Z轴线条数据
//
template<class A,class A2,template<class T1,class A1> class C>
inline void GenIndexesBufferForZAxis(C<MDx11PointF,A>& points,C<unsigned,A2>& indexs){
    size_t num = points.size();
    if(num%4!=0){
        points.clear();
        return;
    }
    typedef C<MDx11PointF,A> TYPE1;
    TYPE1 __v1,__v2,__v3,__v4;
    for(int i=0;i<num/4;++i){
        __v1.push_back(points[i]);
        __v2.push_back(points[num/4+i]);
        __v3.push_back(points[2*num/4+i]);
        __v4.push_back(points[3*num/4+i]);
    }

    num = __v1.size();

    //
    // 计算索引数据
    //
    for(int i=0;i<num-1;++i){
        //
        // side1
        //
        indexs.push_back(i);
        indexs.push_back(num+i);
        indexs.push_back(i+1);

        indexs.push_back(1+i);
        indexs.push_back(num+i);
        indexs.push_back(num+i+1);

        //
        // side2
        //
        indexs.push_back(num+i);
        indexs.push_back(2*num+i);
        indexs.push_back(num+i+1);

        indexs.push_back(num+i+1);
        indexs.push_back(2*num+i);
        indexs.push_back(2*num+i+1);

        //
        // side3
        //
        indexs.push_back(2*num+i);
        indexs.push_back(3*num+i);
        indexs.push_back(2*num+i+1);

        indexs.push_back(2*num+i+1);
        indexs.push_back(3*num+i);
        indexs.push_back(3*num+i+1);

        //
        // side4
        //
        indexs.push_back(3*num+i);
        indexs.push_back(i);
        indexs.push_back(3*num+i+1);

        indexs.push_back(3*num+i+1);
        indexs.push_back(i);
        indexs.push_back(i+1);
    }
}
}
