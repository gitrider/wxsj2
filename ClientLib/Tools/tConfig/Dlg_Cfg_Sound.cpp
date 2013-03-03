// Dlg_Cfg_Sound.cpp : implementation file
//

#include "stdafx.h"
#include "tConfig.h"
#include "Dlg_Cfg_Sound.h"
#include ".\dlg_cfg_sound.h"


// CDlg_Cfg_Sound dialog

IMPLEMENT_DYNAMIC(CDlg_Cfg_Sound, CDialog)
CDlg_Cfg_Sound::CDlg_Cfg_Sound(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Cfg_Sound::IDD, pParent)
{
}

CDlg_Cfg_Sound::~CDlg_Cfg_Sound()
{
}

void CDlg_Cfg_Sound::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_Cfg_Sound, CDialog)
END_MESSAGE_MAP()


// CDlg_Cfg_Sound message handlers

void CDlg_Cfg_Sound::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

//	CDialog::OnCancel();
}

void CDlg_Cfg_Sound::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

//	CDialog::OnOK();
}
