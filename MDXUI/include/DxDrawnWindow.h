

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


