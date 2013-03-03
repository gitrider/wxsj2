// LayoutXMLPreviewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UIEditor.h"
#include "LayoutXMLPreviewDlg.h"


// CLayoutXMLPreviewDlg dialog

IMPLEMENT_DYNAMIC(CLayoutXMLPreviewDlg, CDialog)

CLayoutXMLPreviewDlg::CLayoutXMLPreviewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLayoutXMLPreviewDlg::IDD, pParent)
{

}

CLayoutXMLPreviewDlg::~CLayoutXMLPreviewDlg()
{
}

void CLayoutXMLPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT21, m_richEdit);
}


BEGIN_MESSAGE_MAP(CLayoutXMLPreviewDlg, CDialog)
	ON_WM_SIZE()
	ON_COMMAND(ID_SAVE_XML, &CLayoutXMLPreviewDlg::OnSaveXml)
	ON_COMMAND(ID_CLOSE_XML, &CLayoutXMLPreviewDlg::OnCloseXml)
END_MESSAGE_MAP()


// CLayoutXMLPreviewDlg message handlers


void CLayoutXMLPreviewDlg::SetLayoutText(const CString& szText)
{
	m_richEdit.SetWindowText(utf8_to_mbcs(szText.GetString()));
}
BOOL CLayoutXMLPreviewDlg::OnInitDialog()
{
	return CDialog::OnInitDialog();
}

void CLayoutXMLPreviewDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	m_richEdit.MoveWindow(&CRect(0,0,cx,cy));
}

void CLayoutXMLPreviewDlg::OnSaveXml()
{
	CFileDialog dlg(FALSE, "", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"layoutÎÄ¼þ(*.layout.xml)|*.layout.xml||", AfxGetMainWnd());

	if ( dlg.DoModal() == IDOK )
	{
		CString szPathName = dlg.GetPathName();

		CString szSave;

		m_richEdit.GetWindowText(szSave);

		CString szUtf8  = mbcs_to_utf8(szSave);

		FILE* pfile = fopen(szPathName, "w");

		if (pfile)
		{
			fwrite(szUtf8.GetString(), szUtf8.GetLength(), 1, pfile);

			fclose(pfile);
		}
	}
}

void CLayoutXMLPreviewDlg::OnCloseXml()
{
	ShowWindow(FALSE);
}

void CLayoutXMLPreviewDlg::Initailzie()
{
	m_richEdit.Initialize();

	const WCHAR* szKeyword = L"GUILayout Window Name Property Function Event xml";
	m_richEdit.AddKeywords(szKeyword);
	m_richEdit.SetCaseSensitive(TRUE);
	m_richEdit.AddConstants(L"True False");
	m_richEdit.LimitText(1024*1024*10);
}
