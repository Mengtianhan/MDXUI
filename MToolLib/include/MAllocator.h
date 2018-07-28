#pragma once
#include "MEmptyType.h"
//
// Allocator 拥有 Instance 函数获取单例
// Allocator 分配内存
// Deallocator 释放内存
//

template<class Allocator = MEmptyType>
class MAllocator{
public:
	static void* operator new(std::size_t size);
	static void operator delete(void* p, std::size_t size);
};


//
// 分配器转发
//
template<class Allocator>
struct __Allocator_Dispatch__{
	static void* malloc(std::size_t size){
		return Allocator::Instance()->Allocator(size);
	}

	static void  free(void* p, std::size_t size){
		Allocator::Instance()->Deallocator(p, size);
	}
};

template<>
struct __Allocator_Dispatch__<MEmptyType>{
	static void* malloc(std::size_t size){
		return ::operator new(size);
	}

	static void  free(void* p, std::size_t size){
		::operator delete(p);
	}
};

//
// 分配器实现
//
template<class Allocator>
void* MAllocator<Allocator>::operator new(std::size_t size){
	return __Allocator_Dispatch__<Allocator>::malloc(size);
}

template<class Allocator>
void MAllocator<Allocator>::operator delete(void* p, std::size_t size){
	__Allocator_Dispatch__<Allocator>::free(p, size);
}
