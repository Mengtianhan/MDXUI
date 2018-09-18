

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////


#pragma once
#include <comdef.h>

//
// 下面是几个预定义的IID常量
//


//
// 获取服务器模块的IID
//

// {A98E7D90-CC61-435E-9DAA-CE3822B3E559}
__declspec(selectany) IID IID_ISERV =
        { 0xa98e7d90, 0xcc61, 0x435e, { 0x9d, 0xaa, 0xce, 0x38, 0x22, 0xb3, 0xe5, 0x59 } };



//
// 工厂IID
//

// {D479FB5B-00E0-417D-AD5D-446C7CBC1C6A}
__declspec(selectany) IID IID_IFAC =
        { 0xd479fb5b, 0xe0, 0x417d, { 0xad, 0x5d, 0x44, 0x6c, 0x7c, 0xbc, 0x1c, 0x6a } };


//
// 基础实例IID
//

// {BFE2DADD-FB01-48FB-B0AA-AB26E545C1CA}
__declspec(selectany) IID IID_COMBASE =
        { 0xbfe2dadd, 0xfb01, 0x48fb, { 0xb0, 0xaa, 0xab, 0x26, 0xe5, 0x45, 0xc1, 0xca } };
