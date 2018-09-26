

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
#include "DxLabel.h"
#include "DxScrollBar.h"
#include "DxCaret.h"

namespace DxUI{

	//+-----------------------
	//
	// class CDxTextBlock
	//
	//+-----------------------
	class DXUI_API CDxTextBlock : public CDxLabel
	{
		DECLARE_CLASS(CDxTextBlock)
	public:
		CDxTextBlock();
		~CDxTextBlock();

		DXWindowType	GetWindowSelfDesc() const;
		SIZE			GetHintSize() const;
		void			SetFocus();
		void			KillFocus();
		void			SetSelectedBkColor(const DxColor& col);
		void			SetSelectedTxtColor(const DxColor& col);
		bool			OnNotify(DXNotifyUI* NotifyEvent);
		void			OnTextFormat();
		void			OnTextChangedEvent();
		unsigned		GetMaxCharNum() const;
		void			SetMaxCharNum(unsigned num);
		void			ReceiveEvent(CDxUIEvent* e);
		virtual MString GetSelectedText() const;
		virtual void	ReplaceSelectedText(const MString& Text);
		virtual void	InsertText(int pos, const MString& Text);
		virtual void	OnDealTextForLine(unsigned LineIndex);

	msignals:
		void			TextUpdated(MString Text, CDxWidget* sender); // 文本有更新
		TL::MTLVoidEvent<MString, CDxWidget*> Event_TextUpdated;

	public:
		void			UpdateTextEffects();
		void			OnRendWindow(IPainterInterface* painter);
		
	protected:
		CDxVerticalScrollBar   mVerticalBar;
		CDxCaret			   mCaret;
		int					   mCaretIndex{ 0 };     // 光标索引
		int					   mShowLines{ 0 };      // 显示了多少行
		int					   mLastLineIndex{ -1 }; // 最终显示的行
		int					   mSelectedStartIndex{ 0 }; // 选中的开始位置
		int					   mSelectedStopIndex{ 0 };  // 选中的结束位置
		bool				   bIsShowScrollBar{ false };
		bool				   bIsTrailingHit{ false };
		bool				   bIsClicked{ false };
		unsigned			   mMaxCharNum;// 最大字符
		DxColor				   mSelectedBkColor;
		DxColor				   mSelectedTxtColor;
		bool				   bIsReadOnly{ true };

	protected:
		MString				   mPlaceholderText; // 占位文本
		bool				   bIsHasFocus{ false };
	};
}


