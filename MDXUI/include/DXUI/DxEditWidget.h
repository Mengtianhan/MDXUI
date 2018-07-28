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

