

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
#include "DxWindow.h"

class CalendarImpl;

namespace DxUI{

	//+--------------------------
	//
	// 基本的日历控件
	//
	//+--------------------------
	class DXUI_API CDxCalendar :
		public CDxWindow
	{
		DECLARE_CLASS(CDxCalendar)
	public:
		CDxCalendar();
		~CDxCalendar();

		void		SetYear(int Year);
		void		SetMonth(int Month);
		int			GetYear() const;
		int			GetMonth() const;
		int			GetDay() const;
		int			GetWeekDay() const;

		void		SetPickedColor(const DxColor& col, const DxColor& padcolor = RgbI(167, 202, 216));
		void		SetHoverColor(const DxColor& col);

		void		OnRendWindow(IPainterInterface* painter);
		void		UpdateChildWindowPos();
		bool		OnNotify(DxUI::DXNotifyUI* NotifyEvent);

	msignals:
		//
		// 选择日期时触发该事件
		// 年月日
		//
		TL::MTLVoidEvent<int, int, int, CDxWidget*> Event_DataPicked;

		//
		// 除了年月日之外附件星期
		// 星期 =[0,1,2,3,4,5,6] ---- [日，一，二，三，四，五，六]
		//
		TL::MTLVoidEvent<int, int, int, int, CDxWidget*> Event_Data2Picked;

	private:
		CalendarImpl*    p_Impl{ nullptr };
		int				 m_Month{ 1 };
	
		int				 m_WeekDay{ -1 };
		int				 m_Year;

		int				 m_CurrentYear;
		int				 m_CurrentMonth;
		int				 m_CurrentDay;


		RECT			 m_MonthRect;
		RECT			 m_AddRect;
		RECT			 m_RecRect;
		POINT			 mClickedPt;
		POINT			 mMousePt;
		bool			 bMouseIsInRect{ false };

		
		mj::MMatrixSheet<RECT,int> m_Rects;

		DxColor			mPickedColor;
		DxColor			mHoverColor;
		DxColor			mPadColor;
	};




	//
	// 支持弹出挑选日期
	//
	class CDxNoCaptionDialog;
	class DXUI_API CDxCalendarBox :
		public CDxWindow
	{
		DECLARE_CLASS(CDxCalendarBox)
	public:
		CDxCalendarBox();
		~CDxCalendarBox();

		int			GetYear() const;
		int			GetMonth() const;
		int			GetDay() const;
		int			GetWeekDay() const;

		CDxNoCaptionDialog* GetDropBox();
		CDxCalendar*        GetCalendar();

		void		SetCalendarBoxSize(int w, int h);
		void		SetCalendarWidthWithShowBox(bool isEnabel);
		void		SetShowWeekDayEnabel(bool isEnabel);
		void		SetText(const MString& content);
		void		OnRendWindow(IPainterInterface* painter);
		bool		OnNotify(DxUI::DXNotifyUI* NotifyEvent);
		void		UpdateChildWindowPos();
	msignals:
		//
		// 选择日期时触发该事件
		// 年月日
		//
		TL::MTLVoidEvent<int, int, int, CDxWidget*> Event_DataPicked;

		//
		// 除了年月日之外附件星期
		// 星期 =[0,1,2,3,4,5,6] ---- [日，一，二，三，四，五，六]
		//
		TL::MTLVoidEvent<int, int, int, int, CDxWidget*> Event_Data2Picked;

	private:
		CDxNoCaptionDialog*   p_DropBox{ nullptr };
		CDxCalendar*		  p_CalendarImpl{ nullptr };
		bool				  bIsDropBoxWidthWithShowBox{ false };
		bool				  bIsNeedWeekDay{ true };//是否需要显示星期
	};
}

