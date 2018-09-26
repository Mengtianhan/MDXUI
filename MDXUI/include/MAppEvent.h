

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

#include "DxConfig.h"
#include "MTreeEvent.h"


//
// 该Event仅仅只会处理导航树和Frame菜单的事件，当初设计的时候没有考虑得太多导致现在扩展名字变得有些别扭
//


class MAppManager;
class MAppCreateWindow;



//
// 事件处理类，仅处理菜单和导航事件
//
class DXUI_API MAppEvent :public MTreeEvent
{
public:

	///
	/// 拦截导航树上面的消息,对于右键操作的话第三个参数有效,否则第三个参数为默认参数
	/// 第一个参数所传递的是导航树种的item的全路径
	/// 第二个参数将表示单击还是双击还是右键
	/// 1------>单击
	/// 2------>双击
	/// 3------>右键
	/// 如果打算拦截所有的消息,只需要让hook的关键字以all开头即可
	///
	typedef std::function<void(const MString&, int, const DxUI::DxPointI&)> TREEHOOKFUN;
	typedef std::function<void()> EventFunType;

	explicit MAppEvent();
	virtual ~MAppEvent();

	void BindAppManager(MAppManager* Manager);


	
	//
	//  获取响应的函数
	//
	EventFunType GetEventFun(const std::string& funName);


	///
	/// \brief InstallCommEventFun
	/// \param CommName
	/// \param fun
	/// 绑定公共操作的一些函数
	/// 当HAppManager  InstallCommEvent HAppCommEvent 之后
	/// 便默认注册了 新建 打开 另存为和拦截所有导航栏消息的函数
	///
	void InstallCommEventFun(const MString& CommName, const MAny& fun);

	///
	/// \brief InstallEventFun
	/// \param funName 事件响应函数的名字
	/// \param fun 事件响应函数的函数指针或者函数对象
	/// EventFunType 为 void(void)
	/// 只要满足该类型的函数既可以用于该函数
	/// funName 为lua文件中所制定的事件函数名
	///
	void InstallEventFun(const mj::MString& funName, EventFunType fun);

	///
	/// \brief InstallEventFuns
	/// \param funs 注册一组函数
	///
	void InstallEventFuns(const std::map<mj::MString, EventFunType>& funs);





	///
	/// \brief SetTreeHookFun
	/// \param HookInfo 需要被拦截的信息，只要包含有该关键字信息的item都会被拦截
	/// \param fun 响应函数
	/// 如果想要取消拦截，将第二个参数设置为nullptr 即可
	///
	void SetTreeHookFun(const MString& HookInfo, TREEHOOKFUN fun);



	///
	/// \brief ClearHookInfo
	/// 清除hook信息
	/// 使所有拦截的消息统统无效
	///
	void ClearHookInfo();


	///
	/// \brief OpenProject
	/// \param ProjName
	/// \return
	/// 打开指定工程
	///
	virtual bool OpenProject(const MString& ProjName);

	//
	// 响应菜单项的事件
	//

	virtual void onNew();
	virtual void onNewProject();
	virtual void onOpen();
	virtual void onSave();
	virtual void onSaveAs();
	virtual void onSaveAll();
	virtual void onClose();
	virtual void onCloseAll();
	virtual void onExit();

	virtual void onSysOption();
	virtual void onSwitchLG();


	virtual void onAbout();
	virtual void onManual();

	virtual void onRecoverSet();
	virtual void onStart();
	virtual void onPurse();
	virtual void onStop();

	virtual void OnBindProjectType(unsigned index);

	//----------------------------------
	// 普通的成员函数
	//----------------------------------
public:
	///
	/// \brief ClickedItemStrID
	/// \return
	/// 获取鼠标点击导航栏信息的strid
	/// 仅当当前点击后测试有效
	///
	mj::MString ClickedItemStrID();



	//
	// 下面是响应导航树的事件,不推荐重写，因为此处设置得有Hook函数，如果重写没处理好将会Hook
	//
private:
	///
	/// \brief Clicked
	/// \param iteminfo  导航树的Strid
	/// \param size 长度
	/// 处理鼠标单击导航树的事件
	///
	virtual void Clicked(const char* iteminfo, __int64 size);

	///
	/// \brief DoubleClicked
	/// \param iteminfo
	/// \param size
	/// 处理鼠标双击导航树的事件
	///
	virtual void DoubleClicked(const char* iteminfo, __int64 size);

	///
	/// \brief RightClicked
	/// \param iteminfo
	/// \param size
	/// \param pos 位置，用于弹出pop菜单
	/// 处理导航树右键事件
	///
	virtual void RightClicked(const char* iteminfo, __int64 size, const DxUI::DxPointI& pos);

protected:
	mj::MString				mClickedItem;

	//
	// Hook 函数
	//
	std::map<MString, TREEHOOKFUN> mTreeHookFuns;

	//
	// 操作外部事件函数
	//
	std::map<MString, MAny>  mCommEventFun;

	MString                 mRecoderProjectPath;

	MAppCreateWindow*		p_CreateForm{ nullptr };

protected:
	std::map<std::string, EventFunType> mEventFun;
};



#define HADDMETHON(className,MethodName) \
	mEventFun[#MethodName] = std::bind(&className::MethodName, this);

#define HREMOVEMETHON(MethodName) \
	mEventFun[#MethodName] = nullptr;

