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

//
// 执行函数的包装器
//

#include <MString.h>
#include <MAny.h>
#include <MMap.h>
#include <MFunctionTraits.h>
#include "MSingleton.h"

//
// boost 主要用于类型补全
//
#include <type_traits>
#include <boost/mpl/or.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/add_reference.hpp>



class MDispatch :public MSingleton<MDispatch>
{
public:
    //
    // 模板参数类型自行推导
    //
    template<class T>
    struct paramtype : boost::mpl::eval_if <
            boost::mpl::or_ < std::is_scalar<T>,
            std::is_reference < T >> ,
            boost::mpl::identity<T>, boost::add_reference <T >>
    {};

    template<class T>
    struct constparamtype : boost::mpl::eval_if <
            boost::mpl::or_ < std::is_scalar<T>,
            std::is_reference < T >> ,
            boost::mpl::identity<T>, boost::add_reference <T const >>
    {};

public:
    MDispatch(){}
    ~MDispatch(){}

public:
    //
    // 注册函数,funName 标识函数的函数名，fun 函数对象，使用MSLOT来生成该函数对象
    // 函数参数除了返回参数外，其余的参数都使用const reference传递（标量除外）
    //
    void RegisterFunction(const mj::MString& funName,const mj::MAny& fun){
        if(mFunMap.count(funName)){
            mFunMap.at(funName).push_back(fun);
        }
        else{
            mFunMap[funName] = std::list<mj::MAny>({fun});
        }

    }

    //
    // 执行带返回数据的函数
    //
    template<class R,class ...Args>
    bool RunReturnValue(const mj::MString& funName,R& outValue,Args ... args){
        if(mFunMap.count(funName)){
            typedef std::function<R(typename constparamtype<Args>::type...)> FunType;
            auto funs = mFunMap.at(funName);;
            for(auto& f : funs){
                try{
                    auto fun = mj::any_cast<FunType>(f);
                    outValue = fun(args...);
                    return true;
                }
                catch(...){
                    ;
                }
            }
            return false;
        }
        return false;
    }

    //
    // 执行不带返回数据的函数,所有参数类型都是const reference
    //
    template<class ...Args>
    bool Run(const mj::MString& funName,Args ... args){
        if(mFunMap.count(funName)){
            typedef std::function<void(typename constparamtype<Args>::type...)> FunType;
            auto funs = mFunMap.at(funName);;
            for(auto& f : funs){
                try{
                    auto fun = mj::any_cast<FunType>(f);
                    fun(args...);
                    return true;
                }
                catch(...){
                    ;
                }
            }
            return false;
        }
        return false;
    }


    //
    // 执行不带返回数据的函数，所有参数类型都是引用，非const ,转为某些特殊函数而设定
    //
    template<class...Args>
    bool RunNoConst(const mj::MString& funName,Args& ... args){
        if(mFunMap.count(funName)){
            typedef std::function<void(typename paramtype<Args>::type...)> FunType;
            auto funs = mFunMap.at(funName);;
            for(auto& f : funs){
                try{
                    auto fun = mj::any_cast<FunType>(f);
                    fun(args...);
                    return true;
                }
                catch(...){
                    ;
                }
            }
            return false;
        }
        return false;
    }

    //
    // 查询函数是否存在
    //
    template<class F>
    bool QueryFun(const mj::MString& funName,F& fun){
        if(mFunMap.count(funName)){          
            auto funs = mFunMap.at(funName);;
            for(auto& f : funs){
                try{
                    fun = mj::any_cast<F>(f);
                    return true;
                }
                catch(...){
                    ;
                }
            }
            return false;
        }
        return false;
    }

    bool HasFunByName(const mj::MString&  funName){
        if(mFunMap.count(funName)){
            return true;
        }
        else{
            return false;
        }
    }

    //
    // 重置
    //
    void Reset(){
        mFunMap.clear();
    }

private:
   MMap<mj::MString,std::list<mj::MAny>>  mFunMap; // 字符串到函数的映射
};

