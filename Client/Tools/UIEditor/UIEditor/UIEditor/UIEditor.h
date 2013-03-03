// UIEditor.h : UIEditor 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

#include "interface.h"

// CUIEditorApp:
// 有关此类的实现，请参阅 UIEditor.cpp
//

class CUIEditorApp : public CWinApp
{
public:
	CUIEditorApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance( );

public:
	CUIEditor m_uiEiditor;
	INT m_ShowConsole;
	BOOL m_needCallSkinExit;

// 实现
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();

public:
	BOOL LoadPlugin();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileNewPrj();
public:
	afx_msg void OnFileNewDesignPrj();
public:
	afx_msg void OnOpenDesignPrj();
public:
	afx_msg void OnOpenPrj();
public:
	virtual BOOL OnIdle(LONG lCount);
};

extern CUIEditorApp theApp;