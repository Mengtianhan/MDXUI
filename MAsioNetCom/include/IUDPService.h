#pragma once
#include <comdef.h>
#include <functional>

interface __declspec(uuid("3191A789-0BD9-4BE8-92AF-A6ADF5C4ECD0")) IUDPService : public IUnknown
{
	typedef std::function<void(const char*, __int64, const char*, unsigned __int16)> UDPSERVICEMSFFUN;
	typedef std::function<void(const char*, __int64,const char*, unsigned __int16)> UDPNETINFOFUN;

	//
	// 设置接收所使用的buffer大小
	//
	virtual void __stdcall ResizeBuffer(__int64 size) = 0;

	//
	// 创建一个发送端口
	//
	virtual void  __stdcall CreateSendPort(const char* strip, unsigned __int16 port,char*& outPort) = 0;

	//
	// 设置监听端口
	//
	virtual void __stdcall SetListenPort(unsigned short port) = 0;

	//
	// 设置是否为异步操作，如果异步操作的话所有接收数据都由回调操作，如果为同步,可以同步接收数据，但是依然可以异步发送和接收数据
	//
	virtual void __stdcall SetOperatorMode(bool isAsyc) = 0;

	//
	// 开始监听
	//
	virtual void __stdcall StartListen() = 0;

	//
	// 绑定消息回调函数
	//
	virtual void __stdcall BindNetInfoFun(UDPNETINFOFUN fun) = 0;
	virtual void __stdcall BindMsgFun(UDPSERVICEMSFFUN fun) = 0;

	//
	// 
	//
	virtual void __stdcall Run() = 0;


	//
	// 网络信息内存由com组件自身分配,所以安全的释放模式应该由com组件自身释放
	//
	virtual void __stdcall FreeMemery(char*& ptr) = 0;

	//
	// 异步发送和接收
	//
	virtual void __stdcall PostSendTo(const char* msg, __int64 size, const char* sendport) = 0;
	virtual void __stdcall PostSendTo(const char* msg, __int64 size,
		const char* ip, unsigned __int16 port) = 0;

	//
	// 异步广播数据
	//
	virtual void __stdcall PostBroad(const char* msg, __int64 len) = 0;

	//
	// 投递接收
	//
	virtual void __stdcall PostReceive() = 0;

	//
	// 发送和接收
	//
	virtual __int64 __stdcall SendTo(const char* send, __int64 size, const char* SendPort) = 0;
	virtual __int64 __stdcall RecvFrom(char*& str,char*& recvPort) = 0;

	//
	// 发送消息到指定指定ip和端口上
	//
	virtual __int64 __stdcall SendTo(const char* send, __int64 size, const char* ip,unsigned short port) = 0;

	//
	// 查询接收到数据的ip和端口
	//
	virtual  void __stdcall QuereIpAndPort(const char* portstrid, char*& ip, unsigned __int16& port) = 0;

};

