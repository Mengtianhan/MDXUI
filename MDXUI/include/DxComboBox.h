

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
	class CDxDecorator;
	class CDxListWindow;
	class CDxListItem;
	class CDxListDataMode;
	class CDxTopWindow;

	//+---------------------------------
	//
	// class CDxComboBox 
	// 下拉框
	// CDxComboBox(CDxListDataMode* dataMode = nullptr, CDxListItem* listitem = nullptr);
	// 参数决定了显示外观
	// 当选择改变时触发该函数，该函数可通过TL::MTLDataBind::Connect 进行和外部通信
	// void testFun(int index1, int index2);
	// DxUI::CDxComboBox box;
	// TL::MTLDataBind::Connect(MSIGNAL(DxUI::CDxComboBox, SelectedChanged, &box),TL::MSLOT(testFun));
	//
	//+---------------------------------
	class DXUI_API CDxComboBox :
		public CDxWidget
	{
		DECLARE_CLASS(CDxComboBox)
	public:
		CDxComboBox(CDxListDataMode* dataMode = nullptr, CDxListItem* listitem = nullptr);
		~CDxComboBox();
		void				CreateHwnd(){}
		void				CreateHwnd(HWND parent){}

		void				SetStackedWindowIsVisible(bool isVisible);
		void				SetDropListMaxHeight(int height);
		DXWindowType		GetWindowSelfDesc() const;
		SIZE				GetHintSize() const;
		CDxWidget*			HitTest(int x, int y);
		bool				PointInArea(int x, int y);
		void				AddItem(const MString& itemText);
		void				AddItem(const MString& Icon, const MString& itemText);
		void				AddItems(const TL::Vector<MString>& items);
		void				AddItemsWithIco(const TL::Vector<std::pair<MString, MString>>& items);
		void				AddAnyItem(const mj::MAny& any, const MString& itemText);

		void				RemoveItem(int index);
		CDxListDataMode*	GetCurrentItem() const;
		CDxListDataMode*	GetItemByIndex(int index) const;

		MString				GetCurrentText() const;
		int					GetCurrentIndex() const;

		void				SetCurrentByText(const MString& Text);
		void				SetCurrentByIndex(int index);

		TL::Vector<MString> GetAllItem() const;
		TL::Vector<MString> GetAllItem();


		unsigned			Size();
		unsigned			Counts();
		void				Clear();


		bool				OnNotify(DXNotifyUI* NotifyEvent);
		void				ReceiveEvent(CDxUIEvent* e);
		void				OnRendWindow(IPainterInterface* painter);
		void				UpdateChildWindowPos();

		//
		// 当前选中改变时触发 SelectedChanged 事件
		//
	msignals:
		void				SelectedChanged(int curIndex, int preIndex,CDxWidget* sender);
		TL::MTLVoidEvent<int, int, CDxWidget*> Event_SelectedChanged;
		
	private:
		CDxListDataMode*	pListDataMode{ nullptr };
		CDxListDataMode*	pCurrentDataItem;
		CDxListItem*		pListItem{ nullptr };
		CDxListWindow*		pListWindow{ nullptr };
		CDxTopWindow*		pDropWindow{ nullptr };
		CDxDecorator*		pDeracotor{ nullptr };
		int					mDropListHeight{ 200 };
		int					mDropListMaxHeight{ 200 };
		int					mCurrentIndex{ -1 };
	};

}

