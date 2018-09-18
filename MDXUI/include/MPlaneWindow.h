

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
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

	DxUI::CDxTableWindow			mDataTable;
};

