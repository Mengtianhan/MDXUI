

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
// 二维坐标平滑的平面图
//
//+--------------------------------
class DXUI_API MSmoothPlane :
	public MDcartplot3d
{
	DECLARE_CLASS(MSmoothPlane)
public:
	using MDcartplot3d::SetMeshData;
	using MDcartplot3d::SetFrustumInfo;
public:
	MSmoothPlane();
	~MSmoothPlane();

	void SetMeshData(const MMeshData& meshdata);
	void SetFrustumInfo(double radius);

protected:
	void OnDrawOther(IPainterInterface* painter);
	void OnRender();
	void OnRender3D();
	bool OnMouseEvent(const DxUI::CDxMouseEvent& e);
};

