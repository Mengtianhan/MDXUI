

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

