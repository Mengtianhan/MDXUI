

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
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


