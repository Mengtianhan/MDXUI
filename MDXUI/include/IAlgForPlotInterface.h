

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution
// Copyright (C) 2017-2018 by Mengjin (sh06155@hotmail.com)
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

#pragma once
#include <MToolLib>

//
// 特殊用途的辅助算法接口
//
namespace DxUI{
	class CDxWindow;
	class CDxSpinBox;
	class CDxWidget;
}

struct IAlgForPlotInterace{
	virtual ~IAlgForPlotInterace(){};

	virtual MString Describe() const{ return ""; }
	virtual IAlgForPlotInterace* Clone(){ return new IAlgForPlotInterace; }
	virtual DxUI::CDxWidget* ParamSetWindow(){ return nullptr; }
	virtual void Init(){}
	virtual void Exec(const TL::Vector<double>& xs, const TL::Vector<double>& ys){}
	const TL::Vector<std::pair<MString, MString>>& Result() const{
		return mResult;
	}

protected:
	TL::Vector<std::pair<MString, MString>> mResult;
};