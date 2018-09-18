

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DxConfig.h"

class DXUI_API MTreeEvent
{
public:
	MTreeEvent(){}
	virtual ~MTreeEvent(){}
    virtual void Clicked(const char* iteminfo,__int64 size){}
    virtual void DoubleClicked(const char* iteminfo,__int64 size){}
    virtual void RightClicked(const char* iteminfo,__int64 size,const DxUI::DxPointI& pos){}
};
