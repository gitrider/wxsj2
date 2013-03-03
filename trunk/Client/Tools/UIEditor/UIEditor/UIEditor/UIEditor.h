// UIEditor.h : UIEditor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#include "interface.h"

// CUIEditorApp:
// �йش����ʵ�֣������ UIEditor.cpp
//

class CUIEditorApp : public CWinApp
{
public:
	CUIEditorApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance( );

public:
	CUIEditor m_uiEiditor;
	INT m_ShowConsole;
	BOOL m_needCallSkinExit;

// ʵ��
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