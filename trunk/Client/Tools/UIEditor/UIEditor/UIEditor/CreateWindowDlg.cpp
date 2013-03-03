// CreateWindowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UIEditor.h"
#include "CreateWindowDlg.h"
#include <CEGUIWindowManager.h>

using namespace CEGUI;


// CCreateWindowDlg dialog

IMPLEMENT_DYNAMIC(CCreateWindowDlg, CDialog)

CCreateWindowDlg::CCreateWindowDlg(BOOL setCheck, const CHAR* szDefaultName/* = NULL */,CWnd* pParent /*=NULL*/)
	: CDialog(CCreateWindowDlg::IDD, pParent)
{
	m_bSetCheck = setCheck;
	if( szDefaultName != NULL )
	{
		m_szDefaultName = szDefaultName;
	}
}

CCreateWindowDlg::~CCreateWindowDlg()
{
}

void CCreateWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_CheckBox);
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
}


BEGIN_MESSAGE_MAP(CCreateWindowDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CCreateWindowDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCreateWindowDlg message handlers

void CCreateWindowDlg::OnBnClickedOk()
{
	CString name ;
	m_Edit.GetWindowText(name);
	if (name.IsEmpty())
	{
		MessageBox("请输入窗口名称","提示",MB_OK);
		return;
	}

	//判断是否系统里有这个名字的窗口
	if( WindowManager::getSingleton().isWindowPresent(name.GetString()) )
	{
		MessageBox("输入的窗口名称重复,目前系统里有这个窗口","提示",MB_OK);
		return;
	}
	
	//获取文本
	m_Edit.GetWindowText(m_szDefaultName);

	OnOK();
}

BOOL CCreateWindowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Edit.SetWindowText(m_szDefaultName);

	m_CheckBox.SetCheck(m_bSetCheck);

	m_Edit.SetFocus();

	m_Edit.SetSel(m_szDefaultName.GetLength(), m_szDefaultName.GetLength());

	return FALSE; 
}
