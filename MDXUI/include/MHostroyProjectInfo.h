

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
#include "DxWindow.h"
#include "DxListWindow.h"
#include "DxListItem.h"
#include "DxDataMode.h"

using DxUI::CDxWindow;
using DxUI::CDxListWindow;

class DXUI_API MHostroyProjectInfo :
	public CDxWindow
{
	DECLARE_CLASS(MHostroyProjectInfo)
public:
	MHostroyProjectInfo();
	~MHostroyProjectInfo();

	void	UpdateProjectList();
	void    OpenHostroyProject(CDxWidget* sender);
protected:
	void	OnItemClicked(int index, MString str, CDxWidget* sender);
	void	UpdateChildWindowPos();
	void	OnRendWindow(DxUI::IPainterInterface* painter);

private:
	CDxListWindow*		p_FilesList{ nullptr };
	CDxListWindow*		p_FolderList{ nullptr };
};







//+-------------------------------
//
// 按钮式Item
// class MMultLineListItem
//
//+-------------------------------
class DXUI_API MMultLineListItem : public DxUI::CDxListItem
{
	DECLARE_CLASS(MMultLineListItem)
public:
	MMultLineListItem();
	~MMultLineListItem();
	
protected:
	CDxListItem* CreateThis();
	bool		 DestroyThis();
	bool OnDrawItem(const RECT& rc, DxUI::IPainterInterface* painter);
	bool SetWindowStatus(DxUI::DXWindowStatus status);
	void SetSelected(bool selected);
	void UpdateChildWindowPos();
};


//+----------------------------------
//
// 两行的列表数据模型
// class MMultLineListData
//
//+----------------------------------
class DXUI_API MMultLineListData : public DxUI::CDxListDataMode{
public:
	MMultLineListData();
private:
	DxUI::CDxListDataMode* CreateThis();
	bool			 DestroyThis();
	int Heght() const;
};