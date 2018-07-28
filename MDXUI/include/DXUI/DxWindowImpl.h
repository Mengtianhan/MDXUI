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

		virtual void			SetIdentifyId(int identifyId);
		virtual int				GetIdentifyId() const;
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

