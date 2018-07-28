#pragma once

#include <mutex>

//
// 单线程模式
//
struct MSingleThreadMode{
	inline void Lock(){}
	inline void UnLock(){}
};

//
// 多线程模式
//
struct MMultThreadMode{
	inline void Lock(){
		mMutex.lock();
	}

	inline void UnLock(){
		mMutex.unlock();
	}
	std::mutex mMutex;
};