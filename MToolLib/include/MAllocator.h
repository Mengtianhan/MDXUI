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
