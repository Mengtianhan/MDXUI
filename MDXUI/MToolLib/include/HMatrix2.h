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


//***************************************************************************
// HMatrix
//***************************************************************************

#pragma once
#include <HTuple.h>
#include <vector>
#include <regex>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <fstream>
#include <MMap.h>

//===========================================================================
//==================================HMatrix===================================
//===========================================================================
namespace mj{

	template<typename T>
	inline std::ostream& operator<<(std::ostream& os, const std::vector<T>& c){
		if (c.empty()){
			return os;
		}

		std::copy(c.begin(), --c.end(), std::ostream_iterator<T>(os, " "));
		os << *(--c.end());
		return os;
	}

	//==================================================
	//========HMatrix===================================
	struct null{};

	template<typename T, typename A, template<typename T1, typename A1> class C>
	inline std::ostream& operator<<(std::ostream& os, const std::vector<T, A>& c){
		if (c.empty()){
			return os;
		}

		std::copy(c.begin(), --c.end(), std::ostream_iterator<T>(os, " "));
		os << *(--c.end());
		return os;
	}

	template<class T0 = null, class T1 = null, class T2 = null, class T3 = null, class T4 = null, class T5 = null,
	class T6 = null, class T7 = null, class T8 = null, class T9 = null>
	class HMatrix
	{

		typedef std::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> tuple_data;

	public:
		typedef typename std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

		HMatrix(){ m_data.clear(); }
		~HMatrix(){}


		HMatrix(const HMatrix& other){
			m_data = other.m_data;
		}

		//=======================================
		// 使用数据来构造
		//========================================
		HMatrix(std::vector<value_type>& __data){
			m_data = __data;
		}


		HMatrix& operator=(std::vector<value_type>& __data){
			m_data = __data;
			return *this;
		}

		HMatrix& operator=(const HMatrix& other){
			if (this == &other){
				return *this;
			}
			m_data = other.m_data;
			return *this;
		}

		std::vector<tuple_data>& getData(){
			return m_data;
		}

		const std::vector<tuple_data>& getData() const {
			return m_data;
		}

		void push_back(T0 value0, T1 value1, T2 value2, T3 value3, T4 value4,
			T5 value5, T6 value6, T7 value7, T8 value8, T9 value9){
			m_data.push_back(std::make_tuple(value0, value1, value2, value3, value4, value5, value6, value7, value8, value9));
		}

		void push_back(HMatrix& other){
			std::copy(other.begin(), other.end(), std::back_inserter(m_data));
		}

		void push_back(value_type value){
			m_data.push_back(value);
		}

		static value_type make_value(T0 value0, T1 value1, T2 value2, T3 value3, T4 value4,
			T5 value5, T6 value6, T7 value7, T8 value8, T9 value9){
			return std::make_tuple(value0, value1, value2, value3, value4, value5, value6, value7, value8, value9);
		}

		template<typename T, int N>
		T get_element(value_type v){
			if (N >= 10){
				throw std::runtime_error("index out of arange");
			}
			return std::get<N>(v);
		}

		//===================================
		// 获取满足条件的第一个元素
		//===================================
		template<typename T, int N>
		boost::optional<value_type> get_element(T& value){
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					return boost::optional<value_type>(it);
				}
			}
			return boost::optional<value_type>();
		}

		//============================================
		// 按照指定元素指定方法进行排序
		//============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(){
			std::sort(m_data.begin(), m_data.end(), [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//=============================================
		// 对一个区间进行排序
		//=============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(iterator first, iterator last){
			std::sort(first, last, [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//==============================================
		// 获取指定项的所有数据
		//===============================================
		template<class T, int N>
		std::vector<value_type> get_subElements(const T& value){
			std::vector<value_type> __result;
			for (auto it : m_data){
                                T t = get_element<T, N>(it);
				if (t == value){
					__result.push_back(it);
				}
			}
			return __result;
		}

		template<class T, int N>
		int find(T& value){
			int i = 0;
			for (auto it : m_data){
				++i;
				T t = get_element<T, N>(it);
				if (t == value){
					return i;
				}
			}
			return -1;
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		unsigned colum() {
			if (m_data.empty.empty())
				return 0;
			return 10;
		}

		unsigned row(){
			return m_data.size();
		}

		template<class T, int N>
		T at(unsigned row){
			if (row >= m_data.size())
				throw std::runtime_error("row index beyond vector range!!!");
			return std::get<N>(m_data.at(row));
		}

		template<class T, int N>
		std::vector<T> GetColumData(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
		}

		value_type GetRowData(unsigned row){
			if (m_data.empty())
				return value_type();
			return m_data.at(row);
		}



		void reserve(unsigned size){
			m_data.reserve(size);
		}

		void resize(unsigned size){
			m_data.resize(size);
		}

		void erase(iterator it){
			m_data.erase(it);
		}

		value_type back(){
			if (m_data.empty())
				return value_type();
			return *(--m_data.end());
		}

		void pop_back(){
			if (m_data.empty())
				return;
			m_data.erase(--m_data.end());
		}

		value_type front(){
			if (m_data.empty())
				return value_type();
			return *m_data.begin();
		}

		void pop_front(){
			if (m_data.empty())
				return;
			m_data.erase(m_data.begin());
		}

		iterator begin(){
			return m_data.begin();
		}

		iterator end(){
			return m_data.end();
		}


		void fillHMatrixFromFile(const std::string& filename, const std::string& startline = "(\\[.{1,2000}\\])"){
			std::ifstream inFile(filename);
			if (inFile.fail()){
				throw std::runtime_error("open file for read fail!!!");
			}

			bool b_is_empty = startline.empty();
			std::string read_str;
			std::regex  reg(startline);
			m_data.clear();
			std::vector<std::string> temp_v;
			while (std::getline(inFile, read_str)){
				if (read_str.empty())
					continue;
				if (b_is_empty){
					temp_v.clear();
					boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
					if (temp_v.size() < 10)
						continue;
					try{
						push_back(boost::lexical_cast<T0>(temp_v.at(0)), boost::lexical_cast<T1>(temp_v.at(1)),
							boost::lexical_cast<T2>(temp_v.at(2)), boost::lexical_cast<T3>(temp_v.at(3)), boost::lexical_cast<T4>(temp_v.at(4)),
							boost::lexical_cast<T5>(temp_v.at(5)), boost::lexical_cast<T6>(temp_v.at(6)), boost::lexical_cast<T7>(temp_v.at(7)),
							boost::lexical_cast<T8>(temp_v.at(8)), boost::lexical_cast<T9>(temp_v.at(9)));
					}
					catch (...){
						continue;
					}
				}
				else if (std::regex_match(read_str, reg)){
					while (std::getline(inFile, read_str)){
						if (read_str.empty())
							continue;
						temp_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						if (temp_v.size() < 10)
							continue;
						try{
							push_back(boost::lexical_cast<T0>(temp_v.at(0)), boost::lexical_cast<T1>(temp_v.at(1)),
								boost::lexical_cast<T2>(temp_v.at(2)), boost::lexical_cast<T3>(temp_v.at(3)), boost::lexical_cast<T4>(temp_v.at(4)),
								boost::lexical_cast<T5>(temp_v.at(5)), boost::lexical_cast<T6>(temp_v.at(6)), boost::lexical_cast<T7>(temp_v.at(7)),
								boost::lexical_cast<T8>(temp_v.at(8)), boost::lexical_cast<T9>(temp_v.at(9)));
						}
						catch (...){
							continue;
						}
					}
				}
			}
			inFile.close();
		}

		friend std::ostream& operator<<(std::ostream& os, HMatrix<T0, T1, T2, T4, T5, T6, T7, T8, T9>& m){
			if (m.getData().empty())
				return os;
			os << "[row:" << m.m_data.size() << "\tcolum:" << 10 << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](tuple_data data){
				os.setf(std::ios_base::left, std::ios_base::adjustfield);
				os << std::get<0>(data);
				os << "\t";
				os << std::get<1>(data);
				os << "\t";
				os << std::get<2>(data);
				os << "\t";
				os << std::get<3>(data);
				os << "\t";
				os << std::get<4>(data);
				os << "\t";
				os << std::get<5>(data);
				os << "\t";
				os << std::get<6>(data);
				os << "\t";
				os << std::get<7>(data);
				os << "\t";
				os << std::get<8>(data);
				os << "\t";
				os << std::get<9>(data) << std::endl;

			});
			return os;
		}

	private:
		std::vector<tuple_data>  m_data;
	};

	template<class T>
	class HMatrix<T>{
	public:
		typedef typename std::vector<T>::iterator iterator;
		typedef T value_type;

		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		HMatrix(const HMatrix& other){
			m_data = other.m_data;
		}

		//=======================================
		// 使用数据来构造
		//========================================
		HMatrix(std::vector<value_type>& __data){
			m_data = __data;
		}

		HMatrix& operator=(std::vector<value_type>& __data){
			m_data = __data;
			return *this;
		}

		HMatrix& operator=(const HMatrix& other){
			if (this == &other){
				return *this;
			}
			m_data = other.m_data;
			return *this;
		}

		std::vector<T>& getData(){
			return m_data;
		}

		const std::vector<T>& getData() const {
			return m_data;
		}

		void push_back(T value){
			m_data.push_back(value);
		}

		void push_back(HMatrix& other){
			std::copy(other.begin(), other.end(), std::back_inserter(m_data));
		}

		T get_element(value_type v){
			return v;
		}

		//===================================
		// 获取满足条件的第一个元素
		//===================================
		template<typename T, int N>
		boost::optional<value_type> get_element(T& value){
			for (auto it : m_data){
				if (it == value){
					return boost::optional<value_type>(it);
				}
			}
			return boost::optional<value_type>();
		}

		//============================================
		// 按照指定元素指定方法进行排序
		//============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(){
			std::sort(m_data.begin(), m_data.end(), [&](value_type v1, value_type v2){
				return CMP()(v1, v2);
			});
		}

		//=============================================
		// 对一个区间进行排序
		//=============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(iterator first, iterator last){
			std::sort(first, last, [&](value_type v1, value_type v2){
				return CMP()(v1, v2);
			});
		}

		//==============================================
		// 获取指定项的所有数据
		//===============================================
		template<class T, int N>
		std::vector<value_type> get_elements(const T& value){
			std::vector<value_type> __result;
			for (auto it : m_data){
				if (it == value){
					__result.push_back(it);
				}
			}
			return __result;
		}

		template<class T, int N>
		int find(T& value){
			int i = 0;
			for (auto it : m_data){
				++i;
				if (t == it){
					return i;
				}
			}
			return -1;
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		unsigned colum() {
			if (m_data.empty())
				return 0;
			return 1;
		}

		unsigned row(){
			return m_data.size();
		}

		template<class T>
		std::vector<T> GetColumData(){
			return m_data;
		}

		void reserve(unsigned size){
			m_data.reserve(size);
		}

		void resize(unsigned size){
			m_data.resize(size);
		}

		void erase(iterator it){
			m_data.erase(it);
		}

		value_type back(){
			if (m_data.empty())
				return value_type();
			return *(--m_data.end());
		}

		void pop_back(){
			if (m_data.empty())
				return;
			m_data.erase(--m_data.end());
		}

		value_type front(){
			if (m_data.empty())
				return value_type();
			return *m_data.begin();
		}

		void pop_front(){
			if (m_data.empty())
				return;
			m_data.erase(m_data.begin());
		}

		iterator begin(){
			return m_data.begin();
		}

		iterator end(){
			return m_data.end();
		}

		T at(unsigned row){
			if (row >= m_data.size())
				throw std::runtime_error("row index beyond vector range!!!");
			return m_data.at(row);
		}

		value_type GetRowData(unsigned row){
			if (m_data.empty())
				return value_type();
			return m_data.at(row);
		}

		void fillHMatrixFromFile(const std::string& filename, const std::string& startline = "(\\[.{1,2000}\\])"){
			std::ifstream inFile(filename);
			if (inFile.fail()){
				throw std::runtime_error("open file for read fail!!!");
			}
			bool b_is_empty = startline.empty();
			std::string read_str;
			std::regex  reg(startline);
			std::vector<std::string> temp_v;
			m_data.clear();
			while (std::getline(inFile, read_str)){
				if (read_str.empty())
					continue;
				if (b_is_empty){
					boost::trim(read_str.at(0));
					try{
						push_back(boost::lexical_cast<T>(read_str.at(0)));
					}
					catch (...){
						continue;
					}
				}
				else if (std::regex_match(read_str, reg)){

					while (std::getline(inFile, read_str)){
						if (read_str.empty())
							continue;
						boost::trim(read_str.at(0));
						try{
							push_back(boost::lexical_cast<T>(read_str.at(0)));
						}
						catch (...){
							continue;
						}
					}
				}
			}
			inFile.close();
		}

		friend std::ostream& operator<<(std::ostream& os, HMatrix<T>& m){
			if (m.getData().empty())
				return os;
			os << "[row:" << m.m_data.size() << "\tcolum:" << 1 << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](T data){
				os << data << std::endl;

			});
			return os;
		}
	private:
		std::vector<T>  m_data;
	};

	template<class T0, class T1>
	class HMatrix<T0, T1>{
		typedef std::tuple<T0, T1> tuple_data;
	public:

		typedef typename std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		HMatrix(const HMatrix& other){
			m_data = other.m_data;
		}

		//=======================================
		// 使用数据来构造
		//========================================
		HMatrix(std::vector<value_type>& __data){
			m_data = __data;
		}

		HMatrix& operator=(std::vector<value_type>& __data){
			m_data = __data;
			return *this;
		}

		HMatrix& operator=(const HMatrix& other){
			if (this == &other){
				return *this;
			}
			m_data = other.m_data;
			return *this;
		}

		std::vector<tuple_data>& getData(){
			return m_data;
		}

		const std::vector<tuple_data>& getData() const {
			return m_data;
		}

		void push_back(T0 value0, T1 value1){
			m_data.push_back(std::make_tuple(value0, value1));
		}

		void push_back(value_type value){
			m_data.push_back(value);
		}

		void push_back(HMatrix& other){
			std::copy(other.begin(), other.end(), std::back_inserter(m_data));
		}

		static value_type make_value(T0 value0, T1 value1){
			return std::make_tuple(value0, value1);
		}

		template<typename T, int N>
		T get_element(value_type v){
			if (N >= 2){
				throw std::runtime_error("index out of arange");
			}
			return std::get<N>(v);
		}

		//===================================
		// 获取满足条件的第一个元素
		//===================================
		template<typename T, int N>
		boost::optional<value_type> get_element(T& value){
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					return boost::optional<value_type>(it);
				}
			}
			return boost::optional<value_type>();
		}

		//============================================
		// 按照指定元素指定方法进行排序
		//============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(){
			std::sort(m_data.begin(), m_data.end(), [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//=============================================
		// 对一个区间进行排序
		//=============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(iterator first, iterator last){
			std::sort(first, last, [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//==============================================
		// 获取指定项的所有数据
		//===============================================
		template<class T, int N>
		std::vector<value_type> get_subElements(const T& value){
			std::vector<value_type> __result;
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					__result.push_back(it);
				}
			}
			return __result;
		}
		template<class T, int N>
		int find(T& value){
			int i = 0;
			for (auto it : m_data){
				++i;
				T t = get_element<T, N>(it);
				if (t == value){
					return i;
				}
			}
			return -1;
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		unsigned colum() {
			if (m_data.empty())
				return 0;
			return 2;
		}

		unsigned row(){
			return m_data.size();
		}

		void reserve(unsigned size){
			m_data.reserve(size);
		}

		void resize(unsigned size){
			m_data.resize(size);
		}

		void erase(iterator it){
			m_data.erase(it);
		}

		value_type back(){
			if (m_data.empty())
				return value_type();
			return *(--m_data.end());
		}

		void pop_back(){
			if (m_data.empty())
				return;
			m_data.erase(--m_data.end());
		}

		value_type front(){
			if (m_data.empty())
				return value_type();
			return *m_data.begin();
		}

		void pop_front(){
			if (m_data.empty())
				return;
			m_data.erase(m_data.begin());
		}

		iterator begin(){
			return m_data.begin();
		}

		iterator end(){
			return m_data.end();
		}

		template<class T, int N>
		T at(unsigned row){
			if (row >= m_data.size())
				throw std::runtime_error("row index beyond vector range!!!");
			return std::get<N>(m_data.at(row));
		}

		template<class T, int N>
		std::vector<T> GetColumData(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
		}

		value_type GetRowData(unsigned row){
			if (m_data.empty())
				return value_type();
			return m_data.at(row);
		}

		void fillHMatrixFromFile(const std::string& filename, const std::string& startline = "(\\[.{1,2000}\\])"){
			std::ifstream inFile(filename);
			if (inFile.fail()){
				throw std::runtime_error("open file for read fail!!!");
			}
			bool b_is_empty = startline.empty();
			std::string read_str;
			std::regex  reg(startline);
			m_data.clear();
			std::vector<std::string> temp_v;
			while (std::getline(inFile, read_str)){
				if (read_str.empty())
					continue;
				if (b_is_empty){
					temp_v.clear();
					boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
					if (temp_v.size() < 2)
						continue;
					try{
						push_back(boost::lexical_cast<T0>(temp_v.at(0)), boost::lexical_cast<T1>(temp_v.at(1)));
					}
					catch (...){
						continue;
					}
				}
				else if (std::regex_match(read_str, reg)){
					while (std::getline(inFile, read_str)){
						if (read_str.empty())
							continue;
						temp_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						if (temp_v.size() < 2)
							continue;
						try{
							push_back(boost::lexical_cast<T0>(temp_v.at(0)), boost::lexical_cast<T1>(temp_v.at(1)));
						}
						catch (...){
							continue;
						}
					}
				}
			}
			inFile.close();
		}

		friend std::ostream& operator<<(std::ostream& os, HMatrix<T0, T1>& m){
			if (m.getData().empty())
				return os;
			os << "[row:" << m.m_data.size() << "\tcolum:" << 2 << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](tuple_data data){
				os.setf(std::ios_base::left, std::ios_base::adjustfield);
				os << std::get<0>(data);
				os << "\t";
				os << std::get<1>(data) << std::endl;

			});
			return os;
		}

	private:
		std::vector<tuple_data>  m_data;
	};

	template<class T0, class T1, class T2>
	class HMatrix<T0, T1, T2>{
		typedef std::tuple<T0, T1, T2> tuple_data;
	public:
		typedef typename std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		HMatrix(const HMatrix& other){
			m_data = other.m_data;
		}

		//=======================================
		// 使用数据来构造
		//========================================
		HMatrix(std::vector<value_type>& __data){
			m_data = __data;
		}

		HMatrix& operator=(std::vector<value_type>& __data){
			m_data = __data;
			return *this;
		}

		HMatrix& operator=(const HMatrix& other){
			if (this == &other){
				return *this;
			}
			m_data = other.m_data;
			return *this;
		}

		std::vector<tuple_data>& getData(){
			return m_data;
		}

		const std::vector<tuple_data>& getData() const {
			return m_data;
		}

		void push_back(T0 value0, T1 value1, T2 value2){
			m_data.push_back(std::make_tuple(value0, value1, value2));
		}

		void push_back(value_type value){
			m_data.push_back(value);
		}

		void push_back(HMatrix& other){
			std::copy(other.begin(), other.end(), std::back_inserter(m_data));
		}

		static value_type make_value(T0 value0, T1 value1, T2 value2){
			return std::make_tuple(value0, value1, value2);
		}

		template<typename T, int N>
		T get_element(value_type v){
			if (N >= 3){
				throw std::runtime_error("index out of arange");
			}
			return std::get<N>(v);
		}

		//===================================
		// 获取满足条件的第一个元素
		//===================================
		template<typename T, int N>
		boost::optional<value_type> get_element(T& value){
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					return boost::optional<value_type>(it);
				}
			}
			return boost::optional<value_type>();
		}

		//============================================
		// 按照指定元素指定方法进行排序
		//============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(){
			std::sort(m_data.begin(), m_data.end(), [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//=============================================
		// 对一个区间进行排序
		//=============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(iterator first, iterator last){
			std::sort(first, last, [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//==============================================
		// 获取指定项的所有数据
		//===============================================
		template<class T, int N>
		std::vector<value_type> get_subElements(const T& value){
			std::vector<value_type> __result;
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					__result.push_back(it);
				}
			}
			return __result;
		}

		template<class T, int N>
		int find(T& value){
			int i = 0;
			for (auto it : m_data){
				++i;
				T t = get_element<T, N>(it);
				if (t == value){
					return i;
				}
			}
			return -1;
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		unsigned colum() {
			if (m_data.empty())
				return 0;
			return 3;
		}

		unsigned row(){
			return m_data.size();
		}

		void reserve(unsigned size){
			m_data.reserve(size);
		}

		void resize(unsigned size){
			m_data.resize(size);
		}

		void erase(iterator it){
			m_data.erase(it);
		}

		value_type back(){
			if (m_data.empty())
				return value_type();
			return *(--m_data.end());
		}

		void pop_back(){
			if (m_data.empty())
				return;
			m_data.erase(--m_data.end());
		}

		value_type front(){
			if (m_data.empty())
				return value_type();
			return *m_data.begin();
		}

		void pop_front(){
			if (m_data.empty())
				return;
			m_data.erase(m_data.begin());
		}

		iterator begin(){
			return m_data.begin();
		}

		iterator end(){
			return m_data.end();
		}

		template<class T, int N>
		T at(unsigned row){
			if (row >= m_data.size())
				throw std::runtime_error("row index beyond vector range!!!");
			return std::get<N>(m_data.at(row));
		}

		template<class T, int N>
		std::vector<T> GetColumData(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
		}

		value_type GetRowData(unsigned row){
			if (m_data.empty())
				return value_type();
			return m_data.at(row);
		}

		void fillHMatrixFromFile(const std::string& filename, const std::string& startline = "(\\[.{1,2000}\\])"){
			std::ifstream inFile(filename);
			if (inFile.fail()){
				throw std::runtime_error("open file for read fail!!!");
			}
			bool b_is_empty = startline.empty();
			std::string read_str;
			std::regex  reg(startline);
			m_data.clear();
			std::vector<std::string> temp_v;
			while (std::getline(inFile, read_str)){
				if (read_str.empty())
					continue;
				if (b_is_empty){
					temp_v.clear();
					boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
					if (temp_v.size() < 3)
						continue;
					try{
						push_back(boost::lexical_cast<T0>(temp_v.at(0)), boost::lexical_cast<T1>(temp_v.at(1)),
							boost::lexical_cast<T2>(temp_v.at(2)));
					}
					catch (...){
						continue;
					}
				}
				else if (std::regex_match(read_str, reg)){
					while (std::getline(inFile, read_str)){
						if (read_str.empty())
							continue;
						temp_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						if (temp_v.size() < 3)
							continue;
						try{
							push_back(boost::lexical_cast<T0>(temp_v.at(0)), boost::lexical_cast<T1>(temp_v.at(1)),
								boost::lexical_cast<T2>(temp_v.at(2)));
						}
						catch (...){
							continue;
						}
					}
				}
			}
			inFile.close();
		}

		friend std::ostream& operator<<(std::ostream& os, HMatrix<T0, T1, T2>& m){
			if (m.getData().empty())
				return os;
			os << "[row:" << m.m_data.size() << "\tcolum:" << 3 << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](tuple_data data){
				os.setf(std::ios_base::left, std::ios_base::adjustfield);
				os << std::get<0>(data);
				os << "\t";
				os << std::get<1>(data);
				os << "\t";
				os << std::get<2>(data) << std::endl;
			});
			return os;
		}

	private:
		std::vector<tuple_data>  m_data;
	};

	template<class T0, class T1, class T2, class T3>
	class HMatrix<T0, T1, T2, T3>{
		typedef std::tuple<T0, T1, T2, T3> tuple_data;
	public:
		typedef typename std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

		HMatrix(){ m_data.clear(); }
		~HMatrix(){ m_data.clear(); }

		HMatrix(const HMatrix& other){
			m_data = other.m_data;
		}

		//=======================================
		// 使用数据来构造
		//========================================
		HMatrix(std::vector<value_type>& __data){
			m_data = __data;
		}

		HMatrix& operator=(std::vector<value_type>& __data){
			m_data = __data;
			return *this;
		}

		HMatrix& operator=(const HMatrix& other){
			if (this == &other){
				return *this;
			}
			m_data = other.m_data;
			return *this;
		}

		std::vector<tuple_data>& getData(){
			return m_data;
		}

		const std::vector<tuple_data>& getData() const {
			return m_data;
		}

		void push_back(T0 value0, T1 value1, T2 value2, T3 value3){
			m_data.push_back(std::make_tuple(value0, value1, value2, value3));
		}

		void push_back(value_type value){
			m_data.push_back(value);
		}

		void push_back(HMatrix& other){
			std::copy(other.begin(), other.end(), std::back_inserter(m_data));
		}

		static value_type make_value(T0 value0, T1 value1, T2 value2, T3 value3){
			return std::make_tuple(value0, value1, value2, value3);
		}

		template<typename T, int N>
		T get_element(value_type v){
			if (N >= 4){
				throw std::runtime_error("index out of arange");
			}
			return std::get<N>(v);
		}

		//===================================
		// 获取满足条件的第一个元素
		//===================================
		template<typename T, int N>
		boost::optional<value_type> get_element(T& value){
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					return boost::optional<value_type>(it);
				}
			}
			return boost::optional<value_type>();
		}

		//============================================
		// 按照指定元素指定方法进行排序
		//============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(){
			std::sort(m_data.begin(), m_data.end(), [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//=============================================
		// 对一个区间进行排序
		//=============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(iterator first, iterator last){
			std::sort(first, last, [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//===============================================
		// 获取指定项的所有数据
		//===============================================
		template<class T, int N>
		std::vector<value_type> get_subElements(const T& value){
			std::vector<value_type> __result;
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					__result.push_back(it);
				}
			}
			return __result;
		}

		template<class T, int N>
		int find(T& value){
			int i = 0;
			for (auto it : m_data){
				++i;
				T t = get_element<T, N>(it);
				if (t == value){
					return i;
				}
			}
			return -1;
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		unsigned colum() {
			if (m_data.empty())
				return 0;
			return 4;
		}

		unsigned row(){
			return m_data.size();
		}

		void reserve(unsigned size){
			m_data.reserve(size);
		}

		void resize(unsigned size){
			m_data.resize(size);
		}

		void erase(iterator it){
			m_data.erase(it);
		}

		value_type back(){
			if (m_data.empty())
				return value_type();
			return *(--m_data.end());
		}

		void pop_back(){
			if (m_data.empty())
				return;
			m_data.erase(--m_data.end());
		}

		value_type front(){
			if (m_data.empty())
				return value_type();
			return *m_data.begin();
		}

		void pop_front(){
			if (m_data.empty())
				return;
			m_data.erase(m_data.begin());
		}

		iterator begin(){
			return m_data.begin();
		}

		iterator end(){
			return m_data.end();
		}

		template<class T, int N>
		T at(unsigned row){
			if (row >= m_data.size())
				throw std::runtime_error("row index beyond vector range!!!");
			return std::get<N>(m_data.at(row));
		}

		template<class T, int N>
		std::vector<T> GetColumData(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
		}

		value_type GetRowData(unsigned row){
			if (m_data.empty())
				return value_type();
			return m_data.at(row);
		}

		void fillHMatrixFromFile(const std::string& filename, const std::string& startline = "(\\[.{1,2000}\\])"){
			std::ifstream inFile(filename);
			if (inFile.fail()){
				throw std::runtime_error("open file for read fail!!!");
			}
			bool b_is_empty = startline.empty();
			std::string read_str;
			std::regex  reg(startline);
			m_data.clear();
			std::vector<std::string> temp_v;
			while (std::getline(inFile, read_str)){
				if (read_str.empty())
					continue;
				if (b_is_empty){
					temp_v.clear();
					boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
					if (temp_v.size() < 4)
						continue;
					try{
						push_back(boost::lexical_cast<T0>(temp_v.at(0)), boost::lexical_cast<T1>(temp_v.at(1)),
							boost::lexical_cast<T2>(temp_v.at(2)), boost::lexical_cast<T3>(temp_v.at(3)));
					}
					catch (...){
						continue;
					}
				}
				else if (std::regex_match(read_str, reg)){
					while (std::getline(inFile, read_str)){
						if (read_str.empty())
							continue;
						temp_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						if (temp_v.size() < 4)
							continue;
						try{
							push_back(boost::lexical_cast<T0>(temp_v.at(0)), boost::lexical_cast<T1>(temp_v.at(1)),
								boost::lexical_cast<T2>(temp_v.at(2)), boost::lexical_cast<T3>(temp_v.at(3)));
						}
						catch (...){
							continue;
						}
					}
				}
			}
			inFile.close();
		}

		friend std::ostream& operator<<(std::ostream& os, HMatrix<T0, T1, T2, T3>& m){
			if (m.getData().empty())
				return os;
			os << "[row:" << m.m_data.size() << "\tcolum:" << 4 << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](tuple_data data){
				os.setf(std::ios_base::left, std::ios_base::adjustfield);
				os << std::get<0>(data);
				os << "\t";
				os << std::get<1>(data);
				os << "\t";
				os << std::get<2>(data);
				os << "\t";
				os << std::get<3>(data) << std::endl;
			});
			return os;
		}

	private:
		std::vector<tuple_data>  m_data;
	};

	//4参数double的特化版本
	template<>
	class HMatrix<double, double, std::vector<double>, std::vector<double>>{
		typedef std::tuple<double, double, std::vector<double>, std::vector<double>> tuple_data;
	public:

		typedef std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

		HMatrix(){ m_data.clear(); }
		~HMatrix(){ m_data.clear(); }

		HMatrix(const HMatrix& other){
			m_data = other.m_data;
		}

		//=======================================
		// 使用数据来构造
		//========================================
		HMatrix(std::vector<value_type>& __data){
			m_data = __data;
		}

		HMatrix& operator=(std::vector<value_type>& __data){
			m_data = __data;
			return *this;
		}

		HMatrix& operator=(const HMatrix& other){
			if (this == &other){
				return *this;
			}
			m_data = other.m_data;
			return *this;
		}

		std::vector<tuple_data>& getData(){
			return m_data;
		}

		const std::vector<tuple_data>& getData() const {
			return m_data;
		}

		void push_back(double value0, double value1, std::vector<double>& value2, std::vector<double>& value3){
			m_data.push_back(std::make_tuple(value0, value1, value2, value3));
		}

		void push_back(value_type value){
			m_data.push_back(value);
		}

		void push_back(HMatrix& other){
			std::copy(other.begin(), other.end(), std::back_inserter(m_data));
		}

		static value_type make_value(double value0, double value1, std::vector<double>& value2, std::vector<double>& value3){
			return std::make_tuple(value0, value1, value2, value3);
		}

		template<typename T, int N>
		T get_element(value_type v){
			if (N >= 4){
				throw std::runtime_error("index out of arange");
			}
			return std::get<N>(v);
		}

		//===================================
		// 获取满足条件的第一个元素
		//===================================
		template<typename T, int N>
		boost::optional<value_type> get_element(T& value){
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					return boost::optional<value_type>(it);
				}
			}
			return boost::optional<value_type>();
		}

		//============================================
		// 按照指定元素指定方法进行排序
		//============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(){
			std::sort(m_data.begin(), m_data.end(), [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//=============================================
		// 对一个区间进行排序
		//=============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(iterator first, iterator last){
			std::sort(first, last, [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//==============================================
		// 获取指定项的所有数据
		//===============================================
		template<class T, int N>
		std::vector<value_type> get_subElements(const T& value){
			std::vector<value_type> __result;
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					__result.push_back(it);
				}
			}
			return __result;
		}

		template<class T, int N>
		int find(T& value){
			int i = 0;
			for (auto it : m_data){
				++i;
				T t = get_element<T, N>(it);
				if (t == value){
					return i;
				}
			}
			return -1;
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		unsigned colum() {
			if (m_data.empty())
				return 0;
			return 4;
		}

		unsigned row(){
			return m_data.size();
		}

		void reserve(unsigned size){
			m_data.reserve(size);
		}

		void resize(unsigned size){
			m_data.resize(size);
		}

		void erase(iterator it){
			m_data.erase(it);
		}

		value_type back(){
			if (m_data.empty())
				return value_type();
			return *(--m_data.end());
		}

		void pop_back(){
			if (m_data.empty())
				return;
			m_data.erase(--m_data.end());
		}

		value_type front(){
			if (m_data.empty())
				return value_type();
			return *m_data.begin();
		}

		void pop_front(){
			if (m_data.empty())
				return;
			m_data.erase(m_data.begin());
		}

		iterator begin(){
			return m_data.begin();
		}

		iterator end(){
			return m_data.end();
		}

		template<class T, int N>
		T at(unsigned row){
			if (row >= m_data.size())
				throw std::runtime_error("row index beyond vector range!!!");
			return std::get<N>(m_data.at(row));
		}

		template<class T, int N>
		std::vector<T> GetColumData(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
		}

		value_type GetRowData(unsigned row){
			if (m_data.empty())
				return value_type();
			return m_data.at(row);
		}

		void fillHMatrixFromFile(const std::string& filename, const std::string& startline = "(\\[.{1,2000}\\])"){
			std::ifstream inFile(filename);
			if (inFile.fail()){
				throw std::runtime_error("open file for read fail!!!");
			}
			bool b_is_empty = startline.empty();
			std::string read_str;
			std::regex  reg(startline);
			m_data.clear();
			std::vector<std::string> temp_v;
			while (std::getline(inFile, read_str)){
				if (read_str.empty())
					continue;
				if (b_is_empty){
					temp_v.clear();
					boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
					if (temp_v.size() < 4)
						continue;
					try{
						std::vector<std::string> v1, v2;
						boost::split(v1, temp_v.at(2), boost::is_any_of(","));
						boost::split(v2, temp_v.at(3), boost::is_any_of(","));
						v1.erase(std::remove(v1.begin(), v1.end(), ""), v1.end());
						v2.erase(std::remove(v2.begin(), v2.end(), ""), v2.end());
						std::vector<double> re_1, re_2;
						std::for_each(v1.begin(), v1.end(), [&](std::string str){
							re_1.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v2.begin(), v2.end(), [&](std::string str){
							re_2.push_back(boost::lexical_cast<double>(str));
						});
						push_back(boost::lexical_cast<double>(temp_v.at(0)), boost::lexical_cast<double>(temp_v.at(1)),
							re_1, re_2);
					}
					catch (...){
						//这里主要捕获转换失败异常，如果转换失败，不用管，继续读取下面的内容
						continue;
					}
				}
				else if (std::regex_match(read_str, reg)){
					while (std::getline(inFile, read_str)){
						if (read_str.empty())
							continue;
						temp_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						if (temp_v.size() < 4)
							continue;
						try{
							std::vector<std::string> v1, v2;
							boost::split(v1, temp_v.at(2), boost::is_any_of(","));
							boost::split(v2, temp_v.at(3), boost::is_any_of(","));
							v1.erase(std::remove(v1.begin(), v1.end(), ""), v1.end());
							v2.erase(std::remove(v2.begin(), v2.end(), ""), v2.end());
							std::vector<double> re_1, re_2;
							std::for_each(v1.begin(), v1.end(), [&](std::string str){
								re_1.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v2.begin(), v2.end(), [&](std::string str){
								re_2.push_back(boost::lexical_cast<double>(str));
							});
							push_back(boost::lexical_cast<double>(temp_v.at(0)), boost::lexical_cast<double>(temp_v.at(1)),
								re_1, re_2);
						}
						catch (...){
							//这里主要捕获转换失败异常，如果转换失败，不用管，继续读取下面的内容
							continue;
						}
					}
				}
			}
			inFile.close();
		}

		friend std::ostream& operator<<(std::ostream& os, HMatrix& m){
			if (m.getData().empty())
				return os;
			os << "[row:" << m.m_data.size() << "\tcolum:" << 4 << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](tuple_data data){
				os.setf(std::ios_base::left, std::ios_base::adjustfield);
				os.width(10);
				os << std::get<0>(data);
				os.width(10);
				os << "";
				os << std::get<1>(data);
				os.width(10);
				os << "";
				os << std::get<2>(data);
				os.width(10);
				os << "";
				os << std::get<3>(data) << std::endl;
			});
			return os;
		}

	private:
		std::vector<tuple_data>  m_data;
	};

	//------------------------------------------------------------------------------------
	//4参数int的特化版本
	//------------------------------------------------------------------------------------
	template<>
	class HMatrix<int, int, std::vector<double>, std::vector<double>>{
		typedef std::tuple<int, int, std::vector<double>, std::vector<double>> tuple_data;
	public:

		typedef std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

		HMatrix(){ m_data.clear(); }
		~HMatrix(){ m_data.clear(); }

		HMatrix(const HMatrix& other){
			m_data = other.m_data;
		}

		//=======================================
		// 使用数据来构造
		//========================================
		HMatrix(std::vector<value_type>& __data){
			m_data = __data;
		}

		HMatrix& operator=(std::vector<value_type>& __data){
			m_data = __data;
			return *this;
		}

		HMatrix& operator=(const HMatrix& other){
			if (this == &other){
				return *this;
			}
			m_data = other.m_data;
			return *this;
		}

		std::vector<tuple_data>& getData(){
			return m_data;
		}

		const std::vector<tuple_data>& getData() const {
			return m_data;
		}

		void push_back(int value0, int value1, std::vector<double>& value2, std::vector<double>& value3){
			m_data.push_back(std::make_tuple(value0, value1, value2, value3));
		}

		void push_back(value_type value){
			m_data.push_back(value);
		}

		void push_back(HMatrix& other){
			std::copy(other.begin(), other.end(), std::back_inserter(m_data));
		}

		static value_type make_value(int value0, int value1, std::vector<double>& value2, std::vector<double>& value3){
			return std::make_tuple(value0, value1, value2, value3);
		}

		template<typename T, int N>
		T get_element(value_type v){
			if (N >= 4){
				throw std::runtime_error("index out of arange");
			}
			return std::get<N>(v);
		}

		//===================================
		// 获取满足条件的第一个元素
		//===================================
		template<typename T, int N>
		boost::optional<value_type> get_element(T& value){
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					return boost::optional<value_type>(it);
				}
			}
			return boost::optional<value_type>();
		}

		//============================================
		// 按照指定元素指定方法进行排序
		//============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(){
			std::sort(m_data.begin(), m_data.end(), [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//=============================================
		// 对一个区间进行排序
		//=============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(iterator first, iterator last){
			std::sort(first, last, [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//==============================================
		// 获取指定项的所有数据
		//===============================================
		template<class T, int N>
		std::vector<value_type> get_subElements(const T& value){
			std::vector<value_type> __result;
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					__result.push_back(it);
				}
			}
			return __result;
		}

		template<class T, int N>
		int find(T& value){
			int i = 0;
			for (auto it : m_data){
				++i;
				T t = get_element<T, N>(it);
				if (t == value){
					return i;
				}
			}
			return -1;
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		unsigned colum() {
			if (m_data.empty())
				return 0;
			return 4;
		}

		unsigned row(){
			return m_data.size();
		}

		void reserve(unsigned size){
			m_data.reserve(size);
		}

		void resize(unsigned size){
			m_data.resize(size);
		}

		void erase(iterator it){
			m_data.erase(it);
		}

		value_type back(){
			if (m_data.empty())
				return value_type();
			return *(--m_data.end());
		}

		void pop_back(){
			if (m_data.empty())
				return;
			m_data.erase(--m_data.end());
		}

		value_type front(){
			if (m_data.empty())
				return value_type();
			return *m_data.begin();
		}

		void pop_front(){
			if (m_data.empty())
				return;
			m_data.erase(m_data.begin());
		}

		iterator begin(){
			return m_data.begin();
		}

		iterator end(){
			return m_data.end();
		}

		template<class T, int N>
		T at(unsigned row){
			if (row >= m_data.size())
				throw std::runtime_error("row index beyond vector range!!!");
			return std::get<N>(m_data.at(row));
		}

		template<class T, int N>
		std::vector<T> GetColumData(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
		}

		value_type GetRowData(unsigned row){
			if (m_data.empty())
				return value_type();
			return m_data.at(row);
		}

		void fillHMatrixFromFile(const std::string& filename, const std::string& startline = "(\\[.{1,2000}\\])"){
			std::ifstream inFile(filename);
			if (inFile.fail()){
				throw std::runtime_error("open file for read fail!!!");
			}
			bool b_is_empty = startline.empty();
			std::string read_str;
			std::regex  reg(startline);
			m_data.clear();
			std::vector<std::string> temp_v;
			while (std::getline(inFile, read_str)){
				if (read_str.empty())
					continue;
				if (b_is_empty){
					temp_v.clear();
					boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
					if (temp_v.size() < 4)
						continue;
					try{
						std::vector<std::string> v1, v2;
						boost::split(v1, temp_v.at(2), boost::is_any_of(","));
						boost::split(v2, temp_v.at(3), boost::is_any_of(","));
						v1.erase(std::remove(v1.begin(), v1.end(), ""), v1.end());
						v2.erase(std::remove(v2.begin(), v2.end(), ""), v2.end());
						std::vector<double> re_1, re_2;
						std::for_each(v1.begin(), v1.end(), [&](std::string str){
							re_1.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v2.begin(), v2.end(), [&](std::string str){
							re_2.push_back(boost::lexical_cast<double>(str));
						});
						push_back(boost::lexical_cast<int>(temp_v.at(0)), boost::lexical_cast<int>(temp_v.at(1)),
							re_1, re_2);
					}
					catch (...){
						//这里主要捕获转换失败异常，如果转换失败，不用管，继续读取下面的内容
						continue;
					}
				}
				else if (std::regex_match(read_str, reg)){
					while (std::getline(inFile, read_str)){
						if (read_str.empty())
							continue;
						temp_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						if (temp_v.size() < 4)
							continue;
						try{
							std::vector<std::string> v1, v2;
							boost::split(v1, temp_v.at(2), boost::is_any_of(","));
							boost::split(v2, temp_v.at(3), boost::is_any_of(","));
							v1.erase(std::remove(v1.begin(), v1.end(), ""), v1.end());
							v2.erase(std::remove(v2.begin(), v2.end(), ""), v2.end());
							std::vector<double> re_1, re_2;
							std::for_each(v1.begin(), v1.end(), [&](std::string str){
								re_1.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v2.begin(), v2.end(), [&](std::string str){
								re_2.push_back(boost::lexical_cast<double>(str));
							});
							push_back(boost::lexical_cast<int>(temp_v.at(0)), boost::lexical_cast<int>(temp_v.at(1)),
								re_1, re_2);
						}
						catch (...){
							//这里主要捕获转换失败异常，如果转换失败，不用管，继续读取下面的内容
							continue;
						}
					}
				}
			}
			inFile.close();
		}

		friend std::ostream& operator<<(std::ostream& os, HMatrix& m){
			if (m.getData().empty())
				return os;
			os << "[row:" << m.m_data.size() << "\tcolum:" << 4 << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](tuple_data data){
				os.setf(std::ios_base::left, std::ios_base::adjustfield);
				os.width(10);
				os << std::get<0>(data);
				os.width(10);
				os << "";
				os << std::get<1>(data);
				os.width(10);
				os << "";
				os << std::get<2>(data);
				os.width(10);
				os << "";
				os << std::get<3>(data) << std::endl;
			});
			return os;
		}

	private:
		std::vector<tuple_data>  m_data;
	};

	template<class T0, class T1, class T2, class T3, class T4>
	class HMatrix<T0, T1, T2, T3, T4>{
		typedef std::tuple<T0, T1, T2, T3, T4> tuple_data;
	public:

		typedef typename std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		HMatrix(const HMatrix& other){
			m_data = other.m_data;
		}

		//=======================================
		// 使用数据来构造
		//========================================
		HMatrix(std::vector<value_type>& __data){
			m_data = __data;
		}

		HMatrix& operator=(std::vector<value_type>& __data){
			m_data = __data;
			return *this;
		}

		HMatrix& operator=(const HMatrix& other){
			if (this == &other){
				return &this;
			}
			m_data = other.m_data;
			return *this;
		}

		std::vector<tuple_data>& getData(){
			return m_data;
		}

		const std::vector<tuple_data>& getData() const {
			return m_data;
		}

		void push_back(T0 value0, T1 value1, T2 value2, T3 value3, T4 value4){
			m_data.push_back(std::make_tuple(value0, value1, value2, value3, value4));
		}

		void push_back(value_type value){
			m_data.push_back(value);
		}

		void push_back(HMatrix& other){
			std::copy(other.begin(), other.end(), std::back_inserter(m_data));
		}

		static value_type make_value(T0 value0, T1 value1, T2 value2, T3 value3, T4 value4){
			return std::make_tuple(value0, value1, value2, value3, value4);
		}

		template<typename T, int N>
		T get_element(value_type v){
			if (N >= 5){
				throw std::runtime_error("index out of arange");
			}
			return std::get<N>(v);
		}

		//===================================
		// 获取满足条件的第一个元素
		//===================================
		template<typename T, int N>
		boost::optional<value_type> get_element(T& value){
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					return boost::optional<value_type>(it);
				}
			}
			return boost::optional<value_type>();
		}

		//============================================
		// 按照指定元素指定方法进行排序
		//============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(){
			std::sort(m_data.begin(), m_data.end(), [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//=============================================
		// 对一个区间进行排序
		//=============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(iterator first, iterator last){
			std::sort(first, last, [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//==============================================
		// 获取指定项的所有数据
		//===============================================
		template<class T, int N>
		std::vector<value_type> get_subElements(const T& value){
			std::vector<value_type> __result;
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					__result.push_back(it);
				}
			}
			return __result;
		}

		template<class T, int N>
		int find(T& value){
			int i = 0;
			for (auto it : m_data){
				++i;
				T t = get_element<T, N>(it);
				if (t == value){
					return i;
				}
			}
			return -1;
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		unsigned colum() {
			if (m_data.empty())
				return 0;
			return 5;
		}

		unsigned row(){
			return m_data.size();
		}

		void reserve(unsigned size){
			m_data.reserve(size);
		}

		void resize(unsigned size){
			m_data.resize(size);
		}

		void erase(iterator it){
			m_data.erase(it);
		}

		value_type back(){
			if (m_data.empty())
				return value_type();
			return *(--m_data.end());
		}

		void pop_back(){
			if (m_data.empty())
				return;
			m_data.erase(--m_data.end());
		}

		value_type front(){
			if (m_data.empty())
				return value_type();
			return *m_data.begin();
		}

		void pop_front(){
			if (m_data.empty())
				return;
			m_data.erase(m_data.begin());
		}

		iterator begin(){
			return m_data.begin();
		}

		iterator end(){
			return m_data.end();
		}

		template<class T, int N>
		T at(unsigned row){
			if (row >= m_data.size())
				throw std::runtime_error("row index beyond vector range!!!");
			return std::get<N>(m_data.at(row));
		}

		template<class T, int N>
		std::vector<T> GetColumData(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
		}

		value_type GetRowData(unsigned row){
			if (m_data.empty())
				return value_type();
			return m_data.at(row);
		}


		void fillHMatrixFromFile(const std::string& filename, const std::string& startline = "(\\[.{1,2000}\\])"){
			std::ifstream inFile(filename);
			if (inFile.fail()){
				throw std::runtime_error("open file for read fail!!!");
			}
			bool b_is_empty = startline.empty();
			std::string read_str;
			std::regex  reg(startline);
			m_data.clear();
			std::vector<std::string> temp_v;
			while (std::getline(inFile, read_str)){
				if (read_str.empty())
					continue;
				if (b_is_empty){
					temp_v.clear();
					boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
					if (temp_v.size() < 5)
						continue;
					try{
						push_back(boost::lexical_cast<T0>(temp_v.at(0)), boost::lexical_cast<T1>(temp_v.at(1)),
							boost::lexical_cast<T2>(temp_v.at(2)), boost::lexical_cast<T3>(temp_v.at(3)), boost::lexical_cast<T4>(temp_v.at(4)));
					}
					catch (...){
						continue;
					}
				}
				else if (std::regex_match(read_str, reg)){
					while (std::getline(inFile, read_str)){
						if (read_str.empty())
							continue;
						temp_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						if (temp_v.size() < 5)
							continue;
						try{
							push_back(boost::lexical_cast<T0>(temp_v.at(0)), boost::lexical_cast<T1>(temp_v.at(1)),
								boost::lexical_cast<T2>(temp_v.at(2)), boost::lexical_cast<T3>(temp_v.at(3)), boost::lexical_cast<T4>(temp_v.at(4)));
						}
						catch (...){
							continue;
						}
					}
				}
			}
			inFile.close();
		}

		friend std::ostream& operator<<(std::ostream& os, HMatrix<T0, T1, T2, T4>& m){
			if (m.getData().empty())
				return os;
			os << "[row:" << m.m_data.size() << "\tcolum:" << 5 << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](tuple_data data){
				os.setf(std::ios_base::left, std::ios_base::adjustfield);
				os << std::get<0>(data);
				os << "\t";
				os << std::get<1>(data);
				os << "\t";
				os << std::get<2>(data);
				os << "\t";
				os << std::get<3>(data);
				os << "\t";
				os << std::get<4>(data) << std::endl;
			});
			return os;
		}
	private:
		std::vector<tuple_data>  m_data;
	};

	template<class T0, class T1, class T2, class T3, class T4, class T5>
	class HMatrix<T0, T1, T2, T3, T4, T5>{
		typedef std::tuple<T0, T1, T2, T3, T4, T5> tuple_data;
	public:
		typedef typename std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		HMatrix(const HMatrix& other){
			m_data = other.m_data;
		}

		//=======================================
		// 使用数据来构造
		//========================================
		HMatrix(std::vector<value_type>& __data){
			m_data = __data;
		}

		HMatrix& operator=(std::vector<value_type>& __data){
			m_data = __data;
			return *this;
		}

		HMatrix& operator=(const HMatrix& other){
			if (this == &other){
				return *this;
			}
			m_data = other.m_data;
			return *this;
		}

		std::vector<tuple_data>& getData(){
			return m_data;
		}

		const std::vector<tuple_data>& getData() const {
			return m_data;
		}

		void push_back(T0 value0, T1 value1, T2 value2, T3 value3, T4 value4, T5 value5){
			m_data.push_back(std::make_tuple(value0, value1, value2, value3, value4, value5));
		}

		void push_back(value_type value){
			m_data.push_back(value);
		}

		void push_back(HMatrix& other){
			std::copy(other.begin(), other.end(), std::back_inserter(m_data));
		}

		static value_type make_value(T0 value0, T1 value1, T2 value2, T3 value3, T4 value4, T5 value5){
			return std::make_tuple(value0, value1, value2, value3, value4, value5);
		}

		template<typename T, int N>
		T get_element(value_type v){
			if (N >= 6){
				throw std::runtime_error("index out of arange");
			}
			return std::get<N>(v);
		}

		//===================================
		// 获取满足条件的第一个元素
		//===================================
		template<typename T, int N>
		boost::optional<value_type> get_element(T& value){
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					return boost::optional<value_type>(it);
				}
			}
			return boost::optional<value_type>();
		}

		//============================================
		// 按照指定元素指定方法进行排序
		//============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(){
			std::sort(m_data.begin(), m_data.end(), [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//=============================================
		// 对一个区间进行排序
		//=============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(iterator first, iterator last){
			std::sort(first, last, [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//==============================================
		// 获取指定项的所有数据
		//===============================================
		template<class T, int N>
		std::vector<value_type> get_subElements(const T& value){
			std::vector<value_type> __result;
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					__result.push_back(it);
				}
			}
			return __result;
		}

		template<class T, int N>
		int find(T& value){
			int i = 0;
			for (auto it : m_data){
				++i;
				T t = get_element<T, N>(it);
				if (t == value){
					return i;
				}
			}
			return -1;
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		unsigned colum() {
			if (m_data.empty())
				return 0;
			return 6;
		}

		unsigned row(){
			return m_data.size();
		}

		void reserve(unsigned size){
			m_data.reserve(size);
		}

		void resize(unsigned size){
			m_data.resize(size);
		}

		void erase(iterator it){
			m_data.erase(it);
		}

		value_type back(){
			if (m_data.empty())
				return value_type();
			return *(--m_data.end());
		}

		void pop_back(){
			if (m_data.empty())
				return;
			m_data.erase(--m_data.end());
		}

		value_type front(){
			if (m_data.empty())
				return value_type();
			return *m_data.begin();
		}

		void pop_front(){
			if (m_data.empty())
				return;
			m_data.erase(m_data.begin());
		}

		iterator begin(){
			return m_data.begin();
		}

		iterator end(){
			return m_data.end();
		}

		template<class T, int N>
		T at(unsigned row){
			if (row >= m_data.size())
				throw std::runtime_error("row index beyond vector range!!!");
			return std::get<N>(m_data.at(row));
		}

		template<class T, int N>
		std::vector<T> GetColumData(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
		}

		value_type GetRowData(unsigned row){
			if (m_data.empty())
				return value_type();
			return m_data.at(row);
		}


		void fillHMatrixFromFile(const std::string& filename, const std::string& startline = "(\\[.{1,2000}\\])"){
			std::ifstream inFile(filename);
			if (inFile.fail()){
				throw std::runtime_error("open file for read fail!!!");
			}
			bool b_is_empty = startline.empty();
			std::string read_str;
			std::regex  reg(startline);
			m_data.clear();
			std::vector<std::string> temp_v;
			while (std::getline(inFile, read_str)){
				if (read_str.empty())
					continue;
				if (b_is_empty){
					temp_v.clear();
					boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
					if (temp_v.size() < 6)
						continue;
					try{
						push_back(boost::lexical_cast<T0>(temp_v.at(0)), boost::lexical_cast<T1>(temp_v.at(1)),
							boost::lexical_cast<T2>(temp_v.at(2)), boost::lexical_cast<T3>(temp_v.at(3)), boost::lexical_cast<T4>(temp_v.at(4)),
							boost::lexical_cast<T5>(temp_v.at(5)));
					}
					catch (...){
						continue;
					}
				}
				else if (std::regex_match(read_str, reg)){
					while (std::getline(inFile, read_str)){
						if (read_str.empty())
							continue;
						temp_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						if (temp_v.size() < 6)
							continue;
						try{
							push_back(boost::lexical_cast<T0>(temp_v.at(0)), boost::lexical_cast<T1>(temp_v.at(1)),
								boost::lexical_cast<T2>(temp_v.at(2)), boost::lexical_cast<T3>(temp_v.at(3)), boost::lexical_cast<T4>(temp_v.at(4)),
								boost::lexical_cast<T5>(temp_v.at(5)));
						}
						catch (...){
							continue;
						}
					}
				}
			}
			inFile.close();
		}

		friend std::ostream& operator<<(std::ostream& os, HMatrix<T0, T1, T2, T4, T5>& m){
			if (m.getData().empty())
				return os;
			os << "[row:" << m.m_data.size() << "\tcolum:" << 6 << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](tuple_data data){
				os.setf(std::ios_base::left, std::ios_base::adjustfield);
				os << std::get<0>(data);
				os << "\t";
				os << std::get<1>(data);
				os << "\t";
				os << std::get<2>(data);
				os << "\t";
				os << std::get<3>(data);
				os << "\t";
				os << std::get<4>(data);
				os << "\t";
				os << std::get<5>(data) << std::endl;
			});
			return os;
		}

	private:
		std::vector<tuple_data>  m_data;
	};


	//------------------------------------------------------------
	//6参数double的特化版本
	//------------------------------------------------------------
	template<>
	class HMatrix<double, double, std::vector<double>, std::vector<double>, std::vector<double>, std::vector<double>>{
		typedef std::tuple<double, double, std::vector<double>, std::vector<double>, std::vector<double>, std::vector<double>> tuple_data;
	public:
		typedef std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		HMatrix(const HMatrix& other){
			m_data = other.m_data;
		}

		//=======================================
		// 使用数据来构造
		//========================================
		HMatrix(std::vector<value_type>& __data){
			m_data = __data;
		}

		HMatrix& operator=(std::vector<value_type>& __data){
			m_data = __data;
			return *this;
		}

		HMatrix& operator=(const HMatrix& other){
			if (this == &other){
				return *this;
			}
			m_data = other.m_data;
			return *this;
		}

		std::vector<tuple_data>& getData(){
			return m_data;
		}

		const std::vector<tuple_data>& getData() const {
			return m_data;
		}

		void push_back(double value0, double value1,
			std::vector<double>& value2, std::vector<double>& value3,
			std::vector<double>& value4, std::vector<double>& value5){
			m_data.push_back(std::make_tuple(value0, value1, value2, value3, value4, value5));
		}

		void push_back(value_type value){
			m_data.push_back(value);
		}

		void push_back(HMatrix& other){
			std::copy(other.begin(), other.end(), std::back_inserter(m_data));
		}

		static value_type make_value(double value0, double value1,
			std::vector<double>& value2, std::vector<double>& value3,
			std::vector<double>& value4, std::vector<double>& value5){
			return std::make_tuple(value0, value1, value2, value3, value4, value5);
		}

		template<typename T, int N>
		T get_element(value_type v){
			if (N >= 6){
				throw std::runtime_error("index out of arange");
			}
			return std::get<N>(v);
		}

		//===================================
		// 获取满足条件的第一个元素
		//===================================
		template<typename T, int N>
		boost::optional<value_type> get_element(T& value){
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					return boost::optional<value_type>(it);
				}
			}
			return boost::optional<value_type>();
		}

		//============================================
		// 按照指定元素指定方法进行排序
		//============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(){
			std::sort(m_data.begin(), m_data.end(), [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//=============================================
		// 对一个区间进行排序
		//=============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(iterator first, iterator last){
			std::sort(first, last, [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//==============================================
		// 获取指定项的所有数据
		//===============================================
		template<class T, int N>
		std::vector<value_type> get_subElements(const T& value){
			std::vector<value_type> __result;
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					__result.push_back(it);
				}
			}
			return __result;
		}

		template<class T, int N>
		int find(T& value){
			int i = 0;
			for (auto it : m_data){
				++i;
				T t = get_element<T, N>(it);
				if (t == value){
					return i;
				}
			}
			return -1;
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		unsigned colum() {
			if (m_data.empty())
				return 0;
			return 6;
		}

		unsigned row(){
			return m_data.size();
		}

		void reserve(unsigned size){
			m_data.reserve(size);
		}

		void resize(unsigned size){
			m_data.resize(size);
		}

		void erase(iterator it){
			m_data.erase(it);
		}

		value_type back(){
			if (m_data.empty())
				return value_type();
			return *(--m_data.end());
		}

		void pop_back(){
			if (m_data.empty())
				return;
			m_data.erase(--m_data.end());
		}

		value_type front(){
			if (m_data.empty())
				return value_type();
			return *m_data.begin();
		}

		void pop_front(){
			if (m_data.empty())
				return;
			m_data.erase(m_data.begin());
		}

		iterator begin(){
			return m_data.begin();
		}

		iterator end(){
			return m_data.end();
		}

		template<class T, int N>
		T at(unsigned row){
			if (row >= m_data.size())
				throw std::runtime_error("row index beyond vector range!!!");
			return std::get<N>(m_data.at(row));
		}

		template<class T, int N>
		std::vector<T> GetColumData(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
		}

		value_type GetRowData(unsigned row){
			if (m_data.empty())
				return value_type();
			return m_data.at(row);
		}


		void fillHMatrixFromFile(const std::string& filename, const std::string& startline = "(\\[.{1,2000}\\])"){
			std::ifstream inFile(filename);
			if (inFile.fail()){
				throw std::runtime_error("open file for read fail!!!");
			}
			bool b_is_empty = startline.empty();
			std::string read_str;
			std::regex  reg(startline);
			m_data.clear();
			std::vector<std::string> temp_v;
			while (std::getline(inFile, read_str)){
				if (read_str.empty())
					continue;
				if (b_is_empty){
					try{
						temp_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						if (temp_v.size() < 6)
							continue;

						std::vector<std::string> v1, v2, v3, v4;
						boost::split(v1, temp_v.at(2), boost::is_any_of(","));
						boost::split(v2, temp_v.at(3), boost::is_any_of(","));
						boost::split(v3, temp_v.at(4), boost::is_any_of(","));
						boost::split(v4, temp_v.at(5), boost::is_any_of(","));
						v1.erase(std::remove(v1.begin(), v1.end(), ""), v1.end());
						v2.erase(std::remove(v2.begin(), v2.end(), ""), v2.end());
						v3.erase(std::remove(v3.begin(), v3.end(), ""), v3.end());
						v4.erase(std::remove(v4.begin(), v4.end(), ""), v4.end());

						std::vector<double> re_1, re_2, re_3, re_4;
						std::for_each(v1.begin(), v1.end(), [&](std::string str){
							re_1.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v2.begin(), v2.end(), [&](std::string str){
							re_2.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v3.begin(), v3.end(), [&](std::string str){
							re_3.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v4.begin(), v4.end(), [&](std::string str){
							re_4.push_back(boost::lexical_cast<double>(str));
						});

						push_back(boost::lexical_cast<double>(temp_v.at(0)), boost::lexical_cast<double>(temp_v.at(1)),
							re_1, re_2, re_3, re_4);
					}
					catch (...){
						continue;
					}
				}
				else if (std::regex_match(read_str, reg)){
					while (std::getline(inFile, read_str)){
						try{
							if (read_str.empty())
								continue;
							temp_v.clear();
							boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
							if (temp_v.size() < 6)
								continue;

							std::vector<std::string> v1, v2, v3, v4;
							boost::split(v1, temp_v.at(2), boost::is_any_of(","));
							boost::split(v2, temp_v.at(3), boost::is_any_of(","));
							boost::split(v3, temp_v.at(4), boost::is_any_of(","));
							boost::split(v4, temp_v.at(5), boost::is_any_of(","));
							v1.erase(std::remove(v1.begin(), v1.end(), ""), v1.end());
							v2.erase(std::remove(v2.begin(), v2.end(), ""), v2.end());
							v3.erase(std::remove(v3.begin(), v3.end(), ""), v3.end());
							v4.erase(std::remove(v4.begin(), v4.end(), ""), v4.end());

							std::vector<double> re_1, re_2, re_3, re_4;
							std::for_each(v1.begin(), v1.end(), [&](std::string str){
								re_1.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v2.begin(), v2.end(), [&](std::string str){
								re_2.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v3.begin(), v3.end(), [&](std::string str){
								re_3.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v4.begin(), v4.end(), [&](std::string str){
								re_4.push_back(boost::lexical_cast<double>(str));
							});

							push_back(boost::lexical_cast<double>(temp_v.at(0)), boost::lexical_cast<double>(temp_v.at(1)),
								re_1, re_2, re_3, re_4);
						}
						catch (...){
							continue;
						}
					}
				}
			}
			inFile.close();
		}

		friend std::ostream& operator<<(std::ostream& os, HMatrix& m){
			if (m.getData().empty())
				return os;
			os << "[row:" << m.m_data.size() << "\tcolum:" << 6 << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](tuple_data data){
				os.setf(std::ios_base::left, std::ios_base::adjustfield);
				os.width(10);
				os << std::get<0>(data);
				os.width(10);
				os << std::get<1>(data);
				os << std::get<2>(data);
				os.width(10);
				os << "";
				os << std::get<3>(data);
				os.width(10);
				os << "";
				os << std::get<4>(data);
				os.width(10);
				os << "";
				os << std::get<5>(data) << std::endl;
			});
			return os;
		}

	private:
		std::vector<tuple_data>  m_data;
	};

	//6参数int的特化版本
	template<>
	class HMatrix<int, int, std::vector<double>, std::vector<double>, std::vector<double>, std::vector<double>>{
		typedef std::tuple<int, int, std::vector<double>, std::vector<double>, std::vector<double>, std::vector<double>> tuple_data;
	public:
		typedef std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		HMatrix(const HMatrix& other){
			m_data = other.m_data;
		}

		//=======================================
		// 使用数据来构造
		//========================================
		HMatrix(std::vector<value_type>& __data){
			m_data = __data;
		}

		HMatrix& operator=(std::vector<value_type>& __data){
			m_data = __data;
			return *this;
		}

		HMatrix& operator=(const HMatrix& other){
			if (this == &other){
				return *this;
			}
			m_data = other.m_data;
			return *this;
		}

		std::vector<tuple_data>& getData(){
			return m_data;
		}

		const std::vector<tuple_data>& getData() const {
			return m_data;
		}

		void push_back(int value0, int value1,
			std::vector<double>& value2, std::vector<double>& value3,
			std::vector<double>& value4, std::vector<double>& value5){
			m_data.push_back(std::make_tuple(value0, value1, value2, value3, value4, value5));
		}

		void push_back(value_type value){
			m_data.push_back(value);
		}

		void push_back(HMatrix& other){
			std::copy(other.begin(), other.end(), std::back_inserter(m_data));
		}

		static value_type make_value(int value0, int value1,
			std::vector<double>& value2, std::vector<double>& value3,
			std::vector<double>& value4, std::vector<double>& value5){
			return std::make_tuple(value0, value1, value2, value3, value4, value5);
		}

		template<typename T, int N>
		T get_element(value_type v){
			if (N >= 6){
				throw std::runtime_error("index out of arange");
			}
			return std::get<N>(v);
		}

		//===================================
		// 获取满足条件的第一个元素
		//===================================
		template<typename T, int N>
		boost::optional<value_type> get_element(T& value){
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					return boost::optional<value_type>(it);
				}
			}
			return boost::optional<value_type>();
		}

		//============================================
		// 按照指定元素指定方法进行排序
		//============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(){
			std::sort(m_data.begin(), m_data.end(), [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//=============================================
		// 对一个区间进行排序
		//=============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(iterator first, iterator last){
			std::sort(first, last, [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//==============================================
		// 获取指定项的所有数据
		//===============================================
		template<class T, int N>
		std::vector<value_type> get_subElements(const T& value){
			std::vector<value_type> __result;
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					__result.push_back(it);
				}
			}
			return __result;
		}

		template<class T, int N>
		int find(T& value){
			int i = 0;
			for (auto it : m_data){
				++i;
				T t = get_element<T, N>(it);
				if (t == value){
					return i;
				}
			}
			return -1;
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		unsigned colum() {
			if (m_data.empty())
				return 0;
			return 6;
		}

		unsigned row(){
			return m_data.size();
		}

		void reserve(unsigned size){
			m_data.reserve(size);
		}

		void resize(unsigned size){
			m_data.resize(size);
		}

		void erase(iterator it){
			m_data.erase(it);
		}

		value_type back(){
			if (m_data.empty())
				return value_type();
			return *(--m_data.end());
		}

		void pop_back(){
			if (m_data.empty())
				return;
			m_data.erase(--m_data.end());
		}

		value_type front(){
			if (m_data.empty())
				return value_type();
			return *m_data.begin();
		}

		void pop_front(){
			if (m_data.empty())
				return;
			m_data.erase(m_data.begin());
		}

		iterator begin(){
			return m_data.begin();
		}

		iterator end(){
			return m_data.end();
		}

		template<class T, int N>
		T at(unsigned row){
			if (row >= m_data.size())
				throw std::runtime_error("row index beyond vector range!!!");
			return std::get<N>(m_data.at(row));
		}

		template<class T, int N>
		std::vector<T> GetColumData(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
		}

		value_type GetRowData(unsigned row){
			if (m_data.empty())
				return value_type();
			return m_data.at(row);
		}


		void fillHMatrixFromFile(const std::string& filename, const std::string& startline = "(\\[.{1,2000}\\])"){
			std::ifstream inFile(filename);
			if (inFile.fail()){
				throw std::runtime_error("open file for read fail!!!");
			}
			bool b_is_empty = startline.empty();
			std::string read_str;
			std::regex  reg(startline);
			m_data.clear();
			std::vector<std::string> temp_v;
			while (std::getline(inFile, read_str)){
				if (read_str.empty())
					continue;
				if (b_is_empty){
					try{
						temp_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						if (temp_v.size() < 6)
							continue;

						std::vector<std::string> v1, v2, v3, v4;
						boost::split(v1, temp_v.at(2), boost::is_any_of(","));
						boost::split(v2, temp_v.at(3), boost::is_any_of(","));
						boost::split(v3, temp_v.at(4), boost::is_any_of(","));
						boost::split(v4, temp_v.at(5), boost::is_any_of(","));
						v1.erase(std::remove(v1.begin(), v1.end(), ""), v1.end());
						v2.erase(std::remove(v2.begin(), v2.end(), ""), v2.end());
						v3.erase(std::remove(v3.begin(), v3.end(), ""), v3.end());
						v4.erase(std::remove(v4.begin(), v4.end(), ""), v4.end());

						std::vector<double> re_1, re_2, re_3, re_4;
						std::for_each(v1.begin(), v1.end(), [&](std::string str){
							re_1.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v2.begin(), v2.end(), [&](std::string str){
							re_2.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v3.begin(), v3.end(), [&](std::string str){
							re_3.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v4.begin(), v4.end(), [&](std::string str){
							re_4.push_back(boost::lexical_cast<double>(str));
						});

						push_back(boost::lexical_cast<int>(temp_v.at(0)), boost::lexical_cast<int>(temp_v.at(1)),
							re_1, re_2, re_3, re_4);
					}
					catch (...){
						continue;
					}
				}
				else if (std::regex_match(read_str, reg)){
					while (std::getline(inFile, read_str)){
						try{
							if (read_str.empty())
								continue;
							temp_v.clear();
							boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
							if (temp_v.size() < 6)
								continue;

							std::vector<std::string> v1, v2, v3, v4;
							boost::split(v1, temp_v.at(2), boost::is_any_of(","));
							boost::split(v2, temp_v.at(3), boost::is_any_of(","));
							boost::split(v3, temp_v.at(4), boost::is_any_of(","));
							boost::split(v4, temp_v.at(5), boost::is_any_of(","));
							v1.erase(std::remove(v1.begin(), v1.end(), ""), v1.end());
							v2.erase(std::remove(v2.begin(), v2.end(), ""), v2.end());
							v3.erase(std::remove(v3.begin(), v3.end(), ""), v3.end());
							v4.erase(std::remove(v4.begin(), v4.end(), ""), v4.end());

							std::vector<double> re_1, re_2, re_3, re_4;
							std::for_each(v1.begin(), v1.end(), [&](std::string str){
								re_1.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v2.begin(), v2.end(), [&](std::string str){
								re_2.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v3.begin(), v3.end(), [&](std::string str){
								re_3.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v4.begin(), v4.end(), [&](std::string str){
								re_4.push_back(boost::lexical_cast<double>(str));
							});

							push_back(boost::lexical_cast<int>(temp_v.at(0)), boost::lexical_cast<int>(temp_v.at(1)),
								re_1, re_2, re_3, re_4);
						}
						catch (...){
							continue;
						}
					}
				}
			}
			inFile.close();
		}

		friend std::ostream& operator<<(std::ostream& os, HMatrix& m){
			if (m.getData().empty())
				return os;
			os << "[row:" << m.m_data.size() << "\tcolum:" << 6 << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](tuple_data data){
				os.setf(std::ios_base::left, std::ios_base::adjustfield);
				os.width(10);
				os << std::get<0>(data);
				os.width(10);
				os << std::get<1>(data);
				os << std::get<2>(data);
				os.width(10);
				os << "";
				os << std::get<3>(data);
				os.width(10);
				os << "";
				os << std::get<4>(data);
				os.width(10);
				os << "";
				os << std::get<5>(data) << std::endl;
			});
			return os;
		}

	private:
		std::vector<tuple_data>  m_data;
	};

	//--------------------------------------------------------
	// 7模板参数
	//--------------------------------------------------------
	template<class T0, class T1, class T2, class T3, class T4, class T5, class T6>
	class HMatrix<T0, T1, T2, T3, T4, T5, T6>{
		typedef std::tuple<T0, T1, T2, T3, T4, T5, T6> tuple_data;
	public:
		typedef typename std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		HMatrix(const HMatrix& other){
			m_data = other.m_data;
		}

		//=======================================
		// 使用数据来构造
		//========================================
		HMatrix(std::vector<value_type>& __data){
			m_data = __data;
		}

		HMatrix& operator=(std::vector<value_type>& __data){
			m_data = __data;
			return *this;
		}

		HMatrix& operator=(const HMatrix& other){
			if (this == &other){
				return *this;
			}
			m_data = other.m_data;
			return *this;
		}

		std::vector<tuple_data>& getData(){
			return m_data;
		}

		const std::vector<tuple_data>& getData() const {
			return m_data;
		}

		void push_back(T0 value0, T1 value1, T2 value2, T3 value3, T4 value4, T5 value5, T6 value6){
			m_data.push_back(std::make_tuple(value0, value1, value2, value3, value4, value5, value6));
		}

		void push_back(value_type value){
			m_data.push_back(value);
		}

		void push_back(HMatrix& other){
			std::copy(other.begin(), other.end(), std::back_inserter(m_data));
		}

		static value_type make_value(T0 value0, T1 value1, T2 value2, T3 value3, T4 value4, T5 value5, T6 value6){
			return std::make_tuple(value0, value1, value2, value3, value4, value5, value6);
		}

		template<typename T, int N>
		T get_element(value_type v){
			if (N >= 7){
				throw std::runtime_error("index out of arange");
			}
			return std::get<N>(v);
		}

		//===================================
		// 获取满足条件的第一个元素
		//===================================
		template<typename T, int N>
		boost::optional<value_type> get_element(T& value){
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					return boost::optional<value_type>(it);
				}
			}
			return boost::optional<value_type>();
		}

		//============================================
		// 按照指定元素指定方法进行排序
		//============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(){
			std::sort(m_data.begin(), m_data.end(), [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//=============================================
		// 对一个区间进行排序
		//=============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(iterator first, iterator last){
			std::sort(first, last, [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//==============================================
		// 获取指定项的所有数据
		//===============================================
		template<class T, int N>
		std::vector<value_type> get_subElements(const T& value){
			std::vector<value_type> __result;
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					__result.push_back(it);
				}
			}
			return __result;
		}

		template<class T, int N>
		int find(T& value){
			int i = 0;
			for (auto it : m_data){
				++i;
				T t = get_element<T, N>(it);
				if (t == value){
					return i;
				}
			}
			return -1;
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		unsigned colum() {
			if (m_data.empty())
				return 0;
			return 7;
		}

		unsigned row(){
			return m_data.size();
		}

		void reserve(unsigned size){
			m_data.reserve(size);
		}

		void resize(unsigned size){
			m_data.resize(size);
		}

		void erase(iterator it){
			m_data.erase(it);
		}

		value_type back(){
			if (m_data.empty())
				return value_type();
			return *(--m_data.end());
		}

		void pop_back(){
			if (m_data.empty())
				return;
			m_data.erase(--m_data.end());
		}

		value_type front(){
			if (m_data.empty())
				return value_type();
			return *m_data.begin();
		}

		void pop_front(){
			if (m_data.empty())
				return;
			m_data.erase(m_data.begin());
		}

		iterator begin(){
			return m_data.begin();
		}

		iterator end(){
			return m_data.end();
		}

		template<class T, int N>
		T at(unsigned row){
			if (row >= m_data.size())
				throw std::runtime_error("row index beyond vector range!!!");
			return std::get<N>(m_data.at(row));
		}

		template<class T, int N>
		std::vector<T> GetColumData(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
		}

		value_type GetRowData(unsigned row){
			if (m_data.empty())
				return value_type();
			return m_data.at(row);
		}

		void fillHMatrixFromFile(const std::string& filename, const std::string& startline = "(\\[.{1,2000}\\])"){
			std::ifstream inFile(filename);
			if (inFile.fail()){
				throw std::runtime_error("open file for read fail!!!");
			}
			bool b_is_empty = startline.empty();
			std::string read_str;
			std::regex  reg(startline);
			m_data.clear();
			std::vector<std::string> temp_v;
			while (std::getline(inFile, read_str)){
				if (read_str.empty())
					continue;
				if (b_is_empty){
					try{
						temp_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						if (temp_v.size() < 7)
							continue;
						push_back(boost::lexical_cast<T0>(temp_v.at(0)), boost::lexical_cast<T1>(temp_v.at(1)),
							boost::lexical_cast<T2>(temp_v.at(2)), boost::lexical_cast<T3>(temp_v.at(3)), boost::lexical_cast<T4>(temp_v.at(4)),
							boost::lexical_cast<T5>(temp_v.at(5)), boost::lexical_cast<T6>(temp_v.at(6)));
					}
					catch (...){
						continue;
					}
				}
				if (std::regex_match(read_str, reg)){
					while (std::getline(inFile, read_str)){
						if (read_str.empty())
							continue;
						temp_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						if (temp_v.size() < 7)
							continue;
						try{
							push_back(boost::lexical_cast<T0>(temp_v.at(0)), boost::lexical_cast<T1>(temp_v.at(1)),
								boost::lexical_cast<T2>(temp_v.at(2)), boost::lexical_cast<T3>(temp_v.at(3)), boost::lexical_cast<T4>(temp_v.at(4)),
								boost::lexical_cast<T5>(temp_v.at(5)), boost::lexical_cast<T6>(temp_v.at(6)));
						}
						catch (...){
							continue;
						}
					}
				}
			}
			inFile.close();
		}

		friend std::ostream& operator<<(std::ostream& os, HMatrix<T0, T1, T2, T4, T5, T6>& m){
			if (m.getData().empty())
				return os;
			os << "[row:" << m.m_data.size() << "\tcolum:" << 7 << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](tuple_data data){
				os.setf(std::ios_base::left, std::ios_base::adjustfield);
				os << std::get<0>(data);
				os << "\t";
				os << std::get<1>(data);
				os << "\t";
				os << std::get<2>(data);
				os << "\t";
				os << std::get<3>(data);
				os << "\t";
				os << std::get<4>(data);
				os << "\t";
				os << std::get<5>(data);
				os << "\t";
				os << std::get<6>(data) << std::endl;
			});
			return os;
		}
	private:
		std::vector<tuple_data>  m_data;
	};


	//-----------------------------------------------------
	// 8个模板参数
	//-----------------------------------------------------
	template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	class HMatrix<T0, T1, T2, T3, T4, T5, T6, T7>{
		typedef std::tuple<T0, T1, T2, T3, T4, T5, T6, T7> tuple_data;
	public:
		typedef typename std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		HMatrix(const HMatrix& other){
			m_data = other.m_data;
		}

		//=======================================
		// 使用数据来构造
		//========================================
		HMatrix(std::vector<value_type>& __data){
			m_data = __data;
		}

		HMatrix& operator=(std::vector<value_type>& __data){
			m_data = __data;
			return *this;
		}

		HMatrix& operator=(const HMatrix& other){
			if (this == &other){
				return *this;
			}
			m_data = other.m_data;
			return *this;
		}

		std::vector<tuple_data>& getData(){
			return m_data;
		}

		const std::vector<tuple_data>& getData() const {
			return m_data;
		}

		void push_back(T0 value0, T1 value1, T2 value2, T3 value3, T4 value4, T5 value5, T6 value6, T7 value7){
			m_data.push_back(std::make_tuple(value0, value1, value2, value3, value4, value5, value6, value7));
		}

		void push_back(value_type value){
			m_data.push_back(value);
		}

		void push_back(HMatrix& other){
			std::copy(other.begin(), other.end(), std::back_inserter(m_data));
		}

		static value_type make_value(T0 value0, T1 value1, T2 value2, T3 value3, T4 value4, T5 value5, T6 value6, T7 value7){
			return std::make_tuple(value0, value1, value2, value3, value4, value5, value6, value7);
		}

		template<typename T, int N>
		T get_element(value_type v){
			if (N >= 8){
				throw std::runtime_error("index out of arange");
			}
			return std::get<N>(v);
		}

		//===================================
		// 获取满足条件的第一个元素
		//===================================
		template<typename T, int N>
		boost::optional<value_type> get_element(T& value){
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					return boost::optional<value_type>(it);
				}
			}
			return boost::optional<value_type>();
		}

		//============================================
		// 按照指定元素指定方法进行排序
		//============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(){
			std::sort(m_data.begin(), m_data.end(), [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//=============================================
		// 对一个区间进行排序
		//=============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(iterator first, iterator last){
			std::sort(first, last, [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//==============================================
		// 获取指定项的所有数据
		//===============================================
		template<class T, int N>
		std::vector<value_type> get_subElements(const T& value){
			std::vector<value_type> __result;
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					__result.push_back(it);
				}
			}
			return __result;
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		unsigned colum() {
			if (m_data.empty())
				return 0;
			return 8;
		}

		unsigned row(){
			return m_data.size();
		}

		void reserve(unsigned size){
			m_data.reserve(size);
		}

		void resize(unsigned size){
			m_data.resize(size);
		}

		void erase(iterator it){
			m_data.erase(it);
		}

		value_type back(){
			if (m_data.empty())
				return value_type();
			return *(--m_data.end());
		}

		void pop_back(){
			if (m_data.empty())
				return;
			m_data.erase(--m_data.end());
		}

		value_type front(){
			if (m_data.empty())
				return value_type();
			return *m_data.begin();
		}

		void pop_front(){
			if (m_data.empty())
				return;
			m_data.erase(m_data.begin());
		}

		iterator begin(){
			return m_data.begin();
		}

		iterator end(){
			return m_data.end();
		}

		template<class T, int N>
		T at(unsigned row){
			if (row >= m_data.size())
				throw std::runtime_error("row index beyond vector range!!!");
			return std::get<N>(m_data.at(row));
		}

		template<class T, int N>
		std::vector<T> GetColumData(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
		}

		value_type GetRowData(unsigned row){
			if (m_data.empty())
				return value_type();
			return m_data.at(row);
		}

		void fillHMatrixFromFile(const std::string& filename, const std::string& startline = "(\\[.{1,2000}\\])"){
			std::ifstream inFile(filename);
			if (inFile.fail()){
				throw std::runtime_error("open file for read fail!!!");
			}
			bool b_is_empty = startline.empty();
			std::string read_str;
			std::regex  reg(startline);
			m_data.clear();
			std::vector<std::string> temp_v;
			while (std::getline(inFile, read_str)){
				if (read_str.empty())
					continue;
				if (b_is_empty){
					try{
						temp_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						if (temp_v.size() < 8)
							continue;
						push_back(boost::lexical_cast<T0>(temp_v.at(0)), boost::lexical_cast<T1>(temp_v.at(1)),
							boost::lexical_cast<T2>(temp_v.at(2)), boost::lexical_cast<T3>(temp_v.at(3)), boost::lexical_cast<T4>(temp_v.at(4)),
							boost::lexical_cast<T5>(temp_v.at(5)), boost::lexical_cast<T6>(temp_v.at(6)), boost::lexical_cast<T7>(temp_v.at(7)));
					}
					catch (...){
						continue;
					}
				}
				else if (std::regex_match(read_str, reg)){
					while (std::getline(inFile, read_str)){
						if (read_str.empty())
							continue;
						temp_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						if (temp_v.size() < 8)
							continue;
						try{
							push_back(boost::lexical_cast<T0>(temp_v.at(0)), boost::lexical_cast<T1>(temp_v.at(1)),
								boost::lexical_cast<T2>(temp_v.at(2)), boost::lexical_cast<T3>(temp_v.at(3)), boost::lexical_cast<T4>(temp_v.at(4)),
								boost::lexical_cast<T5>(temp_v.at(5)), boost::lexical_cast<T6>(temp_v.at(6)), boost::lexical_cast<T7>(temp_v.at(7)));
						}
						catch (...){
							continue;
						}
					}
				}
			}
			inFile.close();
		}

		friend std::ostream& operator<<(std::ostream& os, HMatrix<T0, T1, T2, T4, T5, T6, T7>& m){
			if (m.getData().empty())
				return os;
			os << "[row:" << m.m_data.size() << "\tcolum:" << 8 << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](tuple_data data){
				os.setf(std::ios_base::left, std::ios_base::adjustfield);
				os << std::get<0>(data);
				os << "\t";
				os << std::get<1>(data);
				os << "\t";
				os << std::get<2>(data);
				os << "\t";
				os << std::get<3>(data);
				os << "\t";
				os << std::get<4>(data);
				os << "\t";
				os << std::get<5>(data);
				os << "\t";
				os << std::get<6>(data);
				os << "\t";
				os << std::get<7>(data) << std::endl;
			});
			return os;
		}
	private:
		std::vector<tuple_data>  m_data;
	};


	//------------------------------------------------------------
	// 9个模板参数
	//------------------------------------------------------------
	template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	class HMatrix<T0, T1, T2, T3, T4, T5, T6, T7, T8>{
		typedef std::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8> tuple_data;
	public:
		typedef typename std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		HMatrix(const HMatrix& other){
			m_data = other.m_data;
		}

		//=======================================
		// 使用数据来构造
		//========================================
		HMatrix(std::vector<value_type>& __data){
			m_data = __data;
		}

		HMatrix& operator=(std::vector<value_type>& __data){
			m_data = __data;
			return *this;
		}

		HMatrix& operator=(const HMatrix& other){
			if (this == &other){
				return *this;
			}
			m_data = other.m_data;
			return *this;
		}

		std::vector<tuple_data>& getData(){
			return m_data;
		}

		const std::vector<tuple_data>& getData() const {
			return m_data;
		}

		void push_back(T0 value0, T1 value1, T2 value2, T3 value3, T4 value4, T5 value5, T6 value6, T7 value7, T8 value8){
			m_data.push_back(std::make_tuple(value0, value1, value2, value3, value4, value5, value6, value7, value8));
		}

		void push_back(value_type value){
			m_data.push_back(value);
		}

		void push_back(HMatrix& other){
			std::copy(other.begin(), other.end(), std::back_inserter(m_data));
		}

		static value_type make_value(T0 value0, T1 value1, T2 value2, T3 value3, T4 value4, T5 value5, T6 value6, T7 value7, T8 value8){
			return std::make_tuple(value0, value1, value2, value3, value4, value5, value6, value7, value8);
		}

		template<typename T, int N>
		T get_element(value_type v){
			if (N >= 9){
				throw std::runtime_error("index out of arange");
			}
			return std::get<N>(v);
		}

		//===================================
		// 获取满足条件的第一个元素
		//===================================
		template<typename T, int N>
		boost::optional<value_type> get_element(T& value){
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					return boost::optional<value_type>(it);
				}
			}
			return boost::optional<value_type>();
		}

		//============================================
		// 按照指定元素指定方法进行排序
		//============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(){
			std::sort(m_data.begin(), m_data.end(), [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//=============================================
		// 对一个区间进行排序
		//=============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(iterator first, iterator last){
			std::sort(first, last, [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//==============================================
		// 获取指定项的所有数据
		//===============================================
		template<class T, int N>
		std::vector<value_type> get_subElements(const T& value){
			std::vector<value_type> __result;
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					__result.push_back(it);
				}
			}
			return __result;
		}

		template<class T, int N>
		int find(T& value){
			int i = 0;
			for (auto it : m_data){
				++i;
				T t = get_element<T, N>(it);
				if (t == value){
					return i;
				}
			}
			return -1;
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		unsigned colum() {
			if (m_data.empty())
				return 0;
			return 9;
		}

		unsigned row(){
			return m_data.size();
		}

		void reserve(unsigned size){
			m_data.reserve(size);
		}

		void resize(unsigned size){
			m_data.resize(size);
		}

		void erase(iterator it){
			m_data.erase(it);
		}

		value_type back(){
			if (m_data.empty())
				return value_type();
			return *(--m_data.end());
		}

		void pop_back(){
			if (m_data.empty())
				return;
			m_data.erase(--m_data.end());
		}

		value_type front(){
			if (m_data.empty())
				return value_type();
			return *m_data.begin();
		}

		void pop_front(){
			if (m_data.empty())
				return;
			m_data.erase(m_data.begin());
		}

		iterator begin(){
			return m_data.begin();
		}

		iterator end(){
			return m_data.end();
		}

		template<class T, int N>
		T at(unsigned row){
			if (row >= m_data.size())
				throw std::runtime_error("row index beyond vector range!!!");
			return std::get<N>(m_data.at(row));
		}

		template<class T, int N>
		std::vector<T> GetColumData(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
		}

		value_type GetRowData(unsigned row){
			if (m_data.empty())
				return value_type();
			return m_data.at(row);
		}

		void fillHMatrixFromFile(const std::string& filename, const std::string& startline = "(\\[.{1,2000}\\])"){
			std::ifstream inFile(filename);
			if (inFile.fail()){
				throw std::runtime_error("open file for read fail!!!");
			}
			bool b_is_empty = startline.empty();
			std::string read_str;
			std::regex  reg(startline);
			std::vector<std::string> temp_v;
			m_data.clear();
			while (std::getline(inFile, read_str)){
				if (read_str.empty())
					continue;
				if (b_is_empty){
					try{
						temp_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						if (temp_v.size() < 9)
							continue;
						push_back(boost::lexical_cast<T0>(temp_v.at(0)), boost::lexical_cast<T1>(temp_v.at(1)),
							boost::lexical_cast<T2>(temp_v.at(2)), boost::lexical_cast<T3>(temp_v.at(3)), boost::lexical_cast<T4>(temp_v.at(4)),
							boost::lexical_cast<T5>(temp_v.at(5)), boost::lexical_cast<T6>(temp_v.at(6)), boost::lexical_cast<T7>(temp_v.at(7)),
							boost::lexical_cast<T8>(temp_v.at(8)));
					}
					catch (...){
						continue;
					}
				}
				else if (std::regex_match(read_str, reg)){
					while (std::getline(inFile, read_str)){
						if (read_str.empty())
							continue;
						temp_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						if (temp_v.size() < 9)
							continue;
						try{
							push_back(boost::lexical_cast<T0>(temp_v.at(0)), boost::lexical_cast<T1>(temp_v.at(1)),
								boost::lexical_cast<T2>(temp_v.at(2)), boost::lexical_cast<T3>(temp_v.at(3)), boost::lexical_cast<T4>(temp_v.at(4)),
								boost::lexical_cast<T5>(temp_v.at(5)), boost::lexical_cast<T6>(temp_v.at(6)), boost::lexical_cast<T7>(temp_v.at(7)),
								boost::lexical_cast<T8>(temp_v.at(8)));
						}
						catch (...){
							continue;
						}
					}
				}
			}
			inFile.close();
		}

		friend std::ostream& operator<<(std::ostream& os, HMatrix<T0, T1, T2, T4, T5, T6, T7, T8>& m){
			if (m.getData().empty())
				return os;
			os << "[row:" << m.m_data.size() << "\tcolum:" << 9 << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](tuple_data data){
				os.setf(std::ios_base::left, std::ios_base::adjustfield);
				os << std::get<0>(data);
				os << "\t";
				os << std::get<1>(data);
				os << "\t";
				os << std::get<2>(data);
				os << "\t";
				os << std::get<3>(data);
				os << "\t";
				os << std::get<4>(data);
				os << "\t";
				os << std::get<5>(data);
				os << "\t";
				os << std::get<6>(data);
				os << "\t";
				os << std::get<7>(data);
				os << "\t";
				os << std::get<8>(data) << std::endl;
			});
			return os;
		}
	private:
		std::vector<tuple_data>  m_data;
	};

	//--------------------------------------------------------------
	// 10参数double的特化版本
	// 更具特殊用途而设计的
	//--------------------------------------------------------------
	template<>
	class HMatrix < double, double, std::vector<double>,
		std::vector<double>, std::vector<double>,
		std::vector<double>, std::vector<double>,
		std::vector<double>,
		std::vector<double>, std::vector < double >> {
		typedef std::tuple < double, double, std::vector<double>, std::vector<double>, std::vector<double>,
		std::vector<double>, std::vector<double>,
		std::vector<double>, std::vector<double>, std::vector < double >> tuple_data;
	public:

		typedef std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;


		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		HMatrix(const HMatrix& other){
			m_data = other.m_data;
		}

		//=======================================
		// 使用数据来构造
		//========================================
		HMatrix(std::vector<value_type>& __data){
			m_data = __data;
		}

		HMatrix& operator=(std::vector<value_type>& __data){
			m_data = __data;
			return *this;
		}


		HMatrix& operator=(const HMatrix& other){
			if (this == &other){
				return *this;
			}
			m_data = other.m_data;
			return *this;
		}

		std::vector<tuple_data>& getData(){
			return m_data;
		}

		const std::vector<tuple_data>& getData() const {
			return m_data;
		}

		void push_back(double value0, double value1,
			std::vector<double>& value2, std::vector<double>& value3,
			std::vector<double>& value4, std::vector<double>& value5,
			std::vector<double>& value6, std::vector<double>& value7,
			std::vector<double>& value8, std::vector<double>& value9)
		{
			m_data.push_back(std::make_tuple(value0, value1, value2, value3,
				value4, value5, value6, value7, value8, value9));
		}

		void push_back(value_type value){
			m_data.push_back(value);
		}

		void push_back(HMatrix& other){
			std::copy(other.begin(), other.end(), std::back_inserter(m_data));
		}

		static value_type make_value(double value0, double value1,
			std::vector<double>& value2, std::vector<double>& value3,
			std::vector<double>& value4, std::vector<double>& value5,
			std::vector<double>& value6, std::vector<double>& value7,
			std::vector<double>& value8, std::vector<double>& value9){
			return std::make_tuple(value0, value1, value2, value3, value4, value5, value6, value7, value8, value9);
		}

		template<typename T, int N>
		T get_element(value_type v){
			if (N >= 10){
				throw std::runtime_error("index out of arange");
			}
			return std::get<N>(v);
		}

		//===================================
		// 获取满足条件的第一个元素
		//===================================
		template<typename T, int N>
		boost::optional<value_type> get_element(T& value){
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					return boost::optional<value_type>(it);
				}
			}
			return boost::optional<value_type>();
		}

		//============================================
		// 按照指定元素指定方法进行排序
		//============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(){
			std::sort(m_data.begin(), m_data.end(), [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//=============================================
		// 对一个区间进行排序
		//=============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(iterator first, iterator last){
			std::sort(first, last, [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//==============================================
		// 获取指定项的所有数据
		//===============================================
		template<class T, int N>
		std::vector<value_type> get_subElements(const T& value){
			std::vector<value_type> __result;
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					__result.push_back(it);
				}
			}
			return __result;
		}

		template<class T, int N>
		int find(T& value){
			int i = 0;
			for (auto it : m_data){
				++i;
				T t = get_element<T, N>(it);
				if (t == value){
					return i;
				}
			}
			return -1;
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		unsigned colum() {
			if (m_data.empty())
				return 0;
			return 10;
		}

		unsigned row(){
			return m_data.size();
		}

		void reserve(unsigned size){
			m_data.reserve(size);
		}

		void resize(unsigned size){
			m_data.resize(size);
		}

		void erase(iterator it){
			m_data.erase(it);
		}

		value_type back(){
			if (m_data.empty())
				return value_type();
			return *(--m_data.end());
		}

		void pop_back(){
			if (m_data.empty())
				return;
			m_data.erase(--m_data.end());
		}


		value_type front(){
			if (m_data.empty())
				return value_type();
			return *m_data.begin();
		}

		void pop_front(){
			if (m_data.empty())
				return;
			m_data.erase(m_data.begin());
		}

		iterator begin(){
			return m_data.begin();
		}

		iterator end(){
			return m_data.end();
		}

		template<class T, int N>
		T at(unsigned row){
			if (row >= m_data.size())
				throw std::runtime_error("row index beyond vector range!!!");
			return std::get<N>(m_data.at(row));
		}

		template<class T, int N>
		std::vector<T> GetColumData(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
		}

		value_type GetRowData(unsigned row){
			if (m_data.empty())
				return value_type();
			return m_data.at(row);
		}


		void fillHMatrixFromFile(const std::string& filename, const std::string& startline = "(\\[.{1,2000}\\])"){
			std::ifstream inFile(filename);
			if (inFile.fail()){
				throw std::runtime_error("open file for read fail!!!");
			}
			bool b_is_empty = startline.empty();
			std::string read_str;
			std::regex  reg(startline);
			std::vector<std::string> temp_v;
			m_data.clear();
			while (std::getline(inFile, read_str)){
				if (read_str.empty())
					continue;
				if (b_is_empty){
					try{
						temp_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						if (temp_v.size() < 10)
							continue;

						std::vector<std::string> v1, v2, v3, v4, v5, v6, v7, v8;
						boost::split(v1, temp_v.at(2), boost::is_any_of(","));
						boost::split(v2, temp_v.at(3), boost::is_any_of(","));
						boost::split(v3, temp_v.at(4), boost::is_any_of(","));
						boost::split(v4, temp_v.at(5), boost::is_any_of(","));
						boost::split(v5, temp_v.at(6), boost::is_any_of(","));
						boost::split(v6, temp_v.at(7), boost::is_any_of(","));
						boost::split(v7, temp_v.at(8), boost::is_any_of(","));
						boost::split(v8, temp_v.at(9), boost::is_any_of(","));
						v1.erase(std::remove(v1.begin(), v1.end(), ""), v1.end());
						v2.erase(std::remove(v2.begin(), v2.end(), ""), v2.end());
						v3.erase(std::remove(v3.begin(), v3.end(), ""), v3.end());
						v4.erase(std::remove(v4.begin(), v4.end(), ""), v4.end());
						v5.erase(std::remove(v5.begin(), v5.end(), ""), v5.end());
						v6.erase(std::remove(v6.begin(), v6.end(), ""), v6.end());
						v7.erase(std::remove(v7.begin(), v7.end(), ""), v7.end());
						v8.erase(std::remove(v8.begin(), v8.end(), ""), v8.end());

						std::vector<double> re_1, re_2, re_3, re_4, re_5, re_6, re_7, re_8;
						std::for_each(v1.begin(), v1.end(), [&](std::string str){
							re_1.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v2.begin(), v2.end(), [&](std::string str){
							re_2.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v3.begin(), v3.end(), [&](std::string str){
							re_3.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v4.begin(), v4.end(), [&](std::string str){
							re_4.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v5.begin(), v5.end(), [&](std::string str){
							re_5.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v6.begin(), v6.end(), [&](std::string str){
							re_6.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v7.begin(), v7.end(), [&](std::string str){
							re_7.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v8.begin(), v8.end(), [&](std::string str){
							re_8.push_back(boost::lexical_cast<double>(str));
						});

						push_back(boost::lexical_cast<double>(temp_v.at(0)), boost::lexical_cast<double>(temp_v.at(1)),
							re_1, re_2, re_3, re_4, re_5, re_6, re_7, re_8);
					}
					catch (...){
						continue;
					}
				}
				else if (std::regex_match(read_str, reg)){
					while (std::getline(inFile, read_str)){
						try{
							if (read_str.empty())
								continue;
							temp_v.clear();
							boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
							if (temp_v.size() < 10)
								continue;

							std::vector<std::string> v1, v2, v3, v4, v5, v6, v7, v8;
							boost::split(v1, temp_v.at(2), boost::is_any_of(","));
							boost::split(v2, temp_v.at(3), boost::is_any_of(","));
							boost::split(v3, temp_v.at(4), boost::is_any_of(","));
							boost::split(v4, temp_v.at(5), boost::is_any_of(","));
							boost::split(v5, temp_v.at(6), boost::is_any_of(","));
							boost::split(v6, temp_v.at(7), boost::is_any_of(","));
							boost::split(v7, temp_v.at(8), boost::is_any_of(","));
							boost::split(v8, temp_v.at(9), boost::is_any_of(","));
							v1.erase(std::remove(v1.begin(), v1.end(), ""), v1.end());
							v2.erase(std::remove(v2.begin(), v2.end(), ""), v2.end());
							v3.erase(std::remove(v3.begin(), v3.end(), ""), v3.end());
							v4.erase(std::remove(v4.begin(), v4.end(), ""), v4.end());
							v5.erase(std::remove(v5.begin(), v5.end(), ""), v5.end());
							v6.erase(std::remove(v6.begin(), v6.end(), ""), v6.end());
							v7.erase(std::remove(v7.begin(), v7.end(), ""), v7.end());
							v8.erase(std::remove(v8.begin(), v8.end(), ""), v8.end());

							std::vector<double> re_1, re_2, re_3, re_4, re_5, re_6, re_7, re_8;
							std::for_each(v1.begin(), v1.end(), [&](std::string str){
								re_1.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v2.begin(), v2.end(), [&](std::string str){
								re_2.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v3.begin(), v3.end(), [&](std::string str){
								re_3.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v4.begin(), v4.end(), [&](std::string str){
								re_4.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v5.begin(), v5.end(), [&](std::string str){
								re_5.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v6.begin(), v6.end(), [&](std::string str){
								re_6.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v7.begin(), v7.end(), [&](std::string str){
								re_7.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v8.begin(), v8.end(), [&](std::string str){
								re_8.push_back(boost::lexical_cast<double>(str));
							});

							push_back(boost::lexical_cast<double>(temp_v.at(0)), boost::lexical_cast<double>(temp_v.at(1)),
								re_1, re_2, re_3, re_4, re_5, re_6, re_7, re_8);
						}
						catch (...){
							continue;
						}
					}
				}
			}
			inFile.close();
		}

		friend std::ostream& operator<<(std::ostream& os, HMatrix& m){
			if (m.getData().empty())
				return os;
			os << "[row:" << m.m_data.size() << "\tcolum:" << 6 << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](tuple_data data){
				os.setf(std::ios_base::left, std::ios_base::adjustfield);
				os << std::get<0>(data);
				os << "\t";
				os << std::get<1>(data);
				os << "\t";
				os << std::get<2>(data);
				os << "\t";
				os << std::get<3>(data);
				os << "\t";
				os << std::get<4>(data);
				os << "\t";
				os << std::get<5>(data) << std::endl;
			});
			return os;
		}

	private:
		std::vector<tuple_data>  m_data;
	};

	//--------------------------------------------
	// 10参数int的特化版本
	// 这些全特化都是根据特殊用途而设计的
	//--------------------------------------------
	template<>
	class HMatrix < int, int, std::vector<double>,
		std::vector<double>, std::vector<double>,
		std::vector<double>, std::vector<double>,
		std::vector<double>,
		std::vector<double>, std::vector < double >> {
		typedef std::tuple < int, int, std::vector<double>, std::vector<double>, std::vector<double>,
		std::vector<double>, std::vector<double>,
		std::vector<double>, std::vector<double>, std::vector < double >> tuple_data;
	public:

		typedef std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		HMatrix(const HMatrix& other){
			m_data = other.m_data;
		}

		//=======================================
		// 使用数据来构造
		//========================================
		HMatrix(std::vector<value_type>& __data){
			m_data = __data;
		}

		HMatrix& operator=(std::vector<value_type>& __data){
			m_data = __data;
			return *this;
		}

		HMatrix& operator=(const HMatrix& other){
			if (this == &other){
				return *this;
			}
			m_data = other.m_data;
			return *this;
		}

		std::vector<tuple_data>& getData(){
			return m_data;
		}

		const std::vector<tuple_data>& getData() const{
			return m_data;
		}

		void push_back(int value0, int value1,
			std::vector<double>& value2, std::vector<double>& value3,
			std::vector<double>& value4, std::vector<double>& value5,
			std::vector<double>& value6, std::vector<double>& value7,
			std::vector<double>& value8, std::vector<double>& value9)
		{
			m_data.push_back(std::make_tuple(value0, value1, value2, value3,
				value4, value5, value6, value7, value8, value9));
		}

		void push_back(value_type value){
			m_data.push_back(value);
		}

		void push_back(HMatrix& other){
			std::copy(other.begin(), other.end(), std::back_inserter(m_data));
		}

		static value_type make_value(int value0, int value1,
			std::vector<double>& value2, std::vector<double>& value3,
			std::vector<double>& value4, std::vector<double>& value5,
			std::vector<double>& value6, std::vector<double>& value7,
			std::vector<double>& value8, std::vector<double>& value9){
			return std::make_tuple(value0, value1, value2, value3, value4, value5, value6, value7, value8, value9);
		}

		template<typename T, int N>
		T get_element(value_type v){
			if (N >= 10){
				throw std::runtime_error("index out of arange");
			}
			return std::get<N>(v);
		}

		//===================================
		// 获取满足条件的第一个元素
		//===================================
		template<typename T, int N>
		boost::optional<value_type> get_element(T& value){
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					return boost::optional<value_type>(it);
				}
			}
			return boost::optional<value_type>();
		}

		//============================================
		// 按照指定元素指定方法进行排序
		//============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(){
			std::sort(m_data.begin(), m_data.end(), [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//=============================================
		// 对一个区间进行排序
		//=============================================
		template<class T, int N, class CMP = std::less<T>>
		void sort(iterator first, iterator last){
			std::sort(first, last, [&](value_type v1, value_type v2){
				T t1 = get_element<T, N>(v1);
				T t2 = get_element<T, N>(v2);
				return CMP()(t1, t2);
			});
		}

		//==============================================
		// 获取指定项的所有数据
		//===============================================
		template<class T, int N>
		std::vector<value_type> get_subElements(const T& value){
			std::vector<value_type> __result;
			for (auto it : m_data){
				T t = get_element<T, N>(it);
				if (t == value){
					__result.push_back(it);
				}
			}
			return __result;
		}

		template<class T, int N>
		int find(T& value){
			int i = 0;
			for (auto it : m_data){
				++i;
				T t = get_element<T, N>(it);
				if (t == value){
					return i;
				}
			}
			return -1;
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		unsigned colum() {
			if (m_data.empty())
				return 0;
			return 10;
		}

		unsigned row(){
			return m_data.size();
		}

		void reserve(unsigned size){
			m_data.reserve(size);
		}

		void resize(unsigned size){
			m_data.resize(size);
		}

		void erase(iterator it){
			m_data.erase(it);
		}

		value_type back(){
			if (m_data.empty())
				return value_type();
			return *(--m_data.end());
		}

		void pop_back(){
			if (m_data.empty())
				return;
			m_data.erase(--m_data.end());
		}

		value_type front(){
			if (m_data.empty())
				return value_type();
			return *m_data.begin();
		}

		void pop_front(){
			if (m_data.empty())
				return;
			m_data.erase(m_data.begin());
		}

		iterator begin(){
			return m_data.begin();
		}

		iterator end(){
			return m_data.end();
		}

		template<class T, int N>
		T at(unsigned row){
			if (row >= m_data.size())
				throw std::runtime_error("row index beyond vector range!!!");
			return std::get<N>(m_data.at(row));
		}

		template<class T, int N>
		std::vector<T> GetColumData(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
		}

		value_type GetRowData(unsigned row){
			if (m_data.empty())
				return value_type();
			return m_data.at(row);
		}

		void fillHMatrixFromFile(const std::string& filename, const std::string& startline = "(\\[.{1,2000}\\])"){
			std::ifstream inFile(filename);
			if (inFile.fail()){
				throw std::runtime_error("open file for read fail!!!");
			}
			bool b_is_empty = startline.empty();
			std::string read_str;
			std::regex  reg(startline);
			std::vector<std::string> temp_v;
			m_data.clear();
			while (std::getline(inFile, read_str)){
				if (read_str.empty())
					continue;
				if (b_is_empty){
					try{
						temp_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						if (temp_v.size() < 10)
							continue;

						std::vector<std::string> v1, v2, v3, v4, v5, v6, v7, v8;
						boost::split(v1, temp_v.at(2), boost::is_any_of(","));
						boost::split(v2, temp_v.at(3), boost::is_any_of(","));
						boost::split(v3, temp_v.at(4), boost::is_any_of(","));
						boost::split(v4, temp_v.at(5), boost::is_any_of(","));
						boost::split(v5, temp_v.at(6), boost::is_any_of(","));
						boost::split(v6, temp_v.at(7), boost::is_any_of(","));
						boost::split(v7, temp_v.at(8), boost::is_any_of(","));
						boost::split(v8, temp_v.at(9), boost::is_any_of(","));
						v1.erase(std::remove(v1.begin(), v1.end(), ""), v1.end());
						v2.erase(std::remove(v2.begin(), v2.end(), ""), v2.end());
						v3.erase(std::remove(v3.begin(), v3.end(), ""), v3.end());
						v4.erase(std::remove(v4.begin(), v4.end(), ""), v4.end());
						v5.erase(std::remove(v5.begin(), v5.end(), ""), v5.end());
						v6.erase(std::remove(v6.begin(), v6.end(), ""), v6.end());
						v7.erase(std::remove(v7.begin(), v7.end(), ""), v7.end());
						v8.erase(std::remove(v8.begin(), v8.end(), ""), v8.end());

						std::vector<double> re_1, re_2, re_3, re_4, re_5, re_6, re_7, re_8;
						std::for_each(v1.begin(), v1.end(), [&](std::string str){
							re_1.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v2.begin(), v2.end(), [&](std::string str){
							re_2.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v3.begin(), v3.end(), [&](std::string str){
							re_3.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v4.begin(), v4.end(), [&](std::string str){
							re_4.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v5.begin(), v5.end(), [&](std::string str){
							re_5.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v6.begin(), v6.end(), [&](std::string str){
							re_6.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v7.begin(), v7.end(), [&](std::string str){
							re_7.push_back(boost::lexical_cast<double>(str));
						});
						std::for_each(v8.begin(), v8.end(), [&](std::string str){
							re_8.push_back(boost::lexical_cast<double>(str));
						});

						push_back(boost::lexical_cast<int>(temp_v.at(0)), boost::lexical_cast<int>(temp_v.at(1)),
							re_1, re_2, re_3, re_4, re_5, re_6, re_7, re_8);
					}
					catch (...){
						continue;
					}
				}
				else if (std::regex_match(read_str, reg)){
					while (std::getline(inFile, read_str)){
						try{
							if (read_str.empty())
								continue;
							temp_v.clear();
							boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
							if (temp_v.size() < 10)
								continue;

							std::vector<std::string> v1, v2, v3, v4, v5, v6, v7, v8;
							boost::split(v1, temp_v.at(2), boost::is_any_of(","));
							boost::split(v2, temp_v.at(3), boost::is_any_of(","));
							boost::split(v3, temp_v.at(4), boost::is_any_of(","));
							boost::split(v4, temp_v.at(5), boost::is_any_of(","));
							boost::split(v5, temp_v.at(6), boost::is_any_of(","));
							boost::split(v6, temp_v.at(7), boost::is_any_of(","));
							boost::split(v7, temp_v.at(8), boost::is_any_of(","));
							boost::split(v8, temp_v.at(9), boost::is_any_of(","));
							v1.erase(std::remove(v1.begin(), v1.end(), ""), v1.end());
							v2.erase(std::remove(v2.begin(), v2.end(), ""), v2.end());
							v3.erase(std::remove(v3.begin(), v3.end(), ""), v3.end());
							v4.erase(std::remove(v4.begin(), v4.end(), ""), v4.end());
							v5.erase(std::remove(v5.begin(), v5.end(), ""), v5.end());
							v6.erase(std::remove(v6.begin(), v6.end(), ""), v6.end());
							v7.erase(std::remove(v7.begin(), v7.end(), ""), v7.end());
							v8.erase(std::remove(v8.begin(), v8.end(), ""), v8.end());

							std::vector<double> re_1, re_2, re_3, re_4, re_5, re_6, re_7, re_8;
							std::for_each(v1.begin(), v1.end(), [&](std::string str){
								re_1.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v2.begin(), v2.end(), [&](std::string str){
								re_2.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v3.begin(), v3.end(), [&](std::string str){
								re_3.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v4.begin(), v4.end(), [&](std::string str){
								re_4.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v5.begin(), v5.end(), [&](std::string str){
								re_5.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v6.begin(), v6.end(), [&](std::string str){
								re_6.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v7.begin(), v7.end(), [&](std::string str){
								re_7.push_back(boost::lexical_cast<double>(str));
							});
							std::for_each(v8.begin(), v8.end(), [&](std::string str){
								re_8.push_back(boost::lexical_cast<double>(str));
							});

							push_back(boost::lexical_cast<int>(temp_v.at(0)), boost::lexical_cast<int>(temp_v.at(1)),
								re_1, re_2, re_3, re_4, re_5, re_6, re_7, re_8);
						}
						catch (...){
							continue;
						}
					}
				}
			}
			inFile.close();
		}

		friend std::ostream& operator<<(std::ostream& os, HMatrix& m){
			if (m.getData().empty())
				return os;
			os << "[row:" << m.m_data.size() << "\tcolum:" << 6 << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](tuple_data data){
				os.setf(std::ios_base::left, std::ios_base::adjustfield);
				os << std::get<0>(data);
				os << "\t";
				os << std::get<1>(data);
				os << "\t";
				os << std::get<2>(data);
				os << "\t";
				os << std::get<3>(data);
				os << "\t";
				os << std::get<4>(data);
				os << "\t";
				os << std::get<5>(data) << std::endl;
			});
			return os;
		}

	private:
		std::vector<tuple_data>  m_data;
	};

}
