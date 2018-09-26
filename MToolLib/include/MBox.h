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
#include <Windows.h>
#include "MString.h"
using namespace mj;
namespace msgbox{
	static int Error_MessageBox(const std::string& message, 
		const std::string& titile = "Error", 
		HWND hwnd = nullptr)
	{
		return ::MessageBoxExA(hwnd, message.c_str(), titile.c_str(), MB_ICONERROR|MB_OK, \
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}

	static int Info_MessageBox(const std::string& message, 
		const std::string& titile = "Info", 
		HWND hwnd = nullptr)
	{
                return ::MessageBoxExA(hwnd, message.c_str(), titile.c_str(), MB_ICONINFORMATION|MB_YESNO , \
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}

        static int Info_MessageBox2(const std::string& message,
                const std::string& titile = "Info",
                HWND hwnd = nullptr)
        {
                return ::MessageBoxExA(hwnd, message.c_str(), titile.c_str(), MB_ICONINFORMATION|MB_OK , \
                        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
        }

	static int Que_MessageBox(const std::string& message, 
		const std::string& titile = "Info", 
		HWND hwnd = nullptr)
	{
                return ::MessageBoxExA(hwnd, message.c_str(), titile.c_str(),MB_ICONQUESTION|MB_YESNOCANCEL, \
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}
}

namespace box{
//
// 只要是Qt的QWidget以及派生类都能够使用
//
template<class T>
struct MsgBox{
        static int ErrorBox(const MString& info, T* parent,const MString& title){
                if(parent == nullptr)
                    return msgbox::Error_MessageBox(info, title, nullptr);
                else
                    return msgbox::Error_MessageBox(info, title, (HWND)parent->winId());
	}

        static int ShowBox(const MString& info, T* parent,const MString& title){
                if(parent == nullptr)
                    return msgbox::Info_MessageBox2(info, title, nullptr);
                else
                    return msgbox::Info_MessageBox2(info, title, (HWND)parent->winId());
        }

        static int InfoBox(const MString& info, T* parent,const MString& title){
                if(parent == nullptr)
                    return msgbox::Info_MessageBox(info, title, nullptr);
                else
                    return msgbox::Info_MessageBox(info, title, (HWND)parent->winId());
	}

        static int QueBox(const MString& info, T* parent,const MString& title){
                if(parent == nullptr)
                    return msgbox::Que_MessageBox(info, title, nullptr);
                else
                    return msgbox::Que_MessageBox(info, title, (HWND)parent->winId());
	}
};

template<>
struct MsgBox<HWND__>{
        static int ErrorBox(const MString& info, HWND__* parent,const MString& title){
		return msgbox::Error_MessageBox(info,title,parent);
	}

        static int ShowBox(const MString& info, HWND__* parent,const MString& title){
                return msgbox::Info_MessageBox2(info, title, parent);
        }

        static int InfoBox(const MString& info, HWND__* parent, const MString& title){
		return msgbox::Info_MessageBox(info,title,parent);
	}

        static int QueBox(const MString& info, HWND__* parent, const MString& title){
		return msgbox::Que_MessageBox(info, title,parent);
	}
};

//
// 提示信息对话框，仅有一个确认按钮
//
template<class T = HWND__>
static int ErrorBox(const MString& info, T* parent = nullptr,const MString& title = "Error"){
	return MsgBox<T>::ErrorBox(info, parent, title);
}

//
// 提示信息对话框，仅有一个确认按钮
//
template<class T = HWND__>
static int ShowBox(const MString& info, T* parent = nullptr,const MString& title = "Error"){
        return MsgBox<T>::ShowBox(info, parent, title);
}

//
// 这个对话框有两个按钮，YES 和 NO 返回值为IDYES 和 IDNO
//
template<class T = HWND__>
static int InfoBox(const MString& info, T* parent = nullptr, const MString& title = "Info"){
	return MsgBox<T>::InfoBox(info, parent, title);
}

//
// 这个对话框有三个按钮，YES NO 和 CANCEL 返回值为IDYES IDNO 和 IDCANCEL
//
template<class T = HWND__>
static int QueBox(const MString& info, T* parent = nullptr, const MString& title = "Que"){
	return MsgBox<T>::QueBox(info, parent, title);
}
}
