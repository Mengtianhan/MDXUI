

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "DxConfig.h"

class MProjectType;
///
/// \brief The IDealDataInterface class
/// 处理数据接口类
///
class DXUI_API IDealDataInterface : public TL::MTLObject
{
public:
    IDealDataInterface();
    ~IDealDataInterface();

public:
    virtual MString ToString() const;

    ///
    /// \brief Init
    /// \param __Type
    /// 初始化
    ///
	virtual void Init(MProjectType* __Type);

    ///
    /// \brief Destroy
    /// 销毁相关信息
    ///
    virtual void Destroy();

    ///
    /// \brief PushMessage
    /// \param msg
    ///
    virtual void PushMessage(const MString& msg);
};


