#pragma once
#include "DxDialog.h"
#include "DxPushButton.h"
#include "DxEdit.h"
#include "DxSpinBox.h"
#include "DxCheckBox.h"
#include "MAppWindow.h"

using namespace DxUI;

class DXUI_API MNFSysOptionSet :
	public CDxDialog, public MAppWindow
{
public:
	MNFSysOptionSet();
	~MNFSysOptionSet();

	virtual int  OpenWindow();
	virtual bool SaveContext();
	virtual bool SetContext();
	virtual mj::MString ToString(){ return "MNFSysOptionSet"; }

private:
	void OnButton(bool isClicked, CDxWidget* sender);

private:
	CDxSpinBox		m_HorizonAngle;
	CDxSpinBox		m_VerticalAngle;
	CDxSpinBox		m_SampleStep;
	CDxSpinBox		m_SNRadio;
	CDxEdit			m_BeamLibEdit;

	CDxCheckBox		m_HideSensitiveBox;
	CDxCheckBox		m_XAxisBox;
	CDxCheckBox		m_YAxisBox;
	CDxCheckBox		m_WAxisBox;
	CDxCheckBox		m_AZAxisBox;


	CDxPushButton	m_BrowseButton;
	CDxPushButton	m_ConfirmButton;
	CDxPushButton	m_CancelButton;
};

