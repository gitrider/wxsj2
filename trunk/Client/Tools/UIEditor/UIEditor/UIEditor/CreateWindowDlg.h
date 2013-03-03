/*****************************************************************************
文件:	CreateWindowDlg
作者:	方远
邮箱:	fangyuanning@126.com
说明:	创建窗口弹出的对话框
******************************************************************************/
#pragma once
#include "afxwin.h"


// CCreateWindowDlg dialog

class CCreateWindowDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateWindowDlg)

public:
	CCreateWindowDlg(BOOL setCheck, const CHAR* szDefaultName = "" ,CWnd* pParent = NULL);   // standard constructor
	virtual ~CCreateWindowDlg();

// Dialog Data
	enum { IDD = IDD_CREATE_WINDOW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	CButton m_CheckBox;
	CEdit m_Edit;

public:
	BOOL m_bSetCheck;
	CString m_szDefaultName;
public:
	virtual BOOL OnInitDialog();
};
