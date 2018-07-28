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
