#pragma once
#include "DxWidget.h"

namespace DxUI{
	//+-------------------
	//
	// class CDxCaret
	// 文本编辑闪烁的光标
	//
	//+--------------------
	class DXUI_API CDxCaret :
		public CDxWidget
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


