#pragma once
#include "DxWindow.h"
#include "DxStackedWidget.h"
#include "DxPushButton.h"


namespace DxUI{
	class DXUI_API CDxPopWindow :
		public CDxWindow
	{
		DECLARE_CLASS(CDxPopWindow)
	public:
		CDxPopWindow();
		~CDxPopWindow();
		DXWindowType	GetWindowSelfDesc() const;
		void			CreateHwnd(HWND parent);
		HWND			GetRootHwnd() const;
		bool			IsVisible() const;
		CDxWidget*		HitTest(int x, int y);
		CDxWidget*		GetParent();
		bool			PointInArea(int x, int y);
		void			SetCaptionMinHeight(int height);
		void			SetNeedCloseButton(bool isNeed);
		void			Show();
		bool			OnNotify(DXNotifyUI* NotifyEvent);
		void			OnRendWindow(IPainterInterface* painter);
		void			UpdateChildWindowPos();

	private:
		int				mCaptionMinHeight{ 30 };
		bool			bIsNeedCloseButton{ true };
		bool			bIsHoverClose{ false };
		DxRectI			mCloseArea;
		POINT			mOldPoint;
		CDxUserButton	mMinButton;
		CDxUserButton	mMaxButton;
		CDxUserButton	mRestoreButton;
	};
}


