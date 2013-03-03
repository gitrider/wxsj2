/*****************************************************************************
�ļ�:	FunctionDlg
����:	��Զ
����:	fangyuanning@126.com
˵��:	�������������Ҽ������Ĵ���
******************************************************************************/
#pragma once
#include "afxwin.h"


// CFunctionDlg dialog

class CFunctionDlg : public CDialog
{
	DECLARE_DYNAMIC(CFunctionDlg)

public:
	CFunctionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFunctionDlg();

// Dialog Data
	enum { IDD = IDD_FUNCTION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGo();
public:
	afx_msg void OnBnClickedCancel();
public:
	CComboBox m_RightCombo;
public:
	CComboBox m_LeftCombo;

public:
	void SetLeftFunction(CString& leftName) { m_LeftFunctionName = leftName; }
	void SetRightFunction(CString& rightName){ m_RightFunctionName = rightName; }
	void SetWindowName(CString& name){ m_CurrentWindowName = name; }
protected:
	CString m_LeftFunctionName;
	CString m_RightFunctionName;
	CString m_CurrentWindowName;
};
