

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

	//+----------------------
	//
	// class CDxTopWindow
	// ¶¥²ã´°¿Ú
	//
	//+-----------------------
	class DXUI_API CDxTopWindow :
		public CDxWidget
	{
		DECLARE_CLASS(CDxTopWindow)
	public:
		CDxTopWindow();
		~CDxTopWindow();

		DXWindowType	GetWindowSelfDesc() const;
		HWND			GetRootHwnd() const;
		ID2D1HwndRenderTarget* GetRenderTarget();
		CDxWidget*		GetParent();
		void			CreateHwnd(HWND parent);
		void			SetVisible(bool isVisible);
		void			OnRendWindow(IPainterInterface* painter);
	};
}


