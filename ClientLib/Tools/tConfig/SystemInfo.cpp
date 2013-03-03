#include "StdAfx.h"
#include "SystemInfo.h"
#include "TDSystemInfo.h"
#include "Util.h"

CSystemInfo::CSystemInfo()
{
	GetDirectXVersion();
}

CSystemInfo::~CSystemInfo()
{
}

const char* CSystemInfo::GetDirectXVersion(void)
{
	if(m_strDirectXVersion.empty())
	{
		DWORD dwType, dwSize=MAX_PATH;
		CHAR szDxVersion[MAX_PATH] = {0};
		if(ERROR_SUCCESS != ::SHGetValue(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\DirectX", "Version", &dwType, szDxVersion, &dwSize))
		{
			szDxVersion[0] = 0;
		}
		
		ConvertVer(std::string(szDxVersion), m_strDirectXVersion);
	}

	return m_strDirectXVersion.c_str();
}

const char* CSystemInfo::GetSystemInfo(void)
{
	if(m_strSystemInfo.empty())
	{
		char szTemp1[MAX_PATH] = {0};
		char szTemp2[MAX_PATH] = {0};
		
		TDU_OSInfo(szTemp1); 
		m_strSystemInfo += "系统： ";
		m_strSystemInfo += szTemp1;
		m_strSystemInfo += "\n";

		szTemp1[0] = 0;
		TDU_CPUInfo(szTemp1);  
		m_strSystemInfo += " CPU： ";
		m_strSystemInfo += szTemp1;
		m_strSystemInfo += "\n";

		szTemp1[0] = 0;
		TDU_MemoryInfo(szTemp1); 
		m_strSystemInfo += "内存： ";
		m_strSystemInfo += szTemp1;
		m_strSystemInfo += "\n";

		szTemp1[0] = 0;
		TDU_AdapterInfo(szTemp1, szTemp2);
		m_strSystemInfo += "显卡： ";
		m_strSystemInfo += szTemp1;
		m_strSystemInfo += "\n";
		m_strSystemInfo += "驱动： ";
		m_strSystemInfo += szTemp2;
		m_strSystemInfo += "\n";
	}

	return m_strSystemInfo.c_str();
}
