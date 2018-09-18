

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DxListItem.h"


namespace DxUI{

	class CDxDecorator;
	//+----------------------------------------
	//
	// 可以作为独立window使用
	// 
	// +------------------------------+
	// |   标题                 | ▲  |
	// |------------------------------|
	// |							  |
	// |							  |
	// |							  |
	// +------------------------------+
	//
	//+---------------------------------------
	class DXUI_API CDxItemWindow :
		public CDxListItem
	{
		friend class CDxListWindow;
		friend class CDxComboBox;
		friend class CDxListWidget;

		DECLARE_CLASS(CDxItemWindow)
	public:
		CDxItemWindow();
		~CDxItemWindow();
		CDxListItem*	CreateThis();
		bool			DestroyThis();
		void			SetHeaderHeight(int height);
		void			SetSpliterHeight(int height);
		CDxEffects*		GetSpliterEffects();
		CDxDecorator*   GetDecorator();
		int				GetHeaderHeight() const;
		
	protected:
		int				GetHeight() const;
		CDxWidget*		HitTest(int x, int y);
		bool			PointInArea(int x, int y);
		void			SetSelected(bool selected);
		void			ReceiveEvent(CDxUIEvent* e);
		void			UpdateChildWindowPos();
		void			UpdateArea();
		void			OnRendWindow(IPainterInterface* painter);

	private:
		bool			bIsExpansion{ true };
		int				mHeaderHeight{ 30 };
		int				mSpliterHeight{ 1 };
		DxRectI			mNewFrameRect;
		DxRectI			mNewRendRect;
		CDxEffects		mSpliterEffects;
		CDxDecorator*	pDeracotor{ nullptr };
		
	};
}


