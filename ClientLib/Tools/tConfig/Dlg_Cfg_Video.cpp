// Dlg_Cfg_Video.cpp : implementation file
//

#include "stdafx.h"
#include "tConfig.h"
#include "Dlg_Cfg_Video.h"
#include ".\dlg_cfg_video.h"
#include "ControlNode.h"
#include "tConfigDlg.h"
#include "TDUnicodeLayer.h"
#include "InnerString.h"

//--------------------------------
//视频设置

//使用Slider控制的项目
SliderNode g_theSliderNodes[] = 
{
	{ "View_LightmapQuality",	3, {0, 1, 2}, {INNER_LEVEL_NULL,INNER_LEVEL_LOW,	INNER_LEVEL_HIGH},	IDC_SLIDER_VIDEO_1, IDC_STATIC_VIDEO_1},
	{ "View_Fanhunjiao",		3, {0, 1, 2}, {INNER_LEVEL_0,	INNER_LEVEL_2,		INNER_LEVEL_4},		IDC_SLIDER_VIDEO_2, IDC_STATIC_VIDEO_2},
	{ "View_ObjectMove",		3, {0, 1, 2}, {INNER_LEVEL_LOW,	INNER_LEVEL_MID,	INNER_LEVEL_HIGH},	IDC_SLIDER_VIDEO_3, IDC_STATIC_VIDEO_3},

	{ "View_ColorQuality",		3, {0, 1, 2}, {INNER_LEVEL_16,	INNER_LEVEL_32,		INNER_LEVEL_DESKTOP},IDC_SLIDER_VIDEO_4, IDC_STATIC_VIDEO_5},
	{ "View_TextureFiltering",	3, {0, 1, 2}, {INNER_LEVEL_LOW,	INNER_LEVEL_MID,	INNER_LEVEL_HIGH},	IDC_SLIDER_VIDEO_5, IDC_STATIC_VIDEO_6},
	{ "View_HumanLightmap",		3, {0, 1, 2}, {INNER_LEVEL_LOW,	INNER_LEVEL_MID,	INNER_LEVEL_HIGH},	IDC_SLIDER_VIDEO_7, IDC_STATIC_VIDEO_7},
};

//使用CheckBox控制的项目
CheckNode g_theCheckNodes[] =
{
	{"View_FullScreenLight",	IDC_CHECK1},
	{"View_Aplomb",				IDC_CHECK2},
	{"View_FullScreen",			IDC_CHECK3},
};


// CDlg_Cfg_Video dialog

IMPLEMENT_DYNAMIC(CDlg_Cfg_Video, CDialog)
CDlg_Cfg_Video::CDlg_Cfg_Video(CWnd* pParent /*=NULL*/)
	: CDialog(g_nCodePage==1258? (CDlg_Cfg_Video::IDD_VN) : (CDlg_Cfg_Video::IDD), pParent)
{
}

CDlg_Cfg_Video::~CDlg_Cfg_Video()
{
}

void CDlg_Cfg_Video::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, Combo_CardSet);
	DDX_Control(pDX, IDC_EDIT2, m_edtMaxPitch);
	DDX_Control(pDX, IDC_EDIT3, m_edtMinPitch);
}


BEGIN_MESSAGE_MAP(CDlg_Cfg_Video, CDialog)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnCbnSelchangeCombo2)
	ON_EN_CHANGE(IDC_EDIT2, OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, OnEnChangeEdit3)
//	ON_EN_UPDATE(IDC_EDIT2, OnEnUpdateEdit2)
END_MESSAGE_MAP()


// CDlg_Cfg_Video message handlers

void CDlg_Cfg_Video::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

//	CDialog::OnCancel();
}

void CDlg_Cfg_Video::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

//	CDialog::OnOK();
}

BOOL CDlg_Cfg_Video::OnInitDialog()
{
	CDialog::OnInitDialog();

	TD_SetWindowTextW(GetDlgItem(IDC_STATIC_GENERAL_GROUP)->GetSafeHwnd(), 
		getInnerString(INNER_YI_BAN_XUAN_XIANG));
	TD_SetWindowTextW(GetDlgItem(IDC_STATIC_SCREEN_GROUP)->GetSafeHwnd(), 
		getInnerString(INNER_PING_MU_XIAO_GUO));
	TD_SetWindowTextW(GetDlgItem(IDC_STATIC_TEXTURE_GROUP)->GetSafeHwnd(), 
		getInnerString(INNER_WEN_LI_ZHI_LIANG));
	TD_SetWindowTextW(GetDlgItem(IDC_STATIC_OTHER_GROUP)->GetSafeHwnd(), 
		getInnerString(INNER_QI_TA_XUAN_XIANG));
	TD_SetWindowTextW(GetDlgItem(IDC_STATIC_OTHER_GROUP)->GetSafeHwnd(), 
		getInnerString(INNER_QI_TA_XUAN_XIANG));

	TD_SetWindowTextW(GetDlgItem(IDC_STATIC_DIXINGYINYING)->GetSafeHwnd(), 
		getInnerString(INNER_DI_XING_YIN_YING));
	TD_SetWindowTextW(GetDlgItem(IDC_STATIC_QUANPING)->GetSafeHwnd(), 
		getInnerString(INNER_QUANPING_GANGJUCHI));
	TD_SetWindowTextW(GetDlgItem(IDC_STATIC_ANIM)->GetSafeHwnd(), 
		getInnerString(INNER_WUTI_DONGHUA));

	TD_SetWindowTextW(GetDlgItem(IDC_STATIC_TEX_QUALITY)->GetSafeHwnd(), 
		getInnerString(INNER_WENLI_ZHILIANG));
	TD_SetWindowTextW(GetDlgItem(IDC_STATIC_COL_QUALITY)->GetSafeHwnd(), 
		getInnerString(INNER_YANSE_ZHILIANG));
	TD_SetWindowTextW(GetDlgItem(IDC_STATIC_CAIYANG)->GetSafeHwnd(), 
		getInnerString(INNER_CAI_YANG));
	
	TD_SetWindowTextW(GetDlgItem(IDC_STATIC_CHAR_ANIM)->GetSafeHwnd(), 
		getInnerString(INNER_JUESE_YINYING));
	TD_SetWindowTextW(GetDlgItem(IDC_CHECK1)->GetSafeHwnd(), 
		getInnerString(INNER_QUANPING_FANGUANG));

	TD_SetWindowTextW(GetDlgItem(IDC_CHECK2)->GetSafeHwnd(), 
		getInnerString(INNER_ZHUIZHI_TONGBU));
	TD_SetWindowTextW(GetDlgItem(IDC_CHECK3)->GetSafeHwnd(), 
		getInnerString(INNER_QUANPING_MOSHI));
	TD_SetWindowTextW(GetDlgItem(IDC_STATIC_XIANKA_JIASU)->GetSafeHwnd(), 
		getInnerString(INNER_XIANKA_JIASU));


	//Load lastest config file
	CGameConfig* pGameConfig = g_theGameConfigs.GetLastestConfig();
	ASSERT(pGameConfig);

	INT nSliderNum = sizeof(g_theSliderNodes)/sizeof(SliderNode);
	for(int i=0; i<nSliderNum; i++)
	{
		_refreshSliderFromValue(i, pGameConfig->GetValue(g_theSliderNodes[i].szVariableName));
	}

	INT nCheckNum = sizeof(g_theCheckNodes)/sizeof(CheckNode);
	for(int i=0; i<nCheckNum; i++)
	{
		_refreshCheckFromValue(i, pGameConfig->GetValue(g_theCheckNodes[i].szVariableName));
	}

	//FPS
	const char* szDefFPS[] = {"15", "30", "35"};
	for(int i=0; i<sizeof(szDefFPS)/sizeof(const char*); i++)
	{
		SendDlgItemMessage(IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)szDefFPS[i]);
	}
	//get fps in config
	int fps = atoi(pGameConfig->GetValue("System_MaxFPS"));
	bool bSel = false;
	for(int i=0; i<sizeof(szDefFPS)/sizeof(const char*); i++)
	{
		if(atoi(szDefFPS[i]) == fps)
		{
			SendDlgItemMessage(IDC_COMBO1, CB_SETCURSEL, i, 0);
			bSel = true;
			break;
		}
	}
	if(!bSel)
	{
		char szTemp[32];
		_snprintf(szTemp, 32, "%d", fps);
		SendDlgItemMessage(IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)szTemp);
		SendDlgItemMessage(IDC_COMBO1, CB_SETCURSEL, sizeof(szDefFPS)/sizeof(const char*), 0);
	}

	//显卡加速
	Combo_CardSet.AddString( getInnerString(INNER_HARDACCE_LEVEL1));
	Combo_CardSet.AddString( getInnerString(INNER_HARDACCE_LEVEL2));
	Combo_CardSet.AddString( getInnerString(INNER_HARDACCE_LEVEL3));

	int VideoCardSetting = atoi(pGameConfig->GetValue("Accelerate_VideoCard"));
	Combo_CardSet.SetCurSel( VideoCardSetting );

	//初始化仰角极限值
	m_edtMaxPitch.SetLimitText( 2 );
	m_edtMinPitch.SetLimitText( 2 );
	::SetWindowTextA( m_edtMaxPitch.m_hWnd, pGameConfig->GetValue("Camera_MaxPitch") ) ;
	::SetWindowTextA( m_edtMinPitch.m_hWnd, pGameConfig->GetValue("Camera_MinPitch") ) ;

    _recheckVideoOption();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_Cfg_Video::_recheckVideoOption(void)
{
    const VideoOptionMap& videoOptionMap = gHardwareInfoSystem.getVideoOptionMap();
	CGameConfig* pGameConfig = g_theGameConfigs.GetLastestConfig();

    VideoOptionMap::const_iterator it = videoOptionMap.begin();

#if 0
    while (it != videoOptionMap.end())
    {
        char debugString[256];
        sprintf(debugString, "%s : %d : %d\n", it->first.c_str(), it->second.mMinValue, it->second.mMaxValue);
        ::OutputDebugString(debugString);
        ++it;
    }
#endif

	bool bInvalidExist = false;

    INT nSliderNum = sizeof(g_theSliderNodes)/sizeof(SliderNode);
    for(int i=0; i<nSliderNum; i++)
    {
        std::string variableName(g_theSliderNodes[i].szVariableName);

        VideoOptionMap::const_iterator it = videoOptionMap.find(variableName);

        if (it != videoOptionMap.end())
        {
            // 禁用选项
            if (it->second.mMaxValue == 0)
                GetDlgItem(g_theSliderNodes[i].nID_Slider)->EnableWindow(FALSE);

			if(atoi(pGameConfig->GetValue(variableName.c_str())) > it->second.mMaxValue)
			{
				bInvalidExist = true;
				g_theGameConfigs.SetValue(variableName.c_str(), 0);
			}
        }
    }

    INT nCheckNum = sizeof(g_theCheckNodes)/sizeof(CheckNode);
    for(int i=0; i<nCheckNum; i++)
    {
        std::string variableName(g_theCheckNodes[i].szVariableName);

        VideoOptionMap::const_iterator it = videoOptionMap.find(variableName);

        if (it != videoOptionMap.end())
        {
            // 禁用选项
            if (it->second.mMaxValue == 0)
                GetDlgItem(g_theCheckNodes[i].nID_Slider)->EnableWindow(FALSE);

			if(atoi(pGameConfig->GetValue(variableName.c_str())) > it->second.mMaxValue)
			{
				bInvalidExist = true;
				g_theGameConfigs.SetValue(variableName.c_str(), 0);
			}
		}
    }

	if(bInvalidExist)
	{
		TD_MessageBoxW(m_hWnd, getInnerString(INNER_CANT_WORK), getInnerString(INNER_TITLE), MB_OK|MB_ICONINFORMATION);
		refreshFromModifyed();
	}
}

void CDlg_Cfg_Video::refreshFromModifyed(void)
{
	INT nSliderNum = sizeof(g_theSliderNodes)/sizeof(SliderNode);
	for(int i=0; i<nSliderNum; i++)
	{
		if(g_theGameConfigs.IsVariableModifyed(g_theSliderNodes[i].szVariableName))
		{
			_refreshSliderFromValue(i, g_theGameConfigs.GetModifyedValue(g_theSliderNodes[i].szVariableName));
		}
	}

	INT nCheckNum = sizeof(g_theCheckNodes)/sizeof(CheckNode);
	for(int i=0; i<nCheckNum; i++)
	{
		if(g_theGameConfigs.IsVariableModifyed(g_theCheckNodes[i].szVariableName))
		{
			_refreshCheckFromValue(i, g_theGameConfigs.GetModifyedValue(g_theCheckNodes[i].szVariableName));
		}
	}
}

void CDlg_Cfg_Video::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default SB_LEFT   
	INT nSliderNum = sizeof(g_theSliderNodes)/sizeof(SliderNode);
	for(int i=0; i<nSliderNum; i++)
	{
		if(GetDlgItem(g_theSliderNodes[i].nID_Slider) == ((CWnd*)pScrollBar))
		{
			_refreshSliderToValue(i);
		}
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlg_Cfg_Video::_refreshSliderToValue(int nIndex)
{
	static bool MUTEX = false;

	if(nIndex < 0 || nIndex >= sizeof(g_theSliderNodes)/sizeof(SliderNode) || MUTEX) return;

	int nValue = (int)SendDlgItemMessage(g_theSliderNodes[nIndex].nID_Slider, TBM_GETPOS, 0, 0);
	if(nValue < 0 || nValue >= g_theSliderNodes[nIndex].nValueNum) return;

	MUTEX = true;

	if(!_checkValueInvalid(
		g_theSliderNodes[nIndex].szVariableName, 
		g_theSliderNodes[nIndex].nValueBuf[nValue], true)) 
	{
		nValue = 0;
		SendDlgItemMessage(g_theSliderNodes[nIndex].nID_Slider, TBM_SETPOS, TRUE, 0);
	}

	SetDlgItemText(g_theSliderNodes[nIndex].nID_Desc, getInnerString((INNER_STRING_ID)g_theSliderNodes[nIndex].szValueDesc[nValue]));

	g_theGameConfigs.SetValue(g_theSliderNodes[nIndex].szVariableName, 
		g_theSliderNodes[nIndex].nValueBuf[nValue]);
	((CtConfigDlg*)(AfxGetApp()->GetMainWnd()))->getIntergrateDlg()->freshSelect();

	MUTEX = false;
}

bool CDlg_Cfg_Video::_checkValueInvalid(const char* szValueName, int nValue, bool bShowWarning)
{
    const VideoOptionMap& videoOptionMap = gHardwareInfoSystem.getVideoOptionMap();
	VideoOptionMap::const_iterator it = videoOptionMap.find(szValueName);
	if(it != videoOptionMap.end() && nValue > it->second.mMaxValue)
	{
		//超过最大值
		if(bShowWarning)
		{
			TD_MessageBoxW(m_hWnd, getInnerString(INNER_NOT_SUPPORT), getInnerString(INNER_TITLE), MB_OK|MB_ICONSTOP);
		}
		return false;
	}
	return true;
}

void CDlg_Cfg_Video::_refreshSliderFromValue(int nIndex, const char* szValue)
{
	if(nIndex < 0 || nIndex >= sizeof(g_theSliderNodes)/sizeof(SliderNode)) return;

	SendDlgItemMessage(g_theSliderNodes[nIndex].nID_Slider, TBM_SETRANGE, FALSE, MAKELONG(0, g_theSliderNodes[nIndex].nValueNum-1));
	for(int i=0; i<(int)g_theSliderNodes[nIndex].nValueNum; i++)
	{
		if(g_theSliderNodes[nIndex].nValueBuf[i] == atoi(szValue))
		{
			SendDlgItemMessage(
				g_theSliderNodes[nIndex].nID_Slider, TBM_SETPOS, TRUE, i);

			SetDlgItemText(g_theSliderNodes[nIndex].nID_Desc, getInnerString((INNER_STRING_ID)g_theSliderNodes[nIndex].szValueDesc[i]));
			break;
		}
	}
}

void CDlg_Cfg_Video::_refreshCheckFromValue(int nIndex, const char* szValue)
{
	if(nIndex < 0 || nIndex >= sizeof(g_theCheckNodes)/sizeof(CheckNode)) return;

	CheckDlgButton(g_theCheckNodes[nIndex].nID_Slider, atoi(szValue)!=0 ? BST_CHECKED : BST_UNCHECKED);
}

void CDlg_Cfg_Video::_refreshCheckToValue(int nIndex)
{
	if(nIndex < 0 || nIndex >= sizeof(g_theCheckNodes)/sizeof(CheckNode)) return;
	
	BOOL bChecked = IsDlgButtonChecked(g_theCheckNodes[nIndex].nID_Slider) == BST_CHECKED;
	if(!_checkValueInvalid(
		g_theCheckNodes[nIndex].szVariableName, bChecked ? 1 : 0, true)) 
	{
		bChecked = FALSE;
	}

	g_theGameConfigs.SetValue(g_theCheckNodes[nIndex].szVariableName, bChecked ? 1 : 0);

	((CtConfigDlg*)(AfxGetApp()->GetMainWnd()))->getIntergrateDlg()->freshSelect();
}

BOOL CDlg_Cfg_Video::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	//Is check
	for(int i=0; i<sizeof(g_theCheckNodes)/sizeof(CheckNode); i++)
	{
		if(LOWORD(wParam) == g_theCheckNodes[i].nID_Slider)
		{
			_refreshCheckToValue(i);
		}
	}
	
	return CDialog::OnCommand(wParam, lParam);
}

void CDlg_Cfg_Video::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
}

void CDlg_Cfg_Video::OnCbnSelchangeCombo2()
{
	// TODO: Add your control notification handler code here
	int VideoCardSetting = Combo_CardSet.GetCurSel();
	g_theGameConfigs.SetValue( "Accelerate_VideoCard", VideoCardSetting );

}

void CDlg_Cfg_Video::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	char tmp[10] ;
	memset( tmp, 0, 10 ) ;
	::GetWindowTextA( m_edtMaxPitch.m_hWnd, tmp, 10 ) ;
	g_theGameConfigs.SetValue( "Camera_MaxPitch", tmp );
}

void CDlg_Cfg_Video::OnEnChangeEdit3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	char tmp[10] ;
	memset( tmp, 0, 10 ) ;
	::GetWindowTextA( m_edtMinPitch.m_hWnd, tmp, 10 ) ;
	g_theGameConfigs.SetValue( "Camera_MinPitch", tmp );
}

//void CDlg_Cfg_Video::OnEnUpdateEdit2()
//{
//	// TODO:  If this is a RICHEDIT control, the control will not
//	// send this notification unless you override the CDialog::OnInitDialog()
//	// function to send the EM_SETEVENTMASK message to the control
//	// with the ENM_UPDATE flag ORed into the lParam mask.
//
//	// TODO:  Add your control notification handler code here
//	char tmp[10] ;
//	memset( tmp, 0, 10 ) ;
//	::GetWindowTextA( m_edtMaxPitch.m_hWnd, tmp, 10 ) ;
//	int a = 0 ;
//}
