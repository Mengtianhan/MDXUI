

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

namespace DxUI{
	class CDxFlatButton;
	//+-----------------------------------
	// 
	// CDxTabWidget 只显示一个窗口
	//
	//+-----------------------------------
	class DXUI_API CDxTabWidget : public CDxStackedWidget
	{
		DECLARE_CLASS(CDxTabWidget)
	public:
		CDxTabWidget();
		~CDxTabWidget();

		CDxEffects* GetTabEffects();
		void		SetGeomety(const DxRectI& rc);
		void		SetPosition(int x, int y);
		void		SetSize(int w, int h);
		void		SetHeaderHeight(int h);
		void		AddTab(const MString& Title, CDxWidget* window);
		void		AddTab(const MString& Icon, const MString& Title, CDxWidget* window);
		void		RemoveTab(int index);
		CDxWidget*	HitTest(int x, int y);  
		bool		PointInArea(int x, int y);

		void		ReceiveEvent(CDxUIEvent* e);
		void		OnClickedForItem(bool isClicked, CDxWidget* sender);
		void		OnRendWindow(IPainterInterface* painter);
	protected:
		virtual void			OnItemsChanged();
		void					UpdateChildWindowPos();
		void					OnDealItemForDistance(unsigned xdis);
		void					RemoveChild(CDxWidget* childWindow);
	private:
		TL::Vector<CDxFlatButton*>	mHeadItems;
		CDxDecorator				mLeftButton;
		CDxDecorator				mRightButton;
		int							mItemHeight{ 30 };
		int							mItemMaxWidth{ 0 };
		int							mStartShowIndex{ 0 };
		CDxFlatButton*				pCurrentItem{ nullptr };
		CDxEffects					mTabEffects;
	};

}

