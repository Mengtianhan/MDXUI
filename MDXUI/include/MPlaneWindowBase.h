

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
#include "MCurve.h"
namespace DxUI{
	class CDxFloatWindow;
	class CDxListWindow;
	class CDxPushButton;
	class CDxPopWindow;
	class CDxTableWindow;
}

using DxUI::CDxWindow;
using DxUI::DxColor;
using DxUI::DxPointD;
using DxUI::IPainterInterface;
using DxUI::RgbI;
using DxUI::interp;
using DxUI::DXFontInfo;
using DxUI::DXAlignment;
using DxUI::CDxPushButton;
using DxUI::CDxListWindow;
using DxUI::CDxFloatWindow;
using DxUI::CDxPopWindow;
using DxUI::CDxTableWindow;
using DxUI::MPlane;
using DxUI::MPlotTitle;


//+------------------------------------
//
// 绘制平面格子图
//
//+------------------------------------
class DXUI_API MPlaneWindowBase :
	public CDxWindow
{
	DECLARE_CLASS(MPlaneWindowBase)
public:
	struct PlaneDataType{
		TL::Vector<double> mXs;
		TL::Vector<double> mYs;
		TL::Vector<__int16> mXIndexs;
		TL::Vector<__int16> mYIndexs;
	};
	typedef TL::MSNoSqlDB<double, double, double, double> ScaleTranslateType;
	
public:
	MPlaneWindowBase();
	~MPlaneWindowBase();
	void				CreateHwnd(HWND hwnd);

	virtual void		PlotPlane(const MString& Name, const TL::Vector<double>& xs, const TL::Vector<double>& ys);
	virtual void		PlotPlane(const MString& Name, const TL::Vector<double>& xs, const TL::Vector<double>& ys, const mj::dmat& datas);
	virtual void		PlotPlane(const MPlane& plane);

	virtual void		SetDatas(const MString& Name,const mj::dmat& datas);
	virtual void		SetDataByIndex(const MString& Name, int row, int col, double val); // 设置后不会主动更新需要手动调用OnPainterEvent
	virtual void		SetDataByAxis(const MString& Name, double x, double y, double val);// 设置后不会主动更新需要手动调用OnPainterEvent

	void				SetColorLabelText(const MString& text);

	void				Clear();
	void				ClearPlaneData();

	void				SetNeedColorLabel(bool isNeed);
	void				SetTraceMouse(bool isTrace);
	void				SetColorMapRange(const DxColor& mincolor, const DxColor& maxcolor);
	void				SetFixMinValue(bool isFixMin, double minval);
	void				SetFixMaxValue(bool isFixMax, double maxval);

	void				SetMajorGridColor(const DxColor& col);
	void				SetMinorGridColor(const DxColor& col);
	void				SetLabelTextColor(const DxColor& col);
	void				SetPlaneGroundColor(const DxColor& col);

	void				SetTitle(const MPlotTitle& title);
	void				SetHTitle(const MPlotTitle& title);
	void				SetVTitle(const MPlotTitle& title);

	MPlotTitle*			GetTitle();
	MPlotTitle*			GetHTitle();
	MPlotTitle*			GetVTitle();

	const MPlotTitle*	GetTitle() const;
	const MPlotTitle*	GetHTitle() const;
	const MPlotTitle*	GetVTitle() const;

	MPlane*				GetPlane(const MString& Name);
	const MPlane*		GetPlane(const MString& Name) const;


	void		Save(const MString& FileName);
	void		Load(const MString& FileName);
protected:
	void				OnRendWindow(IPainterInterface* painter);
	void				UpdateChildWindowPos();
	void				UpdateColor();
	bool				OnNotify(DxUI::DXNotifyUI* NotifyEvent);
	virtual void		OnRestore();
	virtual void		OnDealTranslateAndScale();
	virtual void		OnDealTranslateDequeScale();
	virtual void		OnUpdateBitmap();
	virtual void		OnDataChanged(){};
	void				UnInitResource();

protected:
	std::map<MString, MPlane>		mPlaneDatas; // 原始数据
	std::map<MString, PlaneDataType> mNormalData;// 处理后的数据
	std::map<MString, PlaneDataType> mBakData;   // 数据备份
	
	MPlotTitle					mTitle;
	MPlotTitle					mHTitle;
	MPlotTitle					mVTitle;
	double						mXScaleValue{ 1.0 };
	double						mYScaleValue{ 1.0 };
	double						mXTranslate{ 0.0 };
	double						mYTranslate{ 0.0 };
	ScaleTranslateType			mScalDeque;
	DxColor						mMajorGridColor;
	DxColor						mMinorGridColor;
	DxColor						mAxisLabelColor;
	DxColor						mMinColor;
	DxColor						mMaxColor;
	DxColor						mPlaneGroundColor; // 绘图区域的颜色
	DXFontInfo					mAxisLabelFont;
	ID2D1Bitmap*				pBitmap{ nullptr };
	ID2D1RenderTarget*			pRendTargetCach{ nullptr };
	MString						mUnitStr;

	double						mMinX;
	double						mMinY;
	double						mMaxX;
	double						mMaxY;


	bool						bIsFixMinVal{ false };
	bool						bIsFixMaxVal{ false };

	double						mFixMinValue{ 0.0 };
	double						mFixMaxValue{ 0.0 };

	bool						bIsNeedColorLabel{ true };
	bool						bIsNeedTraceMouse{ false };
	bool						bIsTranslate{ false };
	bool						bIsClicked{ false };
	POINT						mMousePt;
	POINT						mClickedPt;
	POINT						mPrePt;
	DxUI::DxRectD				mBoundRect;
	DxUI::CDxEffects			m_HelpEffects;
};

