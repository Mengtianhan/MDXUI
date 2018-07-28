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


