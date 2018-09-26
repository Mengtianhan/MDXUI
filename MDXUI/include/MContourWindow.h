

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

