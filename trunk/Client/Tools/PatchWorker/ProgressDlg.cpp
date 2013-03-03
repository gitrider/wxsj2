// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PatchWorker.h"
#include "ProgressDlg.h"
#include "FileCollector.h"

// CProgressDlg dialog

IMPLEMENT_DYNAMIC(CProgressDlg, CDialog)
CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDlg::IDD, pParent)
{
	m_pWorldThread = 0;
	m_bUserAbort = FALSE;
}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_PROGRESS_COLLFILE, OnProgress_CollFile)
	ON_MESSAGE(WM_PROGRESS_TITLE, OnProgress_Title)
	ON_MESSAGE(WM_PROGRESS_ZIPFILE, OnProgress_ZipFile)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CProgressDlg message handlers
void CProgressDlg::DoModal(CWinThread* pThread)
{
	m_pWorldThread = pThread;
	m_bUserAbort = FALSE;
	CDialog::DoModal();
}

BOOL CProgressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	if(m_pWorldThread)
	{
		m_pWorldThread->ResumeThread();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CProgressDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	EndDialog(0);

	m_pWorldThread = 0;
}

LRESULT CProgressDlg::OnProgress_Title(WPARAM wParam, LPARAM lParam)
{
	const char* szTitle = (const char*)wParam;

	this->SetDlgItemText(IDC_STATIC_TITLE, szTitle);

	return 0;
}

LRESULT CProgressDlg::OnProgress_CollFile(WPARAM wParam, LPARAM lParam)
{
	const char* szFileName = (const char*)wParam;

	this->SetDlgItemText(IDC_STATIC_PROGRESS, szFileName);

	return 0;
}

LRESULT CProgressDlg::OnProgress_ZipFile(WPARAM wParam, LPARAM lParam)
{
	const char* szFileName = (const char*)wParam;

	this->SetDlgItemText(IDC_STATIC_PROGRESS, szFileName);

	return 0;
}

void CProgressDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialog::OnCancel();
}


void CProgressDlg::OnBnClickedButtonCancel()
{
	m_bUserAbort = TRUE;

	OnClose();
}
