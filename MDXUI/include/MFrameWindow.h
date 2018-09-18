

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "MMainWindow.h"
#include "IFrameWindowInterface.h"
#include "MAppManager.h"
#include "DxLayout.h"
#include "DxStackedWidget.h"
#include "MAppInfoWindow.h"
#include "DxMenu.h"
#include "MAppStartPage.h"

//
// 框架顶层窗口
//
class  DXUI_API MFrameWindow : 
	public DxUI::MMainWindow, 
	public IFrameWindowInterface
{
public:
	MFrameWindow();
	~MFrameWindow();


	//
	// 初始化窗口
	//
	void  InitWindow(MAppEvent* Event, MAppFrameData* __data = nullptr, MAppFrameUI* __ui = nullptr);


	//
	// 操作子窗口
	//
	DxUI::CDxStackedWidget*		GetWindowContainer();
	void						PushWindow(DxUI::CDxWidget* Window);
	void						RemoveWindowByWindow(DxUI::CDxWidget* Window);
	void						RemoveWindowByIndex(unsigned index);
	void						ShowWindowByIndex(unsigned index);
	void						ShowWindowByWindow(DxUI::CDxWidget* Window);
	int							SubWindowCounts();

private:
	void						OnMenuBarIndexClicked(unsigned index);
	void					    OnInfoStatusChanged(bool isSwitch, DxUI::CDxWidget* sender);

private:
	DxUI::CDxGridLayout*			p_Layout{ nullptr };
	DxUI::CDxStackedWidget*			p_MainStackedWindow{ nullptr };
	MAppInfoWindow*					p_InfoWindow{ nullptr };
	MAppStartPage*					p_StartWindow{ nullptr };
};

