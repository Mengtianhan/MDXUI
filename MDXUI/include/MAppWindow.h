

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "DxConfig.h"


namespace DxUI{
	class CDxStackedWidget;
	class CDxWidget;
}

//
// 整个App 的窗口都应该继承至该类
// 因为Dialog和普通窗口的显示方式就不一样
// 如果通过该类而提供统一接口会带来很多方便
//
class DXUI_API MAppWindow
{
public:
	MAppWindow();
	virtual ~MAppWindow();


	virtual int OpenWindow();

	///
	/// \brief InitConnect
	/// 初始化信号连接
	///
	virtual void InitConnect(){}


	///
	/// \brief SaveContext
	/// \return
	/// SaveContext 将AppUI 显示的数据保存到 AppData中
	///
	virtual bool SaveContext(){ return true; }

	///
	/// \brief SetContext
	/// \return
	/// SetContext  将AppData 中的数据设置到 AppUI中
	///
	virtual bool SetContext(){ return true; }

	///
	/// \brief LockWindow
	/// 锁住窗口界面
	///
	virtual void LockWindow();

	///
	/// \brief UnLockWindow
	/// 解锁窗口
	///
	virtual void UnLockWindow();

	///
	/// \brief OnAnyOporator
	/// \param any
	/// 执行一个任意操作
	///
	virtual void OnAnyOporator(const mj::MAny& any);

	///
	/// \brief ValidateData
	/// \return
	/// 校验输入数据的有效性
	///
	virtual bool ValidateData(){ return true; }

	///
	/// \brief UpdateWindow
	/// \return
	/// 更新窗口
	///
	virtual bool UpdateWindow(){ return true; }


	///
	/// \brief ToString
	/// \return
	/// 惯用接口
	///
	virtual MString ToString(){ return "MAppWindow"; }


	///
	/// \brief IsModified
	/// \return
	/// 检查窗口的信息是否有更改
	///
	virtual bool IsModified(){ return false; }

	///
	/// \brief IsCanAcceptied
	/// \return
	/// IsCanAcceptied
	///
	virtual bool IsCanAcceptied(){ return true; }



	///
	/// \brief SetWindowContainer
	/// \param Container
	/// 当将该窗口放进容器之时需要把该容器记录下来
	/// 如果需要将窗口放进窗口容器
	/// 那么久必须调用该函数保存容器的指针
	/// 因为这样做的话可以很好的控制窗口在容器中的显示
	///
	void SetWindowContainer(DxUI::CDxStackedWidget* Container);

	///
	/// \brief WindowContainer
	/// \return
	/// 获取窗口容器
	///
	DxUI::CDxStackedWidget* WindowContainer() const;


	//
	// protected 为了派生类能够进行访问
	//
protected:
	DxUI::CDxStackedWidget* pWindowContainer{ nullptr }; // 窗口容器

};


//
// 方便生成一个小函数
//
#ifndef CLASS_TOSTRING
#define CLASS_TOSTRING(className) public:\
	virtual MString ToString(){ return #className; }
#endif