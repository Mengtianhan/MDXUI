#pragma once
#include "DxDialog.h"

namespace DxUI{
	//
	// 没有标题的对话框
	// 背景可以透明
	//
	class DXUI_API CDxNoCaptionDialog :
		public CDxDialog
	{
		DECLARE_CLASS(CDxNoCaptionDialog)
	public:
		CDxNoCaptionDialog();
		~CDxNoCaptionDialog();
		LRESULT	OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		void OnRender2D();
		void	OnRendWindow(IPainterInterface* painter);
		int		Exec();
	};
}


