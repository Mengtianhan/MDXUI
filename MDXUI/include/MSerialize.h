

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
#include <MString.h>

namespace DxUI{
	class MXml;
	using namespace mj;
	class MSTRING_API MSerialize
	{
	public:
		MSerialize();
		~MSerialize();

		//
		// 以读的方式打开文件
		//
		void ReadFile(const MString &filename);

		//
		// 为写而打开文件
		//
		void WriteFile(const MString &filename);

		//
		// 关闭文件，操作另一个文件时记得关闭当前文件
		// 对于写文件操作，在些完成之后记得关闭文件
		// 否则数据只会在内存之中
		// 当然在程序退出之时夜会调用该操作
		//
		void CloseFile();

		///
		/// \brief SaveFile
		/// 用于读取的文件但有修改的情况
		///
		void SaveFile();

		bool IsReadFile() const;

		virtual MString ToString() { return "MSerialize"; }


		//
		// 将数据写进文件
		// key xml 文件的key，节点之间以.分割
		// value 要写的值
		//
		template<class T>
		bool __stdcall PutValue(const MString& key, const std::vector<T>& values){
			return PutValues(key, values);
		}

		//
		// 写文件无非是三类数据类型,一类为字符,一类为整形，一类为浮点型
		// 所以下面通过重载函数来解决这个问题
		//
		bool  __stdcall PutValue(const MString& key, const char* value);
		bool  __stdcall PutValues(const MString& key, const std::vector<char*>& values);

		bool  __stdcall PutValue(const MString& key, const std::string& value);
		bool  __stdcall PutValues(const MString& key, const std::vector<std::string>& values);

		bool  __stdcall PutValue(const MString& key, const std::wstring& value);
		bool  __stdcall PutValues(const MString& key, const std::vector<std::wstring>& values);

		bool  __stdcall PutValue(const MString& key, const MString& value);
		bool  __stdcall PutValues(const MString& key, const std::vector<MString>& values);

		bool  __stdcall PutValue(const MString& key, float value);
		bool  __stdcall PutValues(const MString& key, const std::vector<float>& values);

		bool  __stdcall PutValue(const MString& key, double value);
		bool  __stdcall PutValues(const MString& key, const std::vector<double>& values);

		bool  __stdcall PutValue(const MString& key, __int64 value);
		bool  __stdcall PutValues(const MString& key, const std::vector<__int64>& values);

		bool  __stdcall PutValue(const MString& key, __int32 value);
		bool  __stdcall PutValues(const MString& key, const std::vector<__int32>& values);

		bool  __stdcall PutValue(const MString& key, __int16 value);
		bool  __stdcall PutValues(const MString& key, const std::vector<__int16>& values);

		bool  __stdcall PutValue(const MString& key, bool value);
		bool  __stdcall PutValues(const MString& key, const std::vector<bool>& values);

		bool  __stdcall PutValue(const MString& key, unsigned __int64 value);
		bool  __stdcall PutValues(const MString& key, const std::vector<unsigned __int64>& values);

		bool  __stdcall PutValue(const MString& key, unsigned __int32 value);
		bool  __stdcall PutValues(const MString& key, const std::vector<unsigned __int32>& values);

		bool  __stdcall PutValue(const MString& key, unsigned __int16 value);
		bool  __stdcall PutValues(const MString& key, const std::vector<unsigned __int16>& values);

		//
		// 读取数据无非也是读取三类数据
		//
		std::string __stdcall GetValue(const MString& key, const char* defaultvalue);
		std::vector<std::string> __stdcall GetValues(const MString& key, const char* defaultvalue);

		std::string __stdcall GetValue(const MString& key, const std::string& defaultvalue);
		std::vector<std::string> __stdcall GetValues(const MString& key, const std::string& defaultvalue);

		std::wstring __stdcall GetValue(const MString& key, const std::wstring& defaultvalue);
		std::vector<std::wstring> __stdcall GetValues(const MString& key, const std::wstring& defaultvalue);

		MString __stdcall GetValue(const MString& key, const MString& defaultvalue);
		std::vector<MString> __stdcall GetValues(const MString& key, const MString& defaultvalue);

		float __stdcall GetValue(const MString& key, float defaultvalue);
		std::vector<float> __stdcall GetValues(const MString& key, float defaultvalue);

		double __stdcall GetValue(const MString& key, double defaultvalue);
		std::vector<double> __stdcall GetValues(const MString& key, double defaultvalue);

		__int16 __stdcall GetValue(const MString& key, __int16 defaultvalue);
		std::vector<__int16> __stdcall GetValues(const MString& key, __int16 defaultvalue);

		__int32 __stdcall GetValue(const MString& key, __int32 defaultvalue);
		std::vector<__int32> __stdcall GetValues(const MString& key, __int32 defaultvalue);

		__int64 __stdcall GetValue(const MString& key, __int64 defaultvalue);
		std::vector<__int64> __stdcall GetValues(const MString& key, __int64 defaultvalue);

		bool __stdcall GetValue(const MString& key, bool defaultvalue);
		std::vector<bool> __stdcall GetValues(const MString& key, bool defaultvalue);

		unsigned __int16 __stdcall GetValue(const MString& key, unsigned __int16 defaultvalue);
		std::vector<unsigned __int16> __stdcall GetValues(const MString& key, unsigned __int16 defaultvalue);

		unsigned __int32 __stdcall GetValue(const MString& key, unsigned __int32 defaultvalue);
		std::vector<unsigned __int32> __stdcall GetValues(const MString& key, unsigned __int32 defaultvalue);

		unsigned __int64 __stdcall GetValue(const MString& key, unsigned __int64 defaultvalue);
		std::vector<unsigned __int64> __stdcall GetValues(const MString& key, unsigned __int64 defaultvalue);


		//
		// 获取当前xml文件所在路径的父路径
		//
		MString GetParentPath();

		//
		// 创建文件夹
		//
		void CreateFolder(const MString& path);


		//
		// xml 文件是否存在
		//
		bool XmlFileIsExist(const MString& fileName);

	private:
		MXml*    mXmlManager;
		MString  mXmlFile;
		bool    bIsRead;
	};
}

