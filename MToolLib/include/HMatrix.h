
//***************************************************************************
// Matrix
//***************************************************************************

#pragma once
#include <HVector.h>
#include <HTuple.h>
#include <vector>

//===========================================================================
//==================================Matrix===================================
//===========================================================================
namespace mj{
    enum Angle{
        __1Div2Pi = 0,          // 90
        __2Div2Pi,		// 180
        __3Div2Pi,		// 270
        __4Div2Pi,		// 360
        __N1Div2Pi,		// -90
        __N2Div2Pi,		// -180
        __N3Div2Pi,		// -1270
        __N4Div2Pi,		// -360
        __None			// 0
    };
	template<class T>
	class Matrix{
		typedef std::vector<T> vector_data;
	public:
		typedef vector_data value_type;
		typedef typename std::vector<value_type>::iterator iterator;

		Matrix(){
			m_data.clear();
			m_temp_colum.clear();
		}

		Matrix(unsigned row,unsigned colum,T t = T()){
			for (size_t i = 0; i < row; ++i){
				std::vector<T> temp(colum, t);
				m_data.push_back(temp);
			}
			m_temp_colum.clear();
		}

		Matrix(const Matrix& other){
			m_data = other.m_data;
		}

		Matrix(const std::vector<value_type> __data){
			m_data = __data;
		}

		Matrix<T>& operator=(const Matrix<T>& other){
			if (&other == this)
				return *this;
			m_data.clear();
			m_data = other.m_data;
			return *this;
		}

		Matrix<T>& operator=(const std::vector<value_type> __data){
			if (m_data == __data)
				return *this;
			m_data.clear();
			m_data = __data;
			return *this;
		}

		std::vector<vector_data>& getData(){
			return m_data;
		}

		const std::vector<vector_data>& getData() const{
			return m_data;
		}

		unsigned colum(unsigned row){
			if (m_data.empty())
				return 0;
			return m_data.at(row).size();
		}

		unsigned colum(){
			if (m_data.empty())
				return 0;
			return m_data.at(0).size();
		}

		unsigned row(){
			return m_data.size();
		}

		void resize(unsigned row, unsigned colum){
			m_data.resize(row);
			for (int i = 0; i < row; ++i){
				m_data[i].resize(colum);
			}
		}

		void clear(){
			m_data.clear();
		}

		bool empty(){
			return m_data.empty();
		}

		size_t size(){
			return m_data.size();
		}

		T at(unsigned row, unsigned colum) const{
			if (row >= m_data.size())
				throw std::runtime_error("row index beyong range!!!!");
			if (colum >= m_data.at(row).size())
				throw std::runtime_error("colum index beyong range!!!");
			return m_data.at(row).at(colum);
		}


		T at(unsigned row, unsigned colum){
			return static_cast<const Matrix*>(this)->at(row, colum);
		}

		const T& operator()(unsigned row, unsigned colum) const{
			if (row >= m_data.size())
				throw std::runtime_error("row index beyong range!!!!");
			if (colum >= m_data.at(row).size())
				throw std::runtime_error("colum index beyong range!!!");
			return m_data[row][colum];
		}

		T& operator()(unsigned row, unsigned colum){
			return const_cast<T&>(static_cast<const Matrix&>(*this)(row, colum));
		}

		//=============================================
		// 这里的逻辑存在一些错误
		// 这里应该是获取一列数据
		// 但是当时没仔细考虑
		// 就弄了个这名字
		// 但是现在很多程序都在使用这个库
		// 那么想想也就算了
		//=============================================
		std::vector<T> getColumdata(unsigned colum){
			std::vector<T> result;
			result.reserve(m_data.size());
			std::for_each(m_data.begin(), m_data.end(), [&](vector_data data){
				if (data.size() <= colum)
					result.push_back(T());
				else
					result.push_back(data.at(colum));
			});
			return result;
		}

		//========================================
		// 这两个名字应该和上面的翻转过来
		// 但是原因和上面一样
		// 使用的时候记得注意一些就好
		//========================================
		std::vector<T> getRowData(unsigned row){
			std::vector<T> result;
			if (row >= m_data.size()){
				throw std::runtime_error("row index beyong range!!!!!");
			}
			result = m_data.at(row);
			return result;
		}

		void push_back(std::vector<T>& data){
			m_data.push_back(data);
		}

		void push_back(std::initializer_list<T> l){
			m_data.push_back(std::vector<T>(l));
		}

		Matrix<T>& operator+=(T value){
			int row = m_data.size();
			for (int i = 0; i < row; ++i){
				int colum = m_data[i].size();
				for (int j = 0; j < colum; ++j){
					m_data[i][j] += value;
				}
			}
			return *this;
		}

		Matrix<T>& operator-=(T value){
			*this += -1*value;
			return *this;
		}

		Matrix<T>& operator*=(T value){
			int row = m_data.size();
			for (int i = 0; i < row; ++i){
				int colum = m_data[i].size();
				for (int j = 0; j < colum; ++j){
					m_data[i][j] *= value;
				}
			}
			return *this;
		}

		Matrix<T>& operator/=(T value){
			int row = m_data.size();
			for (int i = 0; i < row; ++i){
				int colum = m_data[i].size();
				for (int j = 0; j < colum; ++j){
					m_data[i][j] /= value;
				}
			}
			return *this;
		}

		friend Matrix<T>& operator<<(Matrix<T>& m,std::initializer_list<T> l){
			m.push_back(l);
			return m;
		}

		//--------------------------------------------------
		// 获取一个子阵
		// start_row 其实row
		// row_count 多少行
		// start_colum 其实colum
		// colum_count 多少列
		// 返回的结果是一个拷贝
		//--------------------------------------------------
		Matrix<T>  SubMatrix(unsigned start_row, unsigned row_count, unsigned start_colum, unsigned colum_count){
			Matrix<T> __result(row_count, colum_count);
			if (start_row >= m_data.size() || (row_count + start_row) >= m_data.size() ||
				start_colum >= colum() || (start_colum + colum_count) >= colum())
			{
				throw std::runtime_error("row or colum index beyong range .... ");
			}

			int row = 0, colum = 0;
			for (int i = start_row; i < (row_count + start_row); ++i){
				colum = 0;
				for (int j = start_colum; j < (start_colum + colum_count); ++j){
					__result(row, colum) = (*this)(i, j);
					++colum;
				}
				++row;
			}
			return __result;
		}

		//-----------------------------------------------------
		// 通过一个子阵对矩阵进行修改
		// start_row 需要修改的开始row
		// start_colum 需要修改的开始colum
		// m 替换的数据
		//-----------------------------------------------------
		void ReplacePart(unsigned start_row, unsigned start_colum, Matrix<T>& m){
			if (empty())
			{
				*this = m;
				return;
			}
			if (start_row >= m_data.size() || (m.row() + start_row) >= m_data.size() ||
				start_colum >= colum() || (m.colum()+start_colum) >= colum())
			{
				throw std::runtime_error("row or colum index beyong range .... ");
			}

			int row = 0, colum = 0;
			for (int i = start_row; i < (start_row + m.row()); ++i){
				colum = 0;
				for (int j = start_colum; j < (start_colum + m.colum()); ++j){
					m_data[i][j] = m(row, colum);
					++colum;
				}
				++row;
			}
		}

		//---------------------------------------------
		// 翻转矩阵
		// 将行变成列，将列编程行
		//---------------------------------------------
		Matrix<T> flip(){
			Matrix<T> result;
			if (m_data.empty()){
				return result;
			}
			//===================================
			//检查矩阵所有行数的列数是否相等
			//===================================
			size_t count = m_data.begin()->size();
			for (auto& it : m_data){
				if (count != it.size()){
					return result;
				}
			}

			for (int i = 0; i < count; ++i){
				result.push_back(getColumdata(i));
			}

			return result;
		}

		//================================================
		// 矩阵旋转
		// 只能按照90 180 270 360 等特殊角度旋转
		//================================================
		Matrix<T> rotation(Angle angle = __None){
			Matrix<T> __result;
			if (m_data.empty()){
				return __result;
			}

			//===================================
			//检查矩阵所有行数的列数是否相等
			//===================================
			size_t colum = m_data.begin()->size();
			size_t row = m_data.size();
			for (auto& it : m_data){
				if (colum != it.size()){
					throw std::runtime_error("matrix isn't standar matrix and couldn't rotation");
				}
			}

			//====================================
			// 将数据旋转储存
			//====================================
			switch (angle){
			case __1Div2Pi:
				while (colum--){
					auto __data = getColumdata(colum);
					__result.push_back(__data);
				}
				break;

			case __2Div2Pi:
				while (row--){
					auto __data = getRowData(row);
					std::reverse(__data.begin(), __data.end());
					__result.push_back(__data);
				}
				break;

			case __3Div2Pi:
				for (int i = 0; i < colum; ++i){
					auto __data = getColumdata(colum);
					std::reverse(__data.begin(), __data.end());
					__result.push_back(__data);
				}
				break;

			case __4Div2Pi:
				return *this;
				break;

			case __N1Div2Pi:
				for (int i = 0; i < colum; ++i){
					auto __data = getColumdata(colum);
					std::reverse(__data.begin(), __data.end());
					__result.push_back(__data);
				}
				break;

			case __N2Div2Pi:
				while (row--){
					auto __data = getRowData(row);
					std::reverse(__data.begin(), __data.end());
					__result.push_back(__data);
				}
				break;

			case __N3Div2Pi:
				while (colum--){
					auto __data = getColumdata(colum);
					__result.push_back(__data);
				}
				break;

			case __N4Div2Pi:
				return *this;
				break;

			case __None:
				break;
			}
			return __result;
		}

		//===============================================================
		// 将数据对齐
		//===============================================================
		void homogeneous(){	
			//===========================================
			// 计算出count最大的数据
			//===========================================
			std::vector<size_t> count_v;
			for (auto& it : m_data){
				count_v << it.size();
			}
			int max_count = *(std::max_element(count_v.begin(), count_v.end()));
			Matrix<T> __temp_m;
			for (auto& it : m_data){
				if (it.size() < max_count){
					std::vector<T> __temp_v;
					int diff = max_count - it.size();
					int __first = diff / 2;
					int __second = diff - __first;
					for (int i = 0; i < __first; ++i){
						__temp_v << T();
					}
					__temp_v << it;

					for (int i = 0; i < __second; ++i){
						__temp_v << T();
					}
					__temp_m.push_back(__temp_v);
				}
				else{
					__temp_m.push_back(it);
				}
			}
			*this = __temp_m;
		}

		friend std::ostream& operator<<(std::ostream& os, const Matrix& m){
			if (m.m_data.empty())
				return os;
			os << "[row:" << m.m_data.size() << "\t" << "first colum:" << m.m_data.at(0).size() << "]\n";
			std::for_each(m.m_data.begin(), m.m_data.end(), [&](vector_data data){
				std::copy(data.begin(), data.end() - 1, std::ostream_iterator<T>(os, " \t"));
				os << data.at(data.size() - 1) << std::endl;
			});
			return os;
		}


	private:
		std::vector<vector_data> m_data;
		vector_data				 m_temp_colum;
	};


	//=========================================================
	//通过读取文件数据来填充matrix
	//=========================================================
	template<class T>
	inline void fillMatrixFromFile(const std::string& filename, Matrix<T>& m, const std::string& startline = "(\\[.{1,2000}\\])"){
		std::ifstream inFile(filename);
		if (inFile.fail()){
			throw std::runtime_error("open file for read fail!!!");
		}
		bool b_is_empty = startline.empty();
		std::string read_str;
		std::regex  reg(startline);
		std::vector<std::string> temp_v;
		std::vector<T>           result_v;
		while (std::getline(inFile, read_str)){
			if (read_str.empty())
				continue;
			if (b_is_empty){
				temp_v.clear();
				result_v.clear();
				boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
				if (!(std::is_same<T, std::string>::value))
					temp_v.erase(std::remove(temp_v.begin(), temp_v.end(), ""), temp_v.end());
				auto it = temp_v.begin();
				while (it != temp_v.end()){
					boost::trim(*it);
					T temp = boost::lexical_cast<T>(*it);
					result_v.push_back(temp);
					++it;
				}
				m.push_back(result_v);
			}
			else if (std::regex_match(read_str, reg)){
				while (std::getline(inFile, read_str)){
					if (read_str.empty())
						continue;
					temp_v.clear();
					result_v.clear();
					boost::split(temp_v, read_str, boost::is_any_of(" \t"), boost::token_compress_on);
					if (!(std::is_same<T, std::string>::value))
						temp_v.erase(std::remove(temp_v.begin(), temp_v.end(), ""), temp_v.end());
					auto it = temp_v.begin();
					while (it != temp_v.end()){
						boost::trim(*it);
						T temp = boost::lexical_cast<T>(*it);
						result_v.push_back(temp);
						++it;
					}
					m.push_back(result_v);
				}
			}
		}
		inFile.close();
	}


	template<class T>
	static inline Matrix<T> operator+(Matrix<T>& __data, T value){
		__data += value;
		return __data;
	}

	template<class T>
	static inline Matrix<T> operator-(Matrix<T>& __data, T value){
		return __data + (-1)*value;;
	}

	template<class T>
	static inline Matrix<T> operator*(Matrix<T>& __data, T value){
		__data *= value;
		return __data;
	}

}
