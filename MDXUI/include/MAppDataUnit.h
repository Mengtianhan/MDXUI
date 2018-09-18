

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "DxConfig.h"


namespace DxUI{
	class MSerialize;
}

//
// class MAppDataUnit
// 数据单元
//
class DXUI_API MAppDataUnit
{
public:
	MAppDataUnit();
	~MAppDataUnit();

	///
	/// \brief ValidateData
	/// \return
	/// 校验数据的有效性
	///
	virtual bool ValidateData(){ return true; }

	///
	/// \brief InitData
	/// 初始化数据
	///
	virtual void InitData(){}

	///
	/// \brief InitConnect
	/// 初始化信号连接
	///
	virtual void InitConnect(){}


	///
	/// \brief ToString
	/// \return
	/// 管用接口
	/// 返回类型的一些相关信息
	/// 比如说在类型操作失败的时候需要知道是哪一个类型出错
	///
	virtual MString ToString(){ return "HAppDataBase"; }



	///
	/// \brief SerialToFile
	/// \param Serial
	/// 将数据写进 MSerialize
	///
	virtual void SerialToFile(DxUI::MSerialize* Serial){}

	///
	/// \brief SerialFromFile
	/// \param Serial
	/// 从 MSerialize 中读取数据
	///
	virtual void SerialFromFile(DxUI::MSerialize* Serial){}

};