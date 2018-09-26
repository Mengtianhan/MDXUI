

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


