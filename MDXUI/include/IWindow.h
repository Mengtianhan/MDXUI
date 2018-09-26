

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


