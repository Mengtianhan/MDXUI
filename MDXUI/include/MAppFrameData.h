

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "DxConfig.h"
#include  "MAppDataUnit.h"

//
// class MAppFrameData
//
class DXUI_API MAppFrameData
{
public:
	MAppFrameData();
	virtual ~MAppFrameData();

    ///
    /// \brief Attach
    /// \param __data
    /// 将数据关联到系统框架中
    ///
    void Attach(MAppDataUnit* __data);

    ///
    /// \brief Dettach
    /// \param __data
    /// 将数据从系统框架中移除
    ///
	void Dettach(MAppDataUnit* __data);

    ///
    /// \brief ValidateData
    /// \return
    /// 校验数据的有效性
    ///
    virtual bool ValidateData();

    ///
    /// \brief InitData
    /// \param __Type
    /// \return
    /// 初始化数据,如果不是必须建议不要重写该函数,如有需要进行附加数据的初始化建议重写 InitDataAdd
    ///
    virtual bool InitData();

    ///
    /// \brief InitDataAdd
    /// \param __Type
    /// \return
    /// 初始化数据
    /// 子类不该重写上面的InitData,有必要的话可以重写 InitDataAdd
    ///
    virtual bool InitDataAdd();

    ///
    /// \brief SerialToFile
    /// \param Serial
    /// 将与系统框架关系的所有数据写进MSerialize
    /// MSerialize 键 MSerialize
    ///
    virtual void SerialToFile(DxUI::MSerialize* Serial);

    ///
    /// \brief SerialFromFile
    /// \param Serial
    /// 从 MSerialize 中读取数据
    ///
    virtual void SerialFromFile(DxUI::MSerialize* Serial);

    ///
    /// \brief ToString
    /// \return
    /// 惯例接口
    ///
    virtual MString ToString() {return "HAppFrameData";}

protected:
	TL::Vector<MAppDataUnit*> mNeedOpData;
};

