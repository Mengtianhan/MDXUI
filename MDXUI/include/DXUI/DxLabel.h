#pragma once
#include "DxWindow.h"


namespace DxUI{
	//+------------------------------
	//
	// CDxLabel
	//
	//+------------------------------
	class DXUI_API CDxLabel : public CDxWidget
	{
		DECLARE_CLASS(CDxLabel)
	public:
		CDxLabel();
		~CDxLabel();
		void					CreateHwnd(){}
		void					CreateHwnd(HWND parent){}
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


