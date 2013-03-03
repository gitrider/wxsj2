#include "StdAfx.h"
#include "AXPPackMaker.h"
#include "AXPErrors.h"
#include "AXPUtil.h"
#include "AXPPackFile.h"
#include "AXPUtil.h"

namespace AXP
{
//构造/析构
PakMaker::PakMaker()
{
}

PakMaker::~PakMaker()
{
}

//将磁盘上一个目录加入到包中
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

	//分割后缀过滤器
	std::vector< std::string > vExtFilter;
	convertStringToVector(szExtFilter, vExtFilter, ";", true, true);

	std::set< std::string > sExtFilter;
	for(int i=0; i<(int)vExtFilter.size(); i++)
	{
		sExtFilter.insert(normaliseName(vExtFilter[i], true, false));
	}

	//调用内部递归函数
	return _addDiskFold(szFoldInDisk, sExtFilter, szFoldInPak, bRecursive);
}

//递归加入一批文件, szFoldInDisk=路径名 sFilter=过滤器, strFoldInPak=Pak中的路径 
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

			//文件夹
			if(theFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(!bRecursive) goto FIND_NEXT;

				if(!_addDiskFold(szPathFileName, sFilter, szFileInPak, true)) return false;
			}
			//文件
			else
			{
				//是否是合法后缀的文件
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

//加入磁盘上单个文件到包中
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

//将加入的文件保存成pak文件,
bool PakMaker::savePakFile(const char* szPakFileName, AXP_PAKMAKER_SAVECALLBACK callbackFunc)
{
	assert(szPakFileName);
	if(!szPakFileName || szPakFileName[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//Pak文件
	PackFile pakFile;
	if(!pakFile.createNewPakFile(szPakFileName)) return false;

	//最终文件数
	int nActFileCount = 0;
	//最终文件crc
	unsigned int nTotalCRC = 0xFFFFFFFF;

	//加入文件
	FileNodeBuf::iterator it;
	for(it=m_AllFiles.begin(); it!=m_AllFiles.end(); it++)
	{
		FileNode& file = it->second;
		
		char szTempFileName[1024] = {0};

		if(callbackFunc)
		{
			//调用Callback
			if(!callbackFunc(file.strFileInDisk.c_str(), file.strFileInPak.c_str(), szTempFileName, 1024)) 
			{
				//从列表中删除
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

	//生成(list)文件
	std::string strListFile;
	if(!_genrateListFile(strListFile, nActFileCount, nTotalCRC)) return false;

	//加入(list)文件
	if(!pakFile.insertContents(strListFile.c_str(), 0, PackFile::LIST_FILENAME,
					AC_DISKFILE, false))
	{
		return false;
	}

	//删除临时的temp文件
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

	//生成文件
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
	//加入 "时间戳\n文件计数"
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

	//加入文件名
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

//将加入的文件和现有的pak文件作内容比较，用于程序测试
bool PakMaker::comparePakFile(const char* szPakFileName, AXP_PAKMAKER_COMPARECALLBACK callbackFunc)
{
	assert(szPakFileName && callbackFunc);
	if(!szPakFileName || szPakFileName[0]==0 || callbackFunc==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//Pak文件
	PackFile pakFile;
	if(!pakFile.openPakFile(szPakFileName, true)) return false;

	//保存已经检测的文件
	typedef std::set< std::string > FILENAME_SET;
	FILENAME_SET setFileProcessed;

	//加入文件
	FileNodeBuf::iterator it;
	for(it=m_AllFiles.begin(); it!=m_AllFiles.end(); it++)
	{
		FileNode& file = it->second;
		char szTempFileName[1024] = {0};

		//打开文件
		IStream* pStream = pakFile.openFile(file.strFileInPak.c_str());

		//调用Callback
		if(!callbackFunc(file.strFileInDisk.c_str(), pStream))
		{
			setLastError(AXP_ERR_FILE_DIFF, "%s", file.strFileInPak.c_str());
			if(pStream) pStream->close();
			return false;
		}

		if(pStream)
		{
			setFileProcessed.insert(normaliseName(file.strFileInPak));
			//关闭stream
			pStream->close();
		}
	}

	//是否还有尚未检测的文件存在

	//打开List文件
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

		//分析文件描述
		std::vector< std::string > vStringVec;
		convertStringToVector(szTempLine, vStringVec, "|", true, false);
		if(vStringVec.size() != 3) 
		{
			pListStream->close();
			setLastError(AXP_ERR_FILE_FORMAT, "list file=%s", szTempLine);
			return false;
		}

		//获得文件信息
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
			API接口
*************************************/

//创建一个Maker接口
IPakMaker* createPakMaker(void)
{
	return (IPakMaker*)(new PakMaker);
}
//删除一个Maker接口
void destroyPakMaker(IPakMaker* pMaker)
{
	if(!pMaker) return;

	delete ((PakMaker*)pMaker);
}

}