#include "StdAfx.h"
#include "AXPUtil.h"
#include "AXPErrors.h"

namespace AXP
{

//�淶�ļ���
std::string normaliseName(const std::string& fileName, bool toLowCase, bool norSlash)
{
	if(fileName.empty() || (!norSlash && !toLowCase)) return fileName;

	//����GBK������д�Сдת��
	const unsigned char byANSIBegin_A	= 'A';
	const unsigned char byANSIEnd_Z		= 'Z';

	const unsigned char by1GBKBegin		= 0X81;
	const unsigned char by1GBKEnd		= 0XFE;

	std::string result = fileName;

	int nSize = (int)result.size();
	for(register int i=0; i<nSize;)
	{
		unsigned char& byChar = (unsigned char&)result[i];

		//��дANSIӢ���ַ�A-Z,ת��ΪСд
		if(toLowCase && byChar >= byANSIBegin_A && byChar <= byANSIEnd_Z)
		{
			byChar += 'a'-'A';
			i++; 
			continue;
		}
		// '\' -> '/'
		else if(byChar == '\\' && norSlash)
		{
			byChar = '/';
			i++;
			continue;
		}
		//GBK����
		else if(byChar >= by1GBKBegin && byChar <= by1GBKEnd)
		{
			i+=2;
			continue;
		}
		else
		{
			i++;
		}
	}
	return result;
}

//��ȡ�ļ���С
unsigned int getDiskFileSize(const char* szFileName)
{
	assert(szFileName);
	if(!szFileName || szFileName[0] == 0) return 0;

	//Open file Handle
	HANDLE hFile = ::CreateFile(szFileName,
			GENERIC_READ,
			FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN,
			NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	//Get File Size
	unsigned int nFileSize = ::GetFileSize(hFile, 0);
	
	//Close handle
	::CloseHandle(hFile); hFile=0;

	return nFileSize;
}

//�ָ��ַ���
int convertStringToVector(const char* strResource, std::vector< std::string >& vRet, const char* szKey, bool bOneOfKey, bool bIgnoreEmpty)
{
	vRet.clear();
	
	if(0==strResource || strResource[0]==0)
	{	//�ַ���Ϊ��
		return 0;
	}

	std::string strSrc = strResource;
	std::string::size_type nLeft = 0;
	std::string::size_type nRight;
	if(bOneOfKey)
	{
		nRight = strSrc.find_first_of(szKey);
	}
	else
	{
		nRight = strSrc.find(szKey);
	}

	if(nRight == std::string::npos)
	{
		nRight = strSrc.length();
	}

	while(true)
	{
		std::string strItem = strSrc.substr(nLeft, nRight-nLeft);
		if(strItem.length() > 0 || !bIgnoreEmpty)
		{
			vRet.push_back(strItem);
		}

		if(nRight == strSrc.length())
		{
			break;
		}

		nLeft = nRight + (bOneOfKey ? 1 : strlen(szKey));
		
		if(bOneOfKey)
		{
			std::string strTemp = strSrc.substr(nLeft);
			nRight = strTemp.find_first_of(szKey);
			if(nRight != std::string::npos) nRight += nLeft;
		}
		else
		{
			nRight = strSrc.find(szKey, nLeft);
		}

		if(nRight == std::string::npos)
		{
			nRight = strSrc.length();
		}
	}

	return (int)vRet.size();
}

//���ļ�β������crcУ����
bool attachCRCFlag(const char* szFileName)
{
	assert(szFileName);
	if(!szFileName || szFileName[0] == 0) 
	{
		setLastError(AXP_ERR_PARAM);
		return 0;
	}

	//�����ļ�У����
	unsigned int nCRC;
	if(!crcFile32(szFileName, nCRC)) return false;

	//�򿪸��ļ�
	HANDLE hFile = ::CreateFile(szFileName, 
						GENERIC_READ|GENERIC_WRITE,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						0,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN,
						0);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "File=%s, WinErr=%d", szFileName, ::GetLastError());
		return false;
	}

	//move to end
	if(INVALID_SET_FILE_POINTER == ::SetFilePointer(hFile, 0, 0, FILE_END))
	{
		setLastError(AXP_ERR_FILE_POINT, "File=%s, WinErr=%d", szFileName, ::GetLastError());
		CloseHandle(hFile);
		return false;
	}

	//write crc
	DWORD dwBytesWrite;
	if(!WriteFile(hFile, &nCRC, sizeof(nCRC), &dwBytesWrite, 0) || 
		dwBytesWrite != sizeof(nCRC))
	{
		setLastError(AXP_ERR_FILE_WRITE, "winErr: %d", GetLastError());
		CloseHandle(hFile);
		return false;
	}

	CloseHandle(hFile); hFile=0;
	return true;
}

//����ļ�β����CRCУ����
bool checkCRCFlag(const char* szFileName)
{
	//�����ļ�crc
	unsigned int nCRC;
	if(!crcFile32(szFileName, nCRC, true)) return false;

	//���ļ�
	HANDLE hFile = ::CreateFile(szFileName, 
						GENERIC_READ,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						0,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN,
						0);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "File=%s, WinErr=%d", szFileName, ::GetLastError());
		return false;
	}

	//Get File Size
	unsigned int nFileSize = ::GetFileSize(hFile, 0);

	//size must >= 4
	if(nFileSize < 4)
	{
		CloseHandle(hFile);
		return false;
	}

	//move to end
	if(INVALID_SET_FILE_POINTER == ::SetFilePointer(hFile, -4, 0, FILE_END))
	{
		setLastError(AXP_ERR_FILE_POINT, "File=%s, WinErr=%d", szFileName, ::GetLastError());
		CloseHandle(hFile);
		return false;
	}

	unsigned int nCRCRead;

	//Read Head
	DWORD dwReadBytes;
	if(!ReadFile(hFile, &nCRCRead, sizeof(nCRCRead), &dwReadBytes, 0) ||
		dwReadBytes != sizeof(nCRCRead))
	{
		setLastError(AXP_ERR_FILE_READ, "WinErr=%d", ::GetLastError());
		CloseHandle(hFile);
		return false;
	}
	CloseHandle(hFile); hFile=0;

	return nCRCRead == nCRC;
}

//�ݹ鴴��Ŀ¼
bool forceCreatePath(const char* szPath)
{
	//Ŀ¼�Ѿ�����
	if(::PathFileExists(szPath)) return true;
	//�ܹ�ֱ�Ӵ���
	if(::CreateDirectory(szPath, 0)) return true;

	//ȡ����һ��Ŀ¼
	char szParentPath[MAX_PATH] = {0};
	strncpy(szParentPath, szPath, MAX_PATH);
	if(!::PathRemoveFileSpec(szParentPath)) return false;

	//������һ��Ŀ¼
	if(!forceCreatePath(szParentPath)) return false;

	return TRUE==::CreateDirectory(szPath, 0);
}

//�Ƚ������ļ����Ƿ���ͬһ���ļ�
int compareTwoFileName(const char* szFileName1, const char* szFileName2)
{
	char szTempFileName1[MAX_PATH] = {0};
	char szTempFileName2[MAX_PATH] = {0};

	_snprintf(szTempFileName1, MAX_PATH, "%s", szFileName1);
	_snprintf(szTempFileName2, MAX_PATH, "%s", szFileName2);

	PathRemoveBlanks(szTempFileName1);
	PathRemoveBlanks(szTempFileName2);

	PathRemoveBackslash(szTempFileName1);
	PathRemoveBackslash(szTempFileName2);

	int nRet = _stricmp(szTempFileName1, szTempFileName2);
	if(nRet == 0) return 0;

	char szShortFileName1[MAX_PATH] = {0};
	char szShortFileName2[MAX_PATH] = {0};

	if(GetShortPathName(szTempFileName1, szShortFileName1, MAX_PATH))
	{
		strcpy(szTempFileName1, szShortFileName1);
	}

	if(GetShortPathName(szTempFileName2, szShortFileName2, MAX_PATH))
	{
		strcpy(szTempFileName2, szShortFileName2);
	}

	return _stricmp(szTempFileName1, szTempFileName2);
}

//�õ�����������
unsigned char getZipEncryptXORKey(unsigned int pos)
{
	extern DWORD g_arrdwCrc32Table[256];
	const unsigned int KEY = 0x0056B12F;
	const unsigned int TABLE_LEN = 0x100*4;

	return ((const unsigned char*)g_arrdwCrc32Table)[(BYTE)((pos*13+KEY)%TABLE_LEN)] ^ 0xCE;
}

//��ָ���ļ�����
bool encryptFile(const char* szFileName, bool ignoreTail)
{
	//���ļ�
	HANDLE hInFile = ::CreateFile(szFileName, 
						GENERIC_READ,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						0,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN,
						0);

	if(hInFile == INVALID_HANDLE_VALUE)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "File=%s, WinErr=%d", szFileName, ::GetLastError());
		return false;
	}

	//��������ļ�
	char szOutFileName[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szOutFileName);
	::PathAppend(szOutFileName, ::PathFindFileName(szFileName));
	strcat(szOutFileName, ".tmp");

	HANDLE hOutFile = ::CreateFile(szOutFileName, 
						GENERIC_READ|GENERIC_WRITE,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						0,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_ARCHIVE,
						0);
	if(hOutFile == INVALID_HANDLE_VALUE)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "File=%s, WinErr=%d", szOutFileName, ::GetLastError());
		::CloseHandle(hInFile);
		return false;
	}

	//Get File Size
	unsigned int nFileSize = ::GetFileSize(hInFile, 0);
	if(ignoreTail)
	{
		if(nFileSize <= 4)
		{
			setLastError(AXP_ERR_PARAM, "File=%s(too small)", szFileName);
			::CloseHandle(hInFile);
			return false;
		}

		nFileSize-=4;
	}

	const int MAX_BUFFER_SIZE = 1024;
	BYTE buffer[MAX_BUFFER_SIZE];

	DWORD dwBytesRead;
	BOOL bSuccess = ReadFile(hInFile, buffer, sizeof(buffer), &dwBytesRead, NULL);
	DWORD dwBytesReadTotal = 0;

	while(bSuccess && dwBytesRead)
	{
		//����
		for(DWORD i=0; i<dwBytesRead; i++)
		{
			buffer[i] ^= getZipEncryptXORKey(i+dwBytesReadTotal);
		}

		dwBytesReadTotal+=dwBytesRead;

		if(dwBytesReadTotal > nFileSize)
		{
			dwBytesRead -= (dwBytesReadTotal-nFileSize);
		}

		if(dwBytesRead == 0) break;

		DWORD dwWriteBytes;
		bSuccess = ::WriteFile(hOutFile, buffer, dwBytesRead, &dwWriteBytes, 0);
		if(!bSuccess)
		{
			setLastError(AXP_ERR_FILE_WRITE, "WinErr=%d", ::GetLastError());
			::CloseHandle(hOutFile);
			::CloseHandle(hInFile);
			DeleteFile(szOutFileName);
			return false;
		}

		bSuccess = ReadFile(hInFile, buffer, sizeof(buffer), &dwBytesRead, NULL);
	};

	if(!bSuccess)
	{
		setLastError(AXP_ERR_FILE_WRITE, "WinErr=%d", ::GetLastError());
		::CloseHandle(hOutFile);
		::CloseHandle(hInFile);
		DeleteFile(szOutFileName);
		return false;
	}

	::CloseHandle(hOutFile);
	::CloseHandle(hInFile);

	if(!::MoveFileEx(szOutFileName, szFileName, MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING))
	{
		setLastError(AXP_ERR_FILE_WRITE, "WinErr=%d", ::GetLastError());
		return false;
	}

	return true;
}

//ĳ�ļ��Ƿ��Ǽ��ܺ�zip�ļ�
bool isEncryptedZipFile(const char* szFileName)
{
	extern DWORD g_arrdwCrc32Table[256];

	//���ļ�
	HANDLE hFile = ::CreateFile(szFileName, 
						GENERIC_READ,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						0,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN,
						0);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "File=%s, WinErr=%d", szFileName, ::GetLastError());
		return false;
	}

	//Get File Size
	unsigned int nFileSize = ::GetFileSize(hFile, 0);
	//�ļ�̫С
	if(nFileSize <= 2 )
	{
		::CloseHandle(hFile);
		return false;
	}

	unsigned char temp[8];
	DWORD bytesReaded;
	::ReadFile(hFile, temp, 2, &bytesReaded, 0);
	::CloseHandle(hFile);

//	BYTE _1 = getZipEncryptXORKey(0);
//	BYTE _2 = getZipEncryptXORKey(1);

//	temp[0] ^ getZipEncryptXORKey(0)

	if(	'P' == (temp[0] ^ getZipEncryptXORKey(0))&&
		'K' == (temp[1] ^ getZipEncryptXORKey(1)) )
	{
		return true;
	}

	return false;
}

}