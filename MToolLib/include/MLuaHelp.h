#pragma once
#include <lua.hpp>
#include <lualib.h>
#include <MString.h>
#include <MToolLib>

namespace TL{
	using mj::MString;

	template<class T>
	struct MTLLuaToValue{
		static MString ToValue(lua_State* L, int index){
			return lua_tostring(L, index);
		}
		static void FromValue(lua_State* L, const MString& val){
			lua_pushstring(L, val.toStdString().c_str());
		}
	};

	template<>
	struct MTLLuaToValue<__int8>{
		static __int8 ToValue(lua_State* L, int index){
			return lua_tointeger(L, index);
		}
		static void FromValue(lua_State* L, __int8 val){
			lua_pushinteger(L, val);
		}
	};

	template<>
	struct MTLLuaToValue<__int16>{
		static __int16 ToValue(lua_State* L, int index){
			return lua_tointeger(L, index);
		}
		static void FromValue(lua_State* L, __int16 val){
			lua_pushinteger(L, val);
		}
	};

	template<>
	struct MTLLuaToValue<__int32>{
		static __int32 ToValue(lua_State* L, int index){
			return lua_tointeger(L, index);
		}
		static void FromValue(lua_State* L, __int32 val){
			lua_pushinteger(L, val);
		}
	};

	template<>
	struct MTLLuaToValue<__int64>{
		static __int64 ToValue(lua_State* L, int index){
			return lua_tointeger(L, index);
		}
		static void FromValue(lua_State* L, __int64 val){
			lua_pushinteger(L, val);
		}
	};

	template<>
	struct MTLLuaToValue<unsigned __int8> : public MTLLuaToValue<__int8>{};

	template<>
	struct MTLLuaToValue<unsigned __int16> : public MTLLuaToValue<__int16>{};

	template<>
	struct MTLLuaToValue<unsigned __int32> : public MTLLuaToValue<__int32>{};

	template<>
	struct MTLLuaToValue<unsigned __int64> : public MTLLuaToValue<__int64>{};

	template<>
	struct MTLLuaToValue<double>{
		static double ToValue(lua_State* L, int index){
			return lua_tonumber(L, index);
		}
		static void FromValue(lua_State* L, double val){
			lua_pushnumber(L, val);
		}
	};

	template<>
	struct MTLLuaToValue<float> : public MTLLuaToValue<double>{};


	template<>
	struct MTLLuaToValue<bool>{
		static bool ToValue(lua_State* L, int index){
			return lua_toboolean(L, index);
		}
		static void FromValue(lua_State* L, bool val){
			lua_pushboolean(L, val);
		}
	};


	template<>
	struct MTLLuaToValue<std::string>{
		static std::string ToValue(lua_State* L, int index){
			return lua_tostring(L, index);
		}
		static void FromValue(lua_State* L, const std::string& val){
			lua_pushstring(L, val.c_str());
		}
	};


	struct MTLReadLuaTable
	{
		template<class T>
		static std::vector<T> ReadTable(lua_State* L, int index){
			std::vector<T> res;
			int n = luaL_len(L, index);
			for (int i = 0; i < n; ++i){
				lua_rawgeti(L, index, i + 1);
				T val = MTLLuaToValue<T>::ToValue(L, -1);
				res.push_back(val);
				lua_pop(L, 1);
			}
			return res;
		}
	};



	struct MTLWriteLuaTable{
		template<class T>
		static void WriteTable(lua_State* L, const TL::Vector<T>& datas){
			if (!lua_istable(L, -1)){
				return;
			}
			for (int i = 0; i < datas.size(); ++i){
				lua_pushinteger(L, i + 1);
				MTLLuaToValue<T>::FromValue(L, datas.unsafe_ref(i));
				lua_settable(L, -3);
			}
		}

		template< class K, class T>
		static void WriteTable(lua_State* L, const TL::Map<K, T>& datas){
			if (!lua_istable(L, -1)){
				return;
			}
			for (auto& m : datas){
				MTLLuaToValue<K>::FromValue(L, m->first);
				MTLLuaToValue<T>::FromValue(L, m->second);
				lua_settable(L, -3);
			}
		}
	};

}
