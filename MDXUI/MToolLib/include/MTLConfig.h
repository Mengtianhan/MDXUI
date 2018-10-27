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
#include <functional>

//
// 该宏用于配合 NotifyPropertyChanged 函数的调用
// 使用该宏自动生成函数名,以及一个成员函数指针
// 可以大大减少 NotifyPropertyChanged 的调用麻烦度
//
#ifndef MPropertyNotifer
#define MPropertyNotifer(className,memFun) #memFun,&className::memFun
#endif


//
//  模拟信号,MSLOT是模板函数
//
#ifndef MSIGNAL
#define MSIGNAL(className,memFun,obj) #memFun,&className::memFun,obj
#endif

//
// 将代码类型等等转换为字符串表现形式
//
#ifndef CONVERTTOSTRING
#define CONVERTTOSTRING(other) #other
#endif


//
// 宣告Class信息
// 在class的声明中使用
//
#ifndef DECLARE_CLASS
#define DECLARE_CLASS(className) \
	private:\
	MString ____##className##Name##____{ #className }; \
	static  TL::MClassDynamic<className::MClassFactory>* pclassName; \
	public:\
	static void* MCreateInstance(){\
\
	return new className; \
	}\
	virtual const MString& MClassName() const{\
		\
		return ____##className##Name##____; \
	}
#endif


//
// 针对 DECLARE_CLASS 的实现
//
#ifndef IMPLEMENT_CLASS
#define IMPLEMENT_CLASS(className) \
	TL::MClassDynamic<className::MClassFactory>* className::pclassName = \
	new TL::MClassDynamic<className::MClassFactory>(#className, \
	&className::MCreateInstance);
#endif

#ifndef MCreateInstanceFun
typedef std::function<void*()> MCreateInstanceFun;
#endif