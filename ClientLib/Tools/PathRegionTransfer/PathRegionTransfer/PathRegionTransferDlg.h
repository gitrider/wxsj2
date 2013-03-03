// PathRegionTransferDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include <string>

// CPathRegionTransferDlg dialog
class CPathRegionTransferDlg : public CDialog
{
// Construction
public:
	CPathRegionTransferDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PATHREGIONTRANSFER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void	ProcessPathFile(const std::string& szName);
	void	ProcessFolder(const CString& szFolderName);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedOk();
public:
	CButton m_bTransAllDir;
	CString m_szFileName;
	CString m_szFolderName;
public:
	CEdit m_Edit;
public:
	CStatic m_StatusText;
};
