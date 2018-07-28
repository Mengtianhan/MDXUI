#pragma once
#include <comdef.h>
#include "IComBase.h"

interface __declspec(uuid("114E3952-89A5-455E-8A36-A4215EAF635A")) ISerialPort : public IUnknown
{
	//
	// 创建一个新对象
	//
	virtual ISerialPort* __stdcall CreateNewObj() = 0;
	virtual void __stdcall ResizeBuffer(__int64 size) = 0;

	//
	// 绑定委托函数
	//
	virtual void __stdcall BindMsgFun(CLIENTMSGFUN fun) = 0;
	virtual void __stdcall BindNetInfoFun(CLIENTMSGFUN fun) = 0;

	//
	// 网络信息内存由com组件自身分配,所以安全的释放模式应该由com组件自身释放
	//
	virtual void __stdcall FreeMemery(char* & ptr) = 0;

	//
	// 打开串口
	// SerialName 串口名
	// baud_rate 波特率 9600
	// isAsyni 是否异步 true 异步 false 同步
	// stopbit 停止位 1 || 2
	// parity 奇偶校验位 0 为 默认 >0 偶 <0 奇
	// character_size 数据位 默认 8
	// flow_control 流量控制 0 为系统默认 >0 硬件 <0 软件
	//
	virtual bool __stdcall Connect(const char* SerialName, unsigned baud_rate,
		bool isAsyni, int stopbit, int parity = 0, int character_size = 8, int flow_control = 0) = 0;

	//
	// 同步发送和接收
	//
	virtual int __stdcall Send(const char* send, __int64 size) = 0;
	virtual int __stdcall Receive(char*& rec) = 0;

	//
	// 同步操作,主要用于通信指令上的操作
	//
	virtual int __stdcall Write(const char* sendmsg, size_t size) = 0;
	virtual int __stdcall Read(char*& ch) = 0;

	//
	// 下面使用异步操作
	//
	virtual void __stdcall PostSend(const char* sendmsg, __int64 size) = 0;
	virtual void __stdcall PostReceive() = 0;

	//
	// 检测是否连接
	//
	virtual bool __stdcall IsConnected() = 0;
	virtual void __stdcall Close() = 0;
	virtual bool __stdcall IsOpen() = 0;

	virtual void __stdcall ShowConsole(bool isshow) = 0;
	virtual void __stdcall SetLoggerFile(const char* filename) = 0;
};

