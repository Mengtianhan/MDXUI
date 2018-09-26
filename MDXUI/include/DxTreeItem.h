

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
#include "DxListItem.h"
#include "DxScrollBar.h"

namespace DxUI{
	class CDxTreeWindow;
	class CDxTreeItem;
	class CDxMultTreeWindow;
	class CDxMultTreeItem;

	//+-----------------------------------
	//
	// class CDxTreeItemDecorator
	// 展开收缩的修饰符
	//
	//+-----------------------------------
	class DXUI_API CDxTreeItemDecorator : public CDxDecorator{
		DECLARE_CLASS(CDxTreeItemDecorator)
		friend class CDxTreeItem;
		friend class CDxMultTreeItem;

	public:
		CDxTreeItemDecorator();
		~CDxTreeItemDecorator();	
		bool				OnNotify(DXNotifyUI* NotifyEvent);
		DxColor				GetEraseColor() const;
		void				OnRendWindow(IPainterInterface* painter);
	protected:
		virtual void		SetParentWindowStatus(bool isSelected);

	private:
		void				SetSelected(bool selected);
		bool				IsSelected() const;
		bool				bIsSelected{ false };
		CDxTreeItem*		pOwnWindow{ nullptr };
	};


	//+-------------------------------------------------
	//
	// class CDxTreeItem
	// CDxTreeItem 不对 子 CDxTreeItem 进行区域设置
	// 所有的设置都将由 CDxTreeWindow 进行区域设置
	// UpdateChildWindowPos 将会被针对性重写
	// UpdateArea 不再使用
	//
	//+-------------------------------------------------
	class DXUI_API CDxTreeItem :
		public CDxListItem
	{
		DECLARE_CLASS(CDxTreeItem)
		friend class CDxTreeWindow;
		friend class CDxMultTreeWindow;
		friend class CDxMultTreeItem;

		enum CheckState{Unchecked = 0,PartiallyChecked,Checked};
	public:
		CDxTreeItem();
		~CDxTreeItem();

		CDxTreeItem*				CreateThis();
		bool						DestroyThis();
		
		DXWindowType				GetWindowSelfDesc() const;

		void						AddItem(CDxTreeItem* item);
		virtual CDxTreeItem* 		AddItem(const MString& itemText);
		virtual CDxTreeItem* 		AddItem(const MString& icon, const MString& itemText);

		void						InsertItem(CDxTreeItem* item, CDxTreeItem* positem);
		bool						RemoveItem(CDxTreeItem* item);

		void						Clear();

		CDxTreeItem*				GetItemByText(const MString& Text);

		void						SetGeomety(const DxRectI& rc);
		CDxWidget*					HitTest(int x, int y);
		bool						PointInArea(int x, int y);

		

		DxRectI						GetInvalidateRect() const;

		TL::Vector<CDxTreeItem*>&   GetChildItems();
		const TL::Vector<CDxTreeItem*>&   GetChildItems() const;
		bool						IsExpansion() const;
		void						SetExpansion(bool isExPansion);
		void						NotifyTreeWindowUpdate();

		bool						OnNotify(DXNotifyUI* NotifyEvent);
		void						ReceiveEvent(CDxUIEvent* e);

		CDxTreeItem*				GetParentNode() const;

		void						SetSelected(bool selected);


		void						SetCheckable(bool isCheck);
		bool						IsCheckable() const;
		CheckState					GetCheckState() const;
		void						SetCheckState(CheckState state);


		//
		// 针对多列树结构实现
		//
		virtual void				SetColumsPos(const TL::Vector<int>& pos){}
		virtual void				AddItem(const TL::Vector<MString>& Text){};
		virtual void				AddItem(const MString& icon, const TL::Vector<MString>& Text){};
		virtual void				SetShowText(const TL::Vector<MString>& Texts){};
	protected:
		void						OnViewWindowChanged();
		void					    SetExpansionImpl(bool isExPansion);
		CDxTreeItemDecorator*		GetDecorator();
		void						UpdateChildWindowPos();
		void						UpdateArea(){}
		void						GetItems(std::vector<CDxTreeItem*>& outItems);
		void						OnRendWindow(IPainterInterface* painter);

	protected:
		CDxTreeItemDecorator		mDecorator;
		TL::Vector<CDxTreeItem*>    mItems;
		CDxTreeItem*				pParentNode{ nullptr };
		bool						bIsExpansion{ true };
		bool						bIsCanCheckable{ false };
	};



	//+-------------------------------------------------
	//
	// class CDxMultTreeItem
	// CDxMultTreeItem 不对 子 CDxMultTreeItem 进行区域设置
	// 所有的设置都将由 CDxMultTreeWindow 进行区域设置
	// UpdateChildWindowPos 将会被针对性重写
	// UpdateArea 不再使用
	//
	//+-------------------------------------------------
	class DXUI_API CDxMultTreeItem : public CDxTreeItem{
		DECLARE_CLASS(CDxMultTreeItem)
		using CDxTreeItem::AddItem;
	public:
		CDxMultTreeItem();
		~CDxMultTreeItem();
		CDxTreeItem*				CreateThis();
		bool						DestroyThis();
		void						SetColumsPos(const TL::Vector<int>& pos);

		CDxTreeItem* 				AddItem(const MString& itemText);
		CDxTreeItem* 				AddItem(const MString& icon, const MString& itemText);
		void						AddItem(const TL::Vector<MString>& Text);
		void						AddItem(const MString& icon, const TL::Vector<MString>& Text);
		void						SetShowText(const TL::Vector<MString>& Texts);

		int							GetWidth() const;
	protected:

		void						OnRendWindow(IPainterInterface* painter);

	protected:
		TL::Vector<int>				mColumPos;
		TL::Vector<MString>			mTextVec;
	};
}


