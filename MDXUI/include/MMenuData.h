

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

#if 0
//---------------------------------------
// 用于管理菜单系统的数据结构
// 一个Menu包含一个QMenu，多个Action
// 其中还有可能包含多个MenuData
//---------------------------------------
#include <unordered_map>
#include <string>
#include <vector>

class QMenu;
class QAction;
class HMenuData
{
public:
    HMenuData();
    ~HMenuData();

    HMenuData(const HMenuData& other);
    HMenuData& operator=(const HMenuData& other);

    void   BindMenuData(const std::string& MenuName,QMenu* menu,
                        const std::string& ActionName,QAction* action);   // 设置Data的数据信息
    void   AddAction(QAction* action);                  // 保存子菜单信息
    void   AddAction(QMenu* menu,QAction* act);
    void   AddChild(HMenuData* child);
    void   AddChild(QMenu* menu,HMenuData* child);
    void   RemoveAction(QAction* action);
    void   RemoveChild(HMenuData* child);                // 移除子菜单
    std::vector<QAction*>& GetActions();                 // 获取所有Action
    std::vector<HMenuData*>& GetChildMenu();            // 获取子菜单

    std::string GetMenuName();
    std::string GetActionName();
    QMenu*   GetRootMenu();            // 获取root QMenu
    void GetMenu(QAction* action, QMenu*& menu);  // 通过Action或许拥有他的Menu
    void GetAction(QMenu* menu, QAction*& outAction);   // 根据QMenu获取对应他的Action
    void GetAction(QAction* act,QAction*& outAction);

    //-------------------------------------------
    // 移除菜单项
    //-------------------------------------------
    void   RemoveMenu(QMenu* menu);

    //------------------------------------------
    // 通过一个菜单获取一个菜单
    // 因为这个菜单可能是子菜单
    //-------------------------------------------
    void GetMenu(QMenu* menu,QMenu*& outMenu);

    //------------------------------------------
    // 通过一个字符串获取对应的Menu
    // 因为这个Menu可能是子菜单
    //------------------------------------------
    void GetMenu(const std::string& menu, QMenu*& outMenu);


private:
    std::string                 mMenuValue;
    std::string                 mActionValue;
    QMenu*                      pMenu{nullptr};
    QAction*                    pAction{nullptr};
    std::vector<QAction*>       mActions;
    std::vector<HMenuData*>     mChilds;
};

#endif