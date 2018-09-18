
///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "IDxEditInterface.h"

namespace DxUI{

	

	//+-----------------------
	//
	// 字体渲染类
	//
	//+-----------------------
	class DXUI_API CDxCharaterFormat : public IDWriteTextRenderer
	{
	public:
		CDxCharaterFormat();

		//=---------------------------------
		//  
		// 绘制文本
		//
		//+---------------------------------
		HRESULT Draw(ID2D1RenderTarget * renderTarget,
			IDWriteTextLayout * textLayout,
			D2D1_POINT_2F origin,
			ID2D1Brush * defaultBrush);


		//+----------------------------
		//
		// IUnknown 必须实现的三个接口
		//
		//+----------------------------
		virtual ULONG STDMETHODCALLTYPE AddRef() override;
		virtual ULONG STDMETHODCALLTYPE Release() override;
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) override;


		//+--------------------------------------
		//
		// IDWritePixelSnapping 的三个接口
		//
		//+---------------------------------------
		virtual HRESULT STDMETHODCALLTYPE IsPixelSnappingDisabled(void * clientDrawingContext,
			_Out_ BOOL * isDisabled) override;

		virtual HRESULT STDMETHODCALLTYPE GetPixelsPerDip(void * clientDrawingContext,
			_Out_ FLOAT * pixelsPerDip) override;

		virtual HRESULT STDMETHODCALLTYPE GetCurrentTransform(void * clientDrawingContext,
			_Out_ DWRITE_MATRIX * transform) override;


		//+------------------------------------
		//
		// IDWriteTextRenderer  的几个接口
		//
		//+-----------------------------------
		virtual HRESULT STDMETHODCALLTYPE DrawGlyphRun(void * clientDrawingContext,
			FLOAT baselineOriginX,
			FLOAT baselineOriginY,
			DWRITE_MEASURING_MODE measuringMode,
			_In_ const DWRITE_GLYPH_RUN * glyphRun,
			_In_ const DWRITE_GLYPH_RUN_DESCRIPTION *
			glyphRunDescription,
			IUnknown * clientDrawingEffect) override;

		virtual HRESULT STDMETHODCALLTYPE DrawUnderline(void * clientDrawingContext,
			FLOAT baselineOriginX,
			FLOAT baselineOriginY,
			_In_ const DWRITE_UNDERLINE * underline,
			IUnknown * clientDrawingEffect) override;

		virtual HRESULT STDMETHODCALLTYPE DrawStrikethrough(void * clientDrawingContext,
			FLOAT baselineOriginX,
			FLOAT baselineOriginY,
			_In_ const DWRITE_STRIKETHROUGH * strikethrough,
			IUnknown * clientDrawingEffect) override;

		virtual HRESULT STDMETHODCALLTYPE DrawInlineObject(void * clientDrawingContext,
			FLOAT originX,
			FLOAT originY,
			IDWriteInlineObject * inlineObject,
			BOOL isSideways,
			BOOL isRightToLeft,
			IUnknown * clientDrawingEffect) override;

	private:
		LONG				mRefNum{ 0 };
		ID2D1RenderTarget*	pRenderTarget{ nullptr };
		ID2D1Brush*			pDefaultBrush{ nullptr };
		enum  DXRenderPass
		{
			Dx_Initial = 0,
			Dx_Main,
			Dx_Final
		};

		DXRenderPass					 mRendPass;
		std::vector<DWRITE_LINE_METRICS> mLineMetrics;
		int                              mLineIndex;
		int                              mCharIndex;

		D2D1::Matrix3x2F				mDpiTransform;
		D2D1::Matrix3x2F				mRenderTransform;
		D2D1::Matrix3x2F				mWorldToPixel;
		D2D1::Matrix3x2F				mPixelToWorld;

		D2D1_RECT_F GetRectangle(const DWRITE_GLYPH_RUN * glyphRun,
			const DWRITE_LINE_METRICS * lineMetrics,
			FLOAT baselineOriginX,
			FLOAT baselineOriginY,
			CDxBackgroundMode backgroundMode);

		void FillRectangle(ID2D1RenderTarget * renderTarget,
			ID2D1Brush * brush,
			float x, float y,
			float width, float thickness,
			int offset);
	};

}

