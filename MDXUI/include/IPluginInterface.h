


///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <comdef.h>
#include <MAny.h>

namespace DxUI{
	class CDxWidget;
}

//+------------------------------
//
//  IPluginInterface
//  插件接口
//
//+------------------------------
struct IPluginInterface{
    virtual ~IPluginInterface(){}


	///
	/// \brief
	/// \param riid,pout
	/// \return
	/// 其实简单点说就是类型转换而已
	/// riid == __uuidof(IWindow) 获取的对象类型是 MAppWindow*
	/// riid == __uuidof(IWidget) 获取的对象类型是 CDxWidget*
	///
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void** pout) = 0;


	///
	/// \brief
	/// \param parent
	/// 将该接口附加在指定的窗口上面
	/// 对于gui窗口插件有效
	///
	virtual void Attach(DxUI::CDxWidget* parent){}

    ///
    /// \brief DestroyInstance
	/// 销毁当前实列
    ///
    virtual void DestroyInstance(){}

    ///
    /// \brief OnPutData
    /// \param any_data
    /// 将数据传递到模块之中
    ///
    virtual void OnPutData(const mj::MAny& any_data) = 0;

    ///
    /// \brief OnGetData
    /// \return
    /// 从模块中获取数据
    ///
    virtual mj::MAny OnGetData() const = 0;

    ///
    /// \brief IsWindowPlugin
    /// \return
    /// 模块是否属于窗口模块
	/// 如果当前模块是窗口模块
	/// 需要通过上面的Attach来绑定parent
    ///
    virtual bool IsWindowPlugin() =0;

    ///
    /// \brief OpenWindow
    /// \return
    /// 打开窗口
    ///
    virtual int OpenWindow() = 0;

    ///
    /// \brief OnDestroy
    /// 销毁模块中的数据
    ///
    virtual void OnDestroy() =0;

    ///
    /// \brief OnDescribe
    /// \return
    ///
    virtual const char* OnDescribe() const = 0;

    ///
    /// \brief IsCanAcceptedData
    /// \return
    /// 是否接受消息
    ///
    virtual bool IsCanAcceptedData() const = 0;
};


