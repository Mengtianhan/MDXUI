

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution
// Copyright (C) 2017-2018 by Mengjin (sh06155@hotmail.com)
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


