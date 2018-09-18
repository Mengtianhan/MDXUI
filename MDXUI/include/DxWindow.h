

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DxWidget.h"
namespace DxUI{
	//+---------------------
	//
	// class CDxWindow 
	// ²»´´½¨HWND
	//
	//+---------------------
	class DXUI_API CDxWindow :
		public CDxWidget
	{
		DECLARE_CLASS(CDxWindow)
	public:
		CDxWindow();
		~CDxWindow();
		void	CreateHwnd(){}
		void	CreateHwnd(HWND parent);
		CDxWidget*	HitTest(int x, int y); 
		bool		PointInArea(int x, int y); 
	};
}


