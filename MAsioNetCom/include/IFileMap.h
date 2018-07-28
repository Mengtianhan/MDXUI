#pragma once
#include <comdef.h>

//
// 文件内存映射
//
interface __declspec(uuid("11B3F921-7465-447B-9590-5CD56BAAC6C4")) IFileMap : public IUnknown
{
	//
	// 创建一个新对象
	//
	virtual IFileMap* __stdcall CreateNewObj() = 0;

	//
	// 打开需要被映射的文件
	//
	virtual bool __stdcall OpenFile(const char* filename,__int64 filesize = 0ll) = 0;

	//
	// 映射一块内存出来
	//
	virtual bool __stdcall Map(char*& mem,__int64 __start = 0ll,__int64 size = 0ll) = 0;

	//
	// 关闭映射
	//
	virtual void __stdcall UnMap() = 0;

	//
	// 查询文件是否被打开
	//
	virtual bool __stdcall IsOpen() = 0;
};
