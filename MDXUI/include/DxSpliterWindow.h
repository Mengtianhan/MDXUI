

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DxWindow.h"

namespace DxUI{
	//
	// class CDxSpliterWindow
	// 分割窗口
	//
	class DXUI_API CDxSpliterWindow :
		public CDxWindow
	{
		DECLARE_CLASS(CDxSpliterWindow)
	public:
		CDxSpliterWindow();
		CDxSpliterWindow(DXDirection Direct); // 水平或者垂直
		~CDxSpliterWindow();

		void	SetSpliteDirection(DXDirection Direct);
		void	SetFirstWindowSize(int Size); // 设置一个窗口初始高度或者初始宽度
		void	SetFirstWindowRatio(double ratio); // 设置两个窗口的初始高度或者初始宽度的比例
		void	AddChild(CDxWidget* Window);
		void	UpdateChildWindowPos();
		bool	OnNotify(DXNotifyUI* NotifyEvent);
		void	OnRendWindow(IPainterInterface* painter);
	private:
		int			mFirstPartSize{ -1 };
		double		mFirstRatio{ 1.0 };
		DXDirection mExtentDirect;
		CDxWidget*  pWindow1{ nullptr };
		CDxWidget*  pWindow2{ nullptr };
		bool		bIsMove{ false };
		POINT		mOldPoint;
	};
}


