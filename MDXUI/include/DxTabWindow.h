

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
#include "DxStackedWidget.h"
#include "DxWindow.h"
#include "DxToolBar.h"
#include "DxMiniDockContainer.h"

namespace DxUI{

	//
	// 和 CDxTabWidget的区别在于Tab头可以在四边任意方向
	// Tab 可以被删除
	//
	class DXUI_API CDxTabWindow :
		public CDxWindow
	{
		DECLARE_CLASS(CDxTabWindow)
	public:
		CDxTabWindow();
		~CDxTabWindow();
		void		SetTabItemFactory(CDxToolItem* FactoryItem);
		void		AddTab(const MString& Title, CDxWidget* window);
		void		AddTab(const MString& Icon, const MString& Title, CDxWidget* window);
		void		RemoveTab(int index);
		int			Size() const;
		void		ShowByIndex(int index);
		CDxWidget*  GetWindowByIndex(int index);
		CDxWidget*  GetCurrentWindow();
		int			GetCurrentIndex() const;
		CDxEffects* GetTabEffects() const;

	msignals:
		void  SelectedChanged(int cur,int pre,CDxWidget* sender);
		TL::MTLVoidEvent<int, int, CDxWidget*> Event_SelectedChanged;

	private:
		CDxStackedWidget		m_WindowContainer;
		CDxMiniDockContainer	m_DockWindowContainer;
		CDxToolBar				m_TabHeaderBar;
		CDxToolItem*			p_TabFactory{ nullptr };
		CDxEffects				mTabEffects;
	};

}

