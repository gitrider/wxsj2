#include "StdAfx.h"
#include "AXPPackMaker.h"
#include "AXPErrors.h"
#include "AXPUtil.h"
#include "AXPPackFile.h"
#include "AXPUtil.h"

namespace AXP
{
//����/����
PakMaker::PakMaker()
{
}

PakMaker::~PakMaker()
{
}

//��������һ��Ŀ¼���뵽����
bool PakMaker::addDiskFold(const char* szFoldInDisk, const char* szFoldInPak, const char* szExtFilter, bool bRecursive)
{
	assert(szFoldInDisk );
	if(!szFoldInDisk || szFoldInDisk[0]==0 ) 
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//check is path exist
	if(!PathFileExists(szFoldInDisk))
	{
		setLastError(AXP_ERR_FILE_NOTFOUND, "%s", szFoldInDisk);
		return false;
	}

	//�ָ��׺������
	std::vector< std::string > vExtFilter;
	convertStringToVector(szExtFilter, vExtFilter, ";", true, true);

	std::set< std::string > sExtFilter;
	for(int i=0; i<(int)vExtFilter.size(); i++)
	{
		sExtFilter.insert(normaliseName(vExtFilter[i], true, false));
	}

	//�����ڲ��ݹ麯��
	return _addDiskFold(szFoldInDisk, sExtFilter, szFoldInPak, bRecursive);
}

//�ݹ����һ���ļ�, szFoldInDisk=·���� sFilter=������, strFoldInPak=Pak�е�·�� 
bool PakMaker::_addDiskFold(const std::string& strFoldInDisk, 
							const std::set< std::string >& sFilter, 
							const std::string& strFoldInPak,
							bool bRecursive)
{
	WIN32_FIND_DATA theFindData;

	// TO Search...
	char szFileToSearch[MAX_PATH] = {0};
	strncpy(szFileToSearch, strFoldInDisk.c_str(), MAX_PATH);
	PathAppend(szFileToSearch, "*.*");

	// find first file
	HANDLE hFind = FindFirstFile(szFileToSearch, &theFindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		//empty !
		FindClose(hFind);
		return true;
	}

	BOOL bFind = FALSE;
	do
	{
		if(theFindData.cFileName[0] != '.')
		{
			//File in Disk
			CHAR szPathFileName[MAX_PATH];
			strncpy(szPathFileName, strFoldInDisk.c_str(), MAX_PATH);
			PathAppend(szPathFileName, theFindData.cFileName);

			//File in Pack
			char szFileInPak[MAX_PATH] = {0};
			strncpy(szFileInPak, strFoldInPak.c_str(), MAX_PATH);
			PathAppend(szFileInPak, theFindData.cFileName);

			//�ļ���
			if(theFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(!bRecursive) goto FIND_NEXT;

				if(!_addDiskFold(szPathFileName, sFilter, szFileInPak, true)) return false;
			}
			//�ļ�
			else
			{
				//�Ƿ��ǺϷ���׺���ļ�
				if(!sFilter.empty())
				{
					const char* pExt = ::PathFindExtension(szPathFileName);
					if(!pExt) goto FIND_NEXT;

					if(sFilter.find(normaliseName(pExt+1, true, false)) == sFilter.end())
						goto FIND_NEXT;
				}

				if(!addDiskFile(szPathFileName, szFileInPak)) return false;
			}
		}

FIND_NEXT:
		// Find Next file.
		bFind = FindNextFile(hFind, &theFindData);
	}while(bFind);

	FindClose(hFind);
	return true;
}

//��������ϵ����ļ�������
bool PakMaker::addDiskFile(const char* szFileInDisk, const char* szFileInPak)
{
	assert(szFileInDisk && szFileInPak);

	if(!szFileInDisk || !szFileInPak || szFileInDisk[0]==0 || szFileInPak[0]==0) 
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}
	//check is file exist
	if(!PathFileExists(szFileInDisk)) 
	{
		setLastError(AXP_ERR_FILE_NOTFOUND);
		return false;
	}
	
	FileNode newNode;
	newNode.bExcute = false;
	newNode.strFileInPak = normaliseName(szFileInPak, false, true);
	newNode.strFileInDisk = normaliseName(szFileInDisk, false, true);
	newNode.nCRC = 0;
	newNode.nFileSize = 0;

	m_AllFiles[normaliseName(szFileInPak)] = newNode;

	return true;
}

//��������ļ������pak�ļ�,
bool PakMaker::savePakFile(const char* szPakFileName, AXP_PAKMAKER_SAVECALLBACK callbackFunc)
{
	assert(szPakFileName);
	if(!szPakFileName || szPakFileName[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//Pak�ļ�
	PackFile pakFile;
	if(!pakFile.createNewPakFile(szPakFileName)) return false;

	//�����ļ���
	int nActFileCount = 0;
	//�����ļ�crc
	unsigned int nTotalCRC = 0xFFFFFFFF;

	//�����ļ�
	FileNodeBuf::iterator it;
	for(it=m_AllFiles.begin(); it!=m_AllFiles.end(); it++)
	{
		FileNode& file = it->second;
		
		char szTempFileName[1024] = {0};

		if(callbackFunc)
		{
			//����Callback
			if(!callbackFunc(file.strFileInDisk.c_str(), file.strFileInPak.c_str(), szTempFileName, 1024)) 
			{
				//���б���ɾ��
				file.bExcute = true;
				continue;
			}
		}
		else
		{
			strncpy(szTempFileName, file.strFileInDisk.c_str(), 1024);
		}

		file.strFileInDisk = szTempFileName;
		if(!crcFile32(szTempFileName, file.nCRC)) return false;
		file.nFileSize = getDiskFileSize(szTempFileName);

		if(!pakFile.insertContents(szTempFileName, 0, file.strFileInPak.c_str(), 
						AC_DISKFILE, false))
		{
			return false;
		}

		nActFileCount++;
		nTotalCRC ^= file.nCRC;
	}

	//����(list)�ļ�
	std::string strListFile;
	if(!_genrateListFile(strListFile, nActFileCount, nTotalCRC)) return false;

	//����(list)�ļ�
	if(!pakFile.insertContents(strListFile.c_str(), 0, PackFile::LIST_FILENAME,
					AC_DISKFILE, false))
	{
		return false;
	}

	//ɾ����ʱ��temp�ļ�
	::DeleteFile(strListFile.c_str());

	pakFile.closePakFile();

	return true;
}

bool PakMaker::_genrateListFile(std::string& strTempListFile, int nActFileCount, unsigned int nTotalCRC)
{
	char szTempPath[MAX_PATH] = {0};
	char szTempFile[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szTempPath);
	::GetTempFileName(szTempPath, "~axp", MAX_PATH, szTempFile);

	//�����ļ�
	HANDLE hListFile = ::CreateFile(szTempFile, 
						GENERIC_READ|GENERIC_WRITE,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						0,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_ARCHIVE,
						0);
	
	if(hListFile == INVALID_HANDLE_VALUE)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "WinErr=%d", ::GetLastError());
		return false;
	}

	char szTemp[MAX_PATH*4] = {0};
	//���� "ʱ���\n�ļ�����"
	SYSTEMTIME timeNow;
	::GetLocalTime(&timeNow);
	int nSize = _snprintf(szTemp, MAX_PATH*4, "%08X\n%d\n", 
					nTotalCRC, nActFileCount);
	
	DWORD dwBytesWrite;
	if(!::WriteFile(hListFile, szTemp, nSize, &dwBytesWrite, 0) ||
		dwBytesWrite != nSize)
	{
		setLastError(AXP_ERR_FILE_WRITE, "WinErr=%d", ::GetLastError());
		CloseHandle(hListFile);
		return false;
	}

	//�����ļ���
	FileNodeBuf::iterator it;
	for(it=m_AllFiles.begin(); it!=m_AllFiles.end(); it++)
	{
		FileNode& file = it->second;
		if(file.bExcute) continue;

		nSize = _snprintf(szTemp, MAX_PATH*4, "%s|%08X|%08X\n", 
			file.strFileInPak.c_str(), file.nFileSize, file.nCRC);

		DWORD dwBytesWrite;
		if(!::WriteFile(hListFile, szTemp, nSize, &dwBytesWrite, 0) ||
			dwBytesWrite != nSize)
		{
			setLastError(AXP_ERR_FILE_WRITE, "WinErr=%d", ::GetLastError());
			CloseHandle(hListFile);
			return false;
		}
	}

	CloseHandle(hListFile); hListFile = 0;

	strTempListFile = szTempFile;
	return true;
}

//��������ļ������е�pak�ļ������ݱȽϣ����ڳ������
bool PakMaker::comparePakFile(const char* szPakFileName, AXP_PAKMAKER_COMPARECALLBACK callbackFunc)
{
	assert(szPakFileName && callbackFunc);
	if(!szPakFileName || szPakFileName[0]==0 || callbackFunc==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//Pak�ļ�
	PackFile pakFile;
	if(!pakFile.openPakFile(szPakFileName, true)) return false;

	//�����Ѿ������ļ�
	typedef std::set< std::string > FILENAME_SET;
	FILENAME_SET setFileProcessed;

	//�����ļ�
	FileNodeBuf::iterator it;
	for(it=m_AllFiles.begin(); it!=m_AllFiles.end(); it++)
	{
		FileNode& file = it->second;
		char szTempFileName[1024] = {0};

		//���ļ�
		IStream* pStream = pakFile.openFile(file.strFileInPak.c_str());

		//����Callback
		if(!callbackFunc(file.strFileInDisk.c_str(), pStream))
		{
			setLastError(AXP_ERR_FILE_DIFF, "%s", file.strFileInPak.c_str());
			if(pStream) pStream->close();
			return false;
		}

		if(pStream)
		{
			setFileProcessed.insert(normaliseName(file.strFileInPak));
			//�ر�stream
			pStream->close();
		}
	}

	//�Ƿ�����δ�����ļ�����

	//��List�ļ�
	IStream* pListStream = pakFile.openFile(PackFile::LIST_FILENAME);
	if(!pListStream) return false;

	//skip first line
	pListStream->skipLine();
	
	//read second line
	char szTempLine[MAX_PATH*4] = {0};
	int nLineSize = pListStream->readLine(szTempLine, MAX_PATH*4);
	int nFileCount = atoi(szTempLine);

	while (!pListStream->eof())
	{
		int nLineLength = pListStream->readLine(szTempLine, MAX_PATH*4);
		if(0 == nLineLength) 
		{
			pListStream->close();
			setLastError(AXP_ERR_FILE_FORMAT, "list file second line=0");
			return false;
		}

		//�����ļ�����
		std::vector< std::string > vStringVec;
		convertStringToVector(szTempLine, vStringVec, "|", true, false);
		if(vStringVec.size() != 3) 
		{
			pListStream->close();
			setLastError(AXP_ERR_FILE_FORMAT, "list file=%s", szTempLine);
			return false;
		}

		//����ļ���Ϣ
		if(setFileProcessed.find(normaliseName(vStringVec[0])) == setFileProcessed.end())
		{
			pListStream->close();
			setLastError(AXP_ERR_FILE_DIFF, "redufile=%s", vStringVec[0].c_str());
			return false;
		}
	}

	pListStream->close();
	return true;
}

/************************************
			API�ӿ�
*************************************/

//����һ��Maker�ӿ�
IPakMaker* createPakMaker(void)
{
	return (IPakMaker*)(new PakMaker);
}
//ɾ��һ��Maker�ӿ�
void destroyPakMaker(IPakMaker* pMaker)
{
	if(!pMaker) return;

	delete ((PakMaker*)pMaker);
}

}