

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
#include "DxListItem.h"
#include "DxColorMap.h"
namespace DxUI{
	class CDxFloatWindow;
	class CDxListWindow;
	class CDxListWindowEx;
	class CDxPushButton;
	class CDxPopWindow;
	class CDxTableWindow;
}

using DxUI::CDxWindow;
using DxUI::MCurve;
using DxUI::MMarker;
using DxUI::MPlotTitle;
using DxUI::DxColor;
using DxUI::DxPointD;
using DxUI::IPainterInterface;
using DxUI::RgbI;
using DxUI::interp;
using DxUI::DXFontInfo;
using DxUI::DXAlignment;
using DxUI::CDxPushButton;
using DxUI::CDxListWindow;
using DxUI::CDxListWindowEx;
using DxUI::CDxFloatWindow;
using DxUI::CDxPopWindow;
using DxUI::CDxTableWindow;

//+---------------------------
//
// 图例视图项
//
//+-----------------------------
class DXUI_API CLenged : public DxUI::CDxListItem{
	DECLARE_CLASS(CLenged)
public:
	CLenged();
	~CLenged();
	CLenged* CreateThis();
	bool DestroyThis();
	SIZE GetHintSize() const;
	bool OnDrawItem(const RECT& rc, IPainterInterface* painter);
};



//+-----------------------------------------
//
// 绘制曲线
// 仅仅只是绘制而已附加marker功能
// 需要强大的功能需要自己子类化
// 针对曲线的快捷键操作如下
// Ctrl + 鼠标框选局部放大
// Ctrl + F1 隐藏图例
// F1  显示图例
// Delete 删除当前选中 如果选中的是mark 删除mark 如果选中的是曲线 删除曲线
// Ctrl + A 让选中的曲线高亮顶点，顶点是实心圆点
// Ctrl + X 让选中的曲线高亮顶点，顶点是X
// Ctrl + S 让选中的曲线不绘制线条，只标记顶点，顶点是实心圆点
// Ctrl + Z 清除选中曲线的高亮状态
// Ctrl + D 让选中曲线使用条绘制
// Ctrl + C 清除所有状态 效果和 Ctrl + Z
// Ctrl + F 使选中的mark在锁定和非锁定之间切换 锁定mark之后mark不可被移动
// Shit + 鼠标左键单击 在靠近的曲线上添加marker
//
//+------------------------------------------
class DXUI_API MPlotWindowBase :
	public CDxWindow
{
	DECLARE_CLASS(MPlotWindowBase)
public:
	enum NormalMarkerType{
		Marker_Max = 0,
		Marker_Min,
	};

	enum CurveMarkPointType{
		RoundPoint = 0,
		CrossPoint
	};
public:
	MPlotWindowBase();
	~MPlotWindowBase();

	void		PlotLine(const MString& Name, const TL::Vector<double>& x, const TL::Vector<double>& y, const DxColor& col = RgbI(0, 255, 0), int size = 1);
	void		PlotLine(const MString& Name, const TL::Vector<double>& y, const DxColor& col = RgbI(0, 255, 0), int size = 1);
	void		PlotLine(const MString& Name, const TL::Vector<DxPointD>& y, const DxColor& col = RgbI(0, 255, 0), int size = 1);

	//
	// 添加竖线
	//
	void		PlotVerticalLine(const MString& Name, double x, const DxColor& col = RgbI(0, 255, 0), int size = 1);


	//
	// 在指定曲线后面追加数据
	//
	void		Append(const MString& Name, const TL::Vector<double>& x, const TL::Vector<double>& y);
	void		Append(const MString& Name, const TL::Vector<double>& y);
	void		Append(const MString& Name, double x, double y);
	void		Append(const MString& Name, double y);

	void		AddMarker(const MString& Name, double x, double y, const DxColor& col = RgbI(0, 0, 0));
	void		AddMarker(const MString& Name, double x, double y, const MString& bitmap);
	void		AddMarkerForCurve(const MString& curveName, const MString& markername, double x, const DxColor& col = RgbI(0, 0, 0));
	void		AddMarkerForCurve(const MString& curveName, const MString& markername, double x, const MString& bitmap);
	void		AddMaxMinMarker(const MString& curveName, const MString& markername, NormalMarkerType type, const DxColor& col = RgbI(0, 0, 0));
	void		AddMaxMinMarker(const MString& curveName, const MString& markername, NormalMarkerType type, const MString& bitmap);
	void		SetMarkerFixEnabel(const MString& markername, bool isFix);  // 将marker设置为锁定或者不锁定
	void		ClearFixMarkers(const MString& curveName); // 清除指定曲线上面锁定的marker

	void		FixHorizonAxis(bool isFix, double minx, double maxx);
	void		FixVerticalAxis(bool isFix, double miny, double maxy);
	void		AutoScale();

	void		RemoveMarker(const MString& Name);
	void		RemoveCurve(const MString& Name);
	void		RemoveVerticalCurve(const MString& Name);
	MCurve*		GetCurve(const MString& Name);
	MMarker*	GetMarker(const MString& Name);
	double		GetVerticalPos(const MString& Name);

	bool		CurveIsExist(const MString&  Name);

	void		Clear();

	void		SetMajorGridColor(const DxColor& col);
	void		SetMinorGridColor(const DxColor& col);
	void		SetLabelTextColor(const DxColor& col);
	void		SetPlotBackGroundColor(const DxColor& col);
	void		SetLabelTextFont(const DXFontInfo& font);

	void		SetTraceMouse(bool isTrace);
	void		SetSelectedMarkerColor(const DxColor& col);
	void		SetSelectedCurveSize(int size);


	//
	// 让顶点高亮起来
	//
	void		SetHighLightPointable(bool isMarkPoint); //将曲线的顶点标记出来
	void		SetHighLightPointable(const MString& Name, bool isMarkPoint); // 将指定的某条曲线将顶点标记出来
	void		SetHighLightPointType(const MString& Name, CurveMarkPointType type); // 绘制实心圆点或者是叉
	void		SetHighLightPointColor(const MString& Name, const DxColor& col); // 绘制实心圆点或者是叉

	//
	// 绘制曲线时只需要标记出顶点即可
	//
	void		SetNotNeedLine(const MString& Name, bool isNoLine);


	//
	// 绘制条线线
	//
	void		SetStripLine(const MString& Name, bool isStrip);


	//
	// 设置曲线颜色和粗细
	//
	void		SetCurveColor(const MString& Name, const DxColor& col);
	void		SetCurveSize(const MString& Name, int size);


	//
	// 设置附加显示信息
	//
	void		SetAdditionInformation(const MString& Information);
	void		SetShowAdditionTextAble(bool isEnable);
	void		SetAdditionTextAreaColor(const DxColor& col);


	//
	// 设置标题
	//
	void		SetTitle(const MPlotTitle& title);
	void		SetHTitle(const MPlotTitle& title);
	void		SetVTitle(const MPlotTitle& title);

	MPlotTitle* GetTitle();
	MPlotTitle* GetHTitle();
	MPlotTitle* GetVTitle();

	const MPlotTitle* GetTitle() const;
	const MPlotTitle* GetHTitle() const;
	const MPlotTitle* GetVTitle() const;

	void		SetPlotAreaSizeBox(const RECT& sizeBox);
	void		SetShowMarkerLineEnabel(bool isEnabel);
	void		SetMarkerLineColor(const DxColor& col);
	void		SetMarkerLineIsCross(bool isCross);


	TL::Map<MString, MCurve> GetAllCurves() const;
	TL::Map<MString, MMarker> GetAllMarkers() const;

	
	void		Save(const MString& FileName);
	void		Load(const MString& FileName);

	void		OnDealSelectedChanged(int curindex, int preindex, CDxWidget* sender);
	void		OnRendWindow(IPainterInterface* painter);

	void		LeftMoveMarker(const MString& Name);
	void		RightMoveMarker(const MString& Name);

	TL::Vector<double> GetValuesFromX(double x);


msignals:
	void		SelectedChanged(MString cur, MString pre, CDxWidget* sender);
	void		MarkerPositionChanged(MString Name, double x, double y, CDxWidget* sender);
	void		VerticalLineMove(MString Name, double x,  CDxWidget* sender);
	TL::MTLVoidEvent<MString, MString, CDxWidget*> Event_SelectedChanged;
	TL::MTLVoidEvent<MString, double, double, CDxWidget*> Event_MarkerPositionChanged; // maker 移动时触发事件
	TL::MTLVoidEvent<MString, double, CDxWidget*>	Event_VerticalLineMove;



protected:
	int			GetColorId();

protected:
	void				UpdateChildWindowPos();
	bool				OnNotify(DxUI::DXNotifyUI* NotifyEvent);
	virtual void		OnDealTranslateAndScale();
	virtual void		OnDealTranslateDequeScale();
	virtual void		OnDealPoint(int x, int y);
	virtual void		OnDealMarker(int x, int y);
	virtual bool		OnDealVerticalLine(int x, int y);


protected:
	std::map<MString, MCurve>	mCurves;
	std::map<MString, MMarker>	mMarkers;
	typedef std::vector<DxPointD> CurveDataType;
	typedef TL::MSNoSqlDB<double, double, double, double> ScaleTranslateType;
	std::map<MString, CurveDataType> mNormalData;// 处理后的数据
	std::map<MString, CurveDataType> mBakData;   // 数据备份
	std::map<MString, bool>			 mShowMap;   // 是否显示该曲线
	std::map<MString, bool>			 mMarkerShowMap; // 是否显示该marker
	std::map<MString, bool>			 mShowMarkPointCurvMap; // 指定曲线是否需要标记顶点
	std::map<MString, CurveMarkPointType> mMarkPointTypeMap; // 高亮顶点的类型，实心圆点 或者 ×
	std::map<MString, DxColor>			  mMarkPointColorMap;// 高亮顶点的颜色
	std::map<MString, bool>				  mNotNeedLineCurvMap; // 不需要绘制线条，只需要绘制出顶点即可
	std::map<MString, bool>				  mStripLineMap;  // 绘制垂直竖线
	std::map<MString, MCurve>			  mVerticalLinesMap; // 绘制一些数线条

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
	DxColor						mPlotGroundColor;
	DXFontInfo					mAxisLabelFont;

	double						mMinX;
	double						mMinY;
	double						mMaxX;
	double						mMaxY;

	double						mBacMinX;
	double						mBacMinY;
	double						mBacMaxX;
	double						mBacMaxY;

	bool						bIsFixX{ false };
	bool						bIsFixY{ false };
	double						mFixMinX{ -1000 };
	double						mFixMaxX{ 1000 };
	double						mFixMinY{ -1000 };
	double						mFixMaxY{ 1000 };

	bool						bIsNeedTraceMouse{ false };
	bool						bIsTranslate{ false };
	bool						bIsClicked{ false };
	bool						bIsMarkPoint{ false };// 将顶点标记出来
	bool						bIsShowMarkerLine{ false }; // 显示标线
	bool						bIsCrossShowMarkerLine{ false };
	POINT						mMousePt;
	POINT						mClickedPt;
	POINT						mPrePt;
	DxUI::DxRectD				mBoundRect;


	int							mSelectedSize{ 3 };
	MString						mSelectedCurve;
	MString						mSelectedMarkter;
	MString						mSelectedVerticalLine;
	DxColor						mSelectedMarkerColor;
	DxColor						mMarkerOldColor;
	CDxFloatWindow*				mLengedWindow;
	CDxListWindow*				mListWindow;
	DxUI::CDxColorMap			mColorMapForCurve;
	TL::Vector<int>				mCurveColorIds;

	bool						bIsShowAdditionText{ true };
	MString						m_AdditionTextInformation;  // 附加的一些显示信息
	DxColor						m_AdditionAreaColor;
	DxColor						m_MarkerLineColor;
	DxUI::CDxEffects			m_AdditionTextEffect;

	RECT						m_PlotAreaSizeBox;

	DxUI::CDxPopWindow*			p_HelpPopWindow{ nullptr };
};

