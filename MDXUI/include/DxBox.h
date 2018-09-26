

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
#include "DxDialog.h"
#include "DxPushButton.h"


namespace DxUI{
	//
	// 错误对话框
	//
	class DXUI_API CDxBoxImpl : public CDxDialog{
	public:
		CDxBoxImpl(){}
		~CDxBoxImpl(){}
		int	 Exec();
	};

	//
	//
	//
	class DXUI_API CDxErrorBox :
		public CDxBoxImpl
	{
	public:
		CDxErrorBox();
		~CDxErrorBox();

		void SetButtonText(const MString& ButtonText);
		void SetButtonIcon(const MString& ButtonIco);
		
	protected:
		void OnButton(bool isClicked, CDxWidget* sender);
	private:
		CDxPushButton  mConfirmButton;
	};


	//
	// 提示对话框
	//
	class DXUI_API CDxInfoBox :
		public CDxBoxImpl
	{
	public:
		CDxInfoBox();
		~CDxInfoBox();

		void SetButtonText(const MString& ButtonText);
		void SetButtonIcon(const MString& ButtonIco);
	protected:
		void OnButton(bool isClicked, CDxWidget* sender);
	private:
		CDxPushButton  mConfirmButton;
	};

	//
	// 选择对话框
	//
	class DXUI_API CDxQueBox :
		public CDxBoxImpl
	{
	public:
		enum ButtonID{ IID_Yes = 0, IID_Cancel };
	public:
		CDxQueBox();
		~CDxQueBox();
		void SetButtonText(ButtonID ButtonId, const MString& ButtonText);
		void SetButtonIcon(ButtonID ButtonId, const MString& ButtonIco);
	protected:
		void OnButton(bool isClicked, CDxWidget* sender);
	private:
		CDxPushButton  mConfirmButton;
		CDxPushButton  mCancelButton;
	};

	//
	// 选择对话框
	//
	class DXUI_API CDxQueInfoBox :
		public CDxBoxImpl
	{
	public:
		enum ButtonID{ IID_Yes = 0, IID_No, IID_Cancel };
	public:
		CDxQueInfoBox();
		~CDxQueInfoBox();
		void SetButtonText(ButtonID ButtonId, const MString& ButtonText);
		void SetButtonIcon(ButtonID ButtonId, const MString& ButtonIco);
	protected:
		void OnButton(bool isClicked, CDxWidget* sender);
	private:
		CDxPushButton  mConfirmButton;
		CDxPushButton  mNoButton;
		CDxPushButton  mCancelButton;
	};

	extern "C" DXUI_API int DxErrorBox(const MString& Text, CDxWidget* Parent = nullptr, const MString& YesStr = "确定", const MString& Title = "错误");
	extern "C" DXUI_API int DxInfoBox(const MString& Text, CDxWidget* Parent = nullptr, const MString& YesStr = "确定", const MString& Title = "提示");
	extern "C" DXUI_API int DxQueBox(const MString& Text, CDxWidget* Parent = nullptr, const MString& YesStr = "确定", const MString& NoStr = "取消", const MString& Title = "询问");
	extern "C"DXUI_API int DxQueInfoBox(const MString& Text, CDxWidget* Parent = nullptr, const MString& YesStr = "是", const MString& NoStr = "否", const MString& CancelStr = "取消", const MString& Title = "选择");
}
