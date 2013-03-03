#pragma once
#include "afxwin.h"


// CDlgParamEdit dialog

class CDlgParamEdit : public CDialog
{
	DECLARE_DYNAMIC(CDlgParamEdit)

public:
	CDlgParamEdit(const char* szName, const char* szValue, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgParamEdit();

// Dialog Data
	enum { IDD = IDD_DIALOG_PARAM };

public:
	CString getParamName(void) const { return m_strParamName; }
	CString getParamValue(void) const { return m_strParamValue; }

protected:
	CString m_strParamName;		//!< the name of param
	CString m_strParamValue;	//!< the value of param

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_editParamName;
	CEdit m_editParamValue;
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedOk();
};
