

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
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


