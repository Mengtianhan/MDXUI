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
