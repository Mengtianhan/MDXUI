

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution
// Copyright (C) 2017-2018 by Mengjin (sh06155@hotmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
///////////////////////////////////////////////////////////////////////////////


#pragma once
#include <DxApplication.h>
#include <MString.h>
#include <thread>
using mj::MString;


//+------------------------------------------
//
// 用于监控时间信息
// 依赖 CDxApplication 的消息循环
//
//+------------------------------------------
class DXUI_API CDxTimeMonitor
{
public:
	CDxTimeMonitor();
	~CDxTimeMonitor();

	void	SetSecondsStep(int secs);


	int		GetYear() const;
	int		GetMonth() const;
	int		GetDay() const;
	int		GetDayOfWeek() const;
	int		GetHour() const;
	int		GetMinute() const;
	int		GetSecond() const;


	//
	// 日期变更以天为单位
	// 时间变更以小时为单位
	// 秒间隔可设置 最小为10
	// 日期格式为 年-月-日
	// 
	//
	TL::MTLAsyncVoidEvent<MString, MString, CDxTimeMonitor*>    Event_DataChanged; // 日期变更事件
	TL::MTLAsyncVoidEvent<int, int, CDxTimeMonitor*>			Event_HourChanged; // 时间变更事件
	TL::MTLAsyncVoidEvent<int, int, CDxTimeMonitor*>			Event_SecondChanged; // 秒变更事件


	//
	// 不依赖 消息循环
	//
	TL::MTLVoidEvent<MString, MString, CDxTimeMonitor*>		Event_DataChanged2; // 日期变更事件
	TL::MTLVoidEvent<int, int, CDxTimeMonitor*>				Event_HourChanged2; // 时间变更事件
	TL::MTLVoidEvent<int, int, CDxTimeMonitor*>				Event_SecondChanged2; // 秒变更事件

protected:
	void			EventRun();
private:
	int				mSeconds{ 10 };
	bool			bIsExt{ false };
	std::thread		mThread;
};

