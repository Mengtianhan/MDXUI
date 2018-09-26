

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

