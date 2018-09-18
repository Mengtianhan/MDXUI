

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DxConfig.h"
#include "DxWidget.h"
#include "MAppWindow.h"


class DXUI_API MAppFrameUI
{
public:
	MAppFrameUI();
	virtual ~MAppFrameUI();

    ///
    /// \brief Attach
    /// \param Window
    /// 将窗口和系统框架关联起来
    ///
	void Attach(MAppWindow* Window);

    ///
    /// \brief Dettach
    /// \param window
    /// 将窗口和系统框架断开关联
    ///
	void Dettach(MAppWindow* window);

    ///
    /// \brief SaveContext
    /// \return
    /// SaveContext 将AppUI 显示的数据保存到 AppData中
    ///
    virtual bool SaveContext();


    ///
    /// \brief SetContext
    /// \return
    /// SetContext  将AppData 中的数据设置到 AppUI中
    /// 该函数在整个程序启动完成初始化后立即被执行,如果某些组件需要在这个时机初始化的化此处是最好的地方
    /// 因为该函数在整个程序中仅仅执行一次，除非认为的调用
    ///
    virtual bool SetContext();

    ///
    /// \brief ValidateData
    /// \return
    /// 校验输入信息
    ///
    virtual bool ValidateData();

    ///
    /// \brief UpdateWindow
    /// \return
    /// 更新窗口信息
    ///
    virtual bool UpdateWindow();

    ///
    /// \brief InitUI
    /// \param parent
    /// \return
    /// 初始化UI窗口,该函数在程序初始化的之初优先被调用,如果有违反初始化顺序的东西要初始化请使用其他方式
    /// 比如使用 SetContext 来进行初始化
    ///
    virtual bool InitUI(DxUI::CDxWidget* parent);

    ///
    /// \brief ToString
    /// \return
    /// 惯用接口
    ///
    virtual MString ToString(){return "MAppFrameUI";}

protected:
    TL::Vector<MAppWindow*>  mNeedOpWindows; // 保存需要更新的窗口

};
