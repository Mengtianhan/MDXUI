

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
	//+------------------------------
	//
	// CDxLabel
	//
	//+------------------------------
	class DXUI_API CDxLabel : public CDxWindow
	{
		DECLARE_CLASS(CDxLabel)
	public:
		CDxLabel();
		~CDxLabel();

		bool					SetWindowStatus(DXWindowStatus status){ return false; }
		DXWindowType			GetWindowSelfDesc() const;
		SIZE					GetHintSize() const;
		void					OnResizeEvent();
		void					OnTextChangedEvent();
		virtual	void			OnTextFormat();


		void					ClearTextEffects();
		CDxEffects*				GetTextEffects();
		void					SetTextAlignment(DXTextAlignment Alignment); // 设置文本对齐方式
		void					SetTextFont(int iStart, int nLen, const MString& FontName, int fSize, bool isBold = false, bool isItalic = false); // 设定指定字符的字体
		void					SetTextFont(const MString& FontName, int fSize, bool isBold = false, bool isItalic = false); // 不被特殊指定的字体将使用该字体
		void					SetTextColor(int iStart, int nLen, const DxColor& col); // 设定指定字符的颜色
		void					SetTextSize(int iStart, int nLen, int fSize);           // 设定指定字符的大小
		void					SetTextSelectedColor(int iStart, int nLen, const DxColor& bKcol, const DxColor& TxtCol);// 设定选中时的背景色
		void					SetTextHighLightColor(int iStart, int nLen, const DxColor& col);				// 设定选中文本的高亮颜色
		void					OnRendWindow(IPainterInterface* painter);

	protected:
		CDxEffects				mTextEffective;
		MString					mRenderText;
		unsigned				mStartLine{ 0 };
		unsigned				mTatalLines;
		unsigned				mStartIndex{ 0 };  // 渲染文本的开始索引
		unsigned				mStopIndex{ -1 };   // 渲染文本的结束索引
		TL::Vector<DXLineTextInfo> mTextLinesInfo;
	};
}


