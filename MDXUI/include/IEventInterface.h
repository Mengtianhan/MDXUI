
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
namespace DxUI{
	class CDxWidget;

	//+------------------------------
	//
	// 事件的定义
	//
	//+------------------------------
	class CDxUIEvent
	{
	public:
		CDxUIEvent(CDxWidget* sender) :pSender(sender){}
		virtual ~CDxUIEvent(){}
		const CDxWidget* Sender() const{ return pSender; }
		void  Sender(CDxWidget* sender){
			pSender = sender;
		}
	private:
		CDxWidget* pSender{ nullptr };
	};


	//+--------------------------
	//
	// 显示 隐藏 改变 事件
	//
	//+--------------------------
	class EDxUIVisibleChangedEvent : public CDxUIEvent{
	public:
		EDxUIVisibleChangedEvent(CDxWidget* sender) : CDxUIEvent(sender){}
	};

	class EDxUIRemoveChangedEvent : public CDxUIEvent{
	public:
		EDxUIRemoveChangedEvent(CDxWidget* sender) : CDxUIEvent(sender){}
	};

	//+--------------------------
	//
	// 值改变事件
	//
	//+--------------------------
	template<class T>
	class EDxUIValueChange : public CDxUIEvent{
	public:
		EDxUIValueChange(CDxWidget* sender, const T& oldValue, const T& newValue,const T& ref = T()) :
			CDxUIEvent(sender), 
			mOldValue(oldValue), 
			mNewValue(newValue),
			mRefValue(ref){}
		const T& OldValue() const{ return mOldValue; }
		const T& NewValue() const{ return mNewValue; }
		const T& RefValue() const{ return mRefValue; }
	private:
		T	mOldValue;
		T	mNewValue;
		T   mRefValue;
	};



	//+----------------------
	//
	// 位置发生改变事件
	//
	//+----------------------
	typedef EDxUIValueChange<POINT> EDxPosChange;



	//+------------------------
	//
	// 值发生改变事件
	//
	//+------------------------
	typedef EDxUIValueChange<double> EDxDoubleValueChange;


	//+---------------------------------------
	//
	// 点击事件
	//
	//+----------------------------------------
	class EDxUIClicked : public CDxUIEvent{
	public:
		EDxUIClicked(CDxWidget* sender, bool isClicked, DXMouseButton button) :CDxUIEvent(sender), bisClicked(isClicked), mButton(button){}
		void SetPoint(POINT pt){ mPt = pt; }
		const bool IsClicked() const{ return bisClicked; }
		DXMouseButton Button(){ return mButton; }
		POINT Point(){ return mPt; }
	private:
		bool bisClicked{ false };
		DXMouseButton mButton;
		POINT		mPt;
	};




	//+---------------------------------------
	//
	// 双击事件
	//
	//+----------------------------------------
	class EDxUIDoubleClicked : public CDxUIEvent{
	public:
		EDxUIDoubleClicked(CDxWidget* sender, DXMouseButton button) :CDxUIEvent(sender), mButton(button){}
		DXMouseButton Button() const{ return mButton; }
	private:
		DXMouseButton mButton;
	};


	//+------------------------------------------
	//
	// 选中事件
	//
	//+------------------------------------------
	class EDxUIChecked : public CDxUIEvent{
	public:
		EDxUIChecked(CDxWidget* sender, bool isClicked) :CDxUIEvent(sender), bisClicked(isClicked){}
		const bool IsChecked() const{ return bisClicked; }
	private:
		bool bisClicked{ false };
	};

	//+---------------------------------------
	//
	// 选择改变事件
	//
	//+---------------------------------------
	template<class T>
	class EDxUICurrentTextChanged : public CDxUIEvent{
	public:
		EDxUICurrentTextChanged(CDxWidget* sender, const T& content) :CDxUIEvent(sender), mContent(content){}
		const T& Content() const{ return mText; }
	private:
		T mContent;
	};


	//+-----------------
	//
	// 方向改变事件
	//
	//+------------------
	class EDxUIDirectChanged : public CDxUIEvent{
	public:
		EDxUIDirectChanged(CDxWidget* sender, bool isRight) :CDxUIEvent(sender), mIsRight(isRight){}
		bool DirectIsRight() const{ return mIsRight; }
	private:
		bool mIsRight;
	};


	//+-------------------------------------------
	//
	// 行列变化事件
	// 用于文本或者列表或者table或者树结构的显示
	// 滚动条触发事件
	//
	//+--------------------------------------------
	class EDxUIRowAndColumChanged : public CDxUIEvent{
	public:
		EDxUIRowAndColumChanged(CDxWidget* sender, bool isAdd, int counts = 1) :CDxUIEvent(sender), bIsAdd(isAdd), mCounts(counts){}
		bool IsAddLine() const{ return bIsAdd; }
		int  Counts() const{ return mCounts; }
	private:
		bool bIsAdd;
		int	 mCounts;
	};


	//+-----------------
	//
	// 辅助函数
	//
	//+-----------------
	template<class T>
	T* event_cast(CDxUIEvent* e){
		try{
			T* r = dynamic_cast<T*>(e);
			return r;
		}
		catch (...){
			return nullptr;
		}
	}
}