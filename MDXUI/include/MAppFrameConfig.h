
///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef __MAPI_DEBUG__
#define MSTRING_API 
#else
#if defined(DIRECTUI_EXPORTS)
#define MAPPFRAME_API __declspec(dllexport)
#else
#define MAPPFRAME_API __declspec(dllimport)
#endif
#endif 


#include <mtlobject.h>

