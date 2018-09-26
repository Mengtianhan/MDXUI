

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
#include "DxLabel.h"

namespace DxUI{
	//+------------------------------------------------------------
	//
	// 列表框项
	// 默认的列表框item限制了最大高度为26
	// 所以如果需要修改这个高度可以选择继承之后修改mFixHeight
	// 如果需要重新绘制item的外观，建议继承后重写OnDrawItem即可
	// 因为如果重写OnRendWindow其中会涉及到一些裁剪细节
	// OnDrawItem 重写该函数不用考虑裁剪细节
	//
	//+-------------------------------------------------------------
	class DXUI_API CDxListItem :
		public CDxLabel
	{
		friend class CDxListWindow;
		friend class CDxComboBox;
		friend class CDxListWidget;

		DECLARE_CLASS(CDxListItem)
	public:
		CDxListItem();

	
		virtual CDxListItem* CreateThis();
		virtual bool		 DestroyThis();
		DXWindowType		 GetWindowSelfDesc() const;  
		SIZE				 GetHintSize() const;


		int				GetWidth() const;
		int				GetWidthQuickly() const; // 快速获取宽度 未必准
		void			SetIcon(const MString& icon);
		bool			SetWindowStatus(DXWindowStatus status);

		void			SetGeomety(const DxRectI& rc);
		void			OnResizeEvent();
		void			OnTextFormat();
		void			SetText(const MString& Text);
		void			SetAny(const mj::MAny& any);

		bool			OnNotify(DXNotifyUI* NotifyEvent);


		bool			IsSelected() const;
		DxRectI			GetInvalidateRect() const;


		void			OnRendWindow(IPainterInterface* painter);

		

		//
		// 如果有打算重写item的想法的话，建议重写该函数即可，不建议重写上面的 OnRendWindow
		// rc 是可绘制区域
		// painter 绘制对象
		//
		bool			IsVisible() const;
		virtual bool	OnDrawItem(const RECT& rc, IPainterInterface* painter);



		void			Trigger(); // 触发事件 Clicked
	msignals:
		//
		// 点击Item的时候发射Clicked时间
		//
		void			Clicked(bool isClicked, CDxWidget* sender);
		void			DoubleClicked(bool isClicked, CDxWidget* sender);
		void			Checked(bool isChecked, CDxWidget* sender);  //对于可选的Item来说选中时发射的信号
		void			ClickedPoint(bool isClicked, POINT pt, CDxWidget* sender);

		TL::MTLVoidEvent<bool, CDxWidget*> Event_Clicked;
		TL::MTLVoidEvent<bool, CDxWidget*> Event_Checked;
		TL::MTLVoidEvent<bool, CDxWidget*> Event_DoubleClicked;
		TL::MTLVoidEvent<bool, POINT, CDxWidget*> Event_ClickedPoint;

	protected:
		void			UpdateChildWindowPos();
		void			SetSelected(bool selected);


		bool			bIsSelected{ false };
		int				mTextLayoutLen{ 0 };
		POINT			mOldPoint;
		mj::MAny		mAny;
	};

}

