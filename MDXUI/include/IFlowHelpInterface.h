
///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
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