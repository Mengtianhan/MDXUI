


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


