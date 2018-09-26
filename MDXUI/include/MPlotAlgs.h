


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
#ifdef __MAPI_DEBUG__
#define MPLOT_API 
#else
#if defined(DIRECTUI_EXPORTS)
#define MPLOT_API __declspec(dllexport)
#else
#define MPLOT_API __declspec(dllimport)
#endif
#endif 


#include <IAlgForPlotInterface.h>




//////////////////////////////////////////////////////////////////////////
// 
// 针对曲线执行的算法
//
//////////////////////////////////////////////////////////////////////////
namespace MPlotAlg {

	//////////////////////////////////////////////////////////////////////////
	//
	// 计算最大值
	//
	//////////////////////////////////////////////////////////////////////////
	class MPLOT_API AlgMaxPoint : public IAlgForPlotInterace
	{
	public:
		AlgMaxPoint();
		DxUI::CDxWidget* ParamSetWindow();
		virtual MString Describe() const{ return "最大值"; }
		virtual IAlgForPlotInterace* Clone(){ return new AlgMaxPoint; }
		virtual void Init(){}
		virtual void Exec(const TL::Vector<double>& xs, const TL::Vector<double>& ys);
	private:
		DxUI::CDxWidget* p_ParamWindow{ nullptr };
	};



	//////////////////////////////////////////////////////////////////////////
	//
	// 计算最小
	//
	//////////////////////////////////////////////////////////////////////////
	class MPLOT_API AlgMinPoint : public IAlgForPlotInterace
	{
	public:
		AlgMinPoint();
		DxUI::CDxWidget* ParamSetWindow();
		virtual MString Describe() const{ return "最小值"; }
		virtual IAlgForPlotInterace* Clone(){ return new AlgMinPoint; }
		virtual void Init(){}
		virtual void Exec(const TL::Vector<double>& xs, const TL::Vector<double>& ys);
	private:
		DxUI::CDxWidget* p_ParamWindow{ nullptr };
	};


	//////////////////////////////////////////////////////////////////////////
	//
	// matlab: direct_gain | text: Beam Pointing
	//
	//////////////////////////////////////////////////////////////////////////
	class MPLOT_API AlgBeamPointing : public IAlgForPlotInterace
	{
	public:
		AlgBeamPointing();
		DxUI::CDxWidget* ParamSetWindow();
		virtual MString Describe() const{ return "波束指向"; }
		virtual IAlgForPlotInterace* Clone(){ return new AlgBeamPointing; }
		virtual void Init(){}
		virtual void Exec(const TL::Vector<double>& xs, const TL::Vector<double>& ys);
	private:
		DxUI::CDxWidget* p_ParamWindow{ nullptr };
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//matlab: theta_bw | text: 3dB Beamwidth
	//
	//
	//////////////////////////////////////////////////////////////////////////
	class MPLOT_API Alg3dBBeamWidth : public IAlgForPlotInterace
	{
	public:
		Alg3dBBeamWidth();
		DxUI::CDxWidget* ParamSetWindow();
		virtual MString Describe() const{ return "3dB带宽"; }
		virtual IAlgForPlotInterace* Clone(){ return new Alg3dBBeamWidth; }
		virtual void Init(){}
		virtual void Exec(const TL::Vector<double>& xs, const TL::Vector<double>& ys);
	private:
		DxUI::CDxWidget* p_ParamWindow{ nullptr };
	};



	//////////////////////////////////////////////////////////////////////////
	//
	// s-..=sidelobe, l|r=left/right, sli/sri=ids of left/right-sidelobe
	//
	//////////////////////////////////////////////////////////////////////////
	class MPLOT_API AlgSideLobe : public IAlgForPlotInterace
	{
	public:
		AlgSideLobe();
		virtual MString Describe() const{ return "副瓣"; }
		DxUI::CDxWidget* ParamSetWindow();
		virtual IAlgForPlotInterace* Clone(){ return new AlgSideLobe; }
		virtual void Init();
		virtual void Exec(const TL::Vector<double>& xs, const TL::Vector<double>& ys);
	protected:
		int nSL; //L&R sidelobe count
		TL::Vector<MString> cols;
		TL::Vector<MString> vals;
		DxUI::CDxWidget*    p_ParemWindow{ nullptr };
		DxUI::CDxSpinBox*   p_NSLBox{ nullptr };
	};


	//////////////////////////////////////////////////////////////////////////
	//
	// 计算 N dB
	//
	//////////////////////////////////////////////////////////////////////////
	class MPLOT_API AlgBandwidthNdB : public IAlgForPlotInterace
	{
	public:
		AlgBandwidthNdB();
		virtual MString Describe() const{ return "NdB带宽"; }
		DxUI::CDxWidget* ParamSetWindow();
		virtual IAlgForPlotInterace* Clone(){ return new AlgBandwidthNdB; }
		virtual void Init();
		virtual void Exec(const TL::Vector<double>& xs, const TL::Vector<double>& ys);
	protected:
		double ndB; //dB value of bandwidth

		DxUI::CDxWidget*    p_ParemWindow{ nullptr };
		DxUI::CDxSpinBox*   p_NdBBox{ nullptr };
	};


	///////////////////////////////////////////////////////////////////////////
	//
	// 计算零深
	//
	//////////////////////////////////////////////////////////////////////////
	class MPLOT_API AlgZero : public IAlgForPlotInterace
	{
	public:
		AlgZero();
		DxUI::CDxWidget* ParamSetWindow();
		virtual MString Describe() const{ return "零深"; }
		virtual IAlgForPlotInterace* Clone(){ return new AlgZero; }
		virtual void Init(){}
		virtual void Exec(const TL::Vector<double>& xs, const TL::Vector<double>& ys);
	private:
		DxUI::CDxWidget* p_ParamWindow{ nullptr };
	};

	///////////////////////////////////////////////////////////////////////////
	//
	// 计算RMS
	//
	//////////////////////////////////////////////////////////////////////////
	class MPLOT_API AlgRMS : public IAlgForPlotInterace
	{
	public:
		AlgRMS();
		DxUI::CDxWidget* ParamSetWindow();
		virtual MString Describe() const{ return "RMS"; }
		virtual IAlgForPlotInterace* Clone(){ return new AlgRMS; }
		virtual void Init(){}
		virtual void Exec(const TL::Vector<double>& xs, const TL::Vector<double>& ys);
	private:
		DxUI::CDxWidget* p_ParamWindow{ nullptr };
	};

}//namespace MPlotAlg

