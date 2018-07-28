#pragma once
#include "DxFloatWindow.h"
using DxUI::CDxWindow;
using DxUI::DxColor;
using DxUI::DxPointD;
using DxUI::IPainterInterface;
using DxUI::RgbI;
using DxUI::DXFontInfo;
using DxUI::DXAlignment;
using DxUI::DXNotifyUI;
using DxUI::DXWindowStatus;

//+------------------------------------------
//
// 对基础控件 CDxFloatWindow 的进一步封装
//
//+-------------------------------------------
class DXUI_API MFlotWindow :
	public DxUI::CDxFloatWindow
{
	DECLARE_CLASS(MFlotWindow)
public:
	MFlotWindow();
	~MFlotWindow();
	void			SetCaptionMinHeight(int height);
	void			SetNeedCloseButton(bool isNeed);
	CDxWidget*		HitTest(int x, int y);
	bool			PointInArea(int x, int y);
	bool			OnNotify(DXNotifyUI* NotifyEvent);
	void			OnRendWindow(IPainterInterface* painter);
protected:
	void			UpdateChildWindowPos();

private:
	int				mCaptionMinHeight{ 20 }; // 窗口标题栏最低高度
	bool			bIsNeedCloseButton{ true };
	int				mSlideForOpacity{ 4 };
	POINT			mSlidePos;
	bool			bIsNeedSlider{ true };
	bool			bIsMoveSlider{ false };
	DXWindowStatus	mSlideStatus;
};

