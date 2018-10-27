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
#include <string>
#include <Windows.h>

class MCodeCast
{
public:
	MCodeCast(){}
	~MCodeCast(){}
	static inline std::string wtoc(const wchar_t* str){
		if (str == nullptr)
			return "";
		unsigned count = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, 0);
		std::string __result;
k:		try{
			char* m_char = new char[count];
			WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, str, -1,
				m_char, count, NULL, NULL);
			__result = m_char;
			delete[] m_char;
		}
		catch (std::bad_alloc e){
			goto k;
		}
		return __result;
	}

	static inline std::string wtoc(const std::wstring& src){
		if (&src == nullptr || src.empty())
			return "";
		unsigned count = WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, NULL, 0, NULL, 0);
		std::string __result;
k:		try{
			char* m_char = new char[count];
			WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, src.c_str(), -1,
				m_char, count, NULL, NULL);
			__result = m_char;
			delete[] m_char;
		}
		catch (std::bad_alloc e){
			goto k;
		}
		return __result;
	}

	static inline std::wstring stow(const char* src){
		if (src == nullptr)
			return L"";
		unsigned w_len = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
		std::wstring __result;
k:		try{
			wchar_t* m_w_char = new wchar_t[w_len + 1];
			ZeroMemory(m_w_char, w_len + 1);
			MultiByteToWideChar(CP_ACP, 0, src, -1, m_w_char, w_len);
			__result = m_w_char;
			delete[] m_w_char;
		}
		catch (std::bad_alloc e){
			goto k;
		}
		return __result;
	}

	static inline std::wstring stow(const std::string& src){
		if (&src == nullptr || src.empty())
			return L"";
		unsigned w_len = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);
		std::wstring __result;
  k:	try{
			wchar_t* m_w_char = new wchar_t[w_len + 1];
			ZeroMemory(m_w_char, w_len + 1);
			MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, m_w_char, w_len);
			__result = m_w_char;
			delete[] m_w_char;
		}
		catch (std::bad_alloc e){
			goto k;
		}
		return __result;
	}
};

