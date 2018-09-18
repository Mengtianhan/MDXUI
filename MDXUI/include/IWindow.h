

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include <comdef.h>
#include "MAppWindow.h"
class MAppManager;
namespace DxUI{
	//
	// IWindow 接口 但不一定是窗口
	// IWindow 和 IWidget 从某种意义上来说属于等同的东西，他们的存在主要是为了获取不同的uuid
	//
	interface __declspec(uuid("0F04B04D-B0EE-4857-8700-F6258DAADCD7")) IWindow :
		public MAppWindow
	{
		virtual HRESULT __stdcall QueryInterface(REFIID riid, void** pout) = 0;
		virtual void InitIWindow(MAppManager* Application) = 0;
		virtual void DestroyInstance() = 0;
	};

	interface __declspec(uuid("2E53F53C-C7C3-43CF-801B-D91D198EAF8E")) IWidget :
		virtual public MAppWindow
	{
		virtual HRESULT __stdcall QueryInterface(REFIID riid, void** pout) = 0;
		virtual void InitIWindow(MAppManager* pAppManager) = 0;
		virtual void DestroyInstance() = 0;
	};
}


