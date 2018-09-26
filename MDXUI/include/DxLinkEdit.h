

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
#include "DxEdit.h"
namespace DxUI{
	//+--------------------------
	// 
	// class CDxLinkEdit
	// 简单链接文本
	// 当点击之时会执行回调函数
	// 也可以使用connect进行回调连接
	// 
	//+---------------------------
	class DXUI_API CDxLinkEdit :
		public CDxEdit
	{
		DECLARE_CLASS(CDxLinkEdit)
		typedef std::function<void(const MString&)> RecallFunType;

	public:
		CDxLinkEdit();
		~CDxLinkEdit();

		bool			OnNotify(DXNotifyUI* NotifyEvent);
		void			OnRendWindow(IPainterInterface* painter);

		void			SetHoverColor(const DxColor& col);
		void			BindRecallFun(RecallFunType fun);

	msignals:
		void			Clicked(MString Text, CDxWidget* sender);
		TL::MTLVoidEvent<MString, CDxWidget*>  Event_Clicked;

	private:
		RecallFunType	mFun{ nullptr };
		DxColor			mHoverColor;
	};
}


