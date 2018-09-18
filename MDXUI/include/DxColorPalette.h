

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
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


