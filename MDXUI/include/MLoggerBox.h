

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
#include "MLoggerStream.h"
#include "DxTextBlock.h"
#include <list>

//+---------------------------------
//
// class MLoggerEffects
// 用于自定义添加logger 效果
//
//+---------------------------------
class DXUI_API MLoggerEffects{
public:
	MLoggerEffects();

	enum EffectsEnum{
		SelectedRole, // 选中模型
		FontRole,// 字体
		ColourRole,//颜色
		UnderlineRole,//下划线
	};

	void	SetEffectsType(EffectsEnum type);
	void	SetFrontColor(const DxUI::DxColor& col);
	void	SetBackGroundColor(const DxUI::DxColor& col);
	void	SetTextFont(const MString& FontName, size_t size, bool isBold = false, bool isItalic = false);
	void	SetTextColor(const DxUI::DxColor& col);
	void	SetUnderLineColor(const DxUI::DxColor& col);

	EffectsEnum  EffectsType() const;
	DxUI::DxColor GetFrontColor() const;
	DxUI::DxColor GetBackgroundColor() const;
	DxUI::DXFontInfo GetFont() const;
	DxUI::DxColor   GetTextColor() const;
	DxUI::DxColor   GetUnderLineColor() const;

private:
	EffectsEnum		m_Type;
	DxUI::DxColor	m_BackgroundColor;
	DxUI::DxColor   m_FrontColor;
	DxUI::DxColor   m_TextColor;
	DxUI::DxColor	m_UnderLineColor;
	DxUI::DXFontInfo m_Font;
};




//+------------------------------------------------
//
// class  MLoggerBox
// 日志打印窗口
//
//+------------------------------------------------
class DXUI_API MLoggerBox : public DxUI::CDxTextBlock, public MLoggerStream
{
public:
	MLoggerBox();
	~MLoggerBox();
	MLoggerStream& operator<<(const MString& str);
	
	void SetText(const MString& content);
	void SetErrorTextColor(const DxUI::DxColor& col); // 设置错误信息的文字原色
	void SetWarningTextColor(const DxUI::DxColor& col); // 设置错误提示信息的文字颜色
	void AddUserDefineInfo(const MString& keyword, const MLoggerEffects& Effects);
	void ClearUserDefineInfo();
	void AttachWindow(CDxWidget* window);
	void DetachWindow();
	void OnRendWindow(DxUI::IPainterInterface* painter);

protected:
	virtual MString DealInputText(const MString& str);
	void	OnQueueEvent(unsigned queueId);

private:
	std::list<MString>			m_queueStr;
	DxUI::DxColor				m_ErrorCol;
	DxUI::DxColor				m_WarningCol;
	std::map<MString, MLoggerEffects> mUserDefineEffects;
	std::recursive_mutex		m_Mtx;
	CDxWidget*					p_Window{ nullptr };
};

