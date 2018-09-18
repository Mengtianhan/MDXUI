

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "MPlotWindow.h"


//+----------------------------
//
// MPolarPlotWindow
// 极坐标
// x 参数表示角度 deg
// y 表示长度
//
//+----------------------------
class DXUI_API MPolarPlotWindow :
	public MPlotWindow
{
	DECLARE_CLASS(MPolarPlotWindow)
public:
	MPolarPlotWindow();
	~MPolarPlotWindow();
protected:
	void		UpdateChildWindowPos();
	void		OnDealTranslateAndScale();
	void		OnDealTranslateDequeScale();
	void		OnDealPoint(int x, int y);
	void		OnDealMarker(int x, int y);
	bool		OnNotify(DxUI::DXNotifyUI* NotifyEvent);
	void		OnRendWindow(IPainterInterface* painter);

protected:
	double		mRadius{ 0 };
	std::map<MString, double> mMinSpan;
};

