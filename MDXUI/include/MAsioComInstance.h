

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
#include <IClientNet.h>
#include <ICoroutClientNet.h>
#include <ISerialPort.h>
#include <ICoroutSerialPort.h>
#include <IServiceNet.h>
#include <ICoroutServiceNet.h>
#include <ITimer.h>
#include <IFileMap.h>
#include <IFileSystem.h>
#include <IUDPClient.h>
#include <IUDPService.h>
#include <IThread.h>
#include <IXml.h>
#include <IMLogger.h>
#include <IMSql.h>


class DXUI_API MAsioComInstance
{
public:
    MAsioComInstance();
    ~MAsioComInstance();

    ///
    /// \brief InitCom
    /// \param dllName
    ///
    static void InitCom(const MString& dllName = "");


    static std::shared_ptr<IServiceNet> CreateServiceNet();
    static std::shared_ptr<ICoroutServiceNet> CreateCoroutServiceNet();
    static std::shared_ptr<IClientNet>  CreateClientNet();
    static std::shared_ptr<ICoroutClientNet>  CreateCoroutClientNet();
    static std::shared_ptr<ISerialPort> CreateSerialPort();
    static std::shared_ptr<ICoroutSerialPort> CreateCoroutSerialPort();
    static std::shared_ptr<IUDPService> CreateUDPService();
    static std::shared_ptr<IUDPClient> CreateUDPClient();
    static std::shared_ptr<IMLogger> CreateLogger();
    static std::shared_ptr<ITimer> CreateTimer();
    static std::shared_ptr<IFileMap> CreateFileMap();
    static std::shared_ptr<IXml> CreateXmlPtr();
    static std::shared_ptr<IThread> CreateThreadPtr();
    static std::shared_ptr<IMSql> CreateSqlite();
    static std::shared_ptr<IFileSystem> CreateFileSystem();

private:
    static std::shared_ptr<IClassFactory> pFactory;
};


