

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
#include <vector>
#include "MString.h"
using mj::MString;


extern "C" namespace MFileSystem{
	
	MSTRING_API void MRecursiveDir(const MString& dir); // 迭代打印一个目录下面的所有文件
	MSTRING_API MString MFindFile(const MString& dir, const MString& filename); // 查找单个文件
	MSTRING_API void MFindFiles(const MString& dir, const MString& filename, std::vector<MString>& Result); // 查找文件
	MSTRING_API size_t MCopyFiles(const MString& from_dir, const MString& to_dir, const MString& filename = "*"); // 拷贝文件到另一个目录

	MSTRING_API size_t MRemoveFiles(const MString& dir, const MString& filename = "*"); // 移除文件
	MSTRING_API size_t MRemoveFile(const MString& filename);
	MSTRING_API size_t MRemoveDir(const MString& dir); // 移除文件夹
	MSTRING_API bool MFileIsExist(const MString& filename); // 文件是否存在
	MSTRING_API bool MFolderIsExist(const MString& folderpath); // 文件夹是否存在
	MSTRING_API bool MCreateDir(const MString& director);  // 创建一个路径
	MSTRING_API bool MIsCompletePath(const MString& path); // 是否是全路径

	MSTRING_API MString MParentPath(const MString& director);   // 文件的parent路径
	MSTRING_API MString MFileName(const MString& director);     // 文件名
	MSTRING_API MString MExtensionName(const MString& director);// 文件的扩展名
	MSTRING_API MString MLeafName(const MString& director); // 不带扩展名的部分
	MSTRING_API MString MRootName(const MString& director); // 根名
	MSTRING_API MString MRootPath(const MString& director); // 根路径
	MSTRING_API MString MRootDir(const MString& director);  // 根目录
	MSTRING_API MString MRemoveFilename(const MString& path);   // 移除路径中的文件名
	MSTRING_API MString MReplaceExtension(const MString& path, const MString& newext); // 替换扩展名
		 
	MSTRING_API bool    MIsFolder(const MString& path);
	MSTRING_API bool	 MIsFile(const MString& path);


	MSTRING_API MString MCompletePath(const MString& filename); // 获取全路径
	MSTRING_API MString MGuidString();					 // 获取一个GUUID字符串
	MSTRING_API __int64 MFileSize(const MString& filename);	 // 获取文件大小
	MSTRING_API void MRenameFile(const MString& filename, const MString& name); // 重命名文件

	MSTRING_API MString MCurrentPath();	// 获取当前工作路径
	MSTRING_API MString MGetDllPath(void *&ptr);		// 获取当前执行exe的路径
	MSTRING_API MString MGetExePath();		// 获取当前执行的dll路径
	MSTRING_API void MReplaceFileInfo(const MString& filename, const MString& start, \
		const MString& place_str = "", const MString& end = "#End#");  // 替换一个文件中的内容，开始出，替换成XX,被替换的结尾处，"#End#"表示替换到尾

	MSTRING_API MString MCurrentTime();  // 获取当前系统时间 YYYY/MM/DD HH:MM:SS
	MSTRING_API MString MCurrentTime2(const char* Format = "%02d:%02d:%02d");  // 获取当前系统时间 HH:MM:SS
	MSTRING_API MString MCurrentData(const char* Format = "%4d/%02d/%02d");  // 获取当前系统时间 YYYY/MM/DD

	MSTRING_API tm MFileLastTime(const MString& filename);


	MSTRING_API MString  wtoc(const wchar_t* str); //宽字节到多字节的转换
	MSTRING_API std::wstring stow(const char* src);//多字节到宽字节的转换
}

