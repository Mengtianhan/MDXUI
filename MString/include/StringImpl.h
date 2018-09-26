#pragma once
#include "StringRef.h"
#include <iostream>
//
// 内存管理前向声明
//
template<class T>
class CMemeryPool;


//
// 数据实现类
//
namespace mj{
	//
	// 数据类
	//
    class  MStringImpl : public StringRefrenceObj{
		friend class MString;
		friend class StringRef<MStringImpl>;


		CMemeryPool<char>*  mData;

		//
		// 构造普通字符串 以0结尾
		//
		MStringImpl(const char* str = "");
		MStringImpl(const wchar_t* wchar);

		//
		// 使用一段buffer构造,用于存储字节序
		//
		MStringImpl(char* buf, uint64 length);


		void init(const char* str);

		uint64 size();
		uint64 size() const;

		uint64 length();
		uint64 length() const;

		bool empty();
		void trim();
		void trim_left();
		void trim_right();

		bool operator==(const MStringImpl& other);
		bool operator!=(const MStringImpl& other);
		bool operator<(const MStringImpl& other);
		bool operator>(const MStringImpl& other);

		~MStringImpl();

		void clear();

		//
		// 追加信息
		//
		void append(const char* str);
		void append(const wchar_t* str);
        void append(char* buf,uint64 size);
		char*& c_str();
        const wchar_t *c2w();

		void swap(MStringImpl& other);

		void erase(int beg, int end);
		char* copy(int beg, int end);

		//
		// 查找字符串
		//
		uint64 find(const char* str);

		//
		// 从指定位置开始查找
		//
		uint64 find(const char* str, uint64 start_pos);
		uint64 find_last(const char* str);
		uint64 find_first(const char* str);
		uint64 find_nth(const char* str, uint64 index);
		uint64 find_count(const char* str);

		//
		// 替换第一次出现的
		//
		void replace_first(const char* replaced, const char* value);
		void replace_all(const char* replaced, const char* value);
		void replace_nth(const char* replaced, const char* value, uint64 index);
		void replace_last(const char* replaced, const char* value);

		//
		// 比较操作
		//
		bool end_with(const char* str);
		bool iend_with(const char* str);
		bool start_with(const char* str);
		bool istart_with(const char* str);

		//
		// 大小写转换操作
		//
		void To_upper();
		void To_lower();

		//
		// 转换为utf8编码字符串
		//
        const char *to_utf8();

		//
		// 编码转换
		//
        static const char* utf8_2_gbk(const char* uft8str);
	};
}

