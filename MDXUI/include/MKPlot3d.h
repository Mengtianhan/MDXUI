

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
// K坐标三维方向图
//
//+--------------------------------
class DXUI_API MKPlot3d :
	public MDcartplot3d
{
	DECLARE_CLASS(MKPlot3d)
public:
	using MDcartplot3d::SetMeshData;
public:
	MKPlot3d();
	~MKPlot3d();
	void SetMeshData(const MMeshData& meshdata);
protected:
	void OnRender3D();
};

