

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////


#pragma once
#include "MString.h"
#include <vector>

using mj::MString;
class MXMLParse;


namespace DxUI{

	//
	// 读写 xml 文件
	//
	class MSTRING_API MXml
	{
	public:
		MXml();
		~MXml();

		//
		// 打开文件进行读写
		//
		void    ReadFile(const MString& filename);
		void    WriteFile(const MString& filename);


		//
		// 将数据写进文件
		//
		bool  __stdcall PutValue(const MString& key, const char* value);
		bool  __stdcall PutValue(const MString& key, const std::vector<char*>& values);

		bool  __stdcall PutValue(const MString& key, const std::string& value);
		bool  __stdcall PutValue(const MString& key, const std::vector<std::string>& values);

		bool  __stdcall PutValue(const MString& key, const std::wstring& value);
		bool  __stdcall PutValue(const MString& key, const std::vector<std::wstring>& values);

		bool  __stdcall PutValue(const MString& key, const MString& value);
		bool  __stdcall PutValue(const MString& key, const std::vector<MString>& values);

		bool  __stdcall PutValue(const MString& key, float value);
		bool  __stdcall PutValue(const MString& key, const std::vector<float>& values);

		bool  __stdcall PutValue(const MString& key, double value);
		bool  __stdcall PutValue(const MString& key, const std::vector<double>& values);

		bool  __stdcall PutValue(const MString& key, __int64 value);
		bool  __stdcall PutValue(const MString& key, const std::vector<__int64>& values);

		bool  __stdcall PutValue(const MString& key, __int32 value);
		bool  __stdcall PutValue(const MString& key, const std::vector<__int32>& values);

		bool  __stdcall PutValue(const MString& key, __int16 value);
		bool  __stdcall PutValue(const MString& key, const std::vector<__int16>& values);

		bool  __stdcall PutValue(const MString& key, bool value);
		bool  __stdcall PutValue(const MString& key, const std::vector<bool>& values);

		bool  __stdcall PutValue(const MString& key, unsigned __int64 value);
		bool  __stdcall PutValue(const MString& key, const std::vector<unsigned __int64>& values);

		bool  __stdcall PutValue(const MString& key, unsigned __int32 value);
		bool  __stdcall PutValue(const MString& key, const std::vector<unsigned __int32>& values);

		bool  __stdcall PutValue(const MString& key, unsigned __int16 value);
		bool  __stdcall PutValue(const MString& key, const std::vector<unsigned __int16>& values);



		//
		// 读取数据
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
		// 关闭文件
		// 对于写文件来说，关闭就是保存
		//
		void	CloseFile();

		//
		// 强制保存就算是读文件对其修改后也可以进行保存
		//
		void    SaveFile();
	private:
		MXMLParse* pXml{ nullptr };
	};

}

