// NewVersionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PatchWorker.h"
#include "NewVersionDlg.h"
#include ".\newversiondlg.h"


// CNewVersionDlg dialog

IMPLEMENT_DYNAMIC(CNewVersionDlg, CDialog)
CNewVersionDlg::CNewVersionDlg(CString& strVersion, CWnd* pParent /*=NULL*/)
	: CDialog(CNewVersionDlg::IDD, pParent), 
	m_strVersion(strVersion)
{
}

CNewVersionDlg::~CNewVersionDlg()
{
}

void CNewVersionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_VERSION_NEW, m_editVersion);
}


BEGIN_MESSAGE_MAP(CNewVersionDlg, CDialog)
	ON_EN_UPDATE(IDC_EDIT_VERSION_NEW, OnEnUpdateEditVersionNew)
END_MESSAGE_MAP()


// CNewVersionDlg message handlers
BOOL CNewVersionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_editVersion.SetWindowText(m_strVersion);
	::EnableWindow(GetDlgItem(IDOK)->GetSafeHwnd(), m_strVersion.GetLength() != 0);
	
	m_hLargeFont.CreateFont(24,0,0,0,
		FW_NORMAL,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FF_MODERN | FIXED_PITCH | FF_SWISS,
		"ËÎÌו");
	GetDlgItem(IDC_EDIT_VERSION_NEW)->SetFont(&m_hLargeFont);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNewVersionDlg::OnEnUpdateEditVersionNew()
{
	CString strTemp;
	m_editVersion.GetWindowText(strTemp);
	::EnableWindow(GetDlgItem(IDOK)->GetSafeHwnd(), strTemp.GetLength() != 0);
}

void CNewVersionDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	m_editVersion.GetWindowText(m_strVersion);
	
	CDialog::OnOK();
}
