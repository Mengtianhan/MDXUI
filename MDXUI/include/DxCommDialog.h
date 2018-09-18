

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DxWindow.h"
namespace DxUI{
	class DXUI_API CDxCommDialog
	{
	public:
		CDxCommDialog();
		~CDxCommDialog();

		static void InitResource();
		static void UnInitResource();
		static TL::Vector<MString> GetOpenFileNames(CDxWidget* parent, const MString& filter);
		static MString GetOpenFileName(CDxWidget* parent, const MString& filter);
		static MString GetSaveFileName(CDxWidget* parent, const MString& filter);
		static MString GetSaveFileName(CDxWidget* parent, const MString& filter, const MString& defualtExt);
		static MString GetSaveDirName(CDxWidget* parent);
		static DXFontInfo GetFont(CDxWidget* parent);
		static DxColor	  GetColor(CDxWidget* parent);
	private:
		static bool		s_IsInied;
	};
}

