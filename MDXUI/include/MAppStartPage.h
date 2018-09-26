


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
#include "DxPicture.h"
#include "DxPushButton.h"

class DXUI_API MAppStartPage :
	public DxUI::CDxWindow
{
public:
	MAppStartPage();
	~MAppStartPage();

protected:


	void		UpdateChildWindowPos();
	void		OnPushButtonTemplate(DxUI::CDxPushButton* button);
	void		OnPushButtonTemplate2(DxUI::CDxPushButton* button);
	void		OnButtonStatuChanged(DxUI::DXWindowStatus status, DxUI::CDxWidget* sender);
	void		OnButtonClicked(bool isClicked , DxUI::CDxWidget* sender);
	void		OnRendWindow(DxUI::IPainterInterface* painter);
private:
	DxUI::CDxPicture*			p_PictrueWindow{ nullptr };

	//
	// 工程类型按钮
	//
	DxUI::CDxPushButton*		p_NFTestButton{ nullptr };
	DxUI::CDxPushButton*		p_FFTestButton{ nullptr };
	DxUI::CDxPushButton*		p_CalibrationButton{ nullptr };
	DxUI::CDxPushButton*		p_AnalysisButton{ nullptr };


	//
	// 功能性按钮
	//
	DxUI::CDxPushButton*		p_VNAButton{ nullptr };
	DxUI::CDxPushButton*		p_ScannerButton{ nullptr };
	DxUI::CDxPushButton*		p_PositionerButton{ nullptr };
	DxUI::CDxPushButton*		p_RTCButton{ nullptr };
	DxUI::CDxPushButton*		p_ToolButton{ nullptr };

	MString						m_Describe;
	MString						m_ProductorStr;
};

