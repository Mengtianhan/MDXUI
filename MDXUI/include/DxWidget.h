

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
#include "DxRendImpl.h"
#include "DxEffects.h"

namespace DxUI{
	typedef const DxDimI& CDxDimI;
	class CDxUIEvent;
	class CDxLayout;
	class CDxCaption;
	class CDxStackedWidget;

	//+------------------------------------
	//
	// 控件窗口的基类
	// 每一个窗口都有一个保证不重复的id作为身份标识
	// 其中4个特殊按钮的ID
	// 0 用于最大化
	// 1 用于还原最大化
	// 2 用于最小化
	// 3 用于关闭窗口
	//
	//+------------------------------------
	class DXUI_API CDxWidget : public  CDxRendImpl, public INotifyUI
	{
		DECLARE_CLASS(CDxWidget)

		friend class CDxStackedWidget;
		friend class CDxTableWidget;
	public:
		CDxWidget();
		~CDxWidget();


		//
		// 获取窗口自描述
		//
		virtual DXWindowType	GetWindowSelfDesc() const;

		//
		// 根据窗口坐标查找窗口
		//
		CDxWidget*		GetWindowFromPoint(int x, int y, HWND hwnd);
		CDxWidget*		GetWindowFromIdentify(int identify);
		const CDxEffects* GetEffects() const;
		CDxEffects*		GetEffects();
		CDxEffects*		GetSurfaceEffects();
		void			SetSurfaceEnable(bool isEnable);
		void			SetEffects(const CDxEffects* effects);


		//+-------------------
		//
		// 定时器相应函数
		//
		//+--------------------
		void			OnTimer(){}




		//+---------------------------
		//
		// 创建Hwnd
		//
		//+---------------------------
		virtual void	CreateHwnd();
		virtual void	CreateHwnd(HWND parent);

		virtual void	SetParent(CDxWidget* parent);
		virtual CDxWidget* GetParent();
		virtual CDxWidget* GetVisibleParent();


	

		//+-----------------
		//
		// 设置窗口位置
		//
		//+------------------
		virtual void	SetGeometyDim(CDxDimI x, CDxDimI y, CDxDimI w, CDxDimI h);
		virtual void	GetGeometyDim(DxDimI& x, DxDimI& y, DxDimI& w, DxDimI& h);
		virtual void	SetGeomety(const DxRectI& rc);
		virtual void	SetPosition(int x, int y);
		virtual void	SetSize(int w, int h);
		virtual void	UpdateArea();


		//+--------------------------------------
		//
		// 修改相对坐标的偏移信息
		//
		//+--------------------------------------
		void			SetXOffset(int offset);
		void			SetYOffset(int offset);
		void			SetWOffset(int offset);
		void			SetHOffset(int offset);

		void			AddXOffset(int offset);
		void			AddYOffset(int offset);
		void			AddWOffset(int offset);
		void			AddHOffset(int offset);


		//
		// 设置限制区域
		//
		virtual void	SetLimitSize(int minx, int maxx, int miny, int maxy);
		virtual void	GetLimitSize(int& minx, int& maxx, int& miny, int& maxy);

		//
		// 图像和文本的区域计算方式是基于 mRendArea 区域的
		//
		virtual void	SetImageArea(CDxDimI x, CDxDimI y, CDxDimI w, CDxDimI h);
		virtual void	SetTextArea(CDxDimI x, CDxDimI y, CDxDimI w, CDxDimI h);
		virtual void	GetTextArea(DxDimI& x, DxDimI& y, DxDimI& w, DxDimI& h);
		virtual void	GetIamgeArea(DxDimI& x, DxDimI& y, DxDimI& w, DxDimI& h);


		virtual void	SetIamgeIsExpend(bool isExpend);
		virtual void	SetTextAreaIsLimit(bool isLimit);
	
	
		//
		// 根据id查找窗口 或者 根据 坐标查找窗口
		// 递归查找 子窗口
		//
		virtual CDxWidget*	FindWindowFromId(int id);
		virtual CDxWidget*	HitTest(int x, int y);  // 点击测试，通过一个点查找到相应的控件

		//
		// 查询一个点是否落在窗口中
		//
		virtual bool		PointInArea(int x, int y); // 检查一个点是否落于控件之中


		//
		// 子窗口会回调该函数将事件传递到父窗口
		//
		virtual void	ReceiveEvent(CDxUIEvent* e);
		virtual void	SetFocus();
		virtual void	KillFocus();
		virtual void	StartTimer(int mselapsed); // 开启定时器
		virtual void	StopTimer();  // 关闭定时器

		//
		// 将事件传递出去
		// 该操作将会路由到父窗口的 ReceiveEvent 中
		//
		void			SendEvent(CDxUIEvent* e);

		//
		// 窗口尺寸是否跟随父窗口的尺寸
		//
		void			SetWidthFlowParent(bool isFlow);
		void			SetHeightFlowParent(bool isFlow);


		bool			IsWidthFlowParent() const;
		bool			IsHeightFlowParent() const;

		//
		// 使用相对坐标系
		// 相对坐标系使用的是 一个比例因子 + 偏移大小
		//
		void			SetCoordMethod(bool isRelativeCoord);

		//
		// 长宽比例
		//
		void			SetAspect(double aspect);

		//
		// Pad 将会影响到 文本区域 或者图片区域
		//
		void			SetPadding(RECT pad);
		RECT			GetPadding() const;
		bool			IsRelativeCoord() const;
		double			GetAspect() const;

		//
		// 获取当前窗口依赖的HWND
		//
		virtual HWND	GetRootHwnd() const;

		//
		// 获取当前窗口相对于父窗口的偏移大小 必须是带有 HWND 的窗口
		//
		virtual POINT	GetRootOffset() const;


		//+------------------------------------
		//
		// 固定大小的控件
		//
		//+------------------------------------
		virtual void	SetFixWidth(int w);
		virtual void	SetFixHeight(int h);
		virtual void	SetFixSize(int w, int h);
		virtual int		GetFixWidth() const;
		virtual int		GetFixHeight() const;
		virtual SIZE	GetHintSize() const;




		//+--------------------------------------
		//
		// 边框相关
		//
		//+--------------------------------------
		virtual void	SetSizeBox(const RECT& rect);
		virtual void	SetCaptionBox(const RECT& rect);
		virtual void	SetMaxSize(const SIZE& size);
		virtual void	SetMinSize(const SIZE& size);
		virtual void	SetMaxHeight(int height);
		virtual void	SetMaxWidth(int width);
		virtual void	SetMinHeight(int height);
		virtual void	SetMinWidth(int width);
		virtual void	SetRoundRadius(const SIZE& size);
		virtual void	SetBorderSize(double size);
		virtual void	SetBorderColor(const DxColor& col);
		virtual void	SetNeedBoder(bool isNeed);
		virtual void	SetNeedBorder(bool isNeed); // 修改上面错误的方法名

		virtual RECT    GetSizeBox() const;
		virtual RECT	GetCaptionRect() const;
		virtual SIZE    GetMaxSize() const;
		virtual SIZE	GetMinSize() const;
		virtual SIZE	GetRoundRectSize() const;
		virtual int		GetBorderSize() const;
		virtual DxColor GetBorderColor() const;
		virtual bool	IsNeedBorder() const;

		virtual int		GetWidth() const;
		virtual int		GetHeight() const;
		virtual int		GetX() const;
		virtual int		GetY() const;



		//+----------------------------------
		//
		// 尺寸
		//
		//+----------------------------------
		virtual void	ReSize(unsigned w, unsigned h);
		virtual DxRectI	GetClientRect() const;
		virtual DxRectI	GetFrameRect() const;
		virtual DxRectI	GetGlobalRect() const;


		//
		// 获取一个无效区域 当需要重绘该窗口的时候需要该区域作为重绘区域
		//
		virtual void	SetInvalidateRect(const DxRectI& rc);
		virtual DxRectI	GetInvalidateRect() const;
		virtual void	SetEraseColor(const DxColor& col);
		virtual DxColor GetEraseColor() const;
		virtual void	SetClipRect(const DxRectI& rect);
		virtual DxRectI& GetClipRect();
		virtual const DxRectI& GetClipRect() const;
		virtual void	SetSelected(bool isSelected);
		virtual bool	IsSelected();
		virtual void	SetReadOnly(bool isReadOnly);



		//+-----------------------------------
		//
		// 布局
		//
		//+-----------------------------------
		virtual void		SetLayout(CDxLayout* layout);
		virtual CDxLayout*	GetLayout() const;
		virtual void		SetSpan(int index, int span){}
		virtual void		SetMargin(int left, int top, int right, int bottom){}
		virtual void		SetMargin(int margin){}
		virtual void		SetSeparation(int separation){}
		virtual	void		AddWidget(CDxWidget* widget, int row, int col, int rowspan, int colspan, DXAlignment alignment = Dx_AlignCenter){}
		virtual void		OnLayoutWindowChanged(bool isAdd, CDxWidget* window);
		virtual void		LayoutWindowChanged(bool isAdd, CDxWidget* window);

		


		//+-----------------------
		//
		// 文本
		//
		//+-----------------------
		MString&		GetText();
		const MString&	GetText() const;
		virtual void	SetText(const MString& content);
		virtual void	AppendText(const MString& content);
		virtual void	SetTextAlignment(DXTextAlignment aligment);
		virtual void	Clear();


		//+------------------------
		//
		// 提示
		//
		//+-----------------------
		void SetToolTip(const MString& Tip);
		const MString& GetToolTip() const;



		//+---------------------
		//
		// 状态
		//
		//+---------------------
		void			SetDisabel();
		void			SetEnabel();
		bool			IsDisable();
		bool			IsEnabel();
		void			SetEnabled(bool isEnable);
		void			SetDisableColor(const DxColor& col);


		virtual void	SetStackedWindowIsVisible(bool isVisible);


		//+----------------------
		//
		// 添加标题栏
		//
		//+----------------------
		virtual void	SetCaptionHeight(int h);
		virtual int		GetCaptionHeight() const;
		virtual void	SetCaption(CDxCaption* caption = nullptr);
		CDxCaption*		GetCaption();
		virtual void	SetTitle(const MString& Title);
		const MString&	GetTitle() const;
		virtual void	SetIcon(const MString& Icon);
		const MString&  GetIcon() const;

		MString			GetParentIcon() const;
		MString         GetParentTitle() const;


		//+-----------------------
		//
		// 接受操作系统的通知
		//
		//+-----------------------
		bool			OnNotify(DXNotifyUI* NotifyEvent);


		//+-----------------------
		//
		// 更新子窗口的区域
		//
		//+------------------------
		virtual void	UpdateChildWindowPos();


		//
		// 当尺寸发生变化将执行 OnResizeEvent 函数
		// 当文本发生改变时将执行 OnTextChangedEvent
		// 当字体发生改变时将执行 OnFontChangedEvent
		//
		virtual void	OnResizeEvent();
		virtual void	OnForceUpdate();
		virtual void	OnTextChangedEvent(){}
		virtual void	OnFontChangedEvent(){}
		virtual void	OnQueueEvent(unsigned queueId){}
		virtual void	WndDestroyEvent(CDxWidget* sender);




		//+-----------
		//
		// 显示窗口
		//
		//+-----------
		virtual void		Show();
		virtual void		ShowMaximized();
		virtual void		Show(int x, int y);
		virtual void		Hide();
		virtual int			Exec();

		bool				IsNeedRender();


		//+----------------------
		//
		// 子窗口
		//
		//+----------------------
		virtual void	AddChild(CDxWidget* childWindow);
		virtual void	RemoveChild(CDxWidget* childWindow);
		virtual bool	HaveChild() const;


		//
		// 每个自定义的窗口重写该函数进行窗口绘制
		//
		virtual void	OnRendWindow(IPainterInterface* painter); 



		//
		// 设置背景色
		//
		void			SetBackGroundColor(const DxColor& col);
		const DxColor&  GetBackGroundColor() const;
		virtual	void	SetOpacity(double val); // 设置窗口的不透明度[0,1] 1 表示不透明 0 表示全透明
		double			GetOpacity() const;
		virtual void	SetCoordHwnd(HWND hwnd);
		virtual bool	HasFloatWindow(); // 绘制浮动窗口
		void			OnIdle();


		//
		// 可通过外部将句柄传递进来用于和其他框架交互
		// 目前只支持Attach 不支持Detach
		//
		void			AttachHwnd(HWND hwnd);
		


		//+---------------------------------
		//
		// 数据成员
		//
		//+---------------------------------
	protected:
		CDxWidget*				pParent{ nullptr };
		CDxLayout*				pLayout{ nullptr };
		CDxCaption*				pCaptionLabel{ nullptr };

		DxDimI					mXDim;
		DxDimI					mYDim;
		DxDimI					mWDim;
		DxDimI					mHDim;



		DxRectI					mRendArea;
		DxRectI					mFrameArea;
		DxRectI					mGlobalArea;   
		DxRectI					mCacheRect;
		DxRectI					mInvalidRect;
		RECT					mPadding;
		DxRectI					mOldClipRect;
		DxRectI					mClipRect;// 裁剪区域

		double					mAspect{ 1.0 };
		bool					bIsUseRelativCoord{ false };


		int						mFixWidth{ -1 };
		int						mFixHeight{ -1 };
		bool					bWidthFlowParent{ false }; // 宽度跟随父窗口的变化而变化
		bool					bHeightFlowParent{ false };// 高度跟随父窗口的变化而变化

		//
		// 绘制图片的区域
		//
		DxDimI					mImageAreaX;
		DxDimI					mImageAreaY;
		DxDimI					mImageAreaW;
		DxDimI					mImageAreaH;
		DxRectI					mImageRendArea;
		bool					bIsExpend{ true };// 如果是扩展 使用窗口区域进行绘制


		//
		// 文本区域
		//
		DxDimI					mTextAreaX;
		DxDimI					mTextAreaY;
		DxDimI					mTextAreaW;
		DxDimI					mTextAreaH;
		DxRectI					mTextRendArea;


		//
		// 状态
		//
		DxColor					mDisabelColor;
		DxColor					mBackGroundColor;
		DxColor					mClearColor;
		


		//
		// 内容
		//
		MString					mText;
		DxColor					mTextColor;
		DXTextAlignment			mTextAlignment{ Dx_AlignNormal };
		MString					mTitle;
		MString					mIcon;
		MString					mToolTip;


		//
		// 子窗口
		//
		TL::Vector<CDxWidget*>	mChildList;

		//
		// 边框
		//
		bool						bIsNeedBorder{ false };
		double						mBorderWidth{ 0 };
		double						mOldBorderWidth{ 0 };
		DxColor						mBorderColor;


		//
		// 效果
		//
		CDxEffects					mEffects;
		HWND						mCoordHwnd{ nullptr };

		//
		// 表面效果
		// 也就是在完成绘制之后在表面上一层效果
		// 通常只能是颜色之类的啦
		// 否则就会直接影响到实际的内容
		//
		CDxEffects					mSurfaceEffects;
		bool						bIsNeedSurfaceEffects{ false };
	};



	//+--------------------
	//
	// CDxCaption 
	//
	//+--------------------
	class DXUI_API CDxCaption : public CDxWidget{
		DECLARE_CLASS(CDxCaption)
	public:
		CDxCaption();
		void	CreateHwnd();
		void	CreateHwnd(HWND parent);
		void	OnRendWindow(IPainterInterface* painter); 

		CDxEffects*			GetIconEffects();
		CDxEffects*			GetMinButtonEffects();
		CDxEffects*			GetMaxButtonEffects();
		CDxEffects*			GetCloseButtonEffects();
		CDxEffects*			GetRestoreButtonEffects();
	private:
		CDxWidget*			pIconLabel{ nullptr };
		CDxWidget*			pMinButton{ nullptr };
		CDxWidget*			pMaxButton{ nullptr };
		CDxWidget*			pRestoreButton{ nullptr };
		CDxWidget*			pCloseButton{ nullptr };
	};




	//+--------------------------------------------
	//
	// 对象转换
	//
	//+--------------------------------------------
	template<class T>
	T* object_cast(CDxWidget* ptr){
		try{
			T* obj = dynamic_cast<T*>(ptr);
			return obj;
		}
		catch (...){
			return nullptr;
		}
	}
}


