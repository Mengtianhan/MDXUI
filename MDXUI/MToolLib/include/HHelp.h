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


//----------------------------------------------------
// 主要用于帮助解析字符串的问题
//-----------------------------------------------------
#pragma once
#include <string>
#include <iostream>
#include <sstream>
namespace mj{
	inline void printf(const char* s)
	{
		while (s && *s) {
			if (*s == '%' && *++s != '%')
				throw std::runtime_error("invalid format: missing arguments");
			std::cout << *s++;
		}
	}

	template<typename T, typename... Args>
	void printf(const char* s, T value, Args... args)
	{
		while (s && *s) {
			if (*s == '%' && *++s != '%') {
				std::cout << value;
				return printf(++s, args...);
			}
			std::cout << *s++;
		}
		throw std::runtime_error("extra arguments provided to printf");
	}

	
	class HParseString{
	public:
		HParseString(){}

		virtual ~HParseString(){}

		HParseString(const HParseString& ch){
			m_parse_result = ch.m_parse_result;
		}

		template<class T>
		HParseString& operator=(T ch){
			std::stringstream os;
			os << ch;
			m_parse_result = os.str();
			return *this;
		}

		template<class T>
		HParseString& operator+(T ch){
			std::stringstream os;
			os << ch;
			m_parse_result += os.str();
			return *this;
		}

		template<class T>
		HParseString& operator+=(T ch){
			std::stringstream os;
			os << ch;
			m_parse_result += os.str();
			return *this;
		}
		HParseString& operator=(const char* ch){
			m_parse_result = ch;
			return *this;
		}

		HParseString& operator=(const HParseString& ch){
			if (this == &ch){
				return *this;
			}
			m_parse_result = ch.m_parse_result;
			return *this;
		}

		HParseString& operator+(const char* ch){
			m_parse_result += ch;
			return *this;
		}

		HParseString& operator+=(const char* ch){
			m_parse_result += ch;
			return *this;
		}

		HParseString& operator+(const std::string& ch){
			m_parse_result += ch;
			return *this;
		}

		HParseString& operator+=(const std::string& ch){
			m_parse_result += ch;
			return *this;
		}

		void clear(){
			m_parse_result = "";
		}

		std::string getresult(){
			return m_parse_result;
		}

		std::string getresult() const{
			return m_parse_result;
		}

		template<typename T,typename... Args>
		void operator()(const char* s, T value, Args... args){
			while (s && *s) {
				if (*s == '%' && *++s != '%') {
					std::stringstream io_s;
					io_s << value;
					m_parse_result += io_s.str();;
					return operator()(++s, args...);
				}
				m_parse_result += *s++;
			}
			throw std::runtime_error("extra arguments provided to printf");
		}

        operator std::string(){
              return m_parse_result;
         }

		operator std::string() const{
			return m_parse_result;
		}

		friend std::ostream& operator<<(std::ostream& os, const HParseString& par){
			os << par.m_parse_result;
			return os;
		}

	private:
		void operator()(const char* s)
		{
			while (s && *s) {
				if (*s == '%' && *++s != '%')
					throw std::runtime_error("invalid format: missing arguments");
				m_parse_result += *s++;
			}
		}

		std::string		m_parse_result;
	};


	//---------------------------------------------
	// 需要格式化部分使用%号后面可以随便跟一个东西
	//---------------------------------------------
	class HFormatString{
	public:
		HFormatString(){}
		HFormatString(const std::string& format) :
			m_format_str(format)
		{
		}

		virtual ~HFormatString(){}

		HFormatString(const HFormatString& ch){
			m_format_str = ch.m_format_str;
		}

		template<class T>
		HFormatString& operator=(T ch){
			std::stringstream os;
			os << ch;
			m_format_str = os.str();
			return *this;
		}

		template<class T>
		HFormatString& operator+(T ch){
			std::stringstream os;
			os << ch;
			m_format_str += os.str();
			return *this;
		}

		template<class T>
		HFormatString& operator+=(T ch){
			std::stringstream os;
			os << ch;
			m_format_str += os.str();
			return *this;
		}

		HFormatString& operator=(const char* ch){
			m_format_str = ch;
			return *this;
		}

		HFormatString& operator=(const HFormatString& ch){
			if (this == &ch){
				return *this;
			}
			m_format_str = ch.m_format_str;
			return *this;
		}

		

		HFormatString& operator+(const char* ch){
			m_format_str += ch;
			return *this;
		}

		

		HFormatString& operator+=(const char* ch){
			m_format_str += ch;
			return *this;
		}

		HFormatString& operator+(const std::string& ch){
			m_format_str += ch;
			return *this;
		}

		HFormatString& operator+=(const std::string& ch){
			m_format_str += ch;
			return *this;
		}

		void clear(){
			m_format_str = "";
		}

		std::string getresult(){
			return m_format_str;
		}

		std::string getresult() const{
			return m_format_str;
		}
	
		operator std::string(){
			return m_format_str;
		}

		operator std::string() const{
			return m_format_str;
		}

		HFormatString& Format(const std::string& format){
			m_format_str = format;
			return *this;
		}

		//----------------------------------------
		// 如果不存在解析格式
		// 那么将数据当做解析格式来储存
		//----------------------------------------
		template<typename T>
		HFormatString& operator<<(T value){
			if (m_format_str.empty()){
				std::stringstream s;
				s << value;
				m_format_str = s.str();
				return *this;
			}

			int pos = m_format_str.find("%");
			if (pos != std::string::npos && pos < (m_format_str.size() - 1)){
				if (m_format_str.at(pos + 1) != '%'){
					std::string str(m_format_str.begin(), m_format_str.begin() + pos);
					std::stringstream s;
					s << value;
					str += s.str();
					str += std::string(m_format_str.begin() + pos + 2, m_format_str.end());
					m_format_str = str;
				}
			}
			return *this;
		}

		std::string& FormatStr(){
			return m_format_str;
		}

		friend std::ostream& operator<<(std::ostream& os, const HFormatString& format){
			os << format.m_format_str;
			return os;
		}
		
	private:
		std::string		m_format_str;
	};
}

// HParseString
inline std::string operator+(const std::string& fir, const mj::HParseString& sec){
	std::string __result;
	__result = fir + sec.getresult();
	return __result;
}

inline std::string operator+(const mj::HParseString& fir,const std::string& sec){
	std::string __result;
	__result = fir.getresult() + sec;
	return __result;
}

// HFormatString
inline std::string operator+(const std::string& fir, const mj::HFormatString& sec){
	std::string __result;
	__result = fir + sec.getresult();
	return __result;
}

inline std::string operator+(const mj::HFormatString& fir, const std::string& sec){
	std::string __result;
	__result = fir.getresult() + sec;
	return __result;
}

