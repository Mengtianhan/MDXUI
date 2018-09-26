

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
	//
	// class CDxSpliterWindow
	// 分割窗口
	//
	class DXUI_API CDxSpliterWindow :
		public CDxWindow
	{
		DECLARE_CLASS(CDxSpliterWindow)
	public:
		CDxSpliterWindow();
		CDxSpliterWindow(DXDirection Direct); // 水平或者垂直
		~CDxSpliterWindow();

		void	SetSpliteDirection(DXDirection Direct);
		void	SetFirstWindowSize(int Size); // 设置一个窗口初始高度或者初始宽度
		void	SetFirstWindowRatio(double ratio); // 设置两个窗口的初始高度或者初始宽度的比例
		void	AddChild(CDxWidget* Window);
		void	UpdateChildWindowPos();
		bool	OnNotify(DXNotifyUI* NotifyEvent);
		void	OnRendWindow(IPainterInterface* painter);
	private:
		int			mFirstPartSize{ -1 };
		double		mFirstRatio{ 1.0 };
		DXDirection mExtentDirect;
		CDxWidget*  pWindow1{ nullptr };
		CDxWidget*  pWindow2{ nullptr };
		bool		bIsMove{ false };
		POINT		mOldPoint;
	};
}


