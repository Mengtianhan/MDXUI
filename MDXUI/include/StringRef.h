

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

#ifndef _WIN64
#ifndef uint64
typedef unsigned __int32 uint64;
#endif
#else
#ifndef uint64
typedef unsigned __int64 uint64;
#endif
#endif

#ifndef uint16
#define uint16 unsigned __int16 
#endif
//
// 修改下面的预定于宏可修改预分配内存大小 预定义为8k
//
#ifndef MSTRINGMAXMEMERYSIZE
#define MSTRINGMAXMEMERYSIZE 8096
#endif


#ifdef __MAPI_DEBUG__
#define MSTRING_API 
#else
#if defined(DIRECTUI_EXPORTS)
#define MSTRING_API __declspec(dllexport)
#else
#define MSTRING_API __declspec(dllimport)
#endif
#endif 

namespace mj{

	template<typename T>
	class StringRef{
	public:
        StringRef(T* t = nullptr) :p_Ptr(t){
			init();
		}

		StringRef(const StringRef& p) :p_Ptr(p.p_Ptr){
			init();
		}

		StringRef& operator=(const StringRef& p){
			if (p_Ptr != p.p_Ptr){
				if (p_Ptr){
                    p_Ptr->RemRc();
				}
                p_Ptr = p.p_Ptr;
                init();
			}
			return *this;
		}

		~StringRef(){
			if (p_Ptr){
				p_Ptr->RemRc();
			}
		}

		T* operator->() const{
			return p_Ptr;
		}

		T& operator*() const{
			return *p_Ptr;
		}

		operator char*() const{
			return p_Ptr->c_str();
		}

	private:
		T* p_Ptr;

		void init(){
			if (!p_Ptr){
				return;
			}
			if (!(p_Ptr->IsShareable())){
				p_Ptr = new T(*p_Ptr);
			}
			p_Ptr->AddRc();
		}
	};


	///
	///  实现引用计数
	///

    ///
    /// \brief The StringRefrenceObj class
    ///
	class MSTRING_API StringRefrenceObj{
	public:
		void AddRc();
		void DecRc();
		void RemRc();
		void MarkUnshareable();
		bool IsShareable();
		bool IsShared();
        uint64 Count();
	private:
        uint64 mCount;
		bool  bIsShared;
	protected:
		StringRefrenceObj();
		StringRefrenceObj(const StringRefrenceObj&);
		StringRefrenceObj& operator=(const StringRefrenceObj&);
		virtual ~StringRefrenceObj();
	};
}





//+----------------------------------------------------
//
// 自动加载lib
//
//+----------------------------------------------------
#ifndef DIRECTUI_EXPORTS
#ifdef _WIN64

#ifdef _DEBUG
#pragma comment(lib,"MDXUId.lib")
#else
#pragma comment(lib,"MDXUI.lib")
#endif // _DEBUG

#else 

#ifdef _DEBUG
#pragma comment(lib,"MDXUId_x86.lib")
#else
#pragma comment(lib,"MDXUI_x86.lib")
#endif // _DEBUG 

#endif // _WIN64


#endif // DIRECTUI_EXPORTS