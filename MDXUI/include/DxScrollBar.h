

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DxWidget.h"

namespace DxUI{

	//+--------------------------------
	//
	// 修饰器
	//
	//+--------------------------------
	class DXUI_API CDxDecorator : public CDxWidget
	{
	public:
		CDxDecorator();
		~CDxDecorator();
		void	CreateHwnd(){}
		void	CreateHwnd(HWND parent){}
		DxColor	GetEraseColor() const;
		void	SetColor(DXWindowStatus status, const DxColor& col);
		void	SetShowRegion(const TL::Vector<DxPointD>& points);
		bool	OnNotify(DXNotifyUI* NotifyEvent);
		void	OnRendWindow(IPainterInterface* painter);
	private:
		TL::Vector<DxPointD> mGeometryPoints;
	};


	//+---------------------------------
	//
	// 滚动条上的滑块
	//
	//+---------------------------------
	class DXUI_API CDxChuncker : public CDxWidget
	{
	public:
		CDxChuncker();
		~CDxChuncker();
		void	CreateHwnd(){}
		void	CreateHwnd(HWND parent){}
		DxColor	GetEraseColor() const;
		void	SetColor(DXWindowStatus status, const DxColor& col);
		bool	SetWindowStatus(DXWindowStatus status);
		bool	OnNotify(DXNotifyUI* NotifyEvent);
	private:
		POINT	mOldPoint;
		bool	bIsClicked{ false };
	};

	//+------------------------------------------
	//
	// 滑块的基类
	// 当滑块位置改变时触发函数 ValueChanged
	// CDxScrollBar bar;
	// TL::MTLDataBind::Connect(MSIGNAL(DxUI::CDxScrollBar, ValueChanged, &bar),TL::MSLOT(testFun));
	// testFun:
	// void testFun(double value,CDxWidget* sender);
	//
	//+--------------------------------------------
	class DXUI_API CDxScrollBar : public CDxWidget
	{
	public:
		CDxScrollBar();
		~CDxScrollBar();

		void			CreateHwnd(){}
		void			CreateHwnd(HWND parent){}
		DxColor			GetEraseColor() const;
		virtual	void	SetStep(double step);
		virtual void	SetRange(double minpos, double maxpos);
		virtual void	SetValue(double pos);
		virtual double	GetValue() const;
		void			GetRange(double& minpos, double& maxpos) const;
		CDxChuncker*	GetChunker();
		CDxChuncker*	GetChuncker() const;
		virtual void	Reset(){}


		void			ValueChanged(double val, CDxScrollBar* sender);
	protected:
		CDxChuncker			mChunker;
		double				mMinPos;
		double				mMaxPos;
		double				mCurrentPos;
		double				mStep;
		double				mChunkerOrgOffset{ 12 };
	};




	//+------------------------------------
	//
	// 水平滚动条
	//
	//+-------------------------------------
	class DXUI_API CDxHorizonScrollBar : public CDxScrollBar
	{
		DECLARE_CLASS(CDxHorizonScrollBar)
	public:
		CDxHorizonScrollBar();
		~CDxHorizonScrollBar();
		CDxDecorator*	GetLeftButton();
		CDxDecorator*	GetRightButton();
		void			SetValue(double pos);  // 设置滑块位置
		double			GetValue() const;      // 获取滑块当前位置

		double			GetLength();  //  获取滑块能够滑动的长度	
		void			SetSliderLength(double length); // 设置滑块的长度
		double			GetSliderLength();
		void			ReceiveEvent(CDxUIEvent* e);
		void			Reset();
		bool			OnNotify(DXNotifyUI* NotifyEvent);
		void			OnResizeEvent();
		void			OnRendWindow(IPainterInterface* painter);
	private:
		CDxDecorator		mLeftButton;
		CDxDecorator		mRightButton;
		bool				bIsClicked{ false };
		int					mWidth{ 0 };
	};




	//+----------------------------------
	//
	// 垂直滚动条
	//
	//+----------------------------------
	class DXUI_API CDxVerticalScrollBar : public CDxScrollBar
	{
		DECLARE_CLASS(CDxVerticalScrollBar)
	public:
		CDxVerticalScrollBar();
		~CDxVerticalScrollBar();

		CDxDecorator*	GetUpButton();
		CDxDecorator*	GetDownButton();
		void			SetValue(double pos);  // 设置滑块位置
		double			GetValue() const;      // 获取滑块当前位置

		double			GetLength();  //  获取滑块能够滑动的长度	
		void			SetSliderLength(double length); // 设置滑块的长度
		double			GetSliderLength();
		void			ReceiveEvent(CDxUIEvent* e);
		void			Reset();
		bool			OnNotify(DXNotifyUI* NotifyEvent);
		void			OnResizeEvent();
		void			OnRendWindow(IPainterInterface* painter);
	private:
		CDxDecorator		mUpButton;
		CDxDecorator		mDownButton;
		bool				bIsClicked{ false };
		int					mHeight{ 0 };
	};



	//+--------------------------------
	//
	// 水平滑块
	//
	//+--------------------------------
	class DXUI_API CDxHorizonSlider : public CDxScrollBar
	{
		DECLARE_CLASS(CDxHorizonSlider)
	public:
		CDxHorizonSlider();
		~CDxHorizonSlider();
		DXWindowType	GetWindowSelfDesc() const;
		SIZE			GetHintSize() const;
		void			SetValue(double pos);  // 设置滑块位置
		void			ReceiveEvent(CDxUIEvent* e);
		void			OnRendWindow(IPainterInterface* painter);
	};



	//+-----------------------------
	//
	// 垂直滑块
	//
	//+-----------------------------
	class DXUI_API CDxVerticalSlider : public CDxScrollBar
	{
		DECLARE_CLASS(CDxVerticalSlider)
	public:
		CDxVerticalSlider();
		~CDxVerticalSlider();
		DXWindowType	GetWindowSelfDesc() const;
		SIZE			GetHintSize() const;
		void			SetValue(double pos);  // 设置滑块位置
		void			ReceiveEvent(CDxUIEvent* e);
		void			OnRendWindow(IPainterInterface* painter);
	};
}


