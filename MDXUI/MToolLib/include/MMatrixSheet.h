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

#pragma once
#include <vector>
#include <MParamWrap.h>
#include <MCopyValue.h>
#include <algorithm>
#include <numeric>
#include <functional>
#include <map>
#include <fstream>
#include <complex>
#include <HMath.h>

#ifdef __ARMADILLO__
#include <armadillo>
#endif

#ifndef min
#define min(a,b) (((a) < (b))?(a):(b))
#endif

#ifndef max
#define max(a,b) (((a) > (b))?(a):(b))
#endif

#include <MTLComplex.h>



//+-------------------------------------------------------------------------------------
//
//
// 可以带附加属性的Sheet
// 内存模型为例主序
// 使用列操作的时候速度会比较快
// 所有的fast版本除了矩阵的叉乘外，在debug模式下速度可能会比常规的快
// 但在release模式下常规的速度在fast版本之下
// 为什么快速的版本速度反而没有常规的快呢？
// 因为AMP的array_view对于矩阵的数据是按照行主序处理的
// MMatrixSheet的数据模型是列主序，所以在使用AMP的时候需要将数据转置
// 在AMP计算完成之后有需要将数据转置回来
// 所以和常规的比起多了几次的转置操作速度自然也就下来了
// 既然如此，那么为什么还要提供速度并不快的fast版本呢？
// 算是作为一种解决方案吧
// fast版本的函数只能处理普通的数据类型 如果是复数就不能处理
//
//+--------------------------------------------------------------------------------------
namespace mj{
	
	//
	// 定义一个常量
	//
	const static std::complex<double> MI = std::sqrt(std::complex<double>(-1));

	

	//+-------------------------------------------------------
	//
	// class MMatrixSheet
	// 一个列主序的矩阵数据
	// 并非为了专用的数学计算而生
	// 只是为了更好的辅助管理数据
	// 然后顺便附带了一些简单的数学处理
	//
	//+---------------------------------------------------------
	template<class T, class Property = MAnyHolder, class A = std::allocator<T>, template<class, class>class C = std::vector>
	class MMatrixSheet{
		//+---------------------------------------------
		// 辅助判断
		//
		template<class>
		struct IsSampleType{
			static MMatrixSheet Apply(const MMatrixSheet& left, const MMatrixSheet& right){
				return left.fast_multplie(right);
			}
		};

		template<>
		struct IsSampleType<std::complex<float>> {
			static MMatrixSheet Apply(const MMatrixSheet& left, const MMatrixSheet& right){
				MMatrixSheet<Complex<typename T::value_type>> outMat = left;
				MMatrixSheet<Complex<typename T::value_type>> _right = right;
				return outMat.fast_multplie(_right);
			}
		};

		template<class ScaleType>
		struct IsSampleType<std::complex<ScaleType>> : public IsSampleType<std::complex<float>>{
		};


		template<>
		struct IsSampleType<Complex<float>> {
			static MMatrixSheet Apply(const MMatrixSheet& left, const MMatrixSheet& right){
				return left.fast_multplie(right);
			}
		};

		template<class ScaleType>
		struct IsSampleType<Complex<ScaleType>> : public IsSampleType<Complex<float>>{
		};
		
		//
		//+-----------------------------------------------------------
		//
	public:

		
		typedef T value_type;
		typedef C<T, A> metadata_type;
		typedef typename C<T, A>::iterator iterator;
		typedef typename C<T, A>::const_iterator const_iterator;
		typedef typename MConstParamWrape<T>::type const_reference_type;
		typedef std::map<std::pair<unsigned, unsigned>, Property> PropertyType;

		//
		// 对内部数据的引用
		//
		class ArrayRef{
			template<class T1, class P1, class A1, template<class, class>class C2>
			friend class MMatrixSheet;
			ArrayRef(unsigned num) :mNum(num){ mData.resize(mNum); }
			ArrayRef(const ArrayRef& other) :mNum(other.mNum), mData(other.mData){}
			ArrayRef(ArrayRef&& other) :mNum(other.mNum), mData(other.mData){ other.mNum = 0; other.mData.clear(); }
		public:	
			ArrayRef& operator=(const ArrayRef& other){ 
				if (mData.size() != other.mData.size()){
					return *this;
				}
				for (int i = 0; i < mData.size(); ++i){
					*mData[i] = *(other.mData[i]);
				}
				return *this; 
			}
			ArrayRef& operator=(ArrayRef&& other){ 
				if (mData.size() != other.mData.size()){
					return *this;
				}
				for (int i = 0; i < mData.size(); ++i){
					*mData[i] = *(other.mData[i]);
				}
				return *this; 
			}
			ArrayRef(const metadata_type& other){ assign_value(other); }
			ArrayRef& operator=(const metadata_type& other){ assign_value(other); return *this; }
			T& operator()(unsigned index){ return *(mData[index]); }
			const T& operator()(unsigned index) const{ return *(mData[index]); }
			T& operator[](unsigned index){ return *(mData[index]); }
			const T& operator[](unsigned index) const{ return *(mData[index]); }
			~ArrayRef(){ mData.clear(); mNum = 0; }
			operator metadata_type(){ return tovalue(); }
			operator metadata_type() const{ return const_cast<ArrayRef*>(this)->tovalue(); }
			int  size() const{return mData.size();}
			ArrayRef& operator+=(const T& value){ 
				for (int i = 0; i < mData.size(); ++i){
					*(mData[i]) += value;
				}
				return *this; 
			}

			ArrayRef& operator-=(const T& value){
				for (int i = 0; i < mData.size(); ++i){
					*(mData[i]) -= value;
				}
				return *this;
			}

			ArrayRef& operator*=(const T& value){
				for (int i = 0; i < mData.size(); ++i){
					*(mData[i]) *= value;
				}
				return *this;
			}

			ArrayRef& operator/=(const T& value){
				for (int i = 0; i < mData.size(); ++i){
					*(mData[i]) /= value;
				}
				return *this;
			}

			ArrayRef& operator+=(const ArrayRef& arrs){
				if (arrs.mData.size() != mData.size()){
					return *this;
				}
				for (int i = 0; i < mData.size(); ++i){
					*(mData[i]) += *(arrs.mData[i]);
				}
				return *this;
			}

			ArrayRef& operator-=(const ArrayRef& arrs){
				if (arrs.mData.size() != mData.size()){
					return *this;
				}
				for (int i = 0; i < mData.size(); ++i){
					*(mData[i]) -= *(arrs.mData[i]);
				}
				return *this;
			}

			ArrayRef& operator*=(const ArrayRef& arrs){
				if (arrs.mData.size() != mData.size()){
					return *this;
				}
				for (int i = 0; i < mData.size(); ++i){
					*(mData[i]) *= *(arrs.mData[i]);
				}
				return *this;
			}

			ArrayRef& operator/=(const ArrayRef& arrs){
				if (arrs.mData.size() != mData.size()){
					return *this;
				}
				for (int i = 0; i < mData.size(); ++i){
					*(mData[i]) /= *(arrs.mData[i]);
				}
				return *this;
			}



			//
			// metadata_type res = arr1 + arr2
			//
			friend metadata_type operator+(const ArrayRef& left, const ArrayRef& right){
				metadata_type res;
				if (left.mData.size() != right.mData.size()){
					return res;
				}
				for (int i = 0; i < right.mData.size(); ++i){
					res.push_back(*(left.mData[i]) + *(right.mData[i]));
				}
				return res;
			}

			friend metadata_type operator-(const ArrayRef& left, const ArrayRef& right){
				metadata_type res;
				if (left.mData.size() != right.mData.size()){
					return res;
				}
				for (int i = 0; i < right.mData.size(); ++i){
					res.push_back(*(left.mData[i]) - *(right.mData[i]));
				}
				return res;
			}

			friend metadata_type  operator*(const ArrayRef& left, const ArrayRef& right){
				metadata_type res;
				if (left.mData.size() != right.mData.size()){
					return res;
				}
				for (int i = 0; i < right.mData.size(); ++i){
					res.push_back(*(left.mData[i]) * *(right.mData[i]));
				}
				return res;
			}

			friend metadata_type  operator/(const ArrayRef& left, const ArrayRef& right){
				metadata_type res;
				if (left.mData.size() != right.mData.size()){
					return res;
				}
				for (int i = 0; i < right.mData.size(); ++i){
					res.push_back(*(left.mData[i]) / *(right.mData[i]));
				}
				return res;
			}


			//
			// metadata_type res = val + arr
			//
			friend metadata_type operator+(const T& left, const ArrayRef& right){
				metadata_type res;
				for (int i = 0; i < right.mData.size(); ++i){
					res.push_back(left + *(right.mData[i]));
				}
				return res;
			}

			friend metadata_type operator-(const T& left, const ArrayRef& right){
				metadata_type res;
				for (int i = 0; i < right.mData.size(); ++i){
					res.push_back(left - *(right.mData[i]));
				}
				return res;
			}

			friend metadata_type operator*(const T& left, const ArrayRef& right){
				metadata_type res;
				for (int i = 0; i < right.mData.size(); ++i){
					res.push_back(left * *(right.mData[i]));
				}
				return res;
			}

			friend metadata_type operator/(const T& left, const ArrayRef& right){
				metadata_type res;
				for (int i = 0; i < right.mData.size(); ++i){
					res.push_back(left / *(right.mData[i]));
				}
				return res;
			}

			//
			// metadata_type res = arr + val
			//
			friend metadata_type operator+(const ArrayRef& right, const T& left){
				metadata_type res;
				for (int i = 0; i < right.mData.size(); ++i){
					res.push_back(left + *(right.mData[i]));
				}
				return res;
			}

			friend metadata_type operator-(const ArrayRef& right, const T& left){
				metadata_type res;
				for (int i = 0; i < right.mData.size(); ++i){
					res.push_back(left - *(right.mData[i]));
				}
				return res;
			}

			friend metadata_type operator*(const ArrayRef& right, const T& left){
				metadata_type res;
				for (int i = 0; i < right.mData.size(); ++i){
					res.push_back(left * *(right.mData[i]));
				}
				return res;
			}

			friend metadata_type operator/(const ArrayRef& right, const T& left){
				metadata_type res;
				for (int i = 0; i < right.mData.size(); ++i){
					res.push_back(left / *(right.mData[i]));
				}
				return res;
			}
		private:
			void setData(unsigned index, T* valptr){ if (index<mNum)mData[index] = valptr; }
			void assign_value(const metadata_type& other){
				if (other.empty()){
					return;
				}
				int num1 = other.size();
				int num = num1 > mNum ? mNum : num1;
				for (int i = 0; i < num; ++i){
					*(mData[i]) = other[i];
				}
			}

			metadata_type tovalue(){
				metadata_type outv;
				outv.resize(mNum);
				for (int i = 0; i < mNum; ++i){
					outv[i] = *(mData[i]);
				}
				return outv;
			}
			unsigned  mNum;
			std::vector<T*> mData;
		};

		class MatrixRef{
			template<class T1, class P1, class A1, template<class, class>class C2>
			friend class MMatrixSheet;
			MatrixRef(unsigned rownum, unsigned colnum){ mData.resize(rownum, colnum); }
			MatrixRef(const MatrixRef& other) :mData(other.mData){}
			MatrixRef(MatrixRef&& other) :mData(other.mData){ other.mData.reset(); }
		public:
			MatrixRef& operator=(const MatrixRef& other){ 
				if (other.rows() != rows() || other.cols() != cols()){
					return *this;
				}
				for (int i = 0; i < rows(); ++i){
					for (int j = 0; j < cols(); ++j){
						*mData(i, j) = other(i, j);
					}
				}
				return *this;
			}

			MatrixRef& operator=(MatrixRef&& other){ 
				if (other.rows() != rows() || other.cols() != cols()){
					return *this;
				}
				for (int i = 0; i < rows(); ++i){
					for (int j = 0; j < cols(); ++j){
						*mData(i, j) = other(i, j);
					}
				}
				return *this;
			}
			MatrixRef(const MMatrixSheet& other){ assign_value(other); }
			MatrixRef& operator=(const MMatrixSheet& other){ assign_value(other); return *this; }
			~MatrixRef(){ mData.clear(); }
			T& operator()(unsigned row, unsigned col){ return *(mData(row, col)); }
			const T& operator()(unsigned row, unsigned col) const{ return *(mData(row, col)); }
			operator MMatrixSheet(){ return tovalue(); }
			operator MMatrixSheet() const{ return tovalue(); }
			unsigned rows() const{ return mData.rows(); }
			unsigned cols() const{ return mData.cols(); }


			MatrixRef& operator+=(const T& value){
				for (int i = 0; i < mData.size(); ++i){
					*(mData[i]) += value;
				}
				return *this;
			}

			MatrixRef& operator-=(const T& value){
				for (int i = 0; i < mData.size(); ++i){
					*(mData[i]) -= value;
				}
				return *this;
			}

			MatrixRef& operator*=(const T& value){
				for (int i = 0; i < mData.size(); ++i){
					*(mData[i]) *= value;
				}
				return *this;
			}

			MatrixRef& operator/=(const T& value){
				for (int i = 0; i < mData.size(); ++i){
					*(mData[i]) /= value;
				}
				return *this;
			}

			MatrixRef& operator+=(const MatrixRef& arrs){
				if (arrs.rows() != rows() || arrs.cols() != cols()){
					return *this;
				}
				for (int i = 0; i < mData.size(); ++i){
					*(mData[i]) += *(arrs.mData[i]);
				}
				return *this;
			}

			MatrixRef& operator-=(const MatrixRef& arrs){
				if (arrs.rows() != rows() || arrs.cols() != cols()){
					return *this;
				}
				for (int i = 0; i < mData.size(); ++i){
					*(mData[i]) -= *(arrs.mData[i]);
				}
				return *this;
			}

			MatrixRef& operator*=(const MatrixRef& arrs){
				if (arrs.rows() != rows() || arrs.cols() != cols()){
					return *this;
				}
				for (int i = 0; i < mData.size(); ++i){
					*(mData[i]) *= *(arrs.mData[i]);
				}
				return *this;
			}

			MatrixRef& operator/=(const MatrixRef& arrs){
				if (arrs.rows() != rows() || arrs.cols() != cols()){
					return *this;
				}
				for (int i = 0; i < mData.size(); ++i){
					*(mData[i]) /= *(arrs.mData[i]);
				}
				return *this;
			}
		private:
			void setData(unsigned rowindex, unsigned colindex, T* valptr){ if (rowindex<mData.rows() && colindex < mData.cols())mData(rowindex, colindex) = valptr; }
			void assign_value(const MMatrixSheet& other){
				int rownum = other.rows();
				int colnum = other.cols();
				int __row = rownum > mData.rows() ? mData.rows() : rownum;
				int __col = colnum > mData.cols() ? mData.cols() : colnum;
				for (int i = 0; i < __row; ++i){
					for (int j = 0; j < __col; ++j){
						*(mData(i, j)) = other(i, j);
					}
				}
			}

			MMatrixSheet tovalue(){
				MMatrixSheet outv;
				outv.resize(mData.rows(), mData.cols());
				for (int i = 0; i < mData.rows(); ++i){
					for (int j = 0; j < mData.cols(); ++j){
						outv(i, j) = *(mData(i, j));
					}
				}
				return outv;
			}

			MMatrixSheet tovalue() const{
				return const_cast<MatrixRef*>(this)->tovalue();
			}
			MMatrixSheet<T*>  mData;
		};

		class row_iterator{
		public:
			typedef std::random_access_iterator_tag  iterator_category;
			typedef __int32 difference_type;
			typedef T value_type;
			typedef T *pointer;
			typedef T &reference;

			row_iterator(T* it, const MMatrixSheet* ptr, unsigned row, unsigned index = 0) :it(it), p_ptr(ptr), mRow(row), mIndex(index){}
			row_iterator(const row_iterator& other) :it(other.it), p_ptr(other.p_ptr), mRow(other.mRow), mIndex(other.mIndex){}
			row_iterator& operator=(const row_iterator& other){ it = other.it; p_ptr = other.p_ptr; mRow = other.mRow; mIndex = other.mIndex; return *this; }
			inline const T &operator*() const { return *it; }
			inline const T *operator->() const { return it; }
			inline  T &operator*()  { return *it; }
			inline  T *operator->()  { return it; }
			inline bool operator==(const row_iterator & other) const { if (mRow != other.mRow) return false; return it == other.it; }
			inline bool operator!=(const row_iterator & other) const { if (mRow != other.mRow) return false; return it != other.it; }
			inline bool operator<(const row_iterator& other) const { if (mRow != other.mRow) return false; return it < other.it; }
			inline bool operator<=(const row_iterator& other) const { if (mRow != other.mRow) return false; return it <= other.it; }
			inline bool operator>(const row_iterator& other) const { if (mRow != other.mRow) return false; return it > other.it; }
			inline bool operator>=(const row_iterator& other) const { if (mRow != other.mRow) return false; return it >= other.it; }
			inline row_iterator& operator+=(int n){ if (mIndex + n > p_ptr->mCols)return *this; mIndex += n; it += n*p_ptr->mRows; return *this; }
			inline row_iterator& operator-=(int n){ if (mIndex - n < 0)return *this; mIndex -= n; it -= n*p_ptr->mRows; return *this; }
			inline row_iterator &operator++() { if (mIndex >= p_ptr->mCols) return *this; mIndex++; it += p_ptr->mRows; return *this; }
			inline row_iterator operator++(int) { if (mIndex >= p_ptr->mCols) return *this;  T *n = it;  it += p_ptr->mRows; return row_iterator(p_ptr, n, mRow, mIndex++); }
			inline row_iterator &operator--() { if (mIndex < 0) return *this; it -= p_ptr->mRows; return *this; }
			inline row_iterator operator--(int) { if (mIndex < 0) return *this; T *n = it; it -= p_ptr->mRows; return row_iterator(p_ptr, n, mRow, mIndex--); }
			friend difference_type operator-(const row_iterator& left, const row_iterator& right){ if (left.mRow != right.mRow) return 0; return left.mIndex - right.mIndex; }
		private:
			const MMatrixSheet* p_ptr{ nullptr };
			T*			  it{ nullptr };
			int           mIndex{ 0 };
			int			  mRow{ 0 };
		};

	private:
		metadata_type  mData;
		int			   mRows{ 0 };
		int			   mCols{ 0 };
		PropertyType   mPropertyMap;
	public:
		MMatrixSheet();
		~MMatrixSheet();
		MMatrixSheet(unsigned rows, unsigned cols);
		MMatrixSheet(const MMatrixSheet& other);
		MMatrixSheet(MMatrixSheet&& other);
		MMatrixSheet(const metadata_type& vec, unsigned rownum, unsigned colnum, bool colMajor = true);
		MMatrixSheet(const T* memptr, unsigned rownum, unsigned colnum, bool colMajor = true);
		template<class U, class A2, template<class, class>class C2>
		MMatrixSheet(const C2<U, A2>& other, unsigned rownum, unsigned colnum, bool colMajor = true);
		template<class U>
		MMatrixSheet(const U* memptr, unsigned rownum, unsigned colnum, bool colMajor = true);

		//
		// 使用两个向量的叉乘进行构造
		//
		MMatrixSheet(const metadata_type& colvec, const metadata_type& rowvec);

		//
		// 如果 T 是复数 可以使用两个矩阵构建一个复数矩阵
		//
		template<class U = typename T::value_type, class Property2 = Property, class A2 = std::allocator<U>>
		MMatrixSheet(const MMatrixSheet<U, Property2, A2, C>& realmat, const MMatrixSheet<U, Property2, A2, C>& imagmat);

		MMatrixSheet& operator=(const MMatrixSheet& other);
		MMatrixSheet& operator=(MMatrixSheet&& other);
#ifdef __ARMADILLO__
		MMatrixSheet(const arma::Mat<T>& other);
		MMatrixSheet& operator=(const arma::Mat<T>& other);
		operator arma::Mat<T>() const;
		operator arma::Mat<T>();
#endif

		//
		// 属性集可以不同 但不影响幅值操作 或 复制构造
		//
		template<class Property2>
		MMatrixSheet(const MMatrixSheet<T, Property2, A, C>& other);

		template<class Property2>
		MMatrixSheet(MMatrixSheet<T, Property2, A, C>&& other);

		template<class Property2>
		MMatrixSheet& operator=(const MMatrixSheet<T, Property2, A, C>& other);

		template<class Property2>
		MMatrixSheet& operator=(MMatrixSheet<T, Property2, A, C>&& other);

		//
		// 其他不同类型的转换
		//
		template<class U, class Property2 = Property, class A2 = std::allocator<U> >
		MMatrixSheet(const MMatrixSheet<U, Property2, A2, C>& other);


		template<class U, class Property2 = Property, class A2 = std::allocator<U> >
		MMatrixSheet& operator=(const MMatrixSheet<U, Property2, A2, C>& other);



		//
		// 类型转换
		//
		template<class U, template<class>class M>
		M<U>& ToOtherMatrix(M<U>& other) const;

		template<class U, template<class>class M>
		MMatrixSheet& FromOhterMatrix(const M<U>& other);

		//
		// 使用指针来构建
		//
		void mem_ptr(const T* ptr, unsigned rownum, unsigned colnum, bool colMajor = true);


		template<class U>
		void mem_ptr(const U* ptr, unsigned rownum, unsigned colnum, bool colMajor = true);

		//
		// 填充
		//
		void fill(const_reference_type val);

		//
		// 填充对角线
		//
		void diag(const_reference_type val);

		//
		// 关联内部数据
		//
		void attach(const metadata_type& data, unsigned rownum, unsigned colnum);
		metadata_type dettach();

		//
		// 设置单元格属性信息
		//
		void set_property(unsigned rowindex, unsigned colindex, typename MConstParamWrape<Property>::type propert);
		bool query_property(unsigned rowindex, unsigned colindex) const;
		Property get_property(unsigned rowindex, unsigned colindex) const;
		void clear_property();
		void erase_property(unsigned rowindex, unsigned colindex);
		void erase_row_proberty(unsigned rowindex);
		void erase_col_proberty(unsigned colindex);

		//
		// 获取数据
		//
		ArrayRef row(unsigned index);
		ArrayRef col(unsigned index);
		const ArrayRef row(unsigned index) const;
		const ArrayRef col(unsigned index) const;

		MatrixRef rows(unsigned start, unsigned stop);
		MatrixRef cols(unsigned start, unsigned stop);

		const MatrixRef rows(unsigned start, unsigned stop) const;
		const MatrixRef cols(unsigned start, unsigned stop) const;

		MatrixRef leftcols(unsigned  num);
		MatrixRef rightcols(unsigned num);

		const MatrixRef leftcols(unsigned  num) const;
		const MatrixRef rightcols(unsigned num) const;

		MatrixRef toprows(unsigned num);
		MatrixRef bottomrows(unsigned num);

		const MatrixRef toprows(unsigned num) const;
		const MatrixRef bottomrows(unsigned num) const;

		MatrixRef map(unsigned startrow, unsigned startcol, unsigned rownum, unsigned colnum);
		const MatrixRef map(unsigned startrow, unsigned startcol, unsigned rownum, unsigned colnum) const;

		unsigned rows() const;
		unsigned cols() const;
		unsigned size() const;
		unsigned length() const;

		bool empty() const;
		void clear();

		//
		// 修改矩阵大小
		//
		void resize(unsigned rownum, unsigned colnum);

		//
		// 预分配内存
		//
		void reserve(unsigned rownum, unsigned colnum);

		//
		// 增加数据
		//
		void push_back(const metadata_type& vals, bool iscol = true);
		void insert_row(unsigned pos, const metadata_type& vals);
		void insert_col(unsigned pos, const metadata_type& vals);

		void insert_mat(const MMatrixSheet& other, unsigned pos,bool isCol); // 插入一个矩阵
		void push_mat(const MMatrixSheet& other, bool isCol);   // 追加一个矩阵

		void pop_back(bool iscol = true);
		void pop_front(bool iscol = true);

		void remove_row(unsigned rowindex);
		void remove_col(unsigned colindex);

		void remove_rows(unsigned start, unsigned stop);
		void remove_cols(unsigned start, unsigned stop);

		//
		// 重置数据
		//
		void reset();
		void swap(MMatrixSheet& other);

		//
		// 双索引操作
		//
		T& operator()(unsigned rowindex, unsigned colindex);
		const T& operator()(unsigned rowindex, unsigned colindex) const;

		T& operator()(unsigned index);
		const T& operator()(unsigned index) const;


		T& at(unsigned rowindex, unsigned colindex);
		const T& at(unsigned rowindex, unsigned colindex) const;

		T& at(unsigned index);
		const T& at(unsigned index) const;

		//
		// 针对不能够返回 引用的 类型提供直接操作的方式
		// 比如 bool 在vector中按照位储存所以不能按照引用的方式操作
		//
		void set(unsigned rowindex, unsigned colindex, const_reference_type val);
		const_reference_type get(unsigned rowindex, unsigned colindex) const;

		void set(unsigned index, const_reference_type val);
		const_reference_type get(unsigned index) const;

		//
		// 迭代器操作
		//
		iterator begin();
		iterator end();

		row_iterator begin_row(unsigned rowindex);
		row_iterator end_row(unsigned rowindex);

		iterator begin_col(unsigned colindex);
		iterator end_col(unsigned colindex);

		const_iterator begin() const;
		const_iterator end() const;

		const row_iterator begin_row(unsigned rowindex) const;
		const row_iterator end_row(unsigned rowindex) const;

		const_iterator begin_col(unsigned colindex) const;
		const_iterator end_col(unsigned colindex) const;

		//
		// 获取数据地址
		//
		T* mem_ptr();
		const T* mem_ptr() const;

		//
		// 引用数据对象
		//
		metadata_type& reference_obj();
		const metadata_type& reference_obj() const;

		//
		// operator 操作
		//
		MMatrixSheet& operator+=(const_reference_type value);
		MMatrixSheet& operator-=(const_reference_type value);
		MMatrixSheet& operator*=(const_reference_type value);
		MMatrixSheet& operator/=(const_reference_type value);

		//
		// 矩阵操作
		//
		MMatrixSheet& operator+=(const MMatrixSheet& other);
		MMatrixSheet& operator-=(const MMatrixSheet& other);



		//
		//  矩阵的元素乘
		//
		MMatrixSheet& operator*=(const MMatrixSheet& other);
		//
		// 元素相除
		//
		MMatrixSheet& operator/=(const MMatrixSheet& other);


		//
		// 矩阵的叉乘
		//
		MMatrixSheet& multplie(const MMatrixSheet& other);
		MMatrixSheet& multplie(const metadata_type& other);


		//
		// 使用GPU加速
		// 不修改原始数据
		// 只是操作简单的数据类型
		// 复杂的数据类型不支持，比如复数矩阵
		//
		MMatrixSheet fast_multplie(const MMatrixSheet& other) const;
		MMatrixSheet fast_multplie(const metadata_type& other) const;



		//
		// 矩阵和一个列向量的叉乘
		//
		metadata_type operator*=(const metadata_type& colvec); // 列向量




		//
		// 计算行列式
		//
		T det() const;



		//
		// 求矩阵的逆
		//
		MMatrixSheet inv() const;


		//
		// 转置,不添加求逆矩阵操作，求逆矩阵的操作太过复杂
		//
		MMatrixSheet t() const;


		//
		//
		// 快速转置
		// 使用GPU加速
		// 不支持复杂数据类型
		//
		MMatrixSheet fast_t() const;


		//
		//
		// 操作
		//
		//
		template<class F>
		MMatrixSheet transform_copy(F fun) const;

		template<class F>
		MMatrixSheet& transform(F fun);




		//
		//
		// 快速操作
		// 函数 [=](array_view<T,2> arr,index<2> idx)__GPU_ONLY
		// 看上去似乎快速 其实也要看操作是否复杂是否耗时
		// 如果简单的数据变换实际效率不如常规版本
		//
		template<class F>
		MMatrixSheet fast_transform_copy(F fun) const;

		template<class F>
		MMatrixSheet& fast_transform(F fun);


		//
		//
		// 快速操作
		// 函数 [=](array_view<T> arr,index<1> idx)__GPU_ONLY
		// 只关心元素数值
		// 不关心元素位置
		// 速度较快
		//
		template<class F>
		MMatrixSheet fast_transform_copy1(F fun) const;

		template<class F>
		MMatrixSheet& fast_transform1(F fun);



		//
		// 反转
		// iscol == true 每一列反转
		// iscol == false 每一行反转
		//
		MMatrixSheet& reverse(bool iscol = true);
		MMatrixSheet reverse(bool iscol = true) const;



		//
		// 切割数据
		// 按照一定的步进进行数据提取
		//
		MMatrixSheet slice(unsigned rowstep,unsigned colstep);
		MMatrixSheet slice(unsigned rowstep, unsigned colstep) const;




		//
		// 调整数据
		// 例如将数据调整到+-180之间
		// adjust(360,-180,180)
		// if < -180 then + 360
		// if > 180 then - 360
		//
		template<class Less = std::less<T>, class Greater = std::greater<T>, class Equal_to = std::equal_to<T>>
		void adjust(const_reference_type adjustvalue, const_reference_type __minvalue,
			const_reference_type __maxvalue, const_reference_type excepty,
			Less __lessfun = Less(),
			Greater __greaterfun = Greater(),
			Equal_to __equalfun = Equal_to());




		//
		// 替换
		// if(prd(val,oldval) then val = newval
		//
		template<class cmp = std::equal_to<T>>
		MMatrixSheet& replace(
			typename MConstParamWrape<T>::type oldval,
			typename MConstParamWrape<T>::type newval,
			cmp prd = cmp());




		//
		// 检查包含
		// if(prd(__val,val) return true
		//
		template<class cmp = std::equal_to<T>>
		bool contains(const_reference_type val, cmp prd = cmp());


		//
		// 查找指定值在矩阵中出现的第一个位置
		//
		template<class cmp = std::equal_to<T>>
		bool index(const_reference_type val, unsigned& rowindex, unsigned& colindex, cmp prd = cmp());




		//
		// 搜索满足条件的值
		// 所有满足条件的值都会被记录下来
		//
		template<class cmp = std::equal_to<T>>
		metadata_type search(const_reference_type val, cmp prd = cmp()) const;




		//
		// 统计计算
		//
		T max_value() const;  // 最大值
		T min_value() const;  // 求最小值
		T max_index(unsigned& rowindex, unsigned& colindex) const; // 最大值索引
		T min_index(unsigned& rowindex, unsigned& colindex) const; // 最小值索引
		T sum() const;  // 求和
		T aval() const;  // 平均值
		T standardiff() const; // 标准差
		T squaldiff() const; // 方差
		T standardiff_if(const_reference_type avl) const; // 给定均值求标准差
		T squaldiff_if(const_reference_type avl) const; // 给定均值求方差




		//
		// 忽略某些值再计算
		//
		template<class cmp = std::equal_to<T>>
		T max_value(const_reference_type excepty, cmp prd = cmp()) const;

		template<class cmp = std::equal_to<T>>
		T min_value(const_reference_type excepty, cmp prd = cmp()) const;

		template<class cmp = std::equal_to<T>>
		T max_index(unsigned& rowindex, unsigned& colindex, const_reference_type excepty, cmp prd = cmp()) const;

		template<class cmp = std::equal_to<T>>
		T min_index(unsigned& rowindex, unsigned& colindex, const_reference_type excepty, cmp prd = cmp()) const;

		template<class cmp = std::equal_to<T>>
		T sum(const_reference_type excepty, cmp prd = cmp()) const;

		template<class cmp = std::equal_to<T>>
		T aval(const_reference_type excepty, cmp prd = cmp()) const;

		template<class cmp = std::equal_to<T>>
		T standardiff(const_reference_type excepty, cmp prd = cmp()) const;

		template<class cmp = std::equal_to<T>>
		T squaldiff(const_reference_type excepty, cmp prd = cmp()) const; // 方差

		template<class cmp = std::equal_to<T>>
		T standardiff_if(const_reference_type avl, const_reference_type excepty, cmp prd = cmp()) const;

		template<class cmp = std::equal_to<T>>
		T squaldiff_if(const_reference_type avl, const_reference_type excepty, cmp prd = cmp()) const; // 方差




		//
		// 打印
		//
		void print(const char* msg = "", std::ostream& os = std::cout) const;
		void load(const char* filename, bool isbinary = true);
		void save(const char* filename, bool isbinary = true) const;




		//
		// 此处的 HelpType == MString
		// 为了不引入该头文件
		// 所以只有在使用的时候才对其引入
		// mj::dmat m;
		// m.loadAll<MString>("text.txt")
		// 当文件是纯数据 没有指定大小
		// 但是按照一定的格式存储时可以使用该方法尝试读取数据
		//
		template<class HelpType>
		void loadAll(const char* filename, const char* spliter = ", \t\r\n", bool bIsMatch = true);



		//
		// 使用流操作
		//
		void load(std::istream& is, bool isbinary = true);
		__int64 save(std::ostream& is, bool isbinary = true) const;

		void load(std::istream& is, char spliter);
		void save(std::ostream& os, char spliter) const;




		//
		// 三角函数计算 不在原地修改，如果需要原地修改那么使用transform
		//
		MMatrixSheet cos() const;
		MMatrixSheet sin() const;
		MMatrixSheet tan() const;
		MMatrixSheet abs() const;
		MMatrixSheet log() const;
		MMatrixSheet log2() const;
		MMatrixSheet log10() const;
		MMatrixSheet sqrt() const;
		MMatrixSheet square() const;
		MMatrixSheet rad() const;
		MMatrixSheet pow(const_reference_type n) const;
		MMatrixSheet floor() const;
		MMatrixSheet round() const; 
		MMatrixSheet replace_min(const_reference_type val) const; // 取较小的元素
		MMatrixSheet replace_max(const_reference_type val) const; // 取较大的元素
		MMatrixSheet prod() const;
		MMatrixSheet matSum() const;
		

		//
		// 如果T是复数便可以使用下面的函数
		//
		template<class U = typename T::value_type, class A2 = std::allocator<U>>
		MMatrixSheet<U, Property, A2, C> atan2() const; // 针对复数才有用
		template<class U = typename T::value_type, class A2 = std::allocator<U>>
		MMatrixSheet<U, Property, A2, C> arg() const;  // 针对复数才有用


		//
		// mj::cx_dmat m;
		// mj::dmat m1 = m.abs<double>();
		//
		template<class U = typename T::value_type, class A2 = std::allocator<U>>
		MMatrixSheet<U, Property, A2, C> abs() const;  // 针对复数才有用


		//
		// mj::dmat m;
		// mj::cx_dmat m1 = m.sqrt<std::comple<double>>();
		//
		template<class U, class A2 = std::allocator<U>>
		MMatrixSheet<U, Property, A2, C> sqrt() const;


		//
		// mj::cx_dmat m1;
		// mj::cx_mat m2 = m1.conj();
		//
		MMatrixSheet conj() const; // 共轭复数


		//
		// 针对复矩阵
		// 获取虚部部分
		//
		template<class U = typename T::value_type, class Property2 = Property, class A2 = std::allocator<U>>
		MMatrixSheet<U, Property2, A2, C> imag() const;


		//
		// 针对复矩阵
		// 获取虚部部分
		//
		template<class U, class Property2 = Property, class A2 = std::allocator<U>>
		MMatrixSheet<U, Property2, A2, C>& imag(MMatrixSheet<U, Property2, A2, C>& out) const;


		//
		// 针对复矩阵
		// 设置虚部
		//
		template<class U = typename T::value_type, class Property2 = Property, class A2 = std::allocator<U>>
		void set_imag(const MMatrixSheet<U, Property2, A2, C>& imagmat);


		//
		// 针对复矩阵
		// 获取实部部分
		//
		template<class U = typename T::value_type, class Property2 = Property, class A2 = std::allocator<U>>
		MMatrixSheet<U, Property2, A2, C> real() const;


		//
		// 针对复矩阵
		// 获取实部部分
		//
		template<class U, class Property2 = Property, class A2 = std::allocator<U>>
		MMatrixSheet<U, Property2, A2, C>& real(MMatrixSheet<U, Property2, A2, C>& out) const;


		//
		// 针对复矩阵
		// 设置实部部分
		//
		template<class U = typename T::value_type, class Property2 = Property, class A2 = std::allocator<U>>
		void set_real(const MMatrixSheet<U, Property2, A2, C>& realmat);



		//+------------------------------------------------------------------------------
		//
		//
		// 友元函数
		// mj::dmat m;
		// mj::dmat m2 = 1.0 + m + 1.2..... // 混合运算
		// mj::dmat m3 = m2 * m; // 点成
		// mj::dmat m4 = m2 / m;
		// mj::dmat m5 = m2 % m;  //叉乘
		//
		//
		//+-------------------------------------------------------------------------------
		friend MMatrixSheet operator+(const MMatrixSheet& left, const MMatrixSheet& right){
			MMatrixSheet outmat;
			if (left.cols() != right.cols() || left.rows() != right.rows()){
				return outmat;
			}
			outmat = left;
			outmat += right;
			return outmat;
		}

		friend MMatrixSheet operator-(const MMatrixSheet& left, const MMatrixSheet& right){
			MMatrixSheet outmat;
			if (left.cols() != right.cols() || left.rows() != right.rows()){
				return outmat;
			}
			outmat = left;
			outmat -= right;
			return outmat;
		}

		friend MMatrixSheet operator*(const MMatrixSheet& left, const MMatrixSheet& right){
			MMatrixSheet outmat;
			if (left.cols() != right.cols() || left.rows() != right.rows()){
				return outmat;
			}
			outmat = left;
			outmat *= right;
			return outmat;
		}

		friend MMatrixSheet operator/(const MMatrixSheet& left, const MMatrixSheet& right){
			MMatrixSheet outmat;
			if (left.cols() != right.cols() || left.rows() != right.rows()){
				return outmat;
			}
			outmat = left;
			outmat /= right;
			return outmat;
		}

		//+----------------------------------------------------------
		//
		// 实数矩阵和复数矩阵的操作
		//
		//+----------------------------------------------------------
		friend MMatrixSheet<std::complex<T>> operator+(const MMatrixSheet& left, const MMatrixSheet<std::complex<T>>& right){
			MMatrixSheet<std::complex<T>> outmat;
			if (left.cols() != right.cols() || left.rows() != right.rows()){
				return outmat;
			}
			outmat.resize(left.rows(), left.cols());
			for (int i = 0; i < left.rows(); ++i){
				for (int j = 0; j < left.cols(); ++j){
					outmat(i, j) = left(i, j) + right(i, j);
				}
			}
			return outmat;
		}


		friend MMatrixSheet<std::complex<T>> operator+(const MMatrixSheet<std::complex<T>>& left, const MMatrixSheet& right){
			MMatrixSheet<std::complex<T>> outmat;
			if (left.cols() != right.cols() || left.rows() != right.rows()){
				return outmat;
			}
			outmat.resize(left.rows(), left.cols());
			for (int i = 0; i < left.rows(); ++i){
				for (int j = 0; j < left.cols(); ++j){
					outmat(i, j) = left(i, j) + right(i, j);
				}
			}
			return outmat;
		}


		friend MMatrixSheet<std::complex<T>> operator-(const MMatrixSheet& left, const MMatrixSheet<std::complex<T>>& right){
			MMatrixSheet<std::complex<T>> outmat;
			if (left.cols() != right.cols() || left.rows() != right.rows()){
				return outmat;
			}
			outmat.resize(left.rows(), left.cols());
			for (int i = 0; i < left.rows(); ++i){
				for (int j = 0; j < left.cols(); ++j){
					outmat(i, j) = left(i, j) - right(i, j);
				}
			}
			return outmat;
		}

		friend MMatrixSheet<std::complex<T>> operator-(const MMatrixSheet<std::complex<T>>& left, const MMatrixSheet& right){
			MMatrixSheet<std::complex<T>> outmat;
			if (left.cols() != right.cols() || left.rows() != right.rows()){
				return outmat;
			}
			outmat.resize(left.rows(), left.cols());
			for (int i = 0; i < left.rows(); ++i){
				for (int j = 0; j < left.cols(); ++j){
					outmat(i, j) = left(i, j) - right(i, j);
				}
			}
			return outmat;
		}


		friend MMatrixSheet<std::complex<T>> operator*(const MMatrixSheet& left, const MMatrixSheet<std::complex<T>>& right){
			MMatrixSheet<std::complex<T>> outmat;
			if (left.cols() != right.cols() || left.rows() != right.rows()){
				return outmat;
			}
			outmat.resize(left.rows(), left.cols());
			for (int i = 0; i < left.rows(); ++i){
				for (int j = 0; j < left.cols(); ++j){
					outmat(i, j) = left(i, j) * right(i, j);
				}
			}
			return outmat;
		}

		friend MMatrixSheet<std::complex<T>> operator*(const MMatrixSheet<std::complex<T>>& left, const MMatrixSheet& right){
			MMatrixSheet<std::complex<T>> outmat;
			if (left.cols() != right.cols() || left.rows() != right.rows()){
				return outmat;
			}
			outmat.resize(left.rows(), left.cols());
			for (int i = 0; i < left.rows(); ++i){
				for (int j = 0; j < left.cols(); ++j){
					outmat(i, j) = left(i, j) * right(i, j);
				}
			}
			return outmat;
		}


		friend MMatrixSheet<std::complex<T>> operator/(const MMatrixSheet& left, const MMatrixSheet<std::complex<T>>& right){
			MMatrixSheet<std::complex<T>> outmat;
			if (left.cols() != right.cols() || left.rows() != right.rows()){
				return outmat;
			}
			outmat.resize(left.rows(), left.cols());
			for (int i = 0; i < left.rows(); ++i){
				for (int j = 0; j < left.cols(); ++j){
					outmat(i, j) = left(i, j) / right(i, j);
				}
			}
			return outmat;
		}

		friend MMatrixSheet<std::complex<T>> operator/(const MMatrixSheet<std::complex<T>>& left, const MMatrixSheet& right){
			MMatrixSheet<std::complex<T>> outmat;
			if (left.cols() != right.cols() || left.rows() != right.rows()){
				return outmat;
			}
			outmat.resize(left.rows(), left.cols());
			for (int i = 0; i < left.rows(); ++i){
				for (int j = 0; j < left.cols(); ++j){
					outmat(i, j) = left(i, j) / right(i, j);
				}
			}
			return outmat;
		}

		//+--------------------------------------------------------------------------------
		//
		// 和数值操作
		//
		//+---------------------------------------------------------------------------------
		friend MMatrixSheet operator+(const MMatrixSheet& left, const_reference_type value){
			MMatrixSheet outmat = left;
			outmat += value;
			return outmat;
		}

		friend MMatrixSheet operator-(const MMatrixSheet& left, const_reference_type value){
			MMatrixSheet outmat = left;
			outmat -= value;
			return outmat;
		}

		friend MMatrixSheet operator*(const MMatrixSheet& left, const_reference_type value){
			MMatrixSheet outmat = left;
			outmat *= value;
			return outmat;
		}

		friend MMatrixSheet operator/(const MMatrixSheet& left, const_reference_type value){
			MMatrixSheet outmat = left;
			outmat /= value;
			return outmat;
		}


		//+------------------------------------------------
		//
		// 和复数的操作
		//
		//+------------------------------------------------
		template<class U>
		friend MMatrixSheet<std::complex<U>> operator+(const MMatrixSheet& left, const std::complex<U>& value){
			MMatrixSheet<std::complex<U>> outmat;
			outmat.resize(left.rows(), left.cols());
			for (int i = 0; i < left.rows(); ++i){
				for (int j = 0; j < left.cols(); ++j){
					outmat(i, j) = left(i, j) + value;
				}
			}
			return outmat;
		}


		template<class U>
		friend MMatrixSheet<std::complex<U>> operator-(const MMatrixSheet& left, const std::complex<U>& value){
			MMatrixSheet<std::complex<U>> outmat;
			outmat.resize(left.rows(), left.cols());
			for (int i = 0; i < left.rows(); ++i){
				for (int j = 0; j < left.cols(); ++j){
					outmat(i, j) = left(i, j) - value;
				}
			}
			return outmat;
		}


		template<class U>
		friend MMatrixSheet<std::complex<U>> operator*(const MMatrixSheet& left, const std::complex<U>& value){
			MMatrixSheet<std::complex<U>> outmat;
			outmat.resize(left.rows(), left.cols());
			for (int i = 0; i < left.rows(); ++i){
				for (int j = 0; j < left.cols(); ++j){
					outmat(i, j) = left(i, j) * value;
				}
			}
			return outmat;
		}


		template<class U>
		friend MMatrixSheet<std::complex<U>> operator/(const MMatrixSheet& left, const std::complex<U>& value){
			MMatrixSheet<std::complex<U>> outmat;
			outmat.resize(left.rows(), left.cols());
			for (int i = 0; i < left.rows(); ++i){
				for (int j = 0; j < left.cols(); ++j){
					outmat(i, j) = left(i, j) / value;
				}
			}
			return outmat;
		}

		//+--------------------------------------------------------------------------------
		//
		// 数值在前
		//
		//+--------------------------------------------------------------------------------
		friend MMatrixSheet operator+(const_reference_type value,const MMatrixSheet& right){
			if (right.empty()){
				return right;
			}
			MMatrixSheet outmat = right;
			outmat += value;
			return outmat;
		}

		friend MMatrixSheet operator-(const_reference_type value, const MMatrixSheet& right){
			if (right.empty()){
				return right;
			}
			MMatrixSheet outmat = right;
			for (auto& v : outmat.reference_obj()){
				v = value - v;
			}
			return outmat;
		}

		friend MMatrixSheet operator*(const_reference_type value, const MMatrixSheet& right){
			if (right.empty()){
				return right;
			}
			MMatrixSheet outmat = right;
			for (auto& v : outmat.reference_obj()){
				v *= value;
			}
			return outmat;
		}

		friend MMatrixSheet operator/(const_reference_type value, const MMatrixSheet& right){
			if (right.empty()){
				return right;
			}
			MMatrixSheet outmat = right;
			for (auto& v : outmat.reference_obj()){
				v = value/v;
			}
			return outmat;
		}


		//+-------------------------------------
		//
		// 和复数的操作
		//
		//+-------------------------------------
		template<class U>
		friend MMatrixSheet<std::complex<U>> operator+(const std::complex<U>& value, const MMatrixSheet& left){
			return left + value;
		}


		template<class U>
		friend MMatrixSheet<std::complex<U>> operator-(const std::complex<U>& value,const MMatrixSheet& left){
			MMatrixSheet<std::complex<U>> outmat;
			outmat.resize(left.rows(), left.cols());
			for (int i = 0; i < left.rows(); ++i){
				for (int j = 0; j < left.cols(); ++j){
					outmat(i, j) = value - left(i, j);
				}
			}
			return outmat;
		}


		template<class U>
		friend MMatrixSheet<std::complex<U>> operator*(const std::complex<U>& value, const MMatrixSheet& left){
			return left * value;
		}


		template<class U>
		friend MMatrixSheet<std::complex<U>> operator/(const std::complex<U>& value, const MMatrixSheet& left){
			MMatrixSheet<std::complex<U>> outmat;
			outmat.resize(left.rows(), left.cols());
			for (int i = 0; i < left.rows(); ++i){
				for (int j = 0; j < left.cols(); ++j){
					outmat(i, j) = value / left(i, j);
				}
			}
			return outmat;
		}





		//+------------------------------------------------------
		//
		// 矩阵叉乘的操作符
		// 由于一开始使用* 去作为元素相乘
		// 后来想要重载一个叉乘操作符时发现没啥选择了
		//
		//+-------------------------------------------------------
		friend MMatrixSheet operator%(const MMatrixSheet& left, const MMatrixSheet& right){
			return IsSampleType<T>::Apply(left, right);
		}

		friend MMatrixSheet operator%(const MMatrixSheet& left, const metadata_type& colvec){
			MMatrixSheet right;
			right.attach(colvec, colvec.size(), 1);
			return IsSampleType<T>::Apply(left, right);
		}
		
		friend MMatrixSheet operator%(const metadata_type& colvec, const metadata_type& rowvec){
			MMatrixSheet left;
			left.attach(colvec, colvec.size(), 1);

			MMatrixSheet right;
			right.attach(rowvec, 1, rowvec.size());
			return IsSampleType<T>::Apply(left, right);
		}


		//
		// 复数矩阵和非复数矩阵之间的叉乘
		//
		friend MMatrixSheet<std::complex<T>> operator%(const MMatrixSheet& left, const MMatrixSheet<std::complex<T>>& right){
			MMatrixSheet<Complex<T>> _left = left;
			MMatrixSheet<Complex<T>> _right = right;
			return _left % _right;
		}

		friend MMatrixSheet<std::complex<T>> operator%(const MMatrixSheet<std::complex<T>>& left, const MMatrixSheet& right){
			MMatrixSheet<Complex<T>> _left = left;
			MMatrixSheet<Complex<T>> _right = right;
			return _left % _right;
		}

		friend MMatrixSheet<Complex<T>> operator%(const MMatrixSheet& left, const MMatrixSheet<Complex<T>>& right){
			MMatrixSheet<Complex<T>> _left = left;
			return _left % right;
		}

		friend MMatrixSheet<Complex<T>> operator%(const MMatrixSheet<Complex<T>>& left, const MMatrixSheet& right){
			MMatrixSheet<Complex<T>> _right = right;
			return left % right;
		}



		//+------------------------------------------------------
		//
		// 流输出
		//
		//+------------------------------------------------------
		friend std::ostream& operator<<(std::ostream& os, const MMatrixSheet& mat){
			mat.print("", os);
			return os;
		}

	}; //end template<class T, class Property = MAnyHolder, class A = std::allocator<T>, template<class, class>class C = std::vector> class MMatrixSheet







	/******************************************下面是实现细节没兴趣可以不用关心*************************************************************************/




	/*-----------------------------------------------------------------------------------------------
	*
	*
	*
	*
	*
	*
	*   实现细节
	*
	*
	*
	*
	*
	*
	*
	*
	*
	*
	*-----------------------------------------------------------------------------------------------
	*
	*
	*
	*
	*
	*---------------------------------------------华丽的分割线---------------------------------------*/




	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>::MMatrixSheet()
	{
		mRows = 0;
		mCols = 0;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>::~MMatrixSheet()
	{
		reset();
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>::MMatrixSheet(unsigned rows, unsigned cols){
		mRows = rows;
		mCols = cols;
		mData.resize(mRows*mCols);
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>::MMatrixSheet(const MMatrixSheet& other){
		mRows = other.mRows;
		mCols = other.mCols;
		mData = other.mData;
		mPropertyMap = other.mPropertyMap;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>::MMatrixSheet(MMatrixSheet&& other){
		mRows = other.mRows;
		mCols = other.mCols;
		mData = other.mData;
		mPropertyMap = other.mPropertyMap;
		other.reset();
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>::MMatrixSheet(
		const typename MMatrixSheet::metadata_type& vec,
		unsigned rownum, unsigned colnum, bool colMajor)
	{
		mRows = rownum;
		mCols = colnum;
		if (colMajor){
			mData = vec;
		}
		else{
			mData.resize(mRows*mCols);
			unsigned index = 0;
			for (unsigned i = 0; i < mRows; ++i){
				for (unsigned j = 0; j < mCols; ++j){
					mData(i, j) = vec[index++];
				}
			}
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>::MMatrixSheet(const T* memptr, unsigned rownum, unsigned colnum, bool colMajor)
	{
		mRows = rownum;
		mCols = colnum;
		mData.resize(mRows*mCols);
		if (colMajor){
			memcpy(&mData[0], memptr, sizeof(T)*mRows*mCols);
		}
		else{
			unsigned index = 0;
			for (unsigned i = 0; i < mRows; ++i){
				for (unsigned j = 0; j < mCols; ++j){
					(*this)(i, j) = memptr[index++];
				}
			}
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class U, class A2, template<class, class>class C2>
	MMatrixSheet<T, Property, A, C>::MMatrixSheet(const C2<U, A2>& other, unsigned rownum, unsigned colnum, bool colMajor)
	{
		mRows = rownum;
		mCols = colnum;
		mData.resize(mRows*mCols);
		if (colMajor){
			unsigned index = 0;
			for (unsigned i = 0; i < mCols; ++i){
				for (unsigned j = 0; j < mRows; ++j){
                                        mj::copyvalue((*this)(j, i), other[index++]);
				}
			}
		}
		else{
			unsigned index = 0;
			for (unsigned i = 0; i < mRows; ++i){
				for (unsigned j = 0; j < mCols; ++j){
					mj::copyvalue((*this)(i, j), other[index++]);
				}
			}
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class U>
	MMatrixSheet<T, Property, A, C>::MMatrixSheet(const U* memptr, unsigned rownum, unsigned colnum, bool colMajor)
	{
		mRows = rownum;
		mCols = colnum;
		mData.resize(mRows*mCols);
		if (colMajor){
			unsigned index = 0;
			for (unsigned i = 0; i < mCols; ++i){
				for (unsigned j = 0; j < mRows; ++j){
                    mj::copyvalue((*this)(j, i), memptr[index++]);
				}
			}
		}
		else{
			unsigned index = 0;
			for (unsigned i = 0; i < mRows; ++i){
				for (unsigned j = 0; j < mCols; ++j){
					mj::copyvalue((*this)(i, j), memptr[index++]);
				}
			}
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>::MMatrixSheet(const metadata_type& colvec, const metadata_type& rowvec)
	{
		mRows = colvec.size();
		mCols = rowvec.size();
		mData.resize(mRows*mCols);
		for (unsigned i = 0; i < mCols; ++i){
			for (unsigned j = 0; j < mRows; ++j){
				(*this)(j, i) = rowvec[i] * colvec[j];
			}
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class U, class Property2, class A2>
	MMatrixSheet<T, Property, A, C>::MMatrixSheet(const MMatrixSheet<U, Property2, A2, C>& realmat, const MMatrixSheet<U, Property2, A2, C>& imagmat)
	{
		if (realmat.cols() != imagmat.cols() || realmat.rows() != imagmat.rows()){
			throw std::runtime_error("实部虚部行列不等");
		}
		mRows = realmat.rows();
		mCols = realmat.cols();
		mData.resize(mRows*mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				(*this)(i, j) = T(realmat(i, j), imagmat(i, j));
			}
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::operator=(const MMatrixSheet& other)
	{
		mRows = other.mRows;
		mCols = other.mCols;
		mData = other.mData;
		mPropertyMap = other.mPropertyMap;
		return *this;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::operator=(MMatrixSheet&& other)
	{
		mRows = other.mRows;
		mCols = other.mCols;
		mData = other.mData;
		mPropertyMap = other.mPropertyMap;
		other.reset();
		return *this;
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class Property2>
	MMatrixSheet<T, Property, A, C>::MMatrixSheet(const MMatrixSheet<T, Property2, A, C>& other)
	{
		resize(other.rows(), other.cols());
		memcpy(&mData[0], other.mem_ptr(), sizeof(T)*mRows*mCols);
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class Property2>
	MMatrixSheet<T, Property, A, C>::MMatrixSheet(MMatrixSheet<T, Property2, A, C>&& other)
	{
		resize(other.rows(), other.cols());
		memcpy(&mData[0], other.mem_ptr(), sizeof(T)*mRows*mCols);
		other.reset();
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class Property2>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::operator=(const MMatrixSheet<T, Property2, A, C>& other)
	{
		reset();
		resize(other.rows(), other.cols());
		memcpy(&mData[0], other.mem_ptr(), sizeof(T)*mRows*mCols);
		return *this;
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class Property2>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::operator=(MMatrixSheet<T, Property2, A, C>&& other)
	{
		reset();
		resize(other.rows(), other.cols());
		memcpy(&mData[0], other.mem_ptr(), sizeof(T)*mRows*mCols);
		other.reset();
		return *this;
	}


	//
	// 不同类型之间的转换
	//
	template<class T, class Property, class A, template<class, class>class C>
	template<class U, class Property2, class A2>
	MMatrixSheet<T, Property, A, C>::MMatrixSheet(const MMatrixSheet<U, Property2, A2, C>& other){
		if (!other.empty()){
			resize(other.rows(), other.cols());
			for (int i = 0; i < other.rows(); ++i){
				for (int j = 0; j < other.cols(); ++j){
					mj::copyvalue((*this)(i, j), other(i, j));
				}
			}
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class U, class Property2, class A2>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::operator=(const MMatrixSheet<U, Property2, A2, C>& other){
		reset();
		if (other.empty())
			return *this;
		resize(other.rows(), other.cols());
		for(int i=0;i<other.rows();++i){
			for(int j=0;j<other.cols();++j){
				mj::copyvalue((*this)(i, j), other(i, j));
			}
		}
		return *this;
	}


#ifdef __ARMADILLO__
	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>::MMatrixSheet(const arma::Mat<T>& other){
		mCols = other.n_cols;
		mRows = other.n_rows;
		mData.resize(mRows*mCols);
		memcpy(&mData[0], other.memptr(), sizeof(T)*mRows*mCols);
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::operator=(const arma::Mat<T>& other){
		this->mCols = other.n_cols;
		this->mRows = other.n_rows;
		mData.resize(mRows*mCols);
		memcpy(&mData[0], other.memptr(), sizeof(T)*mRows*mCols);
		return *this;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>::operator arma::Mat<T>() const{
		const T* ptr = static_cast<const T*>(&mData[0]);
		arma::Mat<T> m(ptr, mRows, mCols);
		return m;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>::operator arma::Mat<T>(){
		T* ptr = static_cast<T*>(&mData[0]);
		arma::Mat<T> m(ptr, mRows, mCols);
		return m;
	}
#endif

	//
	// 类型转换
	//
	template<class T, class Property, class A, template<class, class>class C>
	template<class U, template<class>class M>
	M<U>& MMatrixSheet<T, Property, A, C>::ToOtherMatrix(M<U>& other) const
	{
		other.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				mj::copyvalue(other(i, j), (*this)(i, j));
			}
		}
		return other;
	}


	template<class T, class Property, class A, template<class, class>class C>
	template<class U, template<class>class M>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::FromOhterMatrix(const M<U>& other)
	{
		reset();
		resize(other.rows(), other.cols());
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				mj::copyvalue((*this)(i, j), other(i, j));
			}
		}
		return *this;
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::mem_ptr(const T* ptr, unsigned rownum, unsigned colnum, bool colMajor)
	{
		mRows = rownum;
		mCols = colnum;
		mData.resize(mRows*mCols);
		if (colMajor){
			memcpy(&mData[0], ptr, sizeof(T)*mRows*mCols);
		}
		else{
			unsigned index = 0;
			for (unsigned i = 0; i < mRows; ++i){
				for (unsigned j = 0; j < mCols; ++j){
					(*this)(i, j) = ptr[index++];
				}
			}
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class U>
	void MMatrixSheet<T, Property, A, C>::mem_ptr(const U* ptr, unsigned rownum, unsigned colnum, bool colMajor)
	{
		mRows = rownum;
		mCols = colnum;
		mData.resize(mRows*mCols);
		if (colMajor){
			unsigned index = 0;
			for (unsigned i = 0; i < mCols; ++i){
				for (unsigned j = 0; j < mRows; ++j){
					mj::copyvalue((*this)(j, i), ptr[index++]);
				}
			}
		}
		else{
			unsigned index = 0;
			for (unsigned i = 0; i < mRows; ++i){
				for (unsigned j = 0; j < mCols; ++j){
					mj::copyvalue((*this)(i, j), ptr[index++]);
				}
			}
		}
	}

	//
	// 填充
	//
	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::fill(const_reference_type val){
		std::fill(mData.begin(), mData.end(), val);
	}

	//
	// 填充对角线
	//
	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::diag(const_reference_type val){
		if (mRows != mCols)
			return;
		for (unsigned i = 0; i < mRows; ++i){
			(*this)(i, i) = val;
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::attach(const typename MMatrixSheet<T, Property, A, C>::metadata_type& data, unsigned rownum, unsigned colnum)
	{
		mData = data;
		mRows = rownum;
		mCols = colnum;
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::metadata_type MMatrixSheet<T, Property, A, C>::dettach()
	{
		return mData;
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::set_property(unsigned rowindex, unsigned colindex, typename MConstParamWrape<Property>::type propert)
	{
		mPropertyMap[std::make_pair(rowindex, colindex)] = propert;
	}

	template<class T, class Property, class A, template<class, class>class C>
	bool MMatrixSheet<T, Property, A, C>::query_property(unsigned rowindex, unsigned colindex) const
	{
		if (mPropertyMap.count(std::make_pair(rowindex, colindex))){
			return true;
		}
		else{
			return false;
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	Property MMatrixSheet<T, Property, A, C>::get_property(unsigned rowindex, unsigned colindex) const
	{
		auto p = std::make_pair(rowindex, colindex);
		if (mPropertyMap.count(p)){
			return mPropertyMap.at(p);
		}
		else{
			return Property();
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::clear_property(){
		mPropertyMap.clear();
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::erase_property(unsigned rowindex, unsigned colindex){
		auto p = std::make_pair(rowindex, colindex);
		if (mPropertyMap.count(p)){
			mPropertyMap.erase(p);
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::erase_row_proberty(unsigned rowindex){
		auto beg = mPropertyMap.begin();
		for (; beg != mPropertyMap.end();){
			auto  p = beg->first;
			unsigned col = p.first;
			if (col == rowindex){
				mPropertyMap.erase(beg++);
			}
			else{
				++beg;
			}
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::erase_col_proberty(unsigned colindex){
		auto beg = mPropertyMap.begin();
		for (; beg != mPropertyMap.end();){
			auto  p = beg->first;
			unsigned col = p.second;
			if (col == colindex){
				mPropertyMap.erase(beg++);
			}
			else{
				++beg;
			}
		}
	}

	//
	// 获取数据
	//
	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::ArrayRef MMatrixSheet<T, Property, A, C>::row(unsigned index)
	{
		ArrayRef outref(mCols);
		for (unsigned i = 0; i < mCols; ++i){
			outref.setData(i, &(*this)(index, i));
		}
		return outref;
	}

	template<class T, class Property, class A, template<class, class> class C>
	typename MMatrixSheet<T, Property, A, C>::ArrayRef MMatrixSheet<T, Property, A, C>::col(unsigned index)
	{
		ArrayRef outref(mRows);
		for (unsigned i = 0; i < mRows; ++i){
			outref.setData(i, &((*this)(i, index)));
		}
		return outref;
	}

	template<class T, class Property, class A, template<class, class>class C>
	const typename MMatrixSheet<T, Property, A, C>::ArrayRef MMatrixSheet<T, Property, A, C>::row(unsigned index) const
	{
		return const_cast<MMatrixSheet*>(this)->row(index);
	}

	template<class T, class Property, class A, template<class, class>class C>
	const typename MMatrixSheet<T, Property, A, C>::ArrayRef MMatrixSheet<T, Property, A, C>::col(unsigned index) const
	{
		return const_cast<MMatrixSheet*>(this)->col(index);
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::MatrixRef MMatrixSheet<T, Property, A, C>::rows(unsigned start, unsigned stop)
	{
		MatrixRef matref(stop - start, mCols);
		for (unsigned i = start; i < stop; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				matref.setData(i - start, j, &((*this)(i, j)));
			}
		}
		return matref;
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::MatrixRef MMatrixSheet<T, Property, A, C>::cols(unsigned start, unsigned stop)
	{
		MatrixRef matref(mRows, stop - start);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = start; j < stop; ++j){
				matref.setData(i, j - start, &((*this)(i, j)));
			}
		}
		return matref;
	}

	template<class T, class Property, class A, template<class, class>class C>
	const typename MMatrixSheet<T, Property, A, C>::MatrixRef MMatrixSheet<T, Property, A, C>::rows(unsigned start, unsigned stop) const
	{
		return const_cast<MMatrixSheet*>(this)->rows(start, stop);
	}

	template<class T, class Property, class A, template<class, class>class C>
	const typename MMatrixSheet<T, Property, A, C>::MatrixRef MMatrixSheet<T, Property, A, C>::cols(unsigned start, unsigned stop) const
	{
		return const_cast<MMatrixSheet*>(this)->cols(start, stop);
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::MatrixRef MMatrixSheet<T, Property, A, C>::leftcols(unsigned  num)
	{
		MatrixRef matref(mRows, num);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < num; ++j){
				matref.setData(i, j, &((*this)(i, j)));
			}
		}
		return matref;
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::MatrixRef MMatrixSheet<T, Property, A, C>::rightcols(unsigned num)
	{
		MatrixRef matref(mRows, num);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = mCols - num; j < mCols; ++j){
				matref.setData(i, j - (mCols - num), &((*this)(i, j)));
			}
		}
		return matref;
	}

	template<class T, class Property, class A, template<class, class>class C>
	const typename MMatrixSheet<T, Property, A, C>::MatrixRef MMatrixSheet<T, Property, A, C>::leftcols(unsigned  num) const
	{
		return const_cast<MMatrixSheet*>(this)->leftcols(num);
	}

	template<class T, class Property, class A, template<class, class>class C>
	const typename MMatrixSheet<T, Property, A, C>::MatrixRef MMatrixSheet<T, Property, A, C>::rightcols(unsigned num) const
	{
		return const_cast<MMatrixSheet*>(this)->rightcols(num);
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::MatrixRef MMatrixSheet<T, Property, A, C>::toprows(unsigned num)
	{
		MatrixRef matref(num, mCols);
		for (unsigned i = 0; i < num; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				matref.setData(i, j, &((*this)(i, j)));
			}
		}
		return matref;
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::MatrixRef MMatrixSheet<T, Property, A, C>::bottomrows(unsigned num)
	{
		MatrixRef matref(num, mCols);
		for (unsigned i = mRows - num; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				matref.setData(i - (mRows - num), j, &((*this)(i, j)));
			}
		}
		return matref;
	}


	template<class T, class Property, class A, template<class, class>class C>
	const typename MMatrixSheet<T, Property, A, C>::MatrixRef MMatrixSheet<T, Property, A, C>::toprows(unsigned num) const
	{
		return const_cast<MMatrixSheet*>(this)->toprows(num);
	}

	template<class T, class Property, class A, template<class, class>class C>
	const typename MMatrixSheet<T, Property, A, C>::MatrixRef MMatrixSheet<T, Property, A, C>::bottomrows(unsigned num) const
	{
		return const_cast<MMatrixSheet*>(this)->bottomrows(num);
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::MatrixRef MMatrixSheet<T, Property, A, C>::map(unsigned startrow, unsigned startcol, unsigned rownum, unsigned colnum)
	{
		MatrixRef matref(rownum, colnum);
		for (unsigned i = startrow; i < startrow + rownum; ++i){
			for (unsigned j = startcol; j < startcol + colnum; ++j){
				matref.setData(i - startrow, j - startcol, &((*this)(i, j)));
			}
		}
		return matref;
	}

	template<class T, class Property, class A, template<class, class>class C>
	const typename MMatrixSheet<T, Property, A, C>::MatrixRef MMatrixSheet<T, Property, A, C>::map(unsigned startrow, unsigned startcol, unsigned rownum, unsigned colnum) const
	{
		return const_cast<MMatrixSheet*>(this)->map(startrow, startcol, rownum, colnum);
	}

	template<class T, class Property, class A, template<class, class>class C>
	unsigned MMatrixSheet<T, Property, A, C>::rows() const
	{
		return mRows;
	}

	template<class T, class Property, class A, template<class, class>class C>
	unsigned MMatrixSheet<T, Property, A, C>::cols() const
	{
		return mCols;
	}

	template<class T, class Property, class A, template<class, class>class C>
	unsigned MMatrixSheet<T, Property, A, C>::size() const
	{
		return mRows*mCols;
	}

	template<class T, class Property, class A, template<class, class>class C>
	unsigned MMatrixSheet<T, Property, A, C>::length() const{
		return mRows > mCols ? mRows : mCols;
	}

	template<class T, class Property, class A, template<class, class>class C>
	bool MMatrixSheet<T, Property, A, C>::empty() const
	{
		return mData.empty();
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::clear()
	{
		mRows = 0;
		mCols = 0;
		mData.clear();
		mPropertyMap.clear();
	}

	//
	// 修改矩阵大小
	//
	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::resize(unsigned rownum, unsigned colnum)
	{
		if (rownum == mRows && colnum == mCols){
			return;
		}
		if (rownum == mRows){
			mRows = rownum;
			mCols = colnum;
			mData.resize(mRows*mCols);
		}
		else if (colnum == mCols){
			int num = rownum - mRows;
			if (num > 0){
				std::vector<T> vals(num);
				for (int i = mCols ; i > 0;--i){
					mData.insert(mData.begin() + i*mRows, vals.begin(), vals.end());
				}
			}
			else{
				for (int i = mCols; i > 0; --i){
					mData.erase(mData.begin() + i*mRows  + num, mData.begin() + i*mRows);
				}
			}
			mRows = rownum;
			mCols = colnum;
		}
		else if (colnum != mCols && rownum != mRows){
			int num = rownum - mRows;
			if (num > 0){
				std::vector<T> vals(num);
				for (int i = mCols; i > 0; --i){
					mData.insert(mData.begin() + i*mRows, vals.begin(), vals.end());
				}
			}
			else{
				for (int i = mCols; i > 0; --i){
					mData.erase(mData.begin() + i*mRows + num, mData.begin() + i*mRows);
				}
			}
			int size = rownum*colnum;
			mData.resize(size);
			mRows = rownum;
			mCols = colnum;
		}
	}

	//
	// 预分配内存
	//
	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::reserve(unsigned rownum, unsigned colnum)
	{
		mData.reserve(rownum*colnum);
	}

	//
	// 增加数据
	//
	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::push_back(const metadata_type& vals, bool iscol)
	{
		if (iscol){
			if (vals.size() < mRows)
				return;
			std::copy_n(vals.begin(), mRows, std::back_inserter(mData));
			mCols += 1;
		}
		else{
			if (vals.size() < mCols)
				return;
			for (unsigned i = 0; i < mCols; ++i){
				mData.insert(mData.begin() + (i + 1)*mRows + i, vals[i]);
			}
			mRows += 1;
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::insert_row(unsigned pos, const metadata_type& vals)
	{
		if (vals.size() < mCols)
			return;

		for (unsigned i = 0; i < mCols; ++i){
			mData.insert(mData.begin() + i*mRows + i + pos, vals[i]);
		}
		mRows += 1;
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::insert_col(unsigned pos, const metadata_type& vals)
	{
		if (vals.size() < mRows)
			return;
		mData.insert(mData.begin() + pos*mRows, vals.begin(), vals.begin() + mRows);
		mCols += 1;
	}


	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::insert_mat(const MMatrixSheet<T, Property, A, C>& other, unsigned pos, bool isCol){
		if (isCol){
			if (other.rows() != mRows){
				return;
			}
			mData.insert(mData.begin() + pos*mRows, other.mData.begin(), other.mData.end());
			mCols += other.cols();
		}
		else{
			if (other.cols() != mCols)
				return;
			for (unsigned i = 0; i < other.rows(); ++i){
				insert_row(pos + i,other.row(i));
			}
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::push_mat(const MMatrixSheet<T, Property, A, C>& other, bool isCol){
		if (isCol){
			if (other.rows() != mRows){
				return;
			}
			std::copy(other.mData.begin(), other.mData.end(), std::back_inserter(mData));
			mCols += other.cols();
		}
		else{
			if (other.cols() != mCols)
				return;
			for (unsigned i = 0; i < other.rows(); ++i){
				push_back(other.row(i), false);
			}
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::pop_back(bool iscol)
	{
		if (iscol){
			mData.erase(mData.end() - mRows, mData.end());
			mCols -= 1;
		}
		else{
			for (unsigned i = 0; i < mCols; ++i){
				mData.erase(mData.begin() + (i+1)*mRows - i);
			}
			mRows -= 1;
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::pop_front(bool iscol)
	{
		if (iscol){
			mData.erase(mData.begin(), mData.begin() + mRows);
			mCols -= 1;
		}
		else{
			for (unsigned i = 0; i < mCols; ++i){
				mData.erase(mData.begin() + i*mRows - i);
			}
			mRows -= 1;
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::remove_row(unsigned rowindex)
	{
		for (unsigned i = 0; i < mCols; ++i){
			auto it = mData.begin() + (i*mRows + rowindex - i);
			mData.erase(it);
		}
		mRows -= 1;
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::remove_col(unsigned colindex)
	{
		mData.erase(mData.begin() + colindex*mRows, mData.begin() + mRows *colindex + mRows);
		mCols -= 1;
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::remove_rows(unsigned start, unsigned stop)
	{
		unsigned num = stop - start;
		for (unsigned i = 0; i < mCols; ++i){
			mData.erase(mData.begin() + i*mRows + start - i*num, mData.begin() + i*mRows + stop - i*num);
		}
		mRows -= num;
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::remove_cols(unsigned start, unsigned stop)
	{
		int num = stop - start;
		mData.erase(mData.begin() + start*mRows, mData.begin() + stop*mRows);
		mCols -= num;
	}

	//
	// 重置数据
	//
	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::reset()
	{
		mRows = 0;
		mCols = 0;
		mData.swap(C<T, A>());
		mPropertyMap.clear();
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::swap(MMatrixSheet& other)
	{
		mData.swap(other.mData);
		mRows = ohter.mRows;
		mCols = other.mCols;
	}

	//
	// 双索引操作
	//
	template<class T, class Property, class A, template<class, class>class C>
	T& MMatrixSheet<T, Property, A, C>::operator()(unsigned rowindex, unsigned colindex)
	{
		return mData[colindex*mRows + rowindex];
	}

	template<class T, class Property, class A, template<class, class>class C>
	const T& MMatrixSheet<T, Property, A, C>::operator()(unsigned rowindex, unsigned colindex) const
	{
		return mData[colindex*mRows + rowindex];
	}

	template<class T, class Property, class A, template<class, class>class C>
	T& MMatrixSheet<T, Property, A, C>::operator()(unsigned index)
	{
		return mData[index];
	}

	template<class T, class Property, class A, template<class, class>class C>
	const T& MMatrixSheet<T, Property, A, C>::operator()(unsigned index) const
	{
		return mData[index];
	}

	template<class T, class Property, class A, template<class, class>class C>
	T& MMatrixSheet<T, Property, A, C>::at(unsigned rowindex, unsigned colindex){
		return mData.at(colindex*mRows + rowindex);
	}

	template<class T, class Property, class A, template<class, class>class C>
	const T& MMatrixSheet<T, Property, A, C>::at(unsigned rowindex, unsigned colindex) const{
		return mData.at(colindex*mRows + rowindex);
	}

	template<class T, class Property, class A, template<class, class>class C>
	T& MMatrixSheet<T, Property, A, C>::at(unsigned index){
		return mData.at(index);
	}

	template<class T, class Property, class A, template<class, class>class C>
	const T& MMatrixSheet<T, Property, A, C>::at(unsigned index) const{
		return mData.at(index);
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::set(unsigned rowindex, unsigned colindex, typename MMatrixSheet<T, Property, A, C>::const_reference_type val){
		mData[colindex*mRows + rowindex] = val;
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::const_reference_type MMatrixSheet<T, Property, A, C>::get(unsigned rowindex, unsigned colindex) const
	{
		return mData[colindex*mRows + rowindex];
	}


	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::set(unsigned index, typename MMatrixSheet<T, Property, A, C>::const_reference_type val){
		mData[index] = val;
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::const_reference_type MMatrixSheet<T, Property, A, C>::get(unsigned index) const
	{
		return mData[index];
	}
	//
	// 迭代器操作
	//
	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::iterator MMatrixSheet<T, Property, A, C>::begin()
	{
		return mData.begin();
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::iterator MMatrixSheet<T, Property, A, C>::end()
	{
		return mData.end();
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::row_iterator MMatrixSheet<T, Property, A, C>::begin_row(unsigned rowindex)
	{
		return  row_iterator(&mData[rowindex], this, rowindex);
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::row_iterator MMatrixSheet<T, Property, A, C>::end_row(unsigned rowindex)
	{
		return  row_iterator(&mData[mData.size() - 1] + rowindex + 1, this, rowindex, mCols);
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::iterator MMatrixSheet<T, Property, A, C>::begin_col(unsigned colindex)
	{
		return mData.begin() + colindex*mRows;
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::iterator MMatrixSheet<T, Property, A, C>::end_col(unsigned colindex)
	{
		return mData.begin() + (colindex + 1)*mRows;
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::const_iterator MMatrixSheet<T, Property, A, C>::begin() const
	{
		return mData.begin();
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::const_iterator MMatrixSheet<T, Property, A, C>::end() const
	{
		return mData.end();
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename const MMatrixSheet<T, Property, A, C>::row_iterator MMatrixSheet<T, Property, A, C>::begin_row(unsigned rowindex) const
	{
		return  row_iterator(&mData[rowindex], this, rowindex);
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename const MMatrixSheet<T, Property, A, C>::row_iterator MMatrixSheet<T, Property, A, C>::end_row(unsigned rowindex) const
	{
		return  row_iterator(&mData[mData.size() - 1] + rowindex + 1, this, rowindex, mCols);
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::const_iterator MMatrixSheet<T, Property, A, C>::begin_col(unsigned colindex) const
	{
		return const_cast<MMatrixSheet*>(this)->begin_col(colindex);
	}

	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::const_iterator MMatrixSheet<T, Property, A, C>::end_col(unsigned colindex) const
	{
		return const_cast<MMatrixSheet*>(this)->end_col(colindex);
	}

	//
	// 获取数据地址
	//
	template<class T, class Property, class A, template<class, class>class C>
	T* MMatrixSheet<T, Property, A, C>::mem_ptr()
	{
		return &mData[0];
	}

	template<class T, class Property, class A, template<class, class>class C>
	const T* MMatrixSheet<T, Property, A, C>::mem_ptr() const
	{
		return &mData[0];
	}

	//
	// 引用数据对象
	//
	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::metadata_type& MMatrixSheet<T, Property, A, C>::reference_obj()
	{
		return mData;
	}

	template<class T, class Property, class A, template<class, class>class C>
	const typename MMatrixSheet<T, Property, A, C>::metadata_type& MMatrixSheet<T, Property, A, C>::reference_obj() const
	{
		return mData;
	}

	//
	// operator 操作
	//
	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::operator+=(const_reference_type value){
		for (auto& v : *this){
			v += value;
		}
		return *this;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::operator-=(const_reference_type value){
		for (auto& v : *this){
			v -= value;
		}
		return *this;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::operator*=(const_reference_type value){
		for (auto& v : *this){
			v *= value;
		}
		return *this;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::operator/=(const_reference_type value){
		for (auto& v : *this){
			v /= value;
		}
		return *this;
	}

	//
	// 矩阵操作
	//
	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::operator+=(const MMatrixSheet& other){
		if (mRows != other.mRows || mCols != other.mCols)
			return *this;
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				(*this)(i, j) += other(i, j);
			}
		}
		return *this;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::operator-=(const MMatrixSheet& other){
		if (mRows != other.mRows || mCols != other.mCols)
			return *this;
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				(*this)(i, j) -= other(i, j);
			}
		}
		return *this;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::operator*=(const MMatrixSheet& other){
		if (mRows != other.mRows || mCols != other.mCols){
			return *this;
		}
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				(*this)(i, j) *= other(i, j);
			}
		}
		return *this;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::operator/=(const MMatrixSheet& other){
		if (mRows != other.mRows || mCols != other.mCols)
			return *this;
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				(*this)(i, j) /= other(i, j);
			}
		}
		return *this;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	// 矩阵和向量的叉乘
	//
	///////////////////////////////////////////////////////////////////////////
	template<class T, class Property, class A, template<class, class>class C>
	typename MMatrixSheet<T, Property, A, C>::metadata_type MMatrixSheet<T, Property, A, C>::operator*=(const metadata_type& colvec){
		metadata_type temp;
		if (mCols != colvec.size()){
			return temp;
		}
		temp.resize(mRows);
		for (unsigned i = 0; i < mRows; ++i){
			temp(i) = 0;
			for (unsigned j = 0; j < mCols; ++j){
				temp(i) += (*this)(i, j)*colvec[j];
			}
		}
		return temp;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	// 矩阵和矩阵的叉乘
	//
	//////////////////////////////////////////////////////////////////////////
	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::multplie(const MMatrixSheet& other){
		if (mCols != other.mRows){
			return *this;
		}
		MMatrixSheet temp(mRows, other.mCols);
		for (int i = 0; i < mRows; ++i){
			for (int j = 0; j < other.mCols; ++j){
				temp(i, j) = 0;
				for (int k = 0; k < mCols; ++k){
					temp(i, j) += (*this)(i, k)*other(k, j);
				}
			}
		}

		mCols = other.mCols;
		mData = temp.mData;
		return *this;
	}


	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::multplie(const metadata_type& other){
		*this *= other;
		return *this;
	}




	//////////////////////////////////////////////////////////////////////////
	//
	// 快速叉乘
	//
	//////////////////////////////////////////////////////////////////////////
	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::fast_multplie(const MMatrixSheet& other) const{
		if (empty()){
			return *this;
		}
		MMatrixSheet temp(mRows, other.mCols);
		if (mCols != other.mRows){
			return temp;
		}
		try{
			concurrency::array_view<const T, 2> vcols(mCols, mRows, mData);
			concurrency::array_view<const T, 2> vcols2(other.mCols, other.mRows, other.mData);
			concurrency::array_view<T, 2> vtemp(other.mCols, mRows, temp.mData);

			unsigned int __col = other.mCols;
			unsigned int __row = mRows;
			unsigned int __newrow = mCols;

			concurrency::parallel_for_each(vtemp.extent, [=](concurrency::index<2> idx) restrict(amp){
				unsigned int r = idx[1];
				unsigned int j = idx[0];
				T res = T();
				for (int k = 0; k < __newrow; ++k){
					res = res + vcols(k, r)*vcols2(j, k);
				}
				vtemp(j, r) = res;
			});
			vtemp.synchronize();
			vtemp(0, 0);
		}
		catch (...){
			std::cout << "显存不足已支撑快速计算转用普通的计算方式"<<std::endl;
			for (int i = 0; i < mRows; ++i){
				for (int j = 0; j < other.mCols; ++j){
					temp(i, j) = T();
					for (int k = 0; k < mCols; ++k){
						temp(i, j) += (*this)(i, k)*other(k, j);
					}
				}
			}
		}
		return temp;
	}


	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::fast_multplie(const metadata_type& colvec) const{
		if (empty()){
			return *this;
		}
		MMatrixSheet temp(mRows, 1);
		if (mCols != colvec.size()){
			return temp;
		}
	
		try{
			concurrency::array_view<const T, 2> vcols(mCols, mRows, mData);
			concurrency::array_view<const T, 1> vcols2(colvec.size(), colvec);
			concurrency::array_view<T, 1> vtemp(mRows, temp.mData);
			unsigned int __newrow = mCols;
			concurrency::parallel_for_each(vtemp.extent, [=](concurrency::index<1> idx) restrict(amp){
				unsigned int r = idx[0];
				T res = T();
				for (int k = 0; k < __newrow; ++k){
					res += vcols(k, r)*vcols2(k);
				}
				vtemp[r] = res;
			});
			vtemp.synchronize();
			vtemp(0);
		}
		catch (...){
			for (unsigned i = 0; i < mRows; ++i){
				temp(i,0) = 0;
				for (unsigned j = 0; j < mCols; ++j){
					temp(i,0) += (*this)(i, j)*colvec[j];
				}
			}
		}
		return temp;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	// 计算行列式
	//
	//////////////////////////////////////////////////////////////////////////
	template<class T, class Property, class A, template<class, class>class C>
	T MMatrixSheet<T, Property, A, C>::det() const
	{
		if (empty()){
			return T();
		}
			
		if (size() == 1){
			return mData[0];
		}

		if (mRows != mCols){
			return T();
		}

#ifndef __ARMADILLO__
		int switchNum = 0;
		MMatrixSheet m = *this;
		for (int i = 0; i < m.mRows; ++i){
			if (m(i, i) == T()){
				int index = 0;
				for (int k = i; k < m.mCols; ++k){
					if (m(i, k) != T()){
						index = k;
						break;
					}
				}

				for (int k = 0; k < mRows; ++k){
					std::swap(m(k, i), m(k,index));
				}
				++switchNum;
			}

			T val = m(i, i);
			for (int j = i + 1; j < m.cols(); ++j){
				if (m(i, j) != T()){
					T res = m(i, j) / val;
					//
					// cj - ci * res
					//
					for (int k = 0; k < mRows; ++k){
						m(k, j) -= m(k, i)*res;
					}
				}
			}
		}

		T res = m(0, 0);
		for (int i = 1; i < m.rows(); ++i){
			res *= m(i, i);
		}

		return res*std::pow(-1, switchNum);
#else
		//
		// 使用专业的计算速度会更快些
		//
		arma::Mat<T> m1(mem_ptr(), rows(), cols());
		return arma::det(m1);
#endif
	}


	//
	// 求矩阵的逆
	//
	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::inv() const{
#ifdef __ARMADILLO__
		arma::Mat<T> m(&mData[0],mRows,mCols);
		arma::Mat<T> m1 = arma::inv(m);
		MMatrixSheet mout(m1.memptr(),m1.n_rows,m1.n_cols);
		return mout;
#else
		static_assert(false, "Error MMatrixSheet<T, Property, A, C>::inv() Not support inverse of the matrix expression and define __ARMADILLO__ to support inverse");
#endif
	}


	//////////////////////////////////////////////////////////////////////////
	//
	// 转置
	//
	//////////////////////////////////////////////////////////////////////////
	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::t() const{
		MMatrixSheet temp(mCols, mRows);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				temp(j, i) = (*this)(i, j);
			}
		}
		return temp;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	// 快速转置
	//
	//////////////////////////////////////////////////////////////////////////
	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::fast_t() const{
		if (empty()){
			return *this;
		}
		MMatrixSheet temp(mCols, mRows);
		try{
			std::vector<T> __datas = mData;
			concurrency::array_view<const T, 2> vcols(mCols, mRows, __datas);
			concurrency::array_view<T, 2> vcols2(mRows, mCols, temp.mData);
			concurrency::parallel_for_each(vcols.extent, [=](concurrency::index<2> idx) restrict(amp){
				unsigned r = idx[0];
				unsigned c = idx[1];
				vcols2(c, r) = vcols(r, c);
			});
			vcols2.synchronize();
			vcols2(0, 0);
		}
		catch (...){
			for (unsigned i = 0; i < mRows; ++i){
				for (unsigned j = 0; j < mCols; ++j){
					temp(j, i) = (*this)(i, j);
				}
			}
		}
		return temp;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	// 操作
	//
	//////////////////////////////////////////////////////////////////////////
	template<class T, class Property, class A, template<class, class>class C>
	template<class F>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::transform_copy(F fun) const
	{
		MMatrixSheet temp(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				temp(i, j) = fun((*this)(i, j));
			}
		}
		return temp;
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class F>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::transform(F fun){
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				(*this)(i, j) = fun((*this)(i, j));
			}
		}
		return *this;
	}



	//////////////////////////////////////////////////////////////////////////
	//
	// 快速操作
	//
	//////////////////////////////////////////////////////////////////////////
	template<class T, class Property, class A, template<class, class>class C>
	template<class F>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::fast_transform_copy(F fun) const
	{
		if (empty()){
			return *this;
		}
		MMatrixSheet temp = *this;
		MMatrixSheet tmat1 = temp.fast_t();
		std::vector<T> __datas(mRows*mCols);
		concurrency::array_view<const T, 2> vcols(tmat1.rows(), tmat1.cols(), tmat1.mData);
		concurrency::array_view<T, 2> vcols2(tmat1.rows(), tmat1.cols(), __datas);
		concurrency::parallel_for_each(vcols.extent, [=](concurrency::index<2> idx) restrict(amp){
			vcols2[idx] = fun(vcols, idx);
		});
		vcols.synchronize();
		vcols2(0, 0);
		tmat1.mData = __datas;
		temp = tmat1.fast_t();
		return temp;
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class F>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::fast_transform(F fun){
		if (empty()){
			return *this;
		}
		MMatrixSheet tmat1 = fast_t();
		concurrency::array_view<T, 2> vcols(mRows, mCols, tmat1.mData);
		concurrency::parallel_for_each(vcols.extent, [=](concurrency::index<2> idx) restrict(amp){
			vcols[idx] = fun(vcols, idx);
		});
		vcols.synchronize();
		vcols(0, 0);
		mData = tmat1.fast_t().mData;
		return *this;
	}


	template<class T, class Property, class A, template<class, class>class C>
	template<class F>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::fast_transform_copy1(F fun) const{
		if (empty()){
			return *this;
		}
		MMatrixSheet<T, Property, A, C> res = *this;
		concurrency::array_view<T, 1> vcols(mRows*mCols, res.mData);
		concurrency::parallel_for_each(vcols.extent, [=](concurrency::index<1> idx) restrict(amp){
			vcols[idx] = fun(vcols, idx);
		});
		vcols.synchronize();
		vcols(0);
		return res;
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class F>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::fast_transform1(F fun){
		if (empty()){
			return *this;
		}
		concurrency::array_view<T, 1> vcols(mRows*mCols, mData);
		concurrency::parallel_for_each(vcols.extent, [=](concurrency::index<1> idx) restrict(amp){
			vcols[idx] = fun(vcols, idx);
		});
		vcols.synchronize();
		vcols(0);
		return *this;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	// 反转
	//
	//////////////////////////////////////////////////////////////////////////
	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::reverse(bool iscol)
	{
		if (iscol){
			for (unsigned i = 0; i < mCols; ++i){
				std::reverse(mData.begin() + i*mRows, mData.begin() + (i + 1)*mRows);
			}
		}
		else{
			int rc = mCols / 2;
			for (unsigned i = 0; i < mRows; ++i){
				for (unsigned j = 0; j < rc; ++j){
					std::swap((*this)(i, j), (*this)(i, mCols - j - 1));
				}
			}
		}
		return *this;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::reverse(bool iscol) const{
		MMatrixSheet temp = *this;
		temp.reserve(iscol);
		return temp;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::slice(unsigned rowstep, unsigned colstep)
	{
		int __row = mRows / rowstep;
		if (mRows % rowstep > 0){
			__row += 1;
		}

		int __col = mCols / colstep;
		if (mCols % colstep > 0){
			__col += 1;
		}

		MMatrixSheet temp(__row, __col);
		int r = 0;
		for (int i = 0; i < mRows; i += rowstep){
			int c = 0;
			for (int j = 0; j < mCols; j += colstep){
				temp(r, c++) = (*this)(i, j);
			}
			++r;
		}
		return temp;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::slice(unsigned rowstep, unsigned colstep) const
	{
		return const_cast<MMatrixSheet*>(this)->slice(rowstep, colstep);
	}

	//
	// 调整数据
	// 例如将数据调整到+-180之间
	// adjust(360,-180,180)
	//
	template<class T, class Property, class A, template<class, class>class C>
	template<class Less, class Greater, class Equal_to>
	void MMatrixSheet<T, Property, A, C>::adjust(const_reference_type adjustvalue, const_reference_type __minvalue,
		const_reference_type __maxvalue, const_reference_type excepty,
		Less __lessfun,
		Greater __greaterfun,
		Equal_to __equalfun)
	{
                for (unsigned i = 0; i < mRows; ++i){
                        for (unsigned j = 0; j < mCols; ++j){
				if (__equalfun((*this)(i, j), excepty))
					continue;
				if (__lessfun((*this)(i, j), __minvalue)){
					(*this)(i, j) += adjustvalue;
				}
				else if (__greaterfun((*this)(i, j), __maxvalue)){
					(*this)(i, j) -= adjustvalue;
				}
			}
		}
	}

	//
	// 替换
	//
	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::replace(
		typename MConstParamWrape<T>::type oldval,
		typename MConstParamWrape<T>::type newval,
		cmp prd)
	{
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				if (prd((*this)(i, j), oldval)){
					(*this)(i, j) = newval;
				}
			}
		}
		return *this;
	}

	//
	// 检查包含
	//
	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	bool MMatrixSheet<T, Property, A, C>::contains(const_reference_type val, cmp prd)
	{
		auto it = std::find_if(mData.begin(), mData.end(), std::bind(prd,std::placeholders::_1, val));
		if (it == mData.end())
			return false;
		return true;
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	bool MMatrixSheet<T, Property, A, C>::index(const_reference_type val, unsigned& rowindex, unsigned& colindex, cmp prd)
	{
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				if (prd((*this)(i, j), val)){
					rowindex = i;
					colindex = j;
					return true;
				}
			}
		}
		return false;
	}

	//
	// 搜索满足条件的值
	//
	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	typename MMatrixSheet<T, Property, A, C>::metadata_type MMatrixSheet<T, Property, A, C>::search(const_reference_type val, cmp prd) const
	{
		metadata_type temp;
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				if (prd((*this)(i, j), val)){
					temp.push_back(val);
				}
			}
		}
	}

	//
	// 统计计算
	//
	template<class T, class Property, class A, template<class, class>class C>
	T MMatrixSheet<T, Property, A, C>::max_value() const
	{
		if (mRows == 0 || mCols == 0)
			return T();
		return *(std::max_element(mData.begin(), mData.end()));
	}

	template<class T, class Property, class A, template<class, class>class C>
	T MMatrixSheet<T, Property, A, C>::min_value() const
	{
		if (mRows == 0 || mCols == 0)
			return T();
		return *(std::min_element(mData.begin(), mData.end()));
	}

	template<class T, class Property, class A, template<class, class>class C>
	T MMatrixSheet<T, Property, A, C>::max_index(unsigned& rowindex, unsigned& colindex) const
	{
		if (mRows == 0 || mCols == 0)
			return T();
		auto it = std::max_element(mData.begin(), mData.end());
		int num = std::distance(mData.begin(), it);
                colindex = num / mRows;
                rowindex = num % mRows;
		return *it;
	}

	template<class T, class Property, class A, template<class, class>class C>
	T MMatrixSheet<T, Property, A, C>::min_index(unsigned& rowindex, unsigned& colindex) const
	{
		if (mRows == 0 || mCols == 0)
			return T();
		auto it = std::min_element(mData.begin(), mData.end());
		int num = std::distance(mData.begin(), it);
                colindex  = num / mRows;
                rowindex  = num % mRows;
		return *it;
	}

	template<class T, class Property, class A, template<class, class>class C>
	T MMatrixSheet<T, Property, A, C>::sum() const
	{
		if (mRows == 0 || mCols == 0)
			return T();
		return std::accumulate(mData.begin(), mData.end(), T(0.0));
	}

	template<class T, class Property, class A, template<class, class>class C>
	T MMatrixSheet<T, Property, A, C>::aval() const
	{
		if (mRows == 0 || mCols == 0)
			return T();
		return sum() / (mRows*mCols);
	}

	template<class T, class Property, class A, template<class, class>class C>
	T MMatrixSheet<T, Property, A, C>::standardiff() const
	{
		if (mRows == 0 || mCols == 0)
			return T();
		return  std::sqrt(squaldiff());
	}

	template<class T, class Property, class A, template<class, class>class C>
	T MMatrixSheet<T, Property, A, C>::squaldiff() const
	{
		if (mRows == 0 || mCols == 0)
			return T();
		T val = aval();
		T sum = std::accumulate(mData.begin(), mData.end(), T(0.0), [&](T v1, T v){
			return v1 + (v - val)*(v - val);
		});
		return sum / (mRows*mCols);
	}

	template<class T, class Property, class A, template<class, class>class C>
	T MMatrixSheet<T, Property, A, C>::standardiff_if(const_reference_type avl) const
	{
		if (mRows == 0 || mCols == 0)
			return T();
		return  std::sqrt(squaldiff_if(avl));
	}

	template<class T, class Property, class A, template<class, class>class C>
	T MMatrixSheet<T, Property, A, C>::squaldiff_if(const_reference_type avl) const
	{
		if (mRows == 0 || mCols == 0)
			return T();
		T sum = std::accumulate(mData.begin(), mData.end(), T(0.0), [&](T v1, T v){
			return v1 + (v - avl)*(v - avl);
		});
		return sum / (mRows*mCols);
	}

	//
	// 忽略某些值再计算
	//
	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	T MMatrixSheet<T, Property, A, C>::max_value(const_reference_type excepty, cmp prd) const
	{
		std::vector<T> temp;
		for (auto& v : mData){
			if (prd(v, excepty)){
				continue;
			}
			temp.push_back(v);
		}
		if (temp.empty())
			return T();
		return *(std::max_element(temp.begin(), temp.end()));
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	T MMatrixSheet<T, Property, A, C>::min_value(const_reference_type excepty, cmp prd) const
	{
		std::vector<T> temp;
		for (auto& v : mData){
			if (prd(v, excepty)){
				continue;
			}
			temp.push_back(v);
		}
		if (temp.empty())
			return T();
		return *(std::min_element(temp.begin(), temp.end()));
	}


	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	T MMatrixSheet<T, Property, A, C>::max_index(unsigned& rowindex, unsigned& colindex, const_reference_type excepty, cmp prd) const
	{
		std::vector<T> temp;
		for (auto& v : mData){
			if (prd(v, excepty)){
				continue;
			}
			temp.push_back(v);
		}
		if (temp.empty())
			return T();
		T maxv = *(std::max_element(temp.begin(), temp.end()));
		index(maxv, rowindex, colindex);
	}


	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	T MMatrixSheet<T, Property, A, C>::min_index(unsigned& rowindex, unsigned& colindex, const_reference_type excepty, cmp prd) const
	{
		std::vector<T> temp;
		for (auto& v : mData){
			if (prd(v, excepty)){
				continue;
			}
			temp.push_back(v);
		}
		if (temp.empty())
			return T();
		T minv = *(std::min_element(temp.begin(), temp.end()));
		index(maxv, rowindex, colindex);
	}


	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	T MMatrixSheet<T, Property, A, C>::sum(const_reference_type excepty, cmp prd) const
	{
		std::vector<T> temp;
		for (auto& v : mData){
			if (prd(v, excepty)){
				continue;
			}
			temp.push_back(v);
		}
		if (temp.empty())
			return T();
		return std::accumulate(temp.begin(), temp.end(), T(0.0));
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	T MMatrixSheet<T, Property, A, C>::aval(const_reference_type excepty, cmp prd) const
	{
		std::vector<T> temp;
		for (auto& v : mData){
			if (prd(v, excepty)){
				continue;
			}
			temp.push_back(v);
		}
		if (temp.empty())
			return T();
		return std::accumulate(temp.begin(), temp.end(), T(0.0)) / temp.size();
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	T MMatrixSheet<T, Property, A, C>::standardiff(const_reference_type excepty, cmp prd) const
	{
		std::vector<T> temp;
		for (auto& v : mData){
			if (prd(v, excepty)){
				continue;
			}
			temp.push_back(v);
		}
		if (temp.empty())
			return T();

		T val = aval(excepty, prd);
		T sum = std::accumulate(temp.begin(), temp.end(), T(0.0), [&](T v1, T v){
			return v1 + (v - val)*(v - val);
		});
		return std::sqrt(sum / (temp.size()));
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	T MMatrixSheet<T, Property, A, C>::squaldiff(const_reference_type excepty, cmp prd) const
	{
		std::vector<T> temp;
		for (auto& v : mData){
			if (prd(v, excepty)){
				continue;
			}
			temp.push_back(v);
		}
		if (temp.empty())
			return T();

		T val = aval(excepty, prd);
		T sum = std::accumulate(temp.begin(), temp.end(), T(0.0), [&](T v1, T v){
			return v1 + (v - val)*(v - val);
		});
		return sum / (temp.size());
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	T MMatrixSheet<T, Property, A, C>::standardiff_if(const_reference_type avl, const_reference_type excepty, cmp prd) const
	{
		std::vector<T> temp;
		for (auto& v : mData){
			if (prd(v, excepty)){
				continue;
			}
			temp.push_back(v);
		}
		if (temp.empty())
			return T();

		T sum = std::accumulate(temp.begin(), temp.end(), T(0.0), [&](T v1, T v){
			return v1 + (v - avl)*(v - avl);
		});
		return std::sqrt(sum / (temp.size()));
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	T MMatrixSheet<T, Property, A, C>::squaldiff_if(const_reference_type  avl, const_reference_type excepty, cmp prd) const
	{
		std::vector<T> temp;
		for (auto& v : mData){
			if (prd(v, excepty)){
				continue;
			}
			temp.push_back(v);
		}
		if (temp.empty())
			return T();

		T sum = std::accumulate(temp.begin(), temp.end(), T(0.0), [&](T v1, T v){
			return v1 + (v - avl)*(v - avl);
		});
		return sum / (temp.size());
	}

	//
	// 打印
	//
	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::print(const char* msg, std::ostream& os) const{
		os << msg << "[rows=" << mRows << "; cols=" << mCols << "]" << std::endl;
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				os.width(10);
				os.setf(std::ios::left);
				os << (*this)(i, j);
			}
			os << std::endl;
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::load(const char* filename, bool isbinary){
		if (isbinary){
			std::ifstream inFile(filename, std::ios_base::binary);
			if (inFile.fail())
				return;
			reset();
			inFile.read((char*)(&mRows), sizeof(mRows));
			inFile.read((char*)(&mCols), sizeof(mCols));
			if (mRows == 0 || mCols == 0)
				return;
			mData.resize(mRows*mCols);
			inFile.read((char*)(&mData[0]), mRows*mCols*sizeof(T));
			inFile.close();
		}
		else{
			std::ifstream inFile(filename);
			if (inFile.fail())
				return;
			inFile >> mRows >> mCols;
			if (mRows == 0 || mCols == 0)
				return;
			mData.resize(mRows*mCols);
			for (unsigned i = 0; i < mRows; ++i){
				for (unsigned j = 0; j < mCols; ++j){
					inFile >> (*this)(i, j);
				}
			}
			inFile.close();
		}
	}


	template<class T, class Property, class A, template<class, class>class C>
	template<class HelpType>
	void MMatrixSheet<T, Property, A, C>::loadAll(const char* filename, const char* spliter,bool bIsMatch){
		reset();
		HelpType mStr;
		mStr.fromFile(filename);
		if (mStr.empty()){
			return;
		}
		std::vector<HelpType> vStr;
		mStr.split(L"\n", vStr);
		if (vStr.empty()){
			return;
		}
		HelpType mStr2 = vStr[0];
		std::vector<T> vData;
		mStr2.split(spliter, vData);
		if (vData.empty()){
			return;
		}
		int row = vStr.size();
		int col = vData.size();

		if (bIsMatch){
			vData.clear();
			mStr.split(spliter, vData);
			if (vData.size() != row*col){
				return;
			}
			mem_ptr(&vData[0], row, col, false);
		}
		else{
			std::vector<T> __datas;
			for (auto& s : vStr){
				vData.clear();
				s.split(spliter, vData);
				if (vData.size() != col){
					vData.resize(col);
				}
				std::copy(vData.begin(), vData.end(), std::back_inserter(__datas));
			}
			if (__datas.empty()){
				return;
			}
			mem_ptr(&__datas[0], row, col, false);
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::save(const char* filename, bool isbinary) const
	{
		if (isbinary){
			std::ofstream outFile(filename, std::ios_base::binary);
			if (outFile.fail())
				return;
			outFile.write((char*)(&mRows), sizeof(mRows));
			outFile.write((char*)(&mCols), sizeof(mCols));
			outFile.write((char*)(&mData[0]), mRows*mCols*sizeof(T));
			outFile.close();
		}
		else{
			std::ofstream outFile(filename);
			if (outFile.fail())
				return;
			outFile << mRows << "\t" << mCols << std::endl;
			for (unsigned i = 0; i < mRows; ++i){
				for (unsigned j = 0; j < mCols; ++j){
					outFile << (*this)(i, j) << "  ";
				}
				outFile << std::endl;
			}
			outFile.close();
		}
	}

	//
	// 使用流操作数据
	//
	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C> ::load(std::istream& is, bool isbinary)
	{
		if (isbinary){
			reset();
			is.read((char*)(&mRows), sizeof(mRows));
			is.read((char*)(&mCols), sizeof(mCols));
			if (mRows == 0 || mCols == 0)
				return;
			mData.resize(mRows*mCols);
			is.read((char*)(&mData[0]), mRows*mCols*sizeof(T));
		}
		else{
			is >> mRows >> mCols;
			if (mRows == 0 || mCols == 0)
				return;
			mData.resize(mRows*mCols);
			for (unsigned i = 0; i < mRows; ++i){
				for (unsigned j = 0; j < mCols; ++j){
					is >> (*this)(i, j);
				}
			}
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	__int64 MMatrixSheet<T, Property, A, C> ::save(std::ostream& os, bool isbinary) const
	{
		if (isbinary){
			os.write((char*)(&mRows), sizeof(mRows));
			os.write((char*)(&mCols), sizeof(mCols));
			os.write((char*)(&mData[0]), mRows*mCols*sizeof(T));
			return sizeof(mRows)+sizeof(mCols)+mRows*mCols*sizeof(T);
		}
		else{
			os << mRows << "\t" << mCols << std::endl;
			for (unsigned i = 0; i < mRows; ++i){
				for (unsigned j = 0; j < mCols; ++j){
					if (j < mCols - 1)
						os << (*this)(i, j) << "\t";
					else
						os << (*this)(i, j);
				}
				os << std::endl;
			}
			return 0;
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::load(std::istream& is, char spliter){
		if (spliter == '\t' || spliter == ' '){
			load(is, false);
			return;
		}
		is >> mRows >> spliter >> mCols;

		if (mRows == 0 || mCols == 0)
			return;
		mData.resize(mRows*mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				if (j < mCols - 1)
					is >> (*this)(i, j) >> spliter;
				else
					is >> (*this)(i, j);
			}
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::save(std::ostream& os, char spliter) const{
		if (spliter == '\t' || spliter == ' '){
			save(os, false);
			return;
		}
		os << mRows << spliter << mCols << std::endl;
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				if (j < mCols - 1)
					os << (*this)(i, j) << spliter;
				else
					os << (*this)(i, j);
			}
			os << std::endl;
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::cos() const
	{
		MMatrixSheet outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				outmat(i, j) = std::cos((*this)(i, j));
			}
		}
		return outmat;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::sin() const
	{
		MMatrixSheet outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				outmat(i, j) = std::sin((*this)(i, j));
			}
		}
		return outmat;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::tan() const
	{
		MMatrixSheet outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				outmat(i, j) = std::tan((*this)(i, j));
			}
		}
		return outmat;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::abs() const
	{
		MMatrixSheet outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				outmat(i, j) = std::abs((*this)(i, j));
			}
		}
		return outmat;
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class U, class A2>
	MMatrixSheet<U, Property, A2, C> MMatrixSheet<T, Property, A, C>::abs() const
	{
		MMatrixSheet<U, Property, A2, C> outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				outmat(i, j) = std::abs((*this)(i, j));
			}
		}
		return outmat;
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class U, class A2 >
	MMatrixSheet<U, Property, A2, C> MMatrixSheet<T, Property, A, C>::sqrt() const{
		MMatrixSheet<U, Property, A2, C> outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				outmat(i, j) = std::sqrt(U((*this)(i, j)));
			}
		}
		return outmat;
	}


	template<class T, class Property, class A, template<class, class>class C>
	template<class U, class A2>
	MMatrixSheet<U, Property, A2, C> MMatrixSheet<T, Property, A, C>::atan2() const
	{
		MMatrixSheet<U, Property, A2, C> outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				const T& val = (*this)(i, j);
				outmat(i, j) = std::atan2(val.imag(), val.real());
			}
		}
		return outmat;
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class U, class A2>
	MMatrixSheet<U, Property, A2, C> MMatrixSheet<T, Property, A, C>::arg() const
	{
		return atan2();
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::conj() const{
		MMatrixSheet outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				const T& val = (*this)(i, j);
				outmat(i, j) = T(val.real(), -1.0*val.real());
			}
		}
		return outmat;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::log() const
	{
		MMatrixSheet outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				outmat(i, j) = std::log((*this)(i, j));
			}
		}
		return outmat;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::log2() const
	{
		MMatrixSheet outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				outmat(i, j) = std::log2((*this)(i, j));
			}
		}
		return outmat;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::log10() const
	{
		MMatrixSheet outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				outmat(i, j) = std::log10((*this)(i, j));
			}
		}
		return outmat;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::sqrt() const
	{
		MMatrixSheet outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				outmat(i, j) = std::sqrt((*this)(i, j));
			}
		}
		return outmat;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::square() const
	{
		MMatrixSheet outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				outmat(i, j) = (*this)(i, j)*(*this)(i, j);
			}
		}
		return outmat;
	}


	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::rad() const{
		MMatrixSheet outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				outmat(i, j) = (*this)(i, j)/180.0*mj::PI;
			}
		}
		return outmat;
	}


	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::pow(const_reference_type n) const{
		MMatrixSheet outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				outmat(i, j) = std::pow((*this)(i, j) ,n);
			}
		}
		return outmat;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::floor() const{
		MMatrixSheet outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				outmat(i, j) = std::floor((*this)(i, j));
			}
		}
		return outmat;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::round() const{
		MMatrixSheet outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				outmat(i, j) = std::round((*this)(i, j));
			}
		}
		return outmat;
	}


	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::replace_min(const_reference_type val) const{
		MMatrixSheet outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				outmat(i, j) = min((*this)(i, j),val);
			}
		}
		return outmat;
	}


	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::replace_max(const_reference_type val) const{
		MMatrixSheet outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				outmat(i, j) = max((*this)(i, j), val);
			}
		}
		return outmat;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::matSum() const{
		MMatrixSheet outmat;
		if (mRows == 1 || mCols == 1)
		{
			outmat.resize(1, 1);
			outmat(0, 0) = sum();
			return outmat;
		}

		outmat.resize(1, mCols);

		for (unsigned icol = 0; icol < mCols; icol++)
		{
			TL::Vector<T> vCol = col(icol);
			outmat(0, icol) = vCol.sum_value();
		}
		return outmat;
	}


	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C> MMatrixSheet<T, Property, A, C>::prod() const{
		MMatrixSheet outmat;
		if (mRows == 1 || mCols == 1)
		{
			outmat.resize(1, 1);
			T outVal;
			for (int i = 0; i < size(); i++)
			{
				if (i == 0)
				{
					outVal = (*this)(i);
				}
				else
				{
					outVal *= (*this)(i);
				}
			}
			outmat(0, 0) = outVal;
		}
		else
		{
			outmat.resize(1, mCols);
			for (unsigned icol = 0; icol < mCols; icol++)
			{
				T outVal;
				for (unsigned irow = 0; irow < mRows; irow++)
				{
					if (irow == 0)
					{
						outVal = (*this)(irow, icol);
					}
					else
					{
						outVal *= (*this)(irow, icol);
					}
				}
				outmat(0, icol) = outVal;
			}
		}
		return outmat;
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class U, class Property2, class A2>
	MMatrixSheet<U, Property2, A2, C> MMatrixSheet<T, Property, A, C>::imag() const
	{
		MMatrixSheet<U, Property2, A2, C> outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				const T& val = (*this)(i, j);
				outmat(i, j) = val.imag();
			}
		}
		return outmat;
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class U, class Property2, class A2>
	MMatrixSheet<U, Property2, A2, C>& MMatrixSheet<T, Property, A, C>::imag(MMatrixSheet<U, Property2, A2, C>& out) const
	{
		out.clear();
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				const T& val = (*this)(i, j);
				out(i, j) = val.imag();
			}
		}
		return out;
	}


	template<class T, class Property, class A, template<class, class>class C>
	template<class U, class Property2, class A2>
	void MMatrixSheet<T, Property, A, C>::set_imag(const MMatrixSheet<U, Property2, A2, C>& imagmat)
	{
		if (imagmat.cols() != mCols || imagmat.rows() != mRows)
			return;
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				T& val = (*this)(i, j);
				val.imag(imagmat(i, j));
			}
		}
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class U, class Property2, class A2>
	MMatrixSheet<U, Property2, A2, C> MMatrixSheet<T, Property, A, C>::real() const
	{
		MMatrixSheet<U, Property2, A2, C> outmat;
		outmat.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				const T& val = (*this)(i, j);
				outmat(i, j) = val.real();
			}
		}
		return outmat;
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class U, class Property2, class A2>
	MMatrixSheet<U, Property2, A2, C>& MMatrixSheet<T, Property, A, C>::real(MMatrixSheet<U, Property2, A2, C>& out) const
	{
		out.clear();
		out.resize(mRows, mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				const T& val = (*this)(i, j);
				out(i, j) = val.real();
			}
		}
		return out;
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class U, class Property2, class A2>
	void MMatrixSheet<T, Property, A, C>::set_real(const MMatrixSheet<U, Property2, A2, C>& realmat)
	{
		if (realmat.cols() != mCols || realmat.rows() != mRows)
			return;
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < mCols; ++j){
				T& val = (*this)(i, j);
				val.real(realmat(i, j));
			}
		}
	}

	//
	// --------------------------------------------------------------------------
	//

	template<class T, class Property, class A, template<class, class>class C, class F>
	MMatrixSheet<T, Property, A, C>  transform_matrix(
		const MMatrixSheet<T, Property, A, C>& left,
		const MMatrixSheet<T, Property, A, C>& right,
		F prd)
	{
		MMatrixSheet<T, Property, A, C> outmat;
		if (left.cols() != right.cols() || left.rows() != right.rows()){
			return outmat;
		}
		outmat.resize(left.rows(), left.cols());
		for (unsigned i = 0; i < left.rows(); ++i){
			for (unsigned j = 0; j < left.cols(); ++j){
				outmat(i, j) = prd(left(i, j), right(i, j));
			}
		}
		return outmat;
	}

	//
	// 预定义几个类型方便使用
	//
	typedef MMatrixSheet<int> imat;
	typedef MMatrixSheet<double> dmat;
	typedef MMatrixSheet<float> fmat;
	typedef MMatrixSheet<std::complex<int>> cx_imat;
	typedef MMatrixSheet<std::complex<double>> cx_dmat;
	typedef MMatrixSheet<std::complex<float>> cx_fmat;
	typedef MMatrixSheet<Complex<double>> mcx_dmat;  // 可以实现快速计算
	typedef MMatrixSheet<Complex<float>>  mcx_fmat;  // 可以实现快速计算
	typedef MMatrixSheet<Complex<int>>  mcx_imat;  // 可以实现快速计算
	typedef std::complex<double> cv_dvalue;
	typedef std::complex<float> cv_fvalue;
	typedef std::complex<int> cv_ivalue;
	typedef Complex<double> mcv_dvalue;
	typedef Complex<float> mcv_fvalue;
	typedef Complex<int> mcv_ivalue;
}// namespace mj





//+------------------------------------------------------------
//
//
// 封装几个常用函数
// 主要还是为了更方便复数计算
//
//
//+-------------------------------------------------------------
namespace mjmat{


	//
	// 计算绝对值
	// 对于复数来说绝对值就是模长
	//
	template<class T>
	static inline mj::MMatrixSheet<T> abs(const mj::MMatrixSheet<T>& mat){
		mj::MMatrixSheet<T> outmat;
		outmat.resize(mat.rows(), mat.cols());
		for (int i = 0; i < mat.rows(); ++i){
			for (int j = 0; j < mat.cols(); ++j){
				outmat(i, j) = std::abs(mat(i, j));
			}
		}
		return outmat;
	}

	
	template<class T>
	static inline mj::MMatrixSheet<T> abs(const mj::MMatrixSheet<std::complex<T>>& mat){
		mj::MMatrixSheet<T> outmat;
		outmat.resize(mat.rows(), mat.cols());
		for (int i = 0; i < mat.rows(); ++i){
			for (int j = 0; j < mat.cols(); ++j){
				outmat(i, j) = std::abs(mat(i, j));
			}
		}
		return outmat;
	}

	

	//
	// 对一个矩阵进行开根号
	// 返回复矩阵
	// 如果确定只需要实数那么获取实部即可
	//
	template<class T>
	static inline mj::MMatrixSheet<std::complex<T>>  sqrt(const mj::MMatrixSheet<T>& mat){
		mj::MMatrixSheet<std::complex<T>> outmat;
		outmat.resize(mat.rows(), mat.cols());
		for (int i = 0; i < mat.rows(); ++i){
			for (int j = 0; j < mat.cols(); ++j){
				outmat(i, j) = std::sqrt(std::complex<T>(mat(i, j)));
			}
		}
		return outmat;
	}

	template<class T>
	static inline mj::MMatrixSheet<std::complex<T>>  sqrt(const mj::MMatrixSheet<std::complex<T>>& mat){
		mj::MMatrixSheet<std::complex<T>> outmat;
		outmat.resize(mat.rows(), mat.cols());
		for (int i = 0; i < mat.rows(); ++i){
			for (int j = 0; j < mat.cols(); ++j){
				outmat(i, j) = std::sqrt(mat(i, j));
			}
		}
		return outmat;
	}


	//
	// 矩阵平方
	//
	template<class T>
	static inline mj::MMatrixSheet<T> square(const mj::MMatrixSheet<T>& mat){
		mj::MMatrixSheet<T> outmat;
		outmat.resize(mat.rows(), mat.cols());
		for (int i = 0; i < mat.rows(); ++i){
			for (int j = 0; j < mat.cols(); ++j){
				outmat(i, j) = std::pow(mat(i, j),2);
			}
		}
		return outmat;
	}


	//
	// 将虚部置为某数
	//
	static inline void set_image(mj::cx_dmat& mat, double val){
		for (int i = 0; i < mat.rows(); ++i){
			for (int j = 0; j < mat.cols(); ++j){
				mat(i, j).imag(val);
			}
		}
	}


	//
	// 将实部置为某数
	//
	static inline void set_real(mj::cx_dmat& mat, double val){
		for (int i = 0; i < mat.rows(); ++i){
			for (int j = 0; j < mat.cols(); ++j){
				mat(i, j).real(val);
			}
		}
	}


	//
	// 使用指定值填充矩阵
	//
	template<class T>
	static inline mj::MMatrixSheet<T> fill(int row, int col, const T& val){
		mj::MMatrixSheet<T> mat;
		mat.resize(row, col);
		mat.fill(val);
		return mat;
	}



	static inline mj::dmat one(int row, int col){
		mj::dmat mat;
		mat.resize(row, col);
		mat.fill(1);
		return mat;
	}

	static inline mj::dmat zero(int row, int col){
		mj::dmat mat;
		mat.resize(row, col);
		mat.fill(0);
		return mat;
	}

	//
	// 使用随机数填充矩阵
	//
	static inline mj::dmat rand(int row, int col,double minval = -1.0,double maxval = 1.0){
		mj::dmat mat;
		mat.resize(row, col);
		static mj::GenRandom<double> gen(minval, maxval);
		for (int i = 0; i < mat.rows(); ++i){
			for (int j = 0; j < mat.cols(); ++j){
				mat(i, j) = gen();
			}
		}
		return mat;
	}

	//
	// 泛型的操作
	//
	template<class T>
	static inline mj::MMatrixSheet<T> one(int row, int col){
		mj::MMatrixSheet<T> mat;
		mat.resize(row, col);
		mat.fill(1);
		return mat;
	}


	template<class T>
	static inline mj::MMatrixSheet<T> zero(int row, int col){
		mj::MMatrixSheet<T> mat;
		mat.resize(row, col);
		mat.fill(0);
		return mat;
	}

	template<class T>
	static inline mj::MMatrixSheet<T> rand(int row, int col, double minval = -1.0, double maxval = 1.0){
		mj::MMatrixSheet<T> mat;
		mat.resize(row, col);
		static mj::GenRandom<T> gen(minval, maxval);
		for (int i = 0; i < mat.rows(); ++i){
			for (int j = 0; j < mat.cols(); ++j){
				mat(i, j) = gen();
			}
		}
		return mat;
	}


	//
	// 由幅值相位转换到实部虚部
	//
	template<class T>
	static inline mj::MMatrixSheet<std::complex<T>> amph_to_complex(const mj::MMatrixSheet<std::complex<T>>& m,double base = 20.0){
		mj::MMatrixSheet<std::complex<T>> mat;
		mat.resize(m.rows(), m.cols());
		mj::MMatrixSheet<T> __abs = m.real();
		__abs.transform([=](T val){
			return std::pow(10.0, val / base);
		});
		mj::MMatrixSheet<T> __arg = m.imag();
		for (int i = 0; i < mat.rows(); ++i){
			for (int j = 0; j < mat.cols(); ++j){
				mat(i, j) = std::complex<double>(__abs(i, j)*std::cos(__arg(i, j) / 180.0*mj::PI), __abs(i, j)*std::sin(__arg(i, j) / 180.0*mj::PI));
			}
		}
		return mat;
	}


	//
	// 由实部虚部转换到幅度相位
	//
	template<class T>
	static inline mj::MMatrixSheet<std::complex<T>> complex_to_amph(const mj::MMatrixSheet<std::complex<T>>& m,double base = 20.0){
		mj::MMatrixSheet<std::complex<T>> mat;
		mat.resize(m.rows(), m.cols());
		for (int i = 0; i < mat.rows(); ++i){
			for (int j = 0; j < mat.cols(); ++j){
				mat(i, j) = std::complex<double>(base*std::log10(std::abs(m(i, j))), std::arg(m(i, j)/mj::PI*180.0));
			}
		}
		return mat;
	}



	//
	// dim == 0 查找每列的最小值
	// dim == 1 查找每行的最小值
	// 大于等于2查找最小值
	//
	template<class T>
	static inline mj::MMatrixSheet<T> min_ele(const mj::MMatrixSheet<T>& m, int dim = 0){
		mj::MMatrixSheet<T> res;
		if (dim == 0){
			res.resize(1, m.cols());
			int rows = m.rows();
			for (int i = 0; i < m.cols(); ++i){
				auto it = m.reference_obj().begin();
				res(0, i) = *(std::min_element(it + i*rows, it + (i + 1)*rows));
			}
		}
		else if (dim == 1){
			res.resize(m.rows(), 1);
			for (int i = 0; i < m.rows(); ++i){
				std::vector<T> rowdata = m.row(i);
				res(i, 0) = *(std::min_element(rowdata.begin(), rowdata.end()));
			}
		}
		else if (dim >= 2){
			res.resize(1, 1);
			res(0, 0) = *(std::min_element(m.reference_obj().begin(), m.reference_obj().end()));
		}
		return res;
	}


	//
	// dim == 0 查找每列的最大值
	// dim == 1 查找每行的最大值
	// 大于等于2查找最大值
	//
	template<class T>
	static inline mj::MMatrixSheet<T> max_ele(const mj::MMatrixSheet<T>& m, int dim = 0){
		mj::MMatrixSheet<T> res;
		if (dim == 0){
			res.resize(1, m.cols());
			int rows = m.rows();
			for (int i = 0; i < m.cols(); ++i){
				auto it = m.reference_obj().begin();
				res(0, i) = *(std::max_element(it + i*rows, it + (i + 1)*rows));
			}
		}
		else if (dim == 1){
			res.resize(m.rows(), 1);
			for (int i = 0; i < m.rows(); ++i){
				std::vector<T> rowdata = m.row(i);
				res(i, 0) = *(std::max_element(rowdata.begin(), rowdata.end()));
			}
		}
		else if (dim >= 2){
			res.resize(1, 1);
			res(0, 0) = *(std::max_element(m.reference_obj().begin(), m.reference_obj().end()));
		}
		return res;
	}


	//
	// dim == 0 计算每列的总和
	// dim == 1 计算每行的总和
	// 大于等于2计算所有数据
	//
	template<class T>
	static inline mj::MMatrixSheet<T> sum(const mj::MMatrixSheet<T>& m, int dim = 0){
		mj::MMatrixSheet<T> res;
		if (dim == 0){
			res.resize(1, m.cols());
			int rows = m.rows();
			for (int i = 0; i < m.cols(); ++i){	
				auto it = m.reference_obj().begin();
				res(0, i) = std::accumulate(it + i*rows, it + (i + 1)*rows, T(0.0));
			}
		}
		else if (dim == 1){
			res.resize(m.rows(), 1);
			for (int i = 0; i < m.rows(); ++i){
				std::vector<T> rowdata = m.row(i);
				res(i, 0) = std::accumulate(rowdata.begin(), rowdata.end(), T(0.0));
			}
		}
		else if (dim >= 2){
			res.resize(1, 1);
			res(0, 0) = std::accumulate(m.reference_obj().begin(), m.reference_obj().end(), T(0.0));
		}
		return res;
	}


	//
	// dim == 0 计算每列的积分
	// dim == 1 计算每行的积分
	// 大于等于2全积分
	//
	template<class T>
	static inline mj::MMatrixSheet<T> trapz(const mj::MMatrixSheet<T>& m, int dim = 0){
		mj::MMatrixSheet<T> res;
		if (dim == 0){
			int N = m.rows();
			if (m.rows() <= 1){
				res.resize(1, N);
				return res;
			}
			mj::MMatrixSheet<T> up = m.rows(0, N - 1);
			mj::MMatrixSheet<T> down = m.rows(1, N);
			res = sum((0.5 * (up + down)), 0);
		}
		else if (dim == 1){
			int N = m.cols();
			if (m.cols() <= 1){
				res.resize(N, 1);
				return res;
			}
			mj::MMatrixSheet<T> left = m.cols(0, N - 1);
			mj::MMatrixSheet<T> right = m.cols(1, N);
			res = sum((0.5 * (left + right)), 1);
		}
		else if (dim >= 2){
			return trapz(trapz(m), 1);
		}
		return res;
	}

	//
	// 傅里叶矩阵
	//
	template<class T>
	static inline mj::MMatrixSheet<mj::Complex<T>> fftm(int N,double sig = -1){
		mj::MMatrixSheet<mj::Complex<T>> mat;
		mat.resize(N, N);
		double theta = 2 * mj::PI / N;
		try{
			throw "Error";
			concurrency::array_view<mj::Complex<T>, 2> vcols(mat.rows(), mat.cols(), mat.reference_obj());
			concurrency::parallel_for_each(vcols.extent, [=](concurrency::index<2> idx) restrict(amp){
				unsigned r = idx[0];
				unsigned c = idx[1];
				double angle = c*r*theta;
				vcols(c, r)._real = concurrency::fast_math::cos(angle);
				vcols(c, r)._imag = concurrency::fast_math::sin(angle)*sig;
			});
			vcols.synchronize();
			vcols(0, 0);
		}
		catch (...){
			for (int i = 0; i < N; ++i){
				for (int j = 0; j < N; ++j){
					double angle = i*j*theta;
					mat(i, j)._real = std::cos(angle);
					mat(i, j)._imag = std::sin(angle)*sig;
				}
			}
		}
		return mat;
	}

	//
	// 傅里叶逆矩阵
	//
	template<class T>
	static inline mj::MMatrixSheet<mj::Complex<T>> ifftm(int N){
		mj::MMatrixSheet<mj::Complex<T>> mat;
		mat.resize(N, N);
		double theta = 2 * mj::PI / N;
		try{
			throw "Error";
			concurrency::array_view<mj::Complex<T>, 2> vcols(mat.rows(), mat.cols(), mat.reference_obj());
			concurrency::parallel_for_each(vcols.extent, [=](concurrency::index<2> idx) restrict(amp){
				unsigned r = idx[0];
				unsigned c = idx[1];
				double angle = c*r*theta;
				vcols(c, r)._real = concurrency::fast_math::cos(angle) / N;
				vcols(c, r)._imag = concurrency::fast_math::sin(angle) / N;
			});
			vcols.synchronize();
			vcols(0, 0);
		}
		catch (...){
			for (int i = 0; i < N; ++i){
				for (int j = 0; j < N; ++j){
					double angle = i*j*theta;
					mat(i, j)._real = std::cos(angle) / N;
					mat(i, j)._imag = std::sin(angle) / N;
				}
			}
		}
		
		return mat;
	}


	//
	// 傅里叶变换
	// N 行数
	// M 列数
	//
	template<class T>
	static inline std::vector<mj::Complex<T>> fft(const std::vector<mj::Complex<T>>& vec, int N){
		mj::MMatrixSheet<mj::Complex<T>> fm = fftm<T>(N);
		mj::MMatrixSheet<mj::Complex<T>> tempm;
		tempm.resize(N, 1);
		for (int i = 0; i < vec.size(); ++i){
			tempm(i, 0) = vec.at(i);
		}
		tempm = fm%tempm;
		return tempm.dettach();
	}

	//
	// 一维傅里叶矩阵变换
	// N = 1 每列进行变换
	// N = 2 每行进行变换
	//
	template<class T>
	static inline mj::MMatrixSheet<mj::Complex<T>> fft(const mj::MMatrixSheet<mj::Complex<T>>& m, int N = 1){
		mj::MMatrixSheet<mj::Complex<T>> tempm = m;
		if (N == 1){
			mj::MMatrixSheet<mj::Complex<T>> fm = fftm<T>(tempm.rows());
			tempm = fm%tempm;
		}
		else{
			tempm = tempm.t();
			mj::MMatrixSheet<mj::Complex<T>> fm = fftm<T>(tempm.rows());
			tempm = fm%tempm;
			tempm = tempm.t();
		}
		return tempm;
	}

	template<class T>
	static inline mj::MMatrixSheet<mj::Complex<T>> fft(const mj::MMatrixSheet<T>& m, int N = 1){
		return fft(mj::MMatrixSheet<mj::Complex<T>>(m), N);
	}

	template<class T>
	static inline mj::MMatrixSheet<mj::Complex<T>> fft(const mj::MMatrixSheet<std::complex<T>>& m, int N = 1){
		return fft(mj::MMatrixSheet<mj::Complex<T>>(m), N);
	}


	//
	// 二维傅里叶变换
	// N = 1 每行进行变换
	// M = 1 每列进行变换
	//
	template<class T>
	static inline mj::MMatrixSheet<mj::Complex<T>> fft2(const mj::MMatrixSheet<mj::Complex<T>>& m, int N = -1, int M = -1){
		if (N == -1){
			N = m.rows();
		}

		if (M == -1){
			M = m.cols();
		}
		mj::MMatrixSheet<mj::Complex<T>> fm = fftm<T>(M);
		mj::MMatrixSheet<mj::Complex<T>> tempm = m;
		
		if (N == 1){
			//
			// 行变换
			//
			if (tempm.cols() != M){
				tempm.resize(tempm.rows(), M);
			}
			tempm = tempm.t();
			tempm = fm%tempm;
			return tempm.t();
		}
		else if (M == 1){
			//
			// 列变换
			//
			fm = fftm<T>(N);
			if (N != tempm.rows()){
				tempm.resize(N, tempm.cols());
			}

			tempm = fm%tempm;
			return tempm;
		}
		
		//
		// 全变换
		//
		tempm.resize(N, M);
		tempm = tempm.t();
		tempm = fm%tempm;
		tempm = tempm.t();
		fm = fftm<T>(N);
		tempm = fm%tempm;
		return tempm;
	}


	template<class T>
	static inline mj::MMatrixSheet<mj::Complex<T>> fft2(const mj::MMatrixSheet<T>& m, int N = -1, int M = -1){
		return fft2(mj::MMatrixSheet<mj::Complex<T>>(m), N, M);
	}

	template<class T>
	static inline mj::MMatrixSheet<mj::Complex<T>> fft2(const mj::MMatrixSheet<std::complex<T>>& m, int N = -1, int M = -1){
		return fft2(mj::MMatrixSheet<mj::Complex<T>>(m), N, M);
	}

	template<class T>
	static inline std::vector<mj::Complex<T>> ifft(const std::vector<mj::Complex<T>>& vec, int N){
		mj::MMatrixSheet<mj::Complex<T>> fm = ifftm<T>(N);
		mj::MMatrixSheet<mj::Complex<T>> tempm;
		tempm.resize(N, 1);
		for (int i = 0; i < vec.size(); ++i){
			tempm(i, 0) = vec.at(i);
		}
		tempm = fm%tempm;
		return tempm.dettach();
	}

	//
	// 一维傅里叶矩阵变换
	// N = 1 每列进行变换
	// N = 2 每行进行变换
	//
	template<class T>
	static inline mj::MMatrixSheet<mj::Complex<T>> ifft(const mj::MMatrixSheet<mj::Complex<T>>& m, int N = 1){
		mj::MMatrixSheet<mj::Complex<T>> tempm = m;
		if (N == 1){
			mj::MMatrixSheet<mj::Complex<T>> fm = ifftm<T>(tempm.rows());
			tempm = fm%tempm;
		}
		else{
			tempm = tempm.t();
			mj::MMatrixSheet<mj::Complex<T>> fm = ifftm<T>(tempm.rows());
			tempm = fm%tempm;
			tempm = tempm.t();
		}
		return tempm;
	}

	template<class T>
	static inline mj::MMatrixSheet<mj::Complex<T>> ifft(const mj::MMatrixSheet<T>& m, int N = 1){
		return ifft(mj::MMatrixSheet<mj::Complex<T>>(m), N);
	}

	template<class T>
	static inline mj::MMatrixSheet<mj::Complex<T>> ifft(const mj::MMatrixSheet<std::complex<T>>& m, int N = 1){
		return ifft(mj::MMatrixSheet<mj::Complex<T>>(m), N);
	}


	//
	// 二维傅里叶变换
	// N = 1 每行进行变换
	// M = 1 每列进行变换
	//
	template<class T>
	static inline mj::MMatrixSheet<mj::Complex<T>> ifft2(const mj::MMatrixSheet<mj::Complex<T>>& m, int N = -1, int M = -1){
		if (N == -1){
			N = m.rows();
		}

		if (M == -1){
			M = m.cols();
		}

		mj::MMatrixSheet<mj::Complex<T>> fm = ifftm<T>(M);
		mj::MMatrixSheet<mj::Complex<T>> tempm = m;
		
		if (N == 1){
			//
			// 行变换
			//
			if (M != tempm.cols()){
				tempm.resize(tempm.rows(), M);
			}
			tempm = tempm.t();
			tempm = fm%tempm;
			return tempm.t();
		}
		else if (M == 1){
			//
			// 列变换
			//
			if (N != tempm.rows()){
				tempm.resize(N, tempm.cols());
			}
			fm = ifftm<T>(N);
			tempm = fm%tempm;
			return tempm;
		}

		//
		// 全变换
		//
		tempm.resize(N, M);
		tempm = tempm.t();
		tempm = fm%tempm;
		tempm = tempm.t();
		fm = ifftm<T>(N);
		tempm = fm%tempm;
		return tempm;
	}

	template<class T>
	static inline mj::MMatrixSheet<mj::Complex<T>> ifft2(const mj::MMatrixSheet<T>& m, int N = -1, int M = -1){
		return ifft2(mj::MMatrixSheet<mj::Complex<T>>(m), N, M);
	}

	template<class T>
	static inline mj::MMatrixSheet<mj::Complex<T>> ifft2(const mj::MMatrixSheet<std::complex<T>>& m, int N = -1, int M = -1){
		return ifft2(mj::MMatrixSheet<mj::Complex<T>>(m), N, M);
	}



	//
	// e(iθ)
	// sign 表示正负号
	// e(iθ) = cos(θ) + sign*sin(θ)
	// 针对矩阵
	//
	static inline mj::cx_dmat exp(const mj::dmat& mat,int sign = 1){
		mj::cx_dmat outmat;
		outmat.resize(mat.rows(), mat.cols());
		for (int i = 0; i < mat.rows(); ++i){
			for (int j = 0; j < mat.cols(); ++j){
				double val = mat(i, j);
				std::complex<double> cp(std::cos(val), sign*std::sin(val));
				outmat(i, j) = cp;
			}
		}
		return outmat;
	}


	static inline mj::cx_dmat exp(const mj::cx_dmat& mat, int sign = 1){
		mj::cx_dmat outmat;
		outmat.resize(mat.rows(), mat.cols());
		for (int i = 0; i < mat.rows(); ++i){
			for (int j = 0; j < mat.cols(); ++j){
				std::complex<double> val = mat(i, j);
				std::complex<double> ex(std::cos(val.real()), sign*std::sin(val.real()));
				std::complex<double> cp(std::cos(val.imag()), sign*std::sin(val.imag()));
				outmat(i, j) = ex*cp;
			}
		}
		return outmat;
	}


	//
	// 针对向量
	//
	static inline std::vector<std::complex<double>> exp(const std::vector<double>& mat, int sign = 1){
		std::vector<std::complex<double>> outmat;		
		for (int j = 0; j < mat.size(); ++j){
			double val = mat.at(j);
			std::complex<double> cp(std::cos(val), sign*std::sin(val));
			outmat.push_back(cp);
		}
		return outmat;
	}

	//
	// 针对某个值
	//
	static inline std::complex<double> exp(double val, int sign = 1){
		std::complex<double> cp(std::cos(val), sign*std::sin(val));
		return cp;
	}


	//
	// 参数为角度
	// 
	static inline double sind(double angle){
		return std::sin(angle / 180.0*mj::PI);
	}

	static inline double cosd(double angle){
		return std::cos(angle / 180.0*mj::PI);
	}
}




//+------------------------------------------------------
//
//
// 针对复数进行大小比较特化
// 原本标准库不支持该操作
// 之所以要这么干主要有时候会用到数据比较
// 当然主要是判断两组数据是否相等
// cx_dmat m1,m2;
// ...
// cx_dmat m3 = m1 - m2;
// auto minval = m3.min_value();
// auto maxval = m3.max_value();
// bool iseq = minval == maxval;
//
//
//+---------------------------------------------------------
namespace std{

	template<class T>
	static inline bool operator<(const std::complex<T>& left, const std::complex<T>& right){
		if (left.real() >  right.real()){
			return false;
		}
		else if (left.real() >= right.real() && left.real() <= right.real()){
			return left.imag() < right.imag();
		}
		else{
			return true;
		}
	}

	template<class T>
	static inline bool operator>(const std::complex<T>& left, const std::complex<T>& right){
		if (left.real() <  right.real()){
			return false;
		}
		else if (left.real() >= right.real() && left.real() <= right.real()){
			return left.imag() > right.imag();
		}
		else{
			return false;
		}
	}

	template<class T>
	struct less<std::complex<T>>
		: public binary_function<std::complex<T>, std::complex<T>, bool>
	{	
		bool operator()(const std::complex<T>& _Left, const std::complex<T>& _Right) const
		{	
			return (_Left < _Right);
		}
	};

	template<class T>
	struct greater<std::complex<T>>
		: public binary_function<std::complex<T>, std::complex<T>, bool>
	{
		bool operator()(const std::complex<T>& _Left, const std::complex<T>& _Right) const
		{
			return (_Left > _Right);
		}
	};


	//
	//+----------------------------------------------------------------------------------------
	//
	template<class T>
	static inline bool operator<(const mj::Complex<T>& left, const mj::Complex<T>& right){
		if (left.real() >  right.real()){
			return false;
		}
		else if (left.real() >= right.real() && left.real() <= right.real()){
			return left.imag() < right.imag();
		}
		else{
			return true;
		}
	}

	template<class T>
	static inline bool operator>(const mj::Complex<T>& left, const mj::Complex<T>& right){
		if (left.real() <  right.real()){
			return false;
		}
		else if (left.real() >= right.real() && left.real() <= right.real()){
			return left.imag() > right.imag();
		}
		else{
			return false;
		}
	}

	template<class T>
	struct less<mj::Complex<T>>
		: public binary_function<mj::Complex<T>, mj::Complex<T>, bool>
	{
		bool operator()(const mj::Complex<T>& _Left, const mj::Complex<T>& _Right) const
		{
			return (_Left < _Right);
		}
	};

	template<class T>
	struct greater<mj::Complex<T>>
		: public binary_function<mj::Complex<T>, mj::Complex<T>, bool>
	{
		bool operator()(const mj::Complex<T>& _Left, const mj::Complex<T>& _Right) const
		{
			return (_Left > _Right);
		}
	};
}