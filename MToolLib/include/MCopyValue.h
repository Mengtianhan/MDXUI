#pragma once
#include <type_traits>
#include <sstream>
#include <iostream>

namespace mj{
template<class T,class U>
struct IsConvertible:std::is_convertible<T,U>
{
};

template<bool>
struct MCopyValue{
    template<class T,class U>
    static void apply(T& val1,const U& val2){
        val1 = val2;
    }
};

template<>
struct MCopyValue<false>{
    template<class T,class U>
    static void apply(T& val1,const U& val2){
        std::stringstream ss;
        ss<<val2;
        ss>>val1;
    }

    template<class U>
    static void apply(std::string& str,const U& val2){
        std::stringstream ss;
        ss<<val2;
        str = ss.str();
    }
};

//
//  ½«right¸³ÖµÎªleft
//
template<class T,class U>
inline void copyvalue(T& left,const U& right){
    MCopyValue<IsConvertible<U,T>::value>::apply(left,right);
}
}
