#pragma once
#include "DxCheckBox.h"
namespace DxUI{
	//+--------------------------------------
	//
	// CDxRadioButton  单选按钮 
	// 长度会更具文本的长度进行调整
	// 发送点击事件:
	// Clicked(bool isClicked，CDxWidget* sender)
	// 可以使用如下方式进行事件回调
	// CDxRadioButton button;
	// TL::MTLDataBind::Connect(MSIGNAL(DxUI::CDxRadioButton, Clicked, &button),TL::MSLOT(testFun));
	// testFun:
	// void testFun(bool isClicked,CDxWidget* sender);
	// 鼠标单击时发送 EDxUIClicked 事件
	//
	// 重新实现 GetWidth  UpdateChildWindowPos 可以对外观进行修改
	// 默认实现的外观时 图标 + 文本 水平方向排列
	// 实现 OnDrawItem 可以重新绘制 返回 true 生效 如果返回 false 将会在当前绘制上面进行覆盖绘制
	//
	//+--------------------------------------
	class DXUI_API CDxRadioButton :
		public CDxCheckBox
	{
		DECLARE_CLASS(CDxRadioButton)
	public:
		CDxRadioButton();
		~CDxRadioButton();
		DXWindowType	GetWindowSelfDesc() const;
		bool			OnNotify(DXNotifyUI* NotifyEvent);
		void			OnRendWindow(IPainterInterface* painter);
		void			UpdateChildWindowPos();
	};
}


