#pragma once
#include <comdef.h>
#include "IComBase.h"
interface __declspec(uuid("881ABAC6-4A42-4B4F-A34F-30709C929857")) ICoroutServiceNet : public IUnknown
{
	//
	// 设置buffer大小
	//
	virtual void __stdcall ResizeBuffer(__int64 size) = 0;

	//
	// 设置监听端口
	//
	virtual void __stdcall SetListenPort(unsigned port) = 0;
	virtual void __stdcall StartListen() = 0;

	//
	// 同步操作
	//
	virtual int __stdcall Send(const char* send, __int64 size, const char* strid) = 0;
	virtual int __stdcall Read(char*& rec, const char* strid) = 0;

	//
	// 网络信息内存由com组件自身分配,所以安全的释放模式应该由com组件自身释放
	//
	virtual void __stdcall FreeMemery(char* & ptr) = 0;

	//
	// 异步操作
	//
	virtual void __stdcall PostRecive(const char* strid) = 0;
	virtual void __stdcall PostSend(const char* str, __int64 size, const char* strid) = 0;
	virtual void __stdcall PostToAll(const char* send, __int64 size) = 0;

	//
	// 线程中执行协程
	//
	virtual void __stdcall Run() = 0;

	//
	// 主线程中执行协程
	//
	virtual void __stdcall MainThreadRun() = 0;

	virtual void __stdcall BindNetInfoFun(NETINFOFUN fun) = 0;
	virtual void __stdcall BindMsgFun(SERVICEMSFFUN fun) = 0;

	//
	// 通过字符串id查询客户ip和端口
	//
	virtual void __stdcall QueryIPAndPort(const char* clientstrid, char*& ip, unsigned& port) = 0;

	//
	// 踢出某个客户
	//
	virtual void __stdcall DeleteClient(const char* clientstrid) = 0;

	//
	// 查询服务器是否有客户连接
	//
	virtual unsigned __stdcall QueryClientNum() = 0;

	//
	// 查询某个客户是否存在
	//
	virtual bool __stdcall ClientIsExist(const char* clientstrid) = 0;
};

