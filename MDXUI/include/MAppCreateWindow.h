

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

