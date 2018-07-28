
//************************************************************
// HVector.h 主要用于协助标准库的vector的操作
//************************************************************
#pragma once
#include <iterator>
#include <iostream>
#ifdef USE_VECTORHELP
namespace mj{

	template<typename T, typename A, template<typename T1, typename A1>class C>
	inline C<T, A>& operator<<(C<T, A>& c, const T& value){
		c.push_back(value);
		return c;
	}

	template<typename T, typename A, template<typename T1, typename A1>class C>
	inline C<T, A>& operator<<(C<T, A>& c,const C<T, A>& c2){
		std::copy(c2.begin(), c2.end(), std::back_inserter(c));
		return c;
	}

	template<typename T, typename A, template<typename T1, typename A1>class C>
	inline C<T, A>& operator>>(C<T, A>& c, T& value){
		if (c.empty()){
			value = T();
		}
		else{
			value = c.back();
			c.pop_back();
		}
		return c;
	}

	template<typename T, typename A, template<typename T1, typename A1> class C>
	inline std::ostream& operator<<(std::ostream& os, const C<T, A>& c){
		if (c.empty()){
			return os;
		}

		std::copy(c.begin(), --c.end(), std::ostream_iterator<T>(os, " "));
		os << *(--c.end());
		return os;
	}

        template<typename T, typename A, template<typename T1, typename A1> class C>
	inline std::istream& operator>>(std::istream& is, C<T, A>& c){
		c.clear();
		c.assign((std::istream_iterator<T>(is)), std::istream_iterator<T>());
		return is;
	}

        //--------------------------------------------------
        // 两个不同系列之间的转换
        //--------------------------------------------------
        template<typename T, typename AL,
                template<typename U, typename A> class C,
                 template<typename U2,typename A2> class C2>
        inline C<T,AL> transe(const C2<T,AL>& c2){
            C<T,AL> c1(c2.begin(),c2.end());
            return c1;
        }


	//----------------------------------------------
	// 两系列相加
	//----------------------------------------------
	template<typename T, typename AL,
		template<typename U, typename A> class C>
        inline C<T, AL> plus(const C<T, AL>& v1, const C<T, AL>& v2,bool isphase = false){
		C<T, AL> result;
		if (v1.size() != v2.size()){
                        return v1;
		}
                if(isphase){
                    std::transform(v1.begin(), v1.end(), v2.begin(),std::back_inserter(result), [&](T t1, T t2)->T{
                        T t = t1 + t2;
                        if(t > 180){
                            t = t - 360;
                        }
                        else if(t < -180){
                            t = t+360;
                        }
                        return t;
                    });
                }
                else{
                    std::transform(v1.begin(), v1.end(), v2.begin(), \
                                   std::back_inserter(result), \
                                   [&](T t1, T t2)->T{return t1 + t2; });
                }

                return result;
	}

	template<typename T, typename AL,
		template<typename U, typename A> class C>
        inline C<T, AL> operator+(const C<T, AL>& v1, const C<T, AL>& v2){
                return plus(v1, v2);
	}

	//====================================================
	//两系列相减
	//====================================================
	template<typename T, typename AL,
		template<typename U, typename A> class C>
        inline C<T, AL> minus(const C<T, AL>& v1, const C<T, AL>& v2,bool isphase = false){
		C<T, AL> result;
		if (v1.size() != v2.size()){
                        return v1;
		}
                if(isphase){
                    std::transform(v1.begin(), v1.end(), v2.begin(),std::back_inserter(result), [&](T t1, T t2)->T{
                        T t = t1 - t2;
                        if(t > 180){
                            t = t - 360;
                        }
                        else if(t < -180){
                            t = t+360;
                        }
                        return t;
                    });
                }
                else{
                    std::transform(v1.begin(), v1.end(), v2.begin(), \
                                   std::back_inserter(result), \
                                   [&](T t1, T t2)->T{return t1 - t2; });
                }
		return result;
	}

	template<typename T, typename AL,
		template<typename U, typename A> class C>
        inline C<T, AL> operator-(const C<T, AL>& v1, const C<T, AL>& v2){
		return minus(v1,v2);
	}

	//--------------------------------------------------
	// 一个系列加上一个数
	//--------------------------------------------------
	template<typename T, typename AL,
		template<typename U, typename A> class C>
        inline C<T, AL> plus(const C<T, AL>& v1,const T& v2,bool isphase = false){
		C<T, AL> result;
                if(isphase){
                    std::transform(v1.begin(), v1.end(), std::back_inserter(result), [&](T t1)->T{
                        T t = t1 + v2;
                        if(t > 180){
                            t = t - 360;
                        }
                        else if(t < -180){
                            t = t+360;
                        }
                        return t;
                    });
                }
                else{
                    for(int i=0;i<v1.size();++i){
                        result.push_back(v1[i] + v2);
                    }
                }
		return result;
	}

	template<typename T, typename AL,
		template<typename U, typename A> class C>
        inline C<T, AL> operator+(const C<T, AL>& v1, const T& v2){
		return plus(v1,v2);
	}

	//--------------------------------------------------
	// 一个系列减去一个数
	//--------------------------------------------------
	template<typename T, typename AL,
		template<typename U, typename A> class C>
        inline C<T, AL> minus(const C<T, AL>& v1, const T& v2,bool isphase = false){
		C<T, AL> result;
                if(isphase){
                    std::transform(v1.begin(), v1.end(), std::back_inserter(result), [&](T t1)->T{
                        T temp =  t1 - v2;
                        if(temp>180){
                            temp = temp - 360;
                        }
                        else if(temp <-180){
                            temp = temp + 360;
                        }
                        return temp;
                    });

                }
                else{
                    std::transform(v1.begin(), v1.end(), std::back_inserter(result), \
                                   [&](T t1)->T{return t1 - v2; });
                }
                return result;
	}

	template<typename T, typename AL,
		template<typename U, typename A> class C>
        inline C<T, AL> operator-(const C<T, AL>& v1, const T& v2){
		return minus(v1, v2);
	}

	//--------------------------------------------------
	// 一个系列乘以一个数
	//--------------------------------------------------
	template<typename T, typename AL,
		template<typename U, typename A> class C>
        inline C<T, AL> multiple(const C<T, AL>& v1, const T& v2){
		C<T, AL> result;
		std::transform(v1.begin(), v1.end(), std::back_inserter(result), [&](T t1)->T{return t1*v2; });
		return result;
	}

	template<typename T, typename AL,
		template<typename U, typename A> class C>
        inline C<T, AL> operator*(const C<T, AL>& v1, const T& v2){
		return multiple(v1, v2);
	}

        //--------------------------------------------------
        // 两个数对一个系列进行操作
        //--------------------------------------------------
        template<typename T, typename AL,
                template<typename U, typename A> class C>
        inline C<T, AL> plus(const C<T, AL>& v1, const T& v2,const T& v3,bool isphase = false){
                C<T, AL> result;
                int k = 1;
                if(v1.size()%2 == 0){
                    k = 0;
                }
                for(int i=0;i<v1.size()-k; i+=2){
                    if(isphase){
                        T temp = v1.at(i+1)+v3;
                        if(temp>180){
                            temp = temp - 360;
                        }
                        else if(temp <-180){
                            temp = temp + 360;
                        }
                        result<<v1.at(i)+v2<<temp;
                    }
                    else{

                        result<<v1.at(i)+v2<<v1.at(i+1)+v3;
                    }
                }
                return result;
        }

        template<typename T, typename AL,
                template<typename U, typename A> class C>
        inline C<T, AL> minus(const C<T, AL>& v1, const T& v2,const T& v3,bool isphase = false){
                C<T, AL> result;
                int k = 1;
                if(v1.size()%2 == 0){
                    k = 0;
                }
                for(int i=0;i<v1.size()-k; i+=2){
                    if(isphase){
                        T temp = v1.at(i+1)-v3;
                        if(temp>180){
                            temp = temp - 360;
                        }
                        else if(temp <-180){
                            temp = temp + 360;
                        }
                        result<<v1.at(i)-v2<<temp;
                    }
                    else{
                        result<<v1.at(i)-v2<<v1.at(i+1)-v3;
                    }
                }
                return result;
        }

        //==================================================
        // 获取系列的最后一个元素
        //===================================================
        template<typename K,typename V,typename C,typename A,
                 template<typename K1,typename V1,typename C1,typename A1> class M>
        typename M<K,V,C,A>::iterator last(M<K,V,C,A>& m){
            return --m.end();
        }

        template<typename T,typename A,
                 template<typename T1,typename A1> class C>
        typename C<T,A>::iterator last(C<T,A>& c){
            return --c.end();
        }

        template<typename T,typename C,typename A,
                 template<typename T1,typename C1,typename A1> class S>
        typename S<T,C,A>::iterator last(S<T,C,A>& s){
            return --s.end();
        }

}
#endif