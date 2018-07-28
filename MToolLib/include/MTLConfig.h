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