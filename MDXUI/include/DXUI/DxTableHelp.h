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