

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DxTextBlock.h"

namespace DxUI{
	class CDxVerticalScrollBar;
	//+-----------------------------
	//
	// ÊÖ»æÎÄ±¾¿ò
	//
	//+------------------------------

	class DXUI_API CDxEditWidget :public CDxTextBlock
	{
		DECLARE_CLASS(CDxEditWidget)
	public:
		CDxEditWidget();
		~CDxEditWidget();		
		void			SetReadOnly(bool isReadOnly);
		bool			IsReadOnly() const;
		void			SetPlaceholderText(const MString& Text);
		bool			OnNotify(DXNotifyUI* NotifyEvent);
	};

}

