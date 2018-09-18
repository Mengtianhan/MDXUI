

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DxWindow.h"
#include <ITimer.h>
#include <memory>
namespace DxUI{
	//
	// 简单的动画效果
	// 需要重写UpdateTime(doubel time)接口
	// 如果参数传递的是0那么表示没有计时功能
	//
	class DXUI_API CDxAnimationWindow :
		public CDxWindow
	{
		DECLARE_CLASS(CDxAnimationWindow)
	public:
		CDxAnimationWindow();
		~CDxAnimationWindow();

		//
		// 开始 单位ms
		//
		bool IsStarted() const;
		void StartAnimation(int timeinter = 300);
		void StopAnimation();
		void OnTimer();
		void OnRendWindow(IPainterInterface*  painter);


		

		//
		// 通过该接口实现动画效果更新
		//
		virtual void UpdateTime(IPainterInterface*  painter,double time);

	public:
		static IPainterInterface::TransformMatrix RotateMatrix(double angle,const DxPointI& pos); // 旋转
		static IPainterInterface::TransformMatrix TransformMatrix(int x, int y);// 平移
		static IPainterInterface::TransformMatrix ScaleMatrix(double x, double y, const DxPointI& pos);// 缩放
		static IPainterInterface::TransformMatrix SkewMatrix(double x, double y, const DxPointI& pos);// 缩放
		static IPainterInterface::TransformMatrix IdentifyMatrix();//单位矩阵
		static IPainterInterface::TransformMatrix Producat(const IPainterInterface::TransformMatrix& left, const IPainterInterface::TransformMatrix& right); // 矩阵叉乘

	private:
		std::shared_ptr<ITimer> mTimer;//计时器
		bool		bIsStart{ false };
	};
}


