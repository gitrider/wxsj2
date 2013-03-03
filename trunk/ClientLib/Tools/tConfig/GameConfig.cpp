#include "StdAfx.h"
#include ".\gameconfig.h"

INT ConvertSectionInVector(LPCSTR szSectionBuf, INT nLen, std::vector< std::string >& vSectionBuf);

CGameConfig::CGameConfig(const char* szFileName) :
	m_bInvalid(false)
{
	m_bInvalid = LoadFromFile(szFileName);
}

CGameConfig::~CGameConfig(void)
{
}

bool CGameConfig::LoadFromFile(const char* szFileName)
{
	assert(szFileName);

	if(!szFileName || szFileName[0]==0) return false;

	//无法打开
	if(_access(szFileName, 4) != 0) return false;

	//-----------------------------------------------------------
	//取得配置文件的大小
	HANDLE hFile = CreateFileA(szFileName,
							GENERIC_READ,
							FILE_SHARE_READ | FILE_SHARE_WRITE,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	
	if(INVALID_HANDLE_VALUE == hFile) return false;

	DWORD dwHigh;
	DWORD dwFileSize = GetFileSize(hFile, &dwHigh);

	//GetFileTime
	FILETIME timeCreation, timeLastAccess, timeLastWrite;
	GetFileTime(hFile, &timeCreation, &timeLastAccess, &timeLastWrite);
	ULARGE_INTEGER tLastUpdate;
	memcpy(&tLastUpdate, &timeLastWrite, sizeof(ULARGE_INTEGER));
	m_tLastUpdate = tLastUpdate.QuadPart;

	//Close
	CloseHandle(hFile); hFile = NULL;
	if(0==dwFileSize) return false;

	//-----------------------------------------------------------
	//分配足够的内存
	CHAR* pTempBuf = new CHAR[dwFileSize+32];
	if(!pTempBuf) return false;

	//-----------------------------------------------------------
	//从配置文件中读取"Variable"节
	::GetPrivateProfileSectionA("Variable", pTempBuf, dwFileSize, szFileName);
	//分解
	std::vector< std::string > vRet;
	ConvertSectionInVector(pTempBuf, dwFileSize, vRet);

	delete[] pTempBuf; pTempBuf=NULL;

	//加入变量定义
	for(INT i=0; i<(INT)vRet.size(); i++)
	{
		std::string& strLine = vRet[i];

		std::string::size_type tEqu = strLine.find_first_of("= \t");
		if(tEqu == std::string::npos) continue;

		std::string strName = strLine.substr(0, tEqu);

		CHAR szTemp[1024];
		::GetPrivateProfileStringA("Variable", strName.c_str(), "", szTemp, 1024, szFileName);

		m_theConfigBuf.insert(std::make_pair(strName.c_str(), szTemp));
	}

	m_strFileName = szFileName;
	return true;
}

const char* CGameConfig::GetValue(const char* szValueName) const
{
	CONFIG_BUF::const_iterator itFind = m_theConfigBuf.find(szValueName);
	if( itFind == m_theConfigBuf.end())
	{
		return "";
	}

	return itFind->second.c_str();
}

void CGameConfig::SetValue(const char* szValueName, int nValue)
{
    CONFIG_BUF::iterator itFind = m_theConfigBuf.find(szValueName);
    if( itFind != m_theConfigBuf.end())
    {
        char szTemp[32];
        _snprintf(szTemp, 32, "%d", nValue);

        itFind->second = szTemp;
    }
}

void CGameConfig::SaveModifyed(const CONFIG_BUF& toSave)
{
	CONFIG_BUF::const_iterator it;
	for(it=toSave.begin(); it!=toSave.end(); it++)
	{
		if(GetValue(it->first.c_str()) != it->second)
		{
			::WritePrivateProfileStringA("Variable", it->first.c_str(), it->second.c_str(), m_strFileName.c_str());
		}
	}
}

INT ConvertSectionInVector(LPCSTR szSectionBuf, INT nLen, std::vector< std::string >& vSectionBuf)
{
	register const char *pDot = szSectionBuf;

	vSectionBuf.clear();

	while(TRUE)
	{
		if( (*pDot == '\0' && *(pDot-1) == '\0') || (pDot-szSectionBuf >= nLen) )
		{
			break;
		}

		if(*pDot == '\0')
		{
			pDot++;
			continue;
		}

		std::string strTheSingle = pDot;
		vSectionBuf.push_back(strTheSingle);

		pDot += strlen(pDot)+1;
	}

	return (INT)vSectionBuf.size();
}

