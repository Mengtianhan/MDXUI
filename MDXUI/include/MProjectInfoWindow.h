

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
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

