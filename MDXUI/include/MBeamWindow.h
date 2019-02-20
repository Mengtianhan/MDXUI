

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

