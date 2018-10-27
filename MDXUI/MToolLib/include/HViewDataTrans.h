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


//-----------------------------------------------------
// 用于HViewItem的数据转换
//----------------------------------------------------
#pragma once
#include <vector>
#include <HMatrix2.h>
#include "HViewColorMap.h"

template<typename T1,typename T2,typename T3>
class HViewDataTrans
{
public:
	HViewDataTrans(){}
	~HViewDataTrans(){}

	void clear(){
		m_points.clear();
	}

	void push_back(T1 x, T2 y, T3 imag){
		m_points.push_back(x, y, imag);
	}

	void setColorMapModel(bool isFix){
		b_is_fix_color = isFix;
	}

	void setColorMapRange(T3 __min, T3 __max){
		m_color_map.setMapRange(__min, __max);
	}

        mj::HMatrix<T1, T2, HViewColor> getData(){
		hlw::HMatrix<T1, T2, HViewColor> _result;
		if (!b_is_fix_color){
			std::vector<T3> imags = m_points.getColumdata<T3, 2>();
			auto minmax = std::minmax_element(imags.begin(), imags.end());
			T3 __min = *(minmax.first);
			T3 __max = *(minmax.second);
			m_color_map.setMapRange(__min, __max);
		}
		
    	std::transform(m_points.begin(), m_points.end(), std::back_inserter(_result),\
			[&](hlw::HMatrix<T1, T2, T3>::value_type v)->hlw::HMatrix<T1, T2, HViewColor>::value_type{
			T1 x = m_points.get_element<T1,0>(v);
			T2 y = m_points.get_element<T2, 1>(v);
			T3 imag = m_points.get_element<T3, 2>(v);
			HViewColor c = m_color_map.getColorFromOther(imag);
			return hlw::HMatrix<T1, T2, HViewColor>::make_value(x, y, c);
		});
		return _result;
	}
private:
        mj::HMatrix<T1,T2,T3> m_points;    // 储存所有unit测试信息 ，x,y,imag
	HViewColorMap<T3>    m_color_map; // 颜色映射
	bool				 b_is_fix_color{ false };
};

