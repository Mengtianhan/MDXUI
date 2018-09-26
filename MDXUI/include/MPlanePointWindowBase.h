

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
#include "DxColorMap.h"
#include "MPlotWindow.h"
using namespace DxUI;

class MPlanePointWindowBase;

//
// 添加区域接口
// 通过该接口实现外部添加区域部件
//
struct IAddAreaInterface{
public:
	virtual ~IAddAreaInterface(){}
	virtual void SetAreaRect(const DxRectD& rc) = 0;
	virtual int OpenWindow() = 0;
	virtual void SetOwnerWindow(MPlanePointWindowBase* ownerptr) = 0;
};



//
// 绘制二维顶点
//
class DXUI_API MPlanePointWindowBase :
	public CDxWindow
{
	DECLARE_CLASS(MPlanePointWindowBase)
	typedef TL::MSNoSqlDB<double, double, double, double> ScaleTranslateType;
public:
	//
	// 顶点数据
	//
	struct MPointType
	{
		int m_id;
		double m_x;  // x 轴坐标
		double m_y;  // y 轴坐标
		double m_val{ mj::InfinityDF };// 值
	};


public:
	MPlanePointWindowBase();
	~MPlanePointWindowBase();

	void				CreateHwnd(HWND hwnd);

	//
	// 设置单元顶点
	//
	void  SetPoints(const TL::Vector<MPointType>& points);

	void  SetValue(int id, double val);
	void  SetValue(double x, double y, double val);
	void  SetPointRadius(int radius);
	void  SetPointBitmap(const MString& bitmap);
	void  LoadPointFromFile(const MString& filename);
	TL::Vector<MPointType> GetPoints() const;

	//
	// 区域操作
	//
	void  SetIsUseMouseAddArea(bool isUse);
	void  SetAddAreaWindow(IAddAreaInterface* window);
	void  AddArea(const MString& Name,const DxRectD& rc,const DxColor& col = RgbI(255,0,255));
	void  SetAreaColor(const MString& Name, const DxColor& col);
	void  RemoveArea(const MString& Name);
	void  ClearArea();
	void  SetSelectedArea(const MString& Name);
	bool  CheckAreaIsExist(const MString& Name);
	DxColor GetAreaColor(const MString& Name);
	DxRectD GetAreaRect(const MString& Name);
	MString GetSelectedArea() const;
	TL::Map<MString, DxRectD> GetArea() const;
	TL::Vector<MPointType> GetPointsFromArea(const MString& Name);



	void SetColorLabelText(const MString& text);

	void Clear();
	void ClearData();

	void SetNeedColorLabel(bool isNeed);
	void SetTraceMouse(bool isTrace);
	void SetColorMapRange(const DxColor& mincolor, const DxColor& maxcolor);
	void SetFixMinValue(bool isFixMin, double minval);
	void SetFixMaxValue(bool isFixMax, double maxval);

	void SetMajorGridColor(const DxColor& col);
	void SetMinorGridColor(const DxColor& col);
	void SetLabelTextColor(const DxColor& col);
	void SetPointGroundColor(const DxColor& col);

	void  SetDefaultColor(const DxColor& col);
	void  SetTextInfoColor(const DxColor& col);
	void  SetSelectedColor(const DxColor& col);

	void SetTitle(const MPlotTitle& title);
	void SetHTitle(const MPlotTitle& title);
	void SetVTitle(const MPlotTitle& title);

	MPlotTitle*			GetTitle();
	MPlotTitle*			GetHTitle();
	MPlotTitle*			GetVTitle();

	const MPlotTitle*	GetTitle() const;
	const MPlotTitle*	GetHTitle() const;
	const MPlotTitle*	GetVTitle() const;

	//
	// 通过该接口可以设置单元显示的id信息的效果
	//
	CDxEffects* GetPointEffects();

	int			GetCurrentID() const;
	MPointType  GetCurrentPoint() const;

protected:
	void		OnUpdateBitmap();
	void		OnRendWindow(IPainterInterface* painter);
	void		OnRestore();
	void		UpdateChildWindowPos();
	void		UpdateColor();
	void		OnDealTranslateAndScale();
	void		OnDealTranslateDequeScale();
	void        OnDealClicked(int x, int y);
	bool		OnNotify(DxUI::DXNotifyUI* NotifyEvent);
	void		OnDealSelectedChanged(int curindex, int preindex, CDxWidget* sender);
	void		UpdatePointArea();

protected:
	TL::Vector<MPointType>   m_Points; // 原始顶点
	TL::Vector<MPointType>   mNormalData;  // 归一化后的顶点
	TL::Vector<MPointType>   mBakData; // 数据备份
	int						 m_PointRadius{ -1 };
	int						 m_RendRadius{ 10 };
	MString					 m_PointBitmap;

	int						 m_CurrentID{ -1 };
	MPointType				 m_CurrentPos;
	DxColor					 m_DefaultColor;
	DxColor					 m_SelectedColor;

	TL::Map<MString, DxRectD> m_AreaRectMap;    // 区域
	TL::Map<MString, DxRectD> m_BakAreaRectMap;    // 区域
	TL::Map<MString, DxColor> m_AreaColorMap;// 区域颜色
	MString					  m_SelectedAreaName; // 选中区域的名字
	bool					  b_IsUseMouseAddArea{ false }; //是否使用鼠标拉动框选区域

	//
	// 绘图的一些相关信息
	//
protected:
	ScaleTranslateType			mScalDeque;
	ID2D1Bitmap*				pBitmap{ nullptr };
	ID2D1RenderTarget*			pRendTargetCach{ nullptr };
	MString						mUnitStr;

	MPlotTitle					mTitle;
	MPlotTitle					mHTitle;
	MPlotTitle					mVTitle; 

	DXFontInfo					mAxisLabelFont;
	DxColor						mMajorGridColor;
	DxColor						mMinorGridColor;
	DxColor						mAxisLabelColor;
	DxColor						mMinColor;
	DxColor						mMaxColor;
	DxColor						mTextInfoColor;
	DxColor						mPointGroundColor; // 绘图区域的颜色

	double						mXScaleValue{ 1.0 };
	double						mYScaleValue{ 1.0 };
	double						mXTranslate{ 0.0 };
	double						mYTranslate{ 0.0 };

	double						mMinX{ 0.0 };
	double						mMinY{ 0.0 };
	double						mMaxX{ 0.0 };
	double						mMaxY{ 0.0 };


	bool						bIsFixMinVal{ false };
	bool						bIsFixMaxVal{ false };

	double						mFixMinValue{ 0.0 };
	double						mFixMaxValue{ 0.0 };

	bool						bIsNeedColorLabel{ false };
	bool						bIsNeedTraceMouse{ false };
	bool						bIsTranslate{ false };
	bool						bIsClicked{ false };
	POINT						mMousePt;
	POINT						mClickedPt;
	POINT						mPrePt;
	DxRectD						mBoundRect;
	DxUI::CDxColorMap			mColorMap;
	CDxEffects					mPointEffects;
	XMFLOAT4X4					mTransmat; // 变换矩阵

	//
	// 显示区域信息
	//
	CDxFloatWindow*				mLengedWindow;
	CDxListWindow*				mListWindow;

	//
	// MAppWindow 
	//
	IAddAreaInterface*			p_AddAreaWindow{ nullptr };
};

