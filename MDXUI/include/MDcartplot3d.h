

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
#include "MDirectPlotImpl.h"
#include "MCurve.h"
#include "DxTableWindow.h"
#include "DxPopWindow.h"
#include "DxMenu.h"
#include "DxSpinBox.h"
#include "DxPushButton.h"
#include "DxRadioButton.h"
#include "DxComboBox.h"
#include "DxColorPalette.h"

using DxUI::MMeshData;
using DxUI::DxColor;
using DxUI::BasicPCT;
using DxUI::DXFontInfo;
using DxUI::MPlotTitle;
using DxUI::CDxTableWindow;
using DxUI::CDxPopWindow;
using DxUI::CDxMenu;
using DxUI::CDxAction;
using DxUI::CDxPushButton;
using DxUI::CDxRadioButton;
using DxUI::CDxSpinBox;
using DxUI::CDxComboBox;

class MPlotWindow;

//+--------------------------------
//
// 笛卡尔坐标三维方向图
//
//+--------------------------------
class DXUI_API MDcartplot3d :
	public MDirectPlotImpl
{
	DECLARE_CLASS(MDcartplot3d)
public:
	MDcartplot3d();
	~MDcartplot3d();

	//
	// 设置方向图数据
	//
	virtual void SetMeshData(double* x,double* y,double* rowData,unsigned xNum,unsigned yNum,bool dataIsColMaj = true);
	virtual void SetMeshData(const TL::Vector<double>& xs, const TL::Vector<double>& ys, const mj::dmat& m);
	virtual void SetMeshData(const MMeshData& meshdata);


	const MMeshData& GetMeshData();
	const MMeshData& GetMeshData() const;


	void	SetTitle(const MPlotTitle& title);
	void	SetTitle(const MString& title);
	
	void	SetGridLineColor(const DxColor& col);
	void	SetFontColor(const DxColor& col);
	void	SetTextAreaColor(const DxColor& col);
	void	SetTextColor(const DxColor& col);
	void	SetUnitStr(const MString& unit);
	void	SetTextFont(const DXFontInfo& font);
	void	SetInformation(const MString& Info);
	void	SetXTitle(const MString& xstr);
	void	SetYTitle(const MString& ystr);
	void	SetZTitle(const MString& zstr);

	MPlotTitle*			GetTitle();
	const MPlotTitle*	GetTitle() const;


	//
	// 设置各个轴的极限值
	//
	virtual void	SetFrustumInfo
		(
		double xMin, double xMax,
		double yMin, double yMax,
		double zMin, double zMax
		);


	//
	// 设置框框的半径
	//
	virtual void	SetFrustumInfo(double radius);

	void		SetDataShowRange(double minval, double maxvla);  // 设置显示区间 小于最小值 == 最小值 大于最大值 == 最大值

	void		Save(const MString& FileName);
	void		Load(const MString& FileName);
protected:
	virtual void OnUpdateBitmap();
	virtual void OnDrawOther(IPainterInterface* painter);
	void OnRender();
	void OnRender3D();
	void UnInitResource();
	void ReGenTextureForText(int width, int height);
	void OnPaintEventFor2D(IPainterInterface* painter);
	bool OnMouseEvent(const DxUI::CDxMouseEvent& e);
	bool OnKeyBoardEvent(const DxUI::CDxKeyBoardEvent& e);
	void SetTableData(const MMeshData& meshdata);
protected:
	MPlotTitle						mTitle;
	MString                         mXTitle{ "X" };
	MString                         mYTitle{ "Y" };
	MString                         mZTitle{ "Z" };

	ID3D11Buffer*                   pCubeVertexBuffer[6];
	ID3D11Buffer*                   pCubeIndexBuffer[6];
	
	ID3D11Buffer*                   pMeshVertexBuffer{ nullptr };
	ID3D11Buffer*                   pMeshIndexBuffer{ nullptr };

	ID2D1Bitmap*					pBitmap{ nullptr }; // 彩虹条

	ID3D11InputLayout*				pInputLayout{ nullptr };
	unsigned                        mCubeIndexNum{ 6 };

	double							mXAxisMaxValue{ 90 };
	double							mYAxisMaxValue{ 90 };
	double							mZAxisMaxValue{ 90 };
	double							mXAxisMinValue{ -90 };
	double							mYAxisMinValue{ -90 };
	double							mZAxisMinValue{ -90 };
	double							mRadius{ 300 };

	DxColor							mFontColor;
	DxColor							mGridLineColor;
	DxColor							mTextAreaColor;
	DxColor							mTextColor;
	DXFontInfo						mUnitFont;
	MString							mUnitStr;
	MString							mInformation;
	MString							mBakInformation;
	bool							bIsInited{ false };
	int								mTextWidth{ -1 };
	int								mTextHeight{ -1 };

	TL::Vector<unsigned>			mIndexs;
	TL::Vector<BasicPCT>			mPoints;
	MMeshData						mMeshData;



	//
	// +-----------------------------------------------------------
	//
	// 支持数据浏览
	//
	// +-----------------------------------------------------------
	//
protected:
	void	OnRightButtonClicked(int row, int col, DxUI::DxPointI pos, DxUI::CDxWidget* sender);
	void	OnAction(bool isClicked, DxUI::CDxWidget* sender);
	void	OnButton(bool isClicked, DxUI::CDxWidget* sender);

	void	OnButton2(bool isClicked, CDxWidget* sender);
	void	OnButtonTemplate(DxUI::CDxPushButton* button);
	void	OnCheckBox(bool isClicked, CDxWidget* sender);
	void	OnValueChanged(double val, CDxWidget* sender);

	CDxTableWindow					mDataTable;
	CDxPopWindow					mPopWindow;
	MMeshData						mOrgMeshData;

	CDxSpinBox						m_PosBox;
	CDxSpinBox						m_ResBox;
	CDxComboBox						m_ScanBox;
	CDxPushButton					m_AddButton;
	CDxPushButton					m_ApplyButton;

	CDxMenu							mMenu;
	CDxAction*						p_AddRowAction{ nullptr };
	CDxAction*						p_AddColAction{ nullptr };
	CDxAction*						p_PlotRowAction{ nullptr };
	CDxAction*						p_PlotColAction{ nullptr };
	CDxAction*						p_PlotCrossAction{ nullptr };

	int								m_SelRow{ -1 };
	int								m_SelCol{ -1 };
	CDxPopWindow					mPlotPopWindow;
	MPlotWindow*					p_PlotWindow{ nullptr };

	//
	//+--------------------------------------------------------
	//
	DxUI::CDxPopWindow				mCanvaSetWindow;
	DxUI::CDxColorPalette			mColorPalette;
	DxUI::CDxRadioButton			mBackgroundColRad;
	DxUI::CDxRadioButton			mTitileColorRad;
	DxUI::CDxPushButton				mSetButton;


	DxUI::CDxPushButton				mExportButton;
	DxUI::CDxPushButton				mCopyButton;
	DxUI::CDxPushButton				mFileButton;
	DxUI::CDxPushButton				mPasterButton;

	//
	// 锁定颜色映射值范围
	//
	DxUI::CDxCheckBox				mLockMinValBox;
	DxUI::CDxCheckBox				mLockMaxValBox;
	DxUI::CDxSpinBox				mMinValBox;
	DxUI::CDxSpinBox				mMaxValBox;

	//
	// 修改标题
	//
	DxUI::CDxEdit					mTitleEdit;
	DxUI::CDxEdit					mXTitleEdit;
	DxUI::CDxEdit					mYTitleEdit;
	DxUI::CDxEdit					mZTitleEdit;

	DxUI::CDxPushButton				mModifyTitleButton;
	DxUI::CDxPushButton				mModifyXTitleButton;
	DxUI::CDxPushButton				mModifyYTitleButton;
	DxUI::CDxPushButton				mModifyZTitleButton;
};

