

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


class DXUI_API MPointTransfrom
{
public:
    MPointTransfrom();
    ~MPointTransfrom();
    ///
    /// \brief Rotate
    /// \param x
    /// \param y
    /// \param inoutdata
    /// \param Angle 顺时针旋转角度 deg
    /// \param xAxis 1 0
    /// \param yAxis 1 0
    /// \param zAxis 1 0
    /// \return
    /// 绕指定轴进行旋转
    ///
    bool Rotate(
            std::vector<double>& xs,
            std::vector<double>& ys,
            mj::dmat& inoutdata,
            double Angle,
            int xAxis,
            int yAxis,
            int zAxis
            );

    ///
    /// \brief Rotate
    /// \param x
    /// \param y
    /// \param inoutdata
    /// \param xAngle
    /// \param yAngle
    /// \param zAngle
    /// \param xCenter
    /// \param yCenter
    /// \param zCenter
    /// \return
    /// 绕中心点(xCenter,yCenter,zCenter) 进行旋转
    ///
    bool Rotate(std::vector<double>& xs,
            std::vector<double>& ys,
            mj::dmat& inoutdata,
            double xAngle,
            double yAngle,
            double zAngle,
            double xCenter,
            double yCenter,
            double zCenter
            );

    ///
    /// \brief Translate
    /// \param xs
    /// \param ys
    /// \param inoutdata
    /// \param xDistance
    /// \param yDistance
    /// \param zDistance
    /// \param isDisoutrange 是否对超出范围的部分进行裁剪
    /// \return
    /// 平移
    ///
    bool Translate(std::vector<double>& xs,
                   std::vector<double>& ys,
                   mj::dmat& inoutdata,
                   double xDistance,
                   double yDistance,
                   double zDistance,
                   bool isDisoutrange = true);

    ///
    /// \brief Translate
    /// \param xs
    /// \param ys
    /// \param inoutdata
    /// \param xDistance
    /// \param yDistance
    /// \param zDistance
    /// \param minvalue
    /// \param maxvalue
    /// \return
    ///
    bool Translate(std::vector<double>& xs,
                   std::vector<double>& ys,
                   mj::dmat& inoutdata,
                   double xDistance,
                   double yDistance,
                   double zDistance,
                   double minvalue,
                   double maxvalue);

    ///
    /// \brief meshdatatype
    /// bool 标识是否有数据
    /// int 当前数据的序号
    ///
    typedef mj::MMatrixSheet<bool,int> meshdatatype;

    ///
    /// \brief GenMeshIndex
    /// \param data
    /// \param indexs
    /// \return
    /// 根据给定的数据位置生成相应的索引
    ///
    bool GenMeshIndex(const meshdatatype& data,std::vector<unsigned>& indexs);
};


