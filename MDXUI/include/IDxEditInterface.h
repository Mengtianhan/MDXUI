


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
#include "DxConfig.h"

namespace DxUI{


	//+-------------------------------
	//
	// 定义一个文本输入的接口
	//
	//+-------------------------------
	class DXUI_API IDxEditInterface : public IUnknown
	{
	public:
		virtual ULONG STDMETHODCALLTYPE AddRef() override;
		virtual ULONG STDMETHODCALLTYPE Release() override;
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,
			void **ppvObject) override;


		static HRESULT SetForegroundBrush(IDWriteTextLayout * textLayout,
			ID2D1Brush * brush,
			DWRITE_TEXT_RANGE textRange);

		ID2D1Brush * GetForegroundBrush()
		{
			return pForegroundBrush;
		}


		static HRESULT SetBackgroundBrush(IDWriteTextLayout * textLayout,
			CDxBackgroundMode backgroundMode,
			ID2D1Brush * brush,
			DWRITE_TEXT_RANGE textRange);

		void GetBackgroundBrush(CDxBackgroundMode * pMode, ID2D1Brush ** pBrush)
		{
			*pMode = mBackgroundMode;
			*pBrush = pBackgroundBrush;
		}


		static HRESULT SetUnderline(IDWriteTextLayout * textLayout,
			CDxUnderlineType type,
			ID2D1Brush * brush,
			DWRITE_TEXT_RANGE textRange);

		void GetUnderline(CDxUnderlineType * pType, ID2D1Brush ** pBrush)
		{
			*pType = mUnderlineType;
			*pBrush = pUnderlineBrush;
		}

	
		static HRESULT SetStrikethrough(IDWriteTextLayout * textLayout,
			int count,
			ID2D1Brush * brush,
			DWRITE_TEXT_RANGE textRange);

		void GetStrikethrough(int * pCount, ID2D1Brush ** pBrush)
		{
			*pCount = mStrikethroughCount;
			*pBrush = pStrikethroughBrush;
		}


		static HRESULT SetOverline(IDWriteTextLayout * textLayout,
			bool hasOverline,
			ID2D1Brush * brush,
			DWRITE_TEXT_RANGE textRange);

		void GetOverline(bool * pHasOverline, ID2D1Brush ** pBrush)
		{
			*pHasOverline = mHasOverline;
			*pBrush = pOverlineBrush;
		}


		static HRESULT SetHighlight(IDWriteTextLayout * textLayout,
			ID2D1Brush * brush,
			DWRITE_TEXT_RANGE textRange);


		ID2D1Brush * GetHighlight()
		{
			return pHighlightBrush;
		}


	protected:
		IDxEditInterface();            
		IDxEditInterface * Clone();

		static HRESULT SetFormatting(IDWriteTextLayout * textLayout,
			DWRITE_TEXT_RANGE textRange,
			std::function<void(IDxEditInterface *)> setField);

	protected:
		LONG								mRefCount{ 0 };
		ID2D1Brush*							pForegroundBrush{ nullptr };
		CDxBackgroundMode					mBackgroundMode{ CDxBackgroundMode::Dx_TextHeight };
		ID2D1Brush*							pBackgroundBrush{ nullptr };

		CDxUnderlineType                    mUnderlineType{ CDxUnderlineType::Dx_None };
		ID2D1Brush*							pUnderlineBrush{ nullptr };

		int									mStrikethroughCount{ 0 };
		ID2D1Brush*							pStrikethroughBrush{ nullptr };

		bool								mHasOverline{ false };
		ID2D1Brush*							pOverlineBrush{ nullptr };

		ID2D1Brush*							pHighlightBrush{ nullptr };
	};
}


