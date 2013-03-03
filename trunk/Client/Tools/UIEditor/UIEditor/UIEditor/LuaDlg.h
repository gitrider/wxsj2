/*****************************************************************************
�ļ�:	LuaDlg
����:	��Զ
����:	fangyuanning@126.com
˵��:	Ԥ��lua�ű��ĶԻ���
******************************************************************************/
#pragma once
#include "afxcmn.h"
#include "TWScriptEdit.h"
#include "resource.h"


// CLuaDlg dialog

class CLuaDlg : public CDialog
{
	DECLARE_DYNAMIC(CLuaDlg)

public:
	CLuaDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLuaDlg();

// Dialog Data
	enum { IDD = IDD_LUA_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTWScriptEdit m_richEditCtrl;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void OnCancel();
	VOID	Initailzie(VOID);

public:
	void	SetLuaText(const CString& text);
	afx_msg void OnOpenOrCloseLua();
	void	AddKeywords(const char* szKeyword);
	void	AddConstants(const char* szKeyword);
public:
	afx_msg void OnSaveLua();
public:
	afx_msg void OnClose();
public:
	afx_msg void OnAboutLua();
public:
	afx_msg void OnRefreshLua();
};
