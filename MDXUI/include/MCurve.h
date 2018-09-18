

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
	//+----------------------
	//
	// 标记信息
	//
	//+----------------------
	class DXUI_API MMarker{
	public:
		MMarker();
		~MMarker();
		void	SetBitmap(const MString& icon);
		DxColor	SetColor(const DxColor& col);
		void	SetShapeDatas(const TL::Vector<DxPointD>& points);
		void	SetPosition(const DxPointD& pos);
		void	SetName(const MString& Name);
		void	BindCurver(const MString& Curve);
		void	SetFixEnable(bool isFix);
		MString	GetBitmap() const;
		DxColor GetColor() const;
		TL::Vector<DxPointD> GetShapeDatas() const;
		MString GetName() const;
		MString	GetBindCurve() const;
		DxPointD GetPosition() const;
		bool	IsFix() const;
	private:
		MString		mIcon;
		DxColor		mColor;
		TL::Vector<DxPointD> mShapeDatas;
		MString		mName;
		MString		mCurveName;
		DxPointD	mPos;
		bool		bIsFix{ false };
		static  TL::Vector<DxPointD> sShapePoints;
	};



	//+-----------------------
	//
	// 曲线
	//
	//+-------------------------
	class DXUI_API MCurve
	{
	public:
		MCurve();
		~MCurve();

		void	SetName(const MString& Name);
		DxColor  SetColor(const DxColor& col);
		int		 SetSize(int size);
		void	 SetDatas(const TL::Vector<double>& y);
		void	 SetDatas(const TL::Vector<double>& x, const TL::Vector<double>& y);
		void	 SetDatas(const TL::Vector<DxPointD>& points);

		void	 Append(const TL::Vector<double>& x, const TL::Vector<double>& y);
		void	 Append(const TL::Vector<double>& y);
		void	 Append(double x,double y);
		void	 Append(double y);

		DxPointD GetMaxPoint() const;
		DxPointD GetMinPoint() const;
		TL::Vector<DxPointD> GetPoints() const;
		TL::Vector<double>   GetX() const;
		TL::Vector<double>	 GetY() const;
		double		GetMinX() const;
		double		GetMinY() const;
		double		GetMaxX() const;
		double		GetMaxY() const;

		DxColor		GetColor() const;
		int			GetSize() const;
		MString		GetName() const;

	private:
		DxColor				mColor;
		int					mSize;
		TL::Vector<double>	mXs;
		TL::Vector<double>	mYs;
		MString				mName;
	};



	//+---------------------------
	// 
	// Title
	//
	//+---------------------------
	class DXUI_API MPlotTitle{
	public:
		MPlotTitle();
		~MPlotTitle();

		void	   SetFont(const DXFontInfo& info);
		void	   SetFont(const MString& Fontname, int size, bool isbold, bool isitalic);
		void	   SetTitle(const MString& title);
		void	   SetColor(const DxColor& col);

		DXFontInfo GetFont() const;
		MString	   GetTitle() const;
		DxColor	   GetColor() const;
	private:
		MString		mTitle;
		DxColor		mColor;
		DXFontInfo	mFont;
	};


	//+------------------------------
	//
	// 平面绘图数据
	//
	//+------------------------------
	class DXUI_API MPlane{
	public:
		MPlane();
		~MPlane();


		void		SetPlane(const MString& Name, const TL::Vector<double>& xs, const TL::Vector<double>& ys, const mj::dmat& datas);
		void		SetPlane(const MString& Name, const TL::Vector<double>& xs, const TL::Vector<double>& ys);
		void		SetDatas(const mj::dmat& datas);
		void		SetDataByIndex(int row, int col, double val);
		void		SetDataByAxis(double x, double y, double val);
		void		ClearData(); 
		void		Clear();
		void		SetMapColorRange(const DxColor& MinColor, const DxColor& MaxColor);
		void		SetValueRange(double minValue, double maxValue);
		void		GetValueRange(double& minvalue, double& maxvalue);


		MString				GetName() const;
		TL::Vector<double>	GetX() const;
		TL::Vector<double>	GetY() const;
		const mj::dmat&		GetDatas() const;
		mj::dmat&			GetDatas();
		DxColor				GetMinColor() const;
		DxColor				GetMaxColor() const;
		double				GetDataByIndex(int row, int col) const;
		double				GetDataByAxis(double x, double y) const;

		DxColor				GetColor(const double& val) const;
		DxColor				GetColor(const double& val,const double& minval,const double& maxval) const;
		double				GetMinX() const;
		double				GetMinY() const;
		double				GetMaxX() const;
		double				GetMaxY() const;

	protected:
		MString				mName;
		TL::Vector<double>	mXs;
		TL::Vector<double>	mYs;
		mj::dmat			mDatas;
		DxColor				mMinColor;
		DxColor				mMaxColor;
		double				mMaxValue;
		double				mMinValue;
		double				mMinH;
		double				mMinS;
		double				mMinL;
		double				mMaxH;
		double				mMaxS;
		double				mMaxL;
	};


	//+--------------------------
	//
	// class MMeshData
	//
	//+---------------------------
	class DXUI_API MMeshData : public MPlane{
	public:
		MMeshData();
		~MMeshData();
	};
}

