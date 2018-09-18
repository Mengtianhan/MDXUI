

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////


#pragma once
#include "DxConfig.h"
#include "MAppWindow.h"

class MProjectType;

class DXUI_API MAppUI
{
public:
	MAppUI();
	~MAppUI();

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
	/// \brief ToString
	/// \return
	/// 惯用接口
	///
	virtual MString ToString(){ return "HAppUI"; }

	///
	/// \brief ShowWindow
	/// \param window
	/// ShowWindow 显示窗口 PushWindow 压入窗口
	///
	void ShowWindow(MAppWindow* window);

	///
	/// \brief PushWindow
	/// \param window
	/// 添加窗口
	///
	void PushWindow(MAppWindow* window);

	///
	/// \brief RemoveWindow
	/// \param window
	/// 移除窗口
	///
	void RemoveWindow(MAppWindow* window);

	///
	/// \brief CurrentWindow
	/// \return
	/// 获取当前的窗口
	///
	DxUI::CDxWidget* CurrentWindow() const;


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
	///
	virtual bool SetContext();

	///
	/// \brief LockWindow
	/// 锁住窗口信息
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
	/// 执行任意的操作
	///
	virtual void OnAnyOporator(const mj::MAny& any);


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
	/// \brief Destroy
	/// 销毁UI
	///
	virtual void Destroy();

	//
	// 获取当前工程的导航树信息
	// GetRootStrID 获取最顶层Root的StrID
	// PushNag 添加导航信息 格式 如下
	// root#child#chilchild# ........... 不带图标
	// root[图标]#child[图标]#chilchild[图标]# ..带图标
	// EraseNag 删除导航树上的项
	// SetProjectName 设置工程名
	// RegisterWindow 让窗口和item关联起来
	// UnrigsterWindow 让窗口和item不再关联
	//

	///
	/// \brief GetRootStrID
	/// \return
	/// 获取最顶层Root的StrID 通常会和工程名相关的第一项
	///
	MString GetRootStrID();

	///
	/// \brief ModifyProjectName
	/// \param NewName
	/// 对工程显示名进行修改
	///
	void ModifyProjectName(const MString& NewName); // 修改工程名

	///
	/// \brief PushNag
	/// \param Nag
	/// 添加导航item
	/// root[图标]#child[图标]#chilchild[图标]# ..带图标
	///
	void PushNag(const MString& Nag);

	///
	/// \brief PushNag
	/// \param Nags
	/// 添加一组导航item
	///
	void PushNag(const TL::Vector<MString>& Nags);

	///
	/// \brief EraseNag
	/// \param Nags
	/// 删除导航item
	///
	void EraseNag(const MString& Nags);

	///
	/// \brief SetProjectName
	/// \param ProjectName
	/// 设置工程名
	///
	void SetProjectName(const MString& ProjectName);

	///
	/// \brief RegisterWindow
	/// \param NagStrId
	/// \param window
	/// 注册窗口，让窗口和item关联起来
	/// 关联起来之后通过鼠标操作导航栏会切换至相应的窗口
	///
	void RegisterWindow(const MString& NagStrId, MAppWindow* window);

	///
	/// \brief UnrigsterWindow
	/// \param NagStrId
	/// 反注册窗口
	///
	void UnrigsterWindow(const MString& NagStrId);


	///
	/// \brief NagTreeInfo
	/// \return
	/// 获取当前工程的导航树信息
	///
	TL::Vector<MString> NagTreeInfo();

	///
	/// \brief InitUI
	/// \param parent 父窗口
	/// \param __Type 工程信息
	/// \return
	/// 初始化UI窗口
	/// 如果不需要重新创建UI，可以使用UpdateAppUI对UI进行更新
	///
	virtual bool InitUI(DxUI::CDxWidget* parent, MProjectType* __Type);
	virtual bool UpdateAppUI(MProjectType* __Type);



	///
	/// \brief InitConnect
	/// 初始化信号连接
	///
	virtual void InitConnect();



	///
	/// \brief InitUIConnect
	/// InitConnect 默认实现会管理所有组件的信号Connect
	/// 所以要重写应该重写下面的函数
	///
	virtual void InitUIConnect();

	///
	/// \brief WindowContainer
	/// \return
	/// 获取窗口容器
	///
	DxUI::CDxStackedWidget* WindowContainer();

	///
	/// \brief GetWindowFromStrId
	/// \param strId
	/// \return
	/// 通过strid获取一个窗口，前提是要注册的有此窗口
	///
	MAppWindow* GetWindowFromStrId(const MString& strId);

	///
	/// \brief GetStrIDFromWindow
	/// \param window
	/// \return
	/// 通过窗口获取对应的StrID
	MString     GetStrIDFromWindow(MAppWindow* window);

	///
	/// \brief GetProjectStrID
	/// \return
	/// 获取工程的strID
	///
	MString GetProjectStrID() const;

	///
	/// \brief ClearNagInfo
	/// 清除导航信息
	///
	void ClearNagInfo();

	///
	/// \brief Clear
	/// 清除绑定的窗口
	///
	void Clear();

protected:
	void  DealNagInfo();
	void  SetProjectStrID(const MString& strID);

protected:
	TL::Vector<MAppWindow*>		mNeedOpWindows; // 保存需要更新的窗口
	TL::Vector<MString>			mNagTreeInfo;   // 储存带图标信息的item
	TL::Vector<MString>			mNagStrIDs;     // 仅仅储存字符item
	MString						mProjectStrId;  // 工程的StrID
	MString						mProjectName;
	DxUI::CDxWidget*            pParentWindow{ nullptr };  // 保存父窗口的指针 错误提示框可能会用到
	DxUI::CDxStackedWidget*		pWindowContainer{ nullptr }; // UI组件的窗口容器
	std::map<MString, MAppWindow*> mNagWindows; // 导航树上的窗口

};

