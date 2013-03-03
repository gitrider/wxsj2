#pragma once
#include "afxwin.h"


// CFileFilterDlg dialog

class CFileFilterDlg : public CDialog
{
	DECLARE_DYNAMIC(CFileFilterDlg)

public:
	CFileFilterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFileFilterDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	CEdit m_ctrl;
	CString m_strText;
protected:
	virtual void OnOK();
};
