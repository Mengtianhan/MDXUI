

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
// 三维坐标方向图
//
//+--------------------------------
namespace DxUI{
	class MFTFont;
}

using DxUI::MFTFont;

class DXUI_API MSpherePlot :
	public MDcartplot3d
{
	DECLARE_CLASS(MSpherePlot)
public:
	using MDcartplot3d::SetMeshData;
	using MDcartplot3d::SetFrustumInfo;
public:
	MSpherePlot();
	~MSpherePlot();
	void SetMeshData(const MMeshData& meshdata);
	void SetFrustumInfo(double radius);
protected:
	bool OnInitResource3D();
	void OnRender();
	void OnDrawOther(IPainterInterface* painter);

protected:
	std::vector<MPairEx<double, double, MString>> mSphereTexts;
	MFTFont*			p_DxFont{ nullptr };
};

