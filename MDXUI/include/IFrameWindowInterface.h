

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "DxConfig.h"

class DXUI_API IFrameWindowInterface
{
public:
    IFrameWindowInterface();
    virtual ~IFrameWindowInterface();
    virtual void HideToolBar(bool isHide);
    virtual void HideNagTreeWindow(bool isHide);
    virtual void HideOutputWindow(bool isHide);
    virtual void setMaxNagWindowWidth(unsigned width);
    virtual void setMinNagWindowWidth(unsigned width);
    virtual void setMinOutputWindowHeight(unsigned height);
    virtual void setMaxOutputWindowHeight(unsigned height);
};


