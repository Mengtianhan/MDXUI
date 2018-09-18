


///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
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

