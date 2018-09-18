

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
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


