

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DxTopWindow.h"

namespace DxUI{
	class DXUI_API CDxToolTipWindow :
		public CDxTopWindow
	{
		DECLARE_CLASS(CDxToolTipWindow)
	public:
		CDxToolTipWindow();
		~CDxToolTipWindow();

	protected:
		void		Show(int x, int y);
		void		OnRendWindow(IPainterInterface* painter);
	};
}


