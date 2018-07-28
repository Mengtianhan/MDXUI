#pragma once
#include "DxWindow.h"
namespace DxUI{
	//+--------------------------------------
	//
	// CDxFloatWindow  悬浮窗口
	// 鼠标可拖动
	// 可拉动改变大小
	//
	//+--------------------------------------
	class DXUI_API CDxFloatWindow :
		public CDxWindow
	{
		DECLARE_CLASS(CDxFloatWindow)
	public:
		CDxFloatWindow();
		~CDxFloatWindow();
		DXWindowType	GetWindowSelfDesc() const;
		void			SetDragSize(int size);
		void			SetGeomety(const DxRectI& rc);
		void			SetIcon(const MString& Icon);
		CDxWidget*		HitTest(int x, int y);
		bool			PointInArea(int x, int y);
		bool			OnNotify(DXNotifyUI* NotifyEvent);
		DxRectI			GetInvalidateRect() const;
		void			OnRendWindow(IPainterInterface* painter);

	protected:
		void			UpdateChildWindowPos();
		void			UpdateArea();
	protected:
		bool			bIsMove{ false };
		bool			bIsReSize{ false };
		POINT			mOldPoint;
		DXDirection		mDirect;
		bool			bIsLeft;
		bool			bIsHoverClose{ false };
		DxRectI			mCloseArea;
		DxRectI			mClipArea;
		int				mDragSize{ 6 };
	};

}

