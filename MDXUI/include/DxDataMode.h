

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
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


