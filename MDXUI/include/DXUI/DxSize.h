#pragma once
#include <iostream>
namespace DxUI{

	//+---------------------------------
	//
	// 常规绘制图元
	//
	//+--------------------------------
	enum DXShape{
		Dx_Rectangle = 0,
		Dx_RoundedRectangle,
		Dx_Ellipse
	};

	template<class T>
	class DxPoint{
	public:
		DxPoint(const T& x = T(), const T& y = T())
			:mX(x), mY(y)
		{
		}

		DxPoint(const POINT& pt) :mX(pt.x), mY(pt.y){}

		DxPoint(const DxPoint<T>& other) :mX(other.mX), mY(other.mY){}

		DxPoint<T>& operator=(const DxPoint<T>& other){
			if (this == &other)
				return *this;
			mX = other.mX;
			mY = other.mY;
			return *this;
		}

		DxPoint<T>& operator=(const POINT& other){
			mX = other.x;
			mY = other.y;
			return *this;
		}

		operator POINT(){
			POINT pt;
			pt.x = mX;
			pt.y = mY;
			return pt;
		}

		template<class U>
		DxPoint<T>& operator*(const U& value){
			mX *= value;
			mY *= value;
			return *this;
		}

		void X(T x){
			mX = x;
		}

		void Y(T y){
			mY = y;
		}

		T X() const{
			return mX;
		}

		T Y() const{
			return mY;
		}

		double Distance(){
			return std::sqrt(mX*mX + mY*mY);
		}

		friend std::ostream& operator<<(std::ostream& os, const DxPoint<T>& point){
			os << "X = " << point.mX << "\tY = " << point.mY;
			return os;
		}

		friend bool operator==(const DxPoint<T>& left, const DxPoint<T>& right){
			return left.mX == right.mX && left.mY == right.mY;
		}

		friend bool operator!=(const DxPoint<T>& left, const DxPoint<T>& right){
			return !(left == right);
		}
	protected:
		T mX, mY;
	};

	//
	// 定义一个向量
	//
	template<class T>
	class DxVector : public DxPoint<T>{
	public:
		DxVector() : DxPoint(), mZ(0)
		{}

		DxVector(T x, T y, T z) :DxPoint(x, y),
			mZ(z)
		{}

		DxVector(const DxVector& other) :
			DxPoint(other),
			mZ(other.mZ)
		{}

		DxVector& operator=(const DxVector& other){
			if (this == &other)
				return *this;
			DxPoint::operator =(other);
			mZ = other.mZ;
			return *this;
		}

		template<class U>
		DxVector& operator*(const U& value){
			DxPoint::operator *(value);
			mZ *= value;
			return *this;
		}

		void Z(T value){
			mZ = value;
		}

		T Z() const{
			return mZ;
		}

		friend std::ostream& operator<<(std::ostream& os, const DxVector<T>& point){
			os << DxPoint<T>(point) << "\tZ = " << point.mZ;
			return os;
		}

	protected:
		T mZ;
	};

	//
	// 定义一个尺寸
	//
	template<class T>
	class DxSize{
	public:
		DxSize(const T& width = T(), const T& height = T()) :
			mWidth(width), mHeight(height){}

		DxSize(const DxSize<T>& other) :mWidth(other.mWidth), mHeight(other.mHeight){}
		DxSize(const SIZE& size) :mWidth(size.cx), mHeight(size.cy){}

		DxSize<T>& operator=(const DxSize<T>& other){
			if (this == &other)
				return *this;
			mWidth = other.mWidth;
			mHeight = other.mHeight;
			return *this;
		}

		DxSize<T>& operator=(const SIZE& other){
			mWidth = other.cx;
			mHeight = other.cy;
			return *this;
		}

		operator SIZE() const{
			SIZE size;
			size.cx = mWidth;
			size.cy = mHeight;
			return size;
		}

		template<class U>
		DxSize<T>& operator*(const U& value){
			mWidth *= value;
			mHeight *= value;
			return *this;
		}

		void Width(const T& width){
			mWidth = width;
		}

		void Height(const T& height){
			mHeight = height;
		}

		T Width() const{
			return mWidth;
		}

		T Height() const{
			return mHeight;
		}

		
		friend std::ostream& operator<<(std::ostream& os, const DxSize<T>& point){
			os << "Width = " << point.mWidth << "\tHeight = " << point.mHeight << std::endl;
			return os;
		}

		friend bool operator==(const DxSize<T>& left, const DxSize<T>& right){
			return left.mWidth == right.mWidth && left.mHeight == right.mHeight;
		}

		friend bool operator!=(const DxSize<T>& left, const DxSize<T>& right){
			return !(left == right);
		}
	protected:
		T mWidth, mHeight;
	};


	//
	// 表示一个矩形框
	//
	template<class T>
	class DxRect : public DxSize<T>, public DxPoint<T>
	{
	public:
		DxRect() :DxPoint<T>(), DxSize<T>(){}
		DxRect(const T& x, const T& y, const T& width, const T& height) :DxPoint<T>(x, y), DxSize<T>(width, height){}
		DxRect(const DxPoint<T>& lefttop, const DxPoint<T>& rightbottom) :DxPoint<T>(lefttop),
			DxSize<T>(rightbottom.X() - lefttop.X(), rightbottom.Y() - lefttop.Y()){}
		DxRect(const DxPoint<T>& lefttop, const DxSize<T>& wh) :DxPoint<T>(lefttop),
			DxSize<T>(wh){}

		DxRect(const RECT& rect) :DxPoint<T>(rect.left, rect.top), DxSize<T>(rect.right - rect.left, rect.bottom - rect.top){}
		DxRect& operator=(const RECT& rect){
			X(rect.left);
			Y(rect.top);
			Width(rect.right - rect.left);
			Height(rect.bottom - rect.top);
			return *this;
		}
		template<class U>
		DxRect<T>& operator*(const U& value){
			mWidth *= value;
			mHeight *= value;
			return *this;
		}

		operator RECT() const{
			RECT rect;
			rect.left = X();
			rect.top = Y();
			rect.right = Right();
			rect.bottom = Bottom();
			return rect;
		}


		T Left() const{
			return X();
		}

		T Top() const{
			return Y();
		}

		T Right() const{
			return X() + Width();
		}

		T Bottom() const{
			return Y() + Height();
		}

		DxSize<T> Size(){
			return DxSize<T>(Width(), Height());
		}

		void Size(const DxSize<T>& size){
			Width(size.Width());
			Height(size.Height());
		}

		DxPoint<T> LeftTop() const{
			return DxPoint<T>(X(), Y());
		}

		void LeftTop(const DxPoint<T>& lefttop) const{
			X(lefttop.X());
			Y(lefttop.Y());
		}

		DxPoint<T> LeftBottom() const{
			return DxPoint<T>(X(), Y() + Height());
		}

		DxPoint<T> RightTop() const{
			return DxPoint<T>(X() + Width(), Y());
		}

		DxPoint<T> RightBottom() const{
			return DxPoint<T>(X() + Width(), Y() + Height());
		}

		void RightBottom(const DxPoint<T>& rightbottom){
			Width(rightbottom.X() - X());
			Height(rightbottom.Y() - Y());
		}

		//
		// 获取中心点
		//
		DxPoint<T> Center() const{
			return DxPoint<T>(X() + Width() / 2, Y() + Height() / 2);
		}

		//
		// 使用一个点对矩阵位置进行平移
		//
		void Transform(const DxPoint<T>& point){
			X(X() + point.X());
			Y(Y() + point.Y());
		}

		friend std::ostream& operator<<(std::ostream& os, const DxRect<T>& point){
			os << "X = " << point.mX << "\tY = " << point.mY << std::endl;
			os << "Width = " << point.mWidth << "\tHeight = " << point.mHeight << std::endl;
			return os;
		}

		friend bool operator==(const DxRect<T>& left, const DxRect<T>& right){
			return DxSize<T>(left) == DxSize<T>(right) && DxPoint<T>(left) == DxPoint<T>(right);
		}

		friend bool operator!=(const DxRect<T>& left, const DxRect<T>& right){
			return !(left == right);
		}

	};


	//
	// 用于表示一维的顶点
	// 通常配合父窗口使用
	// 比例因子*父窗口的尺寸 + 偏移大小 = 位置
	//
	template<class T>
	class DxDim{
	public:
		DxDim(const float& ratio = 0.f, const T& offset = T()) :mRatio(ratio), mOffset(offset)
		{}

		DxDim(const DxDim<T>& other) :mRatio(other.mRatio), mOffset(other.mOffset)
		{}

		DxDim<T>& operator=(const DxDim<T>& other){
			if (this == &other)
				return *this;
			mRatio = other.mRatio;
			mOffset = other.mOffset;
			return *this;
		}

		void Ratio(const float& ratio){
			mRatio = ratio;
		}

		float Ratio() const{
			return mRatio;
		}

		void Offset(const T& offset){
			mOffset = offset;
		}

		T Offset() const{
			return mOffset;
		}

	private:
		float   mRatio;  // 比例因子
		T		mOffset; // 偏移大小
	};


	//
	// 带比例尺的信息转换到实际大小
	//
	template<class T>
	static inline DxPoint<T> DxDimToPoint(const DxRect<T>& rect, const DxDim<T>& dimX, const DxDim<T>& dimY){
		float scalex = dimX.Ratio();
		if (scalex <= 0.f){
			scalex = 0;
		}
		if (scalex >= 1.f){
			scalex = 1.f;
		}

		float scaley = dimY.Ratio();
		if (scaley <= 0.f){
			scaley = 0;
		}
		if (scaley >= 1.f){
			scaley = 1.f;
		}

		T x = rect.X() + rect.Width()*scalex + dimX.Offset();
		T y = rect.Y() + rect.Height()*scaley + dimY.Offset();
		return DxPoint<T>(x, y);
	}

	template<class T>
	static inline DxSize<T> DxDimToSize(const DxRect<T>& rect, const DxDim<T>& dimW, const DxDim<T>& dimH){
		float scalex = dimW.Ratio();
		if (scalex <= 0.f){
			scalex = 0;
		}
		if (scalex >= 1.f){
			scalex = 1.f;
		}

		float scaley = dimH.Ratio();
		if (scaley <= 0.f){
			scaley = 0;
		}
		if (scaley >= 1.f){
			scaley = 1.f;
		}

		T x = rect.Width()*scalex + dimW.Offset();
		T y = rect.Height()*scaley + dimH.Offset();
		return DxSize<T>(x, y);
	}

	template<class T>
	static inline DxRect<T> DxDimToRect(const DxRect<T>& rect, const DxDim<T>& dimX, const DxDim<T>& dimY,
		const DxDim<T>& dimW, const DxDim<T>& dimH)
	{
		DxPoint<T> p = DxDimToPoint(rect, dimX, dimY);
		DxSize<T> s1  = DxDimToSize(rect, dimW, dimH);
		return DxRect<T>(p, s1);
	}

	//
	// DxPoint 的变换
	//
	template<class T>
	inline DxPoint<T> operator+(const DxPoint<T>& point, const DxSize<T>& size){
		return DxPoint<T>(point.X() + size.Width(), point.Y() + size.Height());
	}

	template<class T>
	inline DxPoint<T> operator-(const DxPoint<T>& point, const DxSize<T>& size){
		return DxPoint<T>(point.X() - size.Width(), point.Y() - size.Height());
	}

	template<class T>
	inline DxPoint<T> operator+(const DxPoint<T>& point, const DxPoint<T>& point2){
		return DxPoint<T>(point.X() + point2.X(), point.Y() + point2.Y());
	}

	template<class T>
	inline DxPoint<T> operator-(const DxPoint<T>& point, const DxPoint<T>& point2){
		return DxPoint<T>(point.X() - point2.X(), point.Y() - point2.Y());
	}

	template<class T>
	inline DxPoint<T> operator*(const DxPoint<T>& point, double val){
		return DxPoint<T>(point.X()*val, point.Y()*val);
	}

	template<class T>
	inline DxPoint<T> operator/(const DxPoint<T>& point, double val){
		return DxPoint<T>(point.X() / val, point.Y() / val);
	}

	template<class T>
	inline DxPoint<T> operator*(double val,const DxPoint<T>& point){
		return DxPoint<T>(point.X()*val, point.Y()*val);
	}

	template<class T>
	inline DxPoint<T> operator/(double val,const DxPoint<T>& point){
		return DxPoint<T>(val / point.X(), val / point.Y());
	}

	//
	// 为了方便使用typedef几个常用类型
	//
	typedef DxPoint<double>		DxPointD;
	typedef DxPoint<float>		DxPointF;
	typedef DxPoint<int>		DxPointI;

	typedef DxVector<double>	DxVectorD;
	typedef DxVector<float>		DxVectorF;
	typedef DxVector<int>		DxVectorI;

	typedef DxSize<double>		DxSizeD;
	typedef DxSize<float>		DxSizeF;
	typedef DxSize<int>			DxSizeI;

	typedef DxRect<double>		DxRectD;
	typedef DxRect<float>		DxRectF;
	typedef DxRect<int>			DxRectI;

	typedef DxDim<double>		DxDimD;
	typedef DxDim<float>		DxDimF;
	typedef DxDim<int>			DxDimI;

	typedef std::pair<DxPointD, DxPointD>	DxLineType;  // 一条线
	typedef TL::Vector<DxLineType>			DxLinesType; // 多条线组成的线
	typedef TL::Vector<DxPointD>			DxPolygonType; // 一个多边形
	typedef TL::Vector<DxPolygonType>		DxPolygonsType; // 一个多边形

	//
	// 曲线方程 椭圆方程
	//
	static inline bool  DxPointInEllipse(double cx, double cy, double xr,double yr,double x,double y){
		double val = ((x - cx)*(x - cx)) / (xr*xr) + ((y - cy)*(y - cy)) / (yr*yr);
		return val > 1.0 ? false : true;
	}

	//
	// 判断点是否在矩形中
	//
	template<class T>
	static inline bool PointInRect(const DxPoint<T>& pt, const DxRect<T>& rect,double radius = -1){
		if (radius <= 0){
			if (pt.X() >= rect.X() && pt.X() <= rect.X() + rect.Width() &&
				pt.Y() >= rect.Y() && pt.Y() <= rect.Y() + rect.Height())
			{
				return true;
			}
			else{
				return false;
			}
		}
		else{
			DxPoint<T> c = rect.Center();
			DxPoint<T> c2 = pt - c;
			if (c2.Distance() <= radius){
				return true;
			}
			else{
				return false;
			}
		}
	}

	template<class T>
	static inline bool PointInRect(const DxPoint<T>& pt, const DxRect<T>& rect, double xradius, double yradius, DXShape shape){
		switch (shape){
		case Dx_Rectangle:
			return PointInRect(pt, rect);
			break;

		case Dx_RoundedRectangle:
			if (true){
				bool isInRect = PointInRect(pt, rect);
				if (isInRect){
					DxPoint<T> c1 = rect.LeftTop();
					DxPoint<T> c2 = rect.LeftBottom();
					DxPoint<T> c3 = rect.RightTop();
					DxPoint<T> c4 = rect.RightBottom();

					c1.X(c1.X() + xradius);
					c1.Y(c1.Y() + yradius);

					c2.X(c1.X() + xradius);
					c2.Y(c1.Y() - yradius);

					c3.X(c1.X() - xradius);
					c3.Y(c1.Y() + yradius);

					c4.X(c1.X() - xradius);
					c4.Y(c1.Y() - yradius);


					DxRect<T> rc1(rect.X(), rect.Y(), xradius, yradius);
					if (PointInRect(pt, rc1)){
						return DxPointInEllipse(c1.X(), c1.Y(), xradius, yradius,pt.X(), pt.Y());
					}

					DxRect<T> rc2(rect.X(), rect.Y() - yradius, xradius, yradius);
					if (PointInRect(pt, rc2)){
						return DxPointInEllipse(c2.X(), c2.Y(), xradius, yradius,pt.X(), pt.Y());
					}

					DxRect<T> rc3(rect.X() - xradius, rect.Y(), xradius, yradius);
					if (PointInRect(pt, rc3)){
						return DxPointInEllipse(c3.X(), c3.Y(), xradius, yradius,pt.X(), pt.Y());
					}

					DxRect<T> rc4(rect.X() - xradius, rect.Y() - yradius, xradius, yradius);
					if (PointInRect(pt, rc4)){
						return DxPointInEllipse(c4.X(), c4.Y(), xradius, yradius,pt.X(), pt.Y());
					}
					return true;
				}
				
			}
			break;

		case Dx_Ellipse:
			if (true){
				DxPoint<T> c = rect.Center();
				return DxPointInEllipse(c.X(), c.Y(), xradius, yradius, pt.X(), pt.Y());
			}
			break;

		default:
			break;
		}
		return PointInRect(pt, rect);
	}


	//
	// 将屏幕坐标转换到世界坐标中
	// [-1,1]
	//
	template<class T>
	static inline DxRectF LocalSpaceToWorldSpace(const DxRect<T>& window, DxRect<T>& client){
		T w = window.Width();
		T h = window.Height();
		T x = window.X();
		T y = window.Y();

		T _w = client.Width();
		T _h = client.Height();
		T _x = client.X();
		T _y = client.Y();

		float __x = 2.0*_x / (x + w)*1.0 - 1.0f;
		float __y = -2.0*_y / (y + h)*1.0 + 1.0f;
		float __w = 2 * _w*1.0f / w*1.0f;
		float __h = 2 * _h*1.0f / h*1.0f;
		DxRectF __result(__x, __y, __w, __h);
		return __result;
	}

	//
	// 将本地坐标转换到纹理坐标
	// [0,1]
	//
	template<class T>
	static inline DxRectF LocalSpaceToTexCood(const DxRect<T>& window, DxRect<T>& client){
		T w = window.Width();
		T h = window.Height();
		T x = window.X();
		T y = window.Y();

		T _w = client.Width();
		T _h = client.Height();
		T _x = client.X();
		T _y = client.Y();

		float __x = _x / (x + w)*1.0;
		float __y = _y / (y + h)*1.0;
		float __w = _w*1.0f / w*1.0f;
		float __h = _h*1.0f / h*1.0f;
		DxRectF __result(__x, __y, __w, __h);
		return __result;
	}

	static inline bool IsRect(const TL::Vector<DxPointD>& ptr, D2D1_RECT_F& rc){
		if (ptr.size() == 4){
			const DxPointD& p1 = ptr.unsafe_ref(3);
			const DxPointD& p2 = ptr.unsafe_ref(1);
			const DxPointD& p3 = ptr.unsafe_ref(0);
			const DxPointD& p4 = ptr.unsafe_ref(2);
			if (p1.X() == p3.X() && p2.X() == p4.X() && p1.Y() == p4.Y() && p2.Y() == p3.Y()){
				rc = { p1.X() - 0.5, p1.Y() - 0.5, p2.X() + 0.5, p2.Y() + 0.5 };
				return true;
			}
		}
		return false;
	}
}
