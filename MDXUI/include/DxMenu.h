

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DxWindow.h"

namespace DxUI{
	//+--------------------------------------
	//
	// CDxMenu  菜单窗口
	// 一个弹出窗口
	//
	//+--------------------------------------
	class CDxAction;
	class DXUI_API CDxMenu :
		public CDxWindow
	{
		DECLARE_CLASS(CDxMenu)
	public:
		CDxMenu();
		~CDxMenu();
		DXWindowType	GetWindowSelfDesc() const;
		HWND			GetRootHwnd() const;
		CDxAction*		AddAction(const MString& Icon, const MString& Text);
		CDxAction*		AddAction(const MString& Text);
		CDxAction*		AddCheckableAction(const MString& Text);

		void			AddSpliter();

		CDxMenu*		AddMenu(const MString& Icon, const MString& Text);
		CDxMenu*		AddMenu(const MString& Text);


		void			SetGeomety(const DxRectI& rc);
		void			SetSize(int w, int h);
		void			ReSize(unsigned w, unsigned h);
		void			CreateHwnd(HWND parent);
		void			SetFocus();
		void			KillFocus();
		CDxWidget*		HitTest(int x, int y);
		bool			PointInArea(int x, int y);
		void			UpdateArea();
		bool			OnNotify(DxUI::DXNotifyUI* NotifyEvent);
		void			OnRendWindow(IPainterInterface* painter);
		ID2D1HwndRenderTarget* GetRenderTarget();
		CDxWidget*		GetParent();
		void			Show();
		int				Exec();
		void			SetVisible(bool isVisible);
		void			Exec(int x, int y);
	protected:
		void			UpdateChildWindowPos();

	private:
		TL::Vector<CDxAction*>	mActions;
		TL::Vector<CDxMenu*>	mMenus;
		
	};
}


