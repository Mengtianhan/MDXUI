

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
#include "DxWnd.h"
#include "DxResource.h"

namespace DxUI{
	enum {
		DX_InvalidID = -10000000
	};

	class CDxWidget;

	//+--------------------------------------
	//
	// 窗口基类
	//
	//+-------------------------------------
	class DXUI_API CDxWindowImpl : public CDxWindowWnd
	{
		DECLARE_CLASS(CDxWindowImpl)
	public:
		CDxWindowImpl();
		~CDxWindowImpl();

		void					SetIdentifyId(int identifyId);
		void					RemoveIdentifyId(int identifyId);
		int						GetIdentifyId() const;
		virtual void			SetVisible(bool isVisible);
		virtual bool			IsVisible() const;
		virtual void			SetZoomable(bool isZoomable);
		virtual bool			IsZoomable() const;
		virtual CDxWidget*		GetWindowFromPoint(int x, int y,HWND hwnd);
		virtual CDxWidget*		GetWindowFromIdentify(int identify);
		virtual void			OnReShape(int w, int h){}
		virtual void			OnRender(){}
		virtual bool			OnMessageHandle(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);

		virtual UINT			GetClassStyle() const;
		virtual LRESULT			MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
		virtual LRESULT			OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT			OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

		virtual LRESULT			OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT			OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT			OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		virtual LRESULT			OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT			OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT			OnMouseWheel(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT			OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT			OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT			OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT			OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT			OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT			OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT			OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT			OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT			OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT			OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT			OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT			HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LONG			GetStyle();

		
		//
		// 窗口显示和隐藏的通知事件
		//
		void				    OnShowEvent(bool isShow, CDxWidget* sender);
	protected:

		int									 mIdentifyId{ DX_InvalidID };
		bool								 bIsVisible{ true };
		bool								 bIsZoomable{ true };
		bool								 bIsEnabel{ true };
		RECT								 mCaptionBox;
		RECT								 mSizeBox;
		SIZE								 mMaxSize;
		SIZE								 mMinSize;
		SIZE								 mRoundRectSize;

		static unsigned  					 getId();
		static TL::Vector<int>				 sIds;
		static TL::Map<int, CDxWindowImpl*>	 sObjMaps;
	};
}

