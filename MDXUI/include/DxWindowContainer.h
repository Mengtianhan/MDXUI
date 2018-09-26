

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
#include "DxStackedWidget.h"
#include "DxPushButton.h"

namespace DxUI{
	//+------------------------------
	//
	// class CDxWindowContainer
	//
	//+------------------------------
	class DXUI_API CDxWindowContainer :
		public CDxWindow
	{
		DECLARE_CLASS(CDxWindowContainer)
	public:
		CDxWindowContainer();
		~CDxWindowContainer();

		void		PushWindow(CDxWidget* Window);
		void	    RemoveWindow(CDxWidget* Window);
		void		AddChild(CDxWidget* childWindow);
		void		RemoveChild(CDxWidget* childWindow);
		void		ShowByWindow(CDxWidget* window);
		void		ShowByIndex(int index);
		CDxWidget*  CurrentWindow() const;
		int			CurrentIndex() const;
		int			GetIndexByWindow(CDxWidget* window);
		CDxWidget*  GetWindowByIndex(int index);
		void		RemoveWindowByIndex(int index);

		int			Counts() const;
		void		SetVisible(bool isVisible);

	protected:
		void		OnPushButtonTemplate(DxUI::CDxPushButton* button);
		void		OnButtonClicked(bool isClicked, CDxWidget* sender);

	private:
		CDxStackedWidget   m_StatckedWindow;
		CDxUserButton	   m_LeftButton;
		CDxUserButton      m_RightButton;
	};

}

