
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


//+------------------------------------------------------------------------------------------
//
// 流程测试的辅助接口
// 该接口配合MAppDAta使用
// 为了简单点
// 可以从MAppManager::AppData获取该接口
// 所以实际使用中如果需要获取该接口那么就要使用多重继承：
// class MyData : public MAppData,public IFlowHelpInterface{};
// IFlowHelpInterface* FlowHelpPtr = MAppManager::Instance()->AppData<IFlowHelpInterface>();
//
//+-------------------------------------------------------------------------------------------
struct IFlowHelpInterface
{
public:

	//
	// 单点测试流程接口
	//
	virtual void doSubrayFlow(){}

	//
	// 信噪比测试接口
	//
	virtual void doSNTestFlow(){}

	//
	// 稳定性测试接口
	//
	virtual void doStabilityFlow(){}

	//
	// 通道校准接口
	//
	virtual void doChannelCalFlow(){}

	//
	// 探头校准接口
	//
	virtual void doProbeCalFlow(){}


	//
	// 重测指定的ray接口
	//
	virtual void doRetestRayFlow(){}

	//
	// 断点续测接口
	//
	virtual void doContinueTestFlow(){}
};