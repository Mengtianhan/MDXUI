

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

	//+------------------------
	//
	// class CDxDialog
	//
	//+------------------------
	class DXUI_API CDxDialog :
		public CDxWidget
	{
		DECLARE_CLASS(CDxDialog)
	public:

		//
		// 对话框退出码
		//
		enum{
			Rejected = 0,
			Accepted = 1,
		};
	public:
		CDxDialog();
		~CDxDialog();
		DXWindowType		GetWindowSelfDesc() const;
		HWND				GetRootHwnd() const;
		virtual void		SetParent(CDxWidget* parent);
		virtual CDxWidget*  GetParent();
		virtual CDxWidget*  GetDialogParent();
		virtual int			Exec();

		//
		// 退出对话框
		// Reject 返回码 == Rejected
		// Accept 返回码 == Accepted
		// Quit 返回码 == quitcode
		//
		void				Reject();
		void				Accept();
		void				Quit(int quitcode);

		ID2D1HwndRenderTarget*	GetRenderTarget();
		int						GetRetrunCode() const;

		std::condition_variable* GetConditionVariable();
	protected:
		void				UpdateChildWindowPos();
		void				OnRendWindow(IPainterInterface* painter);

	protected:
		std::condition_variable mCondition;
		int						mReturnCode;
	};



	//
	// 当跨线程执行对话框时可以使用下面的辅助函数来开启对话
	//
	struct DXUI_API CDxDialogHelp{
		static int  AsyncRunDialog(CDxDialog* Dialog);
	};
	
}


