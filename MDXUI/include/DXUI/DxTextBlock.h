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


