

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DxWindow.h"
#include "Wke/Include/wke.h"

namespace DxUI{

	class CDxWebBroserWindow;
	class IWkeCallback
	{
	public:
		virtual void OnWkeTitleChanged(CDxWebBroserWindow* webView, LPCTSTR title){}
		virtual void OnWkeURLChanged(CDxWebBroserWindow* webView, LPCTSTR url){}
		virtual void OnWkeAlertBox(CDxWebBroserWindow* webView, LPCTSTR msg){}
		virtual bool OnWkeNavigation(CDxWebBroserWindow* webView, wkeNavigationType navigationType, LPCTSTR url){ return false; }
		virtual wkeWebView OnWkeCreateView(CDxWebBroserWindow* webView, const wkeNewViewInfo* info){ return NULL; }
		virtual void OnWkeDocumentReady(CDxWebBroserWindow* webView, const wkeDocumentReadyInfo* info){}
		virtual void OnWkeLoadingFinish(CDxWebBroserWindow* webView, const LPCTSTR url, wkeLoadingResult result, LPCTSTR failedReason){}
		virtual LPCTSTR OnJS2Native(CDxWebBroserWindow *pWeb, LPCTSTR lpMethod, LPCTSTR lpContent, void *pListenObj){ return NULL; }
	};

	//+----------------------------
	//
	// ‰Ø¿¿ΩÁ√Ê
	//
	//+-----------------------------
	class DXUI_API CDxWebBroserWindow :
		public CDxWindow
	{
	public:
		CDxWebBroserWindow();
		~CDxWebBroserWindow();

	protected:

	};
}


