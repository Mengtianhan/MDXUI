

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
	// CDxPushButton  普通按钮 同时带标记
	// 长度会更具文本的长度进行调整
	// 发送点击事件:
	// Clicked(bool isClicked,CDxWidget* sender)
	// 可以使用如下方式进行事件回调
	// CDxPushButton button;
	// TL::MTLDataBind::Connect(MSIGNAL(DxUI::CDxPushButton, Clicked, &button),TL::MSLOT(testFun));
	// testFun:
	// void testFun(bool isClicked,CDxWidget* sender);
	// 鼠标单击时发送 EDxUIClicked 事件
	//
	// 重新实现 GetWidth  UpdateChildWindowPos 可以对外观进行修改
	// 默认实现的外观时 图标 + 文本 水平方向排列
	// 实现 OnDrawItem 可以重新绘制 返回 true 生效 如果返回 false 将会在当前绘制上面进行覆盖绘制
	//
	//+--------------------------------------
	class DXUI_API CDxPushButton :
		public CDxFlatButton
	{
		DECLARE_CLASS(CDxPushButton)
	public:
		CDxPushButton();
		~CDxPushButton();
		DXWindowType	GetWindowSelfDesc() const;
		void			SetChecked(bool isChecked);
		bool			OnNotify(DXNotifyUI* NotifyEvent);
		void			OnRendWindow(IPainterInterface* painter);
	};



	//+---------------------------------------
	//
	// CDxStatusButton 状态按钮
	// 状态随点击而变化
	// 第一次为选择 第二次就为非选择 ......
	//
	//+----------------------------------------
	class DXUI_API CDxStatusButton : public CDxPushButton{
		DECLARE_CLASS(CDxStatusButton)
	public:
		CDxStatusButton();
		~CDxStatusButton();
		DXWindowType	GetWindowSelfDesc() const;
		void			SetChecked(bool isChecked);
		bool			OnNotify(DXNotifyUI* NotifyEvent);
		void			OnRendWindow(IPainterInterface* painter);
	};



	//+------------------------------------
	//
	// class CDxUserButton
	// 可以对图标和文本自由定制
	// 需要对文本和图片区域进行设置
	// 否则结果可能不是自己想要的
	//
	//+------------------------------------
	class DXUI_API CDxUserButton : public CDxPushButton{
		DECLARE_CLASS(CDxUserButton)
	public:
		CDxUserButton();
		~CDxUserButton();

	protected:
		void	UpdateChildWindowPos();
	};

	//+--------------------------------------
	//
	// class CDxHexagonButton 
	// 六边形按钮
	// h = w / 2 * std::sqrt(3);
	// 高和宽满足上面关系式是为正六边形
	// h === 高
	// w === 宽
	// 
	//+--------------------------------------
	class DXUI_API CDxHexagonButton : public CDxPushButton{
		DECLARE_CLASS(CDxHexagonButton)
	public:
		CDxHexagonButton();
		~CDxHexagonButton();
	protected:
		bool OnDrawItem(const RECT& rc, IPainterInterface* painter);
	};
}

