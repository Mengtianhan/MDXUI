

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
namespace DxUI{
	//+----------------------------------
	//
	// class CDxTableItem
	//
	//+----------------------------------
	class DXUI_API CDxTableItem : public CDxEdit
	{
		DECLARE_CLASS(CDxTableItem)
		friend class CDxTableWidget;
		friend class CDxTableWindow;
	public:
		CDxTableItem();
		virtual CDxTableItem*	CreateThis();
		virtual bool			DestroyThis();
		void					SetText(const MString& content);
		void					SetAny(const mj::MAny& any);
		DXWindowType			GetWindowSelfDesc() const;
		bool					SetWindowStatus(DXWindowStatus status);
		void					KillFocus();
		bool					IsVisible() const;
		void					OnTextFormat();
		bool					OnNotify(DXNotifyUI* NotifyEvent);
		void					OnRendWindow(IPainterInterface* painter);
		DxRectI					GetInvalidateRect() const;
		//
		// 如果有打算重写item的想法的话，建议重写该函数即可，不建议重写上面的 OnRendWindow
		// rc 是可绘制区域
		// painter 绘制对象
		//
		virtual bool			OnDrawItem(const RECT& rc, IPainterInterface* painter);
	private:
		void					SetSelected(bool isSelected);
		bool					IsSelected();
	private:
		bool					bIsSelected{ false };
		POINT					mOldPoint;
		mj::MAny				mAny;
		bool					bIsDoubleClicked{ false };
	};




	//+----------------------------
	//
	// CDxTableHeader 
	//
	//+----------------------------
	class DXUI_API CDxTableHeader : public CDxLabel
	{
		DECLARE_CLASS(CDxTableHeader)
		friend class CDxTableWidget;
		friend class CDxTableWindow;
		friend class CDxMultTreeWindow;
	public:
		CDxTableHeader();	

		virtual CDxTableHeader*	CreateThis();
		virtual bool			DestroyThis();
		void					SetText(const MString& content);
		void					SetAny(const mj::MAny& any);
		DXWindowType			GetWindowSelfDesc() const;
		bool					SetWindowStatus(DXWindowStatus status);
		bool					OnNotify(DXNotifyUI* NotifyEvent);
		void					OnRendWindow(IPainterInterface* painter);
		DxRectI					GetInvalidateRect() const;
		int					    GetMinWidth();
	private:
		void					SetSelected(bool isSelected);
		bool					IsSelected();
		void					SetExtentDirect(DXDirection dirct); // 设置拉动方向
		DXDirection				GetExtentDirect() const;
		virtual bool			OnDrawItem(const RECT& rc, IPainterInterface* painter);
	private:
		bool					bIsSelected{ false };
		DXDirection				mExtentDirect;
		bool					bIsMove{ false };
		POINT					mOldPoint;
		mj::MAny				mAny;
	};
}

