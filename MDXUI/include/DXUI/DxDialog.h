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
			Accepted = 0,
			Rejected
		};
	public:
		CDxDialog();
		~CDxDialog();
		DXWindowType		GetWindowSelfDesc() const;
		HWND				GetRootHwnd() const;
		virtual void		SetParent(CDxWidget* parent);
		virtual CDxWidget*  GetParent();
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
}


