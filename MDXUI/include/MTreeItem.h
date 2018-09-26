

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


//---------------------------------------//
// 用于管理树形结构的item信息            //
// 所有参数都是以#连接的字符串           //
//---------------------------------------//
#pragma once
#include <fstream>
#include <string>
#include <vector>

class  MTreeItem
{
    friend class MTreeModuleImpl;
public:
	MTreeItem();
	~MTreeItem();

	MTreeItem(const MTreeItem& item);
	MTreeItem& operator=(const MTreeItem& item);

    //
    // 清除原始信息
    //
    void clear();



    //
    // 添加和删除需要记录的条目
    // 条目之间的关系用点隔开
    //
    bool push_back(const std::string& info);
    void push_back(std::vector<std::string>& info);
    void insert(const std::string& before,const std::string& item);
    bool remove_item(const std::string& item);



    //
    // 下面添加的item为可复选的item
    //
    void push_CheckItem(const std::string& item);
    void insert_checkItem(const std::string& before,const std::string& item);
    void push_CheckItem(std::vector<std::string>& info);




    //
    // remove_CheckItem不会删除item
    // 只是让该item失去可选择功能
    // 若要真正的删除item使用remove_item
    //
    bool remove_CheckItem(const std::string& item);



    //
    // 该函数将所有的item设置为可check或者是不可以check
    //
    void setCheckStatus(bool ischeck);



    //
    // 移除根节点下面的所有子项
    //
    void remove_root(const std::string& rootStr);


private:
    //
    // 通过键查找相对应的值
    //
    std::vector<std::string> getItems(const std::string& key);




    //
    // 判断两个item是否拥有同一个parent
    //
    bool isSameParent(const std::string& item1,const std::string& item2);



    //
    // 判断两者是否是父子关系
    //
    bool isParent(const std::string& parent,const std::string& item);

    bool isDependence(const std::string& majoy,const std::string& sub);




    //
    // 获取父节点信息
    //
    std::string getParent(const std::string& item);




    //
    // 获取自己的name
    //
    std::string getSelfName(const std::string& item);



    //
    // 获取所有信息
    //
    std::vector<std::string>& getAllItems();
    std::vector<std::pair<std::string,bool>>& getItemsInfo();



    //
    // 抛开自己的节点所剩余的信息
    // 有点像getParent
    //
    std::string removeName(const std::string& item);

    std::string getIconStr(const std::string& item);

private:
    std::vector<std::string> m_need_op_info;  //保存需要操作的信息
    std::vector<std::pair<std::string,bool>>  m_item_checkable;//保存需要添加check的item
};

