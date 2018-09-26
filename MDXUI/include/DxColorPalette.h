

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

namespace DxUI{
	//+---------------------------
	//
	//
	// class CDxColorPalette
	// 调色板
	//
	//+---------------------------
	class DXUI_API CDxColorPalette :
		public CDxWindow
	{
		DECLARE_CLASS(CDxColorPalette)
	public:
		CDxColorPalette();
		~CDxColorPalette();

		void		CreateHwnd(HWND hwnd);
		DxColor		GetResult() const;


	msignals:
		//
		// connect该函数可以实时获取颜色
		//
		void		ColorChanged(DxColor col, CDxWidget* sender);
		TL::MTLVoidEvent<DxColor, CDxWidget*> Event_ColorChanged;

	protected:
		bool	OnNotify(DXNotifyUI* NotifyEvent);
		void	UpdateChildWindowPos();
		void	UpdateColor();
		void	UpdateBarColor();
		void	OnRendWindow(IPainterInterface* painter);
	private:
		ID2D1Bitmap*		pBitmap{ nullptr };
		ID2D1Bitmap*		pBarBitmap{ nullptr };
		ID2D1RenderTarget*  pRendTargetCach{ nullptr };
		DxRectI				mColorRect;
		DxRectI				mSaturationRect;
		DxRectI				mSliderRect;
		DxRectI				mShowColorRect;
		DxPointI			mColorPos;
		DxPointI			mSlidePos;
		bool				bIsMoveSlider{ false };
		int					mCurH{ 200 };
		int					mCurS{ 200 };
		int					mCurB{ 100 };
		DxColor				mColor;
	};
}


