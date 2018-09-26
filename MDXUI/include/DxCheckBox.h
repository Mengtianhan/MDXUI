

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
#include "DxFlatButton.h"

namespace DxUI{
	//+--------------------------------------
	//
	// CDxCheckBox  选择按钮 
	// 长度会更具文本的长度进行调整
	// 发送点击事件:
	// Clicked(bool isClicked，CDxWidget* sender)
	// 可以使用如下方式进行事件回调
	// CDxCheckBox button;
	// TL::MTLDataBind::Connect(MSIGNAL(DxUI::CDxCheckBox, Clicked, &button),TL::MSLOT(testFun));
	// testFun:
	// void testFun(bool isClicked,CDxWidget* sender);
	// 鼠标单击时发送 EDxUIClicked 事件
	//
	// 重新实现 GetWidth  UpdateChildWindowPos 可以对外观进行修改
	// 默认实现的外观时 图标 + 文本 水平方向排列
	// 实现 OnDrawItem 可以重新绘制 返回 true 生效 如果返回 false 将会在当前绘制上面进行覆盖绘制
	//
	//+--------------------------------------
	class DXUI_API CDxCheckBox :
		public CDxFlatButton
	{
		friend class CDxGroupBox;
		DECLARE_CLASS(CDxCheckBox)
	public:
		CDxCheckBox();
		~CDxCheckBox();
		DXWindowType	GetWindowSelfDesc() const;
		DxColor			GetEraseColor() const;
		virtual void	SetChecked(bool isChecked);
		virtual void	SetCheckable(bool isable);
		virtual bool	IsCheckable() const;
		virtual bool	IsChecked() const;
		bool			OnNotify(DXNotifyUI* NotifyEvent);
		void			OnRendWindow(IPainterInterface* painter);

	protected:
		void			UpdateChildWindowPos();
		bool			bIsCheckabel{ true };
	};
}


