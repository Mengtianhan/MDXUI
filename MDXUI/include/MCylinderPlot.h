

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
// 柱面坐标三维方向图
//
//+--------------------------------
class DXUI_API MCylinderPlot :
	public MDcartplot3d
{
	DECLARE_CLASS(MCylinderPlot)
public:
	using MDcartplot3d::SetMeshData;
	using MDcartplot3d::SetFrustumInfo;
public:
	MCylinderPlot();
	~MCylinderPlot();

	void SetRadius(double radius);
	void SetMeshData(const MMeshData& meshdata);
	void SetFrustumInfo(double radius);

protected:
	void OnDrawOther(IPainterInterface* painter);
	void OnRender3D();

protected:
	double	mCylinderRadius;
};

