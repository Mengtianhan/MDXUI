

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
	//+-------------------
	//
	// class CDxCaret
	// 文本编辑闪烁的光标
	//
	//+--------------------
	class DXUI_API CDxCaret :
		public CDxWindow
	{
		DECLARE_CLASS(CDxCaret)
	public:
		CDxCaret();
		~CDxCaret();
		bool				SetWindowStatus(DXWindowStatus status){ return false; }
		void				SetColor(const DxColor& col);
		void				StartBlink();
		void				StopBlink();
		void				OnTimer();
		void				OnRendWindow(IPainterInterface* painter);

	private:
		bool	bIsStart{ false };
	};
}


