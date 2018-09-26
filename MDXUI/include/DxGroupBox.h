

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
	class CDxCheckBox;
	class CDxRadioButton;
	//+---------------------------------
	//
	// class CDxGroupBox 
	// 组合框
	// StatusChanged(bool isEnabel)
	// 当状态改变时触发该函数，该函数可通过TL::MTLDataBind::Connect 进行和外部通信
	// void testFun(bool);
	// DxUI::CDxGroupBox box;
	// TL::MTLDataBind::Connect(MSIGNAL(DxUI::CDxSpinBox, StatusChanged, &box),TL::MSLOT(testFun));
	//
	//+---------------------------------
	class DXUI_API CDxGroupBox :
		public CDxWindow
	{
		DECLARE_CLASS(CDxGroupBox)
	public:
		CDxGroupBox();
		~CDxGroupBox();


		CDxWidget*			HitTest(int x, int y);
		bool				PointInArea(int x, int y);
		CDxEffects*			GetSpliterEffects();
		void				SetSpliterHeight(int height);
		void				SetHeaderLineRetract(int distance);
		DXWindowType		GetWindowSelfDesc() const;
		DxColor				GetEraseColor() const;
		void				StatusChanged(bool isEnabel);
		void				SetCheckable(bool isable);
		bool				IsCheckable() const;
		void				SetChecked(bool isChecked);
		bool				IsChecked() const;
		void				ReceiveEvent(CDxUIEvent* e);
		void				OnRendWindow(IPainterInterface* painter);

	msignals:
		void				CheckedStatusChanged(bool isChecked, CDxWidget* sender); // 外部可连接该事件
		TL::MTLVoidEvent<bool, CDxWidget*> Event_CheckedStatusChanged;

	mevent:
		void				LayoutWindowChanged(bool isAdd, CDxWidget* window);
		void				OnDealRadioButtonEvent(bool isClicked, CDxWidget* sender);

	protected:
		void				CheckedStatus(bool isChecked, CDxWidget* sender);
		void				UpdateChildWindowPos();

	private:
		CDxCheckBox*		pCheckBox{ nullptr };
		bool				bIsCheckable{ false };
		bool				bIsChecked{ true };
		int					mHeaderTextLength{ 0 };
		int					mHeaderTextHeight{ 0 };
		ObjTraceType		mObjectTrace;
		int					m_SpliterLineSize{ 1 };
		int				    m_HeaderDistance{ 20 };
		CDxEffects			m_SpliterEffects;
		TL::Vector<CDxRadioButton*>	mRadioButtons;
	};
}


