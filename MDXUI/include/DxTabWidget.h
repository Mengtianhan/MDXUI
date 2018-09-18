

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
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

