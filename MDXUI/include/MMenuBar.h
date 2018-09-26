

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
#include "DxScrollBar.h"
#include "DxWindow.h"


namespace DxUI{
	class MMenuAction;
	class MMenuWindow;
	class CDxFlatButton;


	//+--------------------------
	//
	// class MMenuBar
	// 菜单栏
	//
	//+-------------------------
	class DXUI_API MMenuBar : public CDxStackedWidget
	{
		DECLARE_CLASS(MMenuBar)
	public:
		MMenuBar();
		~MMenuBar();
		void				SetGeomety(const DxRectI& rc);
		void				SetPosition(int x, int y);
		void				SetSize(int w, int h);

		void				SetExPansion(bool isExpansion);
		bool				IsExPansion() const;
		MMenuWindow*		AddMenu(const MString& Icon, const MString& Title);
		MMenuWindow*		AddMenu(const MString& Title);
		
		void			    ShowMenuByIndex(unsigned index);

		void				ClickedIndex(unsigned index);

	protected:
		CDxWidget*			HitTest(int x, int y);
		bool				PointInArea(int x, int y);
		void				ReceiveEvent(CDxUIEvent* e);
		void				OnClickedForItem(bool isClicked, CDxWidget* sender);
		void				UpdateChildWindowPos();
		void				OnDealItemForDistance(unsigned xdis);
		void				OnRendWindow(IPainterInterface* painter);


	private:	
		TL::Vector<CDxFlatButton*>	mHeadItems;
		CDxDecorator				mExpansionButton;
		CDxFlatButton*				pUserLabel{ nullptr };
		int							mItemHeight{ 30 };
		int							mItemMaxWidth{ 0 };
		int							mStartShowIndex{ 0 };
		CDxFlatButton*				pCurrentItem{ nullptr };
		bool						bIsExPansion{ true };
		bool						bMouseIsInArea{ false };
	};

	//+---------------------------------------------
	//
	// class MMenuBar2
	//
	//+--------------------------------------------
	class DXUI_API MMenuBar2 : public CDxWindow{
		DECLARE_CLASS(MMenuBar2)
	public:
		MMenuBar2();
		~MMenuBar2();
		void				SetGeomety(const DxRectI& rc);
		void				SetPosition(int x, int y);
		void				SetSize(int w, int h);

		void				SetExPansion(bool isExpansion);
		bool				IsExPansion() const;
		MMenuWindow*		AddMenu(const MString& Icon, const MString& Title);
		MMenuWindow*		AddMenu(const MString& Title);

		MMenuWindow*		InsertMenu(const MString& Icon, const MString& Title,int pos);
		MMenuWindow*		InsertMenu(const MString& Title,int pos);

		MMenuWindow*		GetMenuFromText(const MString& Text);
		MMenuWindow*		GetMenuFromIndex(int index);

		MMenuWindow*		RemoveMenuByText(const MString& Text);
		MMenuWindow*		RemoveMenuByIndex(int index);

		int					GetIndexFromMenu(MMenuWindow* menu);

		void			    ShowMenuByIndex(unsigned index);

		void				ClickedIndex(unsigned index);
		void				UpdateUserNamer(const MString& UserName);
	protected:
		CDxWidget*			HitTest(int x, int y);
		bool				PointInArea(int x, int y);
		void				ReceiveEvent(CDxUIEvent* e);
		void				OnClickedForItem(bool isClicked, CDxWidget* sender);
		void				UpdateChildWindowPos();
		void				OnDealItemForDistance(unsigned xdis);
		void				OnRendWindow(IPainterInterface* painter);
		


	private:
		TL::Vector<CDxFlatButton*>	mHeadItems;
		CDxDecorator				mExpansionButton;
		CDxFlatButton*				pUserLabel{ nullptr };
		int							mItemHeight{ 30 };
		int							mItemMaxWidth{ 0 };
		int							mStartShowIndex{ 0 };
		CDxFlatButton*				pCurrentItem{ nullptr };
		bool						bIsExPansion{ true };
		CDxStackedWidget		    mStackedWindow;
	};

	//+--------------------------
	//
	// class MMenuWindow
	// 菜单
	// 和CDxMenu 有区别
	//
	//+-------------------------
	class DXUI_API MMenuWindow : public CDxWindow{
		DECLARE_CLASS(MMenuWindow)
	public:
		MMenuWindow();
		~MMenuWindow();

		MMenuAction*		AddAction(MMenuAction* Title);
		MMenuAction*		AddAction(const MString& Title);
		MMenuAction*		AddAction(const MString& Icon, const MString& Title);
		MMenuAction*		InsertAction(const MString& Icon, const MString& Title, int pos);
		MMenuAction*		GetActionFromText(const MString& Text);
		MMenuAction*		GetActionFromIndex(int index);
		void				RemoveActionByText(const MString& Text);
		void				RemoveActionByIndex(int index);
		
		int					GetIndexByAction(MMenuAction* action);
		void				AddSpliter();
		void				InsertSpliter(int pos);

		

	protected:
		void		UpdateChildWindowPos();
		void		OnRendWindow(IPainterInterface* painter);

	private:
		TL::Vector<MMenuAction*>  mActions;
		TL::Vector<DxLineType>	  mSpliters;
	};

	//+--------------------------
	//
	// class MMenuWindow
	// 菜单
	// 和CDxMenu 有区别
	//
	//+-------------------------
	class DXUI_API MMenuAction : public CDxWindow{
		DECLARE_CLASS(MMenuAction)
	public:
		MMenuAction();
		~MMenuAction();

		int				GetWidth() const;
		void			SetIcon(const MString& Icon);
		void			Trigger();
		void			Clicked(bool isClicked, CDxWidget* sender);  // 外部可对该信号进行connect
	protected:
		bool			OnNotify(DXNotifyUI* NotifyEvent);
		void			OnRendWindow(IPainterInterface* painter);
	};

}

