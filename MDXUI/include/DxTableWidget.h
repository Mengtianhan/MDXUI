

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
#include "DxEdit.h"
#include <MMatrixSheet.h>
#include <set>

namespace DxUI{

	class CDxTableHeader;
	class CDxTableItem;
	
	//+-----------------------------------
	//
	// class CDxTableWidget
	// 有多少项就有多少个item
	// 可以自由定制Item
	// 如果是纯数据，在大数据下效率不如CDxTableWindow
	//
	//+-----------------------------------
	class DXUI_API CDxTableWidget : public CDxWidget
	{
		DECLARE_CLASS(CDxTableWidget)

	public:
		typedef mj::MMatrixSheet<CDxWidget*> DxTableItemMat;
		typedef TL::Vector<CDxTableHeader*>     DxLabelList;

	public:
		CDxTableWidget();
		~CDxTableWidget();

		void				CreateHwnd();
		void				CreateHwnd(HWND parent);

		void				SetTableSize(int row, int col);
		void				SetHorizonHeader(const TL::Vector<MString>& HeaderInfo);
		void				SetVerticalHeader(const TL::Vector<MString>& HeaderInfo);
		void				SetHorizonItemWidth(int w);
		void				SetHorizonItemHeight(int h);
		void				SetHorizonItemAutoWithContent(bool isauto);
		void				SetHeaderFactory(CDxTableHeader* Factor);
		int					GetColums() const;
		int					GetRows() const;

		void				SetReadOnly(bool isReadOnly);
		void				Clear();

		void				SetWidget(int row, int col, CDxWidget* item);
		void				SetItem(int row, int col, const MString& Text);
		CDxWidget*			GetItem(int row, int col);
		const CDxWidget*	GetItem(int row, int col) const;
		MString				GetItemText(int row, int col) const;
		const DxTableItemMat& GetAllItem() const;
		void				ReceiveEvent(CDxUIEvent* e);
		void				OnResizeEvent();


		void				SetAvgEnabel(bool isEnabel); // 设置横纵均等分
		void				HorizonHeaderShowable(bool isShow);
		void				VerticalHeaderShowable(bool isShow);

		bool				OnNotify(DXNotifyUI* NotifyEvent);
		void				OnRendWindow(IPainterInterface* painter);


	protected:
		void				UpdateChildWindowPos();
		void				UpdateScrollBarPos();
		virtual void		OnItemsChanged();
		virtual void		OnDealItemForLine(unsigned rowindex, unsigned colindex);
		virtual void		OnDealItemForDistance(unsigned xdis, unsigned ydis);

	private:
		CDxHorizonScrollBar		mHorizonScrollBar;
		CDxVerticalScrollBar	mVerticalScrollBar;
		DxLabelList 			mHorizonHead;
		DxLabelList				mVerticalHead;
		DxTableItemMat			mItems;
		CDxTableHeader*			mNullHeader{ nullptr };
		CDxTableHeader*			pHeaderFactor{ nullptr };
		int						mItemWidth{ 80 };
		int						mItemHeight{ 40 };
		int						mTotallWidth{ 0 };
		int						mTotallHeight{ 0 };
		int						mHorizonHeadHeight{ 40 };
		int						mVerticalHeaderWidth{ 80 };
		int						mXCurrentOffset{ 0 }; 
		int						mYCurrentOffset{ 0 };
		int						mXMaxOffset{ 0 };   
		int						mYMaxOffset{ 0 };
		int						mHorizonLength{ 0 };
		int						mVerticalHeight{ 0 };
		std::set<int>			mXs;
		std::set<int>			mYs;
		TL::Vector<int>			mHorizonSpans;
		TL::Vector<int>			mVerticalSpans;

		bool					bIsReadOnly{ true };
		bool					bIsCanAuto{ true };
		bool					bIsWidthIsAuto{ false };
		bool					bIsHideVerticalHeader{ false };
		bool					bIsHideHorizonHeader{ false };
		bool					bIsAvgSize{ false }; // 均分
		CDxTableHeader*			pCurrentHeader{ nullptr };
		CDxWidget*				pCurrentItem{ nullptr };
		CDxWidget*				pLastSelectItem{ nullptr }; // 搭配 pCurrentItem 用于绘图使用
	};



	
}


