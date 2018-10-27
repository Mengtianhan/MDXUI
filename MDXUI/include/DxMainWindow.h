

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
#include "DxWidget.h"


namespace DxUI{

	class CDxMiniDockContainer;
	class CDxMenuBar;
	class CDxToolBar;
	///+-----------------------------------------
	//
	// 带有标题栏以及最大最小化几个按钮的窗口
	// 作为主窗口使用
	//
	//+-------------------------------------------
	class DXUI_API CDxMainWindow : public CDxWidget
	{
		DECLARE_CLASS(CDxMainWindow)
	public:
		CDxMainWindow();
		~CDxMainWindow();
		CDxToolBar*	GetToolBar() const;
		CDxMenuBar* GetMenuBar() const;
		CDxWidget*  GetStatusBar() const;
		void		SetToolBar(CDxToolBar* toolBar);
		void		SetStatusBar(CDxWidget* statusbar);
		void		SetMenuBar(CDxMenuBar* menuBar); // 替换掉原有的菜单栏
		void		ShowMenuBar(bool isClicked);
		void		SetCentralWidget(CDxWidget* CentralWindow);
	protected:
		void UpdateChildWindowPos();

	private:
		CDxMiniDockContainer*		p_DockWindow{ nullptr };
		CDxToolBar*					p_ToolBar{ nullptr };
		CDxMenuBar*					p_MenuBar{ nullptr };
		CDxWidget*					p_StatusBar{ nullptr };
	};
}


