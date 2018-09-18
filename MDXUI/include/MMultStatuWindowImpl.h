

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DxWindow.h"
#include "DxPopWindow.h"
#include "DxDialog.h"
#include "DxPushButton.h"
#include "DxLabel.h"
#include "DxEdit.h"
#include "DxListWindow.h"
#include "DxSpinBox.h"
#include "DxComboBox.h"

using DxUI::CDxWindow;
using DxUI::CDxPopWindow;
using DxUI::CDxDialog;
using DxUI::CDxPushButton;
using DxUI::CDxLabel;
using DxUI::CDxEdit;
using DxUI::CDxListWindow;
using DxUI::CDxSpinBox;
using DxUI::CDxComboBox;


//+-----------------------
//
// class UiBase
//
//+------------------------
class UiBase{
public:
	typedef std::function<double(double, mj::MString)> TRANSLATEDATAFUNTYPE;
	typedef std::function<void(const std::vector<mj::MString>&)> IMPORTDATAFUNTYPE;
	virtual ~UiBase(){}
	virtual std::vector<double> GetData() = 0;
	virtual void SetUnits(const TL::Vector<MString>& units) = 0;
	virtual void SetData(const std::vector<double>& freqs, const MString& unit) = 0;
	void BindTranlatDataFun(TRANSLATEDATAFUNTYPE fun){ mTanslatFun = fun; }
	void BindImportDataFun(IMPORTDATAFUNTYPE fun){ mImportFun = fun; }
	MString GetUnit(){ return mCurrentUnit; }
protected:
	std::vector<double> mDatas;
	TL::Vector<MString> mUnits; // 保存单位信息
	MString      mLgGroup;
	MString      mCurrentUnit;
	TRANSLATEDATAFUNTYPE mTanslatFun{ nullptr }; // 用于转换数据
	IMPORTDATAFUNTYPE    mImportFun{ nullptr };
};

//
// 下面是一些细节类
//
class UIInput : public CDxDialog
{
public:
	explicit UIInput(const MString& laggroup);
	MString GetData() const;
private:
	void init_connect();
	void slot_button(bool isClicked, CDxWidget* sender);


private:
	void init_style();
	CDxPushButton*		pOkButton;
	CDxPushButton*		pCancelButton;
	CDxLabel*			pInputLabel;
	CDxEdit*			pInputEdit;
	MString				mData;
};

//===================================
class UIDisplay : public CDxDialog
{

public:
	explicit UIDisplay(const MString& laggroup);
	void setdata(double start, double end, int count, MString unit);
	void slot_button();
private:
	CDxListWindow*        pListWidget;
};


//
// 单频点设置界面
//
class UISingle : public CDxWindow, public UiBase{
public:
	explicit UISingle(const MString& laggroup);
	std::vector<double> GetData();
	void SetData(const std::vector<double>& datas, const MString& unit);
	void SetUnits(const TL::Vector<MString>& units);
private:
	void init_connect();
	void freqChange(const MString& Freq);
	void slot_edit(const MString& str);
	void slot_combox(int curIndex, int preIndex, CDxWidget* sender);
private:

	CDxLabel*         pStartLabel;
	CDxLabel*         pEndLabel;
	CDxLabel*         pCountLabel;
	CDxSpinBox*		  pStartEdit;
	CDxSpinBox*	      pEndEdit;
	CDxSpinBox*       pCountBox;
	CDxPushButton*    pDisplayButton;
	CDxComboBox*      pStartBox;
	CDxComboBox*      pEndBox;
};

//
// 范围频率设置界面
//
class UIRange : public CDxWindow, public UiBase{
public:
	explicit UIRange(const MString& laggroup);
	std::vector<double> GetData();
	void SetData(const std::vector<double>& datas, const MString& unit);
	void SetUnits(const TL::Vector<MString>& units);
private:
	void init_connect();
	void freqChange(const MString& Freq);
	void do_button(bool isClicked, CDxWidget* sender);
	void slot_combox(int curIndex, int preIndex, CDxWidget* sender);

private:

	CDxLabel*         pStartLabel;
	CDxLabel*         pEndLabel;
	CDxLabel*         pCountLabel;
	CDxSpinBox*		  pStartEdit;
	CDxSpinBox*	      pEndEdit;
	CDxSpinBox*       pCountBox;
	CDxPushButton*    pDisplayButton;
	CDxComboBox*      pStartBox;
	CDxComboBox*      pEndBox;
	UIDisplay*       pDisplayWidget;
};

//
// 列表频率设置界面
//
class UIList : public CDxWindow, public UiBase{
public:
	explicit UIList(const MString& laggroup);
	std::vector<double> GetData();
	void SetData(const std::vector<double>& datas, const MString& unit);
	void SetUnits(const TL::Vector<MString>& units);

private:
	void init_connect();
	void pauseFile(const MString& filename);
	void importFile();
	void freqChange(const MString& Freq);
	void slot_button(bool isClicked, CDxWidget* sender);
private:

	
	CDxListWindow*              pListWidget;
	CDxComboBox*                pUnitBox;
	CDxPushButton*              pAddButton;
	CDxPushButton*              pImportButton;
	CDxPushButton*              pRemoveutton;
	CDxPushButton*              pClearButton;
	CDxPushButton*              pSortButton;
	UIInput*					pInputWindow{ nullptr };
	std::vector<double>         mListData;
	mj::MString                 mRecoderPath{ "./Data" };
	MString                     mAddWindowTitle{ "Add" };
};



