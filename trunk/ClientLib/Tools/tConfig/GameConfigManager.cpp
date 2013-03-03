#include "StdAfx.h"
#include ".\gameconfigmanager.h"
#include "Resource.h"
#include "tConfigDlg.h"

CGameConfigManager::CGameConfigManager(void) :
	m_pLastestConfig(0)
{
}

CGameConfigManager::~CGameConfigManager(void)
{
    GAMECONFIG_BUF::iterator configIt = m_allConfigFile.begin();

    while (configIt != m_allConfigFile.end())
    {
        delete configIt->second;

        ++configIt;
    }
    m_allConfigFile.clear();
}

//加载所有配置文件，返回配置文件数
int	CGameConfigManager::LoadAllConfigFile(void)
{
	//当前目录
	char szCurrentDirection[MAX_PATH] = {0};
	::GetCurrentDirectoryA(MAX_PATH, szCurrentDirection);

	//-----------------------------
	//System.cfg
	char szSystemCfg[MAX_PATH] = {0};
	strncpy(szSystemCfg, szCurrentDirection, MAX_PATH);
	PathAppendA(szSystemCfg, "System.cfg");
	_tryAddConfigFile(szSystemCfg);

	//-----------------------------
	//Account\\System.cfg
	char szUserCfg[MAX_PATH] = {0};
	strncpy(szUserCfg, szCurrentDirection, MAX_PATH);
	PathRemoveFileSpecA(szUserCfg);
	PathAppendA(szUserCfg, "Accounts\\System.cfg");
	_tryAddConfigFile(szUserCfg);

	//-----------------------------
	//搜索Accounts目录
	char szAccountsPath[MAX_PATH];
	strncpy(szAccountsPath, szCurrentDirection, MAX_PATH);
	::PathRemoveFileSpecA(szAccountsPath);
	::PathAppendA(szAccountsPath, "Accounts");

	char szTempFile[MAX_PATH];
	strncpy(szTempFile, szAccountsPath, MAX_PATH);
	PathAppendA(szTempFile, "*.*");

	WIN32_FIND_DATAA FindData;
	HANDLE hFind = FindFirstFileA(szTempFile, &FindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		return true;
	}

	BOOL bFind = FALSE;
	do
	{
		CHAR szPathFileName[MAX_PATH];
		strncpy(szPathFileName, szAccountsPath, MAX_PATH);
		PathAppendA(szPathFileName, FindData.cFileName);

		if(FindData.cFileName[0] != '.')
		{
			//文件夹
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				_tryAddConfigPath(szPathFileName);
			}
		}

		// Find Next file.
		bFind = FindNextFileA(hFind, &FindData);
	}while(bFind);

	return (int)m_allConfigFile.size();
}

void CGameConfigManager::DeletePrivateFile(void)
{
	//当前目录
	char szCurrentDirection[MAX_PATH] = {0};
	::GetCurrentDirectoryA(MAX_PATH, szCurrentDirection);

	char szAccountsPath[MAX_PATH];
	strncpy(szAccountsPath, szCurrentDirection, MAX_PATH);
	::PathRemoveFileSpecA(szAccountsPath);
	::PathAppendA(szAccountsPath, "Accounts");

	SHFILEOPSTRUCTA shf;
	memset(&shf,0,sizeof(SHFILEOPSTRUCT));
	shf.hwnd = NULL;
	shf.pFrom = szAccountsPath;
	shf.wFunc = FO_DELETE;
	shf.fFlags = FOF_NOCONFIRMMKDIR|FOF_NOCONFIRMATION|
					FOF_NOERRORUI|FOF_SILENT;
	SHFileOperationA(&shf);
}

void CGameConfigManager::_tryAddConfigPath(const char* szConfigPath)
{
	char szTempFile[MAX_PATH];
	strncpy(szTempFile, szConfigPath, MAX_PATH);
	PathAppendA(szTempFile, "*.pfc");

	WIN32_FIND_DATAA FindData;
	HANDLE hFind = FindFirstFileA(szTempFile, &FindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		return;
	}

	BOOL bFind = FALSE;
	do
	{
		CHAR szPathFileName[MAX_PATH];
		strncpy(szPathFileName, szConfigPath, MAX_PATH);
		PathAppendA(szPathFileName, FindData.cFileName);

		if(FindData.cFileName[0] != '.')
		{
			//文件夹
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			}
			else
			{
				_tryAddConfigFile(szPathFileName);
			}
		}

		// Find Next file.
		bFind = FindNextFileA(hFind, &FindData);
	}while(bFind);
}

void CGameConfigManager::_tryAddConfigFile(const char* szConfigFile)
{
	CGameConfig* pNewConfigFile = new CGameConfig(szConfigFile);
	if(!(pNewConfigFile->IsInvalid())) 
    {
        delete pNewConfigFile;
        return;
    }

	m_allConfigFile.insert(std::make_pair(pNewConfigFile->GetLastUpdate(), pNewConfigFile));
}

CGameConfig* CGameConfigManager::GetLastestConfig(void)
{
	if(m_allConfigFile.empty()) return 0;

	return m_allConfigFile.rbegin()->second;
}

void CGameConfigManager::SetValue(const char* szValueName, char* szValue)
{
	char szTemp[32];
	memset( szTemp, 0, 32 ) ;
	sprintf( szTemp, "%s", szValue )  ;
	//_snprintf(szTemp, 32, "%s", szValue);
	
	m_mapToSave[szValueName] = szTemp;

	((CtConfigDlg*)(AfxGetApp()->GetMainWnd()))->setModifyFlag(NeedUpdate());
}
void CGameConfigManager::SetValue(const char* szValueName, int nValue)
{
	char szTemp[32];
	_snprintf(szTemp, 32, "%d", nValue);
	
	m_mapToSave[szValueName] = szTemp;

	((CtConfigDlg*)(AfxGetApp()->GetMainWnd()))->setModifyFlag(NeedUpdate());
}

bool CGameConfigManager::NeedUpdate(void)
{
	CGameConfig* pLastestConfig = GetLastestConfig();

	CONFIG_BUF::iterator it;
	for(it=m_mapToSave.begin(); it!=m_mapToSave.end(); it++)
	{
		const char* oldValue = pLastestConfig->GetValue(it->first.c_str());

		if(oldValue != it->second)
		{
			return true;
		}
	}

	return false;
}

void CGameConfigManager::SaveModifed(void)
{
	GAMECONFIG_BUF::iterator it;
	for(it=m_allConfigFile.begin(); it!=m_allConfigFile.end(); it++)
	{
		it->second->SaveModifyed(m_mapToSave);
	}
}

bool CGameConfigManager::IsVariableModifyed(const char* szName)
{
	return m_mapToSave.find(szName) != m_mapToSave.end();
}

const char* CGameConfigManager::GetModifyedValue(const char* szName)
{
	if(!IsVariableModifyed(szName)) return 0;

	return m_mapToSave[szName].c_str();
}
