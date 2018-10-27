

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
#include "DxDockWindow.h"
#include "DxFlatButton.h"

namespace DxUI{
	class CDxToolItem;
	class CDxAction;
	//+-------------------------------------------------
	//
	// 放在CDxDockContainer中可以任意调整位置的窗口
	// 放置一系列子窗口
	//
	//+-------------------------------------------------
	class DXUI_API CDxToolBar :
		public CDxDockWindow
	{
		DECLARE_CLASS(CDxToolBar)
		friend class CDxDockContainer;
		friend class CDxMiniDockContainer;
	public:
		CDxToolBar();
		~CDxToolBar();

		void AddChild(CDxWidget* childWindow);
		void RemoveChild(CDxWidget* childWindow);


		CDxAction* AddAction(const MString& Icon, const MString& hoverIcon = L"", const MString& clickedIcon = L"");

		//
		// 通过AddItem 和 RemoveItem 添加子窗口
		// 从CDxDockWindow继承而来的 Attach 和 Dettach 可以不用
		// 不支持Layout
		// 显示顺序为添加顺序
		//
		void	AddItem(CDxWidget* Item);
		void	RemoveItem(CDxWidget* Item);
		int		GetIndexByItem(CDxWidget* Item) const;
		int		GetCurrentIndex() const;
		CDxWidget* GetCurrentItem() const;
		CDxWidget* GetItemByIndex(int index) const;
		CDxWidget* GetFirstItem() const;
		CDxWidget* GetLastItem() const;
		int			 Size() const;
		TL::Vector<CDxWidget*> GetAllItems() const;
		void	UpdateChildWindowPos();

	msignals:
		void	ClickedChanged(CDxWidget* curItem, CDxWidget* preItem, CDxWidget* sender);
		TL::MTLVoidEvent<CDxWidget*, CDxWidget*, CDxWidget*> Event_ClickedChanged;
	protected:
		bool	OnNotify(DXNotifyUI* NotifyEvent);
		void	OnRendWindow(IPainterInterface* painter);

	private:
		RECT						m_ParentRc;
		int							m_NeedLength{ -1 };
		CDxWidget*					p_CurrentItem{ nullptr };
		TL::Vector<CDxWidget*>		mItems;
	};




	//+------------------------------------------------
	//
	// 工具栏选项
	//
	//+------------------------------------------------
	class DXUI_API CDxToolItem : public CDxFlatButton{
		DECLARE_CLASS(CDxToolItem)
	public:
		CDxToolItem();
		SIZE	GetHintSize() const;
		virtual CDxToolItem* CreateThis();
		virtual bool DestroyThis();
		void	SetSelected(bool isSelected);
	};
}


