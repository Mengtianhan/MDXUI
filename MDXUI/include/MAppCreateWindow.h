

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DxDialog.h"
#include "DxPushButton.h"
#include "DxEdit.h"
#include "DxLabel.h"
#include "DxPicture.h"
#include "DxComboBox.h"

using DxUI::CDxDialog;
using DxUI::CDxEdit;
using DxUI::CDxPushButton;
using DxUI::CDxLabel;
using DxUI::CDxPicture;
using DxUI::CDxComboBox;

class DXUI_API MAppCreateWindow :
	public CDxDialog
{
	DECLARE_CLASS(MAppCreateWindow)
public:
	MAppCreateWindow();
	~MAppCreateWindow();

	void   SetProjectType(int type);

	MString GetName() const;
	MString GetLocal() const;
	MString GetRelyMode() const;

protected:
	void    UpdateProjectList();
	void    OnButtonTemplate(DxUI::CDxPushButton* button);
	void	OnButtonEvent(bool isClicked, DxUI::CDxWidget* sender);
	void	UpdateChildWindowPos();
	void	OnRendWindow(DxUI::IPainterInterface* painter);
private:

	CDxPicture*	    p_PictrueWindow{ nullptr };

	CDxLabel*		p_ProjectNameLabel{ nullptr };
	CDxLabel*		p_FolderNameLabel{ nullptr };
	CDxLabel*		p_RelyOnLabel{ nullptr };

	CDxEdit*		p_ProjectNameEdit{ nullptr };
	CDxEdit*		p_FolderNameEdit{ nullptr };
	CDxComboBox*	p_RelyOnBox{ nullptr };

	CDxPushButton*	p_FolderButton{ nullptr };
	
	CDxPushButton*	p_ConfirmButton{ nullptr };
	CDxPushButton*  p_CancelButton{ nullptr };
	int				m_Type{ 0 };

};

