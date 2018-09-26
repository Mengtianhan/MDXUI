


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
#include "IObjectBase.h"


//
// 工厂类接口
// 模块中实现该类 然后会通过通过 CreateFactory 函数将其导出
// CreateInstance 创建其他类型
//
class __declspec(uuid("F6B5D822-355D-45B1-A96A-2E162864D832")) IFactoryInterface : public virtual IObjectBase
{
public:
    IFactoryInterface(){}
    virtual ~IFactoryInterface(){}
    virtual HRESULT __stdcall CreateInstance(REFIID riid,void** pout) = 0;
};

//
// CreateInstance 实现宏
//

#define MFACTORY_MAP_BEGIN virtual HRESULT __stdcall CreateInstance(REFIID riid,void** pout){\
    *pout = nullptr;

#define MFACTORY_MAP_END  return E_NOINTERFACE;\
    }

#define MFACTORY_INSTANCE(ClassName,iidforinstance) if(riid == iidforinstance){\
    ClassName* instance = new ClassName;\
    instance->QueryInterface(riid,pout);\
    instance->Release();\
    return S_OK;\
}
