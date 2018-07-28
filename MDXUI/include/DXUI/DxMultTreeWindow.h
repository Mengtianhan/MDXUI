#pragma once
#include "DxWindow.h"
#include "DxScrollBar.h"

namespace DxUI{

	class CDxTreeItem;
	class CDxTableHeader;
	//+----------------------------
	//
	// class  CDxMultTreeWindow
	// 多列树结构
	//
	//+----------------------------
	class DXUI_API CDxMultTreeWindow :
		public CDxWindow
	{
		DECLARE_CLASS(CDxMultTreeWindow)
	public:
		CDxMultTreeWindow();
		~CDxMultTreeWindow();
		DXWindowType GetWindowSelfDesc() const;

		void					SetHeaderLabelShow(bool isShow);
		void					SetHeaderLabelHeight(int height);
		void					SetHeaderEffects(const DxUI::CDxEffects& effects);
		void					SetHeaderLabels(const TL::Vector<MString>& Header);
		void					SetColumsWidth(const TL::Vector<int>& widths);
		void					SetColumCounts(int counts);

		void					AddItem(CDxTreeItem* item);

		void					Clear();

		void					OnResizeEvent();
		virtual void			OnItemsChanged();

		bool					OnNotify(DXNotifyUI* NotifyEvent);
		void					ReceiveEvent(CDxUIEvent* e);

		void					OnRendWindow(IPainterInterface* painter);


		//
		// 当鼠标点击item时触发该事件，该函数可通过TL::MTLDataBind::Connect 进行和外部通信
		// void testFun(CDxTreeItem item);
		// DxUI::CDxListWindow list;
		// TL::MTLDataBind::Connect(MSIGNAL(DxUI::CDxListWindow, ClickedItem, &list),TL::MSLOT(testFun));
		//
	msignals:
		void					ClickedItem(CDxTreeItem* item); // 单击
		void					DBClickedItem(CDxTreeItem* item);// 双击

	protected:
		void					UpdateChildWindowPos();
		void					UpdateViewWindow();
		void					OnDealItemForDistance(unsigned xdis, unsigned ydis);
		void					UpdateScrollBarPos();


	private:
		int							mHeaderHeight{ 30 };
		int							mColums{ 1 };
		TL::Vector<int>				mColumsWidths;
		TL::Vector<MString>			mHeaders;
		CDxHorizonScrollBar			mHorizonScrollBar;
		CDxVerticalScrollBar		mVerticalScrollBar;
		TL::Vector<CDxTableHeader*>	mHeaderItems;
		TL::Vector<CDxTreeItem*>	mItems;  // 仅仅记录root节点的item
		TL::Vector<CDxTreeItem*>    mAllItems; // 记录所有的item包括root
		CDxTreeItem*				pCurrentItem{ nullptr };
		CDxTableHeader*				pCurrentHeaderItem{ nullptr };
		int							mItemMaxWidth{ 0 };
		int							mItemMaxHeight{ 0 };
		int							mXCurrentOffset{ 0 };
		int							mYCurrentOffset{ 0 };
		int							mXMaxOffset{ 0 };
		int							mYMaxOffset{ 0 };
		bool						bIsShowHeader{ true };
		DxUI::CDxEffects			mHeaderEffects;
	};
}


