

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
#include "DxWidget.h"
namespace DxUI{
	class CDxDecorator;
	class CDxEdit;
	//+---------------------------------
	//
	// class CDxSpinBox 
	// 微调框
	// ValueChanged(double val,CDxSpinBox* sender)
	// 当数据改变时触发该函数，该函数可通过TL::MTLDataBind::Connect 进行和外部通信
	// void testFun(int index1, CDxSpinBox* sender);
	// DxUI::CDxSpinBox box;
	// TL::MTLDataBind::Connect(MSIGNAL(DxUI::CDxSpinBox, ValueChanged, &box),TL::MSLOT(testFun));
	//
	//+---------------------------------
	class DXUI_API CDxSpinBox :
		public CDxWidget
	{
		DECLARE_CLASS(CDxSpinBox)
	public:
		CDxSpinBox();
		~CDxSpinBox();
		void				CreateHwnd(){}
		void				CreateHwnd(HWND parent){}
		DXWindowType		GetWindowSelfDesc() const;
		SIZE				GetHintSize() const;
		void				SetStep(double step);
		void				SetRange(double minval, double maxval);
		void				SetValue(double val);

		void				SetReadOnly(bool isReadOnly);
		void				SetDecimals(int decimals);
		int					GetDecimals() const;
		double				GetValue() const;
		void				ReceiveEvent(CDxUIEvent* e);

	mevent:
		void				TextChanged(MString CurText, MString PreText, CDxWidget* sender);


	msignals:
		void				ValueChanged(double val, CDxWidget* sender);
		TL::MTLVoidEvent<double, CDxWidget*> Event_ValueChanged;

	public:
		void				OnRendWindow(IPainterInterface* painter);
		void				UpdateChildWindowPos();


	private:
		CDxDecorator*		pDecValueButton{ nullptr };
		CDxDecorator*		pIncValueButton{ nullptr };
		CDxEdit*			pValueEdit{ nullptr };
		double				mStep{ 1 };
		double				mMinValue;
		double				mMaxValue;
		int					mDecimals;
		bool			    bIsReadOnly{ false };
	};
}


