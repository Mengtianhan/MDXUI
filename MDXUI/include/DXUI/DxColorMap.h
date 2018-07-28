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
		DxColor GetColor(const double& val, const double& minval, const double& maxval) const;

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


