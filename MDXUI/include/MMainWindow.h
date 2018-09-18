

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
	class MMenuBar;
	class MMenuBar2;
	//+------------------------------------
	//
	// class MMainWindow
	// 应用程序主窗口
	// 一个应用程序仅有一个主窗口
	//
	//+------------------------------------
	class DXUI_API MMainWindow : public CDxWidget
	{
		DECLARE_CLASS(MMainWindow)
	public:
		MMainWindow();
		~MMainWindow();

		DXWindowType	GetWindowSelfDesc() const;
		void		SetMenuBar(MMenuBar2* MenuBar);
		MMenuBar2*	GetMenuBar();

	protected:
		void	UpdateChildWindowPos();

	private:
		MMenuBar2*		pMenuBar{ nullptr };
	};
}


