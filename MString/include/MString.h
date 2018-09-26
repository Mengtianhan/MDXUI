#pragma once
#include "StringImpl.h"
#include <string>
#include <sstream>
#include <iostream>
#include <regex>
#include "MStringIter.h"
#include <mutex>
#include <memory>
#include <fstream>

#ifdef __QT__
#include <QString>
#include <QStringList>
#include <QDataStream>
#endif



//
// 整个MString的编码为GBK
// 如果在utf8环境下面需要设置编码方式 setCharacterFormat(MGBK)
//

//
// 整套MString不是异常安全的，所有异常都是std::runtime_error 异常
// 所以在具体使用中注意异常的捕获
//

/***********************************************
* 线程安全:
*************************************************/
namespace mj{

	///
	/// \brief The MBuffer struct
	/// 定义一个标识buffer的东西
	///
	struct MBuffer{
		char*  mBuffer{ nullptr };
		uint64 mLength{ 0ull };

		/*
		*
		* buffer的初始化分配或者什么的都最好使用下面的成员函数来完成，否则资源自己负责
		*
		*/
		bool InitBuffer(uint64 size);
		bool Attach(const char* buffer, uint64 size);
		bool DestroyBuffer();
	};

	//
	// 定义几个和数字相关的常用正则表达式
	// REGFORFLOAT 浮点数
	// REGFORIP IP地址
	// REFFORTEL 电话号码
	// REGFORIDENTITY 18位身份证
	//
	static const char* REGFORFLOAT = "(-?\\d+\\.?\\d*e?E?[+-]?\\d*)";
	static const char* REGFORIP = "((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)";
	static const char* REFFORTEL = "(0{0,1}(13[4-9]|15[7-9]|15[0-2]|18[7-8])[0-9]{8})";
	static const char* REGFORIDENTITY = "([1-9]\\d{5}[1-9]\\d{3}((0\\d)|(1[0-2]))(([0|1|2]\\d)|3[0-1])\\d{4})";


	/**********************************************
	* 下面的定义主要是为了处理模式替换
	* 如果被替换的字符串含有 "*&%*@*" 的时候
	* 那么在替换之前请先将 字符串 替换为其他的
	* 替换完成后再将该字符串替换回来
	***********************************************/
#ifndef __SPLIT__
#undef __SPLIT__
#define __SPLIT__ "^"
#endif 

	class  MString
	{
	public:
		//=======================================
		// 代理class
		// 实现真正的写时复制
		//========================================
		class  MCharProxy{
		public:
			MCharProxy(MString& str, size_t index);
			MCharProxy& operator=(const MCharProxy& r);
			MCharProxy& operator=(char c);

			operator char()const;
			operator char();

			char* operator&();
			const std::string& operator&() const;

		private:
			MString& mStr;
			size_t mIndex;
		}; //class MCharProxy

		//==============================================
		// 让代理类成为友员类
		// 以便能够访问私有数据
		//==============================================
		friend class MCharProxy;

		//
		// 配合迭代器使用的typedef
		//
		typedef MString type;
		typedef const MString const_type;
		typedef MIterator<MString> iterator;
		typedef ConstMIterator<MString> const_iterator;
		typedef std::random_access_iterator_tag iterator_category;
		typedef MCharProxy value_type;
		typedef /*char*/std::ptrdiff_t difference_type;
		typedef /*char*/std::ptrdiff_t distance_type;
		typedef MCharProxy* pointer;
		typedef MCharProxy reference; //这里并非真正的引用，而只是代理引用

		//
		// 查找无效返回值
		//
		static uint64 npos;

		//
		// 浮点数的表现形式
		//
		enum FLOATFORMAT{
			FIXED,      // 使用定点格式
			SCIENTIFIC  // 使用科学计数法格式
		};

		//
		// 字符编码方式 ,MString储存字符串按照原始编码储存,就是给的是什么编码就是什么编码
		// 为了让MString自己知道自己是什么编码，需要手动指定
		// 如果当前属于UTF8编码需要手动设置编码为UTF8
		//
		enum MCharacterCode{
			MGBK = 0,    // gbk方式
			MUTF8		 // utf8 方式
		};

		//
		// 设置浮点数的转换格式
		//
		static void setFloatFormat(FLOATFORMAT format);
		static void setFloatPrecision(int precision);
		static void setCharacterFormat(MCharacterCode code);

		//
		// 该函数将utf8转换程gbk，如果当前编码本身就是gbk那么原样返回
		//
		static MString GBKString(const MString& str);

		//
		// 检查文本是否属于utf8编码
		//
		static bool FileIsUtf8Code(const std::string& fileName);

		//
		// 该文本必须是带完整头部信息的字符串，否则无法判断
		//
		static bool TextIsUtf8Code(const std::string& Text);

		//
		// 不需要是带完整头部信息的字符串判断是否为utf8，但有可能不准确
		// 所以MString的构造赋值的判断只对带有完整信息的字符串进行判断
		// 提供此函数进行额外的辅助判断
		//
		static bool TextIsUtf8Code(const std::string& Text, __int64 len);

		//
		// 宽字节和多字节的转换
		//
		static std::string w2c(const std::wstring& wstr);
		static std::wstring c2w(const std::string& str);


		MString(const char* str = "");
		MString(const std::string& str);
		MString(const wchar_t* str);
		MString(const std::wstring& str);
		MString(const char* buf, uint64 size);
		MString(char value);
		MString& fromFile(const MString& FileName);
		MString& fromFile(const MString &FileName, uint64 pos, uint64 len = 0ull);
		MString& fromUtf8File(const MString& utf8FileName);
		MString(MString&& other);
		MString& operator=(MString&& other);

		~MString();


		//
		// 泛型操作
		//
		template<class T>
		MString& operator+=(const T& value){
			return private_append(value);
		}

		template<class T>
		MString& push_back(const T& value){
			return private_append(value);
		}

		template<class T>
		MString& append(const T& value){
			return private_append(value);
		}

		template<class T>
		MString& operator<<(const T& value){
			return private_append(value);
		}

		//
		// 因为效率和常用性考虑所以这几个常用函数做特殊处理
		//
		MString& operator+=(const std::string& value);
		MString& operator+=(const std::wstring& value);
		MString& operator+=(const char* value);
		MString& operator+=(const wchar_t* value);

		MString& push_back(const std::string& value);
		MString& push_back(const std::wstring& value);
		MString& push_back(const char* value);
		MString& push_back(const wchar_t* value);
		MString& push_back(char* buf, uint64 size);

		MString& append(const std::string& value);
		MString& append(const std::wstring& value);
		MString& append(const char* value);
		MString& append(const wchar_t* value);
		MString& append(const char* buf, uint64 size);


		MString& operator<<(const std::string& value);
		MString& operator<<(const char* value);
		MString& operator<<(const std::wstring& value);
		MString& operator<<(const wchar_t* value);


		size_t find(const MString& value);
		size_t find(const MString& value) const;
		size_t find(const MString& value, unsigned offset);
		size_t find(const MString& value, unsigned offset) const;

		bool contains(const MString& value) const;
		bool contains(const MString& value);

		size_t find_first(const MString& value);
		size_t find_first(const MString& value) const;
		size_t find_last(const MString& value);
		size_t find_last(const MString& value) const;

		size_t find_nth(const MString& value, size_t index);
		size_t find_nth(const MString& value, size_t index) const;
		size_t find_count(const MString& value);
		size_t find_count(const MString& value) const;

		//
		// 替换函数簇
		//
		MString& replace_first(const MString& replaced, const MString& value);
		MString& replace_all(const MString& replaced, const MString& value);
		MString& replace_nth(const MString& replaced, const MString& value, size_t index);
		MString& replace_last(const MString& replaced, const MString& value);

		//
		// 替换拷贝
		// 不修改原始数值
		//
		MString replace_first_copy(const MString& replaced, const MString& value);
		MString replace_first_copy(const MString& replaced, const MString& value) const;
		MString replace_all_copy(const MString& replaced, const MString& value);
		MString replace_all_copy(const MString& replaced, const MString& value) const;
		MString replace_nth_copy(const MString& replaced, const MString& value, size_t index);
		MString replace_nth_copy(const MString& replaced, const MString& value, size_t index) const;
		MString replace_last_copy(const MString& replaced, const MString& value);
		MString replace_last_copy(const MString& replaced, const MString& value) const;

		//
		// 删除函数簇
		//
		MString& erase_first(const MString& erase);
		MString& erase_all(const MString& erase);
		MString& erase_nth(const MString& erase, size_t index);
		MString& erase_last(const MString& erase);

		//
		// 只对备份执行删除操作
		//
		MString erase_first_copy(const MString& erase);
		MString erase_all_copy(const MString& erase);
		MString erase_nth_copy(const MString& erase, size_t index);
		MString erase_last_copy(const MString& erase);
		MString erase_first_copy(const MString& erase) const;
		MString erase_all_copy(const MString& erase) const;
		MString erase_nth_copy(const MString& erase, size_t index) const;
		MString erase_last_copy(const MString& erase) const;

		//
		// 和标准库string的隐式交互
		//
		operator std::string() const;
		operator std::string();
		operator std::wstring() const;
		operator std::wstring();

		std::string toStdString();
		std::string toStdString() const;

		std::wstring toStdWString();
		std::wstring toStdWString() const;

		//
		// 查询是否相等，i开头的是不区分大小写
		//
		bool iequal(const MString& other);
		bool iequal(const MString& other) const;
		bool equal(const MString& other);
		bool equal(const MString& other) const;

		//
		// 查询是否有包含，i开头的是不区分大小写
		//
		bool icontains(const MString& other) const;

		//
		// 对正则表达式的支持
		// reg_match 要求全匹配
		// reg_serch 只要有包含即可
		//
		bool reg_match(const MString& reg);
		bool reg_serch(const MString& reg);

		bool reg_match(const MString& reg) const;
		bool reg_serch(const MString& reg) const;

		//
		// 使用正则表达式进行替换
		//
		MString& reg_replace(const MString& reg, const MString& fomart);

		//
		// 使用正则表达式进行副本替换
		//
		MString reg_replace_copy(const MString& reg, const MString& fomart);
		MString reg_replace_copy(const MString& reg, const MString& fomart) const;

		//
		// 清空字符串
		//
		void clear();

		//
		// 获取字符串的长度
		//
		size_t size();
		const size_t size() const;

		size_t length();
		const size_t length() const;

		//
		// 查询字符串是否共享
		//
		bool isshared();
		bool isshared() const;



		//
		// 查询该字符串是否可共享
		//
		bool isshareable();
		bool isshareable() const;

		//
		// 标记为不共享
		//
		void markunshare();
		void markunshare() const;

		//
		// 是否为数字
		//
		bool isdigit();
		bool isdigit() const;

		//
		// 将文本写进文件
		//
		bool toFile(const MString& fileName);
		bool toFile(const MString& fileName) const;

		//
		// 使用代理有图的好处自然也有他的不好之处
		//
		const MCharProxy operator[](size_t index) const;
		MCharProxy operator[](size_t index);

		//
		// 转换为c字符串
		//
		const char* c_str() const;
		const char* c_str();

		//
		// 转换为c宽字符串
		//
		const wchar_t* c_wstr() const;
		const wchar_t* c_wstr();

		//
		// 查询是否为空
		//
		bool empty();
		bool empty() const;

		//
		// 去掉字符串两端的空白
		//
		MString& trim();
		MString& trim_left();
		MString& trim_right();

		//
		// 获取数据地址
		//
		const void* GetAddr() const;
		void* GetAddr();

		//
		// 获取引用计数
		//
		size_t reference_count() const;

		//
		// 迭代器操作
		//
		iterator begin();
		iterator end();

		const_iterator begin() const;
		const_iterator end() const;

		//
		// 比较操作,i开头的是不区分大小写
		//
		bool end_with(const MString& str);
		bool iend_with(const MString& str);
		bool start_with(const MString& str);
		bool istart_with(const MString& str);

		bool end_with(const MString& str) const;
		bool iend_with(const MString& str) const;
		bool start_with(const MString& str) const;
		bool istart_with(const MString& str) const;


		//
		// 大小写转换操作
		//
		MString& to_upper();
		MString to_upper_copy();
		MString to_upper_copy() const;

		MString& to_lower();
		MString to_lower_copy();
		MString to_lower_copy() const;

		//
		// 交换两个字符串
		//
		void swap(MString& other);


		bool operator==(const MString& other);
		bool operator!=(const MString& other);
		bool operator<(const MString& other);
		bool operator>(const MString& other);

		friend bool operator==(const MString& Ldata, const MString& Rdata){
			return const_cast<MString&>(Ldata).operator==(Rdata);
		}

		friend bool operator!=(const MString& Ldata, const MString& Rdata){
			return const_cast<MString&>(Ldata).operator!=(Rdata);
		}

		friend bool operator<(const MString& Ldata, const MString& Rdata){
			return const_cast<MString&>(Ldata).operator<(Rdata);
		}

		friend bool operator>(const MString& Ldata, const MString& Rdata){
			return const_cast<MString&>(Ldata).operator>(Rdata);
		}

		//
		// 获取一个副本，该副本不共享
		//
		MString copy();

		//
		// 转换为utf8字符串
		//
		MString to_utf8();
		MString to_utf8() const;

		//
		// 转换为char
		//
		char toChar();
		char toChar() const;

		//
		// 快速分割字符串
		//
		void split(const MString& isanyof, std::vector<std::string>& result,bool isDiscardEmpty = true);



		//
		// 下面的操作都是用模板来操作，实现MString的泛型化
		//

		//
		// 泛型构造函数，由于流无法判断原始类型，所以不针对流进行编码转换
		//
		template<class iter>
		MString(iter beg, iter end){
			int len = std::distance(beg, end);
			Init(&(*beg), len);
		}

		template<>
		MString(char* beg, char* end)
		{
			std::string str(beg, end);
			if (TextIsUtf8Code(str)){
				str = utf8_gbk(str);
			}
			Init(str.c_str());
		}

		template<>
		MString(std::istreambuf_iterator<char> beg, std::istreambuf_iterator<char> end)
		{
			std::string str(beg, end);
			if (TextIsUtf8Code(str)){
				str = utf8_gbk(str);
			}
			Init(str.c_str());
		}

		template<>
		MString(std::istream_iterator<char> beg, std::istream_iterator<char> end)
		{
			std::string str(beg, end);
			if (TextIsUtf8Code(str)){
				str = utf8_gbk(str);
			}
			Init(str.c_str());
		}

		//
		// 泛型构造函数
		//
		template<class T>
		MString(T value) {
			std::stringstream s;
			s.setf(sFloatFromat, std::ios_base::floatfield);
			s.precision(sFloatPrecision);
			if (!(s << value).fail()){
				Init(s.str().c_str());
			}
		}


		//
		// 泛型的幅值操作函数
		//
		template<class iter>
		MString& assign(iter beg, iter end){
			chechshared();
			*this = "";
			for (; beg != end;){
				*this += *beg++;
			}
			return *this;
		}

		template<>
		MString& assign(char* beg, char* end){
			chechshared();
			clear();
			std::string str(beg, end);
			*this = str;
			return *this;
		}

		template<>
		MString& assign(const char* beg, const char* end){
			chechshared();
			clear();
			std::string str(beg, end);
			*this = str;
			return *this;
		}

		template<>
		MString& assign(std::istreambuf_iterator<char> beg, std::istreambuf_iterator<char> end){
			chechshared();
			std::string str(beg, end);
			str = TextIsUtf8Code(str) ? utf8_gbk(str) : str;
			clear();
			*this += str;
			return *this;
		}

		template<>
		MString& assign(std::istream_iterator<char> beg, std::istream_iterator<char> end){
			chechshared();
			std::string str(beg, end);
			str = TextIsUtf8Code(str) ? utf8_gbk(str) : str;
			clear();
			*this += str;
			return *this;
		}


		MString& erase(int beg, int end);

		//
		// 往前添加
		//
		template<class T>
		MString& push_front(const T& value){
			chechshared();
			MString __v(value);
			__v += *this;
			*this = __v;
			return *this;
		}



		//
		// 泛型幅值操作
		//
		template<class T>
		MString& operator=(T value){
			chechshared();
			std::stringstream s;
			s.setf(sFloatFromat, std::ios_base::floatfield);
			s.precision(sFloatPrecision);
			if (!(s << value).fail()){
				clear();
				m_value->append(s.str().c_str());
			}
			return *this;
		}

		//
		// 针对const std::string&以及string的特化
		//
		template<>
		MString& operator=(const std::string& str){
			chechshared();
			clear();
			m_value->append(str.c_str());
			return *this;
		}

		template<>
		MString& operator=(std::string str){
			chechshared();
			clear();
			m_value->append(str.c_str());
			return *this;
		}

		template<>
		MString& operator=(const char* str){
			chechshared();
			clear();
			m_value->append(str);
			return *this;
		}

		template<>
		MString& operator=(char* str){
			chechshared();
			clear();
			m_value->append(str);
			return *this;
		}

		template<>
		MString& operator=(const std::wstring& str){
			chechshared();
			clear();
			m_value->append(str.c_str());
			return *this;
		}

		template<>
		MString& operator=(std::wstring str){
			chechshared();
			clear();
			m_value->append(str.c_str());
			return *this;
		}

		template<>
		MString& operator=(const wchar_t* str){
			chechshared();
			clear();
			m_value->append(str);
			return *this;
		}

		template<>
		MString& operator=(wchar_t* str){
			chechshared();
			clear();
			m_value->append(str);
			return *this;
		}

		//
		// 转换为其他类型
		//
		template<class T>
		T ToOtherType() const{
			T x;
			if (m_value->empty())
				return T();
			std::istringstream s(m_value->c_str());
			//
			// 目前结果是只要有能够转换部分就执行转换操作,不需要全部转换
			// 如果需要全部转换成功才算成功的话可以加上下面代码 && s.get() == std::char_traits<char>::eof()
			// 在s>>x 后面
			//
			if (s >> x){
				return x;
			}
			else{
				throw std::runtime_error("convent fail .... ");
			}
		}

		//
		// 针对string和char*的转换
		//
		template<>
		const char* ToOtherType() const{
			return c_str();
		}

		template<>
		std::string ToOtherType() const{
			return toStdString();
		}

		template<>
		const std::string ToOtherType() const{
			return toStdString();
		}

		template<>
		MString ToOtherType() const{
			return *this;
		}

		template<>
		const MString ToOtherType() const{
			return *this;
		}


		template<>
		char ToOtherType() const{
			return toChar();
		}

		//
		// 分割操作
		//
		template<typename T, typename A, template<typename T1, typename A1> class C>
		void split(const MString& isanyof, C<T, A>& result, bool isDiscardEmpty = true){
			result.clear();
			if (isanyof.empty())return;
			std::vector<std::string> __result;
			split(isanyof, __result, isDiscardEmpty);
			if (__result.empty())return;
			for (std::string& mstr : __result){
				try{
					MString __mstr = mstr;
					T other = __mstr.ToOtherType<T>();
					result.push_back(other);
				}
				catch (...){
					;
				}
			}
		}

		template<typename T, typename A, template<typename T1, typename A1> class C>
		void split(const MString& isanyof, C<T, A>& result, bool isDiscardEmpty = true) const{
			const_cast<MString*>(this)->split(isanyof, result, isDiscardEmpty);
		}

		//
		// 针对字符串进行特化
		//
		template<typename A, template<typename T1, typename A1> class C>
		void split(const MString& isanyof, C<std::string, A>& result, bool isDiscardEmpty = true){
			result.clear();
			if (isanyof.empty())return;
			std::vector<std::string> __result;
			split(isanyof, __result, isDiscardEmpty);
			result.assign(__result.begin(), __result.end());
		}

		template<typename A, template<typename T1, typename A1> class C>
		void split(const MString& isanyof, C<std::wstring, A>& result, bool isDiscardEmpty = true){
			result.clear();
			if (isanyof.empty())return;
			std::vector<std::string> __result;
			split(isanyof, __result, isDiscardEmpty);
			result.reserve(__result.size());
			for (auto& s : __result){
				result.push_back(MString(s));
			}
		}

		template<typename A, template<typename T1, typename A1> class C>
		void split(const MString& isanyof, C<MString, A>& result, bool isDiscardEmpty = true){
			result.clear();
			if (isanyof.empty())return;
			C<std::string, A> __result;
			split(isanyof, __result, isDiscardEmpty);
			result.assign(__result.begin(), __result.end());
		}

		//
		// 使用正则表达式分割操作
		//
		template<typename T, typename A, template<typename T1, typename A1> class C>
		void reg_split(const MString& reg, C<T, A>& result){
			result.clear();
			std::vector<MString>  v;
			reg_split(reg, v);
			for (MString& s : v){
				if (s.empty())
					continue;
				try{
					T other = s.ToOtherType<T>();
					result.push_back(other);
				}
				catch (...){
					;
				}
			}
		}


		//
		// 对字符串进行特化
		//
		template<typename A, template<typename T1, typename A1> class C>
		void reg_split(const MString& reg, C<std::string, A>& result){
			result.clear();
			if (reg.empty())
				return;

			MString __str = *this;
			__str.chechshared();
			__str.m_value->MarkUnshareable();
			__str.reg_replace(reg, __SPLIT__);
			__str.split(__SPLIT__, result);
		}

		template<typename A, template<typename T1, typename A1> class C>
		void reg_split(const MString& reg, C<std::wstring, A>& result){
			result.clear();
			if (reg.empty())
				return;

			MString __str = *this;
			__str.chechshared();
			__str.m_value->MarkUnshareable();
			__str.reg_replace(reg, __SPLIT__);
			__str.split(__SPLIT__, result);
		}

		template<typename A, template<typename T1, typename A1> class C>
		void reg_split(const MString& reg, C<MString, A>& result){
			result.clear();
			if (reg.empty())
				return;
			MString __str = *this;
			__str.chechshared();
			__str.m_value->MarkUnshareable();
			__str.reg_replace(reg, __SPLIT__);
			__str.split(__SPLIT__, result);
		}

		template<typename T, typename A, template<typename T1, typename A1> class C>
		void reg_split(const MString& reg, C<T, A>& result) const{
			const_cast<MString*>(this)->reg_split(reg, result);
		}

		//
		// all 判断所有元素是否满足给定需求
		//
		template<class iter, class F>
		bool all(iter beg, iter end, F fun){
			while (beg != end){
				if (!fun(*beg))
					return false;
			}
			return true;
		}

		template<class iter, class F>
		bool all(iter beg, iter end, F fun) const{
			return const_cast<MString*>(this)->all(beg, end, fun);
		}

		template<class F>
		bool all(int beg, int end, F fun){
			int len = m_value->m_data.length();
			beg = end > beg ? beg : end;
			end = end > beg ? end : beg;
			if (beg < 0 || beg >= len || end < 0 || end >= len)
				return false;
			for (; beg != end; ++beg){
				if (!fun(m_value->c_str()[i]))
					return false;
			}
			return true;
		}

		template<class F>
		bool all(F fun){
			int len = m_value->m_data.length();
			for (int i = 0; i < len; ++i){
				if (!fun(m_value->c_str()[i]))
					return false;
			}
			return true;
		}

		template<class F>
		bool all(F fun) const{
			return const_cast<MString*>(this)->all(fun);
		}



		//
		// 必须是全匹配
		//
		template<class T, class A, template<class T1, class A1>class C>
		bool reg_match(const MString& reg, C<T, A>& result){
			result.clear();
			std::wregex __reg(reg.toStdString());
			std::wsmatch m;
			std::wstring __str = this->toStdWString();
			bool __bmatch = std::regex_search(__str, m, __reg);
			if (__bmatch){
				for (int i = 0; i < m.size(); ++i){
					MString str(m[i]);
					try{
						T value = str.ToOtherType<T>();
						result.push_back(value);
					}
					catch (...){
						;
					}
				}
			}
			return __bmatch;
		}

		template<class T, class A, template<class T1, class A1>class C>
		bool reg_match(const MString& reg, C<T, A>& result) const{
			return const_cast<MString*>(this)->reg_match(reg, result);
		}

		//
		// 查找匹配的第一个数据
		//
		template<class T>
		bool reg_find(const MString& reg, T& result){
			std::wregex __reg(reg.toStdWString());
			std::wsmatch m;
			std::wstring __str = this->toStdWString();
			bool __bmatch = std::regex_search(__str, m, __reg);
			if (__bmatch){
				for (int i = 0; i < m.size(); ++i){
					__str = m[i];
					MString str(__str);
					if (str.empty())
						continue;
					try{
						result = str.ToOtherType<T>();
						return true;
					}
					catch (std::runtime_error e){
						std::cout << str << std::endl;
						std::cout << e.what() << std::endl;
						return false;
					}
				}
				return true;
			}
			return false;
		}

		template<class T>
		bool reg_find(const MString& reg, T& result) const{
			return const_cast<MString*>(this)->reg_find(reg, result);
		}

		template<class T>
		bool reg_find_replaced(const MString& reg, T& result){
			MString str;
			bool is_ok = reg_find(reg, str);
			if (is_ok){
				replace_first(str, "");
				trim();
				try{
					result = str.ToOtherType<T>();
				}
				catch (...){
					;
				}
			}
			return is_ok;
		}

		template<class T>
		bool reg_find_replaced(const MString& reg, T& result) const{
			return const_cast<MString*>(this)->reg_find_replaced(reg, result);
		}


		//
		// 只返回第一个匹配的数据信息
		//
		template<class T, class A, template<class T1, class A1> class C>
		bool reg_serch(const MString& reg, C<T, A>& result){
			result.clear();
			std::wregex __reg(reg.toStdWString());
			std::wsmatch m;
			std::wstring wstr = this->toStdWString();
			bool __bmatch = std::regex_search(wstr, m, __reg);
			if (__bmatch){
				for (int i = 0; i < m.size(); ++i){
					std::wstring __wstr(m[i]);
					MString str(__wstr);
					try{
						T value = str.ToOtherType<T>();
						result.push_back(value);
					}
					catch (...){
						;
					}
				}
			}
			return __bmatch;
		}

		template<class T, class A, template<class T1, class A1> class C>
		bool reg_serch(const MString& reg, C<T, A>& result) const{
			return const_cast<MString*>(this)->reg_serch(reg, result);
		}

		//
		// 将所有匹配的全部提取出来
		//
		template<class T, class A, template<class T1, class A1> class C>
		void reg_serch_all(const MString& reg, C<T, A>& result){
			result.clear();
			std::wregex __reg(reg.toStdWString());
			std::wstring str = this->toStdWString();
			std::wsregex_iterator pos(str.begin(), str.end(), __reg);
			std::wsregex_iterator end;
			for (; pos != end;){
				std::wstring wstr((*pos)[0]);
				MString mstr(wstr);
				try{
					T value = mstr.ToOtherType<T>();
					result.push_back(value);
				}
				catch (...){
					;
				}
				++pos;
			}
		}

		//
		// 针对字符串进行特化
		//
		template<class A, template<class T1, class A1> class C>
		void reg_serch_all(const MString& reg, C<std::string, A>& result){
			result.clear();
			std::wregex __reg(reg.toStdWString());
			std::wstring str = this->toStdWString();
			std::wsregex_iterator pos(str.begin(), str.end(), __reg);
			std::wsregex_iterator end;
			for (; pos != end;){
				std::wstring wstr((*pos)[0]);
				MString mstr(wstr);
				result.push_back(mstr);
				++pos;
			}
		}

		template<class A, template<class T1, class A1> class C>
		void reg_serch_all(const MString& reg, C<std::wstring, A>& result){
			result.clear();
			std::wregex __reg(reg.toStdWString());
			std::wstring str = this->toStdWString();
			std::wsregex_iterator pos(str.begin(), str.end(), __reg);
			std::wsregex_iterator end;
			for (; pos != end;){
				result.push_back((*pos)[0]);
				++pos;
			}
		}

		template<class A, template<class T1, class A1> class C>
		void reg_serch_all(const MString& reg, C<MString, A>& result){
			result.clear();
			std::wregex __reg(reg.toStdWString());
			std::wstring str = this->toStdWString();
			std::wsregex_iterator pos(str.begin(), str.end(), __reg);
			std::wsregex_iterator end;
			for (; pos != end;){
				std::wstring  __str = (*pos)[0];
				result.push_back(__str);
				++pos;
			}
		}

		template<class T, class A, template<class T1, class A1> class C>
		void reg_serch_all(const MString& reg, C<T, A>& result) const{
			const_cast<MString*>(this)->reg_serch_all(reg, result);
		}

		//
		// 格式化函数
		// 在使用格式化操作之前请先使用clear函数清空
		// 当前缓存
		// 还有可能会同时改变多个共享数据
		//
		template<typename T, typename... Args>
		void operator()(const char* s, const T& value, const Args&... args){
			chechshared();
			while (s && *s) {
				if (*s == '%') {
					char ch = *++s;
					if (ch == '+'){
						char __ch = *++s;
						if (__ch > 48 && __ch < 58){
							int num = __ch;
							num -= 48;
							__ch = *++s;
							if (__ch >= 48 && __ch < 58){
								int __num = __ch;
								__num -= 48;
								num *= 10;
								num += __num;
								++s;
							}
							std::ostringstream os;
							os.width(num);
							os.setf(std::ios::right);
							os << value;
							*this += os.str();
							return operator ()(s, args...);
						}
						else{
							MString mstr;
							mstr = value;
							*this += mstr;
							return operator ()(s, args...);
						}
					}
					else if (ch == '-'){
						char __ch = *++s;
						if (__ch > 48 && __ch < 58){
							int num = __ch;
							num -= 48;
							__ch = *++s;
							if (__ch >= 48 && __ch < 58){
								int __num = __ch;
								__num -= 48;
								num *= 10;
								num += __num;
								++s;
							}

							std::ostringstream os;
							os.width(num);
							os.setf(std::ios::left);
							os << value;
							*this += os.str();
							return operator ()(s, args...);
						}
						else{
							MString mstr;
							mstr = value;
							*this += mstr;
							return operator ()(s, args...);
						}
					}
					else if ((ch >= 48 && ch < 58) || (ch >= 65 && ch < 91) || (ch >= 97 && ch < 123))
					{
						std::ostringstream os;
						os << value;
						*this += os.str();
						return operator ()(++s, args...);
					}
					else if (ch == '.'){
						ch = *++s;
						std::ostringstream os;
						if (ch >= 48 && ch < 58){
							int num = ch;
							num -= 48;
							os.precision(num);
							char __ch = *++s;
							if (__ch == 'e' || __ch == 'E'){
								os.setf(std::ios_base::scientific, std::ios_base::floatfield);
							}
							else if (__ch == 'f' || __ch == 'F'){
								os.setf(std::ios_base::fixed, std::ios_base::floatfield);
							}
							else{
								--s;
							}
						}
						else{
							--s;
						}
						os << value;
						*this += os.str();
						return operator ()(++s, args...);
					}
					--s;
				}
				*this += *s++;
			}
		}

		template<class T, class... Args>
		static MString Format(const char* s, const T& value, const Args&...args){
			MString str;
			str(s, value, args...);
			return str;
		}


		//
		// QString的交互
		//
#ifdef __QT__
		MString(const QString& value){
			std::wstring wstr = value.toStdWString();
			Init(wstr.c_str());
		}

		MString(QString& value){
			std::wstring wstr = value.toStdWString();
			Init(wstr.c_str());
		}

		template<>
		MString& operator=(const QString& str){
			std::wstring wstr = str.toStdWString();
			*this = wstr;
			return *this;
		}

		template<>
		MString& operator=(QString& str){
			std::wstring wstr = str.toStdWString();
			*this = wstr;
			return *this;
		}

		operator QString() const{
			std::wstring wch = toStdWString();
			QString __Str(QString::fromStdWString(wch));
			return __Str;
		}

		operator QString(){
			std::wstring wch = toStdWString();
			QString __Str(QString::fromStdWString(wch));
			return __Str;
		}

		QString toQString() const{
			std::wstring wch = toStdWString();
			QString __Str(QString::fromStdWString(wch));
			return __Str;
		}

		QString toQString(){
			std::wstring wch = toStdWString();
			QString __Str(QString::fromStdWString(wch));
			return __Str;
		}

		friend QDataStream& operator<<(QDataStream& out, const MString& Mstr){
			out << Mstr.toQString();
			return out;
		}

		friend QStringList& operator<<(QStringList& l, const MString& MStr){
			l << MStr.toQString();
			return l;
		}

		template<typename A, template<typename T1, typename A1> class C>
		void split(const MString& isanyof, C<QString, A>& result,bool isDiscardEmpty = true){
			result.clear();
			if (isanyof.empty())return;
			std::vector<std::string> __result;
			split(isanyof, __result,isDiscardEmpty);
			result.reserve(__result.size());
			for (auto& s : __result){
				result.push_back(MString(s));
			}
		}

		template<typename A, template<typename T1, typename A1> class C>
		void reg_split(const MString& reg, C<QString, A>& result){
			result.clear();
			if (reg.empty())
				return;

			MString __str = *this;
			__str.chechshared();
			__str.m_value->MarkUnshareable();
			__str.reg_replace(reg, __SPLIT__);
			__str.split(__SPLIT__, result);
		}

		template<class A, template<class T1, class A1> class C>
		void reg_serch_all(const MString& reg, C<QString, A>& result){
			result.clear();
			std::wregex __reg(reg.toStdWString());
			std::wstring str = this->toStdWString();
			std::wsregex_iterator pos(str.begin(), str.end(), __reg);
			std::wsregex_iterator end;
			for (; pos != end;){
				result.push_back(QString::fromStdWString(*pos)[0]);
				++pos;
			}
		}
#endif



		template<class iter>
		MString copy(iter beg, iter end){
			MString result;
			for (; beg != end;){
				result += *beg++;
			}
			return result;
		}

		template<class iter>
		MString copy(iter beg, iter end) const{
			return const_cast<MString*>(this)->copy(beg, end);
		}

		template<>
		MString copy(iterator beg, iterator end){
			MString result(&((*this)[beg - begin()]), end - beg);
			return result;
		}

		template<>
		MString copy(iterator beg, iterator end) const{
			return const_cast<MString*>(this)->copy(beg, end);
		}

		template<>
		MString copy(const_iterator beg, const_iterator end){
			const MString __Str(*this);
			MString result(&(const_cast<char*>((__Str.c_str()))[beg - __Str.begin()]), end - beg);
			return result;
		}

		template<>
		MString copy(const_iterator beg, const_iterator end) const{
			return const_cast<MString*>(this)->copy(beg, end);
		}

		template<>
		MString copy(__int64 beg, __int64 end){
			if (beg > end || beg < 0 || end > size())
				throw std::runtime_error("Error .... ");
			MString result(private_copy(beg, end));
			return result;
		}

		template<>
		MString copy(__int64 beg, __int64 end) const{
			return const_cast<MString*>(this)->copy(beg, end);
		}

		template<>
		MString copy(size_t beg, size_t end){
			return copy<__int64>(beg, end);
		}

		template<>
		MString copy(size_t beg, size_t end) const{
			return const_cast<MString*>(this)->copy(beg, end);
		}

		template<>
		MString copy(int beg, int end){
			return copy<__int64>(beg, end);
		}

		template<>
		MString copy(int beg, int end) const{
			return const_cast<MString*>(this)->copy(beg, end);
		}


		//
		// 实现流操作
		//
		friend std::ostream& operator<<(std::ostream& os, const MString& str){
			os << str.toStdString();
			return os;
		}

		friend std::istream& operator>>(std::istream& is, MString& str){
			std::string str1;
			std::getline(is, str1, '\n');
			str = str1;
			return is;
		}

		//
		// 针对宽流的支持
		//
		friend std::wostream& operator<<(std::wostream& os, const MString& str){
			os << str.toStdWString();
			return os;
		}

		friend std::wistream& operator>>(std::wistream& wis, MString& str){
			std::wstring wstr;
			std::getline(wis, wstr);
			str = MString(wstr);
			return wis;
		}

		//
		// 显示转换数据到MString
		//
		template<class T>
		static MString number(T value, FLOATFORMAT Format = FIXED, int pricision = 3){
			std::ostringstream s;
			s.setf(Format, std::ios_base::floatfield);
			s.precision(pricision);
			s << value;
			return s.str();
		}

		template<class T>
		static MString number(const T& value, int pricision, const char& format = 'f'){
			std::ostringstream s;
			if (format == 'f' || format == 'F'){
				s.setf(std::ios_base::fixed, std::ios_base::floatfield);
			}
			else{
				s.setf(std::ios_base::scientific, std::ios_base::floatfield);
			}
			s.precision(pricision);
			s << value;
			return s.str();
		}

		template<class T>
		static MString to_utf8(const T& value){
			MString str = value;
			return str.to_utf8();
		}

		///
		/// 追加buffer
		///
		template<class T>
		MString& append_buffer(const T& value, bool isBigEndian = true){
			chechshared();
			char* ch = nullptr;
			if (isBigEndian){
				ToBigEndianBuffer(value, ch);
			}
			else{
				ToLittleEndianBuffer(value, ch);
			}
			this->append(ch, sizeof(T));
			FreeBuffer(ch);
			return *this;
		}

		template<class T, class A, template<class T1, class A1> class C>
		MString& append_buffer(const C<T, A>& values, bool isBigEndian = true){
			for (T& val : values){
				append_buffer(val, isBigEndian);
			}
			return *this;
		}

		template<class T, template<class T1> class C>
		MString& append_buffer(const C<T>& values, bool isBigEndian = true){
			for (T& val : values){
				append_buffer(val, isBigEndian);
			}
			return *this;
		}

		///
		/// 修改某段buffer
		///
		template<class T>
		MString& modify_buffer(const T& value, uint64 pos, bool isBigEndian = true){
			chechshared();
			if (pos >= this->length()){
				append_buffer(value, isBigEndian);
				return *this;
			}
			char*& buffer = m_value->c_str();

			if (pos + sizeof(T) > this->length()){
				this->erase(pos, this->length());
				append_buffer(value, isBigEndian);
				return *this;
			}

			char* ch = nullptr;
			if (isBigEndian){
				ToBigEndianBuffer(value, ch);
			}
			else{
				ToLittleEndianBuffer(value, ch);
			}

			memcpy(&buffer[pos], ch, sizeof(T));
			FreeBuffer(ch);
			return *this;
		}

		///
		/// \brief Buffer
		/// \param buffer
		/// \param pos
		/// 获取指定段buffer,实行的是深拷贝，用完之后需要使用FreeBuffer对内存进行释放
		///
		void Buffer(MBuffer& buffer, uint64 pos = 0ull, uint64 size = 0ull);



		///
		/// 将buffer还原成一组数据，针对标准库
		///
		template<class T>
		void BufferToValue(T& value, uint64 pos = 0ull, bool isBigEndian = true){
			if (isBigEndian){
				FromBigEndianBuffer(&((*this)[pos]), value);
			}
			else{
				FromLittleEndianBuffer(&((*this)[pos]), value);
			}
			return;
		}

		template<class T, class A, template<class T1, class A1> class C>
		void BufferToValue(C<T, A>& values, uint64 pos = 0ull, bool isBigEndian = true){
			values.clear();
			MBuffer __Buffer;
			Buffer(__Buffer, pos);
			pos = 0ull;
			T val;
			while (pos < __Buffer.mLength){
				if (isBigEndian){
					FromBigEndianBuffer(&(__Buffer.mBuffer[pos]), val);
				}
				else{
					FromLittleEndianBuffer(&(__Buffer.mBuffer[pos]), val);
				}
				values.push_back(val);
				pos += sizeof(T);
			}
			FreeBuffer(__Buffer);
		}

		///
		/// 将buffer还原成一组数据，针对Qt或者其他的容器
		///
		template<class T, template<class T1> class C>
		void BufferToValue(C<T>& values, uint64 pos = 0ull, bool isBigEndian = true){
			values.clear();
			MBuffer __Buffer;
			Buffer(__Buffer, pos);
			pos = 0ull;
			T val;
			while (pos < __Buffer.mLength){
				if (isBigEndian){
					FromBigEndianBuffer(&(__Buffer.mBuffer[pos]), val);
				}
				else{
					FromLittleEndianBuffer(&(__Buffer.mBuffer[pos]), val);
				}
				values.push_back(val);
				pos += sizeof(T);
			}
			FreeBuffer(__Buffer);
		}

		//
		// 下面都是一些静态函数
		//

		//
		// utf8 字符串转换到gbk字符串
		//
		static std::string utf8_gbk(const std::string& uft8str);

		//
		// 生成大小端的buffer
		//
		static void ToBigEndianBuffer(__int8 value, char*& buffer);
		static void ToBigEndianBuffer(__int16 value, char*& buffer);
		static void ToBigEndianBuffer(__int32 value, char*& buffer);
		static void ToBigEndianBuffer(__int64 value, char*& buffer);
		static void ToBigEndianBuffer(unsigned __int8 value, char*& buffer);
		static void ToBigEndianBuffer(unsigned __int16 value, char*& buffer);
		static void ToBigEndianBuffer(unsigned __int32 value, char*& buffer);
		static void ToBigEndianBuffer(unsigned __int64 value, char*& buffer);
		static void ToBigEndianBuffer(float value, char*& buffer);
		static void ToBigEndianBuffer(double value, char*& buffer);

		static void ToLittleEndianBuffer(__int8 value, char*& buffer);
		static void ToLittleEndianBuffer(__int16 value, char*& buffer);
		static void ToLittleEndianBuffer(__int32 value, char*& buffer);
		static void ToLittleEndianBuffer(__int64 value, char*& buffer);
		static void ToLittleEndianBuffer(unsigned __int8 value, char*& buffer);
		static void ToLittleEndianBuffer(unsigned __int16 value, char*& buffer);
		static void ToLittleEndianBuffer(unsigned __int32 value, char*& buffer);
		static void ToLittleEndianBuffer(unsigned __int64 value, char*& buffer);
		static void ToLittleEndianBuffer(float value, char*& buffer);
		static void ToLittleEndianBuffer(double value, char*& buffer);

		//
		// 解析大小端buffer
		//
		static void FromBigEndianBuffer(char* buffer, __int8& value);
		static void FromBigEndianBuffer(char* buffer, __int16& value);
		static void FromBigEndianBuffer(char* buffer, __int32& value);
		static void FromBigEndianBuffer(char* buffer, __int64& value);
		static void FromBigEndianBuffer(char* buffer, unsigned __int8& value);
		static void FromBigEndianBuffer(char* buffer, unsigned __int16& value);
		static void FromBigEndianBuffer(char* buffer, unsigned __int32& value);
		static void FromBigEndianBuffer(char* buffer, unsigned __int64& value);
		static void FromBigEndianBuffer(char* buffer, float& value);
		static void FromBigEndianBuffer(char* buffer, double& value);

		static void FromLittleEndianBuffer(char* buffer, __int8& value);
		static void FromLittleEndianBuffer(char* buffer, __int16& value);
		static void FromLittleEndianBuffer(char* buffer, __int32& value);
		static void FromLittleEndianBuffer(char* buffer, __int64& value);
		static void FromLittleEndianBuffer(char* buffer, unsigned __int8& value);
		static void FromLittleEndianBuffer(char* buffer, unsigned __int16& value);
		static void FromLittleEndianBuffer(char* buffer, unsigned __int32& value);
		static void FromLittleEndianBuffer(char* buffer, unsigned __int64& value);
		static void FromLittleEndianBuffer(char* buffer, float& value);
		static void FromLittleEndianBuffer(char* buffer, double& value);

		///
		/// \brief FreeBuffer
		/// \param buffer
		/// 针对上面所生成的buffer在此处释放
		///
		static void FreeBuffer(char*& buffer);

		///
		/// \brief FreeBuffer
		/// \param buffer 必须是MString中映射出来的buffer
		///
		static void FreeBuffer(MBuffer& buffer);

		///
		/// 获取guid字符串
		///
		static MString GuidString();

		///
		/// 获取当前时间
		///
		static MString CurrentTime();

		/// 
		/// 获取环境变量数据
		///
		static MString EnvString(const MString& EnvKey);

		///
		/// 设置环境变量
		///
		static bool   SetEnv(const MString& EnvKey, const MString& EnvValue);

		///
		/// \brief operator ()
		/// \param s
		///
		void operator()(const char* s);


		//
		// 类型转换系列函数
		//
		int toInt() const;
		int toInt();

		unsigned int toUInt() const;
		unsigned int toUInt();

		short toShort() const;
		short toShort();

		unsigned short toUShort() const;
		unsigned short toUShort();

		long toLong() const;
		long toLong();

		unsigned long toULong() const;
		unsigned long toULong();

		__int64 toLongLong() const;
		__int64 toLongLong();

		unsigned __int64 toULongLong() const;
		unsigned __int64 toULongLong();

		float toFloat() const;
		float toFloat();

		double toDouble() const;
		double toDouble();

		///
		/// 成员变量
		///
	private:

		//
		// 查询当前是否有共享
		//
		inline void chechshared();
		MString& private_append(const MString& value);
		void Init(char *buf, uint64 size);
		void Init(const char *buf, uint64 size);
		void Init(const char* str);
		void Init(const wchar_t* str);
		const char* private_copy(int beg, int end);



		//
		// 数据指针
		//
		StringRef<MStringImpl>  m_value;

		//
		// 主要针对浮点数转换为字符串时是否采用科学技术还是使用顶点格式
		//
		static std::ios_base::fmtflags    sFloatFromat;
		static unsigned					  sFloatPrecision;


		//
		// 字节编码
		//
		static MCharacterCode			 sCharactercode;
	};

	template<bool>
	struct MStringHelp{
		template<class T>
		static MString& do_help(MString& m, const T& t){
			MString s = t;
			m.replace_first(s.c_str(), "");
			return m;
		}
	};

	template<>
	struct MStringHelp<true>{
		template<class T>
		static MString& do_help(MString& m, const T& s){
			m.replace_first(s.c_str(), "");
			return m;
		}
	};

	//
	// 使得标准库容器可以转换为MString
	//
	template<class T, class A, template<class T1, class A1> class C>
	inline MString JoinToMString(const C<T, A>& c, const MString& joinstr){
		MString __result;
		for (auto s : c){
			if (!__result.empty()){
				__result << joinstr;
			}
			__result << s;
		}
		return __result;
	}

	//
	// 使得非标准库的容器可以转换MString
	//
	template<class T, template<class T1> class C>
	inline MString JoinToMString(const C<T>& c, const MString& joinstr){
		MString __result;
		for (auto s : c){
			if (!__result.empty()){
				__result << joinstr;
			}
			__result << s;
		}
		return __result;
	}
}

//===============================================
// 使用operator>>输出数据
// 以空格为间隔
// 输出数据后会改变原始数据
//===============================================

template<class T>
mj::MString& operator>>(mj::MString& m, T& t){
	try{
		t = m.ToOtherType<T>();
	}
	catch (...){
		return m;
	}

	const bool is_true = (std::is_same<T, mj::MString>::value ||
		std::is_same<T, std::string>::value) ? true : false;
	m.trim();
	return mj::MStringHelp<is_true>::do_help(m, t);
}

#ifdef __QT__
inline std::ostream& operator<<(std::ostream& os, QString str){
	mj::MString wch = str.toStdWString();
	os << wch;
	return os;
}

inline QString operator+(const QString& qstr, const mj::MString& mstr){
	QString __result(qstr);
	__result += mstr.toQString();
	return __result;
}

inline QString operator+(const mj::MString& mstr, const QString& qstr){
	QString __result = mstr.toQString();
	__result += qstr;
	return __result;
}

#endif

inline std::string operator+(const char* ch, const mj::MString& mstr){
	mj::MString __result(ch);
	__result << mstr;
	return __result;
}

inline std::string operator+(const std::string& ch, const mj::MString& mstr){
	mj::MString __result(ch);
	__result << mstr;
	return __result;
}

inline mj::MString operator+(const mj::MString& mstr, const char* ch){
	mj::MString __result(mstr);
	__result << ch;
	return __result;
}

inline mj::MString operator+(const mj::MString& mstr, const std::string& ch){
	mj::MString __result(mstr);
	__result << ch;
	return __result;
}

inline mj::MString operator+(const mj::MString& mstr, const mj::MString& ch){
	mj::MString __result(mstr);
	__result << ch;
	return __result;
}

//
// 好吧,该名字空间使用的时候得小心些,因为范围太大了，极容易和其他东西冲突会导致各种莫名其的编译错误
//
namespace mjplus{
	template<class T>
	inline mj::MString operator+(const mj::MString& mstr, T value){
		mj::MString __result(mstr);
		__result << value;
		return __result;
	}

	template<class T>
	inline mj::MString operator+(T value, const mj::MString& mstr){
		mj::MString __result;
		__result << value;
		__result << mstr;
		return __result;
	}
}

//
// 实现和标准库的兼容
//
namespace std{
	template<>
	inline void swap(mj::MString& first, mj::MString& second){
		first.swap(second);
	}

	template<>
	inline void std::iter_swap<mj::MString::iterator, mj::MString::iterator>(
		mj::MString::iterator iter1, mj::MString::iterator iter2){
		char c1 = *iter1;
		char c2 = *iter2;
		size_t index1 = iter1.CurrentIndex();
		size_t index2 = iter2.CurrentIndex();
		mj::MString& __data = iter1.CurrentData();
		__data[index1] = c2;
		__data[index2] = c1;
	}

	template<>
	inline void std::iter_swap<mj::MString::const_iterator, mj::MString::const_iterator>(
		mj::MString::const_iterator iter1, mj::MString::const_iterator iter2)
	{

	}

	template <>
	struct hash<mj::MString>
	{
		std::size_t operator()(const mj::MString& k) const
		{
			return (hash<string>()(k.toStdString()));
		}
	};
}

