

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DxConfig.h"

namespace DxUI{
	class CDxWidget;
	class CDxEffects;

	class DXUI_API CDxResource
	{
	public:
		typedef TL::MMNoSqlDB<std::string, DXWindowStatus, int, int, ID2D1RenderTarget*, ID2D1Bitmap*> CacheBitmapType;
		typedef TL::MMNoSqlDB<CDxWidget*, DXWindowStatus, int, int, ID2D1RenderTarget*, ID2D1Brush*> CacheBrushType;
		typedef TL::MMNoSqlDB<CDxWidget*, size_t, int, int, DXTextAlignment, ID2D1RenderTarget*, IDWriteTextLayout*, IDWriteTextFormat*>  CacheTextLayoutType;
	public:
		CDxResource();
		~CDxResource();

		//+-----------------------------------
		//
		// 静态函数
		//
		//+-----------------------------------
		static void						RegisterInstance(HINSTANCE hInstance);
		static HINSTANCE				GetInstance();
		static HWND 					FindHwndFromId(unsigned id);
		static IDWriteFactory*			GetDWriteFactory();
		static ID2D1Factory*			GetD2DFactory();
		static ID2D1HwndRenderTarget*	CreateRendTarget(HWND hwnd);
		static ID2D1RenderTarget*		CreateRendTargetForBitmap(int width,int height);
		static IWICBitmap*&				GetWICBitmap();// 可以从外部Release
		static IWICImagingFactory*		GetWICFactory();
		static void						SaveBitmapToFile(const MString& FileName, int width, int height);
		static ID2D1Bitmap*				CreateBitmap(LPCWSTR ImageName, ID2D1RenderTarget* RendTarget,unsigned width = 0, unsigned height = 0);
		static ID2D1Bitmap*				CreateBitmap2(const MString& idType,LONG ID, ID2D1RenderTarget* RendTarget,HDC hdc, unsigned width = 0, unsigned height = 0);
		static IDWriteTextFormat*       CreateTextFormat(LPCWSTR FontName, unsigned size, bool isBold = false, bool isI = false);
		static ID2D1Geometry*			CreateRectGeometry(const RECT& rect);
		static ID2D1Geometry*			CreateRoundRectGeometry(const RECT& rect, double radiux, double radiuy);
		static ID2D1Geometry*			CreateEllepseGeometry(const RECT& rect);
		static ID2D1Geometry*			CreateEllepseGeometry(const RECT& rect,double radiux,double radiuy);
		static ID2D1Geometry*			CreateGeometry(const std::vector<DxPointD>& points);
		static ID2D1Geometry*			CreateGeometry(const DxPolygonsType& points,TL::Vector<D2D1_RECT_F>& outRectIndex);
		static ID2D1Geometry*			CreateGeometryForLine(const DxLinesType& points);
		static ID2D1PathGeometry*		CreatePathGeometry();
		static ID2D1StrokeStyle*		CreateDashStrokeStyle();
		static int						GetLineTextLayoutLength(const MString& Text, CDxEffects* pEffects = nullptr);
		static int						GetLineTextLayoutHeight(const MString& Text, CDxEffects* pEffects = nullptr);
		static bool						GetLineTextLayoutInfo(const MString& Text,int& w,int& h, CDxEffects* pEffects = nullptr);
		static bool						GetTextLayoutInfo(const MString& Text, int& w, int& h, CDxEffects* pEffects = nullptr, int rectw = 10000, int recth = 10000);


		static void						CacheBitmap(const MString& FileName, DXWindowStatus status, int w, int h, ID2D1RenderTarget* target, ID2D1Bitmap* bitmap);
		static void						CacheBrush(CDxWidget* window, DXWindowStatus status, int w, int h, ID2D1RenderTarget* target, ID2D1Brush* brush);
		
		static ID2D1Bitmap*				GetBitmap(const MString& FileName, ID2D1RenderTarget* targe, DXWindowStatus status, int w, int h);
		static ID2D1Brush*				GetBrush(CDxWidget* window, ID2D1RenderTarget* target, DXWindowStatus status, int w, int h);

		static void						CacheTextLaout(CDxWidget* window, size_t Text, int w, int h, DXTextAlignment Alignment, ID2D1RenderTarget* target,
			IDWriteTextLayout* layout, IDWriteTextFormat* format);
		static void						GetTextLayout(CDxWidget* window, size_t Text, int w, int h, DXTextAlignment Alignment, ID2D1RenderTarget* target,
			IDWriteTextLayout*& layout, IDWriteTextFormat*& format);

		static void						ClearResourceForRendTarget(ID2D1RenderTarget* target);
	protected:
		static bool		InitDxResource();

		//+--------------------------------
		//
		// 静态数据
		//
		//+--------------------------------
		static HINSTANCE					 s_Instance;
		static ID2D1Factory*				 pD2DFactory;
		static IWICImagingFactory*           pWICFactory;
		static IWICBitmap*					 pWicBitmap;
		static IDWriteFactory*               pDWriteFactory;

		static CacheBitmapType				 sBitmapCache;
		static CacheBrushType				 sBurshCache;
		static CacheTextLayoutType			 sTextLayoutCache;
	};
}


