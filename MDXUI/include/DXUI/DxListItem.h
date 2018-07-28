#pragma once
#include "DxLabel.h"

namespace DxUI{
	//+------------------------------------------------------------
	//
	// 列表框项
	// 默认的列表框item限制了最大高度为26
	// 所以如果需要修改这个高度可以选择继承之后修改mFixHeight
	// 如果需要重新绘制item的外观，建议继承后重写OnDrawItem即可
	// 因为如果重写OnRendWindow其中会涉及到一些裁剪细节
	// OnDrawItem 重写该函数不用考虑裁剪细节
	//
	//+-------------------------------------------------------------
	class DXUI_API CDxListItem :
		public CDxLabel
	{
		friend class CDxListWindow;
		friend class CDxComboBox;
		friend class CDxListWidget;

		DECLARE_CLASS(CDxListItem)
	public:
		CDxListItem();

	
		virtual CDxListItem* CreateThis();
		virtual bool		 DestroyThis();
		DXWindowType		 GetWindowSelfDesc() const;  
		SIZE				 GetHintSize() const;


		int				GetWidth() const;
		int				GetWidthQuickly() const; // 快速获取宽度 未必准
		void			SetIcon(const MString& icon);
		bool			SetWindowStatus(DXWindowStatus status);

		void			SetGeomety(const DxRectI& rc);
		void			OnResizeEvent();
		void			OnTextFormat();
		void			SetText(const MString& Text);
		void			SetAny(const mj::MAny& any);

		bool			OnNotify(DXNotifyUI* NotifyEvent);


		bool			IsSelected() const;
		DxRectI			GetInvalidateRect() const;


		void			OnRendWindow(IPainterInterface* painter);


		//
		// 点击Item的时候发射Clicked时间
		//
		void			Clicked(bool isClicked, CDxWidget* sender);
		void			DoubleClicked(bool isClicked, CDxWidget* sender);
		void			Checked(bool isChecked, CDxWidget* sender);  //对于可选的Item来说选中时发射的信号
		void			ClickedPoint(bool isClicked, POINT pt, CDxWidget* sender);


		void			Trigger(); // 触发上面事件 Clicked

		//
		// 如果有打算重写item的想法的话，建议重写该函数即可，不建议重写上面的 OnRendWindow
		// rc 是可绘制区域
		// painter 绘制对象
		//
		bool			IsVisible() const;
		virtual bool	OnDrawItem(const RECT& rc, IPainterInterface* painter);

	protected:
		void			UpdateChildWindowPos();
		void			SetSelected(bool selected);


		bool			bIsSelected{ false };
		int				mTextLayoutLen{ 0 };
		POINT			mOldPoint;
		mj::MAny		mAny;
	};

}

