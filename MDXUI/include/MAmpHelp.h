
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


#pragma  once
#include <amp.h>
#include <amp_math.h>
namespace mj{
	static inline float fast_interp(float x1, float y1, float x2, float y2, float x3)__GPU_ONLY{
		if (concurrency::fast_math::fabsf(x1 - x2) < 1e-12){
			return y1;
		}
		if (concurrency::fast_math::fabsf(y1 - y2) < 1e-12){
			return y1;
		}
		return (y2 - y1) / (x2 - x1)*(x3 - x2) + y2;
	}
}
