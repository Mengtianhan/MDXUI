

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
#include <MAny.h>

//
// 公共接口
// 这个仅仅只是一个纯粹的接口,目的是为方便的写模块
// 此处的默认行为将将如其函数名一样
//
interface __declspec(uuid("6046C6DB-FD61-4F06-BF7B-D7E94BCFE79E")) ICommInterface
{
    virtual bool __stdcall OnNewOperator() = 0;
    virtual bool __stdcall OnOpenOperator() = 0;
    virtual bool __stdcall OnSaveOperator() = 0;
    virtual bool __stdcall OnSaveAsOperator() = 0;
    virtual bool __stdcall OnCloseOperator() = 0;
    virtual bool __stdcall OnHelpOperator() = 0;

    //
    // 获取模块的描述信息
    //
    virtual const char* __stdcall DocumentInfo() = 0;
    //
    // 从模块中获取结果以及将外部数据和模块进行绑定
    // 对于数据绑定需要有特殊的东西支持,比如MProperty或者MPropertyGroup的子类就可以
    //
    virtual mj::MAny __stdcall OnResult(){return mj::MAny();}
    virtual void __stdcall OnBindData(const mj::MAny& __data){}
};


