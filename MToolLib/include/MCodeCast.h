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

