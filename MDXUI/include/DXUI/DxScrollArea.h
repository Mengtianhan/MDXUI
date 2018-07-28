#pragma once
#include "DxWindow.h"
#include "DxScrollBar.h"

namespace DxUI{
	//+-------------------------
	//
	// class CDxScrollArea
	//
	//+--------------------------
	class DXUI_API CDxScrollArea :
		public CDxWindow
	{
		DECLARE_CLASS(CDxScrollArea)
	public:
		CDxScrollArea();
		~CDxScrollArea();

		void Attach(CDxWidget* window);
		CDxWidget* Dettach();
		void ReceiveEvent(CDxUIEvent* e);
		void UpdateChildWindowPos();
	protected:
		void OnDealItemForDistance(unsigned disx, unsigned disy);
		void OnRendWindow(IPainterInterface* painter);
	private:
		CDxHorizonScrollBar      mHorizonBar;
		CDxVerticalScrollBar	 mVerticalBar;
		CDxWidget*				 p_Window{ nullptr };
		int						 mXCurrentOffset{ 0 };
		int						 mYCurrentOffset{ 0 };
		int					     mXMaxOffset{ 0 };
		int						 mYMaxOffset{ 0 };
	};
}


