

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////


#pragma once
#include "DxWindow.h"
#include "DxListWindow.h"
#include "DxPushButton.h"
#include "DxLabel.h"
#include "DxEdit.h"

using DxUI::CDxListWindowEx;
using DxUI::CDxWindow;
using DxUI::CDxPushButton;
using DxUI::CDxLabel;
using DxUI::CDxEdit;
using DxUI::CDxWidget;


class DXUI_API MBeamWindow :
	public CDxWindow
{
public:
	MBeamWindow();
	~MBeamWindow();

	TL::Vector<MString> GetAvailable();
	TL::Vector<MString> GetSelected();

	void SetBeamPath(const MString& beampath);
	void SetAvailabelItem(const TL::Vector<MString>& items);
	void SetSelectedItem(const TL::Vector<MString>& items);

	void GetSelectedInfo(MString& from, MString& to, MString& step);
	void SetSelectedInfo(const MString& from, const MString& to, const MString& step);

private:
	void doButton(bool isClicked,CDxWidget* sender);

private:
	CDxListWindowEx*    pCanSelectedList{ nullptr };
	CDxListWindowEx*    pSelectedList{ nullptr };

	CDxLabel*			pFromLabel{ nullptr };
	CDxLabel*			pToLabel{ nullptr };
	CDxLabel*			pStepLabel{ nullptr };

	CDxEdit*			pFromEdit{ nullptr };
	CDxEdit*			pToEdit{ nullptr };
	CDxEdit*			pStepEdit{ nullptr };

	CDxPushButton*		pAddSelectedButton{ nullptr };
	CDxPushButton*		pAddByFilterButton{ nullptr };
	CDxPushButton*		pRemoveButton{ nullptr };
};

