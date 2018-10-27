

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution
// Copyright (C) 2017-2018 by Mengjin (sh06155@hotmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
///////////////////////////////////////////////////////////////////////////////


#pragma once
#include "DxWindow.h"
#include "DxMenu.h"
#include "DxFlatButton.h"

namespace DxUI{

	// +--------------------------------------
	//
	// CDxMenuBar  菜单栏
	//
	//+--------------------------------------
	class CDxAction;
	class DXUI_API CDxMenuBar :
		public CDxWindow
	{
		DECLARE_CLASS(CDxMenuBar)
	public:
		CDxMenuBar();
		~CDxMenuBar();

		CDxMenu*		AddMenu(const MString& Name);
		CDxMenu*		InsertMenu(const MString& Name, int index); // 在指定位置插入一个菜单
		void			RemoveMenu(const MString& Name);
		
		CDxMenu*		GetMenu(const MString& Name);
	protected:
		bool			OnNotify(DXNotifyUI* NotifyEvent);
		void			UpdateChildWindowPos();


	private:
		TL::Map<MString, CDxFlatButtonEx*>    m_NameMaps;;
		TL::Vector<CDxFlatButtonEx*>		  m_ShowItems;
		TL::Map<CDxFlatButtonEx*, CDxMenu*>   m_MenuMaps;
		CDxFlatButtonEx*					  p_CurrentItem{ nullptr };
		CDxMenu*							  p_CurrentMenu{ nullptr };
		bool								  bIsClicked{ false };
	};
}


