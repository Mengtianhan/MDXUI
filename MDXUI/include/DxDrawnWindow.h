

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
	//+--------------------------------------
	//
	// CDxDrawnWindow  手绘窗口窗口
	// 实现 OnPainterEvent 自行绘制
	//
	//+--------------------------------------
	class DXUI_API CDxDrawnWindow :
		public CDxWindow
	{
		DECLARE_CLASS(CDxDrawnWindow)
	public:
		CDxDrawnWindow();
		~CDxDrawnWindow();
		DXWindowType	GetWindowSelfDesc() const;
		void		    OnRendWindow(IPainterInterface* painter);

	protected:
		virtual void	OnDrawnEvent(const DxRectI& rc, IPainterInterface* painter); // 响应绘图事件
		virtual bool	OnMouseEvent(const CDxMouseEvent& e); // 响应鼠标事件
	};
}


