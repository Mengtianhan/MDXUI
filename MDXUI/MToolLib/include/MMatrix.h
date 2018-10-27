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

#pragma warning(disable:4996)
#pragma once
#include <tuple>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <exception>
#include <typeinfo>
#include <type_traits>
#include <regex>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
//boost
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

/************************************************************************
*	HMatrix可以将10种不同类型的数据储存起来，每一行的类型相同			*
*   HMatrix_1只能够储存一种类型的矩阵结构，但有N限制列数				*
*	Matrix同HMatrix_1一样，只能储存一种类型，但是不限制列数				*
*   三个矩阵类的方法都一样(public接口)									*
*   getData()获取矩阵中所有的数据，保存在一个vector容器中				*
*   push_back()将数据作为一行压入矩阵，对于HMatrix来说这里的参数必须和	*
*				声明的类型一直而且参数的数目也必须和制定的类型一样多	*
*	push_back()对于HMatrix_1和Matrix来说是个变参模板，可以任意传多少	*
*				都行，如果少于HMatrix_1指定数目，将按指定类型的默认值	*
*				补齐，如果超过指定数目，将自动截取前面的N个				*
*	push_back()对于Matrix来说给多少参数都直接压入矩阵，这是一个不严格的	*
*				矩阵，因为他的每一个的数目都可能不相同					*
*	clear()清除矩阵中所有的数据											*
*   empty()查看矩阵是否为空												*
*	colum()获取矩阵的列数，对于Matrix来说只获取第一行的行数				*
*   getColumdata()获取指定列的数据，以vector的形式返回					*
*	getColumdata()对于HMatrix来说需要指定返回列的类型和，例如：			*
*		HMatrix<double,string,int,float> m;								*
*		m.fillHMatrixFromFile("data.txt");//用文件数据初始化			*
*		vector<string> data = m.getColumdata<string,1>();				*
*		cout<<m<<endl;													*
*		cout<<data<<endl;这里提供流对矩阵和vector的操作					*
*	getColumdata()对于HMatrix_1来说只需要指定返回的列数就好，例如:		*
*		HMatrix_1<double,5>	m;											*
*		m.push_back(10,20,30,40,50,60);									*
*		m.push_back(20,40,80);											*
*		m.push_back(45,50,45,60,31,85,96,20,14);						*
*		vector<double> data = m.getColumdata(1);						*
*		cout<<data<<endl;//20 40 50										*
*	getColumdata()对于Matrix的用法和HMatrix_1一样						*
*	at()获取具体位置的元素，对于HMatrix来说，需要指定元素类型和列，如下:*
*		HMatrix<double,string,int,float> m;								*
*		m.fillHMatrixFromFile("data.txt");//用文件数据初始化			*
*		string data = m.at<string,0>(2);//2行1列的数据，类型为string	*
*	at()对于HMatrix和Matrix来说用法很简单，只需要指明行号和列号即可：	*
*		HMatrix_1<double,5>	m;											*
*		m.push_back(10,20,30,40,50,60);									*
*		m.push_back(20,40,80);											*
*		m.push_back(45,50,45,60,31,85,96,20,14);						*
*		double data = m.at(0,0); //10									*
*	fillHMatrixFromFile()HMatrix专用，参数为文件名和一个正则表达式		*
*		将文件里面的内容，填充进HMatrix中								*
*	fillMatrixFromFile()非成员函数，用于对Matrix的操作，参数为一个文件名*
*		和一个Matrix的引用和一个正则表达式，将文件中数据填充Matrix对象	*
*	由于HMatrix和Matrix都对流的operator<<进行了重载，所以可以很方便的	*
*		将HMatrix和Matrix写进文件中,例如：								*
*		HMatrix<int, int, std::string,std::string> m;					*
*		m.fillHMatrixFromFile("hello.txt");								*
*		cout << m << endl;	//使用标准输出								*
*		cout << m.at<string, 2>(0) << endl;	//获取1行3列的string		*
*		ofstream outFile("test.txt");									*
*		outFile<<m<<endl;//将m写进test的文件里							*
*		hello.txt的内容如下:											*
*			[only test]	//用于匹配，完全匹配后才会读取下面的数据		*
*			10 20 hello world											*
*			20 20 nihao hello											*
*			50 60 bush  haha											*
*			20 46 buha  nihao											*
*	针对HMatrix有个不同的全特化版本，主要是根据特殊用途					*
*   HContainer一个变参模板容器，可以容纳任何数量任何类型的数据			*
*   几个数据结构的用法都差不多，功能也差点，从储存来说HContainer最为强大*
*		但是HContainer的缺点是不能从文件读取数据来初始化.....			*
*************************************************************************/

//===========================================
//将数列相加
//===========================================
template<class T>
std::vector<T> plus(std::vector<T>& v1, std::vector<T>& v2){
	std::vector<T> result;
	if (v1.size() != v2.size()){
		return result;
	}
	int count = v1.size();
	for (int i = 0; i < count; ++i){
		result.push_back(v1.at(i) + v2.at(i));
	}
	return result;
}

//=============================================
//两数列相减
//=============================================
template<class T>
std::vector<T> minus(std::vector<T>& v1, std::vector<T>& v2){
	std::vector<T> result;
	if (v1.size() != v2.size()){
		return result;
	}
	int count = v1.size();
	for (int i = 0; i < count; ++i){
		result.push_back(v1.at(i) - v2.at(i));
	}
	return result;
}

//===============================================
//另外两种模式
//===============================================

//====================================================
//两系列相加
//不只是局限于vector
//====================================================
template<typename T, typename AL,
	template<typename U, typename A> class C>
C<T, AL> plus(C<T, AL>& v1, C<T, AL>& v2){
	C<T, AL> result;
	if (v1.size() != v2.size()){
		return result;
	}

	std::transform(v1.begin(), v1.end(), v2.begin(), std::back_inserter(result), [&](T t1, T t2)->T{return t1 + t2; });

	return result;
}

//====================================================
//两系列相减
//不只是局限于vector
//====================================================
template<typename T, typename AL,
	template<typename U, typename A> class C>
C<T, AL> minus(C<T, AL>& v1, C<T, AL>& v2){
	C<T, AL> result;
	if (v1.size() != v2.size()){
		return result;
	}
	std::transform(v1.begin(), v1.end(), v2.begin(), std::back_inserter(result), [&](T t1, T t2)->T{return t1 - t2; });
	return result;
}

//====================================================================
//采用流式方式对一些容器的简单操作
//====================================================================
template<class T>
inline std::vector<T>& operator<<(std::vector<T>& v, const T& value){
	v.push_back(value);
	return v;
}

template<class T>
inline std::vector<T>& operator>>(std::vector<T>& v, T& value){
	if (v.empty()){
		value = T();
	}
	else{
		value = v.back();
		v.pop_back();
	}
	return v;
}

template<class T>
inline std::list<T>& operator<<(std::list<T>& l, const T& value){
	l.push_back(value);
	return l;
}

template<class T>
inline std::list<T>& operator>>(std::list<T>& l, T& value){
	if (l.empty()){
		value = T();
	}
	else{
		value = l.back();
		l.pop_back();
	}
	return l;
}

template<class T>
inline std::deque<T>& operator<<(std::deque<T>& d, const T& value){
	d.push_back(value);
	return d;
}

template<class T>
inline std::deque<T>& operator>>(std::deque<T>& d, T& value){
	if (d.empty()){
		value = T();
	}
	else{
		value = d.back();
		d.pop_back();
	}
	return d;
}

template<class T>
inline std::set<T>& operator<<(std::set<T>& s, const T& value){
	s.insert(value);
	return s;
}

template<class T>
inline std::set<T>& operator>>(std::set<T>& s, T& value){
	if (s.empty()){
		value = T();
	}
	else{
		auto it = --s.end();
		value = *it;
		s.erase(it);
	}
	return s;
}

template<class Key, class V>
inline std::map<Key, V>& operator<<(std::map<Key, V>& m, const std::pair<Key, V>& value){
	m[value.first] = value.second;
	return m;
}

template<class Key, class V>
inline std::map<Key, V>& operator>>(std::map<Key, V>& m, std::pair<Key, V>& value){
	if (m.empty()){
		value = std::make_pair(Key(), V());
	}
	else{
		auto it = --m.end();
		value = std::make_pair(it->first, it->second);
		m.erase(it);
	}
	return m;
}

//-----------------------------------------------------------------------
namespace hlw{

	template<class T>
	struct is_base_class{
		operator bool(){
			if (std::is_integral<T>::value)
				return true;
			else if (std::is_same<T, std::string>::value)
				return true;
			else if (std::is_same<T, double>::value)
				return true;
			else if (std::is_same<T, float>::value)
				return true;
			else if (std::is_same<T, short>::value)
				return true;
			else if (std::is_same<T, char>::value)
				return true;
			else
				return false;
		}
	};

	template<class C>
	struct is_StdContainer{
		template<class T>
		struct value{
			operator bool(){
				if (std::is_same<C, std::vector<T>>::value){
					return true;
				}
				else if (std::is_same<C, std::list<T>>::value){
					return true;
				}
				else if (std::is_same<C, std::deque<T>>::value){
					return true;
				}
				else{
					return false;
				}
			}
		};
	};

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


	//----------------------------------------------------------------------------
	// 将标准容器配合流操作
	//----------------------------------------------------------------------------
	template<class T>
	inline std::ostream& operator<<(std::ostream& os, std::vector<T>& v){
		if (v.empty())
			return os;
		std::copy(v.begin(), v.end() - 1, std::ostream_iterator<T>(os, ","));
		os << *(v.end() - 1);
		return os;
	}

	template<class T>
	inline std::ostream& operator<<(std::ostream& os, const std::list<T>& l){
		if (l.empty())
			return os;
		std::copy(l.begin(), --l.end(), std::ostream_iterator<T>(os, ","));
		os << *(--l.end());
		return os;
	}

	template<class T>
	inline std::ostream& operator<<(std::ostream& os, const std::deque<T>& d){
		if (d.empty())
			return os;
		std::copy(d.begin(), d.end() - 1, std::ostream_iterator<T>(os, ","));
		os << *(d.end() - 1);
		return os;
	}

	template<class T>
	inline std::ostream& operator<<(std::ostream& os, const std::set<T>& s){
		if (s.empty())
			return os;
		std::copy(s.begin(), s.end(), std::ostream_iterator<T>(os, ","));
		return os;
	}

	template<class T>
	inline std::istream& operator>>(std::ostream& is, std::set<T>& s){
		s.clear();
		std::vector<T> temp((std::istream_iterator<T>(is)), std::istream_iterator<T>());
		for (auto& t : temp){
			s.insert(t);
		}
		return os;
	}

	template<class T>
	inline std::ostream& operator<<(std::ostream& os, const std::multiset<T>& s){
		if (s.empty())
			return os;
		std::copy(s.begin(), s.end(), std::ostream_iterator<T>(os, ","));
		return os;
	}

	template<class T>
	inline std::istream& operator>>(std::ostream& is, std::multiset<T>& s){
		s.clear();
		std::vector<T> temp((std::istream_iterator<T>(is)), std::istream_iterator<T>());
		for (auto& t : temp){
			s.insert(t);
		}
		return os;
	}

	template<class T>
	inline std::ostream& operator<<(std::ostream& os, const std::unordered_set<T>& s){
		if (s.empty())
			return os;
		std::copy(s.begin(), s.end(), std::ostream_iterator<T>(os, ","));
		return os;
	}

	template<class T>
	inline std::istream& operator>>(std::ostream& is, std::unordered_set<T>& s){
		s.clear();
		std::vector<T> temp((std::istream_iterator<T>(is)), std::istream_iterator<T>());
		for (auto& t : temp){
			s.insert(t);
		}
		return os;
	}

	template<class K, class V>
	inline std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m){
		if (m.empty())
			return os;
		std::for_each(m.begin(), m.end(), [&](std::pair<K, V> p){
			os << p.first << "," << p.second << ",";
		});
		return os;
	}

	template<class K, class V>
	inline std::ostream& operator<<(std::ostream& os, const std::multimap<K, V>& m){
		if (m.empty())
			return os;
		std::for_each(m.begin(), m.end(), [&](std::pair<K, V> p){
			os << p.first << "," << p.second << ",";
		});
		return os;
	}

	template<class K, class V>
	inline std::ostream& operator<<(std::ostream& os, const std::unordered_map<K, V>& m){
		if (m.empty())
			return os;
		std::for_each(m.begin(), m.end(), [&](std::pair<K, V> p){
			os << p.first << "," << p.second << ",";
		});
		return os;
	}

	template<class K, class V>
	inline std::ostream& operator<<(std::ostream& os, const std::unordered_multimap<K, V>& m){
		if (m.empty())
			return os;
		std::for_each(m.begin(), m.end(), [&](std::pair<K, V> p){
			os << p.first << "," << p.second << ",";
		});
		return os;
	}

	template<class T>
	inline std::ostream& operator<<(std::ostream& os, const std::unordered_multiset<T>& s){
		if (s.empty())
			return os;
		std::copy(s.begin(), s.end(), std::ostream_iterator<T>(os, ","));
		return os;
	}


	//===============================================================
	//打印vector的vector
	//===============================================================
	template<class T>
	inline std::ostream& operator<<(std::ostream& os, std::vector<std::vector<T>>& c){
		std::for_each(c.begin(), c.end(), [&](std::vector<T>& i_c){
			os << i_c << std::endl;
		});
		return os;
	}



	//==================================================
	//========HMatrix===================================
	struct null{};

	template<class T0 = null, class T1 = null, class T2 = null, class T3 = null, class T4 = null, class T5 = null,
	class T6 = null, class T7 = null, class T8 = null, class T9 = null>
	class HMatrix
	{

		typedef std::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> tuple_data;

	public:
		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		typedef typename std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

		std::vector<tuple_data>& getData(){
			return m_data;
		}

		const std::vector<tuple_data>& getData() const {
			return m_data;
		}

		void push_pack(T0 value0, T1 value1, T2 value2, T3 value3, T4 value4,
			T5 value5, T6 value6, T7 value7, T8 value8, T9 value9){
			m_data.push_back(std::make_tuple(value0, value1, value2, value3, value4, value5, value6, value7, value8, value9));
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
		std::vector<T> getColumdata(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
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
		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		typedef typename std::vector<T>::iterator iterator;
		typedef T value_type;

		std::vector<T>& getData(){
			return m_data;
		}

		const std::vector<T>& getData() const {
			return m_data;
		}

		void push_back(T value){
			m_data.push_back(value);
		}

		T get_element(value_type v){
			return v;
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
		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		typedef typename std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

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
		std::vector<T> getColumdata(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
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
		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		typedef typename std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;


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
		std::vector<T> getColumdata(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
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
		HMatrix(){ m_data.clear(); }
		~HMatrix(){ m_data.clear(); }

		typedef typename std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;


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
		std::vector<T> getColumdata(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
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
		HMatrix(){ m_data.clear(); }
		~HMatrix(){ m_data.clear(); }

		typedef std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

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
		std::vector<T> getColumdata(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
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
		HMatrix(){ m_data.clear(); }
		~HMatrix(){ m_data.clear(); }

		typedef  std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

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
		std::vector<T> getColumdata(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
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
		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		typedef typename std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

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
		std::vector<T> getColumdata(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
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
		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		typedef typename std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

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
		std::vector<T> getColumdata(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
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
		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		typedef  std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

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
		std::vector<T> getColumdata(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
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
		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		typedef  std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

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
		std::vector<T> getColumdata(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
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
		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		typedef typename std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

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
		std::vector<T> getColumdata(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
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
		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		typedef typename std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

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
		std::vector<T> getColumdata(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
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
		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		typedef typename std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

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
		std::vector<T> getColumdata(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
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
		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		typedef  std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

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
		std::vector<T> getColumdata(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
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
		HMatrix(){ m_data.clear(); }
		~HMatrix(){}

		typedef  std::vector<tuple_data>::iterator iterator;
		typedef tuple_data value_type;

		std::vector<tuple_data>& getData(){
			return m_data;
		}

		const std::vector<tuple_data>& getData() const {
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
		std::vector<T> getColumdata(){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				result.push_back(std::get<N>(data));
			});
			return result;
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

	//===========================================================================
	//===================================HMatrix_1===============================
	//===========================================================================
	template<class T, int N>
	class HMatrix_1{
		typedef std::vector<T> vector_data;
	public:
		HMatrix_1(){}
		~HMatrix_1(){}

		typedef vector_data value_type;

		std::vector<vector_data>& getData(){
			return m_data;
		}

		const std::vector<vector_data>& getData() const{
			return m_data;
		}

		unsigned colum(){
			if (m_data.empty())
				return 0;
			return N;
		}

		unsigned row(){
			return m_data.size();
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		std::vector<T> getColumdata(unsigned colum){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](vector_data data){
				result.push_back(data.at(colum));
			});
			return result;
		}

		T at(unsigned row, unsigned colum){
			if (colum >= N)
				throw std::runtime_error("colum index beyong range......");
			if (row >= m_data.size())
				throw std::runtime_error("row index beyong range......");
			return m_data.at(row).at(colum);
		}

		void push_back(std::vector<T>& data){
			if (data.size() > N){
				data.resize(N);
			}
			else{
				for (int i = data.size(); i < N; ++i)
					data.push_back(T());
			}
			m_data.push_back(data);
		}

		template<typename U, typename... Args>
		void push_back(U value, Args ... args){
			push_back(value);
			push_back(args...);

			if (m_temp_colum.empty())
				return;
			if (m_temp_colum.size() < N){
				for (int i = m_temp_colum.size(); i < N; ++i)
					m_temp_colum.push_back(U());
			}
			else{
				m_temp_colum.resize(N);
			}
			m_data.push_back(m_temp_colum);
			m_temp_colum.clear();
		}

		void push_back(value_type value){
			m_data.push_back(value);
		}

		template<typename U, typename... Args>
		static value_type make_value(U value, Args ... args){
			push_back(U value, Args ... args);
			value_type v = m_data.back();
			m_data.pop_back();
			return v;
		}

		T get_element(value_type v, unsigned n){
			if (n >= N){
				throw std::runtime_error("index out of arange");
			}
			return v.at(n);
		}

		friend std::ostream& operator<<(std::ostream& os, const HMatrix_1& m){
			if (m.m_data.empty())
				return os;
			os << "[row:" << m.m_data.size() << "\t" << "colum:" << N << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](vector_data data){
				std::copy(data.begin(), data.end() - 1, std::ostream_iterator<T>(os, " \t"));
				os << data.at(data.size() - 1) << std::endl;
			});
			return os;
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
			std::vector<T>	result_v;
			while (std::getline(inFile, read_str)){
				if (read_str.empty())
					continue;
				if (b_is_empty){
					temp_v.clear();
					result_v.clear();
					boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
					try{
						auto it = temp_v.begin();
						if (!std::is_same<T, std::string>::value){
							temp_v.erase(std::remove_if(temp_v.begin(), temp_v.end(), [](std::string str){
								return str.empty(); }), temp_v.end());
						}

						while (it != temp_v.end()){
							boost::trim(*it);
							T temp = boost::lexical_cast<T>(*it);
							result_v.push_back(temp);
							++it;
						}
						push_back(result_v);
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
						result_v.clear();
						boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
						try{
							auto it = temp_v.begin();
							if (!std::is_same<T, std::string>::value)
								temp_v.erase(std::remove_if(temp_v.begin(), temp_v.end(), [](std::string str){
								return str.empty(); }), temp_v.end());

								while (it != temp_v.end()){
									boost::trim(*it);
									T temp = boost::lexical_cast<T>(*it);
									result_v.push_back(temp);
									++it;
								}
								push_back(result_v);
						}
						catch (...){
							continue;
						}
					}
				}
			}
			inFile.close();
		}

	private:
		void push_back(){}
		template<class U>
		void push_back(U value){
			m_temp_colum.push_back(value);
		}
		std::vector<vector_data> m_data;
		vector_data				 m_temp_colum;

	};

	//===========================================================================
	//==================================Matrix===================================
	//===========================================================================
	template<class T>
	class Matrix{
		typedef std::vector<T> vector_data;
	public:

		typedef vector_data value_type;
		Matrix(){
			m_data.clear();
			m_temp_colum.clear();
		}

		std::vector<vector_data>& getData(){
			return m_data;
		}

		const std::vector<vector_data>& getData() const{
			return m_data;
		}

		unsigned colum(unsigned row){
			if (m_data.empty())
				return 0;
			return m_data.at(row).size();
		}

		unsigned colum(){
			if (m_data.empty())
				return 0;
			return m_data.at(0).size();
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		size_t size(){
			return m_data.size();
		}

		T at(unsigned row, unsigned colum){
			if (row >= m_data.size())
				throw std::runtime_error("row index beyong range!!!!");
			if (colum >= m_data.at(row).size())
				throw std::runtime_error("colum index beyong range!!!");
			return m_data.at(row).at(colum);
		}


		std::vector<T> getColumdata(unsigned colum){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](vector_data data){
				if (data.size() <= colum)
					result.push_back(T());
				else
					result.push_back(data.at(colum));
			});
			return result;
		}

		std::vector<T> getRowData(unsigned row){
			std::vector<T> result;
			if (row >= m_data.size()){
				throw std::runtime_error("row index beyong range!!!!!");
			}

			result = m_data.at(row);
			return result;
		}

		void push_back(std::vector<T>& data){
			m_data.push_back(data);
		}

		//---------------------------------------------
		// 翻转矩阵
		// 将行变成列，将列编程行
		//---------------------------------------------
		Matrix<T> flip(){

			Matrix<T> result;

			if (m_data.empty()){
				return result;
			}

			//===================================
			//检查矩阵所有行数的列数是否相等
			//===================================
			size_t count = m_data.begin()->size();
			for (auto& it : m_data){
				if (count != it.size()){
					return result;
				}
			}

			for (int i = 0; i < count; ++i){
				result.push_back(getColumdata(i));
			}

			return result;
		}


		friend std::ostream& operator<<(std::ostream& os, const Matrix& m){
			if (m.m_data.empty())
				return os;
			os << "[row:" << m.m_data.size() << "\t" << "first colum:" << m.m_data.at(0).size() << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](vector_data data){
				std::copy(data.begin(), data.end() - 1, std::ostream_iterator<T>(os, " \t"));
				os << data.at(data.size() - 1) << std::endl;
			});
			return os;
		}


	private:
		std::vector<vector_data> m_data;
		vector_data				 m_temp_colum;
	};


	//=========================================================
	//通过读取文件数据来填充matrix
	//=========================================================
	template<class T>
	inline void fillMatrixFromFile(const std::string& filename, Matrix<T>& m, const std::string& startline = "(\\[.{1,2000}\\])"){
		std::ifstream inFile(filename);
		if (inFile.fail()){
			throw std::runtime_error("open file for read fail!!!");
		}
		bool b_is_empty = startline.empty();
		std::string read_str;
		std::regex  reg(startline);
		std::vector<std::string> temp_v;
		std::vector<T>           result_v;
		while (std::getline(inFile, read_str)){
			if (read_str.empty())
				continue;
			if (b_is_empty){
				temp_v.clear();
				result_v.clear();
				boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
				if (!(std::is_same<T, std::string>::value))
					temp_v.erase(std::remove(temp_v.begin(), temp_v.end(), ""), temp_v.end());
				auto it = temp_v.begin();
				while (it != temp_v.end()){
					boost::trim(*it);
					T temp = boost::lexical_cast<T>(*it);
					result_v.push_back(temp);
					++it;
				}
				m.push_back(result_v);
			}
			else if (std::regex_match(read_str, reg)){
				while (std::getline(inFile, read_str)){
					if (read_str.empty())
						continue;
					temp_v.clear();
					result_v.clear();
					boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
					if (!(std::is_same<T, std::string>::value))
						temp_v.erase(std::remove(temp_v.begin(), temp_v.end(), ""), temp_v.end());
					auto it = temp_v.begin();
					while (it != temp_v.end()){
						boost::trim(*it);
						T temp = boost::lexical_cast<T>(*it);
						result_v.push_back(temp);
						++it;
					}
					m.push_back(result_v);
				}
			}
		}
		inFile.close();
	}
}
