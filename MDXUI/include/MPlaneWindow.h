

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
#include "MPlaneWindowBase.h"
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


//+------------------------
//
// class MPlaneWindow
// F1 弹出设置窗口
// F2 弹出数据窗口
//
//+-------------------------
class DXUI_API MPlaneWindow :
	public MPlaneWindowBase
{
	DECLARE_CLASS(MPlaneWindow)
public:
	MPlaneWindow();
	~MPlaneWindow();

protected:
	void	OnButton(bool isClicked, CDxWidget* sender);
	void	OnButtonTemplate(DxUI::CDxPushButton* button);
	void	OnCheckBox(bool isClicked, CDxWidget* sender);
	void	OnValueChanged(double val, CDxWidget* sender);
protected:
	bool	OnNotify(DxUI::DXNotifyUI* NotifyEvent);

private:
	DxUI::CDxPopWindow				mCanvaSetWindow;
	DxUI::CDxPopWindow				mDataShowWindow;


	DxUI::CDxColorPalette			mColorPalette;
	DxUI::CDxRadioButton			mBackgroundColRad;
	DxUI::CDxRadioButton			mTitileColorRad;
	DxUI::CDxRadioButton			mGridColorRad;
	DxUI::CDxRadioButton			mPlaneAreaColorRad;
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

	//
	// 修改标题
	//
	DxUI::CDxEdit					mTitleEdit;
	DxUI::CDxEdit					mHTitleEdit;
	DxUI::CDxEdit					mVTitleEdit;

	DxUI::CDxPushButton				mModifyTitleButton;
	DxUI::CDxPushButton				mModifyHTitleButton;
	DxUI::CDxPushButton				mModifyVTitleButton;

	DxUI::CDxTableWindow			mDataTable;
};

