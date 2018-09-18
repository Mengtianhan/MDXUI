//----------------------------------------
// 一个能够储存任意类型的容器
// 容器为变参容器
//----------------------------------------
#pragma once
#include <vector>
#include <iostream>
#include "HTuple.h"
#include <mutex>
#include <fstream>

namespace mj{
        //
        // 预定义一个普通的默认比较器
        //
        template<class T>
        struct MEquals{
            bool operator()(T val1,T val2){
                return val1==val2;
            }
        };

        //
        // 下面是预定义一些稍微特殊的比较器
        //

        //
        // 其实主要是针对MString
        //
        template<class T>
        struct MStrEquals{
            bool operator()(const T& str1,const T& str2){
                T val1 = str1,val2 = str2;
                val1.trim();
                val2.trim();
                return val1==val2;
            }
        };

        //
        // 这个主要是针对浮点数
        //
        template<class T = float>
        struct MFloatEquals{
            MFloatEquals(T res = 0.5):mRes(res){}
            bool operator()(T val1,T val2){
                return std::abs(val1-val2)<=mRes;
            }
        private:
            T mRes{0.5};
        };

        //
        // 这个用于整形的特殊对比，之所以特殊是因为有些相差不大的数我们仍然当他是相等
        //
        template<class T = __int16>
        struct MIntEquals{
            MIntEquals(T res = 0):mRes(res){}
            bool operator()(T val1,T val2){
                return std::abs(val1-val2)<=mRes;
            }
        private:
            T mRes{0};
        };

        //
        // 这是一个专用于辅助操作的类
        //
        template<class T>
        struct MOptional{
        public:
            //
            // 如果没有进行初始化,那么就是空的
            //
            MOptional():pPtr(nullptr){}

            //
            // 如果对其初始化就部位nullptr
            //
            MOptional(const T& value):pPtr(new T(value)){}

            MOptional& operator=(const T& value){
                clear();
                pPtr = new T(value);
                return *this;
            }

            MOptional(const MOptional& other){
                if(other.pPtr)
                    pPtr = new T(*(other.pPtr));
                else
                    pPtr = nullptr;
            }

            //
            // 右值拷贝
            //
            MOptional(MOptional&& other){
                if(other.pPtr)
                    pPtr = new T(*(other.pPtr));
                else
                    pPtr = nullptr;
                other.clear();
            }

            MOptional& operator=(const MOptional& other){
                if(this == &other)
                    return *this;
                clear();
                if(!other)return *this;
                pPtr = new T(*(other.pPtr));
                return *this;
            }

            //
            // 右值赋值
            //
            MOptional& operator=(MOptional&& other){
                if(this == &other)
                    return *this;
                clear();
                if(!other)return *this;
                pPtr = new T(*(other.pPtr));
                other.clear();
                return *this;
            }

            ~MOptional(){
                clear();
            }

            operator bool(){
                return pPtr==nullptr?false:true;
            }

            operator bool() const{
                return pPtr==nullptr?false:true;
            }

            T& operator*(){
                if(pPtr == nullptr){
                    throw std::runtime_error("对空指针非法解引用");
                }
                return *pPtr;
            }

            const T& operator*() const{
                if(pPtr == nullptr){
                    throw std::runtime_error("对空指针非法解引用");
                }
                return *pPtr;
            }

            void clear(){
                if(pPtr){
                    delete pPtr;
                    pPtr = nullptr;
                }
            }

            T* operator->(){
                if(pPtr == nullptr){
                    throw std::runtime_error("对空指针非法操作");
                }
                return pPtr;
            }

            const T* operator->() const{
                if(pPtr == nullptr){
                    throw std::runtime_error("对空指针非法操作");
                }
                return pPtr;
            }

            friend std::ostream& operator<<(std::ostream& os,const MOptional& op){
                if(op){
                    os<<*op;
                }
                return os;
            }

        private:
            T* pPtr{nullptr};
        };


        //
        // 正式内存中的数据库开始了
        //

	template<class T, class...Args>
        class HContainer{
		typedef std::tuple<T, Args...> tuple_data;
	public:
		typedef tuple_data value_type;
		typedef typename std::vector<value_type>::iterator iterator;
                typedef typename std::vector<value_type>::const_iterator const_iterator;

		HContainer(){}
		~HContainer(){}

		

		HContainer(const HContainer& other){
			m_data = other.m_data;
		}

                HContainer(HContainer&& other){
                        m_data = std::move(other.m_data);
                        other.m_data.clear();
                }
                //
		// 使用数据来构造
                //
                HContainer(const std::vector<value_type>& __data){
			m_data = __data;
		}

                //
		// 数据和容器的转换
                //
                HContainer& operator=(const std::vector<value_type>& __data){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        //
			// 证同测试
                        //
			if (m_data == __data || &m_data == &__data){
				return *this;
			}
			m_data = __data;
			return *this;
		}

                //
		// 赋值操作符
                //
		HContainer& operator=(const HContainer& other){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			m_data = other.m_data;
			return *this;
		}

                HContainer& operator=(HContainer&& other){
                    m_data = std::move(other.m_data);
                    other.m_data.clear();
                    return *this;
                }

                //
		// 转换为std::vector
                //
		operator std::vector<value_type>(){
			return m_data;
		}

                template<class A,template<class U,class A1> class C>
                C<value_type,A>& ToOtherType(C<value_type,A>& result){
                    result.assign(m_data.begin(),m_data.end());
                    return result;
                }

                //
                // 转换成表,为了不引入过多的头文件，此处使用模板自行推导
                //
                template<class U,template<class U1> class M>
                M<U>& toMSheet(M<U>& sheet){
                    sheet.clear();
                    for(auto& c : *this){
                        std::ostringstream os;
                        tuple_print(c,os,"#");
                        U str = os.str();
                        M<U>::value_type v;
                        str.split("#",v);
                        sheet.push_back(v);
                    }
                    return sheet;
                }

                ///
                /// 将矩阵数据转换为线性表
                ///
                template<class U,class A,template<class U1,class A1> class C>
                C<U,A>& toVector(C<U,A>& v){
                    v.clear();
                    for(auto& c : *this){
                        std::ostringstream os;
                        tuple_print(c,os,"#");
                        MString str = os.str();
                        C<U,A> __tempv;
                        str.split("#",__tempv);
                        std::copy(__tempv.begin(),__tempv.end(),std::back_inserter(v));
                    }
                    return v;
                }


                //
		// 添加元素
		// 可以使用变参方式
		// 可以直接将一个同类型的容器添加进去
                //
                void push_back(const T& value, const Args&...args){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			m_data.push_back(std::make_tuple(value, args...));
		}

		void push_back(const value_type& value){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			m_data.push_back(value);
		}

		void push_back(const HContainer& other){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			std::copy(other.begin(), other.end(), std::back_inserter(m_data));
		}

                //
		// 插入元素
                //
                void insert(int row,T value, const Args&...args){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			value_type item = std::make_tuple(value,args...);
			m_data.insert(m_data.begin() + row, item);
		}

		void insert(int row,const value_type& value){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			m_data.insert(m_data.begin() + row, value);
		}

                //
                // 引用内部数据
                //
                void make_reference(int row,T& value, Args&... args) const{
                    std::tie(value,args...) = m_data.at(row);
                }

                void make_reference(const value_type& v,T& value, Args&... args) const{
                    std::tie(value,args...) = v;
                }

                value_type make_value(const T& value, const Args&... args) const{
                    return std::make_tuple(value,args...);
                }

                //
		// 弹出元素
		// 需要对返回值进行判断
                //
                MOptional<value_type> back(){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			if (m_data.empty())
                                return MOptional<value_type>();
                        return MOptional<value_type>(m_data.back());
		}

                MOptional<value_type> back() const{
                    if (m_data.empty())
                            return MOptional<value_type>();
                    return MOptional<value_type>(m_data.back());
                }

		void pop_back(){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			if (m_data.empty())
				return;
			m_data.pop_back();
		}

                MOptional<value_type> front(){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			if (m_data.empty())
                                return MOptional<value_type>();
                        return MOptional<value_type>(*m_data.begin());
		}

                MOptional<value_type> front() const{
                        if (m_data.empty())
                                return MOptional<value_type>();
                        return MOptional<value_type>(*m_data.begin());
                }

		void pop_front(){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			if (m_data.empty())
				return;
			m_data.erase(m_data.begin());
		}

                //
		// 获取所有的数据
                //
		std::vector<tuple_data>& GetData(){
			return m_data;
		}

		const std::vector<tuple_data>& GetData() const{
			return m_data;
		}

                //
		// 获取行数
                //
                const unsigned colum() const{
			if (m_data.empty())
				return 0;
			return std::tuple_size<tuple_data>::value;
		}

                //
		// 获取列数
                //
                const unsigned row() const{
			return m_data.size();
		}

                //
		// 获取某一行一列元素的
		// 模板参数N指定行号，普通参数row指定列号
                //

                template<class U, int N>
                U at(unsigned row){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        try{
                                return std::get<N>(m_data.at(row));
                        }
                        catch (...){
                                return U();
                        }
                }

		template<class U, int N>
                U at(unsigned row) const{
			try{
				return std::get<N>(m_data.at(row));
			}
			catch (...){
				return U();
			}
		}

                //
		// 通过一个给定的value_type
		// 获取由模板参数N指定的元素
		// 模板参数N指定行号
                // 可以使用std::get<N>(v)来代替该操作
                //
		template<class U,int N>
                U ElementValue(value_type v) throw(std::runtime_error)
		{
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			if (N >= std::tuple_size<value_type>::value){
				throw std::runtime_error("tuple index out of range......");
			}
			try{
				return std::get<N>(v);
			}
			catch (...){
				return U();
			}
		}

                template<class U,int N>
                U ElementValue(value_type v) const throw(std::runtime_error)
                {
                        if (N >= std::tuple_size<value_type>::value){
                                throw std::runtime_error("tuple index out of range......");
                        }
                        try{
                                return std::get<N>(v);
                        }
                        catch (...){
                                return U();
                        }
                }

                //
		// 通过一个指定的元素
		// 获取拥有该元素的第一个value_type
		// 返回值需要进行判断
                //
                template<class U,int N,class Prd = MEquals<U>>
                MOptional<value_type> Get_Element(const U& value,Prd __ptr = Prd()){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			if (m_data.empty()){
                            return MOptional<value_type>();
			}
			try{
                            for (auto& v : m_data){
                                U u = std::get<N>(v);
                                if (__ptr(u,value)){
                                    return MOptional<value_type>(v);
                                }
                            }
			}
			catch (...)
			{
			}
                        return MOptional<value_type>();
		}

                //
                // 查找满足2该条件的项
                //
                template<class U,int N,class V,int M,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>>
                MOptional<value_type> Get_Element(const U& value1,const V& value2,
                                                        Prd __ptr = Prd(),Prd2 __prd2 = Prd2()){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        if (m_data.empty()){
                            return MOptional<value_type>();
                        }
                        try{
                            for (auto& v : m_data){
                                U u  = std::get<N>(v);
                                V v1 = std::get<M>(v);
                                if (__ptr(u,value1) && __prd2(v1,value2)){
                                    return MOptional<value_type>(v);
                                }
                            }
                        }
                        catch (...)
                        {
                        }
                        return MOptional<value_type>();
                }

                //
                // 查找满足3该条件的项
                //
                template<class U,int N,class V,int M,class V2,int M2,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V2>>
                MOptional<value_type> Get_Element(const U& value1,const V& value2,
                                                        const V2& value3,
                                                        Prd __ptr = Prd(),
                                                        Prd2 __prd2 = Prd2(),
                                                        Prd3 __prd3 = Prd3()){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        if (m_data.empty()){
                                return MOptional<value_type>();
                        }
                        try{
                            for (auto& v : m_data){
                                U u   = std::get<N>(v);
                                V v1  = std::get<M>(v);
                                V2 v2 = std::get<M2>(v);
                                if (__ptr(u,value1) && __prd2(v1,value2) &&
                                        __prd3(v2,value3))
                                {
                                    return MOptional<value_type>(v);
                                }
                            }
                        }
                        catch (...)
                        {
                        }
                        return MOptional<value_type>();
                }

                //
                // 查找满足4该条件的项
                //
                template<class U,int N,class V,int M,class V2,int M2,class V3,int M3,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V2>,class Prd4 = MEquals<V3>>
                MOptional<value_type> Get_Element(const U& value1,const V& value2,
                                                        const V2& value3,const V3& value4,
                                                        Prd __ptr = Prd(),
                                                        Prd2 __prd2 = Prd2(),
                                                        Prd3 __prd3 = Prd3(),
                                                        Prd4 __prd4 = Prd4())
                {
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        if (m_data.empty()){
                                return MOptional<value_type>();
                        }
                        try{
                            for (auto& v : m_data){
                                U u   = std::get<N>(v);
                                V v1  = std::get<M>(v);
                                V2 v2 = std::get<M2>(v);
                                V3 v3 = std::get<M3>(v);
                                if (__ptr(u,value1) && __prd2(v1,value2) &&
                                        __prd3(v2,value3) && __prd4(v3,value4))
                                {
                                    return MOptional<value_type>(v);
                                }
                            }
                        }
                        catch (...)
                        {
                        }
                        return MOptional<value_type>();
                }

                //
                // 查找满足5个条件的项
                //
                template<class U, int N,class V,int M,class V2,int M2,class V3,int M3,
                         class V4,int M4,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V2>,class Prd4 = MEquals<V3>,
                         class Prd5 = MEquals<V4>>
                MOptional<value_type> Get_Element(const U& value1,const V& value2,
                                                        const V2& value3,const V3& value4,
                                                        const V4& value5,
                                                        Prd __ptr = Prd(),
                                                        Prd2 __prd2 = Prd2(),
                                                        Prd3 __prd3 = Prd3(),
                                                        Prd4 __prd4 = Prd4(),
                                                        Prd5 __prd5 = Prd5())
                {
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        if (m_data.empty()){
                                return MOptional<value_type>();
                        }
                        try{
                            for (auto& v : m_data){
                                U u   = std::get<N>(v);
                                V v1  = std::get<M>(v);
                                V2 v2 = std::get<M2>(v);
                                V3 v3 = std::get<M3>(v);
                                V4 v4 = std::get<M4>(v);
                                if (__ptr(u,value1) && __prd2(v1,value2) &&
                                        __prd3(v2,value3) && __prd4(v3,value4)&&
                                        __prd5(v4,value5))
                                {
                                    return MOptional<value_type>(v);
                                }
                            }
                        }
                        catch (...)
                        {
                        }
                        return MOptional<value_type>();
                }

                //
                // 查找满足6个条件的项
                //
                template<class U, int N,class V,int M,class V2,int M2,class V3,int M3,
                         class V4,int M4,class V5,int M5,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V2>,class Prd4 = MEquals<V3>,
                         class Prd5 = MEquals<V4>,class Prd6 = MEquals<V5>>
                MOptional<value_type> Get_Element(const U& value1,const V& value2,
                                                        const V2& value3,const V3& value4,
                                                        const V4& value5,const V5& value6,
                                                        Prd __ptr = Prd(),
                                                        Prd2 __prd2 = Prd2(),
                                                        Prd3 __prd3 = Prd3(),
                                                        Prd4 __prd4 = Prd4(),
                                                        Prd5 __prd5 = Prd5(),
                                                        Prd6 __prd6 = Prd6()
                                                  )
                {
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        if (m_data.empty()){
                                return MOptional<value_type>();
                        }
                        try{
                            for (auto& v : m_data){
                                U u   = std::get<N>(v);
                                V v1  = std::get<M>(v);
                                V2 v2 = std::get<M2>(v);
                                V3 v3 = std::get<M3>(v);
                                V4 v4 = std::get<M4>(v);
                                V5 v5 = std::get<M5>(v);
                                if (__ptr(u,value1) && __prd2(v1,value2) &&
                                        __prd3(v2,value3) && __prd4(v3,value4)&&
                                        __prd5(v4,value5) && __prd6(v5,value6))
                                {
                                    return MOptional<value_type>(v);
                                }
                            }
                        }
                        catch (...)
                        {
                        }
                        return MOptional<value_type>();
                }

                //
		// 通过一个指定的元素
		// 获取拥有该元素的所有数据
                //
                template<class U,int N,class Prd = MEquals<U>>
                HContainer Get_SubContainer(const U& value,Prd __prd = Prd())
                {
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			HContainer __result;
			if (m_data.empty()){
				return __result;
			}
			try{
				for (auto& v : m_data){
                                        U u = std::get<N>(v);

                                        if (__prd(u,value)){
						__result.push_back(v);
					}
				}
			}
			catch (...){
			}
			return __result;
		}

                //
                // 获取拥有2个指定元素的表
                //
                template<class U,int N,class V,int M,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>>
                HContainer Get_SubContainer(const U& value1,const V& value2,
                                            Prd __prd = Prd(),Prd2 __prd2 = Prd2())
                {
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        HContainer __result;
                        if (m_data.empty()){
                                return __result;
                        }
                        try{
                                for (auto& v : m_data){
                                        U u  = std::get<N>(v);
                                        V v1 = std::get<M>(v);
                                        if (__prd(u,value1) && __prd2(v1,value2)){
                                                __result.push_back(v);
                                        }
                                }
                        }
                        catch (...){
                        }
                        return __result;
                }

                //
                // 获取拥有3个指定元素的表
                //
                template<class U,int N,class V,int M,class V2,int M2,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V2>>
                HContainer Get_SubContainer(const U& value1,const V& value2,const V2& value3,
                                            Prd __prd = Prd(),Prd2 __prd2 = Prd2(),
                                            Prd3 __prd3 = Prd3())
                {
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        HContainer __result;
                        if (m_data.empty()){
                                return __result;
                        }
                        try{
                                for (auto& v : m_data){
                                        U u =   std::get<N>(v);
                                        V v1 =  std::get<M>(v);
                                        V2 v2 = std::get<M2>(v);
                                        if (__prd(u,value1) && __prd2(v1,value2)&&
                                                __prd3(v2,value3))
                                        {
                                                __result.push_back(v);
                                        }
                                }
                        }
                        catch (...){
                        }
                        return __result;
                }

                //
                // 获取拥有4个指定元素的表
                //
                template<class U,int N,class V,int M,class V2,int M2,class V3,int M3,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V2>,class Prd4 = MEquals<V3>>
                HContainer Get_SubContainer(const U& value1,const V& value2,
                                            const V2& value3,const V3& value4,
                                            Prd __prd = Prd(),Prd2 __prd2 = Prd2(),
                                            Prd3 __prd3 = Prd3(),Prd4 __prd4 = Prd4())
                {
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        HContainer __result;
                        if (m_data.empty()){
                                return __result;
                        }
                        try{
                                for (auto& v : m_data){
                                        U u   = std::get<N>(v);
                                        V v1  = std::get<M>(v);
                                        V2 v2 = std::get<M2>(v);
                                        V3 v3 = std::get<M3>(v);
                                        if (__prd(u,value1) && __prd2(v1,value2)&&
                                                __prd3(v2,value3) && __prd4(v3,value4))
                                        {
                                                __result.push_back(v);
                                        }
                                }
                        }
                        catch (...){
                        }
                        return __result;
                }

                //
                // 获取拥有5个指定元素的表
                //
                template<class U, int N,class V,int M,class V2,int M2,class V3,int M3,
                         class V4,int M4,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V2>,class Prd4 = MEquals<V3>,
                         class Prd5 = MEquals<V4>>
                HContainer Get_SubContainer(const U& value1,const V& value2,
                                            const V2& value3,const V3& value4,
                                            const V4& value5,
                                            Prd __prd = Prd(),Prd2 __prd2 = Prd2(),
                                            Prd3 __prd3 = Prd3(),Prd4 __prd4 = Prd4(),
                                            Prd5 __prd5 = Prd5())
                {
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        HContainer __result;
                        if (m_data.empty()){
                                return __result;
                        }
                        try{
                                for (auto& v : m_data){
                                        U u   = std::get<N>(v);
                                        V v1  = std::get<M>(v);
                                        V2 v2 = std::get<M2>(v);
                                        V3 v3 = std::get<M3>(v);
                                        V4 v4 = std::get<M4>(v);
                                        if (__prd(u,value1) && __prd2(v1,value2)&&
                                                __prd3(v2,value3) && __prd4(v3,value4)&&
                                                __prd5(v4,value5))
                                        {
                                                __result.push_back(v);
                                        }
                                }
                        }
                        catch (...){
                        }
                        return __result;
                }

                //
                // 获取拥有6个指定元素的表
                //
                template<class U, int N,class V,int M,class V2,int M2,class V3,int M3,
                         class V4,int M4,class V5,int M5,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V2>,class Prd4 = MEquals<V3>,
                         class Prd5 = MEquals<V4>,class Prd6 = MEquals<V5>>
                HContainer Get_SubContainer(const U& value1,const V& value2,
                                            const V2& value3,const V3& value4,
                                            const V4& value5,const V5& value6,
                                            Prd __prd = Prd(),Prd2 __prd2 = Prd2(),
                                            Prd3 __prd3 = Prd3(),Prd4 __prd4 = Prd4(),
                                            Prd5 __prd5 = Prd5(),Prd6 __prd6 = Prd6())
                {
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        HContainer __result;
                        if (m_data.empty()){
                                return __result;
                        }
                        try{
                                for (auto& v : m_data){
                                        U u   = std::get<N>(v);
                                        V v1  = std::get<M>(v);
                                        V2 v2 = std::get<M2>(v);
                                        V3 v3 = std::get<M3>(v);
                                        V4 v4 = std::get<M4>(v);
                                        V5 v5 = std::get<M5>(v);
                                        if (__prd(u,value1) && __prd2(v1,value2)&&
                                                __prd3(v2,value3) && __prd4(v3,value4)&&
                                                __prd5(v4,value5) && __prd6(v5,value6))
                                        {
                                                __result.push_back(v);
                                        }
                                }
                        }
                        catch (...){
                        }
                        return __result;
                }

                //
		// 获取指定行的数据
		// 行号由模板参数N指定
                //
		template<class U, int N>
		std::vector<U> Get_ColumData(){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			std::vector<U> __result;
			std::for_each(m_data.begin(), m_data.end(), [&](tuple_data data){
				__result.push_back(std::get<N>(data));
			});
			return __result;
		}

                //
		// 获取指定列的数据
		// row 为列号
		// 返回值需要进行判断
		// if(__result) //如果存在返回值那么为true
                //
                MOptional<value_type> Get_RowData(unsigned row){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			if (m_data.empty() || m_data.size() <= row)
                                return MOptional<value_type>();
                        return MOptional<value_type>(m_data.at(row));
		}

                //
		// 按照指定元素指定方法进行排序
                //
		template<class U, int N, class CMP = std::less<U>>
		void sort(){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			try{
				std::sort(m_data.begin(), m_data.end(), [&](value_type v1, value_type v2){
                                        U t1 = std::get<N>(v1);
                                        U t2 = std::get<N>(v2);
					return CMP()(t1, t2);
				});
			}
			catch (...)
			{
			}
		}

                //
		// 对一个区间进行排序
                //
		template<class U, int N, class CMP = std::less<U>>
		void sort(int first, int last){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			if (m_data.size() <= last){
				last = m_data.size();;
			}
			try{
				std::sort(m_data.begin() + first, m_data.begin() + last, [&](value_type v1, value_type v2){
                                        U t1 = std::get<N>(v1);
                                        U t2 = std::get<N>(v2);
					return CMP()(t1, t2);
				});
			}
			catch (...){}
		}

                //
		// 查找
		// 通过指定元素查找拥有该元素是否存在
		// 如果存在返回该元素的位置
		// 如果不存在返回-1
                //
                template<class U, int N,class Prd = MEquals<U>>
                int find(const U& value,Prd __ptr = Prd()){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			int i = 0;
			try{
				for (auto it : m_data){
                                        U t = std::get<N>(it);
                                        if (__ptr(t,value)){
						return i;
					}
					++i;
				}
			}
			catch (...)
			{
			}
			return -1;
		}

                //
                // 查找满足2个条件的项
                //
                template<class U, int N,class V,int M,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>>
                int find(const U& value1,const V& value2,
                         Prd __prd = Prd(),Prd2 __prd2 = Prd2()){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        int i = 0;
                        try{
                                for (auto it : m_data){
                                        U t = std::get<N>(it);
                                        V v = std::get<M>(it);
                                        if (__prd(t,value1) && __prd2(v,value2)){
                                                return i;
                                        }
                                        ++i;
                                }
                        }
                        catch (...)
                        {
                        }
                        return -1;
                }

                //
                // 查找满足3个条件的项
                //
                template<class U, int N,class V,int M,class V2,int M2,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V2>>
                int find(const U& value1,const V& value2,const V2& value3,
                         Prd __prd = Prd(),Prd2 __prd2 = Prd2(),
                         Prd3 __prd3 = Prd3()){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        int i = 0;
                        try{
                                for (auto it : m_data){
                                        U t   = std::get<N>(it);
                                        V v   = std::get<M>(it);
                                        V2 v2 = std::get<M2>(it);
                                        if (__prd(t,value1) && __prd2(v,value2)&&
                                                __prd3(v2,value3))
                                        {
                                                return i;
                                        }
                                        ++i;
                                }
                        }
                        catch (...)
                        {
                        }
                        return -1;
                }

                //
                // 查找满足4个条件的项
                //
                template<class U, int N,class V,int M,class V2,int M2,class V3,int M3,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V2>,class Prd4 = MEquals<V3>>
                int find(const U& value1,const V& value2,
                         const V2& value3,const V3& value4,
                         Prd __prd = Prd(),Prd2 __prd2 = Prd2(),
                         Prd3 __prd3 = Prd3(),Prd4 __prd4 = Prd4()){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        int i = 0;
                        try{
                                for (auto it : m_data){
                                        U t   = std::get<N>(it);
                                        V v   = std::get<M>(it);
                                        V2 v2 = std::get<M2>(it);
                                        V3 v3 = std::get<M3>(it);
                                        if (__prd(t,value1) && __prd2(v,value2)&&
                                                __prd3(v2,value3) && __prd4(v3,value4))
                                        {
                                                return i;
                                        }
                                        ++i;
                                }
                        }
                        catch (...)
                        {
                        }
                        return -1;
                }

                //
                // 查找满足5个条件的项
                //
                template<class U, int N,class V,int M,class V2,int M2,class V3,int M3,
                         class V4,int M4,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V2>,class Prd4 = MEquals<V3>,
                         class Prd5 = MEquals<V4>>
                int find(const U& value1,const V& value2,
                         const V2& value3,const V3& value4,
                         const V4& value5,
                         Prd __prd = Prd(),Prd2 __prd2 = Prd2(),
                         Prd3 __prd3 = Prd3(),Prd4 __prd4 = Prd4(),
                         Prd5 __prd5 = Prd5()){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        int i = 0;
                        try{
                                for (auto it : m_data){
                                        U t   = std::get<N>(it);
                                        V v   = std::get<M>(it);
                                        V2 v2 = std::get<M2>(it);
                                        V3 v3 = std::get<M3>(it);
                                        V4 v4 = std::get<M4>(it);
                                        if (__prd(t,value1) && __prd2(v,value2)&&
                                                __prd3(v2,value3) && __prd4(v3,value4)&&
                                                __prd5(v4,value5))
                                        {
                                                return i;
                                        }
                                        ++i;
                                }
                        }
                        catch (...)
                        {
                        }
                        return -1;
                }

                //
                // 查找满足6个条件的项
                //
                template<class U, int N,class V,int M,class V2,int M2,class V3,int M3,
                         class V4,int M4,class V5,int M5,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V2>,class Prd4 = MEquals<V3>,
                         class Prd5 = MEquals<V4>,class Prd6 = MEquals<V5>>
                int find(const U& value1,const V& value2,
                         const V2& value3,const V3& value4,
                         const V4& value5,const V5& value6,
                         Prd __prd = Prd(),Prd2 __prd2 = Prd2(),
                         Prd3 __prd3 = Prd3(),Prd4 __prd4 = Prd4(),
                         Prd5 __prd5 = Prd5(),Prd6 __prd6 = Prd6())
                {
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        int i = 0;
                        try{
                                for (auto it : m_data){
                                        U t   = std::get<N>(it);
                                        V v   = std::get<M>(it);
                                        V2 v2 = std::get<M2>(it);
                                        V3 v3 = std::get<M3>(it);
                                        V4 v4 = std::get<M4>(it);
                                        V5 v5 = std::get<M5>(it);
                                        if (__prd(t,value1) && __prd2(v,value2)&&
                                                __prd3(v2,value3) && __prd4(v3,value4)&&
                                                __prd5(v4,value5) && __prd6(v5,value6))
                                        {
                                                return i;
                                        }
                                        ++i;
                                }
                        }
                        catch (...)
                        {
                        }
                        return -1;
                }

                //
                // 更新满足1条件的项
                //
                template<class U,int N,class Prd = MEquals<U>>
                void update(const U& value,const value_type& val,
                            Prd __prd = Prd())
                {
                    int i = find<U,N,Prd>(value,__prd);
                    if(i!=-1){
                        m_data[i] = val;
                    }
                    else{
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        m_data.push_back(val);
                    }
                }

                //
                // 更新满足2条件的项
                //
                template<class U,int N,class V,int M,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>>
                void update(const U& value1,const V& value2,
                            const value_type& val,
                            Prd __prd = Prd(),Prd2 __prd2 = Prd2())
                {
                    int i = find<U,N,V,M,Prd,Prd2>(value1,value2,__prd,__prd2);
                    if(i!=-1){
                        m_data[i] = val;
                    }
                    else{
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        m_data.push_back(val);
                    }
                }

                //
                // 更新满足3条件的项
                //
                template<class U,int N,class V,int M,class V2,int M2,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V2>>
                void update(const U& value1,const V& value2,const V2& value3,
                            const value_type& val,
                            Prd __prd = Prd(),Prd2 __prd2 = Prd2(),
                            Prd3 __prd3 = Prd3())
                {
                    int i = find<U,N,V,M,V2,M2,Prd,Prd2,Prd3>(value1,value2,value3,
                                                              __prd,__prd2,__prd3);
                    if(i!=-1){
                        m_data[i] = val;
                    }
                    else{
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        m_data.push_back(val);
                    }
                }

                //
                // 更新满足4条件的项
                //
                template<class U,int N,class V,int M,class V2,int M2,class V3,int M3,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V2>,class Prd4 = MEquals<V3>>
                void update(const U& value1,const V& value2,const V2& value3,const V3& value4,
                            const value_type& val,
                            Prd __prd = Prd(),Prd2 __prd2 = Prd2(),
                            Prd3 __prd3 = Prd3(),Prd4 __prd4 = Prd4())
                {
                    int i = find<U,N,V,M,V2,M2,V3,M3,Prd,Prd2,Prd3,Prd4>(value1,value2,value3,value4,
                                                              __prd,__prd2,__prd3,__prd4);
                    if(i!=-1){
                        m_data[i] = val;
                    }
                    else{
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        m_data.push_back(val);
                    }
                }

                //
                // 更新满足5个条件的项
                //
                template<class U, int N,class V,int M,class V2,int M2,class V3,int M3,
                         class V4,int M4,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V2>,class Prd4 = MEquals<V3>,
                         class Prd5 = MEquals<V4>>
                void update(const U& value1,const V& value2,
                         const V2& value3,const V3& value4,
                         const V4& value5,const value_type& val,
                         Prd __prd = Prd(),Prd2 __prd2 = Prd2(),
                         Prd3 __prd3 = Prd3(),Prd4 __prd4 = Prd4(),
                         Prd5 __prd5 = Prd5())
                {
                    int i = find<U,N,V,M,V2,M2,V3,M3,V4,M4,Prd,Prd2,Prd3,Prd4,Prd5>
                            (value1,value2,value3,value4,value5,
                             __prd,__prd2,__prd3,__prd4,__prd5);
                    if(i!=-1){
                        m_data[i] = val;
                    }
                    else{
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        m_data.push_back(val);
                    }
                }

                //
                // 更新满足6个条件的项
                //
                template<class U, int N,class V,int M,class V2,int M2,class V3,int M3,
                         class V4,int M4,class V5,int M5,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V2>,class Prd4 = MEquals<V3>,
                         class Prd5 = MEquals<V4>,class Prd6 = MEquals<V5>>
                void update(const U& value1,const V& value2,
                         const V2& value3,const V3& value4,
                         const V4& value5,const V5& value6,
                         const value_type& val,
                         Prd __prd = Prd(),Prd2 __prd2 = Prd2(),
                         Prd3 __prd3 = Prd3(),Prd4 __prd4 = Prd4(),
                         Prd5 __prd5 = Prd5(),Prd6 __prd6 = Prd6())
                {
                    int i = find<U,N,V,M,V2,M2,V3,M3,V4,M4,V5,M5,Prd,Prd2,Prd3,Prd4,Prd5>
                            (value1,value2,value3,value4,value5,value6,
                             __prd,__prd2,__prd3,__prd4,__prd5,__prd6);
                    if(i!=-1){
                        m_data[i] = val;
                    }
                    else{
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        m_data.push_back(val);
                    }
                }


                //
		// 清除整个数据
                //
		void clear(){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			m_data.clear();
		}

                //
		// 查看数据是否为空
                //
		bool empty(){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			return m_data.empty();
		}

                bool empty() const{
                    return m_data.empty();
                }

                //
		// 迭代器操作
                //
		iterator begin(){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			return m_data.begin();
		}

                const_iterator begin() const{
                    return m_data.begin();
                }

		iterator end(){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			return m_data.end();
		}

                const_iterator end() const{
                    return m_data.end();
                }

                //
                // 删除指定元素
                //
		void erase(iterator it){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			if (it != m_data.end())
				m_data.erase(it);
		}

                //
                // 删除满足条件的第一项
                //
                template<class U,int N,class Prd = MEquals<U>>
                void erase(const U& value,Prd __prd = Prd()){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			auto it = std::find_if(m_data.begin(), m_data.end(), [&](value_type v){
                                return __prd(value,std::get<N>(v));
			});
			if (it != m_data.end()){
				m_data.erase(it);
			}
		}

                //
                // 删除满足1条件的所有项
                //
                template<class U,int N,class Prd = MEquals<U>>
                bool eraseall(const U& value,Prd __prd = Prd()){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
                        iterator  __it = m_data.begin();
                        bool isErase = false;
                        while(__it != m_data.end()){
                            iterator it = std::find_if(__it, m_data.end(), [&](value_type v){
                                    return __prd(value,std::get<N>(v));
                            });
                            if (it != m_data.end()){
                                    it = m_data.erase(it);
                                    isErase = true;
                            }
                            __it = it;
                        }
                        return isErase;
                }


                //
                // 删除满足2个条件的所有项
                //
                template<class U,int N,class V,int M,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>>
                bool eraseall(const U& value1,const V& value2,Prd __prd = Prd(),Prd2 __prd2 = Prd2()){
                    std::unique_lock<std::recursive_mutex> lock(m_mutex);
                    iterator  __it = m_data.begin();
                    bool isErase = false;
                    while(__it != m_data.end()){
                        iterator it = std::find_if(__it, m_data.end(), [&](value_type v){
                                return __prd(value1,std::get<N>(v)) && __prd2(value2,std::get<M>(v));
                        });
                        if (it != m_data.end()){
                                it = m_data.erase(it);
                                isErase = true;
                        }
                        __it = it;
                    }
                    return isErase;
                }

                //
                // 删除满足3个条件的所有项
                //
                template<class U,int N,class V,int M,class V2,int M2,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,class Prd3 = MEquals<V>>
                bool eraseall(const U& value1,const V& value2,const V2& value3,
                              Prd __prd = Prd(),Prd2 __prd2 = Prd2(),Prd3 __prd3 = Prd3()){
                    std::unique_lock<std::recursive_mutex> lock(m_mutex);
                    iterator  __it = m_data.begin();
                    bool isErase = false;
                    while(__it != m_data.end()){
                        iterator it = std::find_if(__it, m_data.end(), [&](value_type v){
                                return __prd(value1,std::get<N>(v)) &&
                                __prd2(value2,std::get<M>(v)) &&
                                __prd3(value3,std::get<M2>(v));
                        });
                        if (it != m_data.end()){
                                it = m_data.erase(it);
                                isErase = true;
                        }
                        __it = it;
                    }
                    return isErase;
                }

                //
                // 删除满足4个条件的所有项
                //
                template<class U,int N,class V,int M,class V2,int M2,class V3,int M3,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V>,class Prd4 = MEquals<V>>
                bool eraseall(const U& value1,const V& value2,const V2& value3,const V3& value4,
                              Prd __prd = Prd(),Prd2 __prd2 = Prd2(),
                              Prd3 __prd3 = Prd3(),Prd4 __prd4 = Prd4()){
                    std::unique_lock<std::recursive_mutex> lock(m_mutex);
                    iterator  __it = m_data.begin();
                    bool isErase = false;
                    while(__it != m_data.end()){
                        iterator it = std::find_if(__it, m_data.end(), [&](value_type v){
                                return __prd(value1,std::get<N>(v)) &&
                                __prd2(value2,std::get<M>(v)) &&
                                __prd3(value3,std::get<M2>(v)) &&
                                __prd4(value4,std::get<M3>(v));
                        });
                        if (it != m_data.end()){
                                it = m_data.erase(it);
                                isErase = true;
                        }
                        __it = it;
                    }
                    return isErase;
                }

                //
                // 删除满足5个条件的所有项
                //
                template<class U, int N,class V,int M,class V2,int M2,class V3,int M3,
                         class V4,int M4,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V2>,class Prd4 = MEquals<V3>,
                         class Prd5 = MEquals<V4>>
                bool eraseall(const U& value1,const V& value2,
                              const V2& value3,const V3& value4,
                              const V4& value5,
                              Prd __prd = Prd(),Prd2 __prd2 = Prd2(),
                              Prd3 __prd3 = Prd3(),Prd4 __prd4 = Prd4(),
                              Prd5 __prd5 = Prd5())
                {
                    std::unique_lock<std::recursive_mutex> lock(m_mutex);
                    iterator  __it = m_data.begin();
                    bool isErase = false;
                    while(__it != m_data.end()){
                        iterator it = std::find_if(__it, m_data.end(), [&](value_type v){
                                return __prd(value1,std::get<N>(v)) &&
                                __prd2(value2,std::get<M>(v)) &&
                                __prd3(value3,std::get<M2>(v)) &&
                                __prd4(value4,std::get<M3>(v)) &&
                                __prd5(value5,std::get<M4>(v));
                        });
                        if (it != m_data.end()){
                                it = m_data.erase(it);
                                isErase = true;
                        }
                        __it = it;
                    }
                    return isErase;
                }

                //
                // 删除满足6个条件的所有项
                //
                template<class U, int N,class V,int M,class V2,int M2,class V3,int M3,
                         class V4,int M4,class V5,int M5,
                         class Prd = MEquals<U>,class Prd2 = MEquals<V>,
                         class Prd3 = MEquals<V2>,class Prd4 = MEquals<V3>,
                         class Prd5 = MEquals<V4>,class Prd6 = MEquals<V5>>
                bool eraseall(const U& value1,const V& value2,
                              const V2& value3,const V3& value4,
                              const V4& value5,const V5& value6,
                              Prd __prd = Prd(),Prd2 __prd2 = Prd2(),
                              Prd3 __prd3 = Prd3(),Prd4 __prd4 = Prd4(),
                              Prd5 __prd5 = Prd5(),Prd6 __prd6 = Prd6())
                {
                    std::unique_lock<std::recursive_mutex> lock(m_mutex);
                    iterator  __it = m_data.begin();
                    bool isErase = false;
                    while(__it != m_data.end()){
                        iterator it = std::find_if(__it, m_data.end(), [&](value_type v){
                                return __prd(value1,std::get<N>(v)) &&
                                __prd2(value2,std::get<M>(v)) &&
                                __prd3(value3,std::get<M2>(v)) &&
                                __prd4(value4,std::get<M3>(v)) &&
                                __prd5(value5,std::get<M4>(v)) &&
                                __prd6(value6,std::get<M5>(v));
                        });
                        if (it != m_data.end()){
                                it = m_data.erase(it);
                                isErase = true;
                        }
                        __it = it;
                    }
                    return isErase;
                }

                //
                // 删除指定项
                //
		void erase(value_type value){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			auto it = std::find_if(m_data.begin(), m_data.end(), [&](value_type v){
				return value == v;
			});
			if (it != m_data.end()){
				m_data.erase(it);
			}
		}

                //
                // 删除指定行
                //
		void erase(int row){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			if (m_data.size() <= row)
				return;
			m_data.erase(m_data.begin() + row);
		}

                //
		// 删除一个区间
                //
		void erase(iterator first, iterator end){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			m_data.erase(first, end);
		}

		void erase(int startrow, int endrow){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			if (startrow <= 0)
				startrow = 0;
			if (endrow >= m_data.size())
				endrow = m_data.size();
			m_data.erase(m_data.begin()+startrow, m_data.begin() + endrow);
		}

                //
		// 改变空间大小
                //
		void reserve(size_t size){
                        std::unique_lock<std::recursive_mutex> lock(m_mutex);
			m_data.reserve(size);
		}

                //
                // 改变数据大小
                //
                void resize(size_t size){
                    std::unique_lock<std::recursive_mutex> lock(m_mutex);
                    m_data.resize(size);
                }

                //
		// 使用流对容器进行输出
                //
		friend std::ostream& operator<<(std::ostream& os, const HContainer& c){
			os << "[ row:" << c.m_data.size() << " colum:" << std::tuple_size<value_type>::value << " ]" << std::endl;
			std::for_each(c.m_data.begin(), c.m_data.end(), [&](tuple_data data){
				tuple_print(data, os);
				os << std::endl;
                        });
			return os;
		}

                //
                // 读写文件
                //
                void WriteToFile(const std::string& FileName,const std::string& spliter = "#"){
                    std::ofstream outFile(FileName);
                    if(outFile.fail())
                          return;
                    for(auto& it : *this){
                        std::string str = TupleToStr(it,spliter);
                        outFile<<str<<std::endl;
                    }
                    outFile.close();
                }

                void ReadFromFile(const std::string& FileName,const std::string& spliter = "#"){
                    std::ifstream inFile(FileName);
                    if(inFile.fail())
                        return;
                    std::string str;
                    this->clear();
                    while(std::getline(inFile,str) && !inFile.eof()){
                        if(str.empty())
                            continue;
                        value_type v;
                        Totuple(str,v,spliter);
                        m_data.push_back(v);
                    }
                    inFile.close();
                }



	private:
		std::vector<tuple_data> m_data;
                std::recursive_mutex    m_mutex;
        };
}
