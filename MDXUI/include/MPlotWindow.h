

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
#include "MPlotWindowBase.h"
#include "DxColorPalette.h"
#include "DxEdit.h"
#include "DxComboBox.h"
#include "DxListWidget.h"
#include "DxStackedWidget.h"
#include "DxFlatButton.h"
#include "DxSpinBox.h"
#include "DxRadioButton.h"
#include "DxCheckBox.h"
#include "DxMenu.h"

//+-------------------------------------------
//
// MPlotWindow 功能相对比较丰富的绘图控件
//
//+-------------------------------------------
class MPlotDataSourceInterface;
class IAlgForPlotInterace;

class DXUI_API MPlotWindow :
	public MPlotWindowBase
{
	DECLARE_CLASS(MPlotWindow)
public:
	enum NormalMarkerType{
		Marker_Max = 0,
		Marker_Min,
	};
public:
	MPlotWindow();
	~MPlotWindow();

	static void RegisterAlg(IAlgForPlotInterace* AlgPtr);

protected:
	void		OnDealSelectedChanged(int curindex, int preindex, CDxWidget* sender);
	void		OnDealButtonEvent(bool iscliked, CDxWidget* sender);
	void		OnRendWindow(IPainterInterface* painter);
	void		OnItem(bool isClicked, CDxWidget* sender);
	void		OnListItemChecked(bool isChecked, int index, CDxWidget* sender);
	void		OnMarkerButton(bool isClicked, CDxWidget* sender);
	void		OnButton(bool isClicked, CDxWidget* sender);
	void		OnCheckBox(bool isClicked, CDxWidget* sender);
	void		OnValueChanged(double val, CDxWidget* sender);
	void		OnAction(bool isClicked,CDxWidget* sender);
	void		OnRightButtonClicked(int row, int col, DxUI::DxPointI pos, CDxWidget* sender);
	void		OnAlgSelectedChanged(int curIndex, int preIndex, CDxWidget* sender);
	void		OnAlgEvent(bool isClicked, CDxWidget* sender);
	void		UpdateChildWindowPos();
	bool		OnNotify(DxUI::DXNotifyUI* NotifyEvent);


private:
	void		OnPushButtonTemplate(CDxPushButton* button);
protected:
	CDxPushButton*				pCurevButton{ nullptr };
	CDxPushButton*				pMarkerButton{ nullptr };
	CDxPushButton*				pDataButton{ nullptr };


	//
	// 弹出窗口
	//
 	CDxPopWindow*				pCurveWindow{ nullptr };
	CDxPopWindow*				pDataWindow{ nullptr };
	CDxPopWindow*				pMarkerWindow{ nullptr };

	//
	// 曲线数据分析
	//
	CDxTableWindow*				pCurveDataTable{ nullptr }; // 显示曲线数据
	CDxTableWindow*				pAlgResultDataTable{ nullptr }; // 显示分析结果
	CDxListWindowEx*			pCurveDataList{ nullptr };
	DxUI::CDxMenu*				pAlgTableMenu{ nullptr };
	DxUI::CDxAction*			pAlgAutoScaleAction{ nullptr };
	DxUI::CDxAction*			pAlgSelectedAction{ nullptr };
	DxUI::CDxAction*			pMarkerAction{ nullptr };
	DxUI::CDxPopWindow*			pAlgSelectedWindow{ nullptr };
	DxUI::CDxListWindowEx*		pAlgSelectedList{ nullptr };
	DxUI::CDxPushButton*		pAlgButton{ nullptr };
	DxUI::CDxStackedWidget*     pAlgParamWindowContainer{ nullptr };
	int							m_AlgCurRow;
	int							m_AlgCurCol;
	std::map<MString, IAlgForPlotInterace*> mPlotAlgMap;
	MPlotWindowBase*			pShowCurveWindow{ nullptr };



	//
	// 曲线管理
	//
	CDxListWindowEx*			p_CurveListWindow{ nullptr };
	DxUI::CDxColorPalette*		p_CurveColorPalette{ nullptr };
	DxUI::CDxEdit*				p_CurveNameEdit{ nullptr };
	DxUI::CDxSpinBox*			p_CurveSizeBox{ nullptr };


	DxUI::CDxPushButton*		p_AddCurveButton{ nullptr };
	DxUI::CDxPushButton*		p_ColorSelectedButton{ nullptr };

	DxUI::CDxListWidget*		p_OptionList{ nullptr };
	DxUI::CDxStackedWidget*		p_OptionContainer{ nullptr };

	DxUI::CDxListItem*			p_ImportItem{ nullptr };
	DxUI::CDxListItem*			p_UserItem{ nullptr };
	DxUI::CDxListItem*			p_FunctionItem{ nullptr };


	DxUI::CDxEdit*				p_InfoCurveNameEdit{ nullptr };
	DxUI::CDxSpinBox*			p_InfoCurveSizeBox{ nullptr };
	DxUI::CDxPushButton*		p_InfoColorButton{ nullptr };
	DxUI::CDxPushButton*		p_ExportButton{ nullptr };
	DxUI::CDxPushButton*		p_CopyButton{ nullptr };
	DxUI::CDxPushButton*		p_RemoveButton{ nullptr };
	DxUI::CDxPushButton*		p_ClearButton{ nullptr };

	DxUI::CDxRadioButton*		p_CurveColorRad{ nullptr };
	DxUI::CDxRadioButton*		p_BackgroundColRad{ nullptr };
	DxUI::CDxRadioButton*		p_PlotAreaColorRad{ nullptr };
	DxUI::CDxRadioButton*		p_TitileColorRad{ nullptr };
	DxUI::CDxRadioButton*		p_AxisColorRad{ nullptr };
	DxUI::CDxRadioButton*		p_GridColorRad{ nullptr };
	DxUI::CDxCheckBox*			p_TraceMouseBox{ nullptr };
	DxUI::CDxPushButton*		p_SetButton{ nullptr };

	DxUI::CDxPushButton*		p_ModifyCurveNameButton{ nullptr };
	DxUI::CDxPushButton*        p_ModifyCurveSizeButton{ nullptr };


	DxUI::CDxEdit*				p_TitleEdit{ nullptr };
	DxUI::CDxEdit*				p_HTitleEdit{ nullptr };
	DxUI::CDxEdit*				p_VTitleEdit{ nullptr };

	DxUI::CDxPushButton*		p_ModifyTitleButton{ nullptr };
	DxUI::CDxPushButton*		p_ModifyHTitleButton{ nullptr };
	DxUI::CDxPushButton*		p_ModifyVTitleButton{ nullptr };

	//
	// 锁定范围
	//
	DxUI::CDxCheckBox*			p_LockHAxisBox{ nullptr };
	DxUI::CDxCheckBox*			p_LockVAxisBox{ nullptr };

	DxUI::CDxSpinBox*			p_HMaxBox{ nullptr };
	DxUI::CDxSpinBox*			p_HMinBox{ nullptr };
	DxUI::CDxSpinBox*			p_VMaxBox{ nullptr };
	DxUI::CDxSpinBox*			p_VMinBox{ nullptr };

	//
	// Marker 管理
	//
	CDxListWindowEx*			p_MarkerListWindow{ nullptr };
	DxUI::CDxEdit*				p_InfoMarkerNameEdit{ nullptr };
	DxUI::CDxEdit*				p_InfoMarkerIconEdit{ nullptr };
	DxUI::CDxEdit*				p_InfoMarkerCurveEdit{ nullptr };
	DxUI::CDxSpinBox*			p_InofXValueBox{ nullptr };
	DxUI::CDxSpinBox*			p_InfoYValueBox{ nullptr };
	DxUI::CDxPushButton*		p_ModifyMarkerNameButton{ nullptr };
	DxUI::CDxPushButton*		p_InfoMarkerColorButton{ nullptr };
	DxUI::CDxPushButton*		p_InfoMarkerIconButton{ nullptr };

	DxUI::CDxPushButton*		p_InfoConfirmButton{ nullptr };
	DxUI::CDxPushButton*		p_MarkerRemoveButton{ nullptr };
	DxUI::CDxPushButton*		p_MarkerClearButton{ nullptr };

	DxUI::CDxComboBox*			p_MarkerCurveBox{ nullptr };
	DxUI::CDxEdit*				p_MarkerNameEdit{ nullptr };
	DxUI::CDxPushButton*		p_MarkerColorButton{ nullptr };

	DxUI::CDxSpinBox*			p_MarkerXValueBox{ nullptr };
	DxUI::CDxPushButton*		p_AddMarkerButton{ nullptr };

	DxUI::CDxPushButton*		p_MaxValueButton{ nullptr };
	DxUI::CDxPushButton*		p_MinValueButton{ nullptr };

	static std::vector<IAlgForPlotInterace*>  s_AlgPlotVector;
};

