#pragma once
#include "DxWidget.h"
namespace DxUI{
	class CDxRadioButton;
	//+---------------------------
	//
	//  窗口布局管理
	//
	//+----------------------------
	class DXUI_API CDxLayout : public CDxWidget
	{
		DECLARE_CLASS(CDxLayout)
	public:
		CDxLayout();
		~CDxLayout();
		void		CreateHwnd();
		void		CreateHwnd(HWND parent);
		void		SetStackedWindowIsVisible(bool isVisible);
		void		SetCoordHwnd(HWND hwnd);
		void		SetOpacity(double val);
		void		UnInitResource();
		DXWindowType GetWindowSelfDesc() const;
		CDxWidget*	FindWindowFromId(int id);
		CDxWidget*	HitTest(int x, int y);  // 点击测试，通过一个点查找到相应的控件
		bool		PointInArea(int x, int y);
		void		AddWidget(CDxWidget* window, int row, int col, int rowspan, int colspan, DXAlignment alignment = Dx_AlignNormal);
		void		SetSpan(int index, int span);
		void		SetMargin(int left, int top, int right, int bottom);
		void		SetMargin(int margin);
		void		SetSeparation(int separation);
		void		SetEnabel();
		void		SetDisabel();
		TL::Vector<CDxRadioButton*> GetRadioButtons() const;
		void		LayoutWindowChanged(bool isAdd, CDxWidget* window);
		bool		HasFloatWindow();
		bool		OnNotify(DXNotifyUI* NotifyEvent);
		void		OnRendWindow(IPainterInterface* painter);

	protected:
		
		TL::MSNoSqlDB<CDxWidget*, int, int, int, int, DXAlignment>  mControls;
		int			mSeparation{ 2 };
		int			mLetMargin{ 0 };
		int			mTopMargin{ 0 };
		int			mRightMargin{ 0 };
		int			mBottomMargin{ 0 };
	};



	//+--------------------------
	//
	// 水平布局
	// 将固定宽度的放在最前或者最后
	//
	//+---------------------------
	class DXUI_API CDxHorizonLayout : public CDxLayout{
		DECLARE_CLASS(CDxHorizonLayout)
	public:
		void UpdateChildWindowPos();
	};


	//+------------------------------
	//
	// 垂直布局
	// 将固定高度放在最下或者最上
	//
	//+------------------------------
	class DXUI_API CDxVerticLayout : public CDxLayout{
		DECLARE_CLASS(CDxVerticLayout)
	public:
		void UpdateChildWindowPos();
	};


	//+------------------------
	//
	//  格子
	//
	//+------------------------
	class DXUI_API CDxGridLayout : public CDxLayout{
		DECLARE_CLASS(CDxGridLayout)
	public:
		void AddWidget(CDxWidget* window, int row, int col, int rowspan, int colspan, DXAlignment alignment = Dx_AlignNormal);
		void UpdateChildWindowPos();
	};
}

