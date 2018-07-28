#pragma once
#include "IAsioInterface.h"

//
// 如果使用C++11那么使用函数对象,否则使用函数指针
//
#if defined(_MSC_VER) && (_MSC_VER > 1300)
typedef std::function<void()> THREADFUNTYPE;
#else
typedef void(*THREADFUNTYPE)()
#endif

interface __declspec(uuid("B39FC0B4-ED61-4EB5-8670-AB42AB10FA4D")) IThread : public IUnknown
{
	virtual unsigned  __stdcall AddFun(THREADFUNTYPE fun, unsigned num = 1) = 0;
	virtual void  __stdcall Run(unsigned thread = -1) = 0;
	virtual void  __stdcall Detach(unsigned thread = -1) = 0;
	virtual void  __stdcall Join(unsigned thread = -1) = 0;
	virtual void  __stdcall SetConditionVar(void * threadconditionvar, unsigned thread = -1) = 0;
	virtual void  __stdcall SetControlVar(bool* IsExit, unsigned thread = -1) = 0;
	virtual void  __stdcall Sleep(unsigned msec) = 0;
	virtual unsigned __stdcall CurrentThreadID() = 0;
};

//
// 默认提供的创建线程管理的工厂函数
//
inline IThread* CreateThreadPtr(){
	IThread* pThread = NULL;
	::CoCreateInstance(
		ASIOCLSID,
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IThread),
		(void**)&pThread
		);
	return pThread;
}

//
// 创建静态线程指针
//
inline IThread* CreateStaticThreadPtr(){
	static IThread* pThread = NULL;
	if (pThread)
		return pThread;
	::CoCreateInstance(
		ASIOCLSID,
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IThread),
		(void**)&pThread
		);
	return pThread;
}