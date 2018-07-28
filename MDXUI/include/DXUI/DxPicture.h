#pragma once
#include "DxWindow.h"

namespace DxUI{
	//+----------------------
	//
	// class CDxPicture
	// 用于展示图像
	//
	//+-----------------------
	class DXUI_API CDxPicture :
		public CDxWindow
	{
		DECLARE_CLASS(CDxPicture)
	public:
		CDxPicture();
		~CDxPicture();

		void PushPictureFile(const MString& FileName);
		void PopPicture();
		void ShowByFileName(const MString& FileName);
		void ShowByIndex(unsigned index);
		bool	OnNotify(DXNotifyUI* NotifyEvent);
		MString GetPicFileNameByIndex(unsigned index) const;
		void	OnRendWindow(IPainterInterface* painter);

	private:
		TL::Vector<MString>   mPictureFiles;
		MString				  mCurrentFile;
	};



	//+--------------------------------
	//
	// class CDxBitmapWindow
	// 直接绘制位图
	//
	//+--------------------------------
	class DXUI_API CDxBitmapWindow :
		public CDxWindow
	{
		DECLARE_CLASS(CDxBitmapWindow)
	public:
		CDxBitmapWindow();
		void AttachBitmap(ID2D1Bitmap* bitmap);
		ID2D1Bitmap* Dettach();
		bool	OnNotify(DXNotifyUI* NotifyEvent);
		void	OnRendWindow(IPainterInterface* painter);
	private:
		ID2D1Bitmap*  p_Bitmap{ nullptr };
	};

}

