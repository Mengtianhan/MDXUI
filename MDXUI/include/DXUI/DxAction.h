#pragma once
#include "DxCheckBox.h"
namespace DxUI{
	//+------------------------------------------------------------
	//
	// 菜单项
	// 如果需要重新绘制item的外观，建议继承后重写OnDrawItem即可
	// 因为如果重写OnRendWindow其中会涉及到一些裁剪细节
	// OnDrawItem 重写该函数不用考虑裁剪细节
	//
	//+-------------------------------------------------------------
	class DXUI_API CDxAction :
		public CDxCheckBox
	{
		friend class CDxMenu;
		DECLARE_CLASS(CDxAction)
	public:
		CDxAction();
		~CDxAction();
		DXWindowType	GetWindowSelfDesc() const;
		DxColor			GetEraseColor() const;
		DxRectI			GetInvalidateRect() const;
		void			SetCheckable(bool isable);
		void			SetIcon(const MString& Text);
		bool			OnNotify(DXNotifyUI* NotifyEvent);
		void			OnRendWindow(IPainterInterface* painter);
		int				GetWidth() const;
		int				GetHeight() const;
	protected:
		void			SetMenu(CDxMenu* menu);
		int				GetIconAreaWidth() const;
		void			SetIconAreaWidth(int w);
		
	protected:
		void			UpdateChildWindowPos();
		CDxMenu*		pRelativeMenu{ nullptr }; // 关联菜单
		bool			bIsNeedIconAreaW{ false };
	};


	//+--------------------------
	//
	// 只绘制一条横线
	//
	//+---------------------------
	class DXUI_API CDxSpliter : public CDxAction{
		friend class CDxMenu;
		DECLARE_CLASS(CDxSpliter)
	public:
		CDxSpliter();
		~CDxSpliter();
		int				GetHeight() const;
		void			OnRendWindow(IPainterInterface* painter);
	protected:
		void			UpdateChildWindowPos();
	};
}


