#pragma once


// CDirectXErrorDlg dialog

class CDirectXErrorDlg : public CDialog
{
	DECLARE_DYNAMIC(CDirectXErrorDlg)

public:
	CDirectXErrorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDirectXErrorDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DX_ERROR };

protected:

    /// ��ʼ��dx setup����Ľӿ�
    bool InitDirectSetupAPIs();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButtonSetup();
};
