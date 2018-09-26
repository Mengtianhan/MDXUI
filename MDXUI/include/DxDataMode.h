

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
#include "MString.h"
#include <Windows.h>
#include <MAny.h>

#ifdef __M_DEBUG__
#define DXDATAMODE_API 
#else
#if defined(DIRECTUI_EXPORTS)
#define DXDATAMODE_API __declspec(dllexport)
#else
#define DXDATAMODE_API __declspec(dllimport)
#endif
#endif 

namespace DxUI{

	using mj::MString;
	class CDxEffects;


	//+----------------------------
	// 
	// class CDxDataBase
	// 数据模型
	// 可用于对new操作符有更改需求的数据
	//
	//+-----------------------------
	class DXDATAMODE_API CDxDataBase{
	public:
		CDxDataBase();
		virtual ~CDxDataBase();
		static void* operator new(size_t size);
		static void operator delete(void* ptr);
		bool IsStackedObject() const;

	private:
		bool	b_IsStackedObj{ false };
	};


	//+-----------------------
	//
	// CDxListDataMode
	// 列表项的数据模型
	//
	//+-----------------------
	class DXDATAMODE_API CDxListDataMode : public CDxDataBase{
	public:
		CDxListDataMode();
		virtual ~CDxListDataMode();
		virtual CDxListDataMode* CreateThis();
		virtual bool			 DestroyThis();

		virtual void SetText(const MString& Text);
		void		SetIcon(const MString& Icon);
		void		SetAny(const mj::MAny& any);

		void		SetHintSize(int w, int h);
		virtual int Width() const;
		virtual int Heght() const;

		virtual int MinWidth() const;
		virtual int MinHeight() const;

		const MString& Text() const;
		const MString& Icon() const;
		mj::MAny		Any() const;

		MString& Text();
		MString& Icon();

		virtual void	SetArea(const RECT& rc);
		RECT			GetArea() const;

		void	SetSelected(bool selected);
		bool	IsSelected() const;
		static void	SetEffects(CDxEffects* effects);
	protected:
		MString		mText;
		MString		mIcon;
		RECT		mArea;
		bool		bIsSelected{ false };
		mj::MAny	mAny;
		bool		bCalDelete{ false };
		static CDxEffects* sEffects;
		int			m_ItemHintWidth;
		int			m_ItemHintHeight;
	};


	//+-----------------------
	//
	// CDTableDataMode
	// 格子表项的数据模型
	//
	//+-----------------------
	class DXDATAMODE_API CDxTableDataMode : public CDxListDataMode{
	public:
		CDxTableDataMode();
		~CDxTableDataMode();
		CDxTableDataMode*	CreateThis();
		bool				DestroyThis();
		void				SetText(const MString& Text);
		void				SetArea(const RECT& rc);
		void				SetOffsetX(int x);
		void				SetOffsetY(int y);
		int					Width() const;
		int					Heght() const;
		SIZE				GetLimitSize();
	protected:
		SIZE				mLimitSize;
	};


	//
	// 表头
	//
	class DXDATAMODE_API CDxTableHeaderMode : public CDxTableDataMode{
	public:
		CDxTableHeaderMode();
		~CDxTableHeaderMode();
		CDxTableHeaderMode* CreateThis();
		bool				DestroyThis();
	};
}


