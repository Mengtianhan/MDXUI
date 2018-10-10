

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
		void ShowByKeyWords(const MString& KeyWords);
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

