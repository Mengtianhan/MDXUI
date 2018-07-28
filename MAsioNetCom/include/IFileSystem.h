#pragma once
#include <comdef.h>
#include <string>
#include "IAsioInterface.h"
#include <vector>

interface  __declspec(uuid("BBD4F39D-75B9-46D5-B791-F03040F74277"))  IFileSystem : public IUnknown
{
	virtual void __stdcall MRecursiveDir(const std::string& dir) = 0; // 迭代打印一个目录下面的所有文件
	virtual std::string __stdcall MFindFile(const std::string& dir, const char* filename) = 0; // 查找单个文件
	virtual void __stdcall MFindFiles(const std::string& dir, const std::string& filename, std::vector<std::string>& Result) = 0; // 查找文件
	virtual size_t __stdcall MCopyFiles(const std::string& from_dir,
		 const std::string& to_dir, 
		 const std::string& filename = "*") = 0; // 拷贝文件到另一个目录

	virtual size_t __stdcall MRemoveFiles(const std::string& dir, const std::string& filename = "*") = 0; // 移除文件
	virtual size_t __stdcall MRemoveDir(const std::string& dir) = 0; // 移除文件夹
	virtual bool __stdcall MRemoveFile(const std::string& file) = 0;
	virtual bool __stdcall MFileIsExist(const std::string& filename) = 0; // 文件是否存在
	virtual bool __stdcall MFolderIsExist(const std::string& folderpath) = 0; // 文件夹是否存在
	virtual bool __stdcall MCreateDir(const std::string& director) = 0;  // 创建一个路径
	virtual bool __stdcall MIsCompletePath(const std::string& path) = 0; // 是否是全路径

	virtual std::string __stdcall MParentPath(const std::string& director) = 0;   // 文件的parent路径
	virtual std::string __stdcall MFileName(const std::string& director) = 0;     // 文件名
	virtual std::string __stdcall MExtensionName(const std::string& director) = 0;// 文件的扩展名
	virtual std::string __stdcall MLeafName(const std::string& director) = 0; // 不带扩展名的部分
	virtual std::string __stdcall MRootName(const std::string& director) = 0; // 根名
	virtual std::string __stdcall MRootPath(const std::string& director) = 0; // 根路径
	virtual std::string __stdcall MRootDir(const std::string& director) = 0;  // 根目录
	virtual std::string __stdcall MRemoveFilename(const std::string& path) = 0;   // 移除路径中的文件名
	virtual std::string __stdcall MReplaceExtension(const std::string& path, const std::string& newext) = 0; // 替换扩展名

	virtual bool __stdcall MIsFolder(const std::string& path) = 0;
	virtual bool __stdcall MIsFile(const std::string& path) = 0;


	virtual std::string __stdcall MCompletePath(const std::string& filename) = 0; // 获取全路径
	virtual std::string __stdcall MGuidString() = 0;					 // 获取一个GUUID字符串
	virtual __int64 __stdcall MFileSize(const std::string& filename) = 0;	 // 获取文件大小
	virtual void __stdcall MRenameFile(const std::string& filename, const std::string& name) = 0; // 重命名文件

	virtual std::string __stdcall MCurrentPath() = 0;	// 获取当前工作路径
	virtual std::string __stdcall MGetDllPath(void* ptr) = 0;		// 获取当前执行exe的路径
	virtual std::string __stdcall MGetExePath() = 0;		// 获取当前执行的dll路径
	virtual void __stdcall MReplaceFileInfo(const std::string& filename, const std::string& start, \
		const std::string& place_str = "", const std::string& end = "#End#") = 0;  // 替换一个文件中的内容，开始出，替换成XX,被替换的结尾处，"#End#"表示替换到尾

	virtual std::string __stdcall MCurrentTime() = 0;  // 获取当前系统时间 YYYY/MM/DD HH:MM:SS

	virtual std::string __stdcall  wtoc(const wchar_t* str) = 0; //宽字节到多字节的转换
	virtual std::wstring __stdcall stow(const char* src) = 0;//多字节到宽字节的转换
};

//
// 默认提供的创建IFileSystem的工厂函数
// 由于该接口没有任何数据
// 所以此处提供的是创建一个静态对象
//
inline IFileSystem* CreateFileSystem(){
        IFileSystem* pFile = NULL;
	::CoCreateInstance(
		ASIOCLSID,
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IFileSystem),
		(void**)&pFile
		);
	return pFile;
}
