// FunctionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UIEditor.h"
#include "FunctionDlg.h"
#include "DataPool.h"


// CFunctionDlg dialog

IMPLEMENT_DYNAMIC(CFunctionDlg, CDialog)

CFunctionDlg::CFunctionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFunctionDlg::IDD, pParent)
{

}

CFunctionDlg::~CFunctionDlg()
{
}

void CFunctionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_RIGHT, m_RightCombo);
	DDX_Control(pDX, IDC_COMBO_LEFT, m_LeftCombo);
}

BOOL CFunctionDlg::OnInitDialog()
{
	BOOL bRet =  CDialog::OnInitDialog();
	m_LeftCombo.AddString("");
	m_LeftCombo.AddString(m_LeftFunctionName);
	m_RightCombo.AddString(m_RightFunctionName);
	m_RightCombo.AddString("");

	if (m_LeftFunctionName.GetLength() > 0)
	{
		m_LeftCombo.SetCurSel(1);
	}
	if (m_RightFunctionName.GetLength() > 0)
	{
		m_RightCombo.SetCurSel(1);
	}
	return bRet;
}



BEGIN_MESSAGE_MAP(CFunctionDlg, CDialog)
	ON_BN_CLICKED(IDC_GO, &CFunctionDlg::OnBnClickedGo)
	ON_BN_CLICKED(IDC_CANCEL, &CFunctionDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CFunctionDlg message handlers

void CFunctionDlg::OnBnClickedGo()
{
	CString leftFunction, rightFunction;
	m_LeftCombo.GetWindowText(leftFunction);
	m_RightCombo.GetWindowText(rightFunction);
	g_DataPool.OnAddFunctionDlg(m_CurrentWindowName, leftFunction, rightFunction);
	CDialog::OnOK();
}

void CFunctionDlg::OnBnClickedCancel()
{
	CDialog::OnCancel();
}
