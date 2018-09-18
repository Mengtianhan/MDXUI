

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DxWindow.h"
#include "DxListWidget.h"
#include "DxListItem.h"
#include "DxStackedWidget.h"
#include "MProjectInfoWindow.h"
#include "MHostroyProjectInfo.h"
#include "MNewProjectWindow.h"


using DxUI::CDxWindow;
using DxUI::IPainterInterface;

class DXUI_API MAppInfoWindow :
	public CDxWindow
{
	DECLARE_CLASS(MAppInfoWindow)
public:
	MAppInfoWindow();
	~MAppInfoWindow();


	void	OnUpdateWindow();
	void	StatusChanged(bool isSwitch, DxUI::CDxWidget* sender);
protected:
	void	do_Item(bool isClicked, DxUI::CDxWidget* sender);
	void	do_OpenHostroy(CDxWidget* sender);

private:
	DxUI::CDxListWidget*			p_ListWindow{ nullptr };
	DxUI::CDxStackedWidget*			p_WindowContianer{ nullptr };


	//
	// 导航项
	//
	DxUI::CDxListItem*				p_SaveItem{ nullptr };
	DxUI::CDxListItem*				p_SaveAsItem{ nullptr };
	DxUI::CDxListItem*				p_OpenItem{ nullptr };
	DxUI::CDxListItem*				p_CloseItem{ nullptr };
	DxUI::CDxListItem*				p_ExitItem{ nullptr };
	DxUI::CDxListItem*				p_OperatorItem{ nullptr };
	DxUI::CDxListItem*				p_ProjectListItem{ nullptr };
	DxUI::CDxListItem*				p_InfoItem{ nullptr };
	DxUI::CDxListItem*				p_HostroyItem{ nullptr };
	DxUI::CDxListItem*				p_NewItem{ nullptr };
	DxUI::CDxListItem*				p_LoggerItem{ nullptr };

	//
	// 三个功能窗口
	//
	MNewProjectWindow*				p_NewProjectWindow{ nullptr };
	MHostroyProjectInfo*			p_HostroyWindow{ nullptr };
	MProjectInfoWindow*				p_ProjectInfoWindow{ nullptr };

};


