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


