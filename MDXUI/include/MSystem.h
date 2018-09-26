

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

#include <Windows.h>
#include <MTLObject.h>
#include <map>

#ifdef __MAPI_DEBUG__
#define DXUI_SYS_API 
#else
#if defined(DIRECTUI_EXPORTS)
#define DXUI_SYS_API __declspec(dllexport)
#else
#define DXUI_SYS_API __declspec(dllimport)
#endif
#endif 


using mj::MString;
class IFactoryInterface;
namespace DxUI{
	class CDxWidget;
	struct IWindow;
}

class DXUI_SYS_API MSystem : public TL::MTLObject
{
    DECLARE_CLASS(MSystem)

    typedef DxUI::IWindow*(*CreateInstanceFun)();
    typedef IFactoryInterface*(*CreateFactoryFun)();

public:
    MSystem();
    ~MSystem();

    //
    // 绑定外部App管理器
    //
    static void BindInstance(MSystem* __MSystem);

    //
    // 创建App管理的唯一实例
    //
    static MSystem* Instance();

    //
    // 销毁App管理的实例
    //
    static void DestroyInstance();


    //
    // 从dll模块中加载窗口模块
    //
	static DxUI::IWindow* LoadWindowFromDll(const MString& dllname);


    static IFactoryInterface* CreateFactory(const MString& dllname);

    //
    // 写注册表
    //
    static bool Register(HKEY syskey,
                         const MString& usekey,
                         const MString& subkey,
                         const MString& value);

    static MString ReadRegister(HKEY syskey,
                                const MString& usekey,
                                const MString& subkey);

    //
    // 卸载注册表
    //
    static bool UnRegister(HKEY syskey,const MString& useKey);

private:
    static MSystem*                                     pSystemSelf;
    static bool                                         bInstanceIsThis;
    static std::map<MString,CreateInstanceFun>			mModuleFuns;
    static std::map<MString,CreateFactoryFun>			mFactoryFuns;
};








//+----------------------------------------------------
//
// 自动加载lib
//
//+----------------------------------------------------
#ifndef DIRECTUI_EXPORTS
#ifdef _WIN64

#ifdef _DEBUG
#pragma comment(lib,"MDXUId.lib")
#else
#pragma comment(lib,"MDXUI.lib")
#endif // _DEBUG

#else 

#ifdef _DEBUG
#pragma comment(lib,"MDXUId_x86.lib")
#else
#pragma comment(lib,"MDXUI_x86.lib")
#endif // _DEBUG 

#endif // _WIN64


#endif // DIRECTUI_EXPORTS