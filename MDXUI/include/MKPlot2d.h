

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "MDcartplot3d.h"


//+--------------------------------
//
// K坐标二维方向图
//
//+--------------------------------
class DXUI_API MKPlot2d :
	public MDcartplot3d
{
	DECLARE_CLASS(MKPlot2d)
public:
	using MDcartplot3d::SetMeshData;
	using MDcartplot3d::SetFrustumInfo;
public:
	MKPlot2d();
	~MKPlot2d();
	void SetMeshData(const MMeshData& meshdata);
	void SetFrustumInfo(double radius);
protected:
	void OnDrawOther(IPainterInterface* painter);
	bool OnMouseEvent(const DxUI::CDxMouseEvent& e);
};

