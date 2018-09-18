

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
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