

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
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

