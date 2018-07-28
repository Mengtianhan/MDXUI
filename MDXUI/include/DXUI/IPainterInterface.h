#pragma once
#include "DxConfig.h"

namespace DxUI{
	class CDxEffects;


	struct IPainterInterface{

		//+----------------------
		//
		// 预定义几个类型
		// 矩形
		// 圆角矩形
		// 椭圆形
		//
		//+-----------------------
		struct BaseShape
		{
			virtual DXShape ShapeType() const = 0;
			virtual double left() const = 0;
			virtual double top() const = 0;
			virtual double right() const = 0;
			virtual double bottom() const = 0;
			virtual double radiusx() const = 0;
			virtual double radiusy() const = 0;
		};

		struct Rectangle : BaseShape {
			Rectangle(double __x, double __y, double __w, double __h) :x(__x), y(__y), w(__w), h(__h){}
			double x, y, w, h;
			DXShape ShapeType() const{ return Dx_Rectangle; }
			double left() const{ return x; }
			double top() const{ return y; }
			double right() const{ return x + w; }
			double bottom() const{ return y + h; }
			double radiusx() const{ return 0; }
			double radiusy() const{ return 0; }
		};

		struct RoundedRectangle : Rectangle{
			RoundedRectangle(double __x, double __y, double __w, double __h, double __xr, double __yr) :Rectangle(__x, __y, __w, __h), xr(__xr), yr(__yr){}
			double xr, yr;
			DXShape ShapeType() const{ return Dx_RoundedRectangle; }
			double radiusx() const{ return xr; }
			double radiusy() const{ return yr; }
		};

		struct Ellipse : RoundedRectangle{
			Ellipse(double __x, double __y, double __w, double __h, double __xr, double __yr) :RoundedRectangle(__x, __y, __w, __h, __xr, __yr){}
			DXShape ShapeType() const{ return Dx_Ellipse; }
		};

		//+-----------------------------
		//
		// 二维平面变换矩阵
		//
		//+-----------------------------
		struct TransformMatrix{
			FLOAT _11;
			FLOAT _12;
			FLOAT _21;
			FLOAT _22;
			FLOAT _31;
			FLOAT _32;

			TransformMatrix(){
				_11 = 1.f;
				_12 = 0.f;
				_21 = 0.f;
				_22 = 1.f;
				_31 = 0.f;
				_32 = 0.f;
			}


			TransformMatrix(FLOAT f11, FLOAT f12, FLOAT f21, FLOAT f22, FLOAT f31, FLOAT f32){
				_11 = f11;
				_12 = f12;
				_21 = f21;
				_22 = f22;
				_31 = f31;
				_32 = f32;
			}

			TransformMatrix(const D2D1_MATRIX_3X2_F& mat){
				_11 = mat._11;
				_12 = mat._12;
				_21 = mat._21;
				_22 = mat._22;
				_31 = mat._31;
				_32 = mat._32;
			}

			operator D2D1_MATRIX_3X2_F() const{
				D2D1_MATRIX_3X2_F mat;
				mat._11 = _11;
				mat._12 = _12;
				mat._21 = _21;
				mat._22 = _22;
				mat._31 = _31;
				mat._32 = _32;
				return mat;
			}

		};

		//+-------------------------------------------
		//
		// 为绘制方便，下面的接口都得实现
		// 当然如果实际没有用处的可以简单的实现即可
		//
		//+-------------------------------------------
		virtual ~IPainterInterface(){};

		virtual void*  GetPainterEnginer() = 0;

		virtual void	BeginDraw() = 0;  // 开始绘制
		virtual void	Clear(const DxColor& col) = 0; // 使用特定色彩擦除背景
		virtual void	EndDraw() = 0;  // 结束绘制

		virtual void	DrawRectangle(const RECT& rc, const DxColor& col,double size) = 0;
		virtual void	DrawRoundedRectangle(const RECT& rc, const SIZE& radius, const DxColor& col, double size) = 0;
		virtual void	DrawEllipse(const RECT& rc, const SIZE& radius, const DxColor& col, double size) = 0;

		virtual void	DrawDashRectangle(const RECT& rc, const DxColor& col, double size) = 0;
		virtual void	DrawDashRoundedRectangle(const RECT& rc, const SIZE& radius, const DxColor& col, double size) = 0;
		virtual void	DrawDashEllipse(const RECT& rc, const SIZE& radius, const DxColor& col, double size) = 0;

		virtual void	FillRectangle(const RECT& rc, const DxColor& col) = 0;
		virtual void	FillRoundedRectangle(const RECT& rc, const SIZE& radius, const DxColor& col) = 0;
		virtual void	FillEllipse(const RECT& rc, const SIZE& radius, const DxColor& col) = 0;

		virtual void	FillRectangle(const RECT& rc, CDxEffects* pEffects) = 0;
		virtual void	FillRoundedRectangle(const RECT& rc, const SIZE& radius, CDxEffects* pEffects) = 0;
		virtual void	FillEllipse(const RECT& rc, const SIZE& radius, CDxEffects* pEffects) = 0;


		virtual void	FillBaseShape(const BaseShape& shape, const DxColor& col) = 0;
		virtual void	DrawBaseShape(const BaseShape& shape, const DxColor& col, double size) = 0;
		virtual void	DrawDashBaseShape(const BaseShape& shape, const DxColor& col, double size) = 0;

		virtual void	DrawBitmap(const RECT& rc, CDxEffects* pEffects) = 0;
		virtual void	DrawBitmap(const RECT& rc, const MString& bitmap,int w = -1,int h = -1) = 0;


		virtual void	DrawText(const MString& Text, const RECT& rc, CDxEffects* pEffects) = 0;  // 只绘制文本，超出区域不管 效率更高
		virtual void	DrawText(const MString& Text, const RECT& rc, const DxColor& col, const DXFontInfo& font,DXAlignment alignt) = 0; // 不使用效果直接绘制
		virtual void	DrawTextWithClip(const MString& Text, const RECT& rc, CDxEffects* pEffects, const RECT& cliprc = { 0, 0, 0, 0 }) = 0; // 绘制文本，超出区域裁剪

		virtual void	BeginClipRect(const RECT& rc) = 0;  // 在绘制之前调用
		virtual void	BeginClipRect(const std::vector<DxPointD>& points) = 0;  // 在绘制之前调用
		virtual void	EndClipRect() = 0;					// 在绘制完成之后调用


		//
		// 下面三个接口的使用需要明确知道void到底是什么
		//
		virtual void	BeginClipRect(const RECT& rc, void*& outptr1, void*& outptr2) = 0;  // 在绘制之前调用
		virtual void	BeginClipRect(const std::vector<DxPointD>& points, void*& outptr1, void*& outptr2) = 0;  // 在绘制之前调用
		virtual void	EndClipRect(void* outptr1, void* outptr2) = 0;					// 在绘制完成之后调用 BeginClipRect 返回的两个指针传递给 EndClipRect

		//
		// 特殊接口，主要用于获取文本的光标信息
		//
		virtual bool	QueCaretInformation(const MString& Text, const RECT& rc, CDxEffects* pEffects, const POINT& pt, int& index, POINT& outpt, double& outHeight, DxColor& outCol, bool& isTrailingHit,DXDrawTextInfo& metrics) = 0;
		virtual bool	QueCaretInformation(const MString& Text, const RECT& rc, CDxEffects* pEffects, int index, bool isTrailingHit, POINT& outpt, double& outHeight, DXDrawTextInfo& metrics) = 0;


		//
		// 特殊接口，用于计算给定字符所需要的长度
		//
		virtual int		GetLineTextLayoutLength(const MString& Text, CDxEffects* pEffects) = 0;

		//
		// 绘制线体
		// DrawLines效率更高
		//
		virtual void	DrawLine(const DxPointD& first, const DxPointD& second, const DxColor& col, double Size) = 0;
		virtual void	DrawLines(const std::vector<DxPointD>& points, const DxColor& col, double Size) = 0;


		virtual void	DrawDashLine(const DxPointD& first, const DxPointD& second, const DxColor& col, double Size) = 0;
		virtual void	DrawDashLines(const std::vector<DxPointD>& points, const DxColor& col, double Size) = 0;

		//
		// 绘制顶点
		// DrawPoints 效率更高
		//
		virtual void	DrawPoint(const DxPointD& pt, double radius, const DxColor& col, double Size = 1, int roundRadius = 5, DXShape shape = Dx_Ellipse) = 0;
		virtual void	DrawPoints(const std::vector<DxPointD>& points, double radius, const DxColor& col, double Size = 1, int roundRadius = 5, DXShape shape = Dx_Ellipse) = 0;


		//
		// 填充定点
		// FillPoints 效率更高
		//
		virtual void	FillPoint(const DxPointD& pt, double radius, const DxColor& col, int roundRadius = 5, DXShape shape = Dx_Ellipse) = 0;
		virtual void	FillPoints(const std::vector<DxPointD>& points, double radius, const DxColor& col, int roundRadius = 5, DXShape shape = Dx_Ellipse) = 0;
		

		//
		// 绘制多边形
		//
		virtual void	DrawGeometry(const std::vector<DxPointD>& points, const DxColor& col, double size) = 0;
		virtual void	FillGeometry(const std::vector<DxPointD>& points, const DxColor& col) = 0;

		//
		// 使用效果进行填充或者绘制几何图形
		// points 使用的是相对 rc 位置
		//
		virtual void	DrawGeometry(const RECT& rc, const std::vector<DxPointD>& points, CDxEffects* pEffects, double size) = 0;
		virtual void	FillGeometry(const RECT& rc, const std::vector<DxPointD>& points, CDxEffects* pEffects) = 0;

		//
		// 针对普通图形的绘制和填充，相对 rc 的位置
		//
		virtual void	DrawGeometry(const RECT& rc, const BaseShape& shape, CDxEffects* pEffects, double size) = 0;
		virtual void	FillGeometry(const RECT& rc, const BaseShape& shape, CDxEffects* pEffects) = 0;

		//
		// 图形计算
		// 绘制两个多边形的所有部分
		//
		virtual void	DrawUnionGeometry(const std::vector<DxPointD>& points1, const std::vector<DxPointD>& points2, const DxColor& col, double size) = 0;
		virtual void	FillUnionGeometry(const std::vector<DxPointD>& points1, const std::vector<DxPointD>& points2, const DxColor& col) = 0;
		

		//
		// 绘制两个多边形的相交部分
		//
		virtual void	DrawIntersectGeometry(const std::vector<DxPointD>& points1, const std::vector<DxPointD>& points2, const DxColor& col, double size) = 0;
		virtual void	FillIntersectGeometry(const std::vector<DxPointD>& points1, const std::vector<DxPointD>& points2, const DxColor& col) = 0;

		//
		// 绘制两个多边形的所有部分，但公共部分除外
		//
		virtual void	DrawXorGeometry(const std::vector<DxPointD>& points1, const std::vector<DxPointD>& points2, const DxColor& col, double size) = 0;
		virtual void	FillXorGeometry(const std::vector<DxPointD>& points1, const std::vector<DxPointD>& points2, const DxColor& col) = 0;


		//
		// 绘制只属于图形1不属于图形2的部分
		//
		virtual void	DrawExcludeGeometry(const std::vector<DxPointD>& points1, const std::vector<DxPointD>& points2, const DxColor& col, double size) = 0;
		virtual void	FillExcludeGeometry(const std::vector<DxPointD>& points1, const std::vector<DxPointD>& points2, const DxColor& col) = 0;


		//
		// 针对普通图形进行重载
		//
		virtual void	DrawUnionGeometry(const BaseShape& shape1, const BaseShape& shape2, const DxColor& col, double size) = 0;
		virtual void	FillUnionGeometry(const BaseShape& shape1, const BaseShape& shape2, const DxColor& col) = 0;

		virtual void	DrawIntersectGeometry(const BaseShape& shape1, const BaseShape& shape2, const DxColor& col, double size) = 0;
		virtual void	FillIntersectGeometry(const BaseShape& shape1, const BaseShape& shape2, const DxColor& col) = 0;

		virtual void	DrawXorGeometry(const BaseShape& shape1, const BaseShape& shape2, const DxColor& col, double size) = 0;
		virtual void	FillXorGeometry(const BaseShape& shape1, const BaseShape& shape2, const DxColor& col) = 0;

		virtual void	DrawExcludeGeometry(const BaseShape& shape1, const BaseShape& shape2, const DxColor& col, double size) = 0;
		virtual void	FillExcludeGeometry(const BaseShape& shape1, const BaseShape& shape2, const DxColor& col) = 0;


		//
		// 变换
		// 
		virtual void	SetTransform(const TransformMatrix& mat) = 0;

		//
		// 绘制效果图
		//
		virtual void	DrawEffects(const RECT& rc, CDxEffects* pEffects) = 0;
	};


}