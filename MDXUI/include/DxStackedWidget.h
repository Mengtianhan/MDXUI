

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
	//+-----------------------------------
	// 
	// CDxStackedWidget 只显示一个窗口
	//
	//+-----------------------------------
	class DXUI_API CDxStackedWidget : public CDxWidget
	{
		DECLARE_CLASS(CDxStackedWidget)
	public:
		CDxStackedWidget();
		~CDxStackedWidget();
		DXWindowType	GetWindowSelfDesc() const;


		void		CreateHwnd();
		void		CreateHwnd(HWND parent);
		void		OnResizeEvent();
		void		SetStackedWindowIsVisible(bool isVisible);
		void		UpdateChildWindowPos();
		void		AddChild(CDxWidget* childWindow);
		void		InsertChild(CDxWidget* childWindow,int pos);
		void		RemoveChild(CDxWidget* childWindow);
		void		ShowByWindow(CDxWidget* window);
		void		ShowByIndex(int index);
		CDxWidget*  CurrentWindow() const;
		int			CurrentIndex() const;
		int			GetIndexByWindow(CDxWidget* window);
		CDxWidget*  GetWindowByIndex(int index);
		void		RemoveWindowByIndex(int index);
		void		RemoveWindowByWindow(CDxWidget* window);
		bool		HaveWindow(CDxWidget* window);
		int			Counts() const;
		void		SetVisible(bool isVisible);
		CDxWidget*	HitTest(int x, int y);
		bool		PointInArea(int x, int y);
		void		OnRendWindow(IPainterInterface* painter);
		

	protected:
		DxRectI		GetInvalidateRect() const;
		void		SetCaptionHeight(int h);
		int			GetCaptionHeight() const;
		void		SetCaption(CDxCaption* caption = nullptr);
		void		SetTitle(const MString& Title);
		void		SetIcon(const MString& Icon);

	protected:
		CDxWidget*	pCurrentWindow{ nullptr };
		int			mCurrentIndex{ -1 };
		DxRectI		mOldRect;
	};
}


