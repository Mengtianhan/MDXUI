

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
#include "DxConfig.h"
#include <MAny.h>

class DXUI_API MLoggerStream
{
    typedef std::function<bool(const char*,__int64)> FeedBackFun;
    typedef std::function<bool(const char*,__int64)> ParamFun;
public:
    MLoggerStream();
    virtual ~MLoggerStream();
    virtual MLoggerStream& operator<<(const MString& str);
    virtual MLoggerStream& TransData(const MString& str);
	virtual bool TransData(const char* buffer, __int64 len);
    virtual bool TransParam(const MString& str);
    void SlotForAnyAction(const MString& StrKey,const MAny& action);
private:
    FeedBackFun   mBackFun{nullptr}; // 使用operator 或者log 的话将会执行该操作 标识字段 FeedBack
    FeedBackFun   mDataFun{nullptr}; // 用于传输数据使用 标识字段 SendData
    ParamFun      mParamFun{nullptr}; // 主要用于传输参数 标识字段为 SendParam
};


