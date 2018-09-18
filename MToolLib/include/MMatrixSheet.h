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
#ifdef __ARMADILLO__
#include <armadillo>
#endif

//
// 可以带附加属性的Sheet
// 内存模型为例主序
// 使用列操作的时候速度会比较快
//
namespace mj{
	template<class T, class Property = MAnyHolder, class A = std::allocator<T>, template<class, class>class C = std::vector>
	class MMatrixSheet{
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
		public:
			template<class T1, class P1, class A1, template<class, class>class C2>
			friend class MMatrixSheet;

			ArrayRef(unsigned num) :mNum(num){ mData.resize(mNum); }
			ArrayRef(const ArrayRef& other) :mNum(other.mNum), mData(other.mData){}
			ArrayRef(ArrayRef&& other) :mNum(other.mNum), mData(other.mData){ other.mNum = 0; other.mData.clear(); }
			ArrayRef& operator=(const ArrayRef& other){ mNum = other.mNum; mData = other.mData; return *this; }
			ArrayRef& operator=(ArrayRef&& other){ mNum = other.mNum; mData = other.mData; other.mNum = 0; other.mData.clear(); return *this; }
			ArrayRef(const metadata_type& other){ assign_value(other); }
			ArrayRef& operator=(const metadata_type& other){ assign_value(other); return *this; }
			T& operator()(unsigned index){ return *(mData[index]); }
			const T& operator()(unsigned index) const{ return *(mData[index]); }
			T& operator[](unsigned index){ return *(mData[index]); }
			const T& operator[](unsigned index) const{ return *(mData[index]); }
			~ArrayRef(){ mData.clear(); mNum = 0; }
			operator metadata_type(){ return tovalue(); }
			operator metadata_type() const{ return const_cast<ArrayRef*>(this)->tovalue(); }

		private:
			void setData(unsigned index, T* valptr){ if (index<mNum)mData[index] = valptr; }
			void assign_value(const metadata_type& other){
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
		public:
			template<class T1, class P1, class A1, template<class, class>class C2>
			friend class MMatrixSheet;

			MatrixRef(unsigned rownum, unsigned colnum){ mData.resize(rownum, colnum); }
			MatrixRef(const MatrixRef& other) :mData(other.mData){}
			MatrixRef(MatrixRef&& other) :mData(other.mData){ other.mData.reset(); }
			MatrixRef& operator=(const MatrixRef& other){ mData = other.mData; return *this; }
			MatrixRef& operator=(MatrixRef&& other){ mData = other.mData; other.mData.reset(); return *this; }
			MatrixRef(const MMatrixSheet& other){ assign_value(other); }
			MatrixRef& operator=(const MMatrixSheet& other){ assign_value(other); return *this; }
			~MatrixRef(){ mData.clear(); }
			T& operator()(unsigned row, unsigned col){ reutrn *(mData(row, col)); }
			const T& operator()(unsigned row, unsigned col) const{ return *(mData(row, col)); }
			operator MMatrixSheet(){ return tovalue(); }
			operator MMatrixSheet() const{ return tovalue(); }

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


		T& at(unsigned rowindex, unsigned colindex);
		const T& at(unsigned rowindex, unsigned colindex) const;

		//
		// 针对不能够返回 应用的 类型提供直接操作的方式
		//
		void set(unsigned rowindex, unsigned colindex, const_reference_type val);
		const_reference_type get(unsigned rowindex, unsigned colindex) const;

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
		metadata_type operator*=(const metadata_type& colvec); // 列向量

		//
		// 计算行列式
		//
		T det() const;


		//
		// 转置,不添加求逆矩阵操作，求逆矩阵的操作太过复杂
		//
		MMatrixSheet t() const;

		//
		// 操作
		//
		template<class F>
		MMatrixSheet transform_copy(F fun) const;

		template<class F>
		MMatrixSheet& transform(F fun);

		//
		// 反转
		//
		MMatrixSheet& reverse(bool iscol = true);
		MMatrixSheet reverse(bool iscol = true) const;

		//
		// 切起数据
		//
		MMatrixSheet slice(unsigned rowstep,unsigned colstep);
		MMatrixSheet slice(unsigned rowstep, unsigned colstep) const;


		//
		// 调整数据
		// 例如将数据调整到+-180之间
		// adjust(360,-180,180)
		//
		template<class Less = std::less<T>, class Greater = std::greater<T>, class Equal_to = std::equal_to<T>>
		void adjust(const_reference_type adjustvalue, const_reference_type __minvalue,
			const_reference_type __maxvalue, const_reference_type excepty,
			Less __lessfun = Less(),
			Greater __greaterfun = Greater(),
			Equal_to __equalfun = Equal_to());

		//
		// 替换
		//
		template<class cmp = std::equal_to<T>>
		MMatrixSheet& replace(
			typename MConstParamWrape<T>::type oldval,
			typename MConstParamWrape<T>::type newval,
			cmp prd = cmp());

		//
		// 检查包含
		//
		template<class cmp = std::equal_to<T>>
		bool contains(const_reference_type val, cmp prd = cmp());

		template<class cmp = std::equal_to<T>>
		bool index(const_reference_type val, unsigned& rowindex, unsigned& colindex, cmp prd = cmp());

		//
		// 搜索满足条件的值
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
		double aval() const;  // 平均值
		double standardiff() const; // 标准差
		double squaldiff() const; // 方差
		double standardiff_if(double avl) const; // 给定均值求标准差
		double squaldiff_if(double avl) const; // 给定均值求方差

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
		double aval(const_reference_type excepty, cmp prd = cmp()) const;

		template<class cmp = std::equal_to<T>>
		double standardiff(const_reference_type excepty, cmp prd = cmp()) const;

		template<class cmp = std::equal_to<T>>
		double squaldiff(const_reference_type excepty, cmp prd = cmp()) const; // 方差

		template<class cmp = std::equal_to<T>>
		double standardiff_if(double avl, const_reference_type excepty, cmp prd = cmp()) const;

		template<class cmp = std::equal_to<T>>
		double squaldiff_if(double avl, const_reference_type excepty, cmp prd = cmp()) const; // 方差

		//
		// 打印
		//
		void print(const char* msg = "", std::ostream& os = std::cout);
		void load(const char* filename, bool isbinary = true);
		void save(const char* filename, bool isbinary = true);

		//
		// 使用流操作
		//
		void load(std::istream& is, bool isbinary = true);
		__int64 save(std::ostream& is, bool isbinary = true);

		void load(std::istream& is, char spliter);
		void save(std::ostream& os, char spliter);

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

		//
		// 如果T是复数便可以使用下面的函数
		//
		template<class U = typename T::value_type, class A2 = std::allocator<U>>
		MMatrixSheet<U, Property, A2, C> atan2() const; // 针对复数才有用
		template<class U = typename T::value_type, class A2 = std::allocator<U>>
		MMatrixSheet<U, Property, A2, C> arg() const;  // 针对复数才有用
		MMatrixSheet conj() const; // 共轭复数

		template<class U = typename T::value_type, class Property2 = Property, class A2 = std::allocator<U>>
		MMatrixSheet<U, Property2, A2, C> imag() const;

		template<class U, class Property2 = Property, class A2 = std::allocator<U>>
		MMatrixSheet<U, Property2, A2, C>& imag(MMatrixSheet<U, Property2, A2, C>& out) const;

		template<class U = typename T::value_type, class Property2 = Property, class A2 = std::allocator<U>>
		void set_imag(const MMatrixSheet<U, Property2, A2, C>& imagmat);

		template<class U = typename T::value_type, class Property2 = Property, class A2 = std::allocator<U>>
		MMatrixSheet<U, Property2, A2, C> real() const;

		template<class U, class Property2 = Property, class A2 = std::allocator<U>>
		MMatrixSheet<U, Property2, A2, C>& real(MMatrixSheet<U, Property2, A2, C>& out) const;

		template<class U = typename T::value_type, class Property2 = Property, class A2 = std::allocator<U>>
		void set_real(const MMatrixSheet<U, Property2, A2, C>& realmat);

		//
		// 友元函数
		//
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

		friend MMatrixSheet operator/(const MMatrixSheet& left, const_reference_type right){
			MMatrixSheet outmat = left;
			outmat /= value;
			return outmat;
		}

		friend std::ostream& operator<<(std::ostream& os, const MMatrixSheet& mat){
			mat.print("", os);
			return os;
		}
	};

	/******************************************下面是实现细节没兴趣可以不用关心*************************************************************************/
	/*-----------------------------------------------------------------------------------------------*/
	/*   实现
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
	/*-----------------------------------------------------------------------------------------------
	*
	*
	*
	*
	*/
	/*---------------------------------------------华丽的分割线---------------------------------------*/

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

#ifdef __ARMADILLO__
	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>::MMatrixSheet(const arma::Mat<T>& other){
		mCols = other.n_cols;
		mRows = other.n_rows;
		resize(mRows, mCols);
		memcpy(&mData[0], other.memptr(), sizeof(double)*mRows*mCols);
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::operator=(const arma::Mat<T>& other){
		this->mCols = other.n_cols;
		this->mRows = other.n_rows;
		resize(mRows, mCols);
		memcpy(&mData[0], other.memptr(), sizeof(double)*mRows*mCols);
		return *this;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>::operator arma::Mat<T>() const{
		const double* ptr = static_cast<const double*>(&mData[0]);
		arma::Mat<T> m(ptr, mRows, mCols);
		return m;
	}

	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>::operator arma::Mat<T>(){
		double* ptr = static_cast<double*>(&mData[0]);
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
	T& MMatrixSheet<T, Property, A, C>::at(unsigned rowindex, unsigned colindex){
		return mData.at(colindex*mRows + rowindex);
	}

	template<class T, class Property, class A, template<class, class>class C>
	const T& MMatrixSheet<T, Property, A, C>::at(unsigned rowindex, unsigned colindex) const{
		return mData.at(colindex*mRows + rowindex);
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
		outmat.resize(mRows, mCols);
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

	//
	// 数量乘
	//
	template<class T, class Property, class A, template<class, class>class C>
	MMatrixSheet<T, Property, A, C>& MMatrixSheet<T, Property, A, C>::multplie(const MMatrixSheet& other){
		if (mCols != other.mRows){
			return *this;
		}
		MMatrixSheet temp(mRows, other.mCols);
		for (unsigned i = 0; i < mRows; ++i){
			for (unsigned j = 0; j < other.mCols; ++j){
				temp(i, j) = 0;
				for (unsigned k = 0; k < mCols; ++k){
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

	//
	// 计算行列式
	//
	template<class T, class Property, class A, template<class, class>class C>
	T MMatrixSheet<T, Property, A, C>::det() const
	{
		if (mRows != mCols){
			throw std::logic_error("非方阵不可求行列式");
		}
		if (empty())
			return T();
		T detval = T();

		//
		// 先计算出+的部分
		//
		for (int k = 0; k < mCols; ++k){
			T tk = 1;
			int index = 0;
			for (int i = k; i < mCols; ++i){
				for (int j = index; j < mRows; ++j){
					tk *= (*this)(j,i);
					break;
				}
				++index;
			}

			for (int i = 0; i < k; ++i){
				for (int j = index; j < mRows; ++j){
					tk *= (*this)(j, i);
					break;
				}
				++index;
			}
			detval += tk;
		}
		//
		// 在计算出-的部分
		//
		for (unsigned k = 0; k < mRows; ++k){
			T tk = 1;
			int index = mCols;
			for (int i = k; i < mRows; ++i){
				for (int j = index; j > 0; --j){
					tk *= (*this)(i, j - 1);
					break;
				}
				--index;
			}

			for (int i = 0; i < k; ++i){
				for (int j = index; j >0; --j){
					tk *= (*this)(i, j-1);
					break;
				}
				--index;
			}
			detval -= tk;
		}
		return detval;
	}


	//
	// 转置
	//
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


	//
	// 操作
	//
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

	//
	// 反转
	//
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
		return std::accumulate(mData.begin(), mData.end(), 0);
	}

	template<class T, class Property, class A, template<class, class>class C>
	double MMatrixSheet<T, Property, A, C>::aval() const
	{
		if (mRows == 0 || mCols == 0)
			return T();
		return sum() / (mRows*mCols);
	}

	template<class T, class Property, class A, template<class, class>class C>
	double MMatrixSheet<T, Property, A, C>::standardiff() const
	{
		if (mRows == 0 || mCols == 0)
			return T();
		return  std::sqrt(squaldiff());
	}

	template<class T, class Property, class A, template<class, class>class C>
	double MMatrixSheet<T, Property, A, C>::squaldiff() const
	{
		if (mRows == 0 || mCols == 0)
			return T();
		double val = aval();
		double sum = std::accumulate(mData.begin(), mData.end(), 0, [&](double v1, T v){
			return v1 + (v - val)*(v - val);
		});
		return sum / (mRows*mCols);
	}

	template<class T, class Property, class A, template<class, class>class C>
	double MMatrixSheet<T, Property, A, C>::standardiff_if(double avl) const
	{
		if (mRows == 0 || mCols == 0)
			return T();
		return  std::sqrt(squaldiff_if(avl));
	}

	template<class T, class Property, class A, template<class, class>class C>
	double MMatrixSheet<T, Property, A, C>::squaldiff_if(double avl) const
	{
		if (mRows == 0 || mCols == 0)
			return T();
		double sum = std::accumulate(mData.begin(), mData.end(), 0, [&](double v1, T v){
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
		return std::accumulate(temp.begin(), temp.end(), 0);
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	double MMatrixSheet<T, Property, A, C>::aval(const_reference_type excepty, cmp prd) const
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
		return std::accumulate(temp.begin(), temp.end(), 0) / temp.size();
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	double MMatrixSheet<T, Property, A, C>::standardiff(const_reference_type excepty, cmp prd) const
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

		double val = aval(excepty, prd);
		double sum = std::accumulate(temp.begin(), temp.end(), 0, [&](double v1, T v){
			return v1 + (v - val)*(v - val);
		});
		return std::sqrt(sum / (temp.size()));
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	double MMatrixSheet<T, Property, A, C>::squaldiff(const_reference_type excepty, cmp prd) const
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

		double val = aval(excepty, prd);
		double sum = std::accumulate(temp.begin(), temp.end(), 0, [&](double v1, T v){
			return v1 + (v - val)*(v - val);
		});
		return sum / (temp.size());
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	double MMatrixSheet<T, Property, A, C>::standardiff_if(double avl, const_reference_type excepty, cmp prd) const
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

		double sum = std::accumulate(temp.begin(), temp.end(), 0, [&](double v1, T v){
			return v1 + (v - avl)*(v - avl);
		});
		return std::sqrt(sum / (temp.size()));
	}

	template<class T, class Property, class A, template<class, class>class C>
	template<class cmp>
	double MMatrixSheet<T, Property, A, C>::squaldiff_if(double avl, const_reference_type excepty, cmp prd) const
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

		double sum = std::accumulate(temp.begin(), temp.end(), 0, [&](double v1, T v){
			return v1 + (v - avl)*(v - avl);
		});
		return sum / (temp.size());
	}

	//
	// 打印
	//
	template<class T, class Property, class A, template<class, class>class C>
	void MMatrixSheet<T, Property, A, C>::print(const char* msg, std::ostream& os){
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
	void MMatrixSheet<T, Property, A, C>::save(const char* filename, bool isbinary)
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
	__int64 MMatrixSheet<T, Property, A, C> ::save(std::ostream& os, bool isbinary)
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
	void MMatrixSheet<T, Property, A, C>::save(std::ostream& os, char spliter){
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
}// namespace mj
