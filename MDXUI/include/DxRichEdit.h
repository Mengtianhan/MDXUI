#pragma once
#include "DxWidget.h"

namespace DxUI{
	class RichEditImpl;
	//+----------------------------
	//
	// CDxRichWidget
	//
	//+-----------------------------
	class DXUI_API CDxRichEdit : public CDxWidget
	{
	public:
		CDxRichEdit();
		~CDxRichEdit();
		void	doInit(HWND hwnd);
		void	setText(const MString& content);
		void	appendText(const MString& content);
		HRESULT TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *plresult) const;
		bool	Notify(DXNotifyUI* NotifyEvent);
		void	rendWindow(const CDxPainter& painter);
		void	rendGDI();
		void	onTimer();
	private:
		RichEditImpl*			pTwh{ nullptr };
	};
}


