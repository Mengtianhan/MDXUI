

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DxWindow.h"
#include "DxScrollBar.h"
#include <MMatrixSheet.h>
#include <set>
#include <MTLPairEx.h>
#include "DxTableHelp.h"

namespace DxUI{

	class CDxTableDataMode;
	class CDxTableHeaderMode;
	class CDxTableHeader;
	class CDxTableItem;
	
	//+------------------------------------
	//
	// class CDxTableWindow
	// 只创建能够显示部分的table
	// 使用的视图模型
	//
	//+------------------------------------
	class DXUI_API CDxTableWindow :
		public CDxWindow
	{
		DECLARE_CLASS(CDxTableWindow)
	public:
		typedef mj::MMatrixSheet<CDxTableDataMode*> DxDataMatType;
		typedef mj::MMatrixSheet<DxColor>			DxItemColorType;
		typedef mj::MMatrixSheet<MString>			DxStringDataType;
		typedef TL::Vector<CDxTableItem*>			DxTableItemType;
		typedef TL::Vector<CDxTableHeaderMode*>		DxHeaderDataType;
		typedef TL::Vector<CDxTableHeader*>			DxHeaderIteamType;
		typedef TL::Vector<MPairEx<int, int, CDxTableItem*>> DxTableItemMapType;
		typedef TL::Map<int, CDxTableHeader*>		DxHeaderItemMapType;
		typedef std::pair<int, int>					DxTableItemIndexType; // 两个索引决定一个item
	public:
		CDxTableWindow();
		~CDxTableWindow();

		DXWindowType			GetWindowSelfDesc() const;

		void					SetItemDataFactory(CDxTableDataMode* item);
		void					SetItemFactory(CDxTableItem* item);
		void					SetHeaderDataFactory(CDxTableHeaderMode* item);
		void					SetHeaderFactory(CDxTableHeader* item);

		void					SetTableSize(int row, int col);
		void					SetData(const TL::smat& datas);
		void					SetData(const DxDataMatType& datas);

		void					SetItemColors(const DxItemColorType& cols);

		void					InsertRow(int beforerow); // 在指定行前面插入一行
		void					InSertCol(int beforecol); // 在指定列前面插入一列
		void					InSertRows(int nCounts, int before);
		void					InSertCols(int nCounts, int before);
		void					AddRow();   // 在末尾添加一行
		void					AddCol();   // 在末尾添加一列
		void					AddRows(int nCounts);  // 删除多行
		void					AddCols(int nCounts); // 删除多列
		void					RemoveRow(int row); // 删除指定行
		void					RemoveCol(int col); // 删除指定列
		void					RemoveRows(int iStart, int nLen); // 删除多行
		void					RemoveCols(int iStart, int nLen); // 删除多列

		void					SetHorizonHeader(const TL::Vector<MString>& HeaderInfo);
		void					SetVerticalHeader(const TL::Vector<MString>& HeaderInfo);

		void					SetHorizonHeader(const DxHeaderDataType& HeaderInfo);
		void					SetVerticalHeader(const DxHeaderDataType& HeaderInfo);

		void					HorizonHeaderShowable(bool isShow);
		void					VerticalHeaderShowable(bool isShow);

		void					SetReadOnly(bool isReadOnly);

		void					ClearColumWidth();
		void					SetColumWidth(int colum, int width);

		CDxTableSelected		GetSelectedData() const;
		DxDataMatType			GetDatas() const;
		DxStringDataType		GetTextDatas() const;

		void					SetHorizonItemAutoWithContent(bool isauto);
		void					SetVerticalItemAutoWithContent(bool isauto);

		void					AutoScale(); // 根据内容自动缩放宽度

		int						GetColums() const;
		int						GetRows() const;

		void					Clear();
		void					ClearSelected();

		void					SetItem(unsigned row, unsigned col,  CDxTableDataMode* item);
		void					SetItem(unsigned row, unsigned col, const MString& Text);
		CDxTableDataMode*		GetItem(unsigned row, unsigned col);
		const CDxTableDataMode* GetItem(unsigned row, unsigned col) const;
		MString					GetItemText(unsigned row, unsigned col) const;


		bool					CopyToClipboard();  // 将选中数据复制到剪贴板中


		void					ReceiveEvent(CDxUIEvent* e);
		void					OnResizeEvent();


		bool					OnNotify(DXNotifyUI* NotifyEvent);
		void					OnRendWindow(IPainterInterface* painter);


		//
		// 选择改变时触发  SelectedChanged 事件
		// 表格大小改变时触发 TableSizeChanged 事件
		// 当某个item的文本发生改变时触发 ItemTextChanged 事件
		// 右键点击在某个单元格内触发 RightButtonClicked 事件
		//
	msignals:
		void					SelectedChanged(DxTableItemIndexType curIndex, DxTableItemIndexType preIndex,CDxWidget* sender);
		void					TableSizeChanged(int rows, int cols, CDxWidget* sender);
		void					ItemTextChanged(int row, int col, MString Text, CDxWidget* sender);
		void					RightButtonClicked(int row, int col, DxPointI pos, CDxWidget* sender);

		TL::MTLVoidEvent<DxTableItemIndexType, DxTableItemIndexType, CDxWidget*> Event_SelectedChanged;
		TL::MTLVoidEvent<int, int, CDxWidget*> Event_TableSizeChanged;
		TL::MTLVoidEvent<int, int, MString, CDxWidget*> Event_ItemTextChanged;
		TL::MTLVoidEvent<int, int, DxPointI, CDxWidget*> Event_RightButtonClicked;
		
	protected:
		void					OnItemTextChanged(MString CurText, MString PreText,CDxWidget* sender);
		void					UpdateChildWindowPos();
		void					UpdateScrollBarPos();
		virtual void			OnItemsChanged();
		virtual void			OnDealItemForDistance(unsigned disx, unsigned disy);
		CDxTableItem*			OnFindItemFromIndex(int row, int col);
		CDxTableDataMode*		GetDataItemFromTableItem(CDxTableItem* item);
		void					HideAllItem();
		
	private:
		CDxTableDataMode*		pDataFactory{ nullptr };
		CDxTableItem*			pItemFactory{ nullptr };
		CDxTableHeaderMode*		pHeaderDataFactory{ nullptr };
		CDxTableHeader*			pHeaderFactory{ nullptr };

		CDxHorizonScrollBar		mHorizonScrollBar;
		CDxVerticalScrollBar	mVerticalScrollBar;

		DxItemColorType			mColorDatas;
		DxStringDataType		mStrDatas;    // 只保存字符
		DxDataMatType			mTableDatas;
		DxTableItemType			mItems;        // Clear 不会清除该数据

		DxHeaderDataType		mHorizonHeaderDatas;
		DxHeaderDataType		mVerticalHeaderDatas;

		DxHeaderIteamType		mHorizonHeaderItems; // Clear 不会清除该数据
		DxHeaderIteamType		mVerticalHeaderItems;// Clear 不会清除该数据

		DxTableItemMapType		mShowItemIndexMap;
		DxHeaderItemMapType		mShowHorizonHeaderMap;
		DxHeaderItemMapType		mShowVerticalHeaderMap;

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


		bool					bIsReadOnly{ true };
		bool					bIsWidthIsAuto{ true };
		bool					bIsCanAuto{ true };
		bool					bIsHeightIsAuto{ true };
		bool					bIsCanAutoVertical{ true };
		bool					bIsNeedUpdateItem{ true };
		std::set<int>			mXs;
		std::set<int>			mYs;
		TL::Vector<int>			mHorizonSpans;
		TL::Vector<int>			mVerticalSpans;


		int						mPreSelectedRow{ -1 };
		int						mPreSelectedCol{ -1 };

		int						mCurrentSelectedRow{ -1 };
		int						mCurrentSelectedCol{ -1 };

		CDxTableHeader*			pCurrentHeader{ nullptr };
		CDxTableItem*			pCurrentItem{ nullptr };
		CDxTableItem*			pLastSelectItem{ nullptr }; // 搭配 pCurrentItem 用于绘图使用
		CDxTableDataMode*		pCurrentDataItem{ nullptr };
		CDxTableDataMode*		pLastSelectedDataItem{ nullptr };
		TL::Vector<CDxTableDataMode*>	mSelectedDataItems;
		TL::Vector<CDxTableItem*>		mSelectedTableItem;
		TL::Vector<CDxTableHeaderMode*> mSelectedHeaderDataItem;
		TL::Vector<CDxTableHeader*>		mSelectedHeaderItem;

		bool					bIsHideVerticalHeader{ false };
		bool					bIsHideHorizonHeader{ false };
		std::map<int, int>		mColumWidth;
	};
}


