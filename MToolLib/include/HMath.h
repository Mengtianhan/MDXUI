
// HMath.h 解决一些数据问题的小函数
#pragma once
#include <random>
#include <time.h>
#include <algorithm>
#include <complex>
#include <math.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <limits>
//----------------------------------------
// 如果使用boost
// 那么就使用boost的随机数生成器
//-----------------------------------------
#ifdef __M_BOOST__
#include <boost/random.hpp>
#endif


namespace mj{

	//
	// 定义PI 和 E
	//
	static const double PI = 3.14159265358979328346264338327950288419717;

	static const double E = 2.7182818284590452354;

	//
	// 定义一个无穷大的浮点数和一个double
	//
	static const float InfinityF = FLT_MAX;
	static const double InfinityDF = DBL_MAX;
	static const __int64 InfinityI = _LLONG_MAX;



	//
	// 设置一个数的精度
	//
	template<typename T>
	inline T precision(T t, int num)
	{
		std::stringstream os;
		os.precision(num);
		os << t;
		T result_;
		os >> result_;
		return result_;
	}

	template<class T>
	inline std::string NumtoString(T d, int num = 3, char c = 'f'){
		std::ostringstream os;
		if (c == 'f')
			os.setf(std::ios_base::fixed, std::ios_base::floatfield);
		else
			os.setf(std::ios_base::scientific, std::ios_base::floatfield);
		os.precision(num);
		os << d;
		return os.str();
	}

	//
	// 定义几个常用的几何类型
	//
	enum Shape_Type{
		Rect = 0,  //矩形
		Ellipse,   //椭圆
		RoundRect, //圆角矩形
		Round,      //圆
		Triangle,  //三角形
		Line,      //线
		Point,     //点
		None       //不做图
	};

	//
	// 生成随机数
	// 均匀分布
	//
	template<class T >
	class GenRandom{
	public:
		GenRandom(T min = 0, T max = 10000000, __int64* tm = nullptr) :
			m_mt(static_cast<unsigned long>(time(tm))),
			m_dist(std::uniform_int_distribution<T>(min, max)){
		}
		T operator()(){
			return m_dist(m_mt);
		}

	private:
		std::mt19937   m_mt;
		std::uniform_int_distribution<T> m_dist;
	};

	//
	// 下面是针对float和double的特化版本
	//
	template<>
	class GenRandom<double>{
	public:
		GenRandom(double min = 0.0, double max = 10000.0, __int64* tm = nullptr) :
			m_mt(static_cast<unsigned long>(time(tm))),
			m_dist(std::uniform_real_distribution<double>(min, max)){
		}
		double operator()(){
			return m_dist(m_mt);
		}

	private:
		std::mt19937   m_mt;
		std::uniform_real_distribution<double> m_dist;
	};

	template<>
	class GenRandom<float>{
	public:
		GenRandom(float min = 0.0, float max = 10000.0, __int64* tm = nullptr) :
			m_mt(static_cast<unsigned long>(time(tm))),
			m_dist(std::uniform_real_distribution<float>(min, max)){
		}
		float operator()(){
			return m_dist(m_mt);
		}

	private:
		std::mt19937   m_mt;
		std::uniform_real_distribution<float> m_dist;
	};

	//
	// 正态分布分布
	// 随机数生成器
	//
	template<class T = double >
	class NormalRandom{
	public:
		NormalRandom(T mean = 0.0, T sigma = 1.0, __int64* tm = nullptr) :
			m_mt(static_cast<unsigned long>(time(tm))),
			m_dist(std::normal_distribution<T>(mean, sigma)){
		}
		T operator()(){
			return m_dist(m_mt);
		}

	private:
		std::mt19937   m_mt;
		std::normal_distribution<T> m_dist;
	};

	//
	// 三角分布
	// 随机数生成器
	// 由于三角发生器由boost提供，所以只给使用boost的工程使用
	//
#ifdef __M_BOOST__
	template<class T = double >
	class TriangleRandom{
	public:
		TriangleRandom(T a = 0.0, T b = 0.5, T c = 1.0, __int64* tm = nullptr) :
			m_mt(static_cast<unsigned long>(time(tm))),
			m_dist(boost::triangle_distribution<T>(a, b, c)){
		}
		T operator()(){
			return m_dist(m_mt);
		}

	private:
		std::mt19937   m_mt;
		boost::triangle_distribution<T> m_dist;
	};

#endif


	//
	// 将一个数转换为二进制数
	//
	class NumToBinaryHelp{
	public:
		std::string operator()(unsigned num, const unsigned bet = 8){
			mValue.clear();
			calnum(num);
			if (mValue.size() >= bet)
				return mValue;
			unsigned __num = (bet - mValue.size());
			for (int i = 0; i < __num; ++i){
				mValue.insert(mValue.begin(), '0');
			}
			return mValue;
		}
	private:
		inline void calnum(unsigned num){
			unsigned __num = num / 2;
			if (__num>0)
				calnum(__num);
			mValue += std::to_string(num % 2);
		}

		std::string mValue;
	};

	//
	// 可以使用下面的函数直接转换
	//
	static inline std::string NumToBinary(unsigned num, const unsigned bit = 8){
		return NumToBinaryHelp()(num, bit);
	}

	//
	// 字符和数字十六进制之间的转换
	//
	static inline int HexBytetoNum(unsigned char c){
		static std::string g_str1("0123456789abcdef");
		static std::string g_str2("0123456789ABCDEF");
		int pos = g_str1.find(c);
		if (pos == std::string::npos){
			pos = g_str2.find(c);
			if (pos == std::string::npos)
				return -1;
		}
		return pos;
	}

	static inline std::string ByteToHexString(unsigned char c){
		std::string result;
		static std::string g_str1("0123456789abcdef");
		static std::string g_str2("0123456789ABCDEF");
		int num = c;
		int one = c / 16;
		int two = c % 16;
		result += g_str1.at(one);
		result += g_str1.at(two);
		return result;
	}

	//
	// 将数字用16进制表示
	//
	static inline std::string NumToHexString(unsigned int num, \
		bool isupper = true, bool ishead = true){
		std::string hex;
		static std::string g_str1("0123456789abcdef");
		static std::string g_str2("0123456789ABCDEF");
		if (num > 0){
			while (num){
				int mod = num % 16;
				num /= 16;
				if (isupper){
					hex += g_str2.at(mod);
				}
				else{
					hex += g_str1.at(mod);
				}

			}
		}
		else{
			if (isupper){
				hex += g_str2.at(num);
			}
			else{
				hex += g_str1.at(num);
			}
		}
		if (ishead){
			hex += "x0";
		}
		std::reverse(hex.begin(), hex.end());
		return hex;
	}


	//
	// 将字符串表示的16进制表示为10进制数
	//
	static inline unsigned int HexToNum(const std::string& hex){
		static std::string g_str1("0123456789abcdef");
		static std::string g_str2("0123456789ABCDEF");
		//
		// 首先判断是否是合法的16进制系列
		//
		bool b1 = false;
		bool b2 = false;
		for (auto& c : hex){
			int pos = g_str1.find(c);
			if (pos == std::string::npos){
				b1 = true;
				break;
			}
		}
		if (b1){
			for (auto& c : hex){
				int pos = g_str2.find(c);
				if (pos == std::string::npos){
					b2 = true;
					break;
				}
			}
		}
		if (b1 && b2){
			throw std::runtime_error("Inalid Parameter....");
		}
		size_t count = hex.size();
		std::string hex_str;
		if (count > 8){
			throw std::runtime_error("Inalid Parameter....");
		}
		else if (count < 8){
			for (int i = 0; i < (8 - count); ++i){
				hex_str += "0";
			}
		}

		hex_str += hex;
		unsigned char str[4];
		count = hex_str.size();
		for (int i = 0; i < count; i += 2){
			unsigned char _c1 = hex_str.at(i);
			unsigned char _c2 = hex_str.at(i + 1);
			int num1 = HexBytetoNum(_c1) + 1;
			int num2 = HexBytetoNum(_c2) + 1;
			if (num1 == 0 && num2 != 0){
				num1 = 1;
			}
			else{

			}
			if (num2 == 0 && num1 != 0){
				num2 = 1;
			}
			int num3 = num1*num2 - 1;
			if (num3 != 0){
				num3 = (num1 - 1) * 16 + num2 - 1;
			}
			unsigned char _c3 = num3;
			str[3 - i / 2] = _c3;
		}
		unsigned int last_num = *(unsigned int*)(str);
		return last_num;
	}

	//
	// 通过一个复数生成对应的幅值和相位
	//
	static inline float GenAmple(float real, float imag, float base = 20.f){
		float image = std::sqrt(real*real + imag*imag);
		if (image != 0.f){
			image = base*std::log10(image);
		}
		return image;
	}

	static inline float GenPhase(float real, float imag){
		return std::atan2(imag, real) / PI * 180.0f;
	}

	//
	// 通过幅值相位还原成复数
	//
	template<class T>
	static inline std::complex<T> RestoreComplex(T ample, T phase, bool isLear = false, double base = 20.0){
		//
		// 将dB转换成线性
		// 将Deg计算出正弦值
		//

		T linear = ample;
		if (std::abs(base - 0) > 0.005){
			linear = std::pow(10.0, ample / base); //将dB转换成线性
		}
		if (isLear){
			linear = ample;
		}
		T rad = std::abs(std::tan(phase / 180.0*PI));

		T k = 1.0;
		T j = 1.0;
		T f = phase / 180.0;
		if (f <= 0){
			k = -1.0;
		}
		if (f >= -0.5 && f <= 0.5){
			j = 1.0;
		}
		else{
			j = -1.0;
		}

		//============================================
		// linear*linear = x*x+y*y
		// rad = y/x    =========================>
		// y = rad*x    =========================>
		// linear*linear = x*x + rad*x*rad*x ====>
		// linear*linear = x*x(1+rad*rad)  ======>
		// x*x = (linear*linear) / (1+rad*rad) ==>
		// x   = linear/sqrt(1+rad*rad) =========>ok
		//============================================
		T x = 1.0, y = 1.0;
		T temp = linear / std::sqrt(1.0 + rad*rad);
		x = temp*j;
		y = rad*temp*k;
		return std::complex<T>(x, y);
	}

	//
	// 两数对比获取最小的一个
	//
	template<typename T>
	static inline T Min(const T& a, const T& b){
		return a < b ? a : b;
	}

	//
	// 两数对比获取最大的一个
	//
	template<typename T>
	static inline T Max(const T& a, const T& b){
		return a > b ? a : b;
	}

	//
	// 通过两个数获取差值结果
	//
	template<typename T>
	static inline T Lerp(T __start1, T __start2, T __end1, T __end2, T dis){
		T __first = dis - __start1;
		T __all = __end1 - __start1;
		float diff = __first*1.0 / __all*1.0;
		T __all_2 = __end2 - __start2;
		return __start2 + __all_2*diff;
	}

	//
	// 计算正态分布密度
	// stdev 标准差
	// ave 为平均值
	//
	static inline float NormDensity(const float value, const float stdev, const float ave){
		if (std::abs(stdev - 0) < 0.0005)
			return 1;
		return 1 / (stdev*std::sqrt(2 * PI))*std::pow(E, (-(value - ave) / (2 * std::pow(stdev, 2))));
	}


	//
	// 简单的积分函数
	//
	template<class T>
	static inline double Integral(T fun, const double __start, const double __end, const double dx = 0.00001){
		double sum = 0;
		for (double i = __start; i <= __end - 0.5*dx; i += dx){
			sum += fun(i + 0.5*dx)*dx;
		}
		return sum;
	}

	//
	//  查找序列中最小的跨距
	//
	template<class T, class A, template<class T1, class A1> class C>
	inline T minspan(const C<T, A>& c, unsigned step = 1){
		if (c.empty())
			return T();

		T __minspan = INT_MAX;
		auto beg = c.begin();
		auto end = c.end();
		for (; beg != end;){
			auto temp = beg;
			int dis = std::distance(beg, end);
			if (dis<step)
				break;
			std::advance(beg, step);
			if (beg != end){
				T tempv = std::abs(*temp - *beg);
				if (__minspan > tempv){
					__minspan = tempv;
				}
			}
		}
		return __minspan;
	}


	//
	// 查找序列中最大跨距
	//
	template<class T, class A, template<class T1, class A1> class C>
	inline T maxspan(const C<T, A>& c, unsigned step = 1){
		if (c.empty())
			return T();

		T __maxspan = INT_MIN;
		auto beg = c.begin();
		auto end = c.end();
		for (; beg != end;){
			auto temp = beg;
			int dis = std::distance(beg, end);
			if (dis < step)
				break;
			std::advance(beg, step);
			if (beg != end){
				T tempv = std::abs(*temp - *beg);
				if (__maxspan < tempv){
					__maxspan = tempv;
				}
			}
		}
		return __maxspan;
	}

	//
	// 获取一个由跨距组成的系列
	//
	template<class U, class A, class T, class A1, template<class T1, class A2> class C>
	inline C<U, A>& spanserial(const C<T, A1>& c, C<U, A>& out, unsigned step = 1){
		if (c.empty())
			return out;
		auto beg = c.begin();
		auto end = c.end();
		for (; beg != end;){
			auto temp = beg;
			int dis = std::distance(beg, end);
			if (dis < step)
				break;
			std::advance(beg, step);
			if (beg != end){
				T tempv = std::abs(*temp - *beg);
				out.push_back(tempv);
			}
		}
		return out;
	}

	//
	// 计算最大公约数,仅支持double和int两种类型
	//
	template<class T, class A, template<class T1, class A1> class C>
	T gcd(const C<T, A>& c){
		return T();
	}

	//
	// 通常情况下将1e-10作为0处理
	//
	template<class A, template<class T1, class A1> class C>
	double gcd(const C<double, A>& c){
		if (c.empty())
			return 1;

		auto fun = [](double x, double y)->double{
			if (x == 1e-10)
				return y;
			if (y == 1e-10)
				return x;
			if (x > y)
				std::swap(x, y);
			double c;
			for (c = std::fmod(x, y); c > 1e-10; c = std::fmod(x, y)){
				x = y;
				y = c;
			}
			return y;
		};
		auto beg = c.begin();
		auto end = c.end();

		double __cd = *(beg);

		for (; beg != end;){
			double val1 = __cd;
			double val2 = *beg;
			__cd = fun(val1, val2);
			++beg;
		}
		return __cd;
	}


	template<class A, template<class T1, class A1> class C>
	double gcd(const C<__int64, A>& c){
		if (c.empty())
			return 1;

		auto fun = [](int x, int y)->int{
			if (std::abs(x)<0)
				return y;
			if (std::abs(y)<0)
				return x;
			if (x>y)
				std::swap(x, y);
			double c;
			for (c = x%y; c>0; c = x%y){
				x = y;
				y = c;
			}
			return y;
		};
		auto beg = c.begin();
		auto end = c.end();

		int __cd = *(beg);

		for (; beg != end;){
			int val1 = __cd;
			int val2 = *beg;
			__cd = fun(val1, val2);
			++beg;
		}
		return __cd;
	}

	//
	// 获取序列中满足条件的值
	//
	template<class CI, class CO, class F>
	inline void Peak2Peak(const CI& c, CO& out, F __prd){
		out.clear();
		if (c.empty())
			return;
		if (c.size() < 2){
			out.push_back(c[0]);
			return;
		}

		int num = c.size();

		for (int i = 1; i < num - 1; i++){
			if (i == 1){
				if (__prd(c[i - 1], c[i])){
					out.push_back(c[0]);
				}
			}
			if (i == num - 2){
				if (!__prd(c[i], c[i + 1])){
					out.push_back(c[i + 1]);
					return;
				}
			}
			if (!__prd(c[i - 1], c[i]) && __prd(c[i], c[i + 1])){
				out.push_back(c[i]);
			}
		}
	}
}

