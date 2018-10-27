
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
#include <Windows.h>
#include "DxConfig.h"
#include <MTLEvent.h>

namespace DxUI{

	//+------------------------------------------
	//
	// 预定义几个窗口风格
	//
	//+------------------------------------------
#define DXUI_WNDSTYLE_CONTAINER  (0)
#define DXUI_WNDSTYLE_FRAME      (WS_VISIBLE | WS_OVERLAPPEDWINDOW)
#define DXUI_WNDSTYLE_CHILD      (WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
#define DXUI_WNDSTYLE_DIALOG     (WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
#define DXUI_WNDSTYLE_MENU       (WS_VISIBLE | WS_POPUP)

#define DXUI_WNDSTYLE_EX_FRAME   (WS_EX_WINDOWEDGE)
#define DXUI_WNDSTYLE_EX_DIALOG  (WS_EX_TOOLWINDOW | WS_EX_DLGMODALFRAME)

#define DXUI_CLASSSTYLE_CONTAINER  (0)
#define DXUI_CLASSSTYLE_FRAME      (CS_VREDRAW | CS_HREDRAW)
#define DXUI_CLASSSTYLE_CHILD      (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)
#define DXUI_CLASSSTYLE_DIALOG     (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)


	//+--------------------------------------
	//
	// 封装窗口HWND
	//
	//+-------------------------------------
	class DXUI_API CDxWindowWnd : public TL::MTLObject
	{
		DECLARE_CLASS(CDxWindowWnd)
	public:
		CDxWindowWnd();
		~CDxWindowWnd();

		HWND					GetHwnd() const;
		operator				HWND() const;



		bool					RegisterWindowClass();
		bool					RegisterSuperclass();


		HWND					Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
		HWND					Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
		HWND					CreateDxWindow(HWND hwndParent, LPCTSTR pstrWindowName, DWORD dwStyle = 0, DWORD dwExStyle = 0);
		HWND					Subclass(HWND hWnd);
		void					Unsubclass();
		void					ShowWindow(bool bShow = true, bool bTakeFocus = true);
		UINT					ShowModal();
		virtual void			CloseHwnd(UINT nRet = IDOK);
		
		void					CenterWindow();	
		void					SetIcon(UINT nRes);
		void					SetCapture();
		void					ReleaseCapture();

		LRESULT					SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
		LRESULT					PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
		void					ResizeClient(int cx = -1, int cy = -1);
		void					MoveWindow(const RECT& rect,bool isNeedRedraw = true);



		//
		// 修改new操作符的行为
		//
		static void* operator new(size_t size);
		static void operator delete(void* ptr);


		//
		// 检查对象是否分配在栈上
		//
		bool IsStackedObject() const;

	protected:

		virtual LPCTSTR			GetSuperClassName() const;
		virtual UINT			GetClassStyle() const;
		virtual LRESULT			HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void			ClearWndRelatedResource(){} // 清除窗口相关资源
	protected:
		HWND					mHwnd;
		WNDPROC					mOldWndProc;
		bool					bIsSubclassed;
		bool					bIsZoomed{ false };
		RECT					m_FrameOrgRect;
		bool					b_IsStackedObj{ false };


		//
		// 窗口对象禁止复制和赋值
		//
	private:
		CDxWindowWnd(const CDxWindowWnd&) = delete;
		CDxWindowWnd& operator=(const CDxWindowWnd&) = delete;
	};
}


