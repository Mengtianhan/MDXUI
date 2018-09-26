

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
#include "MString.h"
#include "DxConfig.h"
#include <unordered_map>
#include "MDx11BufferManage.h"
#include "MDx11LayoutManage.h"
#include <ft2build.h>
#include "MDx11Camera.h"
#include "MDx11Point.h"
#include FT_FREETYPE_H

#define MCheneseFontTTF "simhei.ttf" // 使用该字体可以输入中文
#define MEnglishFontTTF "Arial.ttf"  // 处理引文字符
using mj::MString;

namespace DxUI{

	

	//+----------------------------
	//
	// D3D 文本绘制
	//
	//+----------------------------
	class DXUI_API MFTFont
	{
	public:
		struct FontVertex{
			XMFLOAT3 pos;
			XMFLOAT4 color;
			XMFLOAT2 tex;
		};
		typedef std::tuple<float, float, float, float, ID3D11ShaderResourceView*> CharBitmapType;
	public:
		MFTFont();
		MFTFont(const MString &fontname, const float& size, const bool& antialiased);
		~MFTFont();
		void Init(const MString &fontname, const float& size, const bool& antialiased);
		void InitFont(ID3D11Device*  pDevice, ID3D11DeviceContext* pContext);

		//
		// 受变换矩阵所影响的文本
		//
		void RenderText(
			const DxRectF& rect, const XMFLOAT4X4& worldMatrix,
			const MString& text,
			float z = 0.5, const DxPointF& Penpos = DxPointF(0.f, 40.f),
			const XMFLOAT4& color = XMFLOAT4(1.f, 1.f, 1.f, 1.f)
			);

		//
		// 所变换矩阵所影响的文本
		//
		void Render3DText(const XMFLOAT3& pos,
			const XMFLOAT4& col,
			const MString& text,
			const XMFLOAT4X4& worldMatrix,
			unsigned Textsize = -1,
			MDx11Axis Axis = MDx11XAxis);

		void RenderText(const MString& text);

		void OnSizeChanged(const DxRectF& size);
		void StartPoint(const DxPointF& pt);
		void SetFontColor(const XMFLOAT4& color);
		void SetFontName(const MString& fontname);
		void SetFontSize(unsigned fontsize);
		void SetFont(const MString& fontname, const unsigned fontsize);


		ID3D11ShaderResourceView* GetShaderView(const MString& text, std::pair<int, int>& dimession); // 获取字符串的纹理

		int  GetTextWidth(wchar_t ch);  // 字符的宽度
		int  GetTextHeigh();            // 字符的高度
		int  GetTextLineHeigh();        // 行的高度

		MString	GetFxStr() const;


		MFTFont(const MFTFont& other) = delete;
		MFTFont& operator=(const MFTFont& other) = delete;
	protected:
		void UpdateFont();
		void Free();
		ID3D11ShaderResourceView* GetShaderView(wchar_t ch);


	private:
		ID3D11Device*					p_Device{ nullptr };
		ID3D11DeviceContext*			p_DeviceContex{ nullptr };

		FT_Face							mFTFace{ nullptr };
		FT_GlyphSlot					mFTSlot{ nullptr };

		unsigned						mCharWidth;
		unsigned						mCharHeight;

		ID3D11Buffer*					pVBBuffer{ nullptr };
		ID3D11Buffer*					pIndexBuffer{ nullptr };

		ID3DX11EffectShaderResourceVariable* pFontTextureVariable{ nullptr };


		float							mFontSize;
		bool							bIsAntiAliased;

		float							mAscender;
		float							mDescender;
		float							mFontHeight;
		float							mAddance;
		float							mToplineHeigh;

		MString							mFontName;

		MDx11LayoutManage*				pLayoutManage{ nullptr };
		ID3D11InputLayout*				pLayout{ nullptr };
		ID3DX11Effect*					pEffect{ nullptr };
		MDx11BufferManage*				pBufferManage{ nullptr };
		ID3D11ShaderResourceView*		pFontTexture{ nullptr };

		std::unordered_map<wchar_t, CharBitmapType> mCharTextMap;
		std::unordered_map<MString, ID3D11ShaderResourceView*>  mTextTures;
		std::unordered_map<MString, std::pair<int, int>>  mTextDimension;

		DxRectF							mWindowRect;
		DxRectF							mRendArear;
		DxPointF						mPointPen;
		DxPointF						mOldPointPen;
		XMFLOAT4                        mFontColor;
		FontVertex						mFontVectex[4];
		FontVertex                      mClientRect[4]; // 指定渲染区域
		MDx11Camera                     mCam;
	private:
		DxPointF						mClickPos;
		DxRectF							mClickRect;
	};
}


