#pragma once
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stddef.h>
#include <richedit.h>
#include <tchar.h>
#include <assert.h>
#include <crtdbg.h>
#include <malloc.h>
#include <stdio.h>
#include <OleCtl.h>
#include <textserv.h>
#include <imm.h>
#pragma comment(lib, "imm32.lib")
#include <Windows.h>
#include "DxRichEdit.h"
#include "DxResource.h"

namespace DxUI{
	const LONG cInitTextMax = (32 * 1024) - 1;

	EXTERN_C const IID IID_ITextServices = { // 8d33f740-cf58-11ce-a89d-00aa006cadc5
		0x8d33f740,
		0xcf58,
		0x11ce,
		{ 0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5 }
	};

	EXTERN_C const IID IID_ITextHost = { // c5bdd8d0-d26e-11ce-a89e-00aa006cadc5 
		0xc5bdd8d0,
		0xd26e,
		0x11ce,
		{ 0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5 }
	};

	EXTERN_C const IID IID_ITextServices2 = { // {5169851F-BC70-4820-88A1-5443459C543B}
		0x8d33f740,
		0xcf58,
		0x11ce,
		{ 0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5 }
		};


	EXTERN_C const IID IID_ITextHost2 = { 
		0xc5bdd8d0,
		0xd26e,
		0x11ce,
		{ 0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5 }
	};

#ifndef LY_PER_INCH
#define LY_PER_INCH 1440
#endif

#ifndef HIMETRIC_PER_INCH
#define HIMETRIC_PER_INCH 2540
#endif

	//
	// 其实本该是有定义
	//
#ifndef PCreateTextServices
	typedef HRESULT(STDAPICALLTYPE * PCreateTextServices)(
		IUnknown *punkOuter,
		ITextHost *pITextHost,
		IUnknown **ppUnk);
#endif

	class RichEditImpl : public ITextHost
	{
		friend class MRichEdit;
	public:
		RichEditImpl();
		BOOL Init(CDxRichEdit *re, const CREATESTRUCT *pcs);
		virtual ~RichEditImpl();

		ITextServices2* GetTextServices(void) { return pserv; }

		void SetClientRect(RECT *prc);
		RECT* GetClientRect() { return &rcClient; }
		BOOL IsWordWrap(void) { return fWordWrap; }
		void SetWordWrap(BOOL fWordWrap);
		BOOL IsReadOnly();
		void SetReadOnly(BOOL fReadOnly);

		void SetFont(HFONT hFont);
		void SetColor(DWORD dwColor);
		SIZEL* GetExtent();
		void SetExtent(SIZEL *psizelExtent);
		void LimitText(LONG nChars);
		BOOL IsCaptured();
		BOOL IsShowCaret();
		void NeedFreshCaret();
		INT GetCaretWidth();
		INT GetCaretHeight();

		BOOL GetAllowBeep();
		void SetAllowBeep(BOOL fAllowBeep);
		WORD GetDefaultAlign();
		void SetDefaultAlign(WORD wNewAlign);
		BOOL GetRichTextFlag();
		void SetRichTextFlag(BOOL fNew);
		LONG GetDefaultLeftIndent();
		void SetDefaultLeftIndent(LONG lNewIndent);
		BOOL SetSaveSelection(BOOL fSaveSelection);
		HRESULT OnTxInPlaceDeactivate();
		HRESULT OnTxInPlaceActivate(LPCRECT prcClient);
		BOOL GetActiveState(void) { return fInplaceActive; }
		BOOL DoSetCursor(RECT *prc, POINT *pt);
		void SetTransparent(BOOL fTransparent);
		void GetControlRect(LPRECT prc);
		LONG SetAccelPos(LONG laccelpos);
		WCHAR SetPasswordChar(WCHAR chPasswordChar);
		void SetDisabled(BOOL fOn);
		LONG SetSelBarWidth(LONG lSelBarWidth);
		BOOL GetTimerState();

		void SetCharFormat(CHARFORMAT2W &c);
		void SetParaFormat(PARAFORMAT2 &p);

		// 
		//	IUnknown 需要实现的接口
		// 
		virtual HRESULT _stdcall QueryInterface(REFIID riid, void **ppvObject);
		virtual ULONG _stdcall AddRef(void);
		virtual ULONG _stdcall Release(void);

		// 
		//	ITextHost 需要实现的接口
		// 
		virtual HDC TxGetDC();
		virtual INT TxReleaseDC(HDC hdc);
		virtual BOOL TxShowScrollBar(INT fnBar, BOOL fShow);
		virtual BOOL TxEnableScrollBar(INT fuSBFlags, INT fuArrowflags);
		virtual BOOL TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw);
		virtual BOOL TxSetScrollPos(INT fnBar, INT nPos, BOOL fRedraw);
		virtual void TxInvalidateRect(LPCRECT prc, BOOL fMode);
		virtual void TxViewChange(BOOL fUpdate);
		virtual BOOL TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight);
		virtual BOOL TxShowCaret(BOOL fShow);
		virtual BOOL TxSetCaretPos(INT x, INT y);
		virtual BOOL TxSetTimer(UINT idTimer, UINT uTimeout);
		virtual void TxKillTimer(UINT idTimer);
		virtual void TxScrollWindowEx(INT dx, INT dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT fuScroll);
		virtual void TxSetCapture(BOOL fCapture);
		virtual void TxSetFocus();
		virtual void TxSetCursor(HCURSOR hcur, BOOL fText);
		virtual BOOL TxScreenToClient(LPPOINT lppt);
		virtual BOOL TxClientToScreen(LPPOINT lppt);
		virtual HRESULT TxActivate(LONG * plOldState);
		virtual HRESULT TxDeactivate(LONG lNewState);
		virtual HRESULT TxGetClientRect(LPRECT prc);
		virtual HRESULT TxGetViewInset(LPRECT prc);
		virtual HRESULT TxGetCharFormat(const CHARFORMATW **ppCF);
		virtual HRESULT TxGetParaFormat(const PARAFORMAT **ppPF);
		virtual COLORREF TxGetSysColor(int nIndex);
		virtual HRESULT TxGetBackStyle(TXTBACKSTYLE *pstyle);
		virtual HRESULT TxGetMaxLength(DWORD *plength);
		virtual HRESULT TxGetScrollBars(DWORD *pdwScrollBar);
		virtual HRESULT TxGetPasswordChar(_Out_ TCHAR *pch);
		virtual HRESULT TxGetAcceleratorPos(LONG *pcp);
		virtual HRESULT TxGetExtent(LPSIZEL lpExtent);
		virtual HRESULT OnTxCharFormatChange(const CHARFORMATW * pcf);
		virtual HRESULT OnTxParaFormatChange(const PARAFORMAT * ppf);
		virtual HRESULT TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits);
		virtual HRESULT TxNotify(DWORD iNotify, void *pv);
		virtual HIMC TxImmGetContext(void);
		virtual void TxImmReleaseContext(HIMC himc);
		virtual HRESULT TxGetSelectionBarWidth(LONG *lSelBarWidth);

		//
		// ITextHost2 需要实现的接口
		//
		virtual BOOL		TxIsDoubleClickPending();
		virtual HRESULT		TxGetWindow(HWND *phwnd);
		virtual HRESULT	TxSetForegroundWindow();
		virtual HPALETTE	TxGetPalette();
		virtual HRESULT		TxGetEastAsianFlags(LONG *pFlags);
		virtual HCURSOR		TxSetCursor2(HCURSOR hcur, BOOL bText);
		virtual void		TxFreeTextServicesNotification();
		virtual HRESULT		TxGetEditStyle(DWORD dwItem, DWORD *pdwData);
		virtual HRESULT		TxGetWindowStyles(DWORD *pdwStyle, DWORD *pdwExStyle);
		virtual HRESULT		TxShowDropCaret(BOOL fShow, HDC hdc, LPCRECT prc);
		virtual HRESULT 	TxDestroyCaret();
		virtual HRESULT		TxGetHorzExtent(LONG *plHorzExtent);

		// 
		//	ITextHost interface
		// 
	private:
		CDxRichEdit*		pRichEditUI{ nullptr };
		ULONG				cRefs;					// Reference Count
		ITextServices2*		pserv{ nullptr };	
		DWORD				dwStyle;				// style bits

		unsigned			fEnableAutoWordSel : 1;	// enable Word style auto word selection?
		unsigned			fWordWrap : 1;	// Whether control should word wrap
		unsigned			fAllowBeep : 1;	// Whether beep is allowed
		unsigned			fRich : 1;	// Whether control is rich text
		unsigned			fSaveSelection : 1;	// Whether to save the selection when inactive
		unsigned			fInplaceActive : 1; // Whether control is inplace active
		unsigned			fTransparent : 1; // Whether control is transparent
		unsigned			fTimer : 1;	// A timer is set
		unsigned			fCaptured : 1;
		unsigned			fShowCaret : 1;
		unsigned			fNeedFreshCaret : 1; // 修正改变大小后点击其他位置原来光标不能消除的问题

		INT					iCaretWidth;
		INT					iCaretHeight;
		INT					iCaretLastWidth;
		INT					iCaretLastHeight;
		LONG				lSelBarWidth;			// Width of the selection bar
		LONG  				cchTextMost;			// maximum text size
		DWORD				dwEventMask;			// DoEvent mask to pass on to parent window
		LONG				icf;
		LONG				ipf;
		RECT				rcClient;				// Client Rect for this control
		SIZEL				sizelExtent;			// Extent array
		CHARFORMAT2W		cf;					// Default character format
		PARAFORMAT2			pf;					    // Default paragraph format
		LONG				laccelpos;				// Accelerator position
		WCHAR				chPasswordChar;		    // Password character
	};

	LONG DXtoHimetricX(LONG dx, LONG xPerInch)
	{
		return (LONG)MulDiv(dx, HIMETRIC_PER_INCH, xPerInch);
	}

	LONG DYtoHimetricY(LONG dy, LONG yPerInch)
	{
		return (LONG)MulDiv(dy, HIMETRIC_PER_INCH, yPerInch);
	}

	HRESULT InitDefaultCharFormat(CDxRichEdit* re, CHARFORMAT2W* pcf, HFONT hfont)
	{
		memset(pcf, 0, sizeof(CHARFORMAT2W));
		LOGFONT lf;
		if (!hfont)
		{
			hfont = re->getFont();
		}
		::GetObject(hfont, sizeof(LOGFONT), &lf);

		DWORD dwColor = 0;
		unsigned char b = 0;
		unsigned char c = 0;
		unsigned char d = 255;


		pcf->cbSize = sizeof(CHARFORMAT2W);
		pcf->crTextColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
		LONG yPixPerInch = GetDeviceCaps(re->getDC(), LOGPIXELSY);
		pcf->yHeight = -lf.lfHeight * LY_PER_INCH / yPixPerInch;
		pcf->yOffset = 0;
		pcf->dwEffects = 0;
		pcf->dwMask = CFM_SIZE | CFM_OFFSET | CFM_FACE | CFM_CHARSET | CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE;
		if (lf.lfWeight >= FW_BOLD)
			pcf->dwEffects |= CFE_BOLD;
		if (lf.lfItalic)
			pcf->dwEffects |= CFE_ITALIC;
		if (lf.lfUnderline)
			pcf->dwEffects |= CFE_UNDERLINE;
		pcf->bCharSet = lf.lfCharSet;
		pcf->bPitchAndFamily = lf.lfPitchAndFamily;
#ifdef _UNICODE
		wcscpy(pcf->szFaceName, lf.lfFaceName);
#else
		//need to thunk pcf->szFaceName to a standard char string.in this case it's easy because our thunk is also our copy
		MultiByteToWideChar(CP_ACP, 0, lf.lfFaceName, LF_FACESIZE, pcf->szFaceName, LF_FACESIZE);
#endif

		return S_OK;
	}

	HRESULT InitDefaultParaFormat(CDxRichEdit* re, PARAFORMAT2* ppf)
	{
		memset(ppf, 0, sizeof(PARAFORMAT2));
		ppf->cbSize = sizeof(PARAFORMAT2);
		ppf->dwMask = PFM_ALL;
		ppf->wAlignment = PFA_LEFT;
		ppf->cTabCount = 1;
		ppf->rgxTabs[0] = lDefaultTab;

		return S_OK;
	}

	HRESULT CreateHost(CDxRichEdit *re, const CREATESTRUCT *pcs, RichEditImpl **pptec)
	{
		HRESULT hr = E_FAIL;
		//GdiSetBatchLimit(1);

		RichEditImpl *phost = new RichEditImpl();
		if (phost)
		{
			if (phost->Init(re, pcs))
			{
				*pptec = phost;
				hr = S_OK;
			}
		}

		if (FAILED(hr))
		{
			delete phost;
		}

		return TRUE;
	}

	RichEditImpl::RichEditImpl() : pRichEditUI(NULL)
	{
		::ZeroMemory(&cRefs, sizeof(RichEditImpl)-offsetof(RichEditImpl, cRefs));
		cchTextMost = 10000;
	}

	RichEditImpl::~RichEditImpl()
	{
		if (pserv){
			pserv->OnTxInPlaceDeactivate();
			pserv->Release();
		}
	}

	////////////////////// Create/Init/Destruct Commands ///////////////////////

	BOOL RichEditImpl::Init(CDxRichEdit *re, const CREATESTRUCT *pcs)
	{
		IUnknown *pUnk = NULL;
		HRESULT hr;

		pRichEditUI = re;
		cRefs = 1;
		if (FAILED(InitDefaultCharFormat(re, &cf, NULL)))
			goto err;
		if (FAILED(InitDefaultParaFormat(re, &pf)))
			goto err;
		dwStyle = ES_MULTILINE;


		if (!(dwStyle & ES_LEFT))
		{
			if (dwStyle & ES_CENTER)
				pf.wAlignment = PFA_CENTER;
			else if (dwStyle & ES_RIGHT)
				pf.wAlignment = PFA_RIGHT;
		}

		PCreateTextServices TextServicesProc = NULL;
		HMODULE hmod = LoadLibrary(L"msftedit.dll");
		if (hmod)
		{
			TextServicesProc = (PCreateTextServices)GetProcAddress(hmod, "CreateTextServices");
		}

		if (TextServicesProc)
		{
			HRESULT hr = TextServicesProc(NULL, this, &pUnk);
		}

		hr = pUnk->QueryInterface(IID_ITextServices2, (void **)&pserv);

		pUnk->Release();

		if (FAILED(hr))
		{
			goto err;
		}

		if (pcs && pcs->lpszName)
		{
			if (FAILED(pserv->TxSetText((TCHAR *)pcs->lpszName)))
				goto err;
		}

		return TRUE;

	err:
		return FALSE;
	}



	/////////////////////////////////  IUnknown ////////////////////////////////


	HRESULT RichEditImpl::QueryInterface(REFIID riid, void **ppvObject)
	{
		HRESULT hr = E_NOINTERFACE;
		*ppvObject = NULL;

		if (IsEqualIID(riid, IID_IUnknown)
			|| IsEqualIID(riid, IID_ITextHost))
		{
			AddRef();
			*ppvObject = (ITextHost*) this;
			hr = S_OK;
		}
		return hr;
	}

	ULONG RichEditImpl::AddRef(void)
	{
		return ++cRefs;
	}

	ULONG RichEditImpl::Release(void)
	{
		ULONG c_Refs = --cRefs;

		if (c_Refs == 0)
		{
			delete this;
		}

		return c_Refs;
	}

	/////////////////////////////////  Far East Support  //////////////////////////////////////

	HIMC RichEditImpl::TxImmGetContext(void)
	{
		return NULL;
	}

	void RichEditImpl::TxImmReleaseContext(HIMC himc)
	{
		::ImmReleaseContext(pRichEditUI->getHwnd(), himc);
	}

	//////////////////////////// ITextHost Interface  ////////////////////////////

	HDC RichEditImpl::TxGetDC()
	{
		return pRichEditUI->getDC();
	}

	int RichEditImpl::TxReleaseDC(HDC hdc)
	{
		return 1;
	}

	BOOL RichEditImpl::TxShowScrollBar(INT fnBar, BOOL fShow)
	{
		return TRUE;
	}

	BOOL RichEditImpl::TxEnableScrollBar(INT fuSBFlags, INT fuArrowflags)
	{
		return TRUE;
	}

	BOOL RichEditImpl::TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw)
	{
		return TRUE;
	}

	BOOL RichEditImpl::TxSetScrollPos(INT fnBar, INT nPos, BOOL fRedraw)
	{
		return TRUE;
	}

	void RichEditImpl::TxInvalidateRect(LPCRECT prc, BOOL fMode)
	{
		int a = 10;
		int b = a + a;
		return;
	}

	void RichEditImpl::TxViewChange(BOOL fUpdate)
	{
		int a = 10;
		int b = a + a;
		return;
	}

	BOOL RichEditImpl::TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight)
	{
		iCaretWidth = xWidth;
		iCaretHeight = yHeight;
		return ::CreateCaret(pRichEditUI->getHwnd(), hbmp, xWidth, yHeight);
	}

	BOOL RichEditImpl::TxShowCaret(BOOL fShow)
	{
		if (fShow)
			return ::ShowCaret(pRichEditUI->getHwnd());
		else
			return ::HideCaret(pRichEditUI->getHwnd());
	}

	BOOL RichEditImpl::TxSetCaretPos(INT x, INT y)
	{
		POINT ptCaret = { 0 };
		::GetCaretPos(&ptCaret);
		return ::SetCaretPos(x, y);
	}

	BOOL RichEditImpl::TxSetTimer(UINT idTimer, UINT uTimeout)
	{
		fTimer = TRUE;
	//	CDxRender::CreateTimer(idTimer, uTimeout);
		return TRUE;
	}

	void RichEditImpl::TxKillTimer(UINT idTimer)
	{
	//	CDxRender::KillTimer(idTimer);
		fTimer = FALSE;
	}

	void RichEditImpl::TxScrollWindowEx(INT dx, INT dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT fuScroll)
	{
		return;
	}

	void RichEditImpl::TxSetCapture(BOOL fCapture)
	{
		if (fCapture)
			::SetCapture(pRichEditUI->getHwnd());
		else
			::ReleaseCapture();
		fCaptured = fCapture;
	}

	void RichEditImpl::TxSetFocus()
	{
		::SetFocus(pRichEditUI->getHwnd());
	}

	void RichEditImpl::TxSetCursor(HCURSOR hcur, BOOL fText)
	{
		::SetCursor(hcur);
	}

	BOOL RichEditImpl::TxScreenToClient(LPPOINT lppt)
	{
		return ::ScreenToClient(pRichEditUI->getHwnd(), lppt);
	}

	BOOL RichEditImpl::TxClientToScreen(LPPOINT lppt)
	{
		return ::ClientToScreen(pRichEditUI->getHwnd(), lppt);
	}

	HRESULT RichEditImpl::TxActivate(LONG *plOldState)
	{
		return S_OK;
	}

	HRESULT RichEditImpl::TxDeactivate(LONG lNewState)
	{
		return S_OK;
	}

	HRESULT RichEditImpl::TxGetClientRect(LPRECT prc)
	{
		*prc = rcClient;
		return NOERROR;
	}

	HRESULT RichEditImpl::TxGetViewInset(LPRECT prc)
	{
		prc->left = prc->right = prc->top = prc->bottom = 0;
		return NOERROR;
	}

	HRESULT RichEditImpl::TxGetCharFormat(const CHARFORMATW **ppCF)
	{
		*ppCF = &cf;
		return NOERROR;
	}

	HRESULT RichEditImpl::TxGetParaFormat(const PARAFORMAT **ppPF)
	{
		*ppPF = &pf;
		return NOERROR;
	}

	COLORREF RichEditImpl::TxGetSysColor(int nIndex)
	{
		return ::GetSysColor(nIndex);
	}

	HRESULT RichEditImpl::TxGetBackStyle(TXTBACKSTYLE *pstyle)
	{
		*pstyle = !fTransparent ? TXTBACK_OPAQUE : TXTBACK_TRANSPARENT;
		return NOERROR;
	}

	HRESULT RichEditImpl::TxGetMaxLength(DWORD *pLength)
	{
		*pLength = cchTextMost;
		return NOERROR;
	}

	HRESULT RichEditImpl::TxGetScrollBars(DWORD *pdwScrollBar)
	{
		*pdwScrollBar = dwStyle & (WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL |
			ES_AUTOHSCROLL | ES_DISABLENOSCROLL);

		return NOERROR;
	}

	HRESULT RichEditImpl::TxGetPasswordChar(TCHAR *pch)
	{
		*pch = chPasswordChar;
		return NOERROR;
	}

	HRESULT RichEditImpl::TxGetAcceleratorPos(LONG *pcp)
	{
		*pcp = laccelpos;
		return S_OK;
	}

	HRESULT RichEditImpl::OnTxCharFormatChange(const CHARFORMATW *pcf)
	{
		return S_OK;
	}

	HRESULT RichEditImpl::OnTxParaFormatChange(const PARAFORMAT *ppf)
	{
		return S_OK;
	}

	HRESULT RichEditImpl::TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits)
	{
		DWORD dwProperties = 0;

		if (fRich)
		{
			dwProperties = TXTBIT_RICHTEXT;
		}

		if (dwStyle & ES_MULTILINE)
		{
			dwProperties |= TXTBIT_MULTILINE;
		}

		if (dwStyle & ES_READONLY)
		{
			dwProperties |= TXTBIT_READONLY;
		}

		if (dwStyle & ES_PASSWORD)
		{
			dwProperties |= TXTBIT_USEPASSWORD;
		}

		if (!(dwStyle & ES_NOHIDESEL))
		{
			dwProperties |= TXTBIT_HIDESELECTION;
		}

		if (fEnableAutoWordSel)
		{
			dwProperties |= TXTBIT_AUTOWORDSEL;
		}

		if (fWordWrap)
		{
			dwProperties |= TXTBIT_WORDWRAP;
		}

		if (fAllowBeep)
		{
			dwProperties |= TXTBIT_ALLOWBEEP;
		}

		if (fSaveSelection)
		{
			dwProperties |= TXTBIT_SAVESELECTION;
		}

		*pdwBits = dwProperties & dwMask;
		return NOERROR;
	}


	HRESULT RichEditImpl::TxNotify(DWORD iNotify, void *pv)
	{
		if (iNotify == EN_REQUESTRESIZE) {
			RECT rc;
			REQRESIZE *preqsz = (REQRESIZE *)pv;
			GetControlRect(&rc);
			rc.bottom = rc.top + preqsz->rc.bottom;
			rc.right = rc.left + preqsz->rc.right;
			SetClientRect(&rc);
		}
		return S_OK;
	}

	HRESULT RichEditImpl::TxGetExtent(LPSIZEL lpExtent)
	{
		*lpExtent = sizelExtent;
		return S_OK;
	}

	HRESULT	RichEditImpl::TxGetSelectionBarWidth(LONG *plSelBarWidth)
	{
		*plSelBarWidth = lSelBarWidth;
		return S_OK;
	}

	BOOL RichEditImpl::TxIsDoubleClickPending(){
		return TRUE;
	}

	HRESULT	RichEditImpl::TxGetWindow(HWND *phwnd){
		*phwnd = pRichEditUI->getHwnd();
		return S_OK;
	}

	HRESULT	RichEditImpl::TxSetForegroundWindow() {
		return S_OK;
	}

	HPALETTE RichEditImpl::TxGetPalette(){
		return nullptr;
	}

	HRESULT	RichEditImpl::TxGetEastAsianFlags(LONG *pFlags){
		*pFlags = 0;
		return S_OK;
	}

	HCURSOR	RichEditImpl::TxSetCursor2(HCURSOR hcur, BOOL bText){
		return hcur;
	}

	void RichEditImpl::TxFreeTextServicesNotification(){
		return;
	}

	HRESULT	RichEditImpl::TxGetEditStyle(DWORD dwItem, DWORD *pdwData){
		return S_OK;
	}

	HRESULT	RichEditImpl::TxGetWindowStyles(DWORD *pdwStyle, DWORD *pdwExStyle){
		return S_OK;
	}

	HRESULT RichEditImpl::TxShowDropCaret(BOOL fShow, HDC hdc, LPCRECT prc){
		return S_OK;
	}

	HRESULT RichEditImpl::TxDestroyCaret(){
		return S_OK;
	}

	HRESULT	RichEditImpl::TxGetHorzExtent(LONG *plHorzExtent){
		return S_OK;
	}

	//
	//////////////////////////////////////////////////////////
	//
	void RichEditImpl::SetWordWrap(BOOL fWordWrap)
	{
		fWordWrap = fWordWrap;
		pserv->OnTxPropertyBitsChange(TXTBIT_WORDWRAP, fWordWrap ? TXTBIT_WORDWRAP : 0);
	}

	BOOL RichEditImpl::IsReadOnly()
	{
		return (dwStyle & ES_READONLY) != 0;
	}

	void RichEditImpl::SetReadOnly(BOOL fReadOnly)
	{
		if (fReadOnly)
		{
			dwStyle |= ES_READONLY;
		}
		else
		{
			dwStyle &= ~ES_READONLY;
		}

		pserv->OnTxPropertyBitsChange(TXTBIT_READONLY,
			fReadOnly ? TXTBIT_READONLY : 0);
	}

	void RichEditImpl::SetFont(HFONT hFont)
	{
		if (hFont == NULL) return;
		LOGFONT lf;
		::GetObject(hFont, sizeof(LOGFONT), &lf);
		LONG yPixPerInch = ::GetDeviceCaps(pRichEditUI->getDC(), LOGPIXELSY);
		cf.yHeight = -lf.lfHeight * LY_PER_INCH / yPixPerInch;
		if (lf.lfWeight >= FW_BOLD) cf.dwEffects |= CFE_BOLD;
		else cf.dwEffects &= ~CFE_BOLD;
		if (lf.lfItalic) cf.dwEffects |= CFE_ITALIC;
		else cf.dwEffects &= ~CFE_ITALIC;
		if (lf.lfUnderline) cf.dwEffects |= CFE_UNDERLINE;
		else cf.dwEffects &= ~CFE_UNDERLINE;
		cf.bCharSet = lf.lfCharSet;
		cf.bPitchAndFamily = lf.lfPitchAndFamily;
		_tcscpy(cf.szFaceName, lf.lfFaceName);
		pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE,
			TXTBIT_CHARFORMATCHANGE);
	}

	void RichEditImpl::SetColor(DWORD dwColor)
	{
		cf.crTextColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
		pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE,
			TXTBIT_CHARFORMATCHANGE);
	}

	SIZEL* RichEditImpl::GetExtent()
	{
		return &sizelExtent;
	}

	void RichEditImpl::SetExtent(SIZEL *psizelExtent)
	{
		sizelExtent = *psizelExtent;
		pserv->OnTxPropertyBitsChange(TXTBIT_EXTENTCHANGE, TXTBIT_EXTENTCHANGE);
	}

	void RichEditImpl::LimitText(LONG nChars)
	{
		cchTextMost = nChars;
		if (cchTextMost <= 0) cchTextMost = cInitTextMax;
		pserv->OnTxPropertyBitsChange(TXTBIT_MAXLENGTHCHANGE, TXTBIT_MAXLENGTHCHANGE);
	}

	BOOL RichEditImpl::IsCaptured()
	{
		return fCaptured;
	}

	BOOL RichEditImpl::IsShowCaret()
	{
		return fShowCaret;
	}

	void RichEditImpl::NeedFreshCaret()
	{
		fNeedFreshCaret = TRUE;
	}

	INT RichEditImpl::GetCaretWidth()
	{
		return iCaretWidth;
	}

	INT RichEditImpl::GetCaretHeight()
	{
		return iCaretHeight;
	}

	BOOL RichEditImpl::GetAllowBeep()
	{
		return fAllowBeep;
	}

	void RichEditImpl::SetAllowBeep(BOOL fAllowBeep)
	{
		fAllowBeep = fAllowBeep;

		pserv->OnTxPropertyBitsChange(TXTBIT_ALLOWBEEP,
			fAllowBeep ? TXTBIT_ALLOWBEEP : 0);
	}

	WORD RichEditImpl::GetDefaultAlign()
	{
		return pf.wAlignment;
	}

	void RichEditImpl::SetDefaultAlign(WORD wNewAlign)
	{
		pf.wAlignment = wNewAlign;
		pserv->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
	}

	BOOL RichEditImpl::GetRichTextFlag()
	{
		return fRich;
	}

	void RichEditImpl::SetRichTextFlag(BOOL fNew)
	{
		fRich = fNew;

		pserv->OnTxPropertyBitsChange(TXTBIT_RICHTEXT,
			fNew ? TXTBIT_RICHTEXT : 0);
	}

	LONG RichEditImpl::GetDefaultLeftIndent()
	{
		return pf.dxOffset;
	}

	void RichEditImpl::SetDefaultLeftIndent(LONG lNewIndent)
	{
		pf.dxOffset = lNewIndent;

		pserv->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
	}

	void RichEditImpl::SetClientRect(RECT *prc)
	{
		rcClient = *prc;

		LONG xPerInch = ::GetDeviceCaps(pRichEditUI->getDC(), LOGPIXELSX);
		LONG yPerInch = ::GetDeviceCaps(pRichEditUI->getDC(), LOGPIXELSY);
		sizelExtent.cx = DXtoHimetricX(rcClient.right - rcClient.left, xPerInch);
		sizelExtent.cy = DYtoHimetricY(rcClient.bottom - rcClient.top, yPerInch);

		pserv->OnTxPropertyBitsChange(TXTBIT_VIEWINSETCHANGE, TXTBIT_VIEWINSETCHANGE);
	}

	BOOL RichEditImpl::SetSaveSelection(BOOL f_SaveSelection)
	{
		BOOL fResult = f_SaveSelection;

		fSaveSelection = f_SaveSelection;
		pserv->OnTxPropertyBitsChange(TXTBIT_SAVESELECTION,
			fSaveSelection ? TXTBIT_SAVESELECTION : 0);

		return fResult;
	}

	HRESULT	RichEditImpl::OnTxInPlaceDeactivate()
	{
		HRESULT hr = pserv->OnTxInPlaceDeactivate();

		if (SUCCEEDED(hr))
		{
			fInplaceActive = FALSE;
		}

		return hr;
	}

	HRESULT	RichEditImpl::OnTxInPlaceActivate(LPCRECT prcClient)
	{
		fInplaceActive = TRUE;

		HRESULT hr = pserv->OnTxInPlaceActivate(prcClient);

		if (FAILED(hr))
		{
			fInplaceActive = FALSE;
		}

		return hr;
	}

	BOOL RichEditImpl::DoSetCursor(RECT *prc, POINT *pt)
	{
		RECT rc = prc ? *prc : rcClient;
		if (PtInRect(&rc, *pt))
		{
			RECT *prcClient = (!fInplaceActive || prc) ? &rc : NULL;
			pserv->OnTxSetCursor(DVASPECT_CONTENT, -1, NULL, NULL, pRichEditUI->getDC(),
				NULL, prcClient, pt->x, pt->y);

			return TRUE;
		}

		return FALSE;
	}

	void RichEditImpl::GetControlRect(LPRECT prc)
	{
		prc->top = rcClient.top;
		prc->bottom = rcClient.bottom;
		prc->left = rcClient.left;
		prc->right = rcClient.right;
	}

	void RichEditImpl::SetTransparent(BOOL f_Transparent)
	{
		fTransparent = f_Transparent;
		pserv->OnTxPropertyBitsChange(TXTBIT_BACKSTYLECHANGE, 0);
	}

	LONG RichEditImpl::SetAccelPos(LONG l_accelpos)
	{
		LONG laccelposOld = l_accelpos;
		laccelpos = l_accelpos;
		pserv->OnTxPropertyBitsChange(TXTBIT_SHOWACCELERATOR, 0);
		return laccelposOld;
	}

	WCHAR RichEditImpl::SetPasswordChar(WCHAR ch_PasswordChar)
	{
		WCHAR chOldPasswordChar = chPasswordChar;

		chPasswordChar = ch_PasswordChar;
		pserv->OnTxPropertyBitsChange(TXTBIT_USEPASSWORD,
			(chPasswordChar != 0) ? TXTBIT_USEPASSWORD : 0);

		return chOldPasswordChar;
	}

	void RichEditImpl::SetDisabled(BOOL fOn)
	{
		cf.dwMask |= CFM_COLOR | CFM_DISABLED;
		cf.dwEffects |= CFE_AUTOCOLOR | CFE_DISABLED;

		if (!fOn)
		{
			cf.dwEffects &= ~CFE_DISABLED;
		}

		pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE,
			TXTBIT_CHARFORMATCHANGE);
	}

	LONG RichEditImpl::SetSelBarWidth(LONG l_SelBarWidth)
	{
		LONG lOldSelBarWidth = lSelBarWidth;

		lSelBarWidth = l_SelBarWidth;

		if (lSelBarWidth)
		{
			dwStyle |= ES_SELECTIONBAR;
		}
		else
		{
			dwStyle &= (~ES_SELECTIONBAR);
		}
		pserv->OnTxPropertyBitsChange(TXTBIT_SELBARCHANGE, TXTBIT_SELBARCHANGE);
		return lOldSelBarWidth;
	}

	BOOL RichEditImpl::GetTimerState()
	{
		return fTimer;
	}

	void RichEditImpl::SetCharFormat(CHARFORMAT2W &c)
	{
		cf = c;
	}

	void RichEditImpl::SetParaFormat(PARAFORMAT2 &p)
	{
		pf = p;
	}
}