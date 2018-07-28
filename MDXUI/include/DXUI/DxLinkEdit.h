#pragma once
#include "DxEdit.h"
namespace DxUI{
	//+--------------------------
	// 
	// class CDxLinkEdit
	// 简单链接文本
	// 当点击之时会执行回调函数
	// 也可以使用connect进行回调连接
	// 
	//+---------------------------
	class DXUI_API CDxLinkEdit :
		public CDxEdit
	{
		DECLARE_CLASS(CDxLinkEdit)
		typedef std::function<void(const MString&)> RecallFunType;

	public:
		CDxLinkEdit();
		~CDxLinkEdit();

		bool			OnNotify(DXNotifyUI* NotifyEvent);
		void			OnRendWindow(IPainterInterface* painter);

		void			SetHoverColor(const DxColor& col);
		void			BindRecallFun(RecallFunType fun);
		void			Clicked(MString Text, CDxWidget* sender);
	private:
		RecallFunType	mFun{ nullptr };
		DxColor			mHoverColor;
	};
}


