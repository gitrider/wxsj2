#pragma once
#include "afxwin.h"

// CNewVersionDlg dialog

class CNewVersionDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewVersionDlg)

public:
	CNewVersionDlg(CString& strVersion, CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewVersionDlg();

	CString	GetVersion(void) const { return m_strVersion; }
// Dialog Data
	enum { IDD = IDD_DIALOG_NEWVERSION };

protected:
	// ‰»Î ‰≥ˆ∞Ê±æ∫≈
	CEdit m_editVersion;
	CString		m_strVersion;
	CFont		m_hLargeFont;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnUpdateEditVersionNew();
protected:
	virtual void OnOK();
};
