

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
#include "DxPopWindow.h"

namespace DxUI{
	//
	// 背景效果透明
	// RgbI(1, 5, 11) 将此颜色作为背景色 背景将透明
	//
	class DXUI_API CDxNoCaptionPopWindow :
		public CDxPopWindow
	{
		DECLARE_CLASS(CDxNoCaptionPopWindow)
	public:
		CDxNoCaptionPopWindow();
		~CDxNoCaptionPopWindow();
		void	SetTransparent();
		void	SetTransparentColor(const DxColor& col);
		DxColor GetTransparentColor() const;
		void	SetDurationTime(int times);
		void	OnTimer();
		void	CreateHwnd(HWND parent);
		void	Show();
		LRESULT	OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		void	OnRender2D();
		void	OnRendWindow(IPainterInterface* painter);
	protected:
		DxColor	mTransparentColor;
		int		mDurationTime{ -1 };
	};
}


