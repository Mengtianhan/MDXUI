

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
#include "DxWindowImpl.h"
#include "DxConfig.h"
#include "MDx11BufferManage.h"
#include "MDx11LayoutManage.h"
#include "MDx11Camera.h"
#include "DxPainter.h"
#include <gdiplus.h>

namespace DxUI{
	

	class CDxWidget;

	//+-----------------------------
	//
	// class CDxMouseEvent
	//
	//+-----------------------------
	class DXUI_API CDxMouseEvent{
	public:
		CDxMouseEvent(DXMouseButton button, DXMouseAction action, POINT pt, POINT lastpt, bool isMove);

		POINT CurrentPos() const;
		POINT LastPos() const;
		DXMouseButton Button() const;
		DXMouseAction Action() const;
		bool		  IsMove() const;
	private:
		POINT mPoint;
		POINT mLastPoint;
		DXMouseButton   mButton;
		DXMouseAction	mAction;
		bool			bIsMove;
	};

	//+-----------------------------
	//
	// class CDxKeyBoardEvent
	//
	//+-----------------------------
	class DXUI_API CDxKeyBoardEvent{
	public:
		CDxKeyBoardEvent(WPARAM key,bool isPress);
		WPARAM Key() const;
		bool IsPress() const;
	private:
		WPARAM mKey;
		bool   bIsPress;
	};


	//+------------------------
	//
	//
	// class CDxRendImpl
	//
	//+-------------------------
	class DXUI_API CDxRendImpl : public CDxWindowImpl
	{
		DECLARE_CLASS(CDxRendImpl)
	public:
		CDxRendImpl();
		~CDxRendImpl();
		
		//+------------------------------------------
		//
		// 针对不同的窗口类型选择使用不同的初始化函数
		// OnInitResource2D 初始化2D窗口
		// OnInitResource3D 初始化3D绘制的资源
		// OnRender2D 绘制2D窗口
		// OnRender3D 绘制3D窗口
		//
		//+---------------------------------------------
		virtual bool OnInitResource2D();
		virtual bool OnInitResource3D();
		virtual void UnInitResource();
		virtual bool OnLoadContent();
		virtual void OnReShape(int w, int h);
		
		void		 OnRepaintEvent(); // 强制底层窗口重绘
		virtual void OnPainterEvent(); // 该函数可以在线程里面执行,异步发送绘制消息然后调用 OnRender
		virtual void OnRender();       // 该函数马上执行 线程里面是否可以直接调用未知
		virtual void OnRender2D();
		virtual void OnRender3D();
		virtual void SaveToFile(const MString& fileName);
		virtual void RendToBitmap(ID2D1RenderTarget* RendTargert,  int width, int height, ID2D1Bitmap*& outBitmap);
	
		virtual void OnRendWindow(IPainterInterface* painter);
	
		virtual void OnTimer();
		virtual bool OnMouseEvent(const CDxMouseEvent& e);
		virtual bool OnKeyBoardEvent(const CDxKeyBoardEvent& e);
		bool		 OnMessageHandle(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);

		virtual void			SetWindowShape(DXShape shape);
		virtual void			SetViewPort(const DxRectI& viewArea);
		virtual DXShape			GetWindowShape() const;
		virtual bool			SetWindowStatus(DXWindowStatus status);
		virtual DXWindowStatus	GetWindowStatus() const;
		virtual IPainterInterface* GetPainter();
		virtual IPainterInterface* GetPainter() const;

		//
		// 右键点击的时候会执行该函数
		// x y 鼠标点击处的坐标位置
		//
		virtual void ContextMenuEvent(int x, int y){}

		HDC						GetDC();
		virtual ID2D1HwndRenderTarget*	GetRenderTarget();
		

	msignals:
		//
		// 窗口状态改变时发送该事件
		//
		void					OnWindowStatusChanged(DXWindowStatus status,CDxWidget* sender);
		TL::MTLVoidEvent<DXWindowStatus, CDxWidget*> Event_WindowStatusChanged;

	protected:
		void					ClearWndRelatedResource();
		//
		// 空闲执行函数
		//
		virtual void			OnIdle();


	protected:

		//+---------------------
		//
		// 2D 渲染组件
		//
		//+----------------------
		ID2D1HwndRenderTarget*			pRendTarget{ nullptr };
		ID2D1GdiInteropRenderTarget*	pGdiRendTarget{ nullptr };
		HDC								mHDC{ nullptr };
		IPainterInterface*				pPainter{ nullptr };


		//+------------------------
		//
		// 渲染为位图
		//
		//+------------------------
		ID2D1RenderTarget*			pBitmapRendTarget{ nullptr };



		//+-----------------------
		//
		// 记录当前正在操作的对象
		//
		//+-------------------------
		CDxWidget*					pFocusWindow{ nullptr };
		CDxWidget*					pHoverWindow{ nullptr };
		CDxWidget*					pClickedWindow{ nullptr };
		bool						bIsTraceMouse{ true };
		POINT						mLastMousePos;
		bool						bRightButtonDown{ false };
		bool						bLeftButtonDown{ false };
		bool						bIsRending{ false };

		//+------------------------------------
		//
		//  提示信息
		//
		//+------------------------------------
		CDxWidget*					pToolTipWindow{ nullptr };
		int							mTooltipHoverTime{ 400 };


		DXWindowStatus				mWindowStatus{ Dx_Normal };
		DXShape						mWindowShape{ Dx_Rectangle };
	};

}

