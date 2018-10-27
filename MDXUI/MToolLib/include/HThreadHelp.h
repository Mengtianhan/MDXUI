///////////////////////////////////////////////////////////////////////////////
//
// MToolLib ---- Mengjin Tool Library
// Copyright (C) 2014-2018 by Mengjin (sh06155@hotmail.com)
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

//-------------------------------------------
// 多线程辅助操作
// 主要针对后台处理数据使用
// 建立在生产者和消费者模型上
//-------------------------------------------
#pragma once
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

class HThreadHelp
{
public:
	typedef std::function<void()> __Fun_Type; // 回调函数类型,有多参数需要进行操作
	HThreadHelp();
	~HThreadHelp();

	//------------------------------------------------
	// 设置需要使用作为消费者的线程数
	//------------------------------------------------
	void Set_Customer_Thread_Num(size_t num);

	//------------------------------------------------
	// 设置需要使用作为生产者的线程数
	//------------------------------------------------
	void Set_Producer_Thread_Num(size_t num);

	//-------------------------------------------------
	// 设置消费者线程函数
	//-------------------------------------------------
	void Set_Customer_Fun(__Fun_Type fun);

	//-------------------------------------------------
	// 设置生产者线程函数
	//-------------------------------------------------
	void Set_Producer_Fun(__Fun_Type fun);

	//-------------------------------------------------
	// 是否人为控制线程操作
	//-------------------------------------------------
	void Set_Control_Mode(bool isauto);

	//-------------------------------------------------
	// 开始工作,启动所有设置的线程
	//-------------------------------------------------
	void start();
	
	//-------------------------------------------------
	// 控制生产线程是否等待
	//-------------------------------------------------
	void Produce_Wait(bool iswait);

	//-------------------------------------------------
	// 控制消费者线程是否等待
	//-------------------------------------------------
	void Customer_Wait(bool iswait);
protected:
	void Thread_For_Producer();
	void Thread_For_Customer();
private:
	std::thread m_customer_thread[32];  // 最多8条线程
	std::thread m_producer_thread[32];  // 最多8条线程

	std::mutex  m_customer_mutex;
	std::mutex	m_producer_mutex;

	std::condition_variable m_customer_condition;
	std::condition_variable m_producer_condition;

	std::recursive_mutex	m_mutex1;
	std::recursive_mutex	m_mutex2;

	bool					b_is_exit{ false };
	bool					b_is_start{ false };
	bool					b_is_wait_for_producer{ false };
	bool					b_is_wait_for_customer{ false };
	bool					b_is_auto_control_thread{ true };

	size_t					m_customer_thread_num{ 0 };
	size_t					m_producer_thread_num{ 0 };
	unsigned long long		m_producer_num{ 0 };//生产次数
	unsigned long long		m_customer_num{ 0 };//消费次数

	__Fun_Type				m_producer_fun{ nullptr };
	__Fun_Type				m_customer_fun{ nullptr };
};

