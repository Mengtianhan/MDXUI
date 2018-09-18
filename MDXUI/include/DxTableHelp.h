

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <MTypeTool.h>

namespace DxUI{
	//+------------------------
	//
	// table 选择辅助结构
	// mSelectedCols 选中的行号
	// mData 数据
	// int ----> 列号
	// MPairType<int, MString>  行号 + 数据
	//
	//+-------------------------
	struct CDxTableSelected{
		TL::Vector<int>		mSelectedCols;  
		TL::Map< int, TL::Vector<std::pair<int, MString>>> mData;
	};
}