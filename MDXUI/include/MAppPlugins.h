

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

#include <map>
#include "DxConfig.h"
#include <MSingleton.h>


///
/// 前向声明
///
class IPluginInterface;

///
/// \brief The HAppPlugins class
/// 该类在整个App中作为单列的存在
///
class DXUI_API MAppPlugins : public MSingleton<MAppPlugins>
{
public:
	MAppPlugins();
	~MAppPlugins();

    ///
    /// \brief OnPutData
    /// \param any_data
    /// \param dllName
    /// 向plugin中写入数据
    /// dllName 如果为空 那么向所有plugin中写入数据
    ///
    bool OnPutData(const mj::MAny& any_data,const MString& dllName = "");

    ///
    /// \brief OnGetData
    /// \param dllName
    /// \return
    /// 获取指定plugin中的数据
    ///
    mj::MAny OnGetData(const MString& dllName) const;



    ///
    /// \brief AddPlugin
    /// \param dllName
    ///
    IPluginInterface* AddPlugin(const MString& dllName);

    ///
    /// \brief RemovePlugin
    /// \param dllName
    ///
    void RemovePlugin(const MString& dllName);

    ///
    /// \brief GetPluginList
    /// \return
    ///
    std::vector<MString> GetPluginList() const;

    ///
    /// \brief GetPlugin
    /// \param dllName
    /// \return
    ///
    IPluginInterface* GetPlugin(const MString& dllName);


	///
	/// \brief
	/// \param KeyName
	/// \return
	/// 通过关键字查询IPluginInterface
	/// KeyName 不需要全路径
	/// 只需要名字即可
	/// 不需要全路径
	/// 也不需要后缀名
	///
	IPluginInterface* QueryPlugin(const MString& KeyName);

    ///
    /// \brief LoadPluginFromDll
    /// \param dllName
    /// \return
    ///
    IPluginInterface* LoadPluginFromDll(const MString& dllName);


	///
	/// 直接注册插件进行内部管理
	///
	void RegisterPlugin(const MString& KeyName, IPluginInterface* Plugin);
private:
    std::map<MString,IPluginInterface*>   mPlugins;
};

