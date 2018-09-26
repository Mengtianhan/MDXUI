

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


