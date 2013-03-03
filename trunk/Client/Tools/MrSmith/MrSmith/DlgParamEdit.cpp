// DlgParamEdit.cpp : implementation file
//

#include "stdafx.h"
#include "MrSmith.h"
#include "DlgParamEdit.h"
#include ".\dlgparamedit.h"


// CDlgParamEdit dialog

IMPLEMENT_DYNAMIC(CDlgParamEdit, CDialog)
CDlgParamEdit::CDlgParamEdit(const char* szName, const char* szValue, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgParamEdit::IDD, pParent)
	, m_strParamName(szName)
	, m_strParamValue(szValue)
{
}

CDlgParamEdit::~CDlgParamEdit()
{
}

void CDlgParamEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PARAM_NAME, m_editParamName);
	DDX_Control(pDX, IDC_EDIT_PARAM_VALUE, m_editParamValue);
}


BEGIN_MESSAGE_MAP(CDlgParamEdit, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgParamEdit message handlers

BOOL CDlgParamEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_editParamName.SetWindowText(m_strParamName);
	m_editParamValue.SetWindowText(m_strParamValue);

	if(m_strParamName.IsEmpty())
	{
		m_editParamName.SetFocus();
	}
	else
	{
		m_editParamValue.SetSel(0, -1);
		m_editParamValue.SetFocus();
	}

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgParamEdit::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	m_editParamName.GetWindowText(m_strParamName);
	m_editParamValue.GetWindowText(m_strParamValue);

	CDialog::OnOK();
}

void CDlgParamEdit::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
