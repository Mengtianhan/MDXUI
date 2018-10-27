

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
		CDxWidget*	GetWidget(int row, int col);
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
		void		OnIdle();
		void		SetEraseColorForChildren(const DxColor& col);

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

