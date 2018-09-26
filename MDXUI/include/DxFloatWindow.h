

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
	// CDxFloatWindow  悬浮窗口
	// 鼠标可拖动
	// 可拉动改变大小
	//
	//+--------------------------------------
	class DXUI_API CDxFloatWindow :
		public CDxWindow
	{
		DECLARE_CLASS(CDxFloatWindow)
	public:
		CDxFloatWindow();
		~CDxFloatWindow();
		DXWindowType	GetWindowSelfDesc() const;
		void			SetDragSize(int size);
		void			SetGeomety(const DxRectI& rc);
		void			SetIcon(const MString& Icon);
		CDxWidget*		HitTest(int x, int y);
		bool			PointInArea(int x, int y);
		bool			OnNotify(DXNotifyUI* NotifyEvent);
		DxRectI			GetInvalidateRect() const;
		void			OnRendWindow(IPainterInterface* painter);

	protected:
		void			UpdateChildWindowPos();
		void			UpdateArea();
	protected:
		bool			bIsMove{ false };
		bool			bIsReSize{ false };
		POINT			mOldPoint;
		DXDirection		mDirect;
		bool			bIsLeft;
		bool			bIsHoverClose{ false };
		DxRectI			mCloseArea;
		DxRectI			mClipArea;
		int				mDragSize{ 6 };
	};

}

