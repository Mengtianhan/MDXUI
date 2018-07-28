#pragma once
#include "DxListItem.h"
namespace DxUI{
	//
	// class CDxHSpliter
	// 一条水平分割线
	//
	class DXUI_API CDxHSpliter :
		public CDxListItem
	{
		DECLARE_CLASS(CDxHSpliter)
	public:
		CDxHSpliter();
		~CDxHSpliter();
		int GetHeight() const;
		void SetSelected(bool selected);
		void SetSpliterHeight(int height);
		bool OnDrawItem(const RECT& rc, IPainterInterface* painter);
	protected:
		int			mSpliterHeight{ 2 };
	};


	//
	// class CDxVSpliter
	// 一条垂直分割线
	//
	class DXUI_API CDxVSpliter :
		public CDxListItem
	{
		DECLARE_CLASS(CDxVSpliter)
	public:
		CDxVSpliter();
		~CDxVSpliter();
		DXWindowType GetWindowSelfDesc() const;
		int GetHeight() const;
		int GetWidth() const;
		void SetSelected(bool selected);
		void SetSpliterWidth(int width);
		bool OnDrawItem(const RECT& rc, IPainterInterface* painter);
	protected:
		int			mSpliterWidth{ 2 };
	};
}


