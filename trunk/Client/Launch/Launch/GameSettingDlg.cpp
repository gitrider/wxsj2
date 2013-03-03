// GameSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Launch.h"
#include "GameSettingDlg.h"
#include "LaunchSystem.h"
#include "LaunchUtil.h"
#include ".\gamesettingdlg.h"

// CGameSettingDlg dialog

IMPLEMENT_DYNAMIC(CGameSettingDlg, CDialog)
CGameSettingDlg::CGameSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameSettingDlg::IDD, pParent)
{
}

CGameSettingDlg::~CGameSettingDlg()
{
}

void CGameSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGameSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_LOGIN, OnBnClickedRadioLogin)
	ON_BN_CLICKED(IDC_RADIO_GAMESERVER, OnBnClickedRadioGameserver)
END_MESSAGE_MAP()


// CGameSettingDlg message handlers

BOOL CGameSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//加载设置
	loadGameSetting();

	//初始化界面


	//------------------------
	//从注册表中读取缺省用户名设置
	{
		DWORD dwType, dwSize=MAX_PATH;
		CHAR szUserName[MAX_PATH];
		if(ERROR_SUCCESS != ::SHGetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\KYLIN", "UserID", &dwType, szUserName, &dwSize))
		{
			szUserName[0] = 0;
		}

		SetDlgItemText(IDC_EDIT_USERNAME, szUserName);
	}

	//-------------------
	//服务器列表
	{
		const char* s_szServerList[] = 
		{
			"127.0.0.1 - 本机",
			"192.168.1.59 - 程序测试",
			"192.168.1.133 - Login",
		};
		INT s_ServerNum = sizeof(s_szServerList)/sizeof(const char*);

		//Add 
		for(INT i=0; i<s_ServerNum; i++)
		{
			SendDlgItemMessage(IDC_COMBO_GAMESERVER, CB_ADDSTRING, 0, (LPARAM)s_szServerList[i]);
		}

		//Sel
		std::string  strServer = getVariable("GameServer_Address", "");

		BOOL bSel = FALSE;
		for(INT i=0; i<s_ServerNum; i++)
		{
			if(strncmp(strServer.c_str(), s_szServerList[i], 11) == 0)
			{
				bSel = true;
				SendDlgItemMessage(IDC_COMBO_GAMESERVER, CB_SETCURSEL, i, 0);
				break;
			}
		}
		if(!bSel)
		{
			SendDlgItemMessage(IDC_COMBO_GAMESERVER, CB_ADDSTRING, 0, (LPARAM)strServer.c_str());
			SendDlgItemMessage(IDC_COMBO_GAMESERVER, CB_SETCURSEL, s_ServerNum, 0);
		}
	}	

	//----------------------
	//新帐号性别
	{
		SendDlgItemMessage(IDC_COMBO_GENDER, CB_ADDSTRING, 0, (LPARAM)"女");
		SendDlgItemMessage(IDC_COMBO_GENDER, CB_ADDSTRING, 1, (LPARAM)"男");
		SendDlgItemMessage(IDC_COMBO_GENDER, CB_SETCURSEL, 0, 0);
	}

	//----------------------
	//登录方式
	{
		std::string  strLoginDir = getVariable("GameServer_ConnectDirect", "1");
		if(strLoginDir == "1")
		{
			CheckRadioButton(IDC_RADIO_LOGIN, IDC_RADIO_GAMESERVER, IDC_RADIO_GAMESERVER);
		}
		else
		{
			CheckRadioButton(IDC_RADIO_LOGIN, IDC_RADIO_GAMESERVER, IDC_RADIO_LOGIN);
		}

		//刷新
		onLoginTypeChanged();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CGameSettingDlg::onLoginTypeChanged(void)
{
	UINT idChecked = GetCheckedRadioButton(IDC_RADIO_LOGIN, IDC_RADIO_GAMESERVER);

	bool bLoginDir = (IDC_RADIO_GAMESERVER == idChecked);

	GetDlgItem(IDC_STATIC_GAMESERVER)->EnableWindow(bLoginDir);
	GetDlgItem(IDC_COMBO_GAMESERVER)->EnableWindow(bLoginDir);
	GetDlgItem(IDC_STATIC_USERNAME)->EnableWindow(bLoginDir);
	GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(bLoginDir);
	GetDlgItem(IDC_STATIC_USERGENDER)->EnableWindow(bLoginDir);
	GetDlgItem(IDC_COMBO_GENDER)->EnableWindow(bLoginDir);
}

void CGameSettingDlg::loadGameSetting(void)
{
	std::string strGameCfg = LaunchSystem::GetMe()->getGameConfig();

	m_vGameVariable.clear();

	//-----------------------------------------------------------
	//取得配置文件的大小
	HANDLE hFile = CreateFile(strGameCfg.c_str(),
							GENERIC_READ,
							FILE_SHARE_READ | FILE_SHARE_WRITE,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	
	if(INVALID_HANDLE_VALUE == hFile) 
		return;

	DWORD dwHigh;
	DWORD dwFileSize = GetFileSize(hFile, &dwHigh);
	CloseHandle(hFile); hFile = NULL;
	if(0==dwFileSize) 
		return;

	//-----------------------------------------------------------
	//分配足够的内存
	CHAR* pTempBuf = new CHAR[dwFileSize+32];
	if(!pTempBuf) 
		return;

	//-----------------------------------------------------------
	//从配置文件中读取"Variable"节
	::GetPrivateProfileSection("Variable", pTempBuf, dwFileSize, strGameCfg.c_str());
	//分解
	std::vector< std::string > vRet;
	ConvertSectionInVector(pTempBuf, dwFileSize, vRet);

	delete[] pTempBuf; pTempBuf=NULL;

	//加入变量定义
	for(INT i=0; i<(INT)vRet.size(); i++)
	{
		std::string& strLine = vRet[i];

		std::string::size_type tEqu = strLine.find_first_of("= \t");
		if(tEqu == std::string::npos) 
			continue;

		std::string strName = strLine.substr(0, tEqu);

		CHAR szTemp[1024];
		::GetPrivateProfileString("Variable", strName.c_str(), "", szTemp, 1024, strGameCfg.c_str());

		m_vGameVariable.insert(std::make_pair(strName.c_str(), szTemp));
	}
}

const std::string& CGameSettingDlg::getVariable(const char* szVarName, const char* szDefValue)
{
	VariableMap::iterator itFind = m_vGameVariable.find(szVarName);
	if(itFind == m_vGameVariable.end())
	{
		m_vGameVariable[std::string(szVarName)] = std::string(szDefValue);
		return m_vGameVariable[std::string(szVarName)];
	}

	return itFind->second;
}


void CGameSettingDlg::OnBnClickedRadioLogin()
{
	// TODO: Add your control notification handler code here
	onLoginTypeChanged();
}

void CGameSettingDlg::OnBnClickedRadioGameserver()
{
	// TODO: Add your control notification handler code here
	onLoginTypeChanged();
}

void CGameSettingDlg::OnOK()
{
	CHAR szTemp[MAX_PATH];
	//-------------------------------------------------------------
	//取得新ID
	{
		//GetDlgItemText(IDC_EDIT_USERNAME, szTemp, MAX_PATH);
		//UINT dwUserID = String2CRC(szTemp);

		//m_vGameVariable["User_NAME"] = std::string(szTemp);

		////保存ID到注册表
		//::SHSetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\SOHU\\FireFox", "UserID", REG_SZ, szTemp, (UINT)strlen(szTemp));

		//_snprintf(szTemp, MAX_PATH, "%u", dwUserID);
		//m_vGameVariable["User_GUID"] = std::string(szTemp);
	}

	//-------------------------------------------------------------
	//新帐号性别
	{
		INT nNewPlayer_Gender = (INT)SendDlgItemMessage(IDC_COMBO_GENDER, CB_GETCURSEL, 0, 0); 
		_snprintf(szTemp, MAX_PATH, "%u", nNewPlayer_Gender);
		m_vGameVariable["User_GENDER"] = std::string(szTemp);
	}

	//-------------------------------------------------------------
	//是否直接连接服务器
	{
		bool bLoginDir = (IDC_RADIO_GAMESERVER == GetCheckedRadioButton(IDC_RADIO_LOGIN, IDC_RADIO_GAMESERVER));

		m_vGameVariable["GameServer_ConnectDirect"] = bLoginDir ? "1" : "0";
	}

	//-------------------------------------------------------------
	//服务器
	{
		GetDlgItemText(IDC_COMBO_GAMESERVER, szTemp, MAX_PATH);
		CHAR* pDot=strchr(szTemp, ' ');
		if(pDot) *pDot='\0';
		m_vGameVariable["GameServer_Address"] = szTemp;
	}

	//保存变量
	saveGameSetting();

	CDialog::OnOK();
}

void CGameSettingDlg::saveGameSetting(void)
{
	std::string strGameCfg = LaunchSystem::GetMe()->getGameConfig();

	//如果为只读文件，设置属性(处理受SourceSafe)管理的文件
	UINT dwFileAttr = GetFileAttributes(strGameCfg.c_str());
	if(FILE_ATTRIBUTE_READONLY & dwFileAttr)
	{
		SetFileAttributes(strGameCfg.c_str(), dwFileAttr&(~FILE_ATTRIBUTE_READONLY));
	}

	VariableMap::iterator it;
	for(it = m_vGameVariable.begin(); it!=m_vGameVariable.end(); it++)
	{
		::WritePrivateProfileString("Variable", it->first.c_str(), it->second.c_str(), strGameCfg.c_str());
	}

	//-------------------------------------------------------------
	//恢复文件属性
	if(FILE_ATTRIBUTE_READONLY & dwFileAttr)
	{
		SetFileAttributes(strGameCfg.c_str(), dwFileAttr);
	}
}

