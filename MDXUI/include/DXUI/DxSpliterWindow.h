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


