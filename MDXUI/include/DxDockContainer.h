

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
	class CDxDockWindow;

	//+--------------------------
	//
	// Dock ´°¿ÚÈÝÆ÷
	//
	//+---------------------------
	class DXUI_API CDxDockContainer :
		public CDxWindow
	{
		DECLARE_CLASS(CDxDockContainer)
		enum DockType{
			Dock_West,
			Dock_East,
			Dock_North,
			Dock_Source,
			Dock_Center
		};
		friend class CDxDockWindow;
		
	public:
		CDxDockContainer();
		~CDxDockContainer();
		CDxWidget* HitTest(int x, int y);
		void AddChild(CDxWidget* childWindow);
		void RemoveChild(CDxWidget* childWindow);
		void Attach(CDxWidget* Window);
		CDxWidget* Dettach();
		void DockWindow(CDxDockWindow* Window, DockType Type);
		DxRectI	GetClientRect() const;
		void	SetInterVal(int interval);
		void	UpdateChildWindowPos();

	
	mevent:
		void PositionChanged(POINT pt1, POINT pt2, CDxWidget* sender);
		void VisibleChanged(bool isVisible, CDxWidget* sender);
		void MoveWindowPosition(POINT pt, CDxWidget* sender);
		void ReleaseMousePosition(POINT pt, CDxWidget* sender);
		void StartMousePosition(POINT pt, CDxWidget* sender);

	protected:
		bool	OnNotify(DXNotifyUI* NotifyEvent);
		void	OnRendWindow(IPainterInterface* painter);

	private:
		DxRectI			m_LeftArea;
		DxRectI			m_RightArea;
		DxRectI			m_TopArea;
		DxRectI			m_BottomArea;

		DxRectI			m_CenterArea;
		DxRectI			m_CLeftArea;
		DxRectI			m_CRightArea;
		DxRectI			m_CTopArea;
		DxRectI			m_CBottomArea;

		CDxEffects		m_LeftEffects;
		CDxEffects		m_RightEffects;
		CDxEffects		m_TopEffects;
		CDxEffects		m_BottomEffects;

		CDxWidget*		p_Window{ nullptr };
		CDxDockWindow*	p_OutSideDockWindow{ nullptr };
		std::map<CDxDockWindow*, DockType> mDockType;
		DockType		m_CurrentDockType{ Dock_Center };
		CDxDockWindow*	p_CurrentMoveWindow{ nullptr };
		CDxDockWindow*	p_HoverWindow{ nullptr };
		DxColor			m_SelectAreaColor;
		bool			b_IsInSideArea{ false };
		bool			b_IsDrawFloatRect{ false };
		RECT			m_InsideRect;
		int				m_Interval{ 5 };
		
	};

}

