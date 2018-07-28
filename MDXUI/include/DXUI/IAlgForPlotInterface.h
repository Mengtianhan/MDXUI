#pragma once
#include <MToolLib>

//
// 特殊用途的辅助算法接口
//
namespace DxUI{
	class CDxWindow;
	class CDxSpinBox;
	class CDxWidget;
}

struct IAlgForPlotInterace{
	virtual ~IAlgForPlotInterace(){};

	virtual MString Describe() const{ return ""; }
	virtual IAlgForPlotInterace* Clone(){ return new IAlgForPlotInterace; }
	virtual DxUI::CDxWidget* ParamSetWindow(){ return nullptr; }
	virtual void Init(){}
	virtual void Exec(const TL::Vector<double>& xs, const TL::Vector<double>& ys){}
	const TL::Vector<std::pair<MString, MString>>& Result() const{
		return mResult;
	}

protected:
	TL::Vector<std::pair<MString, MString>> mResult;
};