

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

//======================================
// MString 的迭代器
//======================================

#pragma once

namespace mj{

	template<bool,class T>
	struct __CType{
		typedef typename const T::type& type;
	};

	template<class T>
	struct __CType<false, T>{
		typedef typename T::type& type;
	};

	template<class T>
	class ConstMIterator{
	public:
        friend class MString;
		typedef typename T::const_type type;
		typedef typename T::iterator_category iterator_category;
		typedef typename T::value_type value_type;
		typedef typename T::difference_type difference_type;
		typedef typename T::distance_type distance_type;
		typedef typename T::pointer pointer;
		typedef typename T::reference reference;
		typedef typename type __Type;

		ConstMIterator(__Type& ptr, size_t num, size_t cnum = 0) :
			pData(ptr),
			mNum(num),
			mCurrentNum(cnum)
		{}

		ConstMIterator(const ConstMIterator& other) :
			pData(other.pData),
			mNum(other.mNum),
			mCurrentNum(other.mCurrentNum)
		{}

		ConstMIterator& operator=(const ConstMIterator& other){
			pData = other.pData;
			mNum = other.mNum;
			mCurrentNum = other.mCurrentNum;
			return *this;
		}

		const value_type* operator->(){
			if (mCurrentNum < 0 || mCurrentNum >= mNum){
				throw std::out_of_range("index out of range ..... ");
			}
			return &pData[mCurrentNum];
		}

		const value_type* operator->() const{
			if (mCurrentNum < 0 || mCurrentNum >= mNum){
				throw std::out_of_range("index out of range ..... ");
			}
			return &pData[mCurrentNum];
		}

		const reference operator*(){
			if (mCurrentNum < 0 || mCurrentNum >= mNum){
				throw std::out_of_range("index out of range ..... ");
			}
			return pData[mCurrentNum];
		}

		const reference operator*() const{
			if (mCurrentNum < 0 || mCurrentNum >= mNum){
				throw std::out_of_range("index out of range ..... ");
			}
			return pData[mCurrentNum];
		}

		ConstMIterator operator++(int){
			MIterator temp(*this);
			++mCurrentNum;
			return temp;
		}

		ConstMIterator& operator++(){
			++mCurrentNum;
			return *this;
		}

		ConstMIterator& operator+(size_t index){
			mCurrentNum += index;
			return *this;
		}

		ConstMIterator& operator-(size_t index){
			mCurrentNum -= index;
			return *this;
		}

		size_t pos(){
			return mCurrentNum;
		}

		friend size_t operator-(ConstMIterator first, ConstMIterator second){
			return first.mCurrentNum - second.mCurrentNum;
		}

		friend bool operator<(ConstMIterator first, ConstMIterator second){
			if (first.mNum != second.mNum ||
				first.pData != second.pData){
				throw std::runtime_error("Error ..... ");
			}
			return first.mCurrentNum < second.mCurrentNum;
		}

		friend bool operator>(ConstMIterator first, ConstMIterator second){
			if (first.mNum != second.mNum ||
				first.pData != second.pData){
				throw std::runtime_error("Error ..... ");
			}
			return first.mCurrentNum > second.mCurrentNum;
		}

		friend bool operator==(ConstMIterator first, ConstMIterator second){
			if (first.mNum != second.mNum ||
				first.pData != second.pData){
				throw std::runtime_error("Error ..... ");
			}
			return  first.mCurrentNum == second.mCurrentNum;
		}

		friend bool operator!=(ConstMIterator first, ConstMIterator second){
			if (first.mNum != second.mNum ||
				first.pData != second.pData){
				throw std::runtime_error("Error ..... ");
			}
			return  first.mCurrentNum != second.mCurrentNum;
		}

		size_t CurrentIndex() const{
			return mCurrentNum;
		}

		__Type&  CurrentData(){
			return pData;
		}
	private:
		__Type			pData;				// 数据的起始位置
		size_t			mNum;				// 数据个数
		size_t			mCurrentNum;		// 当前的索引
	};


	template<class T>
	class MIterator{
	public:
        friend class MString;
		typedef typename T::type type;
		typedef typename T::iterator_category iterator_category;
		typedef typename T::value_type value_type;
		typedef typename T::difference_type difference_type;
		typedef typename T::distance_type distance_type;
		typedef typename T::pointer pointer;
		typedef typename T::reference reference;
		typedef typename type __Type;

		MIterator(__Type& ptr, size_t num, size_t cnum = 0) :
			pData(ptr),
			mNum(num),
			mCurrentNum(cnum)
		{}

		MIterator(const MIterator& other) :
			pData(other.pData),
			mNum(other.mNum),
			mCurrentNum(other.mCurrentNum)
		{}

		MIterator& operator=(const MIterator& other){
			pData = other.pData;
			mNum = other.mNum;
			mCurrentNum = other.mCurrentNum;
			return *this;
		}

		value_type* operator->(){
			if (mCurrentNum < 0 || mCurrentNum >= mNum){
				throw std::out_of_range("index out of range ..... ");
			}
			return &pData[mCurrentNum];
		}

		const value_type* operator->() const{
			if (mCurrentNum < 0 || mCurrentNum >= mNum){
				throw std::out_of_range("index out of range ..... ");
			}
			return &pData[mCurrentNum];
		}

		reference operator*(){
			if (mCurrentNum < 0 || mCurrentNum >= mNum){
				throw std::out_of_range("index out of range ..... ");
			}
			return pData[mCurrentNum];
		}

		const reference operator*() const{
			if (mCurrentNum < 0 || mCurrentNum >= mNum){
				throw std::out_of_range("index out of range ..... ");
			}
			return pData[mCurrentNum];
		}

		MIterator operator++(int){
			MIterator temp(*this);
			++mCurrentNum;
			return temp;
		}

		MIterator& operator++(){
			++mCurrentNum;
			return *this;
		}

		MIterator& operator+(size_t index){
			mCurrentNum += index;
			return *this;
		}

		MIterator& operator-(size_t index){
			mCurrentNum -= index;
			return *this;
		}

		size_t pos(){
			return mCurrentNum;
		}

		friend size_t operator-(MIterator first, MIterator second){
			return first.mCurrentNum - second.mCurrentNum;
		}

		friend bool operator<(MIterator first, MIterator second){
			if (first.mNum != second.mNum ||
				first.pData != second.pData){
				throw std::runtime_error("Error ..... ");
			}
			return first.mCurrentNum < second.mCurrentNum;
		}

		friend bool operator>(MIterator first, MIterator second){
			if (first.mNum != second.mNum ||
				first.pData != second.pData){
				throw std::runtime_error("Error ..... ");
			}
			return first.mCurrentNum > second.mCurrentNum;
		}

		friend bool operator==(MIterator first, MIterator second){
			if (first.mNum != second.mNum ||
				first.pData != second.pData){
				throw std::runtime_error("Error ..... ");
			}
			return  first.mCurrentNum == second.mCurrentNum;
		}

		friend bool operator!=(MIterator first, MIterator second){
			if (first.mNum != second.mNum ||
				first.pData != second.pData){
				throw std::runtime_error("Error ..... ");
			}
			return  first.mCurrentNum != second.mCurrentNum;
		}

		size_t CurrentIndex() const{
			return mCurrentNum;
		}

		__Type&  CurrentData(){
			return pData;
		}
	private:
		__Type			pData;				// 数据的起始位置
		size_t			mNum;				// 数据个数
		size_t			mCurrentNum;		// 当前的索引
	};

	//======================================================
	// 一个轻量级的字符管理组件
	//======================================================
	class is_any_of{
	public:
		//==============================================
		// 配合迭代器使用的typedef
		//==============================================
		typedef char* type;
		typedef MIterator<is_any_of> iterator;
		typedef std::random_access_iterator_tag iterator_category;
		typedef char value_type;
		typedef std::ptrdiff_t difference_type;
		typedef std::ptrdiff_t distance_type;
		typedef char* pointer;
		typedef char& reference;

	public:
		is_any_of(const char* s){
			init(s);
		}

		~is_any_of(){
			if (m_data)
				delete[] m_data;
			m_data = nullptr;
		}

		is_any_of(const is_any_of& other){
			init(other.m_data);
		}


		is_any_of& operator=(const is_any_of& other){
			if (this == &other)
				return *this;
			if (m_data)
				delete[] m_data;
			m_data = nullptr;
			init(other.m_data);
			return *this;
		}

		bool empty(){
			if (m_data[0] == 0)
				return true;
			return false;
		}

		iterator begin(){
			return iterator(m_data, strlen(m_data) + 1);
		}

		iterator end(){
			return iterator(m_data, strlen(m_data) + 1, strlen(m_data));
		}

		void * operator&() const{
			return m_data;
		}

                operator std::string(){
                    return std::string(m_data);
                }

	private:
		void init(const std::string& s){
			int len = s.length();
			len = len == 0 ? 1 : len;
			m_data = new char[len + 1];
			strcpy_s(m_data, len + 1, s.c_str());
		}
		char* m_data;
	};
}

