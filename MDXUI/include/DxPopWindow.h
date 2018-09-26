

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
#include "DxStackedWidget.h"
#include "DxPushButton.h"


namespace DxUI{
	class DXUI_API CDxPopWindow :
		public CDxWindow
	{
		DECLARE_CLASS(CDxPopWindow)
	public:
		CDxPopWindow();
		~CDxPopWindow();
		DXWindowType	GetWindowSelfDesc() const;
		void			CreateHwnd(HWND parent);
		HWND			GetRootHwnd() const;
		bool			IsVisible() const;
		CDxWidget*		HitTest(int x, int y);
		CDxWidget*		GetParent();
		bool			PointInArea(int x, int y);
		void			SetCaptionMinHeight(int height);
		void			SetNeedCloseButton(bool isNeed);
		void			Show();
		bool			OnNotify(DXNotifyUI* NotifyEvent);
		void			OnRendWindow(IPainterInterface* painter);
		void			UpdateChildWindowPos();

	private:
		int				mCaptionMinHeight{ 30 };
		bool			bIsNeedCloseButton{ true };
		bool			bIsHoverClose{ false };
		DxRectI			mCloseArea;
		POINT			mOldPoint;
		CDxUserButton	mMinButton;
		CDxUserButton	mMaxButton;
		CDxUserButton	mRestoreButton;
	};
}


