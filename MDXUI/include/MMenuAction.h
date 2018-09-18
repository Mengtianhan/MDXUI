

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////


#pragma  once

#include "DxConfig.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <MAppEvent.h>


class MMenuData;

namespace mj {
    class Lua_Fun;
}

namespace DxUI{
	class MMenuBar2;   // 菜单栏
	class MMenuAction; // 菜单项
	class MMenuWindow; // 菜单
	class CDxWidget;
}


using DxUI::MMenuBar2;
using DxUI::MMenuAction;
using DxUI::MMenuWindow;


//+---------------------------------------------------
//
// 菜单管理类
//
//+---------------------------------------------------
class DXUI_API MMenuActionManager : public TL::MTLObject
{

public:
	MMenuActionManager();
	virtual ~MMenuActionManager();

    typedef std::function<bool()> ActionEventFunType;
    //
    // 绑定事件管理服务
    // 可以替换
    //
	void BindMenuEventer(MAppEvent* Eventer);

    //
    // 该函数绑定的将是一个最基本的事件管理器
    //
	void BindBaseMenuEvent(MAppEvent* Eventer);

    //
    // 配置菜单信息
    //
    void DealMenuAction(const std::string& filename);

    //
    // 添加Separator
    //
    void AddSeparator(const std::string &menu, const std::string& beforeAct);
	void AddSeparator(const std::string& menu, MMenuAction *beforeAct, bool isafter);
	void AddSeparator(MMenuWindow *menu, MMenuAction *beforeAct);

    //
    // menu ： 标识菜单的字符串
    // action : 新添加进来的Action ,事件不由这里管理
    //
	void AddAction(const std::string& menu, MMenuAction* action);


    //
    // menu   ： 标识菜单的字符串,如果该菜单存在使用当前的菜单，如果不存在创建菜单
    // action :  标识Action的字符串,如果当前Action存在使用当前的Action，不存在则创建
    // recallFun : 事件响应的回调函数，使用该方法可以动态修改原有菜单的响应方式
    // 如果menu == toolbar 那么action将添加在工具栏上
    //
    void AddAction(const std::string &menu, const std::string& action, ActionEventFunType recallFun = nullptr);




    //
    // name： Menu的标识字符串，检查菜单是否存在，如果不存在则创建
    // 默认情况是在后面添加菜单
    //
    void AddMenu(const std::string& name,unsigned pos = -1);


    //
    // menu 标识一个有状态的Menu字符串，将会将整个Menu移除
    //
    void RemoveMenu(const std::string& menu);

    //
    // 所移除的Action是内部管理的Action
    //
    void RemoveAction(const std::string& menu,const std::string& action);

	MMenuBar2* GetMenuBar();
	void doAction(bool isClicked, DxUI::CDxWidget* sender);           // 处理内部管理的菜单事件
	void doAddAction(bool isClicked, DxUI::CDxWidget* sender);        // 处理动态创建的菜单事件

private:
    void Clear();
	void DealMenuActionHelp(const std::string &act, std::shared_ptr<mj::Lua_Fun> pLuaFun, MMenuWindow *menu);


protected:
	MMenuBar2*								  p_MenuBar{ nullptr };
    std::string                               mLg;
	std::unordered_map <std::string, MMenuWindow*> mMenuIndex;      // 在重新更新后保证菜单的显示顺序
	std::unordered_map<std::string, MMenuAction*>  mActionMap;       // ActionName ---> Action   
	std::unordered_map<MMenuAction*, std::string>  mActionManager;   // 管理所有的action事件 Action*---->相应事件函数的key
	std::unordered_map<std::string, std::pair<MMenuAction*, ActionEventFunType>> mAddActionEventManage; // 外部动态添加菜单的事件管理

    MAppEvent*                               pEventManager{nullptr};            // 事件管理器
	MAppEvent*                               pBaseEventManager{ nullptr };

};



