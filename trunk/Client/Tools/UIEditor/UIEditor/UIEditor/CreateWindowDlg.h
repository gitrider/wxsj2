/*****************************************************************************
�ļ�:	CreateWindowDlg
����:	��Զ
����:	fangyuanning@126.com
˵��:	�������ڵ����ĶԻ���
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
