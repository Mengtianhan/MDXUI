

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <comdef.h>
#include <functional>
#include "IObjectBase.h"
//
// 用作服务器接口
//
class  __declspec(uuid("8E85A3E9-5B5F-4BA7-A959-CD430D5D6D43")) IServiceInterface : public virtual IObjectBase
{
public:
    IServiceInterface(){}
    virtual ~IServiceInterface(){}
    virtual void  __stdcall OnBindDataDealFun(std::function<bool(const char*, __int64)> fun) = 0;
    virtual void  __stdcall OnBindInfoDetailFun(std::function<bool(const char*,__int64)> fun) = 0;
    virtual std::function<void(const char*, __int64)> __stdcall OnGetInputArgsFun() = 0;
};


