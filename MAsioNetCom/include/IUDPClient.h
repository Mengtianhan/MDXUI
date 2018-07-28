#pragma once
#include <comdef.h>
#include <functional>

interface __declspec(uuid("2D96038E-2523-43BF-BEA2-590FBE69B7DE")) IUDPClient : public IUnknown
{
	typedef std::function<void(const char*, __int64, const char*, unsigned __int16)> UDPSERVICEMSFFUN;
	typedef std::function<void(const char*, __int64, const char*, unsigned __int16)> UDPNETINFOFUN;

	//
	// 创建一个新对象
	//
	virtual IUDPClient* __stdcall CreateNewObj() = 0;

	//
	// 设置接收所使用的buffer大小
	//
	virtual void __stdcall ResizeBuffer(__int64 size) = 0;

	//
	// 绑定消息回调函数
	//
	virtual void __stdcall BindNetInfoFun(UDPNETINFOFUN fun) = 0;
	virtual void __stdcall BindMsgFun(UDPSERVICEMSFFUN fun) = 0;

	//
	// 创建一个发送端口
	//
	virtual void  __stdcall CreateSendPort(const char* strip, unsigned __int16 port, char*& outPort) = 0;

	//
	// 设置是否为异步操作，如果异步操作的话所有接收数据都由回调操作，如果为同步,可以同步接收数据，但是依然可以异步发送和接收数据
	//
	virtual void __stdcall SetOperatorMode(bool isAsyc) = 0;

	//
	// 开启监听，一个socket对象只能run一次
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
	// 异步接收
	//
	virtual void __stdcall PostReceive() = 0;

	//
	// 发送和接收
	//
	virtual __int64 __stdcall SendTo(const char* send, __int64 size, const char* SendPort) = 0;
	virtual __int64 __stdcall RecvFrom(char*& str, char*& recvPort) = 0;

	//
	// 发送消息到指定指定ip和端口上
	//
	virtual __int64 __stdcall SendTo(const char* send, __int64 size, const char* ip, unsigned short port) = 0;

	//
	// 查询接收到数据的ip和端口
	//
	virtual  void __stdcall QuereIpAndPort(const char* portstrid, char*& ip, unsigned __int16& port) = 0;

};

