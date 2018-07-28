#pragma once
#include "DxConfig.h"

namespace DxUI{

	class CDxWidget;

	//+--------------------------
	//
	// CDxEffects 效果信息
	//
	//+-------------------------
	class DXUI_API CDxEffects
	{
	public:

		//+-------------------
		//
		// 文本选中信息
		//
		//+-------------------
		struct TextSelectedInfo{
			int		 mStart{ 0 };
			int		 mLen{ 0 };
			DxColor	 mBackColor;
			DxColor	 mFrontColor;
		};

		//+------------------
		//
		// 文本字体
		//
		//+------------------
		struct TextFontInfo : public DXFontInfo{
			int		 mStart{ 0 };
			int		 mLen{ 0 };
		};


		//+--------------------
		//
		// 文本颜色
		//
		//+---------------------
		struct TextColorInfo{
			int		mStart{ 0 };
			int		mLen{ 0 };
			DxColor mColor;
		};


		//+----------------
		//
		// 字体大小
		//
		//+----------------
		struct TextSizeInfo{
			int		mStart{ 0 };
			int		mLen{ 0 };
			int		mSize{ 0 };
		};


		//+--------------------------
		//
		// 下划线
		//
		//+---------------------------
		struct TextUnderLinerInfo{
			int		mStart{ 0 };
			int		mLen{ 0 };
			DxColor	mColor;
			CDxUnderlineType mType{ Dx_Single };
		};


		//+---------------------
		//
		// 上划线
		//
		//+----------------------
		struct TextOverLinerInfo{
			int		mStart{ 0 };
			int		mLen{ 0 };
			DxColor	mColor;
		};

		//+---------------------
		//
		// 高亮
		//
		//+----------------------
		struct TextHighLightInfo{
			int		mStart{ 0 };
			int		mLen{ 0 };
			DxColor	mColor;
		};

		//+-----------------------
		//
		// 中划线
		// 
		//+-----------------------
		struct TextStrikethroughInfo{
			int		mStart{ 0 };
			int		mLen{ 0 };
			DxColor	mColor;
			int		mCounts{ 1 };
		};


		//+--------------
		//
		// 渐变色类型
		// 线性
		// 放射性
		//
		//+--------------
		enum GradientType{
			Dx_Linear = 0,
			Dx_Radia
		};


		//+----------------
		//
		// 特效类型
		// 图形
		// 色彩
		// 双
		//
		//+----------------
		enum EffectsType{
			Dx_ImageType,
			Dx_ColorType,
			Dx_ImageAndColorType,
		};


		//+----------------
		//
		// 几何图形
		//
		//+-----------------
		enum GeometyShape{
			Dx_Line_Shape = 0,    // 绘制一条线段
			Dx_Fill_Shape,    // 填充一个几何图形
			Dx_Draw_Shape,    // 绘制一个几何图形边框
			Dx_Draw_Null
		};

	public:
		CDxEffects();
		~CDxEffects();


		void	BindWidget(CDxWidget* window);
		void	SetCurrentStatus(DXWindowStatus status); // 设置当前状态
		void	SetColors(DXWindowStatus status, const std::vector<DxColor>& cols,const std::vector<double>& points);  // 设置颜色
		void	SetColor(DXWindowStatus status, const DxColor& col);
		void	SetBitmaps(DXWindowStatus status, const MString& bitmap,int w = -1,int h = -1); // 设置位图
		void	SetResource(DXWindowStatus status, const MString& idType, LONG ID, int w = -1, int h = -1);
		void	SetGradientDirect(DXDirection dirct); // 设置渐变方向，仅对线性渐变有效
		void	SetGradientType(GradientType type);  // 设置渐变类型 线性或者放射性
		void	SetDisabelColor(const DxColor& col);
		void	SetGeometryShapeType(GeometyShape type);
		void	SetGeometryShapeData(const TL::Vector<TL::Vector<DxPointD>>& points);
		void	SetGeometryShapeColor(DXWindowStatus status, const DxColor& col);
		void	SetOpacity(double val);



		//
		// 设置显示区域
		//
		void	SetShowRegion(const RECT& rc, const SIZE& randius, DXShape shap = Dx_Rectangle);  // 设置显示区域 仅对位图有效
		void	SetShowRegion(const SIZE& randius, DXShape shap = Dx_Rectangle);  // 设置显示区域 仅对位图有效
		void	SetShowRegion(const std::vector<DxPointD>& pts); // 设置显示区域,对位图或者其他都有效

		//
		// 文本效果
		//
		void	SetDefaulFont(const MString& fontName, int fSize, bool isBold = false, bool isItalic = false); // 设置文本默认字体


		void	ClearTextSelected();
		void	SetTextSelectedRange(int iStart, int nLen, const DxColor& cBack, const DxColor& cFront); // Set 清除旧数据 并设置新数据
		void	AddTextSelectedRange(int iStart, int nLen, const DxColor& cBack, const DxColor& cFront); // Add 追加新数据

		void	ClearTextFontInfo();
		void	SetTextFontInfo(int iStart, int nLen, const MString& frontName, int fSize, bool isBold = false, bool isItalic = false); // Set 清除旧数据 并设置新数据
		void	AddTextFontInfo(int iStart, int nLen, const MString& frontName, int fSize, bool isBold = false, bool isItalic = false); // Add 追加新数据

		void	SetTextFontProperty(int iStart, int nLen, bool isBold, bool isItalic = false);
		void	AddTextFontProperty(int iStart, int nLen, bool isBold, bool isItalic = false);

		void	ClearTextFontSize();
		void	SetTextFontSize(int iStart, int nLen, int fSize); // Set 清除旧数据 并设置新数据
		void	AddTextFontSize(int iStart, int nLen, int fSize); // Add 追加新数据

		void	ClearTextColor();
		void	SetTextColor(int iStart, int nLen, const DxColor& col); // Set 清除旧数据
		void	AddTextColor(int iStart, int nLen, const DxColor& col);// Add 追加新数据

		void	ClearUnderLine();
		void	SetUnderLine(int iStart, int nLen, const DxColor& col,CDxUnderlineType type);// Set 清除旧数据 并设置新数据
		void	AddUnderLine(int iStart, int nLen, const DxColor& col,CDxUnderlineType type);// Add 追加新数据

		void	ClearOverLine();
		void	SetOverLine(int iStart, int nLen, const DxColor& col);// Set 清除旧数据 并设置新数据
		void	AddOverLine(int iStart, int nLen, const DxColor& col);// Add 追加新数据

		void	ClearStrikeThrough(); 
		void	SetStrikeThrough(int iStart, int nLen, const DxColor& col, int counts = 1);// Set 清除旧数据 并设置新数据
		void	AddStrikeThrough(int iStart, int nLen, const DxColor& col, int counts = 1);// Add 追加新数据


		void	ClearHighlight();
		void	SetHighlight(int iStart, int nLen, const DxColor& col);// Set 清除旧数据 并设置新数据
		void	AddHighlight(int iStart, int nLen, const DxColor& col);// Add 追加新数据

		void	SetTextAlignment(DXTextAlignment alignment); // 设置文本对齐方式
		void	SetTextDefaultColor(const DxColor& col);     // 设置默认文本颜色
		void	SetTextDefaultColor(DXWindowStatus windowstatus, const DxColor& col);
		void	ClearTextDefaultColor();


		CDxWidget*					GetWindow() const;
		DXWindowStatus				GetCurrentStatus() const;
		EffectsType					GetEffectType() const;
		DXDirection					GetDirection() const;
		const std::vector<DxColor>& GetColors() const;
		const std::vector<double>&	GetPoints() const;
		MString						GetBitmap() const;
		std::pair<MString, LONG>    GetResource() const;
		void						QueryImageSize(int& w, int & h) const;
		RECT						GetMaskRect() const;
		SIZE						GetMaskRadius() const;
		DXShape						GetMaskShape() const;
		GradientType				GetGradientType() const;
		GeometyShape				GetGeometryShapeType() const;
		DxColor						GetGeometryShapeColor() const;
		double						GetOpacity() const;
		const TL::Vector<TL::Vector<DxPointD>>& GetGeometryShapeData() const;

		
		const TL::Vector<DxPointD>&	GetGeometryPoints() const;
		const TL::Vector<TextSelectedInfo>& GetTextSelectedInfo() const;
		const TL::Vector<TextFontInfo>& GetTextFontInfo() const;
		const TL::Vector<TextColorInfo>& GetTextColorInfo() const;
		const TL::Vector<TextSizeInfo>&  GetTextSizeInfo() const;
		const TL::Vector<TextUnderLinerInfo>& GetTextUnderLineInfo() const;
		const TL::Vector<TextOverLinerInfo>&  GetOverLineInfo() const;
		const TL::Vector<TextStrikethroughInfo>& GetStrikethroughInfo() const;
		const TL::Vector<TextHighLightInfo>& GetHighLightInfo() const;
		DXTextAlignment				GetTextAlignment() const;
		const DXFontInfo& 			GetDefaultFont() const;
		const DxColor&				GetDefaultColor() const;

	private:
		CDxWidget*											pOwnWindow{ nullptr };
		std::map<DXWindowStatus, std::vector<DxColor>>		mColors;
		std::map<DXWindowStatus, std::vector<double>>		mPoints;
		std::map<DXWindowStatus, MString>					mBitmaps;
		std::map<DXWindowStatus, std::pair<MString,LONG>>	mReSourceMap;
		std::map<DXWindowStatus, DxColor>					mTextColMap;
		DXWindowStatus										mCurrentStatus;
		DXDirection											mDirection;
		GradientType										mGradientType;
		int													mImageWidth;
		int													mImageHeight;
		double												mOpacity{ 1.0 };
		RECT												mClipRect;
		SIZE												mClipRadius;
		DXShape												mClipShape;
		DxColor												mDisableColor;

		//+------------
		//
		// 外观
		//
		//+------------
		TL::Vector<DxPointD>							   mGeometyPoints;


		//+------------------
		//
		// 几何图形
		//
		//+------------------
		TL::Vector<TL::Vector<DxPointD>>					mGeometryShapes;
		GeometyShape										mGeometryType;
		std::map<DXWindowStatus, DxColor>					mGeometryColors;


		//+------------
		//
		// 字符效果
		//
		//+------------
		DXFontInfo											mDefaultFont;
		DxColor												mTextDefaultColor;
		TL::Vector<TextSelectedInfo>						mTextSelectedVec;
		TL::Vector<TextFontInfo>							mTextFontVec;
		TL::Vector<TextColorInfo>							mTextColorVec;
		TL::Vector<TextSizeInfo>							mTextSizeVec;
		TL::Vector<TextUnderLinerInfo>						mTextUnderLinerVec;
		TL::Vector<TextOverLinerInfo>						mTextOverLineVec;
		TL::Vector<TextStrikethroughInfo>					mTextStrickeThroughVec;
		TL::Vector<TextHighLightInfo>						mTextHighLightVec;
		DXTextAlignment										mTextAlignment{ Dx_AlignNormal };
	};
}


