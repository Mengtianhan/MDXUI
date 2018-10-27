

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
/********************************
 *                              *
 *     导航树形结构数据模型     *
 *                              *
 * *****************************/
#include "DxTreeWindow.h"
#include "DxTreeItem.h"
#include <memory>
#include <functional>
#include <vector>
#include "MTreeNavigation.h"
#include "MTreeItem.h"
class MTreeEvent;


//
// class MTreeModuleImpl
//
class MTreeModuleImpl {
    typedef std::tuple<std::string,std::string,std::string,void*> Tree_Data;

public:
    ///
    /// 提供双参数操作
    ///
    typedef std::function<void(const std::string,const std::string)> CLICK_RECALL;
    typedef std::function<void(const std::string,const std::string,DxUI::DxPointI)> RCLICK_RECALL;

    ///
    /// 只有一个参数
    /// 但是该参数包含了点击item的所有信息，从root到自己，每一个节点用#号分隔
    /// 需要自己解析
    /// 当确定节点数不多的时候使用上面的回调
    /// 当节点数较多的时候，使用下面的回调方式
    ///
    typedef std::function<void(const char*,__int64)> A_CLICK_RECALL;
	typedef std::function<void(const char*, __int64, DxUI::DxPointI)> A_RCLICK_RECALL;

public:
	explicit MTreeModuleImpl();
	virtual ~MTreeModuleImpl();
    DxUI::CDxTreeWindow* TreeView();


    void DealFile(const std::string& filename);




    //
    // 更新模型
    //
    void updateTree();

    //
    // 重置数据模型
    //
    void reset();

    //
    // 设置item的显示名称
    // 第一个参数为item的stringid，第二个参数为显示的名称
    //
    bool modifyName(const std::string& strid,const std::string& ShowName);

    //
    // 根据显示名查找item的str id
    //
    std::string getItemStrId(const std::string& showName);
    std::string getItemText(const std::string& strid);


    //
    // 设置header信息
    //
    void setHeaderLabel(const MString& header);
    void setHeaderHide(bool isHide);

    void setFontForItem(const MString& strId,const DxUI::DXFontInfo& font);



    //
    // 绑定鼠标事件回调函数
    //
    void bindClickFun(CLICK_RECALL fun);
    void bindDClickFun(CLICK_RECALL fun);
    void bindRClickFun(RCLICK_RECALL fun);



    //
    // 附加操作函数
    // 需要自己重新解析参数
    // 参数的每一级都是用#号分离
    //
    void bindTreeEvent(MTreeEvent* Event);
    void bindAClickFun(A_CLICK_RECALL fun);
    void bindADClickFun(A_CLICK_RECALL fun);
    void bindARClickFun(A_RCLICK_RECALL fun);



    //
    // 整体管理item信息
    //
    void push_back(const std::string& info);
    void push_back(std::vector<std::string>& info);
    void insert(const std::string& before,const std::string& item);
    bool remove_item(const std::string& item);
    void clear();
    void push_CheckItem(const std::string& item);
    void push_CheckItem(std::vector<std::string>& info);
    void insert_checkItem(const std::string& before,const std::string& item);
    void remove_CheckItem(const std::string& item);

    void remove_root(const std::string& rootStr);


    //
    // 设置item的选择状态
    //
    void setCheckStatus(bool ischeck);

    //
    // 设置项被选中状态
    //
    void setSelectStatus(const std::string& strid,bool isselected);



    //
    // 获取被选中的item信息
    //
    std::vector<std::string> getSelectItems();

    //
    //
    //
    void QueryStandardItem(const std::string& strid,DxUI::CDxTreeItem*& outItem);



protected:

    void buildTree();

    //
    // 递归处理item信息
    //
    void processItems(std::vector<std::string> info);

    //
    // 递归创建item
    //
    void create_items(const std::string& item_str);

    //
    // 创建单个item
    //
    void create_item(const std::string& item_str);

    //
    // 辅助创建单个item
    //
    void create_item_help(const std::string& parentstr,const std::string& selfstr);
    void create_item_help(DxUI::CDxTreeItem* Parentitem,const std::string& parentstr,const std::string& selfstr);



    //
    // 递归append item
    //
    void append_items(std::vector<std::string> items_info);
    void append_items(const std::string& item_info);



    //
    // 递归查询item的前世今生
    //
	void checkItemInfo(DxUI::CDxTreeItem* item);




    //
    // 设置item的check状态
    //
    void setCheckStatus(const std::string& item);



    //
    // 将items按照节点的长短排序
    //
	std::vector<std::string> sortItems(std::vector<std::pair<std::string, DxUI::CDxTreeItem*>>& items);


    MTreeNavigator*     p_tree_view{nullptr};


    MString             m_header_label;

    CLICK_RECALL        m_click_fun{nullptr};
    CLICK_RECALL        m_dclick_fun{nullptr};
    RCLICK_RECALL       m_rclick_fun{nullptr};

    A_CLICK_RECALL      m_a_click_fun{nullptr};
    A_CLICK_RECALL      m_a_dclick_fun{nullptr};
    A_RCLICK_RECALL     m_a_rclick_fun{nullptr};
    std::vector<std::pair<A_CLICK_RECALL,bool>> m_a_click_funs;
    std::vector<std::pair<A_CLICK_RECALL,bool>> m_a_dclick_funs;
    std::vector<std::pair<A_RCLICK_RECALL,bool>> m_a_rclick_funs;


    //
    // 管理item的关系链
    //
    MTreeItem*          m_item_manage; //管理item




    //
    // 顶层item
    //
	std::vector<DxUI::CDxTreeItem*>      m_last_items;  //最终的item，其实也就是最后的几个父item而已



    //
    // 保存item名称到图标信息的映射
    //
    std::vector<std::pair<std::string,std::string>> m_items_info;           //item ---> icon;



    //
    // item的string id 到item的映射
    //
	std::vector<std::pair<std::string, DxUI::CDxTreeItem*>> m_items_items;       // item->string ======= item->item
    std::map<std::string,std::string>                  m_item_names;        // str id 到显示名的映射




    //
    // 保存上面的 m_items_items 的副本
    //
	std::vector<std::pair<std::string, DxUI::CDxTreeItem*>> m_items_items_bak;  //上面的副本



    //
    // 在递归过程中进行中专的一个item管理变量
    //
	std::vector<std::pair<std::string, DxUI::CDxTreeItem*>> m_temp_items;        //在递归调用中作为临时中转的一个容器




    //
    // 一个临时item
    // 用于递归操作中作为临时替换
    //
	DxUI::CDxTreeItem*                   m_temp_item{ nullptr };              //在递归组合中作为临时的item
    std::vector<std::string>			 m_temp_str_for_item;               //在递归组合中作为临时中转的一个容器



    //
    // 保存item是否可选择
    //
    std::vector<std::pair<std::string,bool>>        m_item_checkable;

    //
    // 被选中的item信息
    //
    std::string                          m_selected_item_info;

    MTreeEvent*                          pTreeEvent{nullptr};
};

