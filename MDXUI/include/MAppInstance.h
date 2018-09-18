
///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////


#pragma once


#include "DxConfig.h"
#include "MAppEvent.h"

//#include <IDealDataInterface.h>
//
// 管理从模块中加载出来的工程信息
// 从模块中能够获取的应该有
// HAppData  模块的数据
// HAppUI    模块的UI
// HAppEvent 模块事件
//

class MAppData;
class MAppUI;
class MAppManager;
class IDealDataInterface;
class MAppEventHandle;

//
// 拦截导航树上面的消息,对于右键操作的话第三个参数有效,否则第三个参数为默认参数
// 第一个参数所传递的是导航树种的item的全路径
// 第二个参数将表示单击还是双击还是右键
// 1------>单击
// 2------>双击
// 3------>右键
//
//#ifndef TREEHOOKFUN
//typedef std::function<void(const MString&, int, const QPoint&)> TREEHOOKFUN;
//#endif

//
// 所有的模块通过一个函数创建该模块的Instance
// 函数名为 CreateInstance
// HAppInstance* 返回实例指针
// 参数为void
//

class MAppInstance;
typedef MAppInstance*(*MAppInstanceFun)();


class DXUI_API MAppInstance
{
public:
	MAppInstance(){}
	virtual ~MAppInstance(){}

	///
	/// \brief DestroyInstance
	///
	virtual void DestroyInstance(){}

	///
	/// \brief ToString
	/// \return
	/// 惯用接口
	///
	virtual MString ToString(){ return "MAppInstance"; }

	///
	/// \brief InitInstance
	/// \param pAppManager
	/// 初始化一个模块
	///
	virtual void InitInstance(MAppManager* pAppManager) = 0;

	///
	/// \brief AppData
	/// \return
	/// AppData工厂函数
	///
	virtual MAppData*  AppData() const = 0;

	///
	/// \brief AppUI
	/// \return
	/// AppUI 的工厂函数
	///
	virtual MAppUI*    AppUI() const = 0;

	///
	/// \brief AppEvent
	/// \return
	/// 模块对应的事件管理器
	///
	virtual MAppEvent* AppEvent() const = 0;


	//
	/// \brief AppEventHandler
	/// \return
	/// 通用事件处理,也许是必须,也许不是
	///
	virtual MAppEventHandle* AppEventHandler(){
		return nullptr;
	}


	///
	/// \brief AppDataDealer
	/// \return
	/// 数据处理器,也许是必须,也许不是
	///
	virtual IDealDataInterface* AppDataDealer(){
		return nullptr;
	}


	///
	/// \brief MenuEventFun
	/// \return
	/// Instance模块提供菜单事件相应模块
	/// Frame拿到该函数后会将其安装在HAppEvent中
	/// 让Action得到正确的响应
	///
	virtual std::map<MString, MAppEvent::EventFunType> MenuEventFun(){
		return std::map<MString, MAppEvent::EventFunType>();
	}

	///
	/// \brief NagHookFun
	/// \return
	/// 获取hook函数,只会拦截自己所感兴趣的事件对象
	///
	virtual std::map<MString, MAppEvent::TREEHOOKFUN> NagHookFun(){
		return std::map<MString, MAppEvent::TREEHOOKFUN>();
	}


	///
	/// \brief IsMultProject
	/// \return
	/// 是否适用于多工程,默认仅为单工程模式
	///
	virtual bool IsMultProject(){
		return false;
	}

	///
	/// \brief InstallInstanceAction
	/// 装载instance特有的操作
	/// 装载和卸载都是通过 HAppManager 的 RegisterEventFun 来完成
	/// 如果是装载 FrameUI 和 FrameData 使用 RegisterFrameData 和 RegisterFrameUI 来完成
	/// 对于FrameUI 和 FrameData 建议在 InitInstance 中注册即可,方便控制
	///
	virtual void InstallInstanceAction(MAppManager* pAppManager){}

	///
	/// \brief UnInstallInstanceAction
	/// 卸载instance特有的操作
	/// 对于卸载来说 HAppManager 的 RegisterEventFun 的第三个参数使用nullptr
	/// 如果是卸载 FrameUI 和 FrameData 使用 UnRegisterFrameData 和 UnRegisterFrameUI 来完成
	///
	virtual void UnInstallInstanceAction(MAppManager* pAppManager){}

};

