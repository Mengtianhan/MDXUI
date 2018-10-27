
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
#include <WinMain.hpp>
#include <Windows.h>
#include <MSingleton.h>
#include "MString.h"
#include "DxConfig.h"
#include <condition_variable>
#include <map>
#include <MTLSlotEx.h>
#include <MMultType.h>
#include <MTLEvent.h>

using mj::MString;
class IFactoryInterface;

namespace TL{
	template<class...Args>
	class MTLAsyncVoidEvent;
}

namespace DxUI{
	class IWindow;
	class DXUI_API CDxApplication 
	{
	public:
		typedef IWindow*(*CreateInstanceFun)();
		typedef IFactoryInterface*(*CreateFactoryFun)();

		//
		// CreateInstance
		// CreateFactory
		// 具有这两个导出函数名的模块可使用下面的接口加载窗口或者工厂出来
		//
	public:
		CDxApplication(HINSTANCE __hInstance = nullptr);
		~CDxApplication();

		static CDxApplication* InstanceWithArgs(HINSTANCE __hInstance);
		static CDxApplication* Instance();
		static void Destroy();

		static void			 SetFont(const MString& fontName, unsigned fSize, bool isBold = false, bool isItalic = false);
		static HINSTANCE	 GetHInstance();
		static HFONT		 GetFont();
		static DXFontInfo	 GetFontInfo();
		static MString		 GetExePath();
		static MString		 GetCurrentTime();
		static MString		 GetUUIDStr();


		//
		// 从dll模块中加载窗口模块
		//
		static IWindow* LoadWindowFromDll(const MString& dllname);
		static IFactoryInterface* CreateFactory(const MString& dllname);

		//
		// 写注册表
		//
		static bool Register(HKEY syskey,
			const MString& usekey,
			const MString& subkey,
			const MString& value);

		static MString ReadRegister(HKEY syskey,
			const MString& usekey,
			const MString& subkey);

		//
		// 卸载注册表
		//
		static bool UnRegister(HKEY syskey, const MString& useKey);


		//
		// 获取当前线程id
		//
		static unsigned CurrentThreadID();


		//
		// 获取主线程id
		//
		static unsigned MainThreadID();

		//
		// 条件变量
		//
		static void SetActiveHWND(HWND  hwnd);
		static HWND GetActiveHWND();


		//
		// 跨线程时如果想要将参数传递到主线程中可以使用该接口
		// 注意参数传递过程的有效性
		//
		static void PostMainThread(ulong message, ulong wParam, ulong lParam);

		//
		// 在其他线程中如果想要在主线程中执行函数时可以使用下面的接口
		//
		static void AsyncRunFunction(std::function<void()> Fun);
		static void AsyncRunFunction(void(*Fun)(TL::MTLObject*),TL::MTLObject* Obj);


		//
		// Args 最前最多支持4个
		//
		template<class T,class...Args>
		static void AsyncRunFunction(void(*fun)(T, Args...), const T& val, const Args&...args){
			std::function<void()> Fun = TL2::MSLOT(fun, val, args...);
			AsyncRunFunction(Fun);
		}

		template<class T, class...Args>
		static void AsyncRunFunction(void(T::*fun)(Args...), T* obj, const Args&...args){
			std::function<void()> Fun = TL2::MSLOT(fun, obj, args...);
			AsyncRunFunction(Fun);
		}

		template<class T>
		static void AsyncRunFunction(void(T::*fun)(), T* obj){
			std::function<void()> Fun = TL::MSLOT(fun, obj);
			AsyncRunFunction(Fun);
		}

		template<class...Args>
		static void AsyncRunFunction(TL::MTLAsyncVoidEvent<Args...>& obj, const Args&...args){
			typedef TL::MTLAsyncVoidEvent<Args...> Type;
			Type* Ptr = &obj;
			AsyncRunFunction(&Type::Run, Ptr, args...);
		}


	
		//
		// 如果不知道如何使用的时候就最好不要使用
		//
		static void RunQueryFunction(MSG msg);

		static void SetAppHintSize(const MString& FileName);
		static SIZE GetAppHintSize(const MString& ClassName);



		//
		// 消息循环
		//
		void				 Quit();
		int					 Run();


		//
		// 保存主窗口句柄
		//
		void				SaveMainHwnd(HWND hwnd);
		HWND				GetMainHwnd() const;

	msignals:
		//
		// 自定义事件传递出来
		// 参数分别是 message,wParam,lParam
		//
		static TL::MTLVoidEvent<ulong, ulong, ulong>  Event_Message;


	private:
		bool				bIsDone{ false };
		HWND				mMainHwnd{ nullptr };
		static HINSTANCE	hInstance;
		static DXFontInfo	sDefaultFontInfo;
		static HFONT		sDefaultFont;

	private:
		static std::map<MString, CreateInstanceFun> mModuleFuns;
		static std::map<MString, CreateFactoryFun>  mFactoryFuns;
		static std::map<MString, SIZE>   s_ControlHintSize;
	protected:
		static CDxApplication*	 __sPtr;
		static unsigned  mMainThreadID;
		static std::condition_variable	s_AppCondition;
		static HWND     m_ActiveHwnd;
		
	};

}


namespace TL{
	//
	// 使用operator()操作符让连接函数永远在主线程中执行
	//
	template<class...Args>
	class MTLAsyncVoidEvent : public MTLVoidEvent<Args...>{
	public:
		MTLAsyncVoidEvent(){}
		virtual ~MTLAsyncVoidEvent(){}

		void operator()(const Args&... arg){
			AsynRun(arg...);
		}

		void Run(Args... arg){
			MTLVoidEvent<Args...>::operator()(arg...);
		}

		void AsynRun(const Args&... arg){
			if (DxUI::CDxApplication::MainThreadID() == DxUI::CDxApplication::CurrentThreadID()){
				Run(arg...);
			}
			else{
				DxUI::CDxApplication::AsyncRunFunction(&MTLAsyncVoidEvent::Run, this, arg...);
			}
		}
	};
}