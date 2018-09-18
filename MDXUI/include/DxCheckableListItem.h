

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

	//+-----------------------
	//
	// 一个可选的列表项
	//
	//+-----------------------
	class DXUI_API CDxCheckableListItem :
		public CDxListItem
	{
		DECLARE_CLASS(CDxCheckableListItem)
	public:
		CDxCheckableListItem();
		~CDxCheckableListItem();
		CDxListItem*	CreateThis();
		bool			DestroyThis();
		void			SetIcon(const MString& icon);
		void			UpdateChildWindowPos();
		bool			OnNotify(DXNotifyUI* NotifyEvent);
		void			SetAny(const mj::MAny& any);
		bool			OnDrawItem(const RECT& rc, IPainterInterface* painter);
	private:
		bool			bIsChecked{ true };
		CDxEffects		mIconEffects;
	};
}


