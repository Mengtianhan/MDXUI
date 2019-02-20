
///////////////////////////////////////////////////////////////////////////////
//
// MToolLib ---- Mengjin Tool Library
// Copyright (C) 2014-2018 by Mengjin (sh06155@hotmail.com)
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


#pragma  once
#include <amp.h>
#include <amp_math.h>
#include <complex>

namespace mj{

	//
	//+----------------------------------------------------------------------------------------------
	//
	//  在快速计算中用于替代std::complex
	//
	//+----------------------------------------------------------------------------------------------
	//
	template<class T>
	struct Complex{
		typedef Complex<T> self_type;
		typedef T value_type;
		value_type	_real;
		value_type	_imag;

		operator std::complex<value_type>(){
			return std::complex<value_type>(_real, _imag);
		}

		operator std::complex<value_type>() const{
			return std::complex<value_type>(_real, _imag);
		}


		template<class U>
		operator std::complex<U>(){
			return std::complex<U>(_real, _imag);
		}

		template<class U>
		operator std::complex<U>() const{
			return std::complex<U>(_real, _imag);
		}

		self_type& operator=(const std::complex<value_type>& other){
			_real = other.real();
			_imag = other.imag();
			return *this;
		}


		template<class U>
		self_type& operator=(const std::complex<U>& other){
			_real = other.real();
			_imag = other.imag();
			return *this;
		}


		template<class U>
		self_type& operator=(const U& other){
			_real = other;
			_imag = T();
			return *this;
		}

		value_type real() const{
			return _real;
		}

		value_type imag() const{
			return _imag;
		}

		void real(const value_type& r){
			_real = r;
		}

		void imag(const value_type& i){
			_imag = i;
		}

		self_type& operator+=(const value_type& right){
			_real += right;
			_imag += right;
			return *this;
		}

		self_type& operator-=(const value_type& right){
			_real -= right;
			_imag -= right;
			return *this;
		}

		self_type& operator*=(const value_type& right){
			_real *= right;
			_imag *= right;
			return *this;
		}

		self_type& operator/=(const value_type& right){
			_real /= right;
			_imag /= right;
			return *this;
		}

		self_type& operator+=(const self_type& right){
			value_type __r = _real * right._real - _imag*right._imag;
			value_type __i = _imag*right._real + _real*right._imag;
			_real = __r;
			_imag = __i;
			return *this;
		}

		self_type& operator/=(const self_type& right){
			value_type sum = _real*right._real + _real*right._real;
			value_type __r = (_real * right._real + _imag*right._imag) / sum;
			value_type __i = (_imag*right._real - _real*right._imag) / sum;
			_real = __r;
			_imag = __i;
			return *this;
		}



		friend std::ostream& operator<<(std::ostream& os, const Complex<value_type>& val){
			os.width(0);
			os << "(" << val._real << "," << val._imag << ")";
			return os;
		}


		friend std::stringstream& operator<<(std::stringstream& os, const Complex<value_type>& val){
			os <<val._real << "," << val._imag;
			return os;
		}


		friend std::istream& operator>>(std::istream& is, Complex<value_type>& val){
			is >> val._real;
			if (!is.eof()){
				is >> val._imag;
			}
			return is;
		}
	};

	//
	// 实现快速转换
	//
	template<class T,class U>
	struct IsConvertible<std::complex<T>, Complex<U>> :std::is_convertible<T, T>
	{
	};

	template<class T,class U>
	struct IsConvertible<Complex<T>, std::complex<U>> :std::is_convertible<T, T>
	{
	};

	template<class T, class U>
	struct IsConvertible<Complex<T>, Complex<U>> :std::is_convertible<T, T>
	{
	};

	template<class T>
	struct IsConvertible<T,Complex<T>> :std::is_convertible<T, T>
	{
	};

	


	//
	// Complex
	//
	template<>
	class GenRandom<Complex<double>>{
	public:
		GenRandom(double min = 0.0, double max = 10000.0, __int64* tm = nullptr) :
			m_mt(static_cast<unsigned long>(time(tm))),
			m_dist(std::uniform_real_distribution<double>(min, max)){
		}
		Complex<double> operator()(){
			return{ m_dist(m_mt), m_dist(m_mt) };
		}

	private:
		std::mt19937   m_mt;
		std::uniform_real_distribution<double> m_dist;
	};

	template<>
	class GenRandom<Complex<float>>{
	public:
		GenRandom(double min = 0.0, double max = 10000.0, __int64* tm = nullptr) :
			m_mt(static_cast<unsigned long>(time(tm))),
			m_dist(std::uniform_real_distribution<double>(min, max)){
		}
		Complex<float> operator()(){
			return{ m_dist(m_mt), m_dist(m_mt) };
		}

	private:
		std::mt19937   m_mt;
		std::uniform_real_distribution<double> m_dist;
	};


	template<class T>
	class GenRandom<Complex<T>>{
	public:
		GenRandom(T min = 0, T max = 10000000, __int64* tm = nullptr) :
			m_mt(static_cast<unsigned long>(time(tm))),
			m_dist(std::uniform_int_distribution<T>(min, max)){
		}
		Complex<T> operator()(){
			return{ m_dist(m_mt), m_dist(m_mt) };
		}

	private:
		std::mt19937   m_mt;
		std::uniform_int_distribution<T> m_dist;
	};

	//
	//+----------------------------------------------------------------------------------------------
	// 复数相加
	//
	template<class T>
	inline Complex<T> operator+(const Complex<T>& left, const Complex<T>& right)__GPU_ONLY{
		Complex<T> temp = { left._real + right._real, left._imag + right._imag };
		return temp;
	}

	template<class T>
	inline Complex<T> operator+(const Complex<T>& left, const T& right)__GPU_ONLY{
		Complex<T> temp = { left._real + right, left._imag };
		return temp;
	}

	template<class T>
	inline Complex<T> operator+(const T& left, const Complex<T>& right)__GPU_ONLY{
		Complex<T>  temp{ left + right._real, right._imag };
		return temp;
	}


	//
	// 普通版
	//
	template<class T>
	inline Complex<T> operator+(const Complex<T>& left, const Complex<T>& right){
		Complex<T> temp = { left._real + right._real, left._imag + right._imag };
		return temp;
	}

	template<class T>
	inline Complex<T> operator+(const Complex<T>& left, const T& right){
		Complex<T> temp = { left._real + right, left._imag };
		return temp;
	}

	template<class T>
	inline Complex<T> operator+(const T& left, const Complex<T>& right){
		Complex<T>  temp{ left + right._real, right._imag };
		return temp;
	}

	//
	// 复数相减
	//
	template<class T>
	inline Complex<T> operator-(const Complex<T>& left, const Complex<T>& right)__GPU_ONLY{
		Complex<T>  temp = { left._real - right._real, left._imag - right._imag };
		return temp;
	}

	template<class T>
	inline Complex<T> operator-(const Complex<T>& left, const T& right)__GPU_ONLY{
		Complex<T>  temp = { left._real - right, left._imag };
		return temp;
	}

	template<class T>
	inline Complex<T> operator-(const T& left, const Complex<T>& right)__GPU_ONLY{
		Complex<T>  temp = { left - right._real, right._imag };
		return temp;
	}

	//
	// 普通版
	//
	template<class T>
	inline Complex<T> operator-(const Complex<T>& left, const Complex<T>& right){
		Complex<T>  temp = { left._real - right._real, left._imag - right._imag };
		return temp;
	}

	template<class T>
	inline Complex<T> operator-(const Complex<T>& left, const T& right){
		Complex<T>  temp = { left._real - right, left._imag };
		return temp;
	}

	template<class T>
	inline Complex<T> operator-(const T& left, const Complex<T>& right){
		Complex<T>  temp = { left - right._real, right._imag };
		return temp;
	}

	//
	// 复数相乘
	//
	template<class T>
	inline Complex<T> operator*(const Complex<T>& left, const Complex<T>& right)__GPU_ONLY{
		Complex<T>  temp = { left._real * right._real - left._imag*right._imag, left._imag*right._real + left._real*right._imag };
		return temp;
	}

	template<class T>
	inline Complex<T> operator*(const Complex<T>& left, const T& right)__GPU_ONLY{
		Complex<T>  temp = { left._real * right, left._imag*right };
		return temp;
	}

	template<class T>
	inline Complex<T> operator*(const T& left, const Complex<T>& right)__GPU_ONLY{
		Complex<T>  temp = { left * right._real, left*right._imag };
		return temp;
	}

	//
	// 普通版
	//
	template<class T>
	inline Complex<T> operator*(const Complex<T>& left, const Complex<T>& right){
		Complex<T>  temp = { left._real * right._real - left._imag*right._imag, left._imag*right._real + left._real*right._imag };
		return temp;
	}

	template<class T>
	inline Complex<T> operator*(const Complex<T>& left, const T& right){
		Complex<T>  temp = { left._real * right, left._imag*right };
		return temp;
	}

	template<class T>
	inline Complex<T> operator*(const T& left, const Complex<T>& right){
		Complex<T>  temp = { left * right._real, left*right._imag };
		return temp;
	}


	//
	// 复数相除
	//
	template<class T>
	inline Complex<T> operator/(const Complex<T>& left, const Complex<T>& right)__GPU_ONLY{
		T sum = right._real*right._real + right._real*right._real;
		Complex<T>  temp = { (left._real * right._real + left._imag*right._imag) / sum, (left._imag*right._real - left._real*right._imag) / sum };
		return temp;
	}

	template<class T>
	inline Complex<T> operator/(const Complex<T>& left, const T& right)__GPU_ONLY{
		Complex<T>  temp = { left._real / right, left._imag / right };
		return temp;
	}

	template<class T>
	inline Complex<T> operator/(const T& left, const Complex<T>& right)__GPU_ONLY{
		Complex<T>  temp = { left / right._real, left / right._imag };
		return temp;
	}

	//
	// 普通版
	//
	template<class T>
	inline Complex<T> operator/(const Complex<T>& left, const Complex<T>& right){
		T sum = right._real*right._real + right._real*right._real;
		Complex<T>  temp = { (left._real * right._real + left._imag*right._imag) / sum, (left._imag*right._real - left._real*right._imag) / sum };
		return temp;
	}

	template<class T>
	inline Complex<T> operator/(const Complex<T>& left, const T& right){
		Complex<T>  temp = { left._real / right, left._imag / right };
		return temp;
	}

	template<class T>
	inline Complex<T> operator/(const T& left, const Complex<T>& right){
		Complex<T>  temp = { left / right._real, left / right._imag };
		return temp;
	}

	//
	// GPU版本
	//
	template<class T>
	inline bool operator==(const Complex<T>& left, const Complex<T>& right)__GPU_ONLY
	{
		return left._real == right._real && left._imag == right._imag;
	}

	template<class T>
	inline bool operator==(const Complex<T>& left, const T& right)__GPU_ONLY
	{
		return left._real == right && left._imag == 0;
	}

	template<class T>
	inline bool operator==(const T& left, const Complex<T>& right)__GPU_ONLY
	{
		return left == right && right._imag == 0;
	}

	//
	// 普通版
	//
	template<class T>
	inline bool operator==(const Complex<T>& left, const Complex<T>& right)
	{
		return left._real == right._real && left._imag == right._imag;
	}

	template<class T>
	inline bool operator==(const Complex<T>& left, const T& right)
	{
		return left._real == right && left._imag == 0;
	}

	template<class T>
	inline bool operator==(const T& left, const Complex<T>& right)
	{
		return left == right && right._imag == 0;
	}

	//
	// GPU版本
	//
	template<class T>
	inline bool operator!=(const Complex<T>& left, const Complex<T>& right)__GPU_ONLY
	{
		return !(left == right);
	}

	template<class T>
	inline bool operator!=(const Complex<T>& left, const T& right)__GPU_ONLY
	{
		return !(left == right);
	}

	template<class T>
	inline bool operator!=(const T& left, const Complex<T>& right)__GPU_ONLY
	{
		return !(left == right);
	}

	//
	// 普通版
	//
	template<class T>
	inline bool operator!=(const Complex<T>& left, const Complex<T>& right)
	{
		return !(left == right);
	}

	template<class T>
	inline bool operator!=(const Complex<T>& left, const T& right)
	{
		return !(left == right);
	}

	template<class T>
	inline bool operator!=(const T& left, const Complex<T>& right)
	{
		return !(left == right);
	}

	//
	// GPU 版本
	//
	template<class T>
	inline T abs(const Complex<T>& _Left, int *_Pexp)__GPU_ONLY
	{
		*_Pexp = 0;
		T _Av = _Left._real;
		T _Bv = _Left._imag;

		if (Concurrency::fast_math::isinf(_Av) || Concurrency::fast_math::isinf(_Bv)){
			return (Concurrency::fast_math::sinf(_Bv));
		}
		else if (Concurrency::fast_math::isnan(_Av)){
			return (_Av);
		}
		else if (Concurrency::fast_math::isnan(_Bv)){
			return (_Bv);
		}
		else{
			if (_Av < 0)
				_Av = -_Av;
			if (_Bv < 0)
				_Bv = -_Bv;
			if (_Av < _Bv)
			{
				T _Tmp = _Av;
				_Av = _Bv;
				_Bv = _Tmp;
			}

			if (_Av == 0)
				return (_Av);	// |0| == 0
			if (1 <= _Av)
				*_Pexp = 2, _Av = _Av * (T)0.25, _Bv = _Bv * (T)0.25;
			else
				*_Pexp = -2, _Av = _Av * 4, _Bv = _Bv * 4;

			T _Tmp = _Av - _Bv;
			if (_Tmp == _Av)
				return (_Av);
			else if (_Bv < _Tmp)
			{
				const T _Qv = _Av / _Bv;
				return (_Av + _Bv / (_Qv + Concurrency::fast_math::sqrt(_Qv * _Qv + 1)));
			}
			else
			{
				const T _Root2 = (T)1.4142135623730950488016887242096981L;
				const T _Oneplusroot2high = (T)(10125945.0 / 4194304.0);
				const T _Oneplusroot2low = (T)1.4341252375973918872420969807856967e-7L;

				const T _Qv = _Tmp / _Bv;
				const T _Rv = (_Qv + 2) * _Qv;
				const T _Sv = _Rv / (_Root2 + Concurrency::fast_math::sqrt(_Rv + 2))
					+ _Oneplusroot2low + _Qv + _Oneplusroot2high;
				return (_Av + _Bv / _Sv);
			}
		}
	}

	template<class T>
	inline	T abs(const Complex<T>& _Left)__GPU_ONLY
	{
		int _Leftexp;
		T _Rho = abs(_Left, &_Leftexp);
		if (_Leftexp == 0)
			return (_Rho);	// no scale factor
		else
			return (Concurrency::fast_math::ldexp(_Rho, _Leftexp));
	}

	//
	// 普通版
	//
	template<class T>
	inline T abs(const Complex<T>& _Left){
		return std::abs(_Left);
	}


	//
	// GPU 版
	// 
	template<class T>
	inline Complex<T> sqrt(const Complex<T>& _Left)__GPU_ONLY
	{
		int _Leftexp;
		T _Rho = abs(_Left, &_Leftexp);

		if (_Leftexp == 0){
			Complex<T> temp = { _Rho, _Rho };
			return temp;
		}
		else
		{
			T _Realmag = Concurrency::fast_math::ldexp(_Left._real < 0 ? -_Left._real : _Left._real, -_Leftexp);
			_Rho = Concurrency::fast_math::ldexp(Concurrency::fast_math::sqrt(2 * (_Realmag + _Rho)), _Leftexp / 2 - 1);

			if (0 <= _Left._real){
				Complex<T> temp = { _Rho, _Left._imag / (2 * _Rho) };
				return temp;
			}
			else if (_Left._imag < 0){
				Complex<T> temp = { -_Left._imag / (2 * _Rho), -_Rho };
				return temp;
			}
			else{
				Complex<T> temp = { _Left._imag / (2 * _Rho), _Rho };
				return temp;
			}
		}
	}

	//
	// 普通版
	//
	template<class T>
	inline Complex<T> sqrt(const Complex<T>& _Left){
		return std::sqrt(_Left);
	}

	//
	// 求相位
	// GPU版本
	//
	template<class T>
	inline T arg(const Complex<T>& _Left)__GPU_ONLY{
		return Concurrency::fast_math::atan2(_Left._imag, _Left._real);
	}

	//
	// 普通版
	//
	template<class T>
	inline T arg(const Complex<T>& _Left){
		return std::atan2(_Left._imag, _Left._real);
	}

	//
	// GPU 版
	//
	template<class T>
	inline Complex<T> pow(const Complex<T>& _Left, double n)__GPU_ONLY{
		T am = abs(_Left);
		T theta = arg(_Left);
		am = Concurrency::fast_math::pow(am, n);
		theta = n*theta;
		return{ am*Concurrency::fast_math::cos(theta), am*Concurrency::fast_math::sin(theta) };
	}

	//
	// 普通版
	//
	template<class T>
	inline Complex<T> pow(const Complex<T>& _Left, double n){
		T am = abs(_Left);
		T theta = arg(_Left);
		am = std::pow(am, n);
		theta = n*theta;
		return{ am*std::cos(theta), am*std::sin(theta) };
	}


	//
	// 共轭
	// GPU版本
	//
	template<class T>
	inline Complex<T> conj(const Complex<T>& _Left)__GPU_ONLY{
		return{ _Left._real, -_Left._imag };
	}

	//
	// 普通版
	//
	template<class T>
	inline T conj(const Complex<T>& _Left){
		return{ _Left._real, -_Left._imag };
	}

	//
	// norm
	// GPU版本
	//
	template<class T>
	inline T norm(const Complex<T>& _Left)__GPU_ONLY{
		return _Left._real*_Left._real + _Left._imag*_Left._imag;
	}

	//
	// 普通版
	//
	template<class T>
	inline T norm(const Complex<T>& _Left){
		return _Left._real*_Left._real + _Left._imag*_Left._imag;
	}
}

//
// 特化标准库
//
namespace std{
	template<class T>
	inline T abs(const mj::Complex<T>& _Left){
		return std::abs(std::complex<T>(_Left));
	}

	template<class T>
	inline mj::Complex<T> sqrt(const mj::Complex<T>& _Left){
		mj::Complex<T> res;
		res = std::sqrt(std::complex<T>(_Left));
		return res;
	}


	template<class T>
	inline T arg(const mj::Complex<T>& _Left){
		return mj::arg(_Left);
	}

	template<class T>
	inline mj::Complex<T> pow(const mj::Complex<T>& _Left, double n){
		return mj::pow(_Left, n);
	}
}

