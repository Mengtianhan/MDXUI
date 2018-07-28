#pragma once
#include "DxWindow.h"

namespace DxUI{
	//+----------------------------
	//
	// class CDxProgressBar 
	// 进度条
	// 当需要对外观进行重新绘制可以重写该函数
	// OnDrawItem
	//
	//
	//+----------------------------
	class DXUI_API CDxProgressBar :
		public CDxWindow
	{
		DECLARE_CLASS(CDxProgressBar)
	public:
		CDxProgressBar();
		~CDxProgressBar();

		DXWindowType	GetWindowSelfDesc() const;
		SIZE			GetHintSize() const;
		DxColor			GetEraseColor() const;
		DxRectI			GetInvalidateRect() const;
		void			SetDirect(DXDirection direct);
		void			SetRange(double minval, double maxval);
		void			SetValue(double val);
		double			GetValue() const;
		virtual bool	OnDrawItem(const RECT& rc, IPainterInterface* painter);
		void			OnRendWindow(IPainterInterface* painter);

	private:
		DXDirection		mDirect;
		double			mMinVal;
		double			mMaxVal;
		double			mCurrentVal;
	};
}


