#pragma once
#include <comdef.h>
#include "IComBase.h"
interface __declspec(uuid("6046C6DB-FD61-4F06-BF7B-D7E94BCFE79E")) IClientNet : public IUnknown
{
	//
	// 创建一个新对象
	//
	virtual IClientNet* __stdcall CreateNewObj() = 0;

	//
	// 设置接收所使用的buffer大小
	//
	virtual void __stdcall ResizeBuffer(__int64 size) = 0;

	//
	// 设置ip端口以及退出线程
	//
	virtual void __stdcall SetNetIP(const char* ip) = 0;
	virtual void __stdcall SetNetPort(unsigned port) = 0;
	virtual void __stdcall Exit() = 0;

	//
	// 绑定消息回调函数
	//
	virtual void __stdcall BindMsgFun(CLIENTMSGFUN  fun) = 0;
	virtual void __stdcall BindNetInfoFun(CLIENTMSGFUN fun) = 0;

	//
	// 网络信息内存由com组件自身分配,所以安全的释放模式应该由com组件自身释放
	//
	virtual void __stdcall FreeMemery(char* & ptr) = 0;

	//
	// 异步操作
	//
	virtual void __stdcall PostConnect() = 0;
	virtual void __stdcall PostReceive() = 0;
	virtual void __stdcall PostSend(const char* send, __int64 size) = 0;

	//
	// 同步操作
	//
	virtual void __stdcall Connect() = 0;
	virtual __int64 __stdcall Send(const char* send, __int64 size, __int64 timeout = 5000) = 0;
	virtual __int64 __stdcall Recv(char*& str, __int64 timeout = 5000) = 0;

	//
	// 适用于关于命令协议的操作
	//
	virtual __int64 __stdcall Write(const char* cmd, __int64 size,__int64 timeout = 5000) = 0;
	virtual __int64 __stdcall Read(char*& result, __int64 timeout = 5000) = 0;

	//
	// 开启线程
	//
	virtual void __stdcall Run() = 0;

	//
	// 查询socket的状态，检查是否超时
	//
	virtual bool __stdcall Timeout() = 0;

	//
	// 连接状态
	//
	virtual bool __stdcall IsConnected() = 0;
	virtual void __stdcall Close() = 0;

	virtual void __stdcall ShowConsole(bool isshow) = 0;
	virtual void __stdcall SetLoggerFile(const char* filename) = 0;
};

