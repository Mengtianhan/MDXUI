


///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
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


