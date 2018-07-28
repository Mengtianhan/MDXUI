#pragma once

#include "MMultType.h"
#include "MVector.h"
#include "HContainer.h"
#include <mutex>
#include <MThreadMode.h>
#include <MScopeLock.h>
#include <MMatrixSheet.h>
#include <MTypeWrap.h>

namespace TL{

        template<class T>
        struct MEquals{
            bool operator()(T val1,T val2){
                return val1==val2;
            }
        };


        //
        // 线性插值算法
        // 此处用于提取子表
        // 自然可以用于其他用途
        //
        template<class T>
        struct MLineInterp{
            inline T operator()(T x1, T y1, T x2, T y2, T x3) const{
                if (std::abs(x1 - x2) < 1e-12){
                    return y1;
                }
                if (std::abs(y1 - y2) < 1e-12){
                    return y1;
                }
                return (y2 - y1) / (x2 - x1)*(x3 - x2) + y2;
            }
        };


        //
        // class MInterpType
        //
        struct MInterpType{
        public:
           template<class T,class T2,class T3>
           std::vector<double> operator()(const std::vector<T>& xs,const std::vector<T2>& ys,const std::vector<T3>& disx) const{
                std::vector<double> __xs(xs.begin(),xs.end()),__ys(ys.begin(),ys.end()),__disx(disx.begin(),disx.end());
                std::vector<double> outx = interp(__xs,__ys,__disx);
                return outx;
            }

        protected:
            virtual std::vector<double> interp(const std::vector<double>& xs,const std::vector<double>& ys,const std::vector<double>& disx) const{
                std::vector<double> outvec;
				if (disx.size() == 1){
					return ys;
				}
                int index = 0;
                for(int i=0;i<xs.size();++i){
                    double x1 = xs[i];
                    double y1 = ys[i];
                    if(index >= disx.size() || i == xs.size() - 1){
						double val = disx.back();
						double x2 = xs[i - 1];
						double y2 = ys[i - 1];
						if (val < x2 || i == xs.size() - 1){
							double y = InterpHelp(x2, y2, x1, y1, val);
							outvec.push_back(y);
						}
                        break;
                    }
					double x2 = xs[i + 1];
					double y2 = ys[i + 1];

                    for(;index < disx.size();){
                        double val = disx[index];
                        if(val < x2 || i == xs.size() - 1){
                            double y = InterpHelp(x1,y1,x2,y2,val);
                            outvec.push_back(y);
							++index;
                        }
                        else{
                            break;
                        }
                    }
                }
                return outvec;
            }

            virtual double InterpHelp(double x1, double y1, double x2, double y2, double x3) const{
                return MLineInterp<double>()(x1,y1,x2,y2,x3);
            }
        };




        //+------------------------------------------------------
        //
        // class MNoSqlDB
        // ThreadMode 线程模型 提供Lock 和 UnLock 函数方便加锁和解锁
        // 仿数据库操作的一个数据结构,所以取名为NoSqlDB , 即不需要Sql语句的数据存储方式
        //
        //+--------------------------------------------------------
        template<class ThreadMode, class T, class...Args>
        class MNoSqlDB : public ThreadMode{
	public:
		typedef MMultType<mjTL::MTypeList<T, Args...>>  value_type;
		typedef std::vector<value_type>   data_type;
		typedef typename std::vector<value_type>::iterator iterator;
		typedef typename std::vector<value_type>::const_iterator const_iterator;
	private:
		data_type  mData;
	public:
		MNoSqlDB();
		~MNoSqlDB();
		MNoSqlDB(const MNoSqlDB& other);
		MNoSqlDB(MNoSqlDB&& other);

		MNoSqlDB& operator=(const MNoSqlDB& other);
		MNoSqlDB& operator=(MNoSqlDB&& other);

		template<class __ThreadMode>
		MNoSqlDB(const MNoSqlDB<__ThreadMode, T, Args...>& other);

		template<class __ThreadMode>
		MNoSqlDB(MNoSqlDB<__ThreadMode, T, Args...>&& other);

		template<class __ThreadMode>
		MNoSqlDB& operator=(const MNoSqlDB<__ThreadMode, T, Args...>& other);

		template<class __ThreadMode>
		MNoSqlDB& operator=(MNoSqlDB<__ThreadMode, T, Args...>&& other);

		//
		// 获取底层数据
                // 一旦获取 底层数据后如果直接对数据修改在多线程模型中将变得不受保护
                // 所以如果不是必须不要尝试获取底层的data
		//
		const data_type& data() const{ return mData; }
		data_type& data(){ return mData; }

		//
		// 后面追加
		//
		void push_back(const T& value, const Args&...args);
		void push_back(const value_type& value);
		void push_back(const MNoSqlDB& value);

		//
		// 指定位置插入 如果位置为-1 数据将追加在后面
		//
		void insert(unsigned pos, const T& value, const Args&...args);
		void insert(unsigned pos, const value_type& value);
		void insert(unsigned pos, const MNoSqlDB& value);

		//
		// 获取指定位置的数据
		// 如果指定位置操作范围，将抛出std::out_of_range 异常
		//
		void make_reference(unsigned pos, T& value, Args&... args) const;

		//
		// 对特定数据进行解包
		//
		void make_reference(const value_type& v, T& value, Args&... args) const;

		//
		// 给定参数生成一个值对象
		//
		value_type make_value(const T& value, const Args&... args) const;

		//
		// 获取表中的最后一行数据,如果表为空将抛出std::out_of_range异常
		//
		value_type& back();
		const value_type& back() const;

		value_type& front();
		const value_type& front() const;

		//
		// 弹出最后一项和最前面一项
		//
		void pop_back();
		void pop_front();

		//
		// 获取行列
		//
		unsigned colums() const;
		unsigned rows() const;

                void clear();

                bool empty();
                bool empty() const;

                iterator begin();
                iterator end();

                const_iterator begin() const;
                const_iterator end() const;

                //
                // 打印
                //
                void print(const char* msg,std::ostream& os = std::cout);

                //
                // 保存和加载 --- 文本
                //
                void save(const char* filename,const char* spliter,bool isBinary = true);
                void saveAll(const char* filename,const char* spliter);
                void load(const char* filename,const char* spliter,bool isBinary = true);
                void loadAll(const char* filename,const char* spliter);

                //
                // 从流中读取 或者 将数据写进流
                //
				void istream(std::istream& is, const char* spliter = ", \t");
				void istreamAll(std::istream& is, const char* spliter = ", \t");
                void ostream(std::ostream& os,const char* spliter = ", \t");



                //+----------------------
                //
                // 转换成矩阵的形式,全部变换为字符
                //
                //+-----------------------
                mj::MMatrixSheet<MString> toMatrix() const;
                void fromMatrix(const mj::MMatrixSheet<MString>& m);



                //+----------------------------
                //
                // 针对每一行进行数据修改
                //
                //+-----------------------------
                template<class V1,int M1,class Fun>
                void transform(Fun fun);

                template<int M1,class Fun>
                void transform(Fun fun);




                //+-----------------------------
                //
                // 同时操作多列,参数以引用的方式传递,就地修改无需返回
                //
                //+------------------------------
                template<int M1,int M2,class Fun>
                void mult_transform(Fun fun);

                template<int M1,int M2,int M3,class Fun>
                void mult_transform(Fun fun);

                template<int M1,int M2,int M3,int M4,class Fun>
                void mult_transform(Fun fun);

                template<int M1,int M2,int M3,int M4,int M5,class Fun>
                void mult_transform(Fun fun);

                template<int M1,int M2,int M3,int M4,int M5,int M6,class Fun>
                void mult_transform(Fun fun);



                //+---------------------
                //
                // for_each 和 transform 都是遍历某一列
                // transform 可对原始数据进行修改
                // for_each 不对原始数据进行修改
                //
                //+------------------------
                template<int M1,class Fun>
                void for_each(Fun fun);




                //+----------------
		//
		// 获取指定位置上的数据的引用
		// 如果超出范围将抛出std::out_of_range 异常
		//
                //+----------------
                template<int N>
                typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N>::type& at(unsigned row);

                template<int N>
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N>::type& at(unsigned row) const;



                //+-------------------
                //
                // 如果超出范围将抛出异常
                //
                //+--------------------
                value_type& operator[](unsigned index);
                const value_type operator[](unsigned index) const;



                //+------------------
                //
                // 指定设定某个值
                // 后面有支持多参数的set接口
                // 支持满足条件的设置
                // M 是列
                // row 是行
                //
                //+-------------------
                template<int M>
                void set(unsigned row,const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M>::type& val);



                //+--------------
		//
		// 获取指定数据的数据
		//
                //+---------------
                template<int N>
                typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N>::type& get(const value_type& v);

                template<int N>
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N>::type& get(const value_type& v) const;




                //+------------------
                //
                // 删除数据
                //
                //+-------------------
                iterator erase(iterator __It);
                void erase(iterator __First,iterator __Last);



                //+---------------------
                //
                // 删除指定项
                //
                //+----------------------
                void remove(const value_type& v);




                //+-----------------------
                //
                // 该操作需要先将当前数据进行排序
                //
                //+-----------------------
                template<class V1,int M1,class Prd1 = MEquals<V1>>
                iterator low_index(const V1& value1,Prd1 prd = Prd1());

                template<class V1,int M1,class Prd1 = MEquals<V1>>
                iterator up_index(const V1& value1,Prd1 prd = Prd1());

                template<int M1,class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>>
                iterator low_index(const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,Prd1 prd = Prd1());

                template<int M1,class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>>
                iterator up_index(const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,Prd1 prd = Prd1());



                //+----------------
                //
                // 拼接出新的表
                //
                //+-----------------
                template<class...Other>
                MNoSqlDB<ThreadMode,T,Args...,Other...> cat(const MNoSqlDB<ThreadMode,Other...>& other);



                //+---------------------
		//
		// 下面通过多元数据来操作表
                //  使用item替代get_item将不需要指定元素类型
		//
                //+---------------------
                template<class V1, int M1, class Prd1 = MEquals<V1>>
                value_type& get_item(const V1& value1, Prd1 prd = Prd1());

                template<
                        class V1, int M1, class V2, int M2,
                        class Prd1 = MEquals<V1>, class Prd2 = MEquals<V2 >>
                value_type& get_item(
                        const V1& value1, const V2& value2,
                        Prd1 prd = Prd1(), Prd2 prd2 = Prd2());

		template<
                        class V1, int M1,class V2, int M2,
                        class V3, int M3,class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,class Prd3 = MEquals<V3 >
		>
                value_type& get_item(
                        const V1& value1,const V2& value2,
                        const V3& value3,Prd1 __ptr = Prd1(),
                        Prd2 __prd2 = Prd2(),Prd3 __prd3 = Prd3()
                        );

                template<
                        class V1, int M1,
                        class V2, int M2,
                        class V3, int M3,
                        class V4, int M4,
                        class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,
                        class Prd3 = MEquals<V3 >,
                        class Prd4 = MEquals<V4 >
                        >
                value_type& get_item(
                        const V1& value1, const V2& value2,
                        const V3& value3, const V4& value4,
                        Prd1 __ptr1 = Prd1(), Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(), Prd4 __prd4 = Prd4()
                        );

                template<
                        class V1, int M1,
                        class V2, int M2,
                        class V3, int M3,
                        class V4, int M4,
                        class V5, int M5,
                        class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,
                        class Prd3 = MEquals<V3>,
                        class Prd4 = MEquals<V4>,
                        class Prd5 = MEquals<V5>
                        >
                value_type& get_item(
                        const V1& value1, const V2& value2,
                        const V3& value3, const V4& value4,
                        const V5& value5,
                        Prd1 __ptr1 = Prd1(), Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(), Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5()
                        );

                template<
                        class V1, int M1,
                        class V2, int M2,
                        class V3, int M3,
                        class V4, int M4,
                        class V5, int M5,
                        class V6, int M6,
                        class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,
                        class Prd3 = MEquals<V3 >,
                        class Prd4 = MEquals<V4 >,
                        class Prd5 = MEquals<V5 >,
                        class Prd6 = MEquals<V6 >
                        >
                value_type& get_item(
                        const V1& value1, const V2& value2,
                        const V3& value3, const V4& value4,
                        const V5& value5, const V6& value6,
                        Prd1 __ptr1 = Prd1(), Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(), Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5(), Prd6 __prd6 = Prd6()
                        );





                //+------------------------
                //
                // 通过指定条件，搜索一个字表
                // 使用 subdb 将不需要指定元素类型
                //
                //+-------------------------
                template<class V1, int M1, class Prd1 = MEquals<V1>>
                MNoSqlDB get_subdb(const V1& value1, Prd1 prd = Prd1());

                template<
                        class V1, int M1, class V2, int M2,
                        class Prd1 = MEquals<V1>, class Prd2 = MEquals<V2 >
                        >
                MNoSqlDB get_subdb(
                        const V1& value1, const V2& value2,
                        Prd1 prd = Prd1(), Prd2 prd2 = Prd2());

                template<
                        class V1, int M1,class V2, int M2,
                        class V3, int M3,class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,class Prd3 = MEquals<V3 >
                        >
                MNoSqlDB get_subdb(
                        const V1& value1,const V2& value2,
                        const V3& value3,Prd1 __ptr = Prd1(),
                        Prd2 __prd2 = Prd2(),Prd3 __prd3 = Prd3()
                        );

                template<
                        class V1, int M1,
                        class V2, int M2,
                        class V3, int M3,
                        class V4, int M4,
                        class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,
                        class Prd3 = MEquals<V3 >,
                        class Prd4 = MEquals<V4 >
                        >
                MNoSqlDB get_subdb(
                        const V1& value1, const V2& value2,
                        const V3& value3, const V4& value4,
                        Prd1 __ptr1 = Prd1(), Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(), Prd4 __prd4 = Prd4()
                        );

                template<
                        class V1, int M1,
                        class V2, int M2,
                        class V3, int M3,
                        class V4, int M4,
                        class V5, int M5,
                        class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,
                        class Prd3 = MEquals<V3>,
                        class Prd4 = MEquals<V4>,
                        class Prd5 = MEquals<V5>
                        >
                MNoSqlDB get_subdb(
                        const V1& value1, const V2& value2,
                        const V3& value3, const V4& value4,
                        const V5& value5,
                        Prd1 __ptr1 = Prd1(), Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(), Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5()
                        );

                template<
                        class V1, int M1,
                        class V2, int M2,
                        class V3, int M3,
                        class V4, int M4,
                        class V5, int M5,
                        class V6, int M6,
                        class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,
                        class Prd3 = MEquals<V3 >,
                        class Prd4 = MEquals<V4 >,
                        class Prd5 = MEquals<V5 >,
                        class Prd6 = MEquals<V6 >
                        >
                MNoSqlDB get_subdb(
                        const V1& value1, const V2& value2,
                        const V3& value3, const V4& value4,
                        const V5& value5, const V6& value6,
                        Prd1 __ptr1 = Prd1(), Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(), Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5(), Prd6 __prd6 = Prd6()
                        );





                //+-------------------
                //
                // 获取指定列
                // 可指定元素类型也不可指定元素类型
                //
                //+--------------------
                template<class V,int N>
                MTypeWrap<std::vector<V>> colum() const;

                template<int N>
                MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N>::type>> colum() const;

				template<
					int N,int IF_M1,
					class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type >
				>
				MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type>> colum_if(
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type& value1,
					Prd1 __ptr1 = Prd1()
					) const;

				template<
					int N, int IF_M1, int IF_M2,
					class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type>,
					class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type >
				>
				MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type>> colum_if(
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type& value1,
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type& value2,
					Prd1 __ptr1 = Prd1(),
					Prd2 __prd2 = Prd2()
					) const;

				template<
					int N, int IF_M1, int IF_M2,int IF_M3,
					class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type>,
					class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type >,
					class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M3>::type >
				>
				MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type>> colum_if(
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type& value1,
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type& value2,
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M3>::type& value3,
					Prd1 __ptr1 = Prd1(),
					Prd2 __prd2 = Prd2(),
					Prd3 __prd3 = Prd3()
					) const;

				template<
					int N, int IF_M1, int IF_M2, int IF_M3, int IF_M4,
					class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type>,
					class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type >,
					class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M3>::type>,
					class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M4>::type >
				>
				MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type>> colum_if(
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type& value1,
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type& value2,
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M3>::type& value3,
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M4>::type& value4,
					Prd1 __ptr1 = Prd1(),
					Prd2 __prd2 = Prd2(),
					Prd3 __prd3 = Prd3(),
					Prd4 __prd4 = Prd4()
					) const;


				template<
					int N, int IF_M1, int IF_M2, int IF_M3, int IF_M4, int IF_M5,
					class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type>,
					class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type >,
					class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M3>::type>,
					class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M4>::type >,
					class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M5>::type >
				>
				MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type>> colum_if(
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type& value1,
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type& value2,
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M3>::type& value3,
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M4>::type& value4,
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M5>::type& value5,
					Prd1 __ptr1 = Prd1(),
					Prd2 __prd2 = Prd2(),
					Prd3 __prd3 = Prd3(),
					Prd4 __prd4 = Prd4(),
					Prd5 __prd5 = Prd5()
					) const;

				template<
					int N, int IF_M1, int IF_M2, int IF_M3, int IF_M4, int IF_M5, int IF_M6,
					class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type>,
					class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type >,
					class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M3>::type>,
					class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M4>::type >,
					class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M5>::type>,
					class Prd6 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M6>::type >
				>
				MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type>> colum_if(
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type& value1,
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type& value2,
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M3>::type& value3,
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M4>::type& value4,
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M5>::type& value5,
					const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M6>::type& value6,
					Prd1 __ptr1 = Prd1(),
					Prd2 __prd2 = Prd2(),
					Prd3 __prd3 = Prd3(),
					Prd4 __prd4 = Prd4(),
					Prd5 __prd5 = Prd5(),
					Prd6 __prd6 = Prd6()
					) const;


                //+------------------------
                //
                // 通过指定类型创建一张表
                // 使用colum_db 不需要指定每一列的元素类型
                //
                //+------------------------
                template<
                        class V1,int M1,
                        class V2,int M2,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<__ThreadMode,V1,V2> create_db() const;

                template<
                        class V1,int M1,
                        class V2,int M2,
                        class V3,int M3,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<__ThreadMode,V1,V2,V3> create_db() const;

                template<
                        class V1,int M1,
                        class V2,int M2,
                        class V3,int M3,
                        class V4,int M4,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<__ThreadMode,V1,V2,V3,V4> create_db() const;


                template<
                        class V1,int M1,
                        class V2,int M2,
                        class V3,int M3,
                        class V4,int M4,
                        class V5,int M5,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<__ThreadMode,V1,V2,V3,V4,V5> create_db() const;

                template<
                        class V1,int M1,
                        class V2,int M2,
                        class V3,int M3,
                        class V4,int M4,
                        class V5,int M5,
                        class V6,int M6,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<__ThreadMode,V1,V2,V3,V4,V5,V6> create_db() const;





                //+-----------------
                //
                // 按照给定条件进行排序
                // 可指定元素类型也不可指定元素类型
                //
                //+------------------
                template<class V1,int M1,class Cmp = std::less<V1>>
                void sort(Cmp __cmp = Cmp());

                template<class V1,int M1,class Cmp = std::less<V1>>
                void sort(iterator pos1,iterator pos2,Cmp __cmp = Cmp());


                template<int M1,class Cmp = std::less<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>>
                void sort(Cmp __cmp = Cmp());

                template<int M1,class Cmp = std::less<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>>
                void sort(iterator pos1,iterator pos2,Cmp __cmp = Cmp());




                //+--------------------
                //
                // 唯一化
                // 不需要指定元素类型
                // 指定列即可
                //
                //+---------------------
                template<int N,
                         class Cmp1 = std::less<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N>::type>,
                         class Cmp2 = std::equal_to<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N>::type>>
                void unique(Cmp1 prd = Cmp1(), Cmp2 prd2 = Cmp2());





                //+----------------------
                //
                // 按照给定条件进行查找
                // 使用 find2 时不需要指定类型
                //
                //+------------------------
                template<class V1, int M1, class Prd1 = MEquals<V1>>
                unsigned find(const V1& value1, Prd1 prd = Prd1());

                template<
                        class V1, int M1, class V2, int M2,
                        class Prd1 = MEquals<V1>, class Prd2 = MEquals<V2 >>
                unsigned find(
                        const V1& value1, const V2& value2,
                        Prd1 prd = Prd1(), Prd2 prd2 = Prd2());

                template<
                        class V1, int M1,class V2, int M2,
                        class V3, int M3,class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,class Prd3 = MEquals<V3 >
                        >
                unsigned find(
                        const V1& value1,const V2& value2,
                        const V3& value3,Prd1 __ptr = Prd1(),
                        Prd2 __prd2 = Prd2(),Prd3 __prd3 = Prd3()
                        );

                template<
                        class V1, int M1,
                        class V2, int M2,
                        class V3, int M3,
                        class V4, int M4,
                        class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,
                        class Prd3 = MEquals<V3 >,
                        class Prd4 = MEquals<V4 >
                        >
                unsigned find(
                        const V1& value1, const V2& value2,
                        const V3& value3, const V4& value4,
                        Prd1 __ptr1 = Prd1(), Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(), Prd4 __prd4 = Prd4()
                        );

                template<
                        class V1, int M1,
                        class V2, int M2,
                        class V3, int M3,
                        class V4, int M4,
                        class V5, int M5,
                        class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,
                        class Prd3 = MEquals<V3>,
                        class Prd4 = MEquals<V4>,
                        class Prd5 = MEquals<V5>
                        >
                unsigned find(
                        const V1& value1, const V2& value2,
                        const V3& value3, const V4& value4,
                        const V5& value5,
                        Prd1 __ptr1 = Prd1(), Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(), Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5()
                        );

                template<
                        class V1, int M1,
                        class V2, int M2,
                        class V3, int M3,
                        class V4, int M4,
                        class V5, int M5,
                        class V6, int M6,
                        class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,
                        class Prd3 = MEquals<V3 >,
                        class Prd4 = MEquals<V4 >,
                        class Prd5 = MEquals<V5 >,
                        class Prd6 = MEquals<V6 >
                        >
                unsigned find(
                        const V1& value1, const V2& value2,
                        const V3& value3, const V4& value4,
                        const V5& value5, const V6& value6,
                        Prd1 __ptr1 = Prd1(), Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(), Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5(), Prd6 __prd6 = Prd6()
                        );



                //+-----------------------
                //
                // 按照给定条件进行更新数据
                // 使用 update2 不需要指定元素类型
                //
                //+-------------------------
                template<class V1, int M1, class Prd1 = MEquals<V1>>
                void update(const V1& value1, const value_type& val,Prd1 prd = Prd1());

                template<
                        class V1, int M1, class V2, int M2,
                        class Prd1 = MEquals<V1>, class Prd2 = MEquals<V2 >>
                void update(
                        const V1& value1, const V2& value2,
                        const value_type& val,
                        Prd1 prd = Prd1(), Prd2 prd2 = Prd2());

                template<
                        class V1, int M1,class V2, int M2,
                        class V3, int M3,class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,class Prd3 = MEquals<V3 >
                        >
                void update(
                        const V1& value1,const V2& value2,
                        const V3& value3,
                        const value_type& val,
                        Prd1 __ptr = Prd1(),
                        Prd2 __prd2 = Prd2(),Prd3 __prd3 = Prd3()
                        );

                template<
                        class V1, int M1,
                        class V2, int M2,
                        class V3, int M3,
                        class V4, int M4,
                        class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,
                        class Prd3 = MEquals<V3 >,
                        class Prd4 = MEquals<V4 >
                        >
                void update(
                        const V1& value1, const V2& value2,
                        const V3& value3, const V4& value4,
                        const value_type& val,
                        Prd1 __ptr1 = Prd1(), Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(), Prd4 __prd4 = Prd4()
                        );

                template<
                        class V1, int M1,
                        class V2, int M2,
                        class V3, int M3,
                        class V4, int M4,
                        class V5, int M5,
                        class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,
                        class Prd3 = MEquals<V3>,
                        class Prd4 = MEquals<V4>,
                        class Prd5 = MEquals<V5>
                        >
                void update(
                        const V1& value1, const V2& value2,
                        const V3& value3, const V4& value4,
                        const V5& value5,
                        const value_type& val,
                        Prd1 __ptr1 = Prd1(), Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(), Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5()
                        );

                template<
                        class V1, int M1,
                        class V2, int M2,
                        class V3, int M3,
                        class V4, int M4,
                        class V5, int M5,
                        class V6, int M6,
                        class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,
                        class Prd3 = MEquals<V3 >,
                        class Prd4 = MEquals<V4 >,
                        class Prd5 = MEquals<V5 >,
                        class Prd6 = MEquals<V6 >
                        >
                void update(
                        const V1& value1, const V2& value2,
                        const V3& value3, const V4& value4,
                        const V5& value5, const V6& value6,
                        const value_type& val,
                        Prd1 __ptr1 = Prd1(), Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(), Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5(), Prd6 __prd6 = Prd6()
                        );




                //+------------------------
                //
                // 按照给定条件进行删除
                // 使用 eraseall2 不需要指定元素类型
                //
                //+-------------------------
                template<class V1, int M1, class Prd1 = MEquals<V1>>
                bool eraseall(const V1& value1, Prd1 prd = Prd1());

                template<
                        class V1, int M1, class V2, int M2,
                        class Prd1 = MEquals<V1>, class Prd2 = MEquals<V2 >>
                bool eraseall(
                        const V1& value1, const V2& value2,
                        Prd1 prd = Prd1(), Prd2 prd2 = Prd2());

                template<
                        class V1, int M1,class V2, int M2,
                        class V3, int M3,class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,class Prd3 = MEquals<V3 >
                        >
                bool eraseall(
                        const V1& value1,const V2& value2,
                        const V3& value3,Prd1 __ptr = Prd1(),
                        Prd2 __prd2 = Prd2(),Prd3 __prd3 = Prd3()
                        );

                template<
                        class V1, int M1,
                        class V2, int M2,
                        class V3, int M3,
                        class V4, int M4,
                        class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,
                        class Prd3 = MEquals<V3 >,
                        class Prd4 = MEquals<V4 >
                        >
                bool eraseall(
                        const V1& value1, const V2& value2,
                        const V3& value3, const V4& value4,
                        Prd1 __ptr1 = Prd1(), Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(), Prd4 __prd4 = Prd4()
                        );

                template<
                        class V1, int M1,
                        class V2, int M2,
                        class V3, int M3,
                        class V4, int M4,
                        class V5, int M5,
                        class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,
                        class Prd3 = MEquals<V3>,
                        class Prd4 = MEquals<V4>,
                        class Prd5 = MEquals<V5>
                        >
                bool eraseall(
                        const V1& value1, const V2& value2,
                        const V3& value3, const V4& value4,
                        const V5& value5,
                        Prd1 __ptr1 = Prd1(), Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(), Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5()
                        );

                template<
                        class V1, int M1,
                        class V2, int M2,
                        class V3, int M3,
                        class V4, int M4,
                        class V5, int M5,
                        class V6, int M6,
                        class Prd1 = MEquals<V1>,
                        class Prd2 = MEquals<V2>,
                        class Prd3 = MEquals<V3 >,
                        class Prd4 = MEquals<V4 >,
                        class Prd5 = MEquals<V5 >,
                        class Prd6 = MEquals<V6 >
                        >
                bool eraseall(
                        const V1& value1, const V2& value2,
                        const V3& value3, const V4& value4,
                        const V5& value5, const V6& value6,
                        Prd1 __ptr1 = Prd1(), Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(), Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5(), Prd6 __prd6 = Prd6()
                        );


                //+---------------
                //
                // 实现流操作
                //
                //+---------------
                friend std::ostream& operator<<(std::ostream& os,const MNoSqlDB& db){
                    os<<db.rows()<<std::endl;
                    for(auto& c : db.mData){
                        os<<c<<std::endl;
                    }
                    return os;
                }












                //
                // -------------------------------------------------------------
                // 简化使用时的操作
                // 该操作将会自动推导出相应的类型
                // -------------------------------------------------------------
                //

                //
                // 等效于 get_item
                //
                template<int M1,
                         class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>
                         >
                value_type& item(const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1, Prd1 prd = Prd1());

                template<
                        int M1,
                        int M2,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type >
                        >
                value_type& item(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        Prd1 prd = Prd1(), Prd2 prd2 = Prd2());

                template<
                        int M1,
                        int M2,
                        int M3,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type >
                >
                value_type& item(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        Prd1 __ptr = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type >,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type >
                        >
                value_type& item(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        Prd1 __ptr1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type>
                        >
                value_type& item(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                        Prd1 __ptr1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int M6,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type >,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type >,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type >,
                        class Prd6 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type >
                        >
                value_type& item(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type& value6,
                        Prd1 __ptr1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5(),
                        Prd6 __prd6 = Prd6()
                        );




                //+-----------------------
                //
                // 通过指定条件，搜索一个子表 等效于 get_subdb
                // 得到的是行表
                //
                //+-----------------------
                template<
                        int M1,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>
                        >
                MNoSqlDB subdb(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                         Prd1 prd = Prd1()
                        );

                template<
                        int M1,
                        int M2,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type >
                        >
                MNoSqlDB subdb(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        Prd1 prd = Prd1(),
                        Prd2 prd2 = Prd2()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type >
                        >
                MNoSqlDB subdb(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        Prd1 __ptr = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type>
                        >
                MNoSqlDB subdb(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        Prd1 __ptr1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type>
                        >
                MNoSqlDB subdb(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                        Prd1 __ptr1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int M6,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type>,
                        class Prd6 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type>
                        >
                MNoSqlDB subdb(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type& value6,
                        Prd1 __ptr1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5(),
                        Prd6 __prd6 = Prd6()
                        );





                //+---------------------
                //
                // 通过指定类型创建一张表
                // 无参等于create_db
                //
                //+----------------------
                template<
                        int M1,
                        int M2,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type
                > colum_db() const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type
                > colum_db() const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type
                > colum_db() const;


                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type
                > colum_db() const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int M6,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type
                > colum_db() const;





                //+------------------------
                //
                // 通过指定类型创建一张表 以一个参数为条件
                //
                //+------------------------
                template<
                        int M1,
                        int M2,
                        int N1,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        Prd1 __prd1 = Prd1()
                        ) const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int N1,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        Prd1 __prd1 = Prd1()
                        ) const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int N1,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        Prd1 __prd1 = Prd1()
                        ) const;


                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int N1,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        Prd1 __prd1 = Prd1()
                        ) const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int M6,
                        int N1,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        Prd1 __prd1 = Prd1()
                        ) const;




                //+---------------------
                //
                // 通过指定类型创建一张表 以两个参数为条件
                //
                //+---------------------
                template<
                        int M1,
                        int M2,
                        int N1,
                        int N2,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2()
                        ) const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int N1,
                        int N2,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2()
                        ) const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int N1,
                        int N2,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2()
                        ) const;


                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int N1,
                        int N2,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2()
                        ) const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int M6,
                        int N1,
                        int N2,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2()
                        ) const;




                //+------------------------
                //
                // 通过指定类型创建一张表 以三个参数为条件
                //
                //+------------------------
                template<
                        int M1,
                        int M2,
                        int N1,
                        int N2,
                        int N3,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3()
                        ) const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int N1,
                        int N2,
                        int N3,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3()
                        ) const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int N1,
                        int N2,
                        int N3,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3()
                        ) const;


                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int N1,
                        int N2,
                        int N3,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3()
                        ) const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int M6,
                        int N1,
                        int N2,
                        int N3,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3()
                        ) const;




                //+--------------------
                //
                // 通过指定类型创建一张表 以四个参数为条件
                //
                //+--------------------
                template<
                        int M1,
                        int M2,
                        int N1,
                        int N2,
                        int N3,
                        int N4,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4()
                        ) const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int N1,
                        int N2,
                        int N3,
                        int N4,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4()
                        ) const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int N1,
                        int N2,
                        int N3,
                        int N4,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4()
                        ) const;


                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int N1,
                        int N2,
                        int N3,
                        int N4,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4()
                        ) const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int M6,
                        int N1,
                        int N2,
                        int N3,
                        int N4,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4()
                        ) const;





                //+------------------------
                //
                // 通过指定类型创建一张表 以五个参数为条件
                //
                //+------------------------
                template<
                        int M1,
                        int M2,
                        int N1,
                        int N2,
                        int N3,
                        int N4,
                        int N5,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5()
                        ) const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int N1,
                        int N2,
                        int N3,
                        int N4,
                        int N5,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5()
                        ) const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int N1,
                        int N2,
                        int N3,
                        int N4,
                        int N5,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5()
                        ) const;


                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int N1,
                        int N2,
                        int N3,
                        int N4,
                        int N5,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5()
                        ) const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int M6,
                        int N1,
                        int N2,
                        int N3,
                        int N4,
                        int N5,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5()
                        ) const;





                //+-----------------------
                //
                // 通过指定类型创建一张表 以六个参数为条件
                //
                //+-----------------------
                template<
                        int M1,
                        int M2,
                        int N1,
                        int N2,
                        int N3,
                        int N4,
                        int N5,
                        int N6,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type>,
                        class Prd6 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type& value6,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5(),
                        Prd6 __prd6 = Prd6()
                        ) const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int N1,
                        int N2,
                        int N3,
                        int N4,
                        int N5,
                        int N6,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type>,
                        class Prd6 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type& value6,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5(),
                        Prd6 __prd6 = Prd6()
                        ) const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int N1,
                        int N2,
                        int N3,
                        int N4,
                        int N5,
                        int N6,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type>,
                        class Prd6 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type& value6,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5(),
                        Prd6 __prd6 = Prd6()
                        ) const;


                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int N1,
                        int N2,
                        int N3,
                        int N4,
                        int N5,
                        int N6,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type>,
                        class Prd6 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type& value6,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5(),
                        Prd6 __prd6 = Prd6()
                        ) const;

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int M6,
                        int N1,
                        int N2,
                        int N3,
                        int N4,
                        int N5,
                        int N6,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type>,
                        class Prd6 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type>,
                        class __ThreadMode = ThreadMode
                        >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type
                > colum_db(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type& value6,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5(),
                        Prd6 __prd6 = Prd6()
                        ) const;






                //+--------------------------
                //
                // 使用插值的方式获取指定的内容
                //
                //+--------------------------
                template<int In,
                         int Out1,
                         class InterpOperator
                         >
                MTypeWrap<
                    std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type>
                > interp(
                        const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals
                        );

                template<int In,
                         int Out1,
                         int Out2,
                         class InterpOperator,
                         class __ThreadMode = ThreadMode
                         >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type
                > interp(
                        const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals
                        );


                template<int In,
                         int Out1,
                         int Out2,
                         int Out3,
                         class InterpOperator,
                         class __ThreadMode = ThreadMode
                         >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type
                > interp(
                        const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals
                        );

                template<int In,
                         int Out1,
                         int Out2,
                         int Out3,
                         int Out4,
                         class InterpOperator,
                         class __ThreadMode = ThreadMode
                         >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out4>::type
                > interp(
                        const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals
                        );


                template<int In,
                         int Out1,
                         int Out2,
                         int Out3,
                         int Out4,
                         int Out5,
                         class InterpOperator,
                         class __ThreadMode = ThreadMode
                         >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out4>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out5>::type
                > interp(
                        const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals
                        );

                template<int In,
                         int Out1,
                         int Out2,
                         int Out3,
                         int Out4,
                         int Out5,
                         int Out6,
                         class InterpOperator,
                         class __ThreadMode = ThreadMode
                         >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out4>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out5>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out6>::type
                > interp(
                        const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals
                        );






                //+-----------------------
                //
                // 只能在内部进行插值
                //
                //+-----------------------
                template<int In,
                         int Out1
                         >
                MTypeWrap<
                    std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type>
                > interp(
                        const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals,
                        const MInterpType& op = MInterpType()
                        );

                template<int In,
                         int Out1,
                         int Out2,
                         class __ThreadMode = ThreadMode
                         >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type
                > interp(
                        const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals,
                        const MInterpType& op = MInterpType()
                        );


                template<int In,
                         int Out1,
                         int Out2,
                         int Out3,
                         class __ThreadMode = ThreadMode
                         >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type
                > interp(
                        const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals,
                        const MInterpType& op = MInterpType()
                        );

                template<int In,
                         int Out1,
                         int Out2,
                         int Out3,
                         int Out4,
                         class __ThreadMode = ThreadMode
                         >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out4>::type
                > interp(
                        const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals,
                        const MInterpType& op = MInterpType()
                        );

                template<int In,
                         int Out1,
                         int Out2,
                         int Out3,
                         int Out4,
                         int Out5,
                         class __ThreadMode = ThreadMode
                         >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out4>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out5>::type
                > interp(
                        const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals,
                        const MInterpType& op = MInterpType()
                        );

                template<int In,
                         int Out1,
                         int Out2,
                         int Out3,
                         int Out4,
                         int Out5,
                         int Out6,
                         class __ThreadMode = ThreadMode
                         >
                MNoSqlDB<
                    __ThreadMode,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out4>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out5>::type,
                    typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out6>::type
                > interp(
                        const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals,
                        const MInterpType& op = MInterpType()
                        );





                //+------------------------
                //
                // 按照给定条件进行查找
                //
                //+------------------------
                template<int M1,
                         class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>
                         >
                unsigned find2(const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                              Prd1 prd = Prd1());

                template<
                        int M1,
                        int M2,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type >
                        >
                unsigned find2(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        Prd1 prd = Prd1(), Prd2 prd2 = Prd2());

                template<
                        int M1,
                        int M2,
                        int M3,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>
                        >
                unsigned find2(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        Prd1 __ptr = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type>
                        >
                unsigned find2(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        Prd1 __ptr1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type>
                        >
                unsigned find2(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                        Prd1 __ptr1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int M6,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type>,
                        class Prd6 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type>
                        >
                unsigned find2(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type& value6,
                        Prd1 __ptr1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5(),
                        Prd6 __prd6 = Prd6()
                        );





                //+-------------------------
                //
                // 按照给定条件进行更新数据
                //
                //+--------------------------
                template<
                        int M1,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>
                        >
                void update2(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const value_type& val,
                        Prd1 prd = Prd1());

                template<
                        int M1,
                        int M2,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type >
                        >
                void update2(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const value_type& val,
                        Prd1 prd = Prd1(),
                        Prd2 prd2 = Prd2());

                template<
                        int M1,
                        int M2,
                        int M3,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type >
                        >
                void update2(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const value_type& val,
                        Prd1 __ptr = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type >
                        >
                void update2(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        const value_type& val,
                        Prd1 __ptr1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type>
                        >
                void update2(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                        const value_type& val,
                        Prd1 __ptr1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int M6,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type>,
                        class Prd6 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type>
                        >
                void update2(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type& value6,
                        const value_type& val,
                        Prd1 __ptr1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5(),
                        Prd6 __prd6 = Prd6()
                        );


                //+----------------------
                //
                // 按照给定条件进行删除
                //
                //+----------------------
                template<
                        int M1,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>
                        >
                bool eraseall2(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        Prd1 prd = Prd1());

                template<
                        int M1,
                        int M2,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>
                        >
                bool eraseall2(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        Prd1 prd = Prd1(),
                        Prd2 prd2 = Prd2()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>
                        >
                bool eraseall2(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        Prd1 __ptr = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type >,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type >
                        >
                bool eraseall2(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        Prd1 __ptr1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type>
                        >
                bool eraseall2(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                        Prd1 __ptr1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int M6,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type >,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type >,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type >,
                        class Prd6 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type >
                        >
                bool eraseall2(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type& value6,
                        Prd1 __ptr1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5(),
                        Prd6 __prd6 = Prd6()
                        );




                //+-----------------------
                //
                // 按照给定条件进行删除
                //
                //+-----------------------
                template<
                        int M1,
                        class Fun1
                        >
                bool erase_if(
                        Fun1 fun
                        );

                template<
                        int M1,
                        int M2,
                        class Fun1,
                        class Fun2
                        >
                bool erase_if(
                        Fun1 fun1,
                        Fun2 fun2
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        class Fun1,
                        class Fun2,
                        class Fun3
                        >
                bool erase_if(
                        Fun1 fun1,
                        Fun2 fun2,
                        Fun1 fun3
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        class Fun1,
                        class Fun2,
                        class Fun3,
                        class Fun4
                        >
                bool erase_if(
                        Fun1 fun1,
                        Fun2 fun2,
                        Fun3 fun3,
                        Fun4 fun4
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        class Fun1,
                        class Fun2,
                        class Fun3,
                        class Fun4,
                        class Fun5
                        >
                bool erase_if(
                        Fun1 fun1,
                        Fun2 fun2,
                        Fun3 fun3,
                        Fun4 fun4,
                        Fun5 fun5
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int M6,
                        class Fun1,
                        class Fun2,
                        class Fun3,
                        class Fun4,
                        class Fun5,
                        class Fun6
                        >
                bool erase_if(
                        Fun1 fun1,
                        Fun2 fun2,
                        Fun3 fun3,
                        Fun4 fun4,
                        Fun5 fun5,
                        Fun1 fun6
                        );



                //+-----------------------
                //
                // 检查包含指定项的个数
                //
                //+-----------------------
                template<
                        int M1,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>
                        >
                unsigned counts(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        Prd1 prd = Prd1());

                template<
                        int M1,
                        int M2,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>
                        >
                unsigned counts(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        Prd1 prd = Prd1(),
                        Prd2 prd2 = Prd2()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>
                        >
                unsigned counts(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        Prd1 __ptr = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type >,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type >
                        >
                unsigned counts(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        Prd1 __ptr1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type>
                        >
                unsigned counts(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                        Prd1 __ptr1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5()
                        );

                template<
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int M6,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type >,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type >,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type >,
                        class Prd6 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type >
                        >
                unsigned counts(
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type& value6,
                        Prd1 __ptr1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5(),
                        Prd6 __prd6 = Prd6()
                        );




                //+--------------------------
                //
                // 按条件进行设置某个值
                //
                //+---------------------------
                template<
                        int M,
                        int M1,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>
                        >
                void set(
                        unsigned row,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M>::type& val,
                        Prd1 __prd1 = Prd1()
                        );

                template<
                        int M,
                        int M1,
                        int M2,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>
                        >
                void set(
                        unsigned row,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M>::type& val,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2()
                        );

                template<
                        int M,
                        int M1,
                        int M2,
                        int M3,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>
                        >
                void set(
                        unsigned row,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M>::type& val,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3()
                        );

                template<
                        int M,
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type>
                        >
                void set(
                        unsigned row,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M>::type& val,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4()
                        );

                template<
                        int M,
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type>
                        >
                void set(
                        unsigned row,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M>::type& val,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5()
                        );

                template<
                        int M,
                        int M1,
                        int M2,
                        int M3,
                        int M4,
                        int M5,
                        int M6,
                        class Prd1 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type>,
                        class Prd2 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type>,
                        class Prd3 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type>,
                        class Prd4 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type>,
                        class Prd5 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type>,
                        class Prd6 = MEquals<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type>
                        >
                void set(
                        unsigned row,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type& value6,
                        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M>::type& val,
                        Prd1 __prd1 = Prd1(),
                        Prd2 __prd2 = Prd2(),
                        Prd3 __prd3 = Prd3(),
                        Prd4 __prd4 = Prd4(),
                        Prd5 __prd5 = Prd5(),
                        Prd6 __prd6 = Prd6()
                        );
	};






        //
        //--------------------------------------------------------------------------
        //
        //  实现细节
        //
        //--------------------------------------------------------------------------
        //
        template<class ThreadMode,class T, class...Args>
        MNoSqlDB<ThreadMode,T,Args...>::MNoSqlDB(){
            mData.clear();
        }

        template<class ThreadMode,class T, class...Args>
        MNoSqlDB<ThreadMode,T,Args...>::~MNoSqlDB(){
            mData.clear();
        }

        template<class ThreadMode,class T, class...Args>
        MNoSqlDB<ThreadMode,T,Args...>::MNoSqlDB(const MNoSqlDB<ThreadMode,T,Args...>& other):mData(other.mData)
        {
        }

        template<class ThreadMode,class T, class...Args>
        MNoSqlDB<ThreadMode,T,Args...>::MNoSqlDB(MNoSqlDB<ThreadMode,T,Args...>&& other)
        {
            mData = other.mData;
            other.clear();
        }

        template<class ThreadMode,class T, class...Args>
        MNoSqlDB<ThreadMode,T,Args...>& MNoSqlDB<ThreadMode,T,Args...>::operator=(const MNoSqlDB<ThreadMode,T,Args...>& other)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            mData = other.mData;
            return *this;
        }

        template<class ThreadMode,class T, class...Args>
        MNoSqlDB<ThreadMode,T,Args...>& MNoSqlDB<ThreadMode,T,Args...>::operator=(MNoSqlDB<ThreadMode,T,Args...>&& other)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            mData = other.mData;
            other.clear();
            return *this;
        }

        template<class ThreadMode,class T, class...Args>
        template<class __ThreadMode>
        MNoSqlDB<ThreadMode,T,Args...>::MNoSqlDB(const MNoSqlDB<__ThreadMode,T,Args...>& other)
        {
            mData = other.data();
        }

        template<class ThreadMode,class T, class...Args>
        template<class __ThreadMode>
        MNoSqlDB<ThreadMode,T,Args...>::MNoSqlDB(MNoSqlDB<__ThreadMode,T,Args...>&& other)
        {
            mData = other.data();
            other.clear();
        }

        template<class ThreadMode,class T, class...Args>
        template<class __ThreadMode>
        MNoSqlDB<ThreadMode,T,Args...>& MNoSqlDB<ThreadMode,T,Args...>::operator=(const MNoSqlDB<__ThreadMode,T,Args...>& other)
        {
            mData = other.mData;
            return *this;
        }

        template<class ThreadMode,class T, class...Args>
        template<class __ThreadMode>
        MNoSqlDB<ThreadMode,T,Args...>& MNoSqlDB<ThreadMode,T,Args...>::operator=(MNoSqlDB<__ThreadMode,T,Args...>&& other)
        {
            mData = other.mData;
            other.clear();
            return *this;
        }

        //
        // 后面追加
        //
        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::push_back(const T& value, const Args&...args)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            mData.push_back(TL::make_multtype(value,args...));
        }

        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::push_back(const typename MNoSqlDB<ThreadMode,T,Args...>::value_type& value)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            mData.push_back(value);
        }

        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::push_back(const MNoSqlDB<ThreadMode,T,Args...>& value)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            std::copy(value.begin(),value.end(),std::back_inserter(mData));
        }

        //
        // 指定位置插入 如果位置为-1 数据将追加在后面
        //
        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::insert(unsigned pos, const T& value, const Args&...args)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            if(pos >= mData.size()){
                mData.push_back(TL::make_multtype(value,args...));
            }
            else{
                mData.insert(mData.begin()+pos,make_multtype(value,args...));
            }
        }

        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::insert(unsigned pos, const value_type& value)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            if(pos >= mData.size()){
                mData.push_back(value);
            }
            else{
                mData.insert(mData.begin()+pos,value);
            }
        }

        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::insert(unsigned pos, const MNoSqlDB<ThreadMode,T,Args...>& value)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            if(pos >= mData.size()){
                push_back(value);
            }
            else{
                int i = 0;
                for(auto & v: value){
                    mData.insert(mData.begin()+pos+i++,v);
                }
            }
        }

        //
        // 获取指定位置的数据
        // 如果指定位置操作范围，将抛出std::out_of_range 异常
        //
        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::make_reference(unsigned pos, T& value, Args&... args) const
        {
			const_cast<MNoSqlDB*>(this)->Lock();
			MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode, T, Args...>::UnLock, const_cast<MNoSqlDB*>(this)));
            if(pos > mData.size()+1){
                throw std::out_of_range("MNoSqlDB<T,Args...> Out Of Range");
            }
            typename MNoSqlDB<ThreadMode,T,Args...>::value_type& val = mData[pos];
            TL::tie(val,value,args...);
        }

        //
        // 对特定数据进行解包
        //
        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::make_reference(const typename MNoSqlDB<ThreadMode,T,Args...>::value_type& v, T& value, Args&... args) const{
            TL::tie(v,value,args...);
        }

        //
        // 给定参数生成一个值对象
        //
        template<class ThreadMode,class T, class...Args>
        typename MNoSqlDB<ThreadMode,T,Args...>::value_type MNoSqlDB<ThreadMode,T,Args...>::make_value(const T& value, const Args&... args) const{
            return TL::make_multtype(value,args...);
        }

        //
        // 获取表中的最后一行数据,如果表为空将抛出std::out_of_range异常
        //
        template<class ThreadMode,class T, class...Args>
        typename MNoSqlDB<ThreadMode,T,Args...>::value_type& MNoSqlDB<ThreadMode,T,Args...>::back(){
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            if(mData.empty()){
                throw std::out_of_range("MNoSqlDB<T,Args...>::back() Out of Range Data Is Empty");
            }
            return mData.back();
        }

        template<class ThreadMode,class T, class...Args>
        const typename MNoSqlDB<ThreadMode,T,Args...>::value_type& MNoSqlDB<ThreadMode,T,Args...>::back() const{
            return const_cast<MNoSqlDB<ThreadMode,T,Args...>*>(this)->back();
        }

        template<class ThreadMode,class T, class...Args>
        typename MNoSqlDB<ThreadMode,T,Args...>::value_type& MNoSqlDB<ThreadMode,T,Args...>::front(){
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            if(mData.empty()){
                throw std::out_of_range("MNoSqlDB<T,Args...>::front() Out of Range Data Is Empty");
            }
            return mData.front();
        }

        template<class ThreadMode,class T, class...Args>
        const typename MNoSqlDB<ThreadMode,T,Args...>::value_type& MNoSqlDB<ThreadMode,T,Args...>::front() const{
            return const_cast<MNoSqlDB<ThreadMode,T,Args...>*>(this)->front();
        }

        //
        // 弹出最后一项和最前面一项
        //
        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::pop_back(){
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            if(mData.empty()){
                return;
            }
            mData.pop_back();
        }

        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::pop_front(){
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            if(mData.empty()){
                return;
            }
            mData.erase(mData.begin());
        }

        //
        // 获取行列
        //
        template<class ThreadMode,class T, class...Args>
        unsigned MNoSqlDB<ThreadMode,T,Args...>::colums() const{
            return 1+sizeof...(Args);
        }

        template<class ThreadMode,class T, class...Args>
        unsigned MNoSqlDB<ThreadMode,T,Args...>::rows() const{
            return mData.size();
        }

        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::clear(){
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            mData.clear();
        }

        template<class ThreadMode,class T, class...Args>
        bool MNoSqlDB<ThreadMode,T,Args...>::empty(){
            return mData.empty();
        }

        template<class ThreadMode,class T, class...Args>
        bool MNoSqlDB<ThreadMode,T,Args...>::empty() const{
            return mData.empty();
        }

        template<class ThreadMode,class T, class...Args>
        typename MNoSqlDB<ThreadMode,T,Args...>::iterator MNoSqlDB<ThreadMode,T,Args...>::begin(){
            return mData.begin();
        }

        template<class ThreadMode,class T, class...Args>
        typename MNoSqlDB<ThreadMode,T,Args...>::iterator MNoSqlDB<ThreadMode,T,Args...>::end(){
            return mData.end();
        }

        template<class ThreadMode,class T, class...Args>
        typename MNoSqlDB<ThreadMode,T,Args...>::const_iterator MNoSqlDB<ThreadMode,T,Args...>::begin() const{
            return mData.begin();
        }

        template<class ThreadMode,class T, class...Args>
        typename MNoSqlDB<ThreadMode,T,Args...>::const_iterator MNoSqlDB<ThreadMode,T,Args...>::end() const{
            return mData.end();
        }


        //
        // 打印
        //
        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::print(const char* msg,std::ostream& os)
        {
            os<<msg;
            os<<"[ row = "<<mData.size()<<" , colum = "<<1+sizeof...(Args)<<" ]"<<std::endl;
            for(auto& c : mData){
                TL::print(c,os,"\t");
            }
        }

        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::save(const char* filename,const char* spliter,bool isBinary )
        {
            std::ofstream outFile(filename);
            if(outFile.fail())
                return;
            outFile<<mData.size()<<std::endl;
            for(auto& c : mData){
                TL::print(c,outFile,spliter);
            }

        }

        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::saveAll(const char* filename,const char* spliter)
        {
            std::ofstream outFile(filename);
            if(outFile.fail())
                return;
            for(auto& c : mData){
                TL::print(c,outFile,spliter);
            }
        }

        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::load(const char* filename,const char* spliter,bool isBinary)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            std::ifstream inFile(filename);
            if(inFile.fail()){
                return;
            }
            int num = 0;
            inFile>>num;
            mData.clear();
            if(num <= 0){
                return;
            }
            mData.resize(num);
            for(int i=0;i<num;){
                std::string str;
                std::getline(inFile,str);
                MString str2 = str;
                str2.trim();
                if(str2.empty()){
                    continue;
                }
                value_type val;
                if(TL::FromStr(str,val,spliter)){
                    mData[i] = val;
                    ++i;
                }
            }
        }

        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::loadAll(const char* filename,const char* spliter)
        {
            std::ifstream inFile(filename);
            if(inFile.fail()){
                return;
            }
            mData.clear();
            while(inFile.eof() == false){
                std::string str;
                std::getline(inFile,str);
                MString str2 = str;
                str2.trim();
                if(str2.empty()){
                    continue;
                }
                value_type val;
                if(TL::FromStr(str,val,spliter)){
                    mData.push_back(val);
                }
            }
        }

        template<class ThreadMode,class T, class...Args>
		void MNoSqlDB<ThreadMode, T, Args...>::istream(std::istream& is, const char* spliter)
        {
			Lock();
			MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode, T, Args...>::UnLock, this));
            int num = 0;
            is>>num;     
            mData.clear();
            if(num <= 0){
                return;
            }
            mData.resize(num);
            for(int i=0;i<num;){
                std::string str;
                std::getline(is,str);
                MString str2 = str;
                str2.trim();
                if(str2.empty()){
                    continue;
                }
                value_type val;
                if(TL::FromStr(str,val,spliter)){
                    mData[i++] = val;
                }
            }
        }

		template<class ThreadMode, class T, class...Args>
		void MNoSqlDB<ThreadMode, T, Args...>::istreamAll(std::istream& is, const char* spliter)
		{
			Lock();
			MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode, T, Args...>::UnLock, this));
			mData.clear();
			while (!is.eof()){
				std::string str;
				std::getline(is, str);
				MString str2 = str;
				str2.trim();
				if (str2.empty()){
					continue;
				}
				value_type val;
				if (TL::FromStr(str, val, spliter)){
					mData.push_back(val);
				}
			}
		}

        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::ostream(std::ostream& os,const char* spliter)
        {
            os<<mData.size()<<std::endl;
            for(auto& c : mData){
                TL::print(c,os,spliter);
            }
        }

        template<class ThreadMode,class T, class...Args>
        mj::MMatrixSheet<MString> MNoSqlDB<ThreadMode,T,Args...>::toMatrix() const
        {
			const_cast<MNoSqlDB*>(this)->Lock();
			MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode, T, Args...>::UnLock, const_cast<MNoSqlDB*>(this)));
            mj::MMatrixSheet<MString> out(mData.size(),1+sizeof...(Args));
            int i = 0;
            for(auto& c : mData){
                MString str = TL::ToStr(c,"#");
                std::vector<MString> vs;
                str.split("#",vs);
                out.row(i++) = vs;
            }
            return out;
        }

        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::fromMatrix(const mj::MMatrixSheet<MString>& m)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode, T, Args...>::UnLock, this));
            mData.clear();
            for(int i=0;i<m.rows();++i){
                std::vector<MString> vs = m.row(i);
                MString str = mj::JoinToMstring(vs,"#");
                value_type val;
                TL::FromStr(str,val,"#");
                mData.push_back(val);
            }
        }

        template<class ThreadMode,class T, class...Args>
        template<class V1,int M1,class Fun>
        void MNoSqlDB<ThreadMode,T,Args...>::transform(Fun fun){
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode, T, Args...>::UnLock, this));
            for(auto& c : mData){
                MTypeValue<V1>& ref = TL::reference_at<M1>(c);
                ref = fun(ref.ref());
            }
        }

        template<class ThreadMode,class T, class...Args>
        template<int M1,class Fun>
        void MNoSqlDB<ThreadMode,T,Args...>::transform(Fun fun){
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode, T, Args...>::UnLock, this));
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type __Type;
            for(auto& c : mData){
                MTypeValue<__Type>& ref = TL::reference_at<M1>(c);
                ref = fun(ref.ref());
            }
        }

        template<class ThreadMode,class T, class...Args>
        template<int M1,class Fun>
        void MNoSqlDB<ThreadMode,T,Args...>::for_each(Fun fun)
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type __Type;
            for(auto& c : mData){
                MTypeValue<__Type>& ref = TL::reference_at<M1>(c);
                __Type& val = ref.ref();
                fun(val);
            }
        }


        //
        // 同时操作多列,参数以引用的方式传递,就地修改无需返回
        //
        template<class ThreadMode,class T, class...Args>
        template<int M1,int M2,class Fun>
        void MNoSqlDB<ThreadMode,T,Args...>::mult_transform(Fun fun)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            for(auto& __val : mData){
				Type1& val1 = TL::get<M1>(__val);
				Type2& val2 = TL::get<M2>(__val);
                fun(val1,val2);
            }
        }

        template<class ThreadMode,class T, class...Args>
        template<int M1,int M2,int M3,class Fun>
        void MNoSqlDB<ThreadMode,T,Args...>::mult_transform(Fun fun)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            for(auto& __val : mData){
                Type1& val1 = TL::get<M1>(__val);
				Type2& val2 = TL::get<M2>(__val);
				Type3& val3 = TL::get<M3>(__val);
                fun(val1,val2,val3);
            }
        }

        template<class ThreadMode,class T, class...Args>
        template<int M1,int M2,int M3,int M4,class Fun>
        void MNoSqlDB<ThreadMode,T,Args...>::mult_transform(Fun fun)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            for(auto& __val : mData){
				Type1& val1 = TL::get<M1>(__val);
				Type2& val2 = TL::get<M2>(__val);
				Type3& val3 = TL::get<M3>(__val);
				Type4& val4 = TL::get<M4>(__val);
                fun(val1,val2,val3,val4);
            }
        }

        template<class ThreadMode,class T, class...Args>
        template<int M1,int M2,int M3,int M4,int M5,class Fun>
        void MNoSqlDB<ThreadMode,T,Args...>::mult_transform(Fun fun)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;
            for(auto& __val : mData){
				Type1& val1 = TL::get<M1>(__val);
				Type2& val2 = TL::get<M2>(__val);
				Type3& val3 = TL::get<M3>(__val);
				Type4& val4 = TL::get<M4>(__val);
				Type5& val5 = TL::get<M5>(__val);
                fun(val1,val2,val3,val4,val5);
            }
        }

        template<class ThreadMode,class T, class...Args>
        template<int M1,int M2,int M3,int M4,int M5,int M6,class Fun>
        void MNoSqlDB<ThreadMode,T,Args...>::mult_transform(Fun fun){
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type Type6;
            for(auto& __val : mData){
				Type1& val1 = TL::get<M1>(__val);
				Type2& val2 = TL::get<M2>(__val);
				Type3& val3 = TL::get<M3>(__val);
				Type4& val4 = TL::get<M4>(__val);
				Type5& val5 = TL::get<M5>(__val);
				Type6& val6 = TL::get<M6>(__val);
                fun(val1,val2,val3,val4,val5,val6);
            }
        }

        //
        // 获取指定位置上的数据的引用
        // 如果超出范围将抛出std::out_of_range 异常
        //
        template<class ThreadMode,class T, class...Args>
        template<int N>
        typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N>::type& MNoSqlDB<ThreadMode,T,Args...>::at(unsigned row){
            if(row > mData.size()-1){
                throw std::out_of_range("MNoSqlDB<T,Args...>::at Out of Range");
            }
            return this->get<N>(mData[row]);
        }

        template<class ThreadMode,class T, class...Args>
        template<int N>
        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N>::type& MNoSqlDB<ThreadMode,T,Args...>::at(unsigned row) const{
            return const_cast<MNoSqlDB<ThreadMode,T,Args...>*>(this)->at<V>(row);
        }

        //
        // 如果超出范围将抛出异常
        //
        template<class ThreadMode,class T, class...Args>
        typename MNoSqlDB<ThreadMode,T,Args...>::value_type& MNoSqlDB<ThreadMode,T,Args...>::operator[](unsigned index){
			if (index > mData.size() - 1){
                throw std::out_of_range("MNoSqlDB<T,Args...>::operator[] Out of Range");
            }
            return mData[index];
        }

        template<class ThreadMode,class T, class...Args>
        const typename MNoSqlDB<ThreadMode,T,Args...>::value_type MNoSqlDB<ThreadMode,T,Args...>::operator[](unsigned index) const{
            return const_cast<MNoSqlDB<ThreadMode,T,Args...>*>(this)->operator [](index);
        }

        //
        // 更新指定位置的值
        //
        template<class ThreadMode,class T, class...Args>
        template<int M>
        void MNoSqlDB<ThreadMode,T,Args...>::set(unsigned row,const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M>::type& val)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode, T, Args...>::UnLock, this));
            if(M > sizeof...(Args) || row > mData.size() - 1){
                return;
            }
            TL::reference_at<M>(mData[row]) = val;
        }

        //
        // 获取指定数据的数据
        //
        template<class ThreadMode,class T, class...Args>
        template<int N>
        typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N>::type& MNoSqlDB<ThreadMode,T,Args...>::get(const typename MNoSqlDB<ThreadMode,T,Args...>::value_type& v)
        {
            auto& refout = TL::reference_at<N>(v);
            return refout.ref();
        }

        template<class ThreadMode,class T, class...Args>
        template<int N>
        const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N>::type& MNoSqlDB<ThreadMode,T,Args...>::get(const typename MNoSqlDB<ThreadMode,T,Args...>::value_type& v) const
        {
            const auto& refout = TL::reference_at<N>(v);
            return refout.ref();
        }

        //
        // 删除数据
        //
        template<class ThreadMode,class T, class...Args>
        typename MNoSqlDB<ThreadMode, T, Args...>::iterator MNoSqlDB<ThreadMode, T, Args...>::erase(typename MNoSqlDB<ThreadMode,T,Args...>::iterator __It)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            auto it = mData.erase(__It);
			return it;
        }


        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::erase(
                typename MNoSqlDB<ThreadMode,T,Args...>::iterator __First,
                typename MNoSqlDB<ThreadMode,T,Args...>::iterator __Last
                )
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            mData.erase(__First,__Last);
        }


        template<class ThreadMode,class T, class...Args>
        void MNoSqlDB<ThreadMode,T,Args...>::remove(const value_type& v){
			Lock();
			MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode, T, Args...>::UnLock, this));
			mData.erase(std::find(mData.begin(), mData.end(), v), mData.end());
        }

        //
        // 该操作需要先当前数据进行排序
        //
        template<class ThreadMode,class T, class...Args>
        template<class V1,int M1,class Prd1>
        typename MNoSqlDB<ThreadMode,T,Args...>::iterator MNoSqlDB<ThreadMode,T,Args...>::low_index(const V1& value1,Prd1 prd)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            auto beg = mData.begin();
            for(;beg != mData.end();++beg){
                V1& val = TL::get<M1>(*beg);
                if(prd(val,value1)){
                    return beg;
                }
            }
            return mData.end();
        }

        template<class ThreadMode,class T, class...Args>
        template<class V1,int M1,class Prd1>
        typename MNoSqlDB<ThreadMode,T,Args...>::iterator MNoSqlDB<ThreadMode,T,Args...>::up_index(const V1& value1,Prd1 prd)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            auto beg = mData.begin();
            for(;beg != mData.end();++beg){
                V1& val = TL::get<M1>(*beg);
                if(prd(val,value1) && beg + 1 != mData.end()){
                    val = TL::get<M1>(*(beg+1));
                    if(!prd(val,value1)){
                        return beg+1;
                    }
                }
            }
            return mData.end();
        }

        template<class ThreadMode,class T, class...Args>
        template<int M1,class Prd1>
        typename MNoSqlDB<ThreadMode,T,Args...>::iterator MNoSqlDB<ThreadMode,T,Args...>::low_index(const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,Prd1 prd)
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type __Type;
            return low_index<__Type,M1>(value1,prd);
        }

        template<class ThreadMode,class T, class...Args>
        template<int M1,class Prd1>
        typename MNoSqlDB<ThreadMode,T,Args...>::iterator MNoSqlDB<ThreadMode,T,Args...>::up_index(const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,Prd1 prd)
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type __Type;
            return up_index<__Type,M1>(value1,prd);
        }

        //
        // 拼接出新的表
        //
        template<class ThreadMode,class T, class...Args>
        template<class...Other>
        MNoSqlDB<ThreadMode,T,Args...,Other...> MNoSqlDB<ThreadMode,T,Args...>::cat(const MNoSqlDB<ThreadMode,Other...>& other)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            MNoSqlDB<ThreadMode,T,Args...,Other...> out;
            int num = mData.size()>other.rows() ? other.rows() : mData.size();
            for (int i = 0; i<num; ++i){
                auto val = TL::multtype_cat(mData[i],other[i]);
                out.push_back(val);
            }
            return out;
        }


        //
        // 下面通过多元数据来操作表
        //
        template<class ThreadMode,class T, class...Args>
        template<class V1, int M1, class Prd1>
        typename MNoSqlDB<ThreadMode,T,Args...>::value_type& MNoSqlDB<ThreadMode,T,Args...>::get_item(const V1& value1, Prd1 prd)
        {
			Lock();
			MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode, T, Args...>::UnLock, this));
            for(auto& c : mData){
                V1 val = TL::reference_at<M1>(c);
                if(prd(val,value1)){
                    return c;
                }
            }
            throw std::out_of_range("MNoSqlDB<T,Args...,ThreadMode>::get_item Out Of Range");
        }


        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1, class V2, int M2,
                class Prd1, class Prd2>
        typename MNoSqlDB<ThreadMode,T,Args...>::value_type& MNoSqlDB<ThreadMode,T,Args...>::get_item(
                const V1& value1, const V2& value2,
                Prd1 prd, Prd2 prd2)
        {
			Lock();
			MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode, T, Args...>::UnLock, this));
            for(auto& c : mData){
                V1 val = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                if(prd(val,value1) && prd2(val2,value2)){
                    return c;
                }
            }
            throw std::out_of_range("MNoSqlDB<T,Args...,ThreadMode>::get_item Out Of Range");
        }


        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,class V2, int M2,
                class V3, int M3,class Prd1,
                class Prd2,class Prd3
        >
        typename MNoSqlDB<ThreadMode,T,Args...>::value_type& MNoSqlDB<ThreadMode,T,Args...>::get_item(
                const V1& value1,const V2& value2,
                const V3& value3,Prd1 __ptr,
                Prd2 __prd2,Prd3 __prd3
                )
        {
			Lock();
			MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode, T, Args...>::UnLock, this));
            for(auto& c : mData){
                V1 val = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                V3 val3 = TL::reference_at<M3>(c);
                if(__ptr(val,value1) && __prd2(val2,value2) && __prd3(val3,value3)){
                    return c;
                }
            }
            throw std::out_of_range("MNoSqlDB<T,Args...,ThreadMode>::get_item Out Of Range");
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,
                class V2, int M2,
                class V3, int M3,
                class V4, int M4,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4
                >
        typename MNoSqlDB<ThreadMode,T,Args...>::value_type& MNoSqlDB<ThreadMode,T,Args...>::get_item(
                const V1& value1, const V2& value2,
                const V3& value3, const V4& value4,
                Prd1 __ptr1, Prd2 __prd2,
                Prd3 __prd3, Prd4 __prd4
                )
        {
			Lock();
			MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode, T, Args...>::UnLock, this));
            for(auto& c : mData){
                V1 val = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                V3 val3 = TL::reference_at<M3>(c);
                V4 val4 = TL::reference_at<M4>(c);
                if(__ptr1(val,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                        __prd4(val4,value4)){
                    return c;
                }
            }
            throw std::out_of_range("MNoSqlDB<T,Args...,ThreadMode>::get_item Out Of Range");
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,
                class V2, int M2,
                class V3, int M3,
                class V4, int M4,
                class V5, int M5,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5
                >
        typename MNoSqlDB<ThreadMode,T,Args...>::value_type& MNoSqlDB<ThreadMode,T,Args...>::get_item(
                const V1& value1, const V2& value2,
                const V3& value3, const V4& value4,
                const V5& value5,
                Prd1 __ptr1, Prd2 __prd2,
                Prd3 __prd3, Prd4 __prd4,
                Prd5 __prd5
                )
        {
			Lock();
			MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode, T, Args...>::UnLock, this));
            for(auto& c : mData){
                V1 val = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                V3 val3 = TL::reference_at<M3>(c);
                V4 val4 = TL::reference_at<M4>(c);
                V5 val5 = TL::reference_at<M5>(c);
                if(__ptr1(val,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                        __prd4(val4,value4) && __prd5(val5,value5)){
                    return c;
                }
            }
            throw std::out_of_range("MNoSqlDB<T,Args...,ThreadMode>::get_item Out Of Range");
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,
                class V2, int M2,
                class V3, int M3,
                class V4, int M4,
                class V5, int M5,
                class V6, int M6,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class Prd6
                >
        typename MNoSqlDB<ThreadMode,T,Args...>::value_type& MNoSqlDB<ThreadMode,T,Args...>::get_item(
                const V1& value1, const V2& value2,
                const V3& value3, const V4& value4,
                const V5& value5, const V6& value6,
                Prd1 __ptr1, Prd2 __prd2,
                Prd3 __prd3, Prd4 __prd4,
                Prd5 __prd5, Prd6 __prd6
                )
        {
			Lock();
			MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode, T, Args...>::UnLock, this));
            for(auto& c : mData){
                V1 val = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                V3 val3 = TL::reference_at<M3>(c);
                V4 val4 = TL::reference_at<M4>(c);
                V5 val5 = TL::reference_at<M5>(c);
                V6 val6 = TL::reference_at<M6>(c);
                if(__ptr1(val,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                        __prd4(val4,value4) && __prd5(val5,value5)&&__prd6(val6,value6)){
                    return c;
                }
            }
            throw std::out_of_range("MNoSqlDB<T,Args...,ThreadMode>::get_item Out Of Range");
        }

        //
        // 通过指定条件，搜索一个字表
        //
        template<class ThreadMode,class T, class...Args>
        template<class V1, int M1, class Prd1>
        MNoSqlDB<ThreadMode,T,Args...> MNoSqlDB<ThreadMode,T,Args...>::get_subdb(const V1& value1, Prd1 prd )
        {
            MNoSqlDB<ThreadMode,T,Args...> out;
            for(auto& c : mData){
                V1 val = TL::reference_at<M1>(c);
                if(prd(val,value1)){
                    out.push_back(c);
                }
            }
            return out;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1, class V2, int M2,
                class Prd1, class Prd2
                >
        MNoSqlDB<ThreadMode,T,Args...> MNoSqlDB<ThreadMode,T,Args...>::get_subdb(
                const V1& value1, const V2& value2,
                Prd1 prd, Prd2 prd2)
        {
            MNoSqlDB<ThreadMode,T,Args...> out;
            for(auto& c : mData){
                V1 val = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                if(prd(val,value1) && prd2(val2,value2)){
                    out.push_back(c);
                }
            }
            return out;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,class V2, int M2,
                class V3, int M3,class Prd1,
                class Prd2,class Prd3
                >
        MNoSqlDB<ThreadMode,T,Args...> MNoSqlDB<ThreadMode,T,Args...>::get_subdb(
                const V1& value1,const V2& value2,
                const V3& value3,Prd1 __ptr,
                Prd2 __prd2,Prd3 __prd3
                )
        {
            MNoSqlDB<ThreadMode,T,Args...> out;
            for(auto& c : mData){
                V1 val = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                V3 val3 = TL::reference_at<M3>(c);
                if(__ptr(val,value1) && __prd2(val2,value2) && __prd3(val3,value3)){
                    out.push_back(c);
                }
            }
            return out;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,
                class V2, int M2,
                class V3, int M3,
                class V4, int M4,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4
                >
        MNoSqlDB<ThreadMode,T,Args...> MNoSqlDB<ThreadMode,T,Args...>::get_subdb(
                const V1& value1, const V2& value2,
                const V3& value3, const V4& value4,
                Prd1 __ptr1, Prd2 __prd2,
                Prd3 __prd3, Prd4 __prd4
                )
        {
            MNoSqlDB<ThreadMode,T,Args...> out;
            for(auto& c : mData){
                V1 val = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                V3 val3 = TL::reference_at<M3>(c);
                V4 val4 = TL::reference_at<M4>(c);
                if(__ptr1(val,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                        __prd4(val4,value4)){
                    out.push_back(c);
                }
            }
            return out;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,
                class V2, int M2,
                class V3, int M3,
                class V4, int M4,
                class V5, int M5,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5
                >
        MNoSqlDB<ThreadMode,T,Args...> MNoSqlDB<ThreadMode,T,Args...>::get_subdb(
                const V1& value1, const V2& value2,
                const V3& value3, const V4& value4,
                const V5& value5,
                Prd1 __ptr1, Prd2 __prd2,
                Prd3 __prd3, Prd4 __prd4,
                Prd5 __prd5
                )
        {
            MNoSqlDB<ThreadMode,T,Args...> out;
            for(auto& c : mData){
                V1 val = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                V3 val3 = TL::reference_at<M3>(c);
                V4 val4 = TL::reference_at<M4>(c);
                V5 val5 = TL::reference_at<M5>(c);
                if(__ptr1(val,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                        __prd4(val4,value4) && __prd5(val5,value5)){
                    out.push_back(c);
                }
            }
            return out;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,
                class V2, int M2,
                class V3, int M3,
                class V4, int M4,
                class V5, int M5,
                class V6, int M6,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class Prd6
                >
        MNoSqlDB<ThreadMode,T,Args...> MNoSqlDB<ThreadMode,T,Args...>::get_subdb(
                const V1& value1, const V2& value2,
                const V3& value3, const V4& value4,
                const V5& value5, const V6& value6,
                Prd1 __ptr1, Prd2 __prd2,
                Prd3 __prd3, Prd4 __prd4,
                Prd5 __prd5, Prd6 __prd6
                )
        {
            MNoSqlDB<ThreadMode,T,Args...> out;
            for(auto& c : mData){
                V1 val = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                V3 val3 = TL::reference_at<M3>(c);
                V4 val4 = TL::reference_at<M4>(c);
                V5 val5 = TL::reference_at<M5>(c);
                V6 val6 = TL::reference_at<M6>(c);
                if(__ptr1(val,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                        __prd4(val4,value4) && __prd5(val5,value5)&&__prd6(val6,value6)){
                    out.push_back(c);
                }
            }
            return out;
        }

        //
        // 获取指定列
        //
        template<class ThreadMode,class T, class...Args>
        template<class V,int N>
        MTypeWrap<std::vector<V>> MNoSqlDB<ThreadMode, T, Args...>::colum() const
        {
            std::vector<V> out;
            out.resize(mData.size());
            int index = 0;
            for(auto& c : mData){
                V val = TL::reference_at<N>(c);
                out[index++] = val;
            }
            return out;
        }

        template<class ThreadMode,class T, class...Args>
        template<int N>
        MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N>::type>> MNoSqlDB<ThreadMode, T, Args...>::colum() const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N>::type __Type;
            return colum<__Type,N>();
        }


		//
		// 获取满足条件的列
		// 其实就是一列的表
		//
		template<class ThreadMode, class T, class...Args>
		template<
			int N, int IF_M1,
			class Prd1
		>
		MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type>> MNoSqlDB<ThreadMode, T, Args...>::colum_if(
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type& value1,
			Prd1 __ptr1) const
		{
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type OutType;
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type Type1;

			MTypeWrap<std::vector<OutType>> res;
			for (auto& c : mData){
				Type1 val1 = TL::reference_at<IF_M1>(c);
				OutType val = TL::reference_at<N>(c);
				if (__ptr1(val1, value1)){
					res.push_back(val);
				}
			}
			return res;
		}



		template<class ThreadMode, class T, class...Args>
		template<
			int N, int IF_M1, int IF_M2,
			class Prd1,
			class Prd2
		>
		MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type>> MNoSqlDB<ThreadMode, T, Args...>::colum_if(
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type& value1,
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type& value2,
			Prd1 __ptr1,
			Prd2 __prd2) const
		{
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type OutType;

			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type Type1;
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type Type2;

			MTypeWrap<std::vector<OutType>> res;
			for (auto& c : mData){
				Type1 val1 = TL::reference_at<IF_M1>(c);
				Type2 val2 = TL::reference_at<IF_M2>(c);
				OutType val = TL::reference_at<N>(c);
				if (__ptr1(val1, value1) && __prd2(val2, value2)){
					res.push_back(val);
				}
			}
			return res;
		}


		template<class ThreadMode, class T, class...Args>
		template<
			int N, int IF_M1, int IF_M2, int IF_M3,
			class Prd1,
			class Prd2,
			class Prd3
		>
		MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type>> MNoSqlDB<ThreadMode, T, Args...>::colum_if(
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type& value1,
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type& value2,
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M3>::type& value3,
			Prd1 __ptr1,
			Prd2 __prd2,
			Prd3 __prd3) const
		{
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type OutType;

			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type Type1;
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type Type2;
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M3>::type Type3;

			MTypeWrap<std::vector<OutType>> res;
			for (auto& c : mData){
				Type1 val1 = TL::reference_at<IF_M1>(c);
				Type2 val2 = TL::reference_at<IF_M2>(c);
				Type3 val3 = TL::reference_at<IF_M3>(c);
				OutType val = TL::reference_at<N>(c);
				if (__ptr1(val1, value1) && __prd2(val2, value2) && __prd3(val3, value3)){
					res.push_back(val);
				}
			}
			return res;
		}



		template<class ThreadMode, class T, class...Args>
		template<
			int N, int IF_M1, int IF_M2, int IF_M3, int IF_M4,
			class Prd1,
			class Prd2,
			class Prd3,
			class Prd4
		>
		MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type>> MNoSqlDB<ThreadMode, T, Args...>::colum_if(
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type& value1,
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type& value2,
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M3>::type& value3,
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M4>::type& value4,
			Prd1 __ptr1,
			Prd2 __prd2,
			Prd3 __prd3,
			Prd4 __prd4) const
		{
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type OutType;

			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type Type1;
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type Type2;
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M3>::type Type3;
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M4>::type Type4;

			MTypeWrap<std::vector<OutType>> res;
			for (auto& c : mData){
				Type1 val1 = TL::reference_at<IF_M1>(c);
				Type2 val2 = TL::reference_at<IF_M2>(c);
				Type3 val3 = TL::reference_at<IF_M3>(c);
				Type4 val4 = TL::reference_at<IF_M4>(c);
				OutType val = TL::reference_at<N>(c);
				if (__ptr1(val1, value1) && __prd2(val2, value2) && __prd3(val3, value3) && __prd4(val4, value4)){
					res.push_back(val);
				}
			}
			return res;
		}



		template<class ThreadMode, class T, class...Args>
		template<
			int N, int IF_M1, int IF_M2, int IF_M3, int IF_M4, int IF_M5,
			class Prd1,
			class Prd2,
			class Prd3,
			class Prd4,
			class Prd5
		>
		MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type>> MNoSqlDB<ThreadMode, T, Args...>::colum_if(
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type& value1,
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type& value2,
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M3>::type& value3,
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M4>::type& value4,
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M5>::type& value5,
			Prd1 __ptr1,
			Prd2 __prd2,
			Prd3 __prd3,
			Prd4 __prd4,
			Prd5 __prd5) const
		{
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type OutType;

			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type Type1;
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type Type2;
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M3>::type Type3;
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M4>::type Type4;
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M5>::type Type5;

			MTypeWrap<std::vector<OutType>> res;
			for (auto& c : mData){
				Type1 val1 = TL::reference_at<IF_M1>(c);
				Type2 val2 = TL::reference_at<IF_M2>(c);
				Type3 val3 = TL::reference_at<IF_M3>(c);
				Type4 val4 = TL::reference_at<IF_M4>(c);
				Type5 val5 = TL::reference_at<IF_M5>(c);
				OutType val = TL::reference_at<N>(c);
				if (__ptr1(val1, value1) && __prd2(val2, value2) && __prd3(val3, value3) && __prd4(val4, value4) && __prd5(val5, value5)){
					res.push_back(val);
				}
			}
			return res;
		}



		template<class ThreadMode, class T, class...Args>
		template<
			int N, int IF_M1, int IF_M2, int IF_M3, int IF_M4, int IF_M5, int IF_M6,
			class Prd1,
			class Prd2,
			class Prd3,
			class Prd4,
			class Prd5,
			class Prd6
		>
		MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type>> MNoSqlDB<ThreadMode, T, Args...>::colum_if(
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type& value1,
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type& value2,
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M3>::type& value3,
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M4>::type& value4,
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M5>::type& value5,
			const typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M6>::type& value6,
			Prd1 __ptr,
			Prd2 __prd2,
			Prd3 __prd3,
			Prd4 __prd4,
			Prd5 __prd5,
			Prd6 __prd6) const
		{
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type OutType;
			
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M1>::type Type1;
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M2>::type Type2;
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M3>::type Type3;
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M4>::type Type4;
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M5>::type Type5;
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, IF_M6>::type Type6;

			MTypeWrap<std::vector<OutType>> res;
			for (auto& c : mData){
				Type1 val1 = TL::reference_at<IF_M1>(c);
				Type2 val2 = TL::reference_at<IF_M2>(c);
				Type3 val3 = TL::reference_at<IF_M3>(c);
				Type4 val4 = TL::reference_at<IF_M4>(c);
				Type5 val5 = TL::reference_at<IF_M5>(c);
				Type6 val6 = TL::reference_at<IF_M6>(c);
				OutType val = TL::reference_at<N>(c);
				if (__ptr1(val1, value1) && __prd2(val2, value2) && __prd3(val3, value3) && __prd4(val4, value4) && __prd5(val5, value5) && __prd6(val6, value6)){
					res.push_back(val);
				}
			}
			return res;
		}

        //
        // 通过指定类型创建一张表
        //
        template<class ThreadMode,class T, class...Args>
        template<
                class V1,int M1,
                class V2,int M2,
                class __ThreadMode
                >
        MNoSqlDB<__ThreadMode,V1,V2> MNoSqlDB<ThreadMode,T,Args...>::create_db() const
        {
            MNoSqlDB<__ThreadMode,V1,V2> out;
            for(auto& c : mData){
                V1 val1 = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                out.push_back(val1,val2);
            }
            return out;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1,int M1,
                class V2,int M2,
                class V3,int M3,
                class __ThreadMode
                >
        MNoSqlDB<__ThreadMode,V1,V2,V3> MNoSqlDB<ThreadMode,T,Args...>::create_db() const
        {
            MNoSqlDB<__ThreadMode,V1,V2,V3> out;
            for(auto& c : mData){
                V1 val1 = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                V3 val3 = TL::reference_at<M3>(c);
                out.push_back(val1,val2,val3);
            }
            return out;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1,int M1,
                class V2,int M2,
                class V3,int M3,
                class V4,int M4,
                class __ThreadMode
                >
        MNoSqlDB<__ThreadMode,V1,V2,V3,V4> MNoSqlDB<ThreadMode,T,Args...>::create_db() const
        {
            MNoSqlDB<__ThreadMode,V1,V2,V3,V4> out;
            for(auto& c : mData){
                V1 val1 = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                V3 val3 = TL::reference_at<M3>(c);
                V4 val4 = TL::reference_at<M4>(c);
                out.push_back(val1,val2,val3,val4);
            }
            return out;
        }


        template<class ThreadMode,class T, class...Args>
        template<
                class V1,int M1,
                class V2,int M2,
                class V3,int M3,
                class V4,int M4,
                class V5,int M5,
                class __ThreadMode
                >
        MNoSqlDB<__ThreadMode,V1,V2,V3,V4,V5> MNoSqlDB<ThreadMode,T,Args...>::create_db() const
        {
            MNoSqlDB<__ThreadMode,V1,V2,V3,V4,V5> out;
            for(auto& c : mData){
                V1 val1 = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                V3 val3 = TL::reference_at<M3>(c);
                V4 val4 = TL::reference_at<M4>(c);
                V5 val5 = TL::reference_at<M5>(c);
                out.push_back(val1,val2,val3,val4,val5);
            }
            return out;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1,int M1,
                class V2,int M2,
                class V3,int M3,
                class V4,int M4,
                class V5,int M5,
                class V6,int M6,
                class __ThreadMode
                >
        MNoSqlDB<__ThreadMode,V1,V2,V3,V4,V5,V6> MNoSqlDB<ThreadMode,T,Args...>::create_db() const{
            MNoSqlDB<__ThreadMode,V1,V2,V3,V4,V5,V6> out;
            for(auto& c : mData){
                V1 val1 = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                V3 val3 = TL::reference_at<M3>(c);
                V4 val4 = TL::reference_at<M4>(c);
                V5 val5 = TL::reference_at<M5>(c);
                V6 val6 = TL::reference_at<M6>(c);
                out.push_back(val1,val2,val3,val4,val5,val6);
            }
            return out;
        }

        //
        // 按照给定条件进行排序
        //
        template<class ThreadMode,class T, class...Args>
        template<class V1,int M1,class Cmp>
        void MNoSqlDB<ThreadMode,T,Args...>::sort(Cmp __cmp)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            std::sort(mData.begin(),mData.end(),[&](const value_type& val1,const value_type& val2){
                V1 __val1 = TL::reference_at<M1>(val1);
                V1 __val2 = TL::reference_at<M1>(val2);
                return __cmp(__val1,__val2);
            });
        }

        template<class ThreadMode,class T, class...Args>
        template<class V1,int M1,class Cmp>
        void MNoSqlDB<ThreadMode,T,Args...>::sort(
                typename MNoSqlDB<ThreadMode,T,Args...>::iterator pos1,
                typename MNoSqlDB<ThreadMode,T,Args...>::iterator pos2,
                Cmp __cmp)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            std::sort(pos1,pos2,[&](const value_type& val1,const value_type& val2){
                V1 __val1 = TL::reference_at<M1>(val1);
                V1 __val2 = TL::reference_at<M1>(val2);
                return __cmp(__val1,__val2);
            });
        }

        //
        // 简化的排序
        //
        template<class ThreadMode,class T, class...Args>
        template<int M1,class Cmp>
        void MNoSqlDB<ThreadMode,T,Args...>::sort(Cmp __cmp)
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            sort<Type1,M1>(__cmp);
        }

        template<class ThreadMode,class T, class...Args>
        template<int M1,class Cmp>
        void MNoSqlDB<ThreadMode,T,Args...>::sort(
                typename MNoSqlDB<ThreadMode,T,Args...>::iterator pos1,
                typename MNoSqlDB<ThreadMode,T,Args...>::iterator pos2,
                Cmp __cmp)
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            sort<Type1,M1>(pos1,pos2,__cmp);
        }


        //
        // 指定列的元素唯一化
        //
        template<class ThreadMode,class T, class...Args>
        template<int N,
                 class Cmp1,
                 class Cmp2
                 >
        void MNoSqlDB<ThreadMode,T,Args...>::unique(Cmp1 prd1,Cmp2 prd2)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N>::type V1;
            std::sort(mData.begin(),mData.end(),[&](const value_type& val1,const value_type& val2){
                V1 __val1 = TL::reference_at<N>(val1);
                V1 __val2 = TL::reference_at<N>(val2);
                return prd1(__val1,__val2);
            });
            mData.erase(std::unique(mData.begin(),mData.end(),[&](const value_type& val1,const value_type& val2){
                V1 __val1 = TL::reference_at<N>(val1);
                V1 __val2 = TL::reference_at<N>(val2);
                return prd2(__val1,__val2);
            }),mData.end());
        }



        //
        // 按照给定条件进行查找
        //
        template<class ThreadMode,class T, class...Args>
        template<class V1, int M1, class Prd1>
        unsigned MNoSqlDB<ThreadMode,T,Args...>::find(const V1& value1, Prd1 prd )
        {
            unsigned index = 0;
            for(auto& c : mData){
                V1 val = TL::reference_at<M1>(c);
                if(prd(val,value1)){
                    return index;
                }
                ++index;
            }
            return -1;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1, class V2, int M2,
                class Prd1, class Prd2>
        unsigned MNoSqlDB<ThreadMode,T,Args...>::find(
                const V1& value1, const V2& value2,
                Prd1 prd, Prd2 prd2)
        {
            unsigned index = 0;
            for(auto& c : mData){
                V1 val = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                if(prd(val,value1) && prd2(val2,value2)){
                    return index;
                }
                ++index;
            }
            return -1;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,class V2, int M2,
                class V3, int M3,class Prd1,
                class Prd2,class Prd3
                >
        unsigned MNoSqlDB<ThreadMode,T,Args...>::find(
                const V1& value1,const V2& value2,
                const V3& value3,Prd1 __ptr,
                Prd2 __prd2 ,Prd3 __prd3
                )
        {
            unsigned index = 0;
            for(auto& c : mData){
                V1 val = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                V3 val3 = TL::reference_at<M3>(c);
                if(__ptr(val,value1) && __prd2(val2,value2) && __prd3(val3,value3)){
                    return index;
                }
                ++index;
            }
            return -1;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,
                class V2, int M2,
                class V3, int M3,
                class V4, int M4,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4
                >
        unsigned MNoSqlDB<ThreadMode,T,Args...>::find(
                const V1& value1, const V2& value2,
                const V3& value3, const V4& value4,
                Prd1 __ptr1, Prd2 __prd2,
                Prd3 __prd3, Prd4 __prd4
                )
        {
            unsigned index = 0;
            for(auto& c : mData){
                V1 val = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                V3 val3 = TL::reference_at<M3>(c);
                V4 val4 = TL::reference_at<M4>(c);
                if(__ptr1(val,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                        __prd4(val4,value4)){
                    return index;
                }
                ++index;
            }
            return -1;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,
                class V2, int M2,
                class V3, int M3,
                class V4, int M4,
                class V5, int M5,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5
                >
        unsigned MNoSqlDB<ThreadMode,T,Args...>::find(
                const V1& value1, const V2& value2,
                const V3& value3, const V4& value4,
                const V5& value5,
                Prd1 __ptr1, Prd2 __prd2,
                Prd3 __prd3, Prd4 __prd4,
                Prd5 __prd5
                )
        {
            unsigned index = 0;
            for(auto& c : mData){
                V1 val = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                V3 val3 = TL::reference_at<M3>(c);
                V4 val4 = TL::reference_at<M4>(c);
                V5 val5 = TL::reference_at<M5>(c);

                if(__ptr1(val,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                        __prd4(val4,value4) && __prd5(val5,value5)){
                    return index;
                }
                ++index;
            }
            return -1;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,
                class V2, int M2,
                class V3, int M3,
                class V4, int M4,
                class V5, int M5,
                class V6, int M6,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class Prd6
                >
        unsigned MNoSqlDB<ThreadMode,T,Args...>::find(
                const V1& value1, const V2& value2,
                const V3& value3, const V4& value4,
                const V5& value5, const V6& value6,
                Prd1 __ptr1, Prd2 __prd2,
                Prd3 __prd3, Prd4 __prd4,
                Prd5 __prd5, Prd6 __prd6
                )
        {
            unsigned index = 0;
            for(auto& c : mData){
                V1 val = TL::reference_at<M1>(c);
                V2 val2 = TL::reference_at<M2>(c);
                V3 val3 = TL::reference_at<M3>(c);
                V4 val4 = TL::reference_at<M4>(c);
                V5 val5 = TL::reference_at<M5>(c);
                V6 val6 = TL::reference_at<M6>(c);

                if(__ptr1(val,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                        __prd4(val4,value4) && __prd5(val5,value5) && __prd6(val6,value6)){
                    return index;
                }
                ++index;
            }
            return -1;
        }

        //
        // 按照给定条件进行更新数据
        //
        template<class ThreadMode,class T, class...Args>
        template<class V1, int M1, class Prd1>
        void MNoSqlDB<ThreadMode,T,Args...>::update(const V1& value1, const value_type& val,Prd1 prd)
        {
            unsigned index = find<V1,M1>(value1,prd);
            if(index != -1){
                mData[index] = val;
            }
            else{
                push_back(val);
            }
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1, class V2, int M2,
                class Prd1, class Prd2>
        void MNoSqlDB<ThreadMode,T,Args...>::update(
                const V1& value1, const V2& value2,
                const value_type& val,
                Prd1 prd , Prd2 prd2)
        {
            unsigned index = find<V1,M1,V2,M2>(value1,value2,prd,prd2);
            if(index != -1){
                mData[index] = val;
            }
            else{
                push_back(val);
            }
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,class V2, int M2,
                class V3, int M3,class Prd1,
                class Prd2,class Prd3
                >
        void MNoSqlDB<ThreadMode,T,Args...>::update(
                const V1& value1,const V2& value2,
                const V3& value3,
                const value_type& val,
                Prd1 __ptr1,
                Prd2 __prd2,Prd3 __prd3
                )
        {
            unsigned index = find<V1,M1,V2,M2,V3,M3>(value1,value2,value3,
                                               __ptr1,__prd2,__prd3);
            if(index != -1){
                mData[index] = val;
            }
            else{
                push_back(val);
            }
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,
                class V2, int M2,
                class V3, int M3,
                class V4, int M4,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4
                >
        void MNoSqlDB<ThreadMode,T,Args...>::update(
                const V1& value1, const V2& value2,
                const V3& value3, const V4& value4,
                const value_type& val,
                Prd1 __ptr1, Prd2 __prd2,
                Prd3 __prd3, Prd4 __prd4
                )
        {
            unsigned index = find<V1,M1,V2,M2,V3,M3,V4,M4>(
                        value1,value2,value3,value4,
                        __ptr1,__prd2,__prd3,__prd4);
            if(index != -1){
                mData[index] = val;
            }
            else{
                push_back(val);
            }
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,
                class V2, int M2,
                class V3, int M3,
                class V4, int M4,
                class V5, int M5,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5
                >
        void MNoSqlDB<ThreadMode,T,Args...>::update(
                const V1& value1, const V2& value2,
                const V3& value3, const V4& value4,
                const V5& value5,
                const value_type& val,
                Prd1 __ptr1, Prd2 __prd2 ,
                Prd3 __prd3, Prd4 __prd4 ,
                Prd5 __prd5
                )
        {
            unsigned index = find<V1,M1,V2,M2,V3,M3,V4,M4,V5,M5>(
                        value1,value2,value3,value4,value5,
                        __ptr1,__prd2,__prd3,__prd4,__prd5);
            if(index != -1){
                mData[index] = val;
            }
            else{
                push_back(val);
            }
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,
                class V2, int M2,
                class V3, int M3,
                class V4, int M4,
                class V5, int M5,
                class V6, int M6,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class Prd6
                >
        void MNoSqlDB<ThreadMode,T,Args...>::update(
                const V1& value1, const V2& value2,
                const V3& value3, const V4& value4,
                const V5& value5, const V6& value6,
                const value_type& val,
                Prd1 __ptr1, Prd2 __prd2,
                Prd3 __prd3, Prd4 __prd4 ,
                Prd5 __prd5, Prd6 __prd6
                )
        {
            unsigned index = find<V1,M1,V2,M2,V3,M3,V4,M4,V5,M5,V6,M6>(
                        value1,value2,value3,value4,value5,value6,
                        __ptr1,__prd2,__prd3,__prd4,__prd5,__prd6);
            if(index != -1){
                mData[index] = val;
            }
            else{
                push_back(val);
            }
        }


        //
        // 按照给定条件进行删除
        //
        template<class ThreadMode,class T, class...Args>
        template<class V1, int M1, class Prd1>
        bool MNoSqlDB<ThreadMode,T,Args...>::eraseall(const V1& value1, Prd1 prd )
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            bool isErase = false;
            iterator  __it = mData.begin();
            while(__it != mData.end()){
                V1 val = TL::reference_at<M1>(*__it);
                if(prd(val,value1)){
                    isErase = true;
					__it = mData.erase(__it);
                }
                else{
                    ++__it;
                }
            }
            return isErase;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1, class V2, int M2,
                class Prd1, class Prd2>
        bool MNoSqlDB<ThreadMode,T,Args...>::eraseall(
                const V1& value1, const V2& value2,
                Prd1 prd, Prd2 prd2)
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            bool isErase = false;
            iterator  __it = mData.begin();
            while(__it != mData.end()){
                V1 val = TL::reference_at<M1>(*__it);
                V2 val2 = TL::reference_at<M2>(*__it);
                if(prd(val,value1) && prd2(val2,value2)){
                    isErase = true;
                    __it = mData.erase(__it);
                }
                else{
                    ++__it;
                }
            }
            return isErase;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,class V2, int M2,
                class V3, int M3,class Prd1,
                class Prd2,class Prd3
                >
        bool MNoSqlDB<ThreadMode,T,Args...>::eraseall(
                const V1& value1,const V2& value2,
                const V3& value3,Prd1 __ptr,
                Prd2 __prd2 ,Prd3 __prd3
                )
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            bool isErase = false;
            iterator  __it = mData.begin();
            while(__it != mData.end()){
                V1 val = TL::reference_at<M1>(*__it);
                V2 val2 = TL::reference_at<M2>(*__it);
                V3 val3 = TL::reference_at<M3>(*__it);
                if(__ptr(val,value1) && __prd2(val2,value2) && __prd3(val3,value3)){
                    isErase = true;
					__it = mData.erase(__it);
                }
                else{
                    ++__it;
                }
            }
            return isErase;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,
                class V2, int M2,
                class V3, int M3,
                class V4, int M4,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4
                >
        bool MNoSqlDB<ThreadMode,T,Args...>::eraseall(
                const V1& value1, const V2& value2,
                const V3& value3, const V4& value4,
                Prd1 __ptr1, Prd2 __prd2,
                Prd3 __prd3, Prd4 __prd4
                )
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            bool isErase = false;
            iterator  __it = mData.begin();
            while(__it != mData.end()){
                V1 val = TL::reference_at<M1>(*__it);
                V2 val2 = TL::reference_at<M2>(*__it);
                V3 val3 = TL::reference_at<M3>(*__it);
                V4 val4 = TL::reference_at<M4>(*__it);
                if(__ptr1(val,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                        __prd4(val4,value4)){
                    isErase = true;
					__it = mData.erase(__it);
                }
                else{
                    ++__it;
                }
            }
            return isErase;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,
                class V2, int M2,
                class V3, int M3,
                class V4, int M4,
                class V5, int M5,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5
                >
        bool MNoSqlDB<ThreadMode,T,Args...>::eraseall(
                const V1& value1, const V2& value2,
                const V3& value3, const V4& value4,
                const V5& value5,
                Prd1 __ptr1, Prd2 __prd2,
                Prd3 __prd3, Prd4 __prd4,
                Prd5 __prd5
                )
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            bool isErase = false;
            iterator  __it = mData.begin();
            while(__it != mData.end()){
                V1 val = TL::reference_at<M1>(*__it);
                V2 val2 = TL::reference_at<M2>(*__it);
                V3 val3 = TL::reference_at<M3>(*__it);
                V4 val4 = TL::reference_at<M4>(*__it);
                V5 val5 = TL::reference_at<M5>(*__it);

                if(__ptr1(val,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                        __prd4(val4,value4) && __prd5(val5,value5)){
                    isErase = true;
					__it = mData.erase(__it);
                }
                else{
                    ++__it;
                }
            }
            return isErase;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                class V1, int M1,
                class V2, int M2,
                class V3, int M3,
                class V4, int M4,
                class V5, int M5,
                class V6, int M6,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class Prd6
                >
        bool MNoSqlDB<ThreadMode,T,Args...>::eraseall(
                const V1& value1, const V2& value2,
                const V3& value3, const V4& value4,
                const V5& value5, const V6& value6,
                Prd1 __ptr1, Prd2 __prd2,
                Prd3 __prd3, Prd4 __prd4,
                Prd5 __prd5, Prd6 __prd6
                )
        {
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            bool isErase = false;
            iterator  __it = mData.begin();
            while(__it != mData.end()){
                V1 val = TL::reference_at<M1>(*__it);
                V2 val2 = TL::reference_at<M2>(*__it);
                V3 val3 = TL::reference_at<M3>(*__it);
                V4 val4 = TL::reference_at<M4>(*__it);
                V5 val5 = TL::reference_at<M5>(*__it);
                V6 val6 = TL::reference_at<M6>(*__it);

                if(__ptr1(val,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                        __prd4(val4,value4) && __prd5(val5,value5) && __prd6(val6,value6)){
                    isErase = true;
					__it = mData.erase(__it);
                }
                else{
                    ++__it;
                }
            }
            return isErase;
        }


        //
        // -------------------------------------------------------------
        // 简化使用时的操作
        // 该操作将会自动推导出相应的类型
        // -------------------------------------------------------------
        //
        template<class ThreadMode,class T, class...Args>
        template<int M1,
                 class Prd1
                 >
        typename MNoSqlDB<ThreadMode,T,Args...>::value_type& MNoSqlDB<ThreadMode,T,Args...>::item(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                Prd1 prd
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            return get_item<Type1,M1>(value1,prd);
        }


        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                class Prd1,
                class Prd2
                >
        typename MNoSqlDB<ThreadMode,T,Args...>::value_type& MNoSqlDB<ThreadMode,T,Args...>::item(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                Prd1 prd , Prd2 prd2
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            return get_item<Type1,M1,Type2,M2>(value1,value2,prd,prd2);
        }


        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                class Prd1,
                class Prd2,
                class Prd3
        >
        typename MNoSqlDB<ThreadMode,T,Args...>::value_type& MNoSqlDB<ThreadMode,T,Args...>::item(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                Prd1 __ptr ,
                Prd2 __prd2,
                Prd3 __prd3
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            return get_item<Type1,M1,Type2,M2,Type3,M3>(value1,value2,value3,__ptr,__prd2,__prd3);
        }


        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4
                >
        typename MNoSqlDB<ThreadMode,T,Args...>::value_type& MNoSqlDB<ThreadMode,T,Args...>::item(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                Prd1 __ptr1 = Prd1(),
                Prd2 __prd2 = Prd2(),
                Prd3 __prd3 = Prd3(),
                Prd4 __prd4 = Prd4()
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            return get_item<Type1,M1,Type2,M2,Type3,M3,Type4,M4>(
                        value1,value2,value3,value4,
                        __ptr1,__prd2,__prd3,__prd4);
        }


        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5
                >
        typename MNoSqlDB<ThreadMode,T,Args...>::value_type& MNoSqlDB<ThreadMode,T,Args...>::item(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                Prd1 __ptr1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;
            return get_item<Type1,M1,Type2,M2,Type3,M3,Type4,M4,Type5,M5>(
                        value1,value2,value3,value4,value5,
                        __ptr1,__prd2,__prd3,__prd4,__prd5);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int M6,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class Prd6
                >
        typename MNoSqlDB<ThreadMode,T,Args...>::value_type& MNoSqlDB<ThreadMode,T,Args...>::item(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type& value6,
                Prd1 __ptr1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5,
                Prd6 __prd6
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type Type6;
			return get_item<Type1, M1, Type2, M2, Type3, M3, Type4, M4, Type5, M5, Type6,M6>(
                        value1,value2,value3,value4,value5,value6,
                        __ptr1,__prd2,__prd3,__prd4,__prd5,__prd6);
        }

        //
        // 通过指定条件，搜索一个子表
        //
        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                class Prd1
                >
        MNoSqlDB<ThreadMode,T,Args...> MNoSqlDB<ThreadMode,T,Args...>::subdb(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                 Prd1 prd
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            return get_subdb<Type1,M1>
                    (value1,
                     prd);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                class Prd1,
                class Prd2
                >
        MNoSqlDB<ThreadMode,T,Args...> MNoSqlDB<ThreadMode,T,Args...>::subdb(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                Prd1 prd,
                Prd2 prd2
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            return get_subdb<Type1,M1,Type2,M2>
                    (value1,value2,
                     prd,prd2);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                class Prd1,
                class Prd2,
                class Prd3
                >
        MNoSqlDB<ThreadMode,T,Args...> MNoSqlDB<ThreadMode,T,Args...>::subdb(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                Prd1 __ptr,
                Prd2 __prd2,
                Prd3 __prd3
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            return get_subdb<Type1,M1,Type2,M2,Type3,M3>
                    (value1,value2,value3,
                     __ptr,__prd2,__prd3);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4
                >
        MNoSqlDB<ThreadMode,T,Args...> MNoSqlDB<ThreadMode,T,Args...>::subdb(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                Prd1 __ptr1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            return get_subdb<Type1,M1,Type2,M2,Type3,M3,Type4,M4>
                    (value1,value2,value3,value4,
                     __ptr1,__prd2,__prd3,__prd4);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5
                >
        MNoSqlDB<ThreadMode,T,Args...> MNoSqlDB<ThreadMode,T,Args...>::subdb(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                Prd1 __ptr1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;
            return get_subdb<Type1,M1,Type2,M2,Type3,M3,Type4,M4,Type5,M5>
                    (value1,value2,value3,value4,value5,
                     __ptr1,__prd2,__prd3,__prd4,__prd5);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int M6,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class Prd6
                >
        MNoSqlDB<ThreadMode,T,Args...> MNoSqlDB<ThreadMode,T,Args...>::subdb(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type& value6,
                Prd1 __ptr1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5,
                Prd6 __prd6
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type Type6;
            return get_subdb<Type1,M1,Type2,M2,Type3,M3,Type4,M4,Type5,M5,Type6,M6>
                    (value1,value2,value3,value4,value5,value6,
                     __ptr1,__prd2,__prd3,__prd4,__prd5,__prd6);
        }


        //
        // 通过指定类型创建一张表
        //
        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db() const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            return create_db<Type1,M1,Type2,M2,__ThreadMode>();
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db() const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            return create_db<Type1,M1,Type2,M2,Type3,M3,__ThreadMode>();
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db() const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            return create_db<Type1,M1,Type2,M2,Type3,M3,Type4,M4,__ThreadMode>();
        }


        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db() const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;
            return create_db<Type1,M1,Type2,M2,Type3,M3,Type4,M4,Type5,M5,__ThreadMode>();
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int M6,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db() const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type Type6;
            return create_db<Type1,M1,Type2,M2,Type3,M3,Type4,M4,Type5,M5,Type6,M6,__ThreadMode>();
        }


        //
        // 通过指定类型创建一张表 以一个参数为条件
        //
        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int N1,
                class Prd1,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                Prd1 __prd1
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;

            MNoSqlDB<__ThreadMode,MType1,MType2> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                if(__prd1(val1,value1))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    outdb.push_back(__val1,__val2);
                }
            }
            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int N1,
                class Prd1,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                Prd1 __prd1
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                if(__prd1(val1,value1))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    outdb.push_back(__val1,__val2,__val3);
                }
            }
            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int N1,
                class Prd1,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                Prd1 __prd1
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type MType4;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3,MType4> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                if(__prd1(val1,value1))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    MType4& __val4 = TL::get<M4>(c);
                    outdb.push_back(__val1,__val2,__val3,__val4);
                }
            }
            return outdb;
        }


        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int N1,
                class Prd1,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                Prd1 __prd1
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type MType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type MType5;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3,MType4,MType5> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                if(__prd1(val1,value1))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    MType4& __val4 = TL::get<M4>(c);
                    MType5& __val5 = TL::get<M5>(c);
                    outdb.push_back(__val1,__val2,__val3,__val4,__val5);
                }
            }
            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int M6,
                int N1,
                class Prd1,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                Prd1 __prd1
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type MType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type MType5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type MType6;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3,MType4,MType5,MType6> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                if(__prd1(val1,value1))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    MType4& __val4 = TL::get<M4>(c);
                    MType5& __val5 = TL::get<M5>(c);
                    MType6& __val6 = TL::get<M6>(c);
                    outdb.push_back(__val1,__val2,__val3,__val4,__val5,__val6);
                }
            }
            return outdb;
        }

        //
        // 通过指定类型创建一张表 以两个参数为条件
        //
        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int N1,
                int N2,
                class Prd1,
                class Prd2,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                Prd1 __prd1,
                Prd2 __prd2
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;

            MNoSqlDB<__ThreadMode,MType1,MType2> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                if(__prd1(val1,value1) && __prd2(val2,value2))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    outdb.push_back(__val1,__val2);
                }
            }
            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int N1,
                int N2,
                class Prd1,
                class Prd2,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                Prd1 __prd1,
                Prd2 __prd2
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                if(__prd1(val1,value1) && __prd2(val2,value2))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    outdb.push_back(__val1,__val2,__val3);
                }
            }
            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int N1,
                int N2,
                class Prd1,
                class Prd2,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                Prd1 __prd1,
                Prd2 __prd2
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type MType4;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3,MType4> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                if(__prd1(val1,value1) && __prd2(val2,value2))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    MType4& __val4 = TL::get<M4>(c);
                    outdb.push_back(__val1,__val2,__val3,__val4);
                }
            }
            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int N1,
                int N2,
                class Prd1,
                class Prd2,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                Prd1 __prd1,
                Prd2 __prd2
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type MType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type MType5;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3,MType4,MType5> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                if(__prd1(val1,value1) && __prd2(val2,value2))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    MType4& __val4 = TL::get<M4>(c);
                    MType5& __val5 = TL::get<M5>(c);
                    outdb.push_back(__val1,__val2,__val3,__val4,__val5);
                }
            }
            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int M6,
                int N1,
                int N2,
                class Prd1,
                class Prd2,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                Prd1 __prd1,
                Prd2 __prd2
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type MType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type MType5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type MType6;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3,MType4,MType5,MType6> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                if(__prd1(val1,value1) && __prd2(val2,value2))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    MType4& __val4 = TL::get<M4>(c);
                    MType5& __val5 = TL::get<M5>(c);
                    MType6& __val6 = TL::get<M6>(c);
                    outdb.push_back(__val1,__val2,__val3,__val4,__val5,__val6);
                }
            }
            return outdb;
        }

        //
        // 通过指定类型创建一张表 以三个参数为条件
        //
        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int N1,
                int N2,
                int N3,
                class Prd1,
                class Prd2,
                class Prd3,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;

            MNoSqlDB<__ThreadMode,MType1,MType2> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    outdb.push_back(__val1,__val2);
                }
            }
            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int N1,
                int N2,
                int N3,
                class Prd1,
                class Prd2,
                class Prd3,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    outdb.push_back(__val1,__val2,__val3);
                }
            }
            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int N1,
                int N2,
                int N3,
                class Prd1,
                class Prd2,
                class Prd3,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type MType4;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3,MType4> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    MType4& __val4 = TL::get<M4>(c);
                    outdb.push_back(__val1,__val2,__val3,__val4);
                }
            }
            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int N1,
                int N2,
                int N3,
                class Prd1,
                class Prd2,
                class Prd3,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type MType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type MType5;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3,MType4,MType5> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    MType4& __val4 = TL::get<M4>(c);
                    MType5& __val5 = TL::get<M5>(c);
                    outdb.push_back(__val1,__val2,__val3,__val4,__val5);
                }
            }
            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int M6,
                int N1,
                int N2,
                int N3,
                class Prd1,
                class Prd2,
                class Prd3,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type MType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type MType5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type MType6;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3,MType4,MType5,MType6> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    MType4& __val4 = TL::get<M4>(c);
                    MType5& __val5 = TL::get<M5>(c);
                    MType6& __val6 = TL::get<M6>(c);
                    outdb.push_back(__val1,__val2,__val3,__val4,__val5,__val6);
                }
            }
            return outdb;
        }

        //
        // 通过指定类型创建一张表 以四个参数为条件
        //
        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int N1,
                int N2,
                int N3,
                int N4,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type NType4;

            MNoSqlDB<__ThreadMode,MType1,MType2> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                NType4& val4 = TL::get<N4>(c);

                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3) &&
                        __prd4(val4,value4))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    outdb.push_back(__val1,__val2);
                }
            }
            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int N1,
                int N2,
                int N3,
                int N4,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type NType4;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                NType4& val4 = TL::get<N4>(c);

                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3) &&
                        __prd4(val4,value4))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    outdb.push_back(__val1,__val2,__val3);
                }
            }
            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int N1,
                int N2,
                int N3,
                int N4,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type MType4;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type NType4;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3,MType4> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                NType4& val4 = TL::get<N4>(c);

                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3) &&
                        __prd4(val4,value4))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    MType4& __val4 = TL::get<M4>(c);
                    outdb.push_back(__val1,__val2,__val3,__val4);
                }
            }
            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int N1,
                int N2,
                int N3,
                int N4,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type MType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type MType5;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type NType4;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3,MType4,MType5> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                NType4& val4 = TL::get<N4>(c);

                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3) &&
                        __prd4(val4,value4))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    MType4& __val4 = TL::get<M4>(c);
                    MType5& __val5 = TL::get<M5>(c);
                    outdb.push_back(__val1,__val2,__val3,__val4,__val5);
                }
            }
            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int M6,
                int N1,
                int N2,
                int N3,
                int N4,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type MType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type MType5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type MType6;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type NType4;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3,MType4,MType5,MType6> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                NType4& val4 = TL::get<N4>(c);

                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3) &&
                        __prd4(val4,value4))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    MType4& __val4 = TL::get<M4>(c);
                    MType5& __val5 = TL::get<M5>(c);
                    MType6& __val6 = TL::get<M6>(c);
                    outdb.push_back(__val1,__val2,__val3,__val4,__val5,__val6);
                }
            }
            return outdb;
        }

        //
        // 通过指定类型创建一张表 以五个参数为条件
        //
        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int N1,
                int N2,
                int N3,
                int N4,
                int N5,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type NType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type NType5;

            MNoSqlDB<__ThreadMode,MType1,MType2> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                NType4& val4 = TL::get<N4>(c);
                NType5& val5 = TL::get<N5>(c);

                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3) &&
                        __prd4(val4,value4) && __prd5(val5,value5))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    outdb.push_back(__val1,__val2);
                }
            }

            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int N1,
                int N2,
                int N3,
                int N4,
                int N5,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type NType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type NType5;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                NType4& val4 = TL::get<N4>(c);
                NType5& val5 = TL::get<N5>(c);

                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3) &&
                        __prd4(val4,value4) && __prd5(val5,value5))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    outdb.push_back(__val1,__val2,__val3);
                }
            }

            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int N1,
                int N2,
                int N3,
                int N4,
                int N5,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type MType4;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type NType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type NType5;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3,MType4> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                NType4& val4 = TL::get<N4>(c);
                NType5& val5 = TL::get<N5>(c);

                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3) &&
                        __prd4(val4,value4) && __prd5(val5,value5))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    MType4& __val4 = TL::get<M4>(c);

                    outdb.push_back(__val1,__val2,__val3,__val4);
                }
            }

            return outdb;
        }


        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int N1,
                int N2,
                int N3,
                int N4,
                int N5,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type MType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type MType5;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type NType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type NType5;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3,MType4,MType5> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                NType4& val4 = TL::get<N4>(c);
                NType5& val5 = TL::get<N5>(c);

                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3) &&
                        __prd4(val4,value4) && __prd5(val5,value5))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    MType4& __val4 = TL::get<M4>(c);
                    MType5& __val5 = TL::get<M5>(c);
                    outdb.push_back(__val1,__val2,__val3,__val4,__val5);
                }
            }

            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int M6,
                int N1,
                int N2,
                int N3,
                int N4,
                int N5,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type MType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type MType5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type MType6;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type NType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type NType5;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3,MType4,MType5,MType6> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                NType4& val4 = TL::get<N4>(c);
                NType5& val5 = TL::get<N5>(c);

                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3) &&
                        __prd4(val4,value4) && __prd5(val5,value5))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    MType4& __val4 = TL::get<M4>(c);
                    MType5& __val5 = TL::get<M5>(c);
                    MType6& __val6 = TL::get<M6>(c);
                    outdb.push_back(__val1,__val2,__val3,__val4,__val5,__val6);
                }
            }
            return outdb;
        }

        //
        // 通过指定类型创建一张表 以六个参数为条件
        //
        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int N1,
                int N2,
                int N3,
                int N4,
                int N5,
                int N6,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class Prd6,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type& value6,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5,
                Prd6 __prd6
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type NType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type NType5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type NType6;

            MNoSqlDB<__ThreadMode,MType1,MType2> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                NType4& val4 = TL::get<N4>(c);
                NType5& val5 = TL::get<N5>(c);
                NType6& val6 = TL::get<N6>(c);

                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3) &&
                        __prd4(val4,value4) && __prd5(val5,value5) && __prd6(val6,value6))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    outdb.push_back(__val1,__val2);
                }
            }

            return outdb;
        }


        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int N1,
                int N2,
                int N3,
                int N4,
                int N5,
                int N6,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class Prd6,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type& value6,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5,
                Prd6 __prd6
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type NType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type NType5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type NType6;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                NType4& val4 = TL::get<N4>(c);
                NType5& val5 = TL::get<N5>(c);
                NType6& val6 = TL::get<N6>(c);

                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3) &&
                        __prd4(val4,value4) && __prd5(val5,value5) && __prd6(val6,value6))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);

                    outdb.push_back(__val1,__val2,__val3);
                }
            }

            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int N1,
                int N2,
                int N3,
                int N4,
                int N5,
                int N6,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class Prd6,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type& value6,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5,
                Prd6 __prd6
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type MType4;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type NType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type NType5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type NType6;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3,MType4> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                NType4& val4 = TL::get<N4>(c);
                NType5& val5 = TL::get<N5>(c);
                NType6& val6 = TL::get<N6>(c);

                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3) &&
                        __prd4(val4,value4) && __prd5(val5,value5) && __prd6(val6,value6))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    MType4& __val4 = TL::get<M4>(c);

                    outdb.push_back(__val1,__val2,__val3,__val4);
                }
            }

            return outdb;
        }


        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int N1,
                int N2,
                int N3,
                int N4,
                int N5,
                int N6,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class Prd6,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type& value6,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5,
                Prd6 __prd6
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type MType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type MType5;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type NType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type NType5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type NType6;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3,MType4,MType5> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                NType4& val4 = TL::get<N4>(c);
                NType5& val5 = TL::get<N5>(c);
                NType6& val6 = TL::get<N6>(c);

                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3) &&
                        __prd4(val4,value4) && __prd5(val5,value5) && __prd6(val6,value6))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    MType4& __val4 = TL::get<M4>(c);
                    MType5& __val5 = TL::get<M5>(c);

                    outdb.push_back(__val1,__val2,__val3,__val4,__val5);
                }
            }

            return outdb;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int M6,
                int N1,
                int N2,
                int N3,
                int N4,
                int N5,
                int N6,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class Prd6,
                class __ThreadMode
                >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type
        > MNoSqlDB<ThreadMode,T,Args...>::colum_db(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type& value5,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type& value6,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5,
                Prd6 __prd6
                ) const
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type MType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type MType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type MType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type MType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type MType5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type MType6;

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N1>::type NType1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N2>::type NType2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N3>::type NType3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N4>::type NType4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N5>::type NType5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,N6>::type NType6;

            MNoSqlDB<__ThreadMode,MType1,MType2,MType3,MType4,MType5,MType6> outdb;
            for(auto& c : mData){
                NType1& val1 = TL::get<N1>(c);
                NType2& val2 = TL::get<N2>(c);
                NType3& val3 = TL::get<N3>(c);
                NType4& val4 = TL::get<N4>(c);
                NType5& val5 = TL::get<N5>(c);
                NType6& val6 = TL::get<N6>(c);



                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3) &&
                        __prd4(val4,value4) && __prd5(val5,value5) && __prd6(val6,value6))
                {
                    MType1& __val1 = TL::get<M1>(c);
                    MType2& __val2 = TL::get<M2>(c);
                    MType3& __val3 = TL::get<M3>(c);
                    MType4& __val4 = TL::get<M4>(c);
                    MType5& __val5 = TL::get<M5>(c);
                    MType6& __val6 = TL::get<M6>(c);

                    outdb.push_back(__val1,__val2,__val3,__val4,__val5,__val6);
                }
            }

            return outdb;
        }


        ///
        /// 根据指定差值方式进行差值得到结果
        ///
        template<class ThreadMode,class T, class...Args>
        template<int In,
                 int Out1,
                 class InterpOperator
                 >
        MTypeWrap<
            std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type>
        > MNoSqlDB<ThreadMode,T,Args...>::interp(
                const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type TypeIn;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type TypeOut;
            MNoSqlDB<ThreadMode,TypeIn,TypeOut> temp = this->colum_db<In,Out>();
            MTypeWrap<std::vector<TypeOut>> outVec;
            if(temp.empty())
                return outVec;

            temp.unique<0>();

            MTypeWrap<std::vector<TypeIn>> xs = temp.colum<0>();
            MTypeWrap<std::vector<TypeOut>> ys = temp.colum<1>();
            outVec = InterpOperator()(xs.toType(),ys.toType(),vals.toType());
            return outVec;
        }



        template<class ThreadMode,class T, class...Args>
        template<int In,
                 int Out1,
                 int Out2,
                 class InterpOperator,
                 class __ThreadMode
                 >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type
        > MNoSqlDB<ThreadMode,T,Args...>::interp(
                const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type TypeIn;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type TypeOut1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type TypeOut2;
            MNoSqlDB<ThreadMode,TypeIn,TypeOut1,TypeOut2> temp = colum_db<In,Out1,Out2>();
            MNoSqlDB<__ThreadMode,TypeOut1,TypeOut2> outMat;
            if(temp.empty())
                return outMat;

            temp.unique<0>();

            MTypeWrap<std::vector<TypeIn>> xs = temp.colum<0>();
            MTypeWrap<std::vector<TypeOut1>> ys1 = temp.colum<1>();
            MTypeWrap<std::vector<TypeOut2>> ys2 = temp.colum<2>();

			MTypeWrap<std::vector<TypeOut1>> ys11 = InterpOperator()(xs.toType(), ys1.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut2>> ys21 = InterpOperator()(xs.toType(), ys2.toType(), vals.toType());

            for(int i=0;i<ys11.size();++i){
                outMat.push_back(ys11.unsafe_at(i),ys21.unsafe_at(i));
            }

            return outMat;
        }


        template<class ThreadMode,class T, class...Args>
        template<int In,
                 int Out1,
                 int Out2,
                 int Out3,
                 class InterpOperator,
                 class __ThreadMode
                 >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type
        > MNoSqlDB<ThreadMode,T,Args...>::interp(
                const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type TypeIn;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type TypeOut1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type TypeOut2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type TypeOut3;
            MNoSqlDB<ThreadMode,TypeIn,TypeOut1,TypeOut2,TypeOut3> temp = colum_db<In,Out1,Out2,Out3>();
            MNoSqlDB<__ThreadMode,TypeOut1,TypeOut2,TypeOut3> outMat;
            if(temp.empty())
                return outMat;

            temp.unique<0>();
            MTypeWrap<std::vector<TypeIn>> xs = temp.colum<0>();
            MTypeWrap<std::vector<TypeOut1>> ys1 = temp.colum<1>();
            MTypeWrap<std::vector<TypeOut2>> ys2 = temp.colum<2>();
            MTypeWrap<std::vector<TypeOut3>> ys3 = temp.colum<3>();

			MTypeWrap<std::vector<TypeOut1>> ys11 = InterpOperator()(xs.toType(), ys1.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut2>> ys21 = InterpOperator()(xs.toType(), ys2.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut3>> ys31 = InterpOperator()(xs.toType(), ys3.toType(), vals.toType());

            for(int i = 0;i<ys11.size();++i){
                outMat.push_back(ys11.unsafe_at(i),ys21.unsafe_at(i),ys31.unsafe_at(i));
            }
            return outMat;
        }


        template<class ThreadMode,class T, class...Args>
        template<int In,
                 int Out1,
                 int Out2,
                 int Out3,
                 int Out4,
                 class InterpOperator,
                 class __ThreadMode
                 >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out4>::type
        > MNoSqlDB<ThreadMode,T,Args...>::interp(
                const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type TypeIn;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type TypeOut1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type TypeOut2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type TypeOut3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out4>::type TypeOut4;
            MNoSqlDB<ThreadMode,TypeIn,TypeOut1,TypeOut2,TypeOut3,TypeOut4> temp = colum_db<In,Out1,Out2,Out3,Out4>();
            MNoSqlDB<__ThreadMode,TypeOut1,TypeOut2,TypeOut3,TypeOut4> outMat;
            if(temp.empty())
                return outMat;

            temp.unique<0>();

            MTypeWrap<std::vector<TypeIn>> xs = temp.colum<0>();
            MTypeWrap<std::vector<TypeOut1>> ys1 = temp.colum<1>();
            MTypeWrap<std::vector<TypeOut2>> ys2 = temp.colum<2>();
            MTypeWrap<std::vector<TypeOut3>> ys3 = temp.colum<3>();
            MTypeWrap<std::vector<TypeOut4>> ys4 = temp.colum<4>();

			MTypeWrap<std::vector<TypeOut1>> ys11 = InterpOperator()(xs.toType(), ys1.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut2>> ys21 = InterpOperator()(xs.toType(), ys2.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut3>> ys31 = InterpOperator()(xs.toType(), ys3.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut4>> ys41 = InterpOperator()(xs.toType(), ys4.toType(), vals.toType());

            for(int i = 0;i<ys11.size();++i){
                outMat.push_back(ys11.unsafe_at(i),ys21.unsafe_at(i),ys31.unsafe_at(i),ys41.unsafe_at(i));
            }

            return outMat;
        }



        template<class ThreadMode,class T, class...Args>
        template<int In,
                 int Out1,
                 int Out2,
                 int Out3,
                 int Out4,
                 int Out5,
                 class InterpOperator,
                 class __ThreadMode
                 >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out4>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out5>::type
        > MNoSqlDB<ThreadMode,T,Args...>::interp(
                const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type TypeIn;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type TypeOut1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type TypeOut2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type TypeOut3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out4>::type TypeOut4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out5>::type TypeOut5;
            MNoSqlDB<ThreadMode,TypeIn,TypeOut1,TypeOut2,TypeOut3,TypeOut4,TypeOut5> temp =
                    colum_db<In,Out1,Out2,Out3,Out4,Out5>();

            MNoSqlDB<__ThreadMode,TypeOut1,TypeOut2,TypeOut3,TypeOut4,TypeOut5> outMat;
            if(temp.empty())
                return outMat;

            temp.unique<0>();

            MTypeWrap<std::vector<TypeIn>> xs = temp.colum<0>();
            MTypeWrap<std::vector<TypeOut1>> ys1 = temp.colum<1>();
            MTypeWrap<std::vector<TypeOut2>> ys2 = temp.colum<2>();
            MTypeWrap<std::vector<TypeOut3>> ys3 = temp.colum<3>();
            MTypeWrap<std::vector<TypeOut4>> ys4 = temp.colum<4>();
            MTypeWrap<std::vector<TypeOut5>> ys5 = temp.colum<5>();


			MTypeWrap<std::vector<TypeOut1>> ys11 = InterpOperator()(xs.toType(), ys1.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut2>> ys21 = InterpOperator()(xs.toType(), ys2.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut3>> ys31 = InterpOperator()(xs.toType(), ys3.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut4>> ys41 = InterpOperator()(xs.toType(), ys4.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut5>> ys51 = InterpOperator()(xs.toType(), ys5.toType(), vals.toType());

            for(int i = 0;i<ys11.size();++i){
                outMat.push_back(
					ys11.unsafe_at(i),
					ys21.unsafe_at(i),
					ys31.unsafe_at(i),
					ys41.unsafe_at(i),
					ys51.unsafe_at(i));
            }

            return outMat;
        }


        template<class ThreadMode,class T, class...Args>
        template<int In,
                 int Out1,
                 int Out2,
                 int Out3,
                 int Out4,
                 int Out5,
                 int Out6,
                 class InterpOperator,
                 class __ThreadMode
                 >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out4>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out5>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out6>::type
        > MNoSqlDB<ThreadMode,T,Args...>::interp(
                const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type TypeIn;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type TypeOut1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type TypeOut2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type TypeOut3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out4>::type TypeOut4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out5>::type TypeOut5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out6>::type TypeOut6;
            MNoSqlDB<ThreadMode,TypeIn,TypeOut1,TypeOut2,TypeOut3,TypeOut4,TypeOut5> temp =
                    colum_db<In,Out1,Out2,Out3,Out4,Out5>();
            MNoSqlDB<ThreadMode,TypeIn,TypeOut6> temp2 = colum_db<In,Out6>();
            MNoSqlDB<__ThreadMode,TypeOut1,TypeOut2,TypeOut3,TypeOut4,TypeOut5,TypeOut6> outMat;
            if(temp.empty() || temp2.empty())
                return outMat;

            temp.unique<0>();
            temp2.unique<0>();

            MTypeWrap<std::vector<TypeIn>> xs = temp.colum<0>();
            MTypeWrap<std::vector<TypeOut1>> ys1 = temp.colum<1>();
            MTypeWrap<std::vector<TypeOut2>> ys2 = temp.colum<2>();
            MTypeWrap<std::vector<TypeOut3>> ys3 = temp.colum<3>();
            MTypeWrap<std::vector<TypeOut4>> ys4 = temp.colum<4>();
            MTypeWrap<std::vector<TypeOut5>> ys5 = temp.colum<5>();
            MTypeWrap<std::vector<TypeOut6>> ys6 = temp2.colum<1>();


			MTypeWrap<std::vector<TypeOut1>> ys11 = InterpOperator()(xs.toType(), ys1.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut2>> ys21 = InterpOperator()(xs.toType(), ys2.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut3>> ys31 = InterpOperator()(xs.toType(), ys3.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut4>> ys41 = InterpOperator()(xs.toType(), ys4.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut5>> ys51 = InterpOperator()(xs.toType(), ys5.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut6>> ys61 = InterpOperator()(xs.toType(), ys6.toType(), vals.toType());

            for(int i = 0;i<ys11.size();++i){
                outMat.push_back(
                            ys11.unsafe_at(i),
                            ys21.unsafe_at(i),
                            ys31.unsafe_at(i),
                            ys41.unsafe_at(i),
                            ys51.unsafe_at(i),
                            ys61.unsafe_at(i)
                            );
            }

            return outMat;
        }


        //
        // 只能在内部进行插值
        //
        template<class ThreadMode,class T, class...Args>
        template<int In,
                 int Out1
                 >
        MTypeWrap<
            std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type>
        > MNoSqlDB<ThreadMode,T,Args...>::interp(
                const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals,
                const MInterpType& op
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type TypeIn;
			typedef typename mjTL::index_type<mjTL::MTypeList<T, Args...>, Out1>::type TypeOut;
			MNoSqlDB<ThreadMode, TypeIn, TypeOut> temp = colum_db<In, Out1>();
            MTypeWrap<std::vector<TypeOut>> outVec;
            if(temp.empty())
                return outVec;

            temp.unique<0>();
            MTypeWrap<std::vector<TypeIn>> xs = temp.colum<0>();
            MTypeWrap<std::vector<TypeOut>> ys = temp.colum<1>();
            outVec = op(xs.toType(),ys.toType(),vals.toType());
            return outVec;
        }



        template<class ThreadMode,class T, class...Args>
        template<int In,
                 int Out1,
                 int Out2,
                 class __ThreadMode
                 >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type
        > MNoSqlDB<ThreadMode,T,Args...>::interp(
                const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals,
                const MInterpType& op
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type TypeIn;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type TypeOut1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type TypeOut2;
            MNoSqlDB<ThreadMode,TypeIn,TypeOut1,TypeOut2> temp = colum_db<In,Out1,Out2>();
            MNoSqlDB<__ThreadMode,TypeOut1,TypeOut2> outMat;
            if(temp.empty())
                return outMat;

            temp.unique<0>();

            MTypeWrap<std::vector<TypeIn>> xs = temp.colum<0>();
            MTypeWrap<std::vector<TypeOut1>> ys1 = temp.colum<1>();
            MTypeWrap<std::vector<TypeOut2>> ys2 = temp.colum<2>();

			MTypeWrap<std::vector<TypeOut1>> ys11 = op(xs.toType(), ys1.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut2>> ys22 = op(xs.toType(), ys2.toType(), vals.toType());

			for (int i = 0; i<ys11.size(); ++i){
				outMat.push_back(ys11.unsafe_at(i), ys22.unsafe_at(i));
            }

            return outMat;
        }


        template<class ThreadMode,class T, class...Args>
        template<int In,
                 int Out1,
                 int Out2,
                 int Out3,
                 class __ThreadMode
                 >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type
        > MNoSqlDB<ThreadMode,T,Args...>::interp(
                const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals,
                const MInterpType& op
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type TypeIn;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type TypeOut1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type TypeOut2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type TypeOut3;
            MNoSqlDB<ThreadMode,TypeIn,TypeOut1,TypeOut2,TypeOut3> temp = colum_db<In,Out1,Out2,Out3>();
            MNoSqlDB<__ThreadMode,TypeOut1,TypeOut2,TypeOut3> outMat;
            if(temp.empty())
                return outMat;

            temp.unique<0>();
            MTypeWrap<std::vector<TypeIn>> xs = temp.colum<0>();
			MTypeWrap<std::vector<TypeOut1>> ys1 = temp.colum<1>();
            MTypeWrap<std::vector<TypeOut2>> ys2 = temp.colum<2>();
            MTypeWrap<std::vector<TypeOut3>> ys3 = temp.colum<3>();

			MTypeWrap<std::vector<TypeOut1>> ys11 = op(xs.toType(), ys1.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut2>> ys21 = op(xs.toType(), ys2.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut3>> ys31 = op(xs.toType(), ys3.toType(), vals.toType());

            for(int i = 0;i<ys11.size();++i){
                outMat.push_back(ys11[i],ys21[i],ys31[i]);
            }
            return outMat;
        }




        template<class ThreadMode,class T, class...Args>
        template<int In,
                 int Out1,
                 int Out2,
                 int Out3,
                 int Out4,
                 class __ThreadMode
                 >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out4>::type
        > MNoSqlDB<ThreadMode,T,Args...>::interp(
                const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals,
                const MInterpType& op
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type TypeIn;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type TypeOut1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type TypeOut2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type TypeOut3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out4>::type TypeOut4;
            MNoSqlDB<ThreadMode,TypeIn,TypeOut1,TypeOut2,TypeOut3,TypeOut4> temp = colum_db<In,Out1,Out2,Out3,Out4>();
            MNoSqlDB<__ThreadMode,TypeOut1,TypeOut2,TypeOut3,TypeOut4> outMat;
            if(temp.empty())
                return outMat;

            temp.unique<0>();

            MTypeWrap<std::vector<TypeIn>> xs = temp.colum<0>();
            MTypeWrap<std::vector<TypeOut1>> ys1 = temp.colum<1>();
            MTypeWrap<std::vector<TypeOut2>> ys2 = temp.colum<2>();
            MTypeWrap<std::vector<TypeOut3>> ys3 = temp.colum<3>();
            MTypeWrap<std::vector<TypeOut4>> ys4 = temp.colum<4>();

			MTypeWrap<std::vector<TypeOut1>> ys11 = op(xs.toType(), ys1.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut2>> ys21 = op(xs.toType(), ys2.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut3>> ys31 = op(xs.toType(), ys3.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut4>> ys41 = op(xs.toType(), ys4.toType(), vals.toType());

            for(int i = 0;i<ys11.size();++i){
                outMat.push_back(ys11.unsafe_at(i),ys21.unsafe_at(i),ys31.unsafe_at(i),ys41.unsafe_at(i));
            }

            return outMat;
        }



        template<class ThreadMode,class T, class...Args>
        template<int In,
                 int Out1,
                 int Out2,
                 int Out3,
                 int Out4,
                 int Out5,
                 class __ThreadMode
                 >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out4>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out5>::type
        > MNoSqlDB<ThreadMode,T,Args...>::interp(
                const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals,
                const MInterpType& op
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type TypeIn;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type TypeOut1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type TypeOut2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type TypeOut3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out4>::type TypeOut4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out5>::type TypeOut5;
            MNoSqlDB<ThreadMode,TypeIn,TypeOut1,TypeOut2,TypeOut3,TypeOut4,TypeOut5> temp =
                    colum_db<In,Out1,Out2,Out3,Out4,Out5>();

            MNoSqlDB<__ThreadMode,TypeOut1,TypeOut2,TypeOut3,TypeOut4,TypeOut5> outMat;
            if(temp.empty())
                return outMat;

            temp.unique<0>();

            MTypeWrap<std::vector<TypeIn>> xs = temp.colum<0>();
            MTypeWrap<std::vector<TypeOut1>> ys1 = temp.colum<1>();
            MTypeWrap<std::vector<TypeOut2>> ys2 = temp.colum<2>();
            MTypeWrap<std::vector<TypeOut3>> ys3 = temp.colum<3>();
            MTypeWrap<std::vector<TypeOut4>> ys4 = temp.colum<4>();
            MTypeWrap<std::vector<TypeOut5>> ys5 = temp.colum<5>();


			MTypeWrap<std::vector<TypeOut1>> ys11 = op(xs.toType(), ys1.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut2>> ys21 = op(xs.toType(), ys2.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut3>> ys31 = op(xs.toType(), ys3.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut4>> ys41 = op(xs.toType(), ys4.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut5>> ys51 = op(xs.toType(), ys5.toType(), vals.toType());

            for(int i = 0;i<ys11.size();++i){
                outMat.push_back(
					ys11.unsafe_at(i),
					ys21.unsafe_at(i),
					ys31.unsafe_at(i),
					ys41.unsafe_at(i),
					ys51.unsafe_at(i));
            }

            return outMat;
        }



        template<class ThreadMode,class T, class...Args>
        template<int In,
                 int Out1,
                 int Out2,
                 int Out3,
                 int Out4,
                 int Out5,
                 int Out6,
                 class __ThreadMode
                 >
        MNoSqlDB<
            __ThreadMode,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out4>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out5>::type,
            typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out6>::type
        > MNoSqlDB<ThreadMode,T,Args...>::interp(
                const MTypeWrap<std::vector<typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type>>& vals,
                const MInterpType& op
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,In>::type TypeIn;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out1>::type TypeOut1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out2>::type TypeOut2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out3>::type TypeOut3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out4>::type TypeOut4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out5>::type TypeOut5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,Out6>::type TypeOut6;
            MNoSqlDB<ThreadMode,TypeIn,TypeOut1,TypeOut2,TypeOut3,TypeOut4,TypeOut5> temp =
                    colum_db<In,Out1,Out2,Out3,Out4,Out5>();
            MNoSqlDB<ThreadMode,TypeIn,TypeOut6> temp2 = colum_db<In,Out6>();
            MNoSqlDB<__ThreadMode,TypeOut1,TypeOut2,TypeOut3,TypeOut4,TypeOut5,TypeOut6> outMat;
            if(temp.empty() || temp2.empty())
                return outMat;

            temp.unique<0>();
            temp2.unique<0>();

            MTypeWrap<std::vector<TypeIn>> xs = temp.colum<0>();
            MTypeWrap<std::vector<TypeOut1>> ys1 = temp.colum<1>();
            MTypeWrap<std::vector<TypeOut2>> ys2 = temp.colum<2>();
            MTypeWrap<std::vector<TypeOut3>> ys3 = temp.colum<3>();
            MTypeWrap<std::vector<TypeOut4>> ys4 = temp.colum<4>();
            MTypeWrap<std::vector<TypeOut5>> ys5 = temp.colum<5>();
            MTypeWrap<std::vector<TypeOut6>> ys6 = temp2.colum<1>();


			MTypeWrap<std::vector<TypeOut1>> ys11 = op(xs.toType(), ys1.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut2>> ys21 = op(xs.toType(), ys2.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut3>> ys31 = op(xs.toType(), ys3.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut4>> ys41 = op(xs.toType(), ys4.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut5>> ys51 = op(xs.toType(), ys5.toType(), vals.toType());
			MTypeWrap<std::vector<TypeOut5>> ys61 = op(xs.toType(), ys6.toType(), vals.toType());

            for(int i = 0;i<ys11.size();++i){
                outMat.push_back(
                            ys11.unsafe_at(i),
                            ys21.unsafe_at(i),
                            ys31.unsafe_at(i),
                            ys41.unsafe_at(i),
                            ys51.unsafe_at(i),
                            ys61.unsafe_at(i)
                            );
            }

            return outMat;
        }



        //
        // 按照给定条件进行查找
        //
        template<class ThreadMode,class T, class...Args>
        template<int M1,
                 class Prd1
                 >
        unsigned MNoSqlDB<ThreadMode,T,Args...>::find2(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                Prd1 prd
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            return find<Type1,M1>
                    (value1,
                     prd);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                class Prd1,
                class Prd2
                >
        unsigned MNoSqlDB<ThreadMode,T,Args...>::find2(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                Prd1 prd, Prd2 prd2
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            return find<Type1,M1,Type2,M2>
                    (value1,value2,
                     prd,prd2);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                class Prd1,
                class Prd2,
                class Prd3
                >
        unsigned MNoSqlDB<ThreadMode,T,Args...>::find2(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                Prd1 __ptr,
                Prd2 __prd2,
                Prd3 __prd3
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            return find<Type1,M1,Type2,M2,Type3,M3>
                    (value1,value2,value3,
                     __ptr,__prd2,__prd3);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4
                >
        unsigned MNoSqlDB<ThreadMode,T,Args...>::find2(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                Prd1 __ptr1 ,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            return find<Type1,M1,Type2,M2,Type3,M3,Type4,M4>
                    (value1,value2,value3,value4,
                     __ptr1,__prd2,__prd3,__prd4);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                class Prd1 ,
                class Prd2 ,
                class Prd3 ,
                class Prd4 ,
                class Prd5
                >
        unsigned MNoSqlDB<ThreadMode,T,Args...>::find2(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                Prd1 __ptr1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;
            return find<Type1,M1,Type2,M2,Type3,M3,Type4,M4,Type5,M5>
                    (value1,value2,value3,value4,value5,
                     __ptr1,__prd2,__prd3,__prd4,__prd5);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int M6,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class Prd6
                >
        unsigned MNoSqlDB<ThreadMode,T,Args...>::find2(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type& value6,
                Prd1 __ptr1 ,
                Prd2 __prd2 ,
                Prd3 __prd3 ,
                Prd4 __prd4 ,
                Prd5 __prd5 ,
                Prd6 __prd6
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type Type6;
            return find<Type1,M1,Type2,M2,Type3,M3,Type4,M4,Type5,M5,Type6,M6>
                    (value1,value2,value3,value4,value5,value6,
                     __ptr1,__prd2,__prd3,__prd4,__prd5,__prd6);
        }

        //
        // 按照给定条件进行更新数据
        //
        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                class Prd1
                >
        void MNoSqlDB<ThreadMode,T,Args...>::update2(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename MNoSqlDB<ThreadMode,T,Args...>::value_type& val,
                Prd1 prd
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            update<Type1,M1>
                    (value1,
                     val,
                     prd);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                class Prd1,
                class Prd2
                >
        void MNoSqlDB<ThreadMode,T,Args...>::update2(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename MNoSqlDB<ThreadMode,T,Args...>::value_type& val,
                Prd1 prd ,
                Prd2 prd2
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            update<Type1,M1,Type2,M2>
                    (value1,value2,
                     val,
                     prd,prd2);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                class Prd1,
                class Prd2,
                class Prd3
                >
        void MNoSqlDB<ThreadMode,T,Args...>::update2(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename MNoSqlDB<ThreadMode,T,Args...>::value_type& val,
                Prd1 __ptr ,
                Prd2 __prd2,
                Prd3 __prd3
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            update<Type1,M1,Type2,M2,Type3,M3>
                    (value1,value2,value3,
                     val,
                     __ptr,__prd2,__prd3);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4
                >
        void MNoSqlDB<ThreadMode,T,Args...>::update2(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                const typename MNoSqlDB<ThreadMode,T,Args...>::value_type& val,
                Prd1 __ptr1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            update<Type1,M1,Type2,M2,Type3,M3,Type4,M4>
                    (value1,value2,value3,value4,
                     val,
                     __ptr1,__prd2,__prd3,__prd4);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5
                >
        void MNoSqlDB<ThreadMode,T,Args...>::update2(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                const typename MNoSqlDB<ThreadMode,T,Args...>::value_type& val,
                Prd1 __ptr1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5
                )
        {

            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;
            update<Type1,M1,Type2,M2,Type3,M3,Type4,M4,Type5,M5>
                    (value1,value2,value3,value4,value5,
                     val,
                     __ptr1,__prd2,__prd3,__prd4,__prd5);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int M6,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class Prd6
                >
        void MNoSqlDB<ThreadMode,T,Args...>::update2(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type& value6,
                const typename MNoSqlDB<ThreadMode,T,Args...>::value_type& val,
                Prd1 __ptr1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5,
                Prd6 __prd6
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type Type6;
            update<Type1,M1,Type2,M2,Type3,M3,Type4,M4,Type5,M5,Type6,M6>
                    (value1,value2,value3,value4,value5,value6,
                     val,
                     __ptr1,__prd2,__prd3,__prd4,__prd5,__prd6);
        }

        //
        // 按照给定条件进行删除
        //
        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                class Prd1
                >
        bool MNoSqlDB<ThreadMode,T,Args...>::eraseall2(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                Prd1 prd
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            return eraseall<Type1,M1>
                    (value1,
                     prd);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                class Prd1,
                class Prd2
                >
        bool MNoSqlDB<ThreadMode,T,Args...>::eraseall2(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                Prd1 prd ,
                Prd2 prd2
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            return eraseall<Type1,M1,Type2,M2>
                    (value1,value2,
                     prd,prd2);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                class Prd1,
                class Prd2,
                class Prd3
                >
        bool MNoSqlDB<ThreadMode,T,Args...>::eraseall2(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                Prd1 __ptr,
                Prd2 __prd2,
                Prd3 __prd3
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            return eraseall<Type1,M1,Type2,M2,Type3,M3>
                    (value1,value2,value3,
                     __ptr,__prd2,__prd3);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4
                >
        bool MNoSqlDB<ThreadMode,T,Args...>::eraseall2(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                Prd1 __ptr1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            return eraseall<Type1,M1,Type2,M2,Type3,M3,Type4,M4>
                    (value1,value2,value3,value4,
                     __ptr1,__prd2,__prd3,__prd4);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5
                >
        bool MNoSqlDB<ThreadMode,T,Args...>::eraseall2(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                Prd1 __ptr1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;
            return eraseall<Type1,M1,Type2,M2,Type3,M3,Type4,M4,Type5,M5>
                    (value1,value2,value3,value4,value5,
                     __ptr1,__prd2,__prd3,__prd4,__prd5);
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int M6,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class Prd6
                >
        bool MNoSqlDB<ThreadMode,T,Args...>::eraseall2(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type& value6,
                Prd1 __ptr1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5,
                Prd6 __prd6
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type Type6;
            return eraseall<Type1,M1,Type2,M2,Type3,M3,Type4,M4,Type5,M5,Type6,M6>
                    (value1,value2,value3,value4,value5,value6,
                     __ptr1,__prd2,__prd3,__prd4,__prd5,__prd6);
        }

        //
        // 按照给定条件进行删除
        //
        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                class Fun1
                >
        bool MNoSqlDB<ThreadMode,T,Args...>::erase_if(
                Fun1 fun
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;

            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));

            auto beg = mData.begin();
            bool isErase = false;
            for(;beg!=mData.end();){
                Type1& val1 = TL::get<M1>(*beg);
                if(fun(val1) ){
                    beg= mData.erase(beg);
                    isErase = true;
                }
                else{
                    ++beg;
                }
            }
            return isErase;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                class Fun1,
                class Fun2
                >
        bool MNoSqlDB<ThreadMode,T,Args...>::erase_if(
                Fun1 fun1,
                Fun2 fun2
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));

            bool isErase = false;
            auto beg = mData.begin();
            for(;beg!=mData.end();){
                Type1& val1 = TL::get<M1>(*beg);
                Type2& val2 = TL::get<M2>(*beg);
                if(fun1(val1) && fun2(val2)){
					beg = mData.erase(beg);
                    isErase = true;
                }
                else{
                    ++beg;
                }
            }
            return isErase;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                class Fun1,
                class Fun2,
                class Fun3
                >
        bool MNoSqlDB<ThreadMode,T,Args...>::erase_if(
                Fun1 fun1,
                Fun2 fun2,
                Fun1 fun3
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));

            bool isErase = false;
            auto beg = mData.begin();
            for(;beg!=mData.end();){
                Type1& val1 = TL::get<M1>(*beg);
                Type2& val2 = TL::get<M2>(*beg);
                Type3& val3 = TL::get<M3>(*beg);
                if(fun1(val1) && fun2(val2) && fun3(val3) ){
					beg = mData.erase(beg);
                    isErase = true;
                }
                else{
                    ++beg;
                }
            }
            return isErase;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                class Fun1,
                class Fun2,
                class Fun3,
                class Fun4
                >
        bool MNoSqlDB<ThreadMode,T,Args...>::erase_if(
                Fun1 fun1,
                Fun2 fun2,
                Fun3 fun3,
                Fun4 fun4
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));

            bool isErase = false;
            auto beg = mData.begin();
            for(;beg!=mData.end();){
                Type1& val1 = TL::get<M1>(*beg);
                Type2& val2 = TL::get<M2>(*beg);
                Type3& val3 = TL::get<M3>(*beg);
                Type4& val4 = TL::get<M4>(*beg);
                if(fun1(val1) && fun2(val2) && fun3(val3) && fun4(val4)){
					beg = mData.erase(beg);
                    isErase = true;
                }
                else{
                    ++beg;
                }
            }
            return isErase;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                class Fun1,
                class Fun2,
                class Fun3,
                class Fun4,
                class Fun5
                >
        bool MNoSqlDB<ThreadMode,T,Args...>::erase_if(
                Fun1 fun1,
                Fun2 fun2,
                Fun3 fun3,
                Fun4 fun4,
                Fun5 fun5
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));

            bool isErase = false;
            auto beg = mData.begin();
            for(;beg!=mData.end();){
                Type1& val1 = TL::get<M1>(*beg);
                Type2& val2 = TL::get<M2>(*beg);
                Type3& val3 = TL::get<M3>(*beg);
                Type4& val4 = TL::get<M4>(*beg);
                Type5& val5 = TL::get<M5>(*beg);
                if(fun1(val1) && fun2(val2) && fun3(val3) && fun4(val4)&&fun5(val5)){
					beg = mData.erase(beg);
                    isErase = true;
                }
                else{
                    ++beg;
                }
            }
            return isErase;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int M6,
                class Fun1,
                class Fun2,
                class Fun3,
                class Fun4,
                class Fun5,
                class Fun6
                >
        bool MNoSqlDB<ThreadMode,T,Args...>::erase_if(
                Fun1 fun1,
                Fun2 fun2,
                Fun3 fun3,
                Fun4 fun4,
                Fun5 fun5,
                Fun1 fun6
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type Type6;
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));

            bool isErase = false;
            auto beg = mData.begin();
            for(;beg!=mData.end();){
                Type1& val1 = TL::get<M1>(*beg);
                Type2& val2 = TL::get<M2>(*beg);
                Type3& val3 = TL::get<M3>(*beg);
                Type4& val4 = TL::get<M4>(*beg);
                Type5& val5 = TL::get<M5>(*beg);
                Type6& val6 = TL::get<M6>(*beg);
                if(fun1(val1) && fun2(val2) && fun3(val3) && fun4(val4)&&fun5(val5)&&fun6(val6)){
					beg = mData.erase(beg);
                    isErase = true;
                }
                else{
                    ++beg;
                }
            }
            return isErase;
        }

        //
        // 检查包含指定项的条目
        //
        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                class Prd1
                >
        unsigned MNoSqlDB<ThreadMode,T,Args...>::counts(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                Prd1 prd
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;

            unsigned count = 0;
            for(auto& c : mData){
                Type1 val1 = TL::reference_at<M1>(c);
                if(prd(val1,value1) ){
                    count++;
                }
            }
            return count;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                class Prd1,
                class Prd2
                >
        unsigned MNoSqlDB<ThreadMode,T,Args...>::counts(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                Prd1 prd ,
                Prd2 prd2
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;

            unsigned count = 0;
            for(auto& c : mData){
                Type1 val1 = TL::reference_at<M1>(c);
                Type2 val2 = TL::reference_at<M2>(c);
                if(prd(val1,value1) &&  prd2(val2,value2)){
                    count++;
                }
            }
            return count;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                class Prd1,
                class Prd2,
                class Prd3
                >
        unsigned MNoSqlDB<ThreadMode,T,Args...>::counts(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                Prd1 __ptr,
                Prd2 __prd2,
                Prd3 __prd3
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;

            unsigned count = 0;
            for(auto& c : mData){
                Type1 val1 = TL::reference_at<M1>(c);
                Type2 val2 = TL::reference_at<M2>(c);
                Type3 val3 = TL::reference_at<M3>(c);
                if(__ptr(val1,value1) &&
                        __prd2(val2,value2) &&
                        __prd3(val3,value3)){
                    count++;
                }
            }
            return count;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4
                >
        unsigned MNoSqlDB<ThreadMode,T,Args...>::counts(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                Prd1 __ptr1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;

            unsigned count = 0;
            for(auto& c : mData){
                Type1 val1 = TL::reference_at<M1>(c);
                Type2 val2 = TL::reference_at<M2>(c);
                Type3 val3 = TL::reference_at<M3>(c);
                Type4 val4 = TL::reference_at<M4>(c);
                if(__ptr1(val1,value1) &&
                        __prd2(val2,value2) &&
                        __prd3(val3,value3)&&
                        __prd4(val4,value4) ){
                    count++;
                }
            }
            return count;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5
                >
        unsigned MNoSqlDB<ThreadMode,T,Args...>::counts(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                Prd1 __ptr1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;

            unsigned count = 0;
            for(auto& c : mData){
                Type1 val1 = TL::reference_at<M1>(c);
                Type2 val2 = TL::reference_at<M2>(c);
                Type3 val3 = TL::reference_at<M3>(c);
                Type4 val4 = TL::reference_at<M4>(c);
                Type5 val5 = TL::reference_at<M5>(c);
                if(__ptr1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                        __prd4(val4,value4) && __prd5(val5,value5)){
                    count++;
                }
            }
            return count;
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int M6,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class Prd6
                >
        unsigned MNoSqlDB<ThreadMode,T,Args...>::counts(
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type& value6,
                Prd1 __ptr1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5,
                Prd6 __prd6
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type Type6;

            unsigned count = 0;
            for(auto& c : mData){
                Type1 val1 = TL::reference_at<M1>(c);
                Type2 val2 = TL::reference_at<M2>(c);
                Type3 val3 = TL::reference_at<M3>(c);
                Type4 val4 = TL::reference_at<M4>(c);
                Type5 val5 = TL::reference_at<M5>(c);
                Type6 val6 = TL::reference_at<M6>(c);

                if(__ptr1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                        __prd4(val4,value4) && __prd5(val5,value5) && __prd6(val6,value6)){
                    count++;
                }
            }
            return count;
        }

        //
        // 按照某个条件设置某个值
        //
        template<class ThreadMode,class T, class...Args>
        template<
                int M,
                int M1,
                class Prd1
                >
        void MNoSqlDB<ThreadMode,T,Args...>::set(
                unsigned row,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M>::type& val,
                Prd1 __prd1
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));

            if(row == -1){
                for(auto& __val : mData){
                    Type1 val1 = TL::reference_at<M1>(__val);
                    if(__prd1(val1,value1)){
                        TL::get<M>(__val) = val;
                    }
                }
            }
            else{
                if(row >= mData.size()){
                    return;
                }
                typename MNoSqlDB<ThreadMode,T,Args...>::value_type& __val = mData[row];
                Type1 val1 = TL::reference_at<M1>(__val);
                if(__prd1(val1,value1)){
                    TL::get<M>(__val) = val;
                }
            }
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M,
                int M1,
                int M2,
                class Prd1,
                class Prd2
                >
        void MNoSqlDB<ThreadMode,T,Args...>::set(
                unsigned row,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M>::type& val,
                Prd1 __prd1,
                Prd2 __prd2
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));

            if(row == -1){
                for(auto& __val : mData){
                    Type1 val1 = TL::reference_at<M1>(__val);
                    Type2 val2 = TL::reference_at<M2>(__val);
                    if(__prd1(val1,value1) && __prd2(val2,value2)){
                        TL::get<M>(__val) = val;
                    }
                }
            }
            else{
                if(row >= mData.size()){
                    return;
                }
                typename MNoSqlDB<ThreadMode,T,Args...>::value_type& __val = mData[row];
                Type1 val1 = TL::reference_at<M1>(__val);
                Type2 val2 = TL::reference_at<M2>(__val);
                if(__prd1(val1,value1) && __prd2(val2,value2)){
                    TL::get<M>(__val) = val;
                }
            }
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M,
                int M1,
                int M2,
                int M3,
                class Prd1,
                class Prd2,
                class Prd3
                >
        void MNoSqlDB<ThreadMode,T,Args...>::set(
                unsigned row,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M>::type& val,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));
            if(row == -1){
                for(auto& __val : mData){
                    Type1 val1 = TL::reference_at<M1>(__val);
                    Type2 val2 = TL::reference_at<M2>(__val);
                    Type3 val3 = TL::reference_at<M3>(__val);

                    if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3)){
                        TL::get<M>(__val) = val;
                    }
                }
            }
            else{
                if(row >= mData.size()){
                    return;
                }
                typename MNoSqlDB<ThreadMode,T,Args...>::value_type& __val = mData[row];
                Type1 val1 = TL::reference_at<M1>(__val);
                Type2 val2 = TL::reference_at<M2>(__val);
                Type3 val3 = TL::reference_at<M3>(__val);

                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3)){
                    TL::get<M>(__val) = val;
                }
            }
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M,
                int M1,
                int M2,
                int M3,
                int M4,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4
                >
        void MNoSqlDB<ThreadMode,T,Args...>::set(
                unsigned row,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M>::type& val,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));

            if(row == -1){
                for(auto& __val : mData){
                    Type1 val1 = TL::reference_at<M1>(__val);
                    Type2 val2 = TL::reference_at<M2>(__val);
                    Type3 val3 = TL::reference_at<M3>(__val);
                    Type4 val4 = TL::reference_at<M4>(__val);

                    if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                            __prd4(val4,value4)){
                        TL::get<M>(__val) = val;
                    }
                }
            }
            else{
                if(row >= mData.size()){
                    return;
                }
                typename MNoSqlDB<ThreadMode,T,Args...>::value_type& __val = mData[row];
                Type1 val1 = TL::reference_at<M1>(__val);
                Type2 val2 = TL::reference_at<M2>(__val);
                Type3 val3 = TL::reference_at<M3>(__val);
                Type4 val4 = TL::reference_at<M4>(__val);

                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                        __prd4(val4,value4)){
                    TL::get<M>(__val) = val;
                }
            }
        }

        template<class ThreadMode,class T, class...Args>
        template<
                int M,
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5
                >
        void MNoSqlDB<ThreadMode,T,Args...>::set(
                unsigned row,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M>::type& val,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;

            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));

            if(row == -1){
                for(auto& __val : mData){
                    Type1 val1 = TL::reference_at<M1>(__val);
                    Type2 val2 = TL::reference_at<M2>(__val);
                    Type3 val3 = TL::reference_at<M3>(__val);
                    Type4 val4 = TL::reference_at<M4>(__val);
                    Type5 val5 = TL::reference_at<M5>(__val);

                    if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                            __prd4(val4,value4) && __prd5(val5,value5)){
                        TL::get<M>(__val) = val;
                    }
                }
            }
            else{
                if(row >= mData.size()){
                    return;
                }
                typename MNoSqlDB<ThreadMode,T,Args...>::value_type& __val = mData[row];
                Type1 val1 = TL::reference_at<M1>(__val);
                Type2 val2 = TL::reference_at<M2>(__val);
                Type3 val3 = TL::reference_at<M3>(__val);
                Type4 val4 = TL::reference_at<M4>(__val);
                Type5 val5 = TL::reference_at<M5>(__val);

                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                        __prd4(val4,value4) && __prd5(val5,value5)){
                    TL::get<M>(__val) = val;
                }
            }
        }


        template<class ThreadMode,class T, class...Args>
        template<
                int M,
                int M1,
                int M2,
                int M3,
                int M4,
                int M5,
                int M6,
                class Prd1,
                class Prd2,
                class Prd3,
                class Prd4,
                class Prd5,
                class Prd6
                >
        void MNoSqlDB<ThreadMode,T,Args...>::set(
                unsigned row,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type& value1,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type& value2,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type& value3,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type& value4,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type& value5,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type& value6,
                const typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M>::type& val,
                Prd1 __prd1,
                Prd2 __prd2,
                Prd3 __prd3,
                Prd4 __prd4,
                Prd5 __prd5,
                Prd6 __prd6
                )
        {
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M1>::type Type1;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M2>::type Type2;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M3>::type Type3;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M4>::type Type4;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M5>::type Type5;
            typedef typename mjTL::index_type<mjTL::MTypeList<T,Args...>,M6>::type Type6;

            Lock();
            MExitFunction<void> ExitFun(std::bind(&MNoSqlDB<ThreadMode,T,Args...>::UnLock,this));

            if(row == -1){
                for(auto& __val : mData){
                    Type1 val1 = TL::reference_at<M1>(__val);
                    Type2 val2 = TL::reference_at<M2>(__val);
                    Type3 val3 = TL::reference_at<M3>(__val);
                    Type4 val4 = TL::reference_at<M4>(__val);
                    Type5 val5 = TL::reference_at<M5>(__val);
                    Type6 val6 = TL::reference_at<M6>(__val);

                    if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                            __prd4(val4,value4) && __prd5(val5,value5) && __prd6(val6,value6)){
                        TL::get<M>(__val) = val;
                    }
                }
            }
            else{
                if(row >= mData.size()){
                    return;
                }
                typename MNoSqlDB<ThreadMode,T,Args...>::value_type& __val = mData[row];
                Type1 val1 = TL::reference_at<M1>(__val);
                Type2 val2 = TL::reference_at<M2>(__val);
                Type3 val3 = TL::reference_at<M3>(__val);
                Type4 val4 = TL::reference_at<M4>(__val);
                Type5 val5 = TL::reference_at<M5>(__val);
                Type6 val6 = TL::reference_at<M6>(__val);

                if(__prd1(val1,value1) && __prd2(val2,value2) && __prd3(val3,value3)&&
                        __prd4(val4,value4) && __prd5(val5,value5) && __prd6(val6,value6)){
                    TL::get<M>(__val) = val;
                }
            }
        }
}
