

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

#pragma once;

#include "DxConfig.h"
#include <vector>

class MTreeModuleImpl;
class MTreeEvent;
namespace DxUI{
	class CDxWidget;
	class CDxTreeItem;
}



class DXUI_API MTreeModule
{
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
    typedef std::function<void(const char*,__int64,DxUI::DxPointI)> A_RCLICK_RECALL;


public:
	explicit MTreeModule();
	~MTreeModule();

	DxUI::CDxWidget* TreeView();

    void DealFile(const std::string& filename);



    //
    // 更新模型
    //
    void UpdateTree();



    //
    // 重置数据模型
    //
    void Reset();



    //
    // 设置item的显示名称
    // 第一个参数为item的stringid，第二个参数为显示的名称
    //
    bool ModifyName(const std::string& strid,const std::string& ShowName);

    void SetSelectStatus(const std::string& strid,bool isselected);



    //
    // 根据显示名查找item的str id
    //
    std::string GetItemStrId(const std::string& showName);
    std::string GetItemText(const std::string& strid);



    //
    // 设置header信息
    //
    void SetHeaderLabel(const MString& header);
    void SetHeaderHide(bool isHide);

    void SetItemFont(const std::string& strid,const DxUI::DXFontInfo& font);



    //
    // 绑定鼠标事件回调函数
    //
    void BindClickFun(CLICK_RECALL fun);
    void BindDClickFun(CLICK_RECALL fun);
    void BindRClickFun(RCLICK_RECALL fun);




    //
    // 附加操作函数
    // 需要自己重新解析参数
    // 参数的每一级都是用#号分离
    //
    void BindTreeEvent(MTreeEvent* Event);
    void BindAClickFun(A_CLICK_RECALL fun);
    void BindADClickFun(A_CLICK_RECALL fun);
    void BindARClickFun(A_RCLICK_RECALL fun);

    //-------------------------------------------
    // 整体管理item信息
    //-------------------------------------------
    void push_back(const std::string& info);
    void push_back(std::vector<std::string>& info);
    void insert(const std::string& before,const std::string& item);
    bool remove_item(const std::string& item);
    void clear();
    void push_CheckItem(const std::string& item);
    void push_CheckItem(std::vector<std::string>& info);
    void insert_CheckItem(const std::string& before,const std::string& item);
    void remove_CheckItem(const std::string& item);


    void remove_root(const std::string& rootId);

    //-------------------------------------------
    // 设置item的选择状态
    //-------------------------------------------
    void SetCheckStatus(bool ischeck);

    //------------------------------------------
    // 获取被选中的item信息
    //------------------------------------------
    std::vector<std::string> GetSelectItems();

    //
    // 查询QStandarItem
    //
	void QueryStandardItem(const std::string& itemstrid, DxUI::CDxTreeItem*& outItem);




private:
    MTreeModuleImpl* pTreeImpl;
};

