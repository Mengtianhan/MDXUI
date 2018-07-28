#pragma once


#include <comdef.h>

//
// 由于组件都需要Release，所以此处提供智能指针的选择
//
#include <memory>


//
// 组件ID
//



#ifdef X64
#ifdef _DEBUG
// {4C99DE88-4492-4330-BB6A-DE19253EABDB}
__declspec(selectany) GUID ASIOCLSID =
{ 0x4c99de88, 0x4492, 0x4330, { 0xbb, 0x6a, 0xde, 0x19, 0x25, 0x3e, 0xab, 0xdb } };

#else

// {368EA519-CF02-4221-BA2F-AA90332F9BEF}
__declspec(selectany) GUID ASIOCLSID =
{ 0x368ea519, 0xcf02, 0x4221, { 0xba, 0x2f, 0xaa, 0x90, 0x33, 0x2f, 0x9b, 0xef } };

#endif
#else
#ifdef _DEBUG
// {FE623F09-3358-4284-BBD0-FDABACC6F506}
__declspec(selectany) GUID ASIOCLSID =
{ 0xfe623f09, 0x3358, 0x4284, { 0xbb, 0xd0, 0xfd, 0xab, 0xac, 0xc6, 0xf5, 0x6 } };

#else
__declspec(selectany) GUID ASIOCLSID =
{ 0xa469a2b, 0xca40, 0x4e93, { 0xa1, 0x5, 0x77, 0x61, 0x3d, 0x21, 0xb2, 0x2 } };
#endif
#endif

//
// 引入这几个头文件即可
//

#include "IClientNet.h"
#include "ISerialPort.h"
#include "IServiceNet.h"
#include "IMLogger.h"

//
// 功能一样，内部实现不一样
//
#include "ICoroutClientNet.h"
#include "ICoroutSerialPort.h"
#include "ICoroutServiceNet.h"

//
// 引入UDP
//
#include "IUDPService.h"
#include "IUDPClient.h"

//
// 计时器
//
#include "ITimer.h"

//
// 文件内存映射
//
#include "IFileMap.h"

class MINitCom{
public:
	MINitCom(){
		CoInitialize(nullptr);
	}
	~MINitCom(){
		CoUninitialize();
	}
};

//
// 定义一个宏来初始化com组件
//
#ifndef INITCOM
#define INITCOM MINitCom __GlobInitCom__;
#endif


//
// 预定义一个删除com组件的函数
//
template<class T>
inline void ReleaseCom(T* comptr){
    std::cout<<"Release COM Ptr"<<std::endl;
    if(comptr){
        comptr->Release();
    }
}

//
// 此处不增加引用计数,为什么呢？
//
template<class T>
std::shared_ptr<T> SharedComPtr(T* __comptr){
    return std::shared_ptr<T>(__comptr,ReleaseCom<T>);
}

//
// 因为用到不少的函数绑定，直接添加这个命名空间比较方便
//
using namespace std::placeholders;

//
// 默认提供的创建服务端网络的工厂函数
//
inline IServiceNet* CreateServiceNet(){
	IServiceNet* Serv = NULL;
	::CoCreateInstance(
		ASIOCLSID,
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IServiceNet),
		(void**)&Serv
		);
	return Serv;
}

//
// 创建使用协程实现的网络服务器
//
inline ICoroutServiceNet* CreateCoroutServiceNet(){
	ICoroutServiceNet* Serv = NULL;
	::CoCreateInstance(
		ASIOCLSID,
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(ICoroutServiceNet),
		(void**)&Serv
		);
	return Serv;
}

//
// 默认提供的创建客户端网络的工厂函数
//

inline IClientNet*  CreateClientNet(){
	IClientNet* Client = NULL;
	::CoCreateInstance(
		ASIOCLSID,
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IClientNet),
		(void**)&Client
		);
	return Client;
}

//
// 创建使用协程实现的网络客户端
//
inline ICoroutClientNet*  CreateCoroutClientNet(){
	ICoroutClientNet* Client = NULL;
	::CoCreateInstance(
		ASIOCLSID,
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(ICoroutClientNet),
		(void**)&Client
		);
	return Client;
}

//
// 默认提供的创建串口的工厂函数
//
inline ISerialPort* CreateSerialPort(){
	ISerialPort* Serial = NULL;
	::CoCreateInstance(
		ASIOCLSID,
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(ISerialPort),
		(void**)&Serial
		);
	return Serial;
}

//
// 创建协程实现的串口
//
inline ICoroutSerialPort* CreateCoroutSerialPort(){
	ICoroutSerialPort* Serial = NULL;
	::CoCreateInstance(
		ASIOCLSID,
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(ICoroutSerialPort),
		(void**)&Serial
		);
	return Serial;
}

//
// 创建UDPService
//
inline IUDPService* CreateUDPService(){
	IUDPService* Serv = NULL;
	::CoCreateInstance(
		ASIOCLSID,
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IUDPService),
		(void**)&Serv
		);
	return Serv;
}

//
// 创建UPPClient
//
inline IUDPClient* CreateUDPClient(){
	IUDPClient* Serv = NULL;
	::CoCreateInstance(
		ASIOCLSID,
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IUDPClient),
		(void**)&Serv
		);
	return Serv;
}

//
// 默认提供的创建日志记录的工厂函数
//
inline IMLogger* CreateLogger(){
	IMLogger* Logger = NULL;
	::CoCreateInstance(
		ASIOCLSID,
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IMLogger),
		(void**)&Logger
		);
	return Logger;
}

//
// 计时器的工厂函数
//
inline ITimer* CreateTimer(){
	ITimer* timeptr = NULL;
	::CoCreateInstance(
		ASIOCLSID,
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(ITimer),
		(void**)&timeptr
		);
	return timeptr;
}

//
// 文件内存映射的工厂函数
//
inline IFileMap* CreateFileMap(){
	IFileMap* fileMap = NULL;
	::CoCreateInstance(
		ASIOCLSID,
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IFileMap),
		(void**)&fileMap
		);
	return fileMap;
}
