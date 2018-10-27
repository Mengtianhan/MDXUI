

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
	// 放在CDxDockContainer中可以任意调整位置的窗口
	// 带有一个标题
	//
	class DXUI_API CDxDockWindow :
		public CDxWindow
	{
		DECLARE_CLASS(CDxDockWindow)
		friend class CDxDockContainer;
		friend class CDxMiniDockContainer;
	public:
		CDxDockWindow();
		~CDxDockWindow();
		CDxEffects* GetImageEffects();
		CDxEffects* GetTitleEffects();
		void AddChild(CDxWidget* childWindow);
		void RemoveChild(CDxWidget* childWindow);
		void Attach(CDxWidget* Window);
		CDxWidget* Dettach();
		void	SetCloseEnable(bool isEnable);
		void	SetMoveEnable(bool isEnable);
		bool	IsMoveEnabel() const;
		bool	IsCloseEnable() const;

		void	UpdateChildWindowPos();

	protected:
		bool	OnNotify(DXNotifyUI* NotifyEvent);
		void	OnRendWindow(IPainterInterface* painter);


	msignals:
		void PositionChanged(POINT pt1, POINT pt2,CDxWidget* sender);
		void StartMousePosition(POINT pt, CDxWidget* sender);
		void MoveWindowPosition(POINT pt, CDxWidget* sender);
		void ReleaseMousePosition(POINT pt, CDxWidget* sender);

		TL::MTLVoidEvent<POINT, POINT,CDxWidget*>  Event_PositionChanged;
		TL::MTLVoidEvent<bool, CDxWidget*>  Event_VisibleChanged;
		TL::MTLVoidEvent<POINT, CDxWidget*>  Event_StartMousePosition;
		TL::MTLVoidEvent<POINT, CDxWidget*>  Event_MoveWindowPosition;
		TL::MTLVoidEvent<POINT, CDxWidget*>  Event_ReleaseMousePosition;

	protected:
		CDxWidget*		p_Window{ nullptr };
		CDxEffects		m_ImageEffects;
		bool			bIsMove{ false };
		bool			bIsStartMove{ false };
		bool			bIsReSize{ false };
		POINT			mOldPoint;
		DXDirection		mDirect;
		bool			bIsLeft;
		bool			bIsHoverClose{ false };
		bool			bIsCloseEnable{ true };
		bool			bIsMoveEnable{ true };
	};
}


