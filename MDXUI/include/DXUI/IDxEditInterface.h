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


