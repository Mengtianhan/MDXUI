///////////////////////////////////////////////////////////////////////////////
//
// MToolLib ---- Mengjin Tool Library
// Copyright (C) 2014-2018 by Mengjin (sh06155@hotmail.com)
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
#include "MString.h"
#include <fstream>
#include <lua.hpp>
#include <lualib.h>
#include <filesystem>
#include <memory>
#include <map>
#include <HPath_File.h>
namespace mj{
	template<bool>
	struct Lua_Fun_Help{
		template<class T>
		static void do_it(lua_State* mL, char c, T& value){
			MString str;
			switch (c)
			{
			case 'd':
			case 'D':
			case 'f':
			case 'F':
				str << value;
				lua_pushnumber(mL, str.ToOtherType<double>());
				break;

			case 'i':
			case 'I':
			case 'L':
			case 'l':
				str << value;
				lua_pushinteger(mL, str.ToOtherType<__int64>());
				break;

			case 'b':
			case 'B':
				str << value;
				lua_pushboolean(mL, str.ToOtherType<bool>());
				break;

			default:
				luaL_error(mL, "Invalid option %c", c);
				break;
			}

		}
	};

	template<>
	struct Lua_Fun_Help<true>{
		template<class T>
		static void do_it(lua_State* mL, char c, T& value){
			MString str;
			str << value;
			std::string  stdstr = str;
			lua_pushstring(mL, stdstr.c_str());
		}
	};


	class Lua_Fun{
	protected:
		Lua_Fun(){
			mL = luaL_newstate();
			luaL_openlibs(mL);
			lua_checkstack(mL, 200);
		}

		~Lua_Fun(){
			clear();
		}

	public:
		static void Destroy(Lua_Fun* fun){
			if (fun){
				fun->~Lua_Fun();
			}
		}

		//------------------------------------------------
		// 创建一个新的解释器
		//-------------------------------------------------
		static std::shared_ptr<Lua_Fun> NewInstance(){
			std::shared_ptr<Lua_Fun> data(new Lua_Fun, &Lua_Fun::Destroy);
			return data;
		}

		static std::shared_ptr<Lua_Fun> Instance(){
			static std::shared_ptr<Lua_Fun> data(new Lua_Fun, &Lua_Fun::Destroy);
			return data;
		}

		template<class F>
		void RegisterCFun(const std::string& key, F f){
			if (mL){
				lua_register(mL, key.c_str(), f);
			}
		}

		//---------------------------------------------------
		// 获取全局对象的值
		//---------------------------------------------------
		template<class T>
		T GetValue(const std::string& key, T defaultvalue = T()){
			lua_getglobal(mL, key.c_str());
			MString str;
			if (std::is_same<T, double>::value ||
				std::is_same<T, float>::value){
				str << lua_tonumber(mL, -1);
			}
			else if (std::is_same<T, int>::value ||
				std::is_same<T, size_t>::value ||
				std::is_same<T, short>::value ||
				std::is_same<T, unsigned short>::value ||
				std::is_same<T, __int64>::value ||
				std::is_same<T, unsigned __int64>::value ||
				std::is_same<T, long>::value ||
				std::is_same<T, unsigned long>::value){
				str << lua_tointeger(mL, -1);
			}
			else if (std::is_same<T, bool>::value){
				str << lua_toboolean(mL, -1);
			}
			else{
				str << lua_tostring(mL, -1);
			}
			try{
				defaultvalue = str.ToOtherType<T>();
			}
			catch (std::runtime_error e){
				;
			}
			lua_pop(mL, 1);
			return defaultvalue;
		}

		//--------------------------------------------------
		// 设置全局变量
		//--------------------------------------------------
		template<class T>
		void SetValue(const std::string& key, T value){
			lua_getglobal(mL, key.c_str());
			MString str(value);
			if (std::is_same<T, double>::value ||
				std::is_same<T, float>::value){
				lua_pushnumber(mL, str.ToOtherType<double>());
			}
			else if (std::is_same<T, int>::value ||
				std::is_same<T, size_t>::value ||
				std::is_same<T, short>::value ||
				std::is_same<T, unsigned short>::value ||
				std::is_same<T, __int64>::value ||
				std::is_same<T, unsigned __int64>::value ||
				std::is_same<T, long>::value ||
				std::is_same<T, unsigned long>::value){
				lua_pushinteger(mL, str.ToOtherType<__int64>());
			}
			else if (std::is_same<T, bool>::value){
				lua_pushboolean(mL, str.ToOtherType<bool>());
			}
			else{
				lua_pushstring(mL, str.c_str());
			}
			lua_setglobal(mL, key.c_str());
			lua_pop(mL, 2);
		}

		//
		// 通过一个函数来设置一个table
		//
		template<class T,class A,template<class T1,class A1> class C>
		void SetLuaTable(const std::string& funname, const C<T, A>& Input){                
			for (int i = 0; i < Input.size(); ++i){
                            SetFun(funname.c_str());
                            T val = Input[i];
                            RunLuaFun("d=",val);
			}

		}

		//----------------------------------------------------
		// 读取table指定索引的值
		//----------------------------------------------------
		template<class T>
		T ReadLuaTable(const std::string& table_name, int index, T defaultvalue)
		{
			if (mL == nullptr)
				return defaultvalue;

			lua_getglobal(mL, table_name.c_str());
			if (!lua_istable(mL, -1)){
				std::cerr << "isn't table .... " << std::endl;
				return defaultvalue;
			}
			int n = luaL_len(mL, -1);
			if (index <1 || index > n){
				throw std::runtime_error("out of range");
			}
			lua_rawgeti(mL, -1, index);

			MString str;
			if (std::is_same<T, double>::value ||
				std::is_same<T, float>::value){
				str << lua_tonumber(mL, -1);
			}
			else if (std::is_same<T, int>::value ||
				std::is_same<T, size_t>::value ||
				std::is_same<T, short>::value ||
				std::is_same<T, unsigned short>::value ||
				std::is_same<T, __int64>::value ||
				std::is_same<T, unsigned __int64>::value ||
				std::is_same<T, long>::value ||
				std::is_same<T, unsigned long>::value){
				str << lua_tointeger(mL, -1);
			}
			else if (std::is_same<T, bool>::value){
				str << lua_toboolean(mL, -1);
			}
			else{
				str << lua_tostring(mL, -1);
			}

			try{
				defaultvalue = str.ToOtherType<T>();
			}
			catch (std::runtime_error e){
				;
			}
			lua_pop(mL, 1);
			return defaultvalue;
		}

		//---------------------------------------------------
		// 设置指定表的键值
		//---------------------------------------------------
		template<class K, class V>
		bool  SetLuaTable(const std::string& table, K key, V value){
			if (mL == nullptr){
				std::cerr << "Lua isn't exist ... " << std::endl;
				return false;
			}

			lua_getglobal(mL, table.c_str());
			if (!lua_istable(mL, -1)){
				std::cerr << table << " isn't table .... " << std::endl;
				return false;
			}

			MString strKey(key);
			if (std::is_same<K, int>::value ||
				std::is_same<K, size_t>::value ||
				std::is_same<K, short>::value ||
				std::is_same<K, unsigned short>::value ||
				std::is_same<K, __int64>::value ||
				std::is_same<K, unsigned __int64>::value ||
				std::is_same<K, long>::value ||
				std::is_same<K, unsigned long>::value)
			{
				lua_pushinteger(mL, strKey.ToOtherType<int>());
			}
			else{
				lua_pushstring(mL, strKey.c_str());
			}
			//-----------------------------------------------
			// 设置值
			//-----------------------------------------------
			MString strValue(value);
			if (std::is_same<V, double>::value ||
				std::is_same<V, float>::value){
				lua_pushnumber(mL, strValue.ToOtherType<double>());
			}
			else if (std::is_same<V, int>::value ||
				std::is_same<V, size_t>::value ||
				std::is_same<V, short>::value ||
				std::is_same<V, unsigned short>::value ||
				std::is_same<V, __int64>::value ||
				std::is_same<V, unsigned __int64>::value ||
				std::is_same<V, long>::value ||
				std::is_same<V, unsigned long>::value){
				lua_pushinteger(mL, strValue.ToOtherType<__int64>());
			}
			else if (std::is_same<V, bool>::value){
				lua_pushboolean(mL, strValue.ToOtherType<bool>());
			}
			else{
				lua_pushstring(mL, strValue.c_str());
			}
			lua_settable(mL, -3);
			lua_pop(mL, 1);
			return true;
		}


		//
		// 很多复杂的函数需要预先设置函数
		//
		void SetFun(const char* fun){
			clear();
			init(fun);
		}

		//
		// 设置需要执行的成员函数
		//
		bool ReadyTableFun(const std::string& table_name, const char* fun){
			if (mL == nullptr)
				return false;
			lua_getglobal(mL, table_name.c_str());
			if (!lua_istable(mL, -1)){
				return false;
			}
			lua_getfield(mL, -1, fun);
			if (!lua_isfunction(mL, -1)){
				return false;
			}
			mFun = fun;
			mOnce = 0;
			mNumArgs = 0;
			mNumResults = 0;
			mResult.clear();
			return true;
		}

		//
		// 执行一个参数的lua函数
		//
		bool RunTableFun(const std::string& table_name, const std::string& funName, const std::string& Arg1){
			if (mL == nullptr)
				return false;
			lua_getglobal(mL, table_name.c_str());
			if (!lua_istable(mL, -1)){
				lua_pop(mL, -1);
				return false;
			}
			lua_getfield(mL, -1, funName.c_str());
			if (!lua_isfunction(mL, -1)){
				lua_pop(mL, -1);
				return false;
			}
			lua_pushstring(mL,Arg1.c_str());
			if (lua_pcall(mL, 1, 0, 0)){
				mj::HParseString par;
				par("Error run function %1 : %2\nLine : %3\nFunction : %4", mFun, lua_tostring(mL, -1),
					__LINE__, __FUNCTION__);
				lua_pop(mL, -1);
				return false;
			}
			lua_pop(mL, -1);
			return true;
		}

		//
		// 执行2个参数的lua函数
		//
		bool RunTableFun(const std::string& table_name, const std::string& funName, const std::string& Arg1, const std::string& Arg2){
			if (mL == nullptr)
				return false;
			lua_getglobal(mL, table_name.c_str());
			if (!lua_istable(mL, -1)){
				lua_pop(mL, -1);
				return false;
			}
			lua_getfield(mL, -1, funName.c_str());
			if (!lua_isfunction(mL, -1)){
				lua_pop(mL, -1);
				return false;
			}
			lua_pushstring(mL, Arg1.c_str());
			lua_pushstring(mL, Arg2.c_str());
			if (lua_pcall(mL, 2, 0, 0)){
				mj::HParseString par;
				par("Error run function %1 : %2\nLine : %3\nFunction : %4", mFun, lua_tostring(mL, -1),
					__LINE__, __FUNCTION__);
				lua_pop(mL, -1);
				return false;
			}
			lua_pop(mL, -1);
			return true;
		}

		template<class F>
		bool RegisterTableCFun(const std::string& table_name,const std::string& key, F f){
			if (mL == nullptr){
				return false;
			}
			lua_getglobal(mL, table_name.c_str());
			if (!lua_istable(mL, -1)){
				return false;
			}
			lua_pushstring(mL, key.c_str());
			lua_pushcfunction(mL, f);
			lua_settable(mL, -3);
			return true;
		}

		//
		// 设置lua文件
		//
		void SetLuaFile(const std::string& filename){
			if (!file::fileisexist(filename))
				return;
			if (mCurrentLuaFile == filename){
				return;
			}

			std::string strFilename(filename);
			mj::MString mjStr(filename);
			if (mjStr.istart_with(".")){
				std::tr2::sys::path p(strFilename);
				strFilename = std::tr2::sys::complete(p);
			}

			int re = luaL_dofile(mL, strFilename.c_str());// luaL_loadfile(mL, strFilename.c_str());

			if (LUA_OK != re){
				mj::HParseString par;
				par("can't run config file and error code is %1 ", lua_tostring(mL, -1));
				box::Error_MessageBox(par);
				lua_pop(mL, -1);
				return;
				luaL_error(mL, "can't run config file and error code is %s ", lua_tostring(mL, -1));
			}
			else{
				lua_resume(mL, 0, 0);
			}
			mCurrentLuaFile = filename;
		}

		//
		// 读取一个表的所有内容
		//
		template<class T>
		void Read_Lua_Table(const std::string& table_name, std::vector<T>& outPut){
			outPut.clear();
			if (!mL)
				return;
			lua_getglobal(mL, table_name.c_str());
			if (!lua_istable(mL, -1)){
				return;
			}
			GetFunTable(outPut);
			lua_pop(mL, -1); // lua_pop(mL,1);
		}

		//
		// 读取一个表的指定key的值
		//
		MString Read_Lua_Table(const std::string& table_name, const std::string& key)
		{
			if (mL == nullptr)
				return "lua is not valiable.....";
			MString str;
			lua_getglobal(mL, table_name.c_str());
			if (!lua_istable(mL, -1)){
				std::cerr << "isn't table .... " << std::endl;
				return str;
			}
			lua_getfield(mL, -1, key.c_str());
                        if(lua_isnil(mL,-1))
                            return str;
			str << lua_tostring(mL, -1);
			lua_pop(mL, -1); // lua_pop(mL,1);
			return str;
		}


		//
		// 这属于比较复杂的情况了，所以在执行该函数之前，先执行SetFun
		// format 为需要执行的函数的参数格式
		// args 为参数列表，以#号分割
		// outPut 为返回结果
		//
		template<class U, class A, template<class U1, class A1>class C>
		void Read_Lua_Table(const char* format, const mj::MString& args, C<U, A>& outPut){
			outPut.clear();
			if (mFun.empty() || mL == nullptr)
				return;
			std::deque<mj::MString> __Args;
			const_cast<mj::MString&>(args).split("#", __Args);
			if (args.empty()){
				//
				// 执行函数
				//
				if (lua_pcall(mL, mNumArgs, 1, 0)){
					mj::HParseString par;
					par("Error run function %1 : %2\nLine : %3\nFunction : %4", mFun, lua_tostring(mL, -1),
						__LINE__, __FUNCTION__);
					box::Error_MessageBox(par);
					lua_pop(mL, -1);
					return;
					luaL_error(mL, "Error run function %s : %s", mFun, lua_tostring(mL, -1));
				}
				else{
					GetFunTable(outPut);
				}
				lua_pop(mL, -1);//
				return;
			}

			if (*format){

				switch (*format++)
				{
				case 's':  // 参数为字符串
				case 'S':
					Lua_Fun_Help<true>::do_it(mL, *(format - 1), __Args.front());
					break;

				default:
					Lua_Fun_Help<false>::do_it(mL, *(format - 1), __Args.front());
					break;
				}
				mNumArgs++;
				__Args.pop_front();
				luaL_checkstack(mL, 1, "Too many argments....");
				mj::MString str;
				for (auto& s : __Args)
					str << s << "#";
				Read_Lua_Table(format, str, outPut);
			}
			else{
				Read_Lua_Table(format, "", outPut);
			}
		}

		//
		// 读取一个二进制文件
		//
		template<class T, class A, template<class T1, class A1>class C>
		void RunLuaFun(C<T, A>& outPut, const std::string& filename, size_t lock = sizeof(T)){
			outPut.clear();
			if (mFun.empty() || mL == nullptr)
				return;
			if (block == 0)
				block = sizeof(T);
			lua_pushstring(mL, filename.c_str());
			lua_pushinteger(mL, block);
			if (lua_pcall(mL, 2, 1, 0)){
				mj::HParseString par;
				par("Error run function %1 : %2\nLine : %3\nFunction : %4", mFun, lua_tostring(mL, -1),
					__LINE__, __FUNCTION__);
				box::Error_MessageBox(par);
				lua_pop(mL, -1);
				return;
				luaL_error(mL, "Error run function %s : %s", mFun, lua_tostring(mL, -1));
			}
			else{
				if (!lua_istable(mL, -1)){
					return;
				}
				int n = luaL_len(mL, -1);
				for (int i = 1; i <= n; ++i){
					lua_rawgeti(mL, -1, i);
					double f = lua_tonumber(mL, -1);
					MString str(f);
					outPut.push_back(str.ToOtherType<T>());
					lua_pop(mL, 1);
				}
			}
			lua_pop(mL, -1);
		}



		//
		// 执行一个无参的lua函数
		//
		void RunLuaFun(){
			if (mFun.empty() || mL == nullptr)
				return;
			if (lua_pcall(mL, 0, 0, 0)){
				mj::HParseString par;
				par("Error run function %1 : %2\nLine : %3\nFunction : %4", mFun, lua_tostring(mL, -1),
					__LINE__, __FUNCTION__);
				box::Error_MessageBox(par);
				lua_pop(mL, -1);
				return;
				luaL_error(mL, "Error run function %s : %s", mFun, lua_tostring(mL, -1));
			}
			lua_pop(mL, -1);
		}

		//
		// 执行一个lua函数
		// Input 参数数组 Input_N数据大小
		// outPut 返回结果
		//
		template<class T>
		void RunLuaFun(T* Input, int Input_N, std::vector<T>& outPut){
			if (mFun.empty() || mL == nullptr)
				return;
			outPut.clear();
			if (Input == nullptr){
				if (lua_pcall(mL, 0, 1, 0)){
					mj::HParseString par;
					par("Error run function %1 : %2\nLine : %3\nFunction : %4", mFun, lua_tostring(mL, -1),
						__LINE__, __FUNCTION__);
					box::Error_MessageBox(par);
					lua_pop(mL, -1);
					return;
					luaL_error(mL, "Error run function %s : %s", mFun, lua_tostring(mL, -1));
				}
				else{
					GetFunTable(outPut);
				}
			}
			else{
				lua_newtable(mL);
				for (int i = 0; i < Input_N; ++i){
					AddToTable(i + 1, Input[i]);
				}
				if (lua_pcall(mL, 1, 1, 0)){
					mj::HParseString par;
					par("Error run function %1 : %2\nLine : %3\nFunction : %4", mFun, lua_tostring(mL, -1),
						__LINE__, __FUNCTION__);
					box::Error_MessageBox(par);
					lua_pop(mL, -1);
					return;
					luaL_error(mL, "Error run function %s : %s", mFun, lua_tostring(mL, -1));
				}
				else{
					GetFunTable(outPut);
				}
			}
			lua_pop(mL, -1);
		}

		//
		// 功能同上
		//
		template<class T, class R, class A, class A2, template<class T1, class A1> class C1, template<class T2, class A3> class C2>
		void RunLuaFun(C1<T, A>& Input, C2<R, A2>& outPut){
			if (mFun.empty() || mL == nullptr)
				return;

			outPut.clear();
			if (Input.empty()){
				if (lua_pcall(mL, 0, 1, 0)){
					mj::HParseString par;
					par("Error run function %1 : %2\nLine : %3\nFunction : %4", mFun, lua_tostring(mL, -1),
						__LINE__, __FUNCTION__);
					box::Error_MessageBox(par);
					lua_pop(mL, -1);
					return;
					luaL_error(mL, "Error run function %s : %s", mFun, lua_tostring(mL, -1));
				}
				else{
					GetFunTable(outPut);
				}
			}

			else{
				lua_newtable(mL);
				for (int i = 0; i < Input.size(); ++i){
					AddToTable(i + 1, Input[i]);
				}
				if (lua_pcall(mL, 1, 1, 0)){
					mj::HParseString par;
					par("Error run function %1 : %2\nLine : %3\nFunction : %4", mFun, lua_tostring(mL, -1),
						__LINE__, __FUNCTION__);
					box::Error_MessageBox(par);
					lua_pop(mL, -1);
					return;
					luaL_error(mL, "Error run function %s : %s", mFun, lua_tostring(mL, -1));
				}
				else{
					GetFunTable(outPut);
				}
			}
			lua_pop(mL, -1);
		}

		//
		// fun lua函数名
		// outPut 返回结果
		// 执行一个无参函数返回一个系列
		//
		template<class T, class A, template<class T1, class A1>class C>
		void RunLuaFun(const std::string& fun, C<T, A>& outPut){
			if (mL == nullptr || fun.empty())
				return;

			outPut.clear();
			lua_getglobal(mL, fun.c_str());
			if (lua_pcall(mL, 0, 1, 0)){
				mj::HParseString par;
				par("Error run function %1 : %2\nLine : %3\nFunction : %4", fun, lua_tostring(mL, -1),
					__LINE__, __FUNCTION__);
				box::Error_MessageBox(par);
				lua_pop(mL, -1);
				return;
				luaL_error(mL, "Error run function %s : %s", fun, lua_tostring(mL, -1));
			}
			else{
				GetFunTable(outPut);
			}
			lua_pop(mL, -1); // lua_pop(mL,1);
		}

		//
		// 输入两组数
		// 返回一组数
		//
		template<class T, class U>
		void RunLuaFun(T* Input1, int Input_N1, U* Input2, int Input_N2, std::vector<T>& outPut){
			if (mFun.empty() || mL == nullptr)
				return;

			outPut.clear();
			if (Input1 == nullptr && Input2 == nullptr){
				if (lua_pcall(mL, 0, 1, 0)){
					mj::HParseString par;
					par("Error run function %1 : %2\nLine : %3\nFunction : %4", mFun, lua_tostring(mL, -1),
						__LINE__, __FUNCTION__);
					box::Error_MessageBox(par);
					lua_pop(mL, -1);
					return;
					luaL_error(mL, "Error run function %s : %s", mFun, lua_tostring(mL, -1));
				}
				else{
					GetFunTable(outPut);
				}
			}
			else if (Input1 == nullptr){
				lua_run_fun(Input2, Input_N2, outPut);
			}
			else if (Input2 == nullptr){
				lua_run_fun(Input1, Input_N1, outPut);
			}

			else{
				lua_newtable(mL);
				for (int i = 0; i < Input_N1; ++i){
					AddToTable(i + 1, Input1[i]);
				}
				lua_newtable(mL);
				for (int i = 0; i < Input_N2; ++i){
					AddToTable(i + 1, Input2[i]);
				}
				if (lua_pcall(mL, 2, 1, 0)){
					mj::HParseString par;
					par("Error run function %1 : %2\nLine : %3\nFunction : %4", mFun, lua_tostring(mL, -1),
						__LINE__, __FUNCTION__);
					box::Error_MessageBox(par);
					lua_pop(mL, -1);
					return;
					luaL_error(mL, "Error run function %s : %s", mFun, lua_tostring(mL, -1));
				}
				else{
					GetFunTable(outPut);
				}
			}
			lua_pop(mL, -1); // lua_pop(mL,1);
		}

		//
		// 功能同上
		//
		template<class T, class U, class R, class A, template<class T1, class A1> class C1,
			template<class T2, class A2> class C2, template<class T3, class A3> class C3>
		void RunLuaFun(C1<T, A>& Input1, C2<U, A>& Input2, C3<R, A>& outPut){
			if (mFun.empty() || mL == nullptr)
				return;

			outPut.clear();
			if (Input1.empty() && Input2.empty()){
				if (lua_pcall(mL, 0, 1, 0)){
					mj::HParseString par;
					par("Error run function %1 : %2\nLine : %3\nFunction : %4", mFun, lua_tostring(mL, -1),
						__LINE__, __FUNCTION__);
					box::Error_MessageBox(par);
					lua_pop(mL, -1);
					return;
					luaL_error(mL, "Error run function %s : %s", mFun, lua_tostring(mL, -1));
				}
				else{
					GetFunTable(outPut);
				}
			}
			else if (Input1.empty()){
				RunLuaFun(Input2, outPut);
			}
			else if (Input2.empty()){
				RunLuaFun(Input1, outPut);
			}


			else{
				lua_newtable(mL);
				for (int i = 0; i < Input1.size(); ++i){
					AddToTable(i + 1, Input1[i]);
				}
				lua_newtable(mL);
				for (int i = 0; i < Input2.size(); ++i){
					AddToTable(i + 1, Input2[i]);
				}
				if (lua_pcall(mL, 2, 1, 0)){
					mj::HParseString par;
					par("Error run function %1 : %2\nLine : %3\nFunction : %4", mFun, lua_tostring(mL, -1),
						__LINE__, __FUNCTION__);
					box::Error_MessageBox(par);
					lua_pop(mL, -1);
					return;
					luaL_error(mL, "Error run function %s : %s", mFun, lua_tostring(mL, -1));
				}
				else{
					GetFunTable(outPut);
				}
			}
			lua_pop(mL, -1); // lua_pop(mL,1);
		}

		//
		// 输入3组数返回一组数
		//
		template<class T, class U, class V, class R, class A, template<class T1, class A1> class C1,
			template<class T2, class A2> class C2, template<class T3, class A3> class C3, template<class T4, class A4>class C4>
		void RunLuaFun(C1<T, A>& Input1, C2<U, A>& Input2, C3<V, A>& Input3, C4<R, A>& outPut){

			if (mFun.empty() || mL == nullptr)
				return;

			outPut.clear();
			if (Input1.empty() && Input2.empty() && Input3.empty()){
				if (lua_pcall(mL, 0, 1, 0)){
					mj::HParseString par;
					par("Error run function %1 : %2\nLine : %3\nFunction : %4", mFun, lua_tostring(mL, -1),
						__LINE__, __FUNCTION__);
					box::Error_MessageBox(par);
					lua_pop(mL, -1);
					return;
					luaL_error(mL, "Error run function %s : %s", mFun, lua_tostring(mL, -1));
				}
				else{
					GetFunTable(outPut);
				}
			}
			else if (Input1.empty() && Input3.empty()){
				RunLuaFun(Input2, outPut);
			}
			else if (Input2.empty() && Input3.empty()){
				RunLuaFun(Input1, outPut);
			}
			else if (Input2.empty() && Input1.empty())
			{
				RunLuaFun(Input3, outPut);
			}
			else if (Input1.empty()){
				RunLuaFun(Input2, Input3, outPut);
			}
			else if (Input2.empty()){
				RunLuaFun(Input1, Input3, outPut);
			}
			else if (Input3.empty()){
				RunLuaFun(Input1, Input2, outPut);
			}

			else{
				lua_newtable(mL);
				for (int i = 0; i < Input1.size(); ++i){
					AddToTable(i + 1, Input1[i]);
				}
				lua_newtable(mL);
				for (int i = 0; i < Input2.size(); ++i){
					AddToTable(i + 1, Input2[i]);
				}
				lua_newtable(mL);
				for (int i = 0; i < Input3.size(); ++i){
					AddToTable(i + 1, Input3[i]);
				}
				if (lua_pcall(mL, 3, 1, 0)){
					mj::HParseString par;
					par("Error run function %1 : %2\nLine : %3\nFunction : %4", mFun, lua_tostring(mL, -1),
						__LINE__, __FUNCTION__);
					box::Error_MessageBox(par);
					lua_pop(mL, -1);
					return;
					luaL_error(mL, "Error run function %s : %s", mFun, lua_tostring(mL, -1));
				}
				else{
					GetFunTable(outPut);
				}
			}
			lua_pop(mL, -1); // lua_pop(mL,1);
		}



		//
		// 执行任意lua函数
		// 但是目前版本不能返回表
		//
		template<class T, class...Args>
		void RunLuaFun(const char* sig, T& value, Args&...args){
			if (mL == nullptr || mFun.empty()){
				return;
			}
			size_t N = 0;
			bool is_true = false;
			if (*sig){

				switch (*sig++)
				{
				case 's':  //
				case 'S':
					Lua_Fun_Help<true>::do_it(mL, *(sig - 1), value);
					break;

				case '>':
				case '=':
					goto  endwhile;
				default:
					Lua_Fun_Help<false>::do_it(mL, *(sig - 1), value);
					break;
				}
				mNumArgs++;
				luaL_checkstack(mL, 1, "Too many argments....");
				RunLuaFun(sig, args...);
			}
		endwhile:


			if (mOnce != 0){
				lua_pop(mL, -1); // lua_pop(mL,1);
				return;
			}

			mOnce++;
			mNumResults = strlen(sig);

			if (lua_pcall(mL, mNumArgs, mNumResults, 0)){
				mj::HParseString par;
				par("Error run function %1 : %2\nLine : %3\nFunction : %4", mFun, lua_tostring(mL, -1),
					__LINE__, __FUNCTION__);
				box::Error_MessageBox(par);
				lua_pop(mL, -1);
				return;
			}
			if (mNumResults == 0){
				lua_pop(mL, -1);
				return;
			}

			mNumResults = -mNumResults;
			result_help(sig, value, args...);
		}

		void RunLuaFun(const char* sig){}


		//
		// 这个函数是将结果放在mResult 然后通过该函数获取结果
		//
		std::vector<MString>& GetResult(){
			return mResult;
		}

	private:

		//
		// 获取结果的辅助函数
		//
		template<class T, class...Args>
                void result_help(const char* sig, T& value, Args&...args){
                        MString str;
			if (*sig){
				switch (*sig++)
				{
				case 'd':
				case 'D':
				case 'f':
				case 'F':
                                        value = lua_tonumber(mL, mNumResults);
                                        str << lua_tonumber(mL, mNumResults);
					mResult.push_back(str);
					break;

				case 'i':
				case 'I':
				case 'L':
				case 'l':
                                        value = lua_tointeger(mL, mNumResults);
                                        str << lua_tointeger(mL, mNumResults);
					mResult.push_back(str);
					break;

				case 'b':
				case 'B':
                                        value = lua_toboolean(mL, mNumResults);
                                        str << lua_toboolean(mL, mNumResults);
					mResult.push_back(str);
					break;

				case 's':
				case 'S':
                                        str << lua_tostring(mL, mNumResults);
					mResult.push_back(str);
                                        value = str.ToOtherType<T>();
					break;
				default:
					break;
				}

				if (mNumResults == -1)
					return;
				++mNumResults;
			}

			result_help(sig, args...);
		}

		void result_help(const char* c){}


		void clear(){
			//			if (mL){
			//				lua_close(mL);
			//			}
		}

		//
		// 初始化函数
		//
		void init(const std::string& fun){
			mFun = fun;
			if (fun.empty())
				return;
			lua_getglobal(mL, mFun.c_str());
			mOnce = 0;
			mNumArgs = 0;
			mNumResults = 0;
			mResult.clear();
		}

		//
		// 辅助函数将数据添加进表中
		//
		template<class Key, class Value>
		void AddToTable(Key k, Value val){
			if (mL){
				if (lua_istable(mL, -1)){
					MString str;
					str << k;
					if (std::is_same<Key, double>::value ||
						std::is_same<Key, float>::value){
						lua_pushnumber(mL, str.ToOtherType<double>());
					}
					else if (std::is_same<Key, int>::value ||
						std::is_same<Key, size_t>::value ||
						std::is_same<Key, short>::value ||
						std::is_same<Key, unsigned short>::value ||
						std::is_same<Key, __int64>::value ||
						std::is_same<Key, unsigned __int64>::value ||
						std::is_same<Key, long>::value ||
						std::is_same<Key, unsigned long>::value){
						lua_pushinteger(mL, str.ToOtherType<Key>());
					}
					else if (std::is_same<Key, bool>::value){
						lua_pushboolean(mL, str.ToOtherType<Key>());
					}
					else{
						std::string stdstr = str;
						lua_pushstring(mL, stdstr.c_str());
					}

					str.clear();
					str << val;
					if (std::is_same<Value, double>::value ||
						std::is_same<Value, float>::value){
						lua_pushnumber(mL, str.ToOtherType<Value>());
					}
					else if (std::is_same<Value, int>::value ||
						std::is_same<Value, size_t>::value ||
						std::is_same<Value, short>::value ||
						std::is_same<Value, unsigned short>::value ||
						std::is_same<Value, __int64>::value ||
						std::is_same<Value, unsigned __int64>::value ||
						std::is_same<Value, long>::value ||
						std::is_same<Value, unsigned long>::value){
						lua_pushinteger(mL, str.ToOtherType<Value>());
					}
					else if (std::is_same<Value, bool>::value){
						lua_pushinteger(mL, str.ToOtherType<Value>());
					}
					else{
						std::string stdstr = str;
						lua_pushstring(mL, stdstr.c_str());
					}
					lua_settable(mL, -3);
				}
			}
		}

		//
		// 辅助函数，获取表
		//
		template<class T, class A, template<class T1, class A1>class C>
		void GetFunTable(C<T, A>& outPut){
			if (!lua_istable(mL, -1)){
				return;
			}
			int n = luaL_len(mL, -1);
			for (int i = 1; i <= n; ++i){
				MString str;
				lua_rawgeti(mL, -1, i);
				if (std::is_same<T, double>::value ||
					std::is_same<T, float>::value){
					str << lua_tonumber(mL, -1);
					outPut.push_back(str.ToOtherType<T>());
				}
				else if (std::is_same<T, int>::value ||
					std::is_same<T, size_t>::value ||
					std::is_same<T, short>::value ||
					std::is_same<T, unsigned short>::value ||
					std::is_same<T, __int64>::value ||
					std::is_same<T, unsigned __int64>::value ||
					std::is_same<T, long>::value ||
					std::is_same<T, unsigned long>::value){
					str << lua_tointeger(mL, -1);
					outPut.push_back(str.ToOtherType<T>());
				}
				else if (std::is_same<T, bool>::value){
					str << lua_toboolean(mL, -1);
					outPut.push_back(str.ToOtherType<T>());
				}
				else{
					str << lua_tostring(mL, -1);
					outPut.push_back(str.ToOtherType<T>());
				}
				lua_pop(mL, 1);
			}
		}

		int mNumArgs{ 0 };     // 输入参数个数
		int mNumResults{ 0 };  // 返回结果个数
		std::string mFun;
		std::string mCurrentLuaFile; // 当前加载的lua文件
		int   mOnce{ 0 };
		std::vector<MString> mResult; // 结果系列
		lua_State* mL{ nullptr };

	};

}
