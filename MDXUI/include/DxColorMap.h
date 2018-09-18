


///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DxWindow.h"

namespace DxUI{
	//+-----------------------
	//
	// ²ÊºçÌõ¸¨ÖúÑÕÉ«Ó³Éä
	//
	//+------------------------
	class DXUI_API CDxColorMap :
		public TL::MTLObject
	{
	public:
		CDxColorMap();
		~CDxColorMap();

		void  SetColorRange(const DxColor& minColor, const DxColor& maxColor);
		void  SetMapRange(double minValue, double maxValue);
		DxColor GetColor(const double& val);
		DxColor GetColor(const double& val, const DxColor& defaultcolor);
		DxColor GetColor(const double& val, const double& minval, const double& maxval) const;
		DxColor GetColor(const double& val, const double& minval, const double& maxval,const DxColor& defaultcolor) const;
		double  GetMinValue() const;
		double  GetMaxValue() const;
		DxColor GetMinColor() const;
		DxColor GetMaxColor() const;

	private:
		DxColor      m_MinColor; // defualt blue
		DxColor		 m_MaxColor; // defualt green
		double       m_MinValue; // defualt -1000
		double		 m_MaxValue; // defualt 1000

		//
		// ¸¨Öú±äÁ¿
		//
	private:
		double		mMinH;
		double		mMinS;
		double		mMinL;
		double		mMaxH;
		double		mMaxS;
		double		mMaxL;
	};
}


