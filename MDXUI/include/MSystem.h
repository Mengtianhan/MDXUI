

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
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

