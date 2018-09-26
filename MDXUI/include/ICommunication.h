

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

class DXUI_API ICommunication : public TL::MTLObject
{
public:
    ICommunication();
    virtual ~ICommunication();
public:
    virtual MString ToString() const;

    ///
    /// \brief Connect
    /// \return
    /// 连接操作
    ///
    virtual bool Connect();

    ///
    /// \brief Close
    /// \return
    /// 断开连接
    ///
    virtual void Close();

    ///
    /// \brief IsConnect
    /// \return
    /// 检查连接状态
    ///
    virtual bool IsConnect();

    ///
    /// \brief Send
    /// \param msg
    /// \param len
    /// \return
    /// 发送数据
    /// 该函数需要根据具体的通信模块重新,系统自己调用
    ///
    virtual bool Send(const char* msg,__int64 len);


    ///
    /// \brief Read
    /// \param msg
    /// \param len
    /// \return
    /// 接收数据,该函数不需要被重写,只需要被执行即可
    ///
    void Read(const char* msg,__int64 len);

    ///
    /// \brief setConnectStatus
    /// \param isok
    /// 手动设置通信模块状态
    ///
    void setConnectStatus(bool isok);
};


