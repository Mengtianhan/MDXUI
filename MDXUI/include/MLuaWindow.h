

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

#ifdef __MAPI_DEBUG__
#define LUA_DXUI_API 
#else
#if defined(DIRECTUI_EXPORTS)
#define LUA_DXUI_API __declspec(dllexport)
#else
#define LUA_DXUI_API __declspec(dllimport)
#endif
#endif 

namespace DxUI{
	class CDxWidget;
	class CDxApplication;
	class MSerialize;
	class CDxEffects;
}



using namespace DxUI;

struct lua_State;

//
// 用于包装CDxWindow
//
struct CDxWidgetWrap{
	CDxWidget* p_window = nullptr;
};


struct CDxApplicationWrap{
	CDxApplication* p_App = nullptr;
};


struct MXmlWrap{
	MSerialize* p_Xml = nullptr;
};


struct CDxEffectsWrap{
	CDxEffects* p_Effects = nullptr;
};

//+------------------------------------
//
// 辅助lua和C++交互操作
//
//+------------------------------------
extern "C" LUA_DXUI_API  CDxApplication* __stdcall GetAppliactionFromLua(lua_State* L);
extern "C" LUA_DXUI_API  MSerialize* __stdcall GetXmlFromLua(lua_State* L);
extern "C" LUA_DXUI_API  CDxWidget*	__stdcall GetWidgetFromLua(lua_State* L);



//
//+----------------------------------------------------------------------------------------
//
namespace LuaPlus{
	class LuaStateOwner;
}


class LUA_DXUI_API CDxLuaWindow{
public:
	CDxLuaWindow();
	~CDxLuaWindow();
	void		DoFile(const char* FileName);
	CDxWidget*  Window(const char* Name);
	LuaPlus::LuaStateOwner* GetState() const;
private:
	LuaPlus::LuaStateOwner*  p_LuaState{ nullptr };
};