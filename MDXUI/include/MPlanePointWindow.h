

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
#include "MPlanePointWindowBase.h"
#include "DxPopWindow.h"
#include "DxColorPalette.h"
#include "DxEdit.h"
#include "DxComboBox.h"
#include "DxListWidget.h"
#include "DxStackedWidget.h"
#include "DxFlatButton.h"
#include "DxSpinBox.h"
#include "DxRadioButton.h"
#include "DxCheckBox.h"
#include "DxPushButton.h"
#include "DxTableWindow.h"


//
// F1 快捷键可以调用一些简单的设置操作
//
class DXUI_API MPlanePointWindow :
	public MPlanePointWindowBase
{
	DECLARE_CLASS(MPlanePointWindow)
public:
	MPlanePointWindow();
	~MPlanePointWindow();

protected:
	void	OnButton(bool isClicked, CDxWidget* sender);
	void	OnButtonTemplate(DxUI::CDxPushButton* button);
	void	OnCheckBox(bool isClicked, CDxWidget* sender);
	void	OnValueChanged(double val, CDxWidget* sender);
protected:
	bool	OnNotify(DxUI::DXNotifyUI* NotifyEvent);

private:
	DxUI::CDxPopWindow				mCanvaSetWindow;
	

	DxUI::CDxColorPalette			mColorPalette;
	DxUI::CDxRadioButton			mBackgroundColRad;
	DxUI::CDxRadioButton			mPlaneAreaColorRad;
	DxUI::CDxRadioButton			mTitileColorRad;
	DxUI::CDxRadioButton			mGridColorRad;
	DxUI::CDxCheckBox				mTraceMouseBox;
	DxUI::CDxCheckBox				mBorderBox;
	DxUI::CDxPushButton				mSetButton;
	DxUI::CDxPushButton				mExportButton;
	DxUI::CDxPushButton				mCopyButton;
	DxUI::CDxPushButton				mFileButton;
	DxUI::CDxPushButton				mPasterButton;

	//
	// 锁定颜色映射值范围
	//
	DxUI::CDxCheckBox				mLockMinValBox;
	DxUI::CDxCheckBox				mLockMaxValBox;
	DxUI::CDxSpinBox				mMinValBox;
	DxUI::CDxSpinBox				mMaxValBox;
};

