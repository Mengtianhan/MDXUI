

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "DxConfig.h"

//
// 协助处理信号的东西最好是继承至该类,因为这样可以使用HAppManager协助管理
//
class DXUI_API MAppEventHandle : public TL::MTLObject
{
public:
	MAppEventHandle();
	virtual ~MAppEventHandle();
    virtual void Destroy();

    ///
    /// \brief OnStart
    ///
    virtual void OnStart();

    ///
    /// \brief OnPause
    ///
    virtual void OnPause();

    ///
    /// \brief OnStop
    ///
    virtual void OnStop();
};

