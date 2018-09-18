

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DxEdit.h"
namespace DxUI{

	//+--------------------------------------
	//
	// CDxFlatButton  扁平按钮 同时带标记
	// 长度会更具文本的长度进行调整
	// 发送点击事件:
	// Clicked(bool isClicked,CDxWidget* sender)
	// 可以使用如下方式进行事件回调
	// CDxFlatButton button;
	// TL::MTLDataBind::Connect(MSIGNAL(DxUI::CDxFlatButton, Clicked, &button),TL::MSLOT(testFun));
	// testFun:
	// void testFun(bool isClicked,CDxWidget* sender);
	// 鼠标单击 或者状态为true 时发送 EDxUIClicked 事件
	//
	// 重新实现 GetWidth  UpdateChildWindowPos 可以对外观进行修改
	// 默认实现的外观时 图标 + 文本 水平方向排列
	// 实现 OnDrawItem 可以重新绘制 返回 true 生效 如果返回 false 将会在当前绘制上面进行覆盖绘制
	//
	//+--------------------------------------
	class DXUI_API CDxFlatButton :
		public CDxEdit
	{
		DECLARE_CLASS(CDxFlatButton)
	public:
		CDxFlatButton();
		~CDxFlatButton();

		DXWindowType	GetWindowSelfDesc() const;
		SIZE			GetHintSize() const;
		int				GetWidth() const;
		int				GetHeight() const;
		int				GetWidthQuickly() const; // 快速获取宽度 未必准
		int				GetHeightQuickly() const; // 快速获取宽度 未必准
		DxRectI			GetInvalidateRect() const;
		bool			SetWindowStatus(DXWindowStatus status);
		void			SetIcon(const MString& icon);

		void			SetGeomety(const DxRectI& rc);

		virtual void	SetChecked(bool isChecked);
		bool			IsChecked() const;

		void			SetClipRect(const DxRectI& rc);
		DxRectI&		GetClipRect();
		const DxRectI&	GetClipRect() const;

		void			OnTextFormat();

		virtual void	Trigger();  // 触发 一个点击信号

	msignals:
		virtual void	Clicked(bool isClicked,CDxWidget* sender);  // 外部可对该信号进行connect
		

		//+---------------
		//
		// 使用 Event_Clicked 可以直接使用 += 操作符进行事件绑定
		// Event_Clicked += fun
		// Event_Clicked += fun
		// += 返回一个事件id
		// 如果想要移除事件响应函数 -= 即可
		// Button 添加该事件仅作尝试
		//
		//+---------------
		TL::MTLVoidEvent<bool, CDxWidget*>   Event_Clicked;

	public:
		//
		// 当需要对外观进行重新绘制可以重写该函数
		//
		virtual bool	OnDrawItem(const RECT& rc, IPainterInterface* painter);

		bool			OnNotify(DXNotifyUI* NotifyEvent);
		void			OnRendWindow(IPainterInterface* painter);

	protected:
		void			OnResizeEvent();
		void			UpdateChildWindowPos();

	protected:
		DxRectI			mClipRect;
		bool			bIsClicked{ false };
	};


	//+-------------------------------------------------------
	//
	// 高度不受限制的平面按钮
	// 接受点击，但不受点击状态影响下一次点击
	//
	//+-------------------------------------------------------
	class DXUI_API CDxFlatButtonEx : public CDxFlatButton{
		DECLARE_CLASS(CDxFlatButtonEx)
	public:
		CDxFlatButtonEx();
		~CDxFlatButtonEx();
		SIZE			GetHintSize() const;
		bool			SetWindowStatus(DXWindowStatus status);
		void			SetChecked(bool isChecked);
		bool			OnNotify(DXNotifyUI* NotifyEvent);
	};
}


