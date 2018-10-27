

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
#include "DxCheckBox.h"
namespace DxUI{
	//+------------------------------------------------------------
	//
	// 菜单项
	// 如果需要重新绘制item的外观，建议继承后重写OnDrawItem即可
	// 因为如果重写OnRendWindow其中会涉及到一些裁剪细节
	// OnDrawItem 重写该函数不用考虑裁剪细节
	//
	//+-------------------------------------------------------------
	class DXUI_API CDxAction :
		public CDxCheckBox
	{
		friend class CDxMenu;
		friend class CDxMenuForBar;
		DECLARE_CLASS(CDxAction)
	public:
		CDxAction();
		~CDxAction();
		DXWindowType	GetWindowSelfDesc() const;
		DxColor			GetEraseColor() const;
		DxRectI			GetInvalidateRect() const;
		void			SetCheckable(bool isable);
		void			SetIcon(const MString& Text);
		bool			OnNotify(DXNotifyUI* NotifyEvent);
		void			OnRendWindow(IPainterInterface* painter);
		int				GetWidth() const;
		int				GetHeight() const;
	protected:
		void			SetMenu(CDxMenu* menu);
		int				GetIconAreaWidth() const;
		void			SetIconAreaWidth(int w);
		
	protected:
		void			UpdateChildWindowPos();
		CDxMenu*		pRelativeMenu{ nullptr }; // 关联菜单
		bool			bIsNeedIconAreaW{ false };
	};


	//+--------------------------
	//
	// 只绘制一条横线
	//
	//+---------------------------
	class DXUI_API CDxSpliter : public CDxAction{
		friend class CDxMenu;
		DECLARE_CLASS(CDxSpliter)
	public:
		CDxSpliter();
		~CDxSpliter();
		int				GetHeight() const;
		void			OnRendWindow(IPainterInterface* painter);
	protected:
		void			UpdateChildWindowPos();
	};
}


