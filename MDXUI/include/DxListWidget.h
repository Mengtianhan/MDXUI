

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
#include "DxScrollBar.h"

namespace DxUI{
	class CDxListItem;
	//+------------------------------------
	//
	// 列表框
	// 不同于 CDxListWindow
	// 不使用数据模型
	//
	//+-------------------------------------
	class DXUI_API CDxListWidget :
		public CDxWindow
	{
		DECLARE_CLASS(CDxListWidget)
	public:
		CDxListWidget();
		~CDxListWidget();


		void				SetMultSelectedEnable(bool isMultSelected);
		void				AddItem(CDxListItem* item);
		bool				Empty() const;
		int					Size() const;
		void				RemoveItem(int index);

		CDxListItem*		FirstItem() const;
		CDxListItem*		LastItem() const;
		CDxListItem*		GetCurrentItem() const;
		CDxListItem*		GetItemByIndex(int index) const;
		CDxListItem*		GetItemByText(const MString& Text) const;
		int					GetIndexByItem(CDxListItem* item) const;

		TL::Vector<CDxListItem*> GetAllItem() const;
		TL::Vector<CDxListItem*> GetSelected() const;

		MString				GetItemText(int index) const;
		void				SetSelected(int index);

		void				Clear();

		void				OnResizeEvent();
		void				OnForceUpdate();
		virtual void		OnItemsChanged();


		bool				OnNotify(DXNotifyUI* NotifyEvent);
		void				ReceiveEvent(CDxUIEvent* e);


		void				OnRendWindow(IPainterInterface* painter);


		int					GetMaxHeight() const; // 获取所需要的高度——不使用滚动条
		int					GetMaxWidth() const;  // 获取所需要的宽度——不使用滚动条

	msignals:
		//
		// 当选择改变时触发该函数，该函数可通过TL::MTLDataBind::Connect 进行和外部通信
		// void testFun(int index1, int index2);
		// DxUI::CDxListWindow list;
		// TL::MTLDataBind::Connect(MSIGNAL(DxUI::CDxListWindow, SelectedChanged, &list),TL::MSLOT(testFun));
		// 
		void				SelectedChanged(int curIndex, int preIndex, CDxWidget* sender);
		void				SelectedIterms(TL::Vector<int> selecteditems, CDxWidget* sender);

		TL::MTLVoidEvent<int, int, CDxWidget*>  Event_SelectedChanged;
		TL::MTLVoidEvent<TL::Vector<int>, CDxWidget*> Event_SelectedIterms;

	protected:
		void				OnDealItemForDistance(int disx, int disy);
		void				UpdateScrollBarPos();

	private:
		CDxVerticalScrollBar		mVerticalScrollBar;
		CDxHorizonScrollBar			mHorizonScrollBar;
		TL::Vector<CDxListItem*>	mItems;
		int							mItemMaxWidth{ 0 };
		int							mItemMaxHeight{ 0 };
		int							mXCurrentOffset{ 0 };		// 当前偏移距离
		int							mYCurrentOffset{ 0 };		// 当前偏移距离
		int							mXMaxOffset{ 0 };			// 最大偏移距离
		int							mYMaxOffset{ 0 };			// 最大偏移距离
		int							mCurrentSelectedIndex{ -1 }; // 当前选中的索引
		TL::Vector<int>				mSelectedIndexs;             // 当前所有选中的索引
		bool						bIsMultSelectedEnabel{ false };
		CDxListItem*				pCurrentItem{ nullptr };	 // 当前选中的item
		
	};
}


