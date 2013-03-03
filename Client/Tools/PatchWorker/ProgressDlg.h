#pragma once


// CProgressDlg dialog

class CProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(CProgressDlg)
public:
	void DoModal(CWinThread* pThread);
	BOOL	GetUserAbort(void) const { return m_bUserAbort; }

protected:
	CWinThread*	m_pWorldThread;
	BOOL		m_bUserAbort;

public:
	CProgressDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProgressDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg LRESULT OnProgress_CollFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgress_Title(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgress_ZipFile(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonCancel();
protected:
	virtual void OnCancel();
};
