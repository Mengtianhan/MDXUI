#pragma once
#include "DxWidget.h"

namespace DxUI{

	///+-----------------------------------------
	//
	// 带有标题栏以及最大最小化几个按钮的窗口
	// 作为主窗口使用
	//
	//+-------------------------------------------
	class DXUI_API CDxMainWindow : public CDxWidget
	{
		DECLARE_CLASS(CDxMainWindow)
	public:
		CDxMainWindow();
		~CDxMainWindow();

		CDxWidget*		hitTest(int x, int y);
		bool			pointInArea(int x, int y);
		void			setTitle(const MString& Title);
		void			setIcon(const MString& Icon);
		
	private:
		CDxWidget*		pIConLabel{ nullptr };
		CDxWidget*		pMinButton{ nullptr };
		CDxWidget*		pMaxButton{ nullptr };
		CDxWidget*		pStoreButton{ nullptr };
		CDxWidget*		pCloseButton{ nullptr };
		CDxWidget*		pTitle{ nullptr };
	};
}


