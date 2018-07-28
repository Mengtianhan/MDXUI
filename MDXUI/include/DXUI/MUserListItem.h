#pragma once
#include "DxListItem.h"

using DxUI::IPainterInterface;

//+-----------------------------------
//
// 增加一些自定义的列表项
//
//+-----------------------------------

//
// 按钮式Item
//
class DXUI_API MAppInfoMenuItem : public DxUI::CDxListItem{
	DECLARE_CLASS(MAppInfoMenuItem)
public:
	MAppInfoMenuItem();
	~MAppInfoMenuItem();
	bool OnDrawItem(const RECT& rc, IPainterInterface* painter);
	bool SetWindowStatus(DxUI::DXWindowStatus status);
	void SetSelected(bool selected);
};


//
// 选中式Item
//
class DXUI_API MAppInfoLabelItem : public DxUI::CDxListItem{
	DECLARE_CLASS(MAppInfoLabelItem)
public:
	MAppInfoLabelItem();
	~MAppInfoLabelItem();
	int GetHeight() const;
	DxUI::CDxEffects*	GetSpecializedEffects();
	void SetOtherColor(const DxUI::DxColor& col);
	bool OnDrawItem(const RECT& rc, IPainterInterface* painter);
private:
	DxUI::CDxEffects		m_Effective;
	DxUI::DxColor			m_OherColor;
};


//
// 仅仅作为分割线使用
//
class DXUI_API MSpliterListItem : public DxUI::CDxListItem{
	DECLARE_CLASS(MAppInfoLabelItem)
public:
	MSpliterListItem();
	~MSpliterListItem();
	int GetHeight() const;
	void SetSelected(bool selected);
	void SetSpliterHeight(int height);
	bool OnDrawItem(const RECT& rc, IPainterInterface* painter);
private:
	int			mSpliterHeight{ 2 };
};


//
// 空ListItem 什么也不做 就是占位
//
class DXUI_API MDumpListItem : public DxUI::CDxListItem{
	DECLARE_CLASS(MDumpListItem)
public:
	MDumpListItem();
	~MDumpListItem();
	int GetHeight() const;
	void SetSelected(bool selected);
	bool OnDrawItem(const RECT& rc, IPainterInterface* painter);
};