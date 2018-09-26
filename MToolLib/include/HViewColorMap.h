
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



//------------------------------
// HViewColorMap.h

//-------------------------------------------
// 用于映射颜色
//-------------------------------------------
#pragma once
#include <math.h>
#include <cmath>
#ifdef __QT__
#include <QColor>
#endif
#include <iostream>
struct HViewColor{
    unsigned char r;
    unsigned char g;
    unsigned char b;
#ifdef __QT__
    operator QColor(){
        QColor c(r,g,b);
        return c;
    }
#endif
};

namespace mj{
static const HViewColor  red{ 255, 0, 0 };
static const HViewColor  blue{ 0, 0, 255 };
static const  HViewColor  white{ 255, 255, 255 };
static const  HViewColor  green{ 0, 255, 0 };
static const  HViewColor  black{ 0, 0, 0 };

inline HViewColor tocolor(double rad){
    unsigned char base = 255;
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    double r_d = 0;
    double g_d = 0;
    double b_d = 0;
    if(rad > 1.0){
        r = 255;
        g = 0;
        b = 0;
    }
    else if (std::abs(rad - 0.75)<0.005){
        r = 255;
        g = 255;
        b = 0;
    }
    else if(rad>0.75 && rad <= 1.0){
        g_d = rad - 0.75;
        r = 255;
        g = 255 - g_d * 4 * 255;
        b = 0;
    }
    else if (rad > 0.5 && rad <= 0.75){
        r_d = rad - 0.5;
        r = r_d * 4 * 255;
        g = 255;
        b = 0;
    }
    else if (rad > 0.25 && rad <= 0.5){
        b_d = 0.5 - rad;
        r = 0;
        g = 255;
        b = b_d * 4 * 255;
    }
    else if (rad >= 0 && rad <= 0.25){
        g_d = rad;
        r = 0;
        g = g_d * 4 * 255;
        b = 255;
    }
    else if(rad < 0){
        r = 0;
        g = 0;
        b = 255;
    }
    HViewColor _result{ r, g, b };
    return _result;
}
}

template<class T>
class HInteval{
public:
    HInteval(T min_color = T(), T max_color = T()) :
        m_max(max_color), m_min(min_color)
    {
        m_span = m_max - m_min;
    }

    void setMaxValue(T value){
        m_max = value;
        m_span = m_max - m_min;
    }

    void setMinValue(T value){
        m_min = value;
        m_span = m_max - m_min;
    }

    double getRatio(T value){

        T t1 = value - m_min;

        if (std::abs(m_span - 0)<0.05)
            return 0;     
        return double(t1) / (double)m_span;
    }

    void setInteval(T min_color, T max_color) volatile{
        m_max = max_color;
        m_min = min_color;
        m_span = m_max - m_min;
    }
private:
    T	m_max;
    T	m_min;
    T	m_span;
};



template<typename T>
class HViewColorMap{
public:
    HViewColorMap(T min_color = T(), T max_color = T()):
        m_inteval(HInteval<T>(min_color,max_color))
    {

    }
    virtual ~HViewColorMap(){}

    //------------------------------------------------
    // 设置隐射范围
    //------------------------------------------------
    void setMapRange(T min_color, T max_color)
    {
        m_inteval.setInteval(min_color, max_color);
    }

    void setMaxValue(T max_color){
        m_inteval.setMaxValue(max_color);
    }

    void setMinValue(T min_color){
        m_inteval.setMinValue(min_color);
    }

    HViewColor getColorFromfloat(T value) //将数值映射成相应的颜色值
    {
        double rad = m_inteval.getRatio(value);
        if (rad <= 0){
            return mj::blue;
        }
        else if( rad >=1){
            return mj::red;
        }
        return mj::tocolor(rad);
    }

    unsigned long getColor(T value)   //将数值映射成相应的颜色值，不过由unsigned long表示
    {

    }

private:
    HInteval<T> m_inteval;
};
