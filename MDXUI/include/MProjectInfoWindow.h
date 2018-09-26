

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
#include "DxWindow.h"
#include "DxTreeWindow.h"
#include "DxPicture.h"
#include "DxMultTreeWindow.h"

using DxUI::CDxWindow;
using DxUI::CDxPicture;
using DxUI::CDxTreeItem;
using DxUI::CDxTreeWindow;
using DxUI::CDxMultTreeWindow;

class DXUI_API MProjectInfoWindow :
	public CDxWindow
{
	DECLARE_CLASS(MProjectInfoWindow)
public:
	MProjectInfoWindow();
	~MProjectInfoWindow();

	void	OnUpdateWindow();
protected:
	void	UpdateChildWindowPos();
	void	OnRendWindow(DxUI::IPainterInterface* painter);
	
private:
	CDxPicture*			p_PictrueWindow{ nullptr };
	CDxMultTreeWindow*	p_MultTreeWindow{ nullptr };

	MString				m_ProjectName;
	MString				m_ProjectType;

	TL::Map<MString, TL::Vector<std::pair<MString, MString>>>  mProperty;
};

