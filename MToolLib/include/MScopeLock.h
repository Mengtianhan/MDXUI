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
#include <functional>
#include <MFunctionTraits.h>

///
/// 主要用于有指定Lock和UnLock的对象
///
template<class T>
class MScopeLock{
public:
    MScopeLock(T& obj):lockObj(obj){
        obj.Lock();
    }

    MScopeLock(T* obj):lockObj(*obj){
        obj->Lock();
    }

    ~MScopeLock(){
        lockObj.UnLock();
    }

private:
    T& lockObj;
};

///
/// \brief The MScopeBool class
/// 通常用在某些时候需要在返回时修改控制变量值的时候很有用
///
class MScopeBool{
public:
    MScopeBool(bool& val):bVal(val){}
    ~MScopeBool(){bVal = !bVal;}
private:
    bool& bVal;
};

///
/// 值对象外覆器
/// 主要用来在不必要的时候恢复值
/// 这个有必要的内存开销
///
template<class T>
class MScopeValue{
public:
    MScopeValue(T& val,const T& covervalue):mVal(val),mCoverVal(covervalue){

    }
    ~MScopeValue(){mVal = mCoverVal;}
private:
    T& mVal;
    T  mCoverVal;
};

///
/// 操作对象外覆器
/// 封装一个函数,在对象销毁时执行该函数
///
template<class R,class...Args>
class MExitFunction{
public:

    typedef std::tuple<Args...> ArgsType;
    typedef std::function<R(Args...)> FunType;

    MExitFunction(FunType fun,const Args&... args):mFun(fun),mArgs(std::make_tuple(args...)){
    }
    ~MExitFunction(){
        if(mFun){
            mj::ManualFun(mFun,mArgs);
        }
    }

    void Run(){
        if(mFun){
            mj::ManualFun(mFun,mArgs);
            mFun = nullptr;
        }
    }
private:
    FunType mFun{nullptr};
    ArgsType mArgs;
};

template<class R>
class MExitFunction<R>{
public:
    typedef std::function<R()> FunType;

    MExitFunction(FunType fun):mFun(fun){
    }
    ~MExitFunction(){
        if(mFun){
            mFun();
        }
    }

    void Run(){
        if(mFun){
            mFun();
            mFun = nullptr;
        }
    }

private:
    FunType mFun{nullptr};
};
