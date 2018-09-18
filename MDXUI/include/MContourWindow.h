

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "MPlaneWindow.h"
using DxUI::DxLinesType;
using DxUI::DxLineType;
using DxUI::DxPolygonType;
using DxUI::DxPolygonsType;


class DXUI_API MContourWindow :
	public MPlaneWindow
{
	DECLARE_CLASS(MContourWindow)
	using MPlaneWindow::PlotPlane;
public:
	MContourWindow();
	~MContourWindow();
	void		SetContourLevels(int level);
	void		SetNeedLine(bool isNeed);
	void		Clear();
protected:
	void		UpdateChildWindowPos();
	void		PlotPlane(const MString& Name, const TL::Vector<double>& xs, const TL::Vector<double>& ys){}
	void		SetDataByIndex(const MString& Name, int row, int col, double val){}
	void		SetDataByAxis(const MString& Name, double x, double y, double val){}
	void		OnRendWindow(IPainterInterface* painter);
	void		OnDataChanged();
	void		OnCreateGeometry();

protected:
	int							mLevel{ 12 };
	TL::Vector<double>			mContourLevelVal;
	TL::Vector<DxColor>			mContourLevelCol;
	TL::Vector<DxLinesType>		mContourLines;
	TL::Vector<DxPolygonsType>	mContourPolygons;
	std::map<MString, PlaneDataType> mConroutData;
	int							mOldWidth{ -1 };
	int							mOldHeight{ -1 };
	double						mSizeXScale{ 1.0 };
	double						mSizeYScale{ 1.0 };
	bool						bIsNeedLine{ true };
	ID2D1Bitmap*				pContourBitmap{ nullptr };
};

