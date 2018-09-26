

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