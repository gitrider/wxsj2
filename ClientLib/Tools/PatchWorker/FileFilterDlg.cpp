// FileFilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PatchWorker.h"
#include "FileFilterDlg.h"
#include ".\filefilterdlg.h"


// CFileFilterDlg dialog

IMPLEMENT_DYNAMIC(CFileFilterDlg, CDialog)
CFileFilterDlg::CFileFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileFilterDlg::IDD, pParent)
{
}

CFileFilterDlg::~CFileFilterDlg()
{
}

void CFileFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_ctrl);
}

BEGIN_MESSAGE_MAP(CFileFilterDlg, CDialog)
//	ON_WM_CREATE()
END_MESSAGE_MAP()


// CFileFilterDlg message handlers

BOOL CFileFilterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	CStringArray sa;
	CString strText;

	WIN32_FILE_ATTRIBUTE_DATA wfat = {0};
	if( GetFileAttributesEx( "PatchWorker.ini", GetFileExInfoStandard, &wfat ) ) {
		char* szTemp = new char[wfat.nFileSizeLow+1];
		char* p = szTemp;
		GetPrivateProfileSection( "FF", szTemp, wfat.nFileSizeLow, "./PatchWorker.ini" );
		while( *p ) {
			if( !p ) break;
			_mbslwr( (byte*)p );
			strText += p;
			strText += "\r\n";
			p += strlen( p ) + 1;
		}
		delete [] szTemp;
	}

	m_ctrl.Clear();
	m_ctrl.SetWindowText( strText );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}




void CFileFilterDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	UpdateData( TRUE );

	char szLine[4096];
	int nRet = m_ctrl.GetWindowText( szLine, 4096 );
	m_strText = szLine;

	FILE* fp = fopen( "PatchWorker.ini", "wb" );
	if( fp ) {
		fwrite( "[FF]\r\n", 1, 6, fp );
		fwrite( szLine, 1, nRet, fp );
		fclose( fp );
	}

	CDialog::OnOK();
}
