#pragma once
#include <comdef.h>

//
// 计时器
//
interface __declspec(uuid("97694F70-72BF-467E-9778-57E010586DE0")) ITimer : public IUnknown
{
	//
	// 创建一个新对象
	//
	virtual ITimer* __stdcall CreateNewObj() = 0;

	//
	// 获取从Reset开始的时间
	//
	virtual float TotalTime()const = 0;

	//
	// 获取两次调用Tick之间的时间
	//
	virtual float DeltaTime()const = 0;

	//
	// 重置计时器
	//
	virtual void Reset() = 0;

	//
	// 开始及时
	//
	virtual void Start() = 0;

	//
	// 暂停计时
	//
	virtual void Stop() = 0;

	//
	// 通过查询cpu的滴答数然后计数出两次调用该函数的时间间隔，秒为单位，通过DeltaTime可以获取
	//
	virtual void Tick() = 0;
};