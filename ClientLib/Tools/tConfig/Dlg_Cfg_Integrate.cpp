// Dlg_Cfg_Integrate.cpp : implementation file
//

#include "stdafx.h"
#include "tConfig.h"
#include "Dlg_Cfg_Integrate.h"
#include ".\dlg_cfg_integrate.h"
#include "tConfigDlg.h"
#include "DefConfig.h"
#include "StaticLink.h"
#include "TDUnicodeLayer.h"
#include "InnerString.h"

// CDlg_Cfg_Integrate dialog

IMPLEMENT_DYNAMIC(CDlg_Cfg_Integrate, CDialog)
CDlg_Cfg_Integrate::CDlg_Cfg_Integrate(CWnd* pParent /*=NULL*/)
	: CDialog(g_nCodePage==1258? (CDlg_Cfg_Integrate::IDD_VN) : (CDlg_Cfg_Integrate::IDD), pParent)
{
}

CDlg_Cfg_Integrate::~CDlg_Cfg_Integrate()
{
	DeleteLinkFont();
}

void CDlg_Cfg_Integrate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_Cfg_Integrate, CDialog)
	ON_BN_CLICKED(IDC_RADIO_SET_LOW, OnBnClickedRadioSetLow)
	ON_BN_CLICKED(IDC_RADIO_SET_MIDDLE, OnBnClickedRadioSetMiddle)
	ON_BN_CLICKED(IDC_RADIO_SET_HIGH, OnBnClickedRadioSetHigh)
	ON_BN_CLICKED(IDC_RADIO_SET_DEF, OnBnClickedRadioSetDef)
END_MESSAGE_MAP()


// CDlg_Cfg_Integrate message handlers

void CDlg_Cfg_Integrate::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

//	CDialog::OnOK();
}

void CDlg_Cfg_Integrate::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

//	CDialog::OnCancel();
}

BOOL CDlg_Cfg_Integrate::OnInitDialog()
{
	CDialog::OnInitDialog();

	TD_SetWindowTextW(GetDlgItem(IDC_STATIC_SYSTEM_GROUP)->GetSafeHwnd(), 
		getInnerString(INNER_XI_TONG_XIN_XI));

	TD_SetWindowTextW(GetDlgItem(IDC_STATIC_SETTING_GROUP)->GetSafeHwnd(), 
		getInnerString(INNER_YOU_XI_SHE_ZHI));


	TD_SetWindowTextW(GetDlgItem(IDC_RADIO_SET_DEF)->GetSafeHwnd(), 
		getInnerString(INNER_SETTING_DEF_DESC));

	TD_SetWindowTextW(GetDlgItem(IDC_RADIO_SET_LOW)->GetSafeHwnd(), 
		getInnerString(INNER_SETTING_LOW_DESC));
	TD_SetWindowTextW(GetDlgItem(IDC_RADIO_SET_MIDDLE)->GetSafeHwnd(), 
		getInnerString(INNER_SETTING_MID_DESC));
	TD_SetWindowTextW(GetDlgItem(IDC_RADIO_SET_HIGH)->GetSafeHwnd(), 
		getInnerString(INNER_SETTING_HIGH_DESC));

	CreateLinkFont(GetSafeHwnd());
	static AX_URL_LNK g_TipsLink = 
	{ 
		"1", 
		NULL, TRUE, 0, 
		getInnerString(INNER_STRING_ZHU_YI),
	};
	
	HWND hTipsWnd = GetDlgItem(IDC_STATIC_TIPS)->GetSafeHwnd();

//	::GetWindowText(hTipsWnd, g_TipsLink.szText, 32);
	SetWindowLong(hTipsWnd, GWL_USERDATA, (LONG)(LONG_PTR)(&g_TipsLink));
	g_TipsLink.m_oldWndProc = (WNDPROC)(LONG_PTR)SetWindowLong(hTipsWnd, GWL_WNDPROC, (LONG)(LONG_PTR)StaticLnkWndProc);

	// TODO:  Add extra initialization here
	CtConfigDlg* pMainWnd = (CtConfigDlg*)(AfxGetApp()->GetMainWnd());
	if(pMainWnd)
	{
		TD_SetWindowTextW(GetDlgItem(IDC_EDIT_INFO)->m_hWnd, gHardwareInfoSystem.getHardwareInfoAsString());
	}

	//Init tooltip
	m_tooltips.Create(this, TTS_NOPREFIX | TTS_ALWAYSTIP);
	m_tooltips.SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_tooltips.ShowWindow(SW_SHOW);

	RECT rect;
	GetDlgItem(IDC_STATIC_TIPS)->GetClientRect(&rect);

    /* INITIALIZE MEMBERS OF THE TOOLINFO STRUCTURE */
	TOOLINFO ti;
    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_SUBCLASS;
    ti.hwnd = GetDlgItem(IDC_STATIC_TIPS)->GetSafeHwnd();
    ti.hinst = 0;//AfxGetApp()->m_hInstance;
    ti.uId = 0;
    ti.lpszText = (LPWSTR)getInnerString(INNER_SET_TIPS_DESC);
        // ToolTip control will cover the whole window
    ti.rect.left = rect.left;    
    ti.rect.top = rect.top;
    ti.rect.right = rect.right;
    ti.rect.bottom = rect.bottom;

    /* SEND AN ADDTOOL MESSAGE TO THE TOOLTIP CONTROL WINDOW */
    m_tooltips.SendMessage(TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);	
	m_tooltips.SetMaxTipWidth(220);
	m_tooltips.SetDelayTime(TTDT_AUTOPOP, 1000000);
	m_tooltips.SetDelayTime(TTDT_INITIAL, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_Cfg_Integrate::toCommendConfig(void)
{
	CheckRadioButton(IDC_RADIO_SET_LOW, IDC_RADIO_SET_DEF, IDC_RADIO_SET_DEF);
	OnBnClickedRadioSetDef();
}

void CDlg_Cfg_Integrate::OnBnClickedRadioSetLow()
{
	// TODO: Add your control notification handler code here
	for(int i=0; i<g_cfgLowSize; i++)
	{
		g_theGameConfigs.SetValue(g_cfgLow[i].szName, atoi(g_cfgLow[i].szValue));
	}

	//刷新
	((CtConfigDlg*)(AfxGetApp()->GetMainWnd()))->getVideoDlg()->refreshFromModifyed();
}

void CDlg_Cfg_Integrate::OnBnClickedRadioSetMiddle()
{
	// TODO: Add your control notification handler code here
	for(int i=0; i<g_cfgMiddleSize; i++)
	{
		g_theGameConfigs.SetValue(g_cfgMiddle[i].szName, atoi(g_cfgMiddle[i].szValue));
	}

	//刷新
	((CtConfigDlg*)(AfxGetApp()->GetMainWnd()))->getVideoDlg()->refreshFromModifyed();
}

void CDlg_Cfg_Integrate::OnBnClickedRadioSetHigh()
{
	// TODO: Add your control notification handler code here
	for(int i=0; i<g_cfgHighSize; i++)
	{
		g_theGameConfigs.SetValue(g_cfgHigh[i].szName, atoi(g_cfgHigh[i].szValue));
	}

	//刷新
	((CtConfigDlg*)(AfxGetApp()->GetMainWnd()))->getVideoDlg()->refreshFromModifyed();
}

void CDlg_Cfg_Integrate::freshSelect(void)
{
	CheckRadioButton(IDC_RADIO_SET_LOW, IDC_RADIO_SET_DEF, -1);
}

void CDlg_Cfg_Integrate::OnBnClickedRadioSetDef()
{
	// TODO: Add your control notification handler code here
	for(int i=0; i<g_cfgCommendSize; i++)
	{
		g_theGameConfigs.SetValue(g_cfgCommend[i].szName, atoi(g_cfgCommend[i].szValue));
	}

	//刷新
	((CtConfigDlg*)(AfxGetApp()->GetMainWnd()))->getVideoDlg()->refreshFromModifyed();
}
