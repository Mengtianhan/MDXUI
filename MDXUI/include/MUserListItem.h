

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