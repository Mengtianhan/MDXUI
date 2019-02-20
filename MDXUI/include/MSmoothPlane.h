

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
	void OnPaintEventFor2D(IPainterInterface* painter);
};

