

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////


#pragma once
#include "DxWindow.h"
#include "DxGroupBox.h"
#include "DxRadioButton.h"
#include "DxStackedWidget.h"

using DxUI::CDxWindow;
using DxUI::CDxRadioButton;
using DxUI::CDxStackedWidget;
using DxUI::CDxGroupBox;


//
// 用于设置三种状态的东西
//

class UiBase;
class DXUI_API MFrequencyWindow :
	public CDxWindow
{

	typedef std::function<double(double, const mj::MString&)> TRANSLATEDATAFUNTYPE;
	typedef std::function<void(const std::vector<mj::MString>&)> IMPORTDATAFUNTYPE;
public:
	MFrequencyWindow(const MString& laggroup);
	~MFrequencyWindow();
	std::vector<double> GetData();

	//
	// 设置数据
	//
	void SetData(unsigned type,const std::vector<double>& data,const MString &unit);

	//
	// 设置单位列表
	//
	void SetUnits(const TL::Vector<MString>& units);

	//
	// 设置单位转换函数
	//
	void BindTranlatDataFun(TRANSLATEDATAFUNTYPE fun);

	//
	// 设置单位转换函数
	//
	void BindUnitTranslateFun(TRANSLATEDATAFUNTYPE fun);

	//
	// 设置导入其他非默认类型文件时使用的外部处理函数
	//
	void BindImportDataFun(IMPORTDATAFUNTYPE fun);

	//
	// 获取类型
	//
	unsigned GetType() const;

	//
	// 获取单位
	//
	MString GetUnit(); // 获取单位信息

	
	//
	// 三种类型设置只需要一种 type -- 1 2 3
	//
	void setOnlyType(unsigned type);
private:
	void init_connect();
	void slot_rad(bool is_checked, CDxWidget* sender);
private:
	void init_style();
	double DefualtUnitTranslateFun(double value, const MString& unit);

	CDxGroupBox*      pTypeGroup;       //类型框架
	CDxGroupBox*      pInfoGroup;       //信息框架
	CDxRadioButton*   pSingleRad;   //单频点
	CDxRadioButton*   pRangeRad;    //范围
	CDxRadioButton*   pListRad;     //列表
	CDxStackedWidget* pMultWidget;  //info

	UiBase*    pSingleigdet;
	UiBase*    pRangeWidget;
	UiBase*    pListWidget;

private:
	unsigned             mType; //1 2 3

	TRANSLATEDATAFUNTYPE mUnitTranslateFun{ nullptr };

};

