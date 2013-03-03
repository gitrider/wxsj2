#include "StdAfx.h"
#include "AxpPatchMaker.h"
#include "AxpErrors.h"
#include "AxpUtil.h"
#include "AxpPackFile.h"
#include "XZip.h"
#include "AxpPackMaker.h"

namespace AXP
{

//版本文件名
const char* PatchMaker::VERSION_FILENAME	= "(version)";
const char* PatchMaker::PATCH_COMMAND		= "(command)";

/********************************************
				构造/析构
*********************************************/
PatchMaker::PatchMaker()
{
}

PatchMaker::~PatchMaker()
{
}

//分析目录, 如果参数szVersion存在，则会重新创建版本文件"(version)"
bool PatchMaker::analysePath(const char* szPath, const char* szVersion)
{
	assert(szPath);
	if(!szPath || szPath[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//是否已经分析过
	std::string strPathName = normaliseName(szPath);
	VERSION_INFO_COLLECT::iterator itFind = m_verCollects.find(strPathName);
	if(itFind != m_verCollects.end())
	{
		//删除，重新分析
		m_verCollects.erase(itFind);
	}

	//创建数据容器
	m_verCollects.insert(std::make_pair(strPathName, VERSION_INFO()));
	itFind = m_verCollects.find(strPathName);
	assert(itFind != m_verCollects.end());

	VERSION_INFO& verInfo = itFind->second;

	//路径名
	verInfo.strPath = szPath;

	//版本文件
	char szVersionFile[MAX_PATH] = {0};
	strncpy(szVersionFile, szPath, MAX_PATH);
	PathAppend(szVersionFile, VERSION_FILENAME);

	//使用旧版本
	if(!szVersion || szVersion[0] == 0)
	{
		//分析version文件
		if(!_collectFromVersionFile(szVersionFile, verInfo))
		{
			m_verCollects.erase(itFind);
			return false;
		}
		else
		{
            return true;
		}
	}
	else
	{
		//分析新目录
		if(!_collectFromPath(szPath, szVersionFile, szVersion, verInfo))
		{
			m_verCollects.erase(itFind);
			return false;
		}
		else
		{
			return true;
		}
	}

	return false;
}

//通过目录收集数据,并且产生新的version文件
bool PatchMaker::_collectFromPath(const char* szPath, const char* szVersionFile, const char* szVersion, VERSION_INFO& verInfo)
{
	//参数检查
	assert(szPath && szVersionFile && szVersion);
	if(!szPath || !szVersionFile ||!szVersion || szPath[0]==0 || szVersionFile[0]==0 || szVersion[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//删除旧的版本文件
	if(!DeleteFile(szVersionFile) && ERROR_FILE_NOT_FOUND != GetLastError())
	{
		setLastError(AXP_ERR_FILE_ACCESS, "%s", szVersionFile);
		return false;
	}

	//收集目录中的文件
	if(!_collectFromPathRecursive(szPath, "", verInfo)) return false;

	//产生版本文件
	HANDLE hPakFile = ::CreateFile( szVersionFile, 
						GENERIC_READ|GENERIC_WRITE,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						0,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_ARCHIVE,
						0);

	if(hPakFile == INVALID_HANDLE_VALUE)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "WinErr=%d", ::GetLastError());
		return false;
	}

	//写版本信息
	char szTemp[MAX_PATH*4] = {0};
	int nSize = _snprintf(szTemp, MAX_PATH, "%s|%d\n", szVersion, (unsigned int)verInfo.theFiles.size());

	DWORD dwWriteBytes;
	if(!WriteFile(hPakFile, szTemp, nSize, &dwWriteBytes, 0) ||
		dwWriteBytes != nSize)
	{
		setLastError(AXP_ERR_FILE_WRITE, "WinErr=%d", ::GetLastError());
		CloseHandle(hPakFile);
		return false;
	}

	//写文件信息
	for(int i=0; i<(int)verInfo.theFiles.size(); i++)
	{
		const FILE_INFO& theFile = verInfo.theFiles[i];

		nSize = _snprintf(szTemp, MAX_PATH*4, "%s|%08X|%08X|%s\n", 
			theFile.strFile.c_str(), theFile.nFileSize, theFile.nCRC, theFile.strPakName.c_str());

		if(!WriteFile(hPakFile, szTemp, nSize, &dwWriteBytes, 0) ||
			dwWriteBytes != nSize)
		{
			setLastError(AXP_ERR_FILE_WRITE, "WinErr=%d", ::GetLastError());
			CloseHandle(hPakFile);
			return false;
		}
	}

	//关闭文件句柄
	CloseHandle(hPakFile); hPakFile=0;
	return true;
}

//递归收集路径
bool PatchMaker::_collectFromPathRecursive(const char* szPath, const char* szPathRelative, VERSION_INFO& verInfo)
{
	WIN32_FIND_DATA theFindData;

	// TO Search...
	char szFileToSearch[MAX_PATH] = {0};
	strncpy(szFileToSearch, szPath, MAX_PATH);
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
			strncpy(szPathFileName, szPath, MAX_PATH);
			PathAppend(szPathFileName, theFindData.cFileName);

			//File Name Relative
			char szNameRelative[MAX_PATH] = {0};
			strncpy(szNameRelative, szPathRelative, MAX_PATH);
			PathAppend(szNameRelative, theFindData.cFileName);

			//文件夹
			if(theFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(!_collectFromPathRecursive(szPathFileName, szNameRelative, verInfo)) return false;
			}
			//文件
			else
			{
				//add file!
				if(!_insertFile(szPathFileName, szNameRelative, verInfo)) return false;
			}
		}

		// Find Next file.
		bFind = FindNextFile(hFind, &theFindData);
	}while(bFind);

	FindClose(hFind);
	return true;
}

//加入单个文件,如果该文件为打包文件,则会加入其中的小文件
bool PatchMaker::_insertFile(const char* szFileName, const char* szRelativeName, VERSION_INFO& verInfo)
{
	//文件是否存在
	if(_access(szFileName, 4))
	{
		setLastError(AXP_ERR_FILE_NOTFOUND, "%s", szFileName);
		return false;
	}

	//是否是打包文件
	if(_stricmp(::PathFindExtension(szFileName), PackFile::PAKFILE_EXT) == 0)
	{
		PackFile pakFile;
		bool bSuccess = pakFile.openPakFile(szFileName, true);
		if (!bSuccess) return false;

		//Open list file
		IStream* pListStream = pakFile.openFile(PackFile::LIST_FILENAME);
		if(!pListStream) 
		{
			setLastError(AXP_ERR_FILE_NOTFOUND, "%s in %s", PackFile::LIST_FILENAME, szFileName);
			return false;
		}

		//skip first line
		pListStream->skipLine();

		//read second line
		char szLine[MAX_PATH*4] = {0};
		pListStream->readLine(szLine, MAX_PATH*4);
		int nFileCounts = atoi(szLine);
		verInfo.theFiles.reserve(verInfo.theFiles.size()+nFileCounts);

		/* Process the file line for line */
        while (!pListStream->eof())
		{
            int nLineLength = pListStream->readLine(szLine, MAX_PATH*4);
			if(0 == nLineLength) continue;

			//分析文件描述
			std::vector< std::string > vStringVec;
			convertStringToVector(szLine, vStringVec, "|", true, false);
			if(vStringVec.size() != 3) 
			{
				pListStream->close();

				setLastError(AXP_ERR_FILE_FORMAT, "%s", szFileName);
				return false;
			}

			FILE_INFO newFile;
			newFile.strFile = vStringVec[0];
			sscanf(vStringVec[1].c_str(), "%08X", &(newFile.nFileSize));
			sscanf(vStringVec[2].c_str(), "%08X", &(newFile.nCRC));
			newFile.strPakName = szRelativeName;
			newFile.bNeedDel = true;

			verInfo.theFiles.push_back(newFile);
		};
		
		//计算list文件本身的crc值
		unsigned int nListCRC;
		pListStream->seek(0);
		unsigned int nListSize = pListStream->size();

		char* pTempBuf = new char[nListSize];
		pListStream->read(pTempBuf, nListSize);
		crcMemory32(pTempBuf, nListSize, nListCRC);
		delete[] pTempBuf; pTempBuf=0;

		pListStream->close(); pListStream = 0;

		//加入list文件本身
		FILE_INFO newFile;
		newFile.strFile = PackFile::LIST_FILENAME;
		newFile.nFileSize = nListSize;
		newFile.nCRC = nListCRC;
		newFile.strPakName = szRelativeName;
		newFile.bNeedDel = false;

		verInfo.theFiles.push_back(newFile);

		//加入pak文件
		verInfo.pakFiles.insert(normaliseName(szRelativeName));
	}
	else
	{
		FILE_INFO newFile;
		newFile.strFile = szRelativeName;
		newFile.nFileSize = getDiskFileSize(szFileName);
		crcFile32(szFileName, newFile.nCRC);
		newFile.strPakName = "";
		newFile.bNeedDel = true;

		verInfo.theFiles.push_back(newFile);
	}

	return true;
}

//通过Version文件收集数据
bool PatchMaker::_collectFromVersionFile(const char* szVersionFile, VERSION_INFO& verInfo)
{
	//参数检查
	assert(szVersionFile);
	if(!szVersionFile || szVersionFile[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//通过c方式打开版本文件
	FILE* fpVersion = fopen(szVersionFile, "r");
	if(!fpVersion)
	{
		setLastError(AXP_ERR_FILE_NOTFOUND, "%s", szVersionFile);
		return false;
	}

	char szTemp[MAX_PATH*4] = {0};
	//读取第一行信息
	fgets(szTemp, MAX_PATH*4, fpVersion);

	//Trim right
	std::string strLine = szTemp;
	std::string::size_type posTrim = strLine.find_last_not_of("\r\n");
	strLine = strLine.substr(0, posTrim==std::string::npos ? posTrim : posTrim+1);

	//分割
	std::vector< std::string > vStringRet;
	convertStringToVector(strLine.c_str(), vStringRet, "|", true, false);

	if(vStringRet.size() != 2)
	{
		setLastError(AXP_ERR_FILE_FORMAT, "%s", szVersionFile);
		fclose(fpVersion);
		return false;
	}

	int nFileCounts = atoi(vStringRet[1].c_str());

	verInfo.strVersion = vStringRet[0];
	verInfo.theFiles.reserve(nFileCounts);

	//开始读文件描述
	do
	{
		//读取一行
		if(0==fgets(szTemp, MAX_PATH*4, fpVersion)) break;

		//Trim right
		strLine = szTemp;
		std::string::size_type posTrim = strLine.find_last_not_of("\r\n");
		strLine = strLine.substr(0, posTrim==std::string::npos ? posTrim : posTrim+1);

		//分割
		std::vector< std::string > vStringRet;
		convertStringToVector(strLine.c_str(), vStringRet, "|", true, false);

		if(vStringRet.size() != 4)
		{
			setLastError(AXP_ERR_FILE_FORMAT, "%s", szVersionFile);
			fclose(fpVersion);
			return false;
		}

		FILE_INFO newFile;
		newFile.strFile = vStringRet[0];
		sscanf(vStringRet[1].c_str(), "%08X", &(newFile.nFileSize));
		sscanf(vStringRet[2].c_str(), "%08X", &(newFile.nCRC));
		newFile.strPakName = vStringRet[3];
		newFile.bNeedDel = true;

		verInfo.theFiles.push_back(newFile);

		//加入pak文件
		if(!newFile.strPakName.empty())
		{
			verInfo.pakFiles.insert(normaliseName(newFile.strPakName));
		}

	}while(true);

	fclose(fpVersion); fpVersion=0;

	if(verInfo.theFiles.size() != nFileCounts)
	{
		setLastError(AXP_ERR_FILE_FORMAT, "%s", szVersionFile);
		return false;
	}

	return true;
}

//得到某个目录的版本，该目录必须经过分析，否则会返回 ""
const char* PatchMaker::getPathVersion(const char* szPath)
{
	//参数检查
	assert(szPath);
	if(!szPath || szPath[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//是否已经分析过
	std::string strPathName = normaliseName(szPath);
	VERSION_INFO_COLLECT::iterator itFind = m_verCollects.find(strPathName);
	if(itFind == m_verCollects.end())
	{
		//未经过分析
		setLastError(AXP_ERR_ANALYSENEED, "%s", szPath);

		static std::string BLANK_STRING("");
		return BLANK_STRING.c_str();
	}

	return itFind->second.strVersion.c_str();
}

//根据两个目录的分析结果，产生版本差异Patch
bool PatchMaker::generatePatch(const char* szOldVersionPath, const char* szNewVersionpath, 
						const char* szPatchFile, AXP_PATCHMAKER_CALLBACK callBackFunc)
{
	//参数检查
	assert(szOldVersionPath && szNewVersionpath && szPatchFile);
	if(	!szOldVersionPath || szOldVersionPath[0]==0 || 
		!szNewVersionpath || szNewVersionpath[0]==0 ||
		!szPatchFile || szPatchFile[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//是否已经分析过
	std::string strOldPath = normaliseName(szOldVersionPath);
	VERSION_INFO_COLLECT::iterator itOld = m_verCollects.find(strOldPath);
	if(itOld == m_verCollects.end())
	{
		//未经过分析
		setLastError(AXP_ERR_ANALYSENEED, "%s", szOldVersionPath);
		return false;
	}

	std::string strNewPath = normaliseName(szNewVersionpath);
	VERSION_INFO_COLLECT::iterator itNew = m_verCollects.find(strNewPath);
	if(itNew == m_verCollects.end())
	{
		//未经过分析
		setLastError(AXP_ERR_ANALYSENEED, "%s", szNewVersionpath);
		return false;
	}

	VERSION_INFO& oldVer = itOld->second;
	VERSION_INFO& newVer = itNew->second;

	//创建旧版本数据搜索索引表
	FILE_SEARCH_MAP mapSearch;
	_generateSearchMap(oldVer, mapSearch);

	//Patch创建
	PATCH_INFO patchInfo;
	patchInfo.strNewVersion = newVer.strVersion;
	patchInfo.strOldVersion = oldVer.strVersion;

	//用来记录在旧版本中完全不存在的pak包
	PAKFILE_SET newPakComplete;

	//0. 开始分析需要根据新版本添加的文件
	for(int i=0; i<(int)newVer.theFiles.size(); i++)
	{
		const FILE_INFO& fileInfo = newVer.theFiles[i];

		//检索key
		std::pair< std::string, std::string > pairString = 
			std::make_pair(
				normaliseName(fileInfo.strPakName), 
				normaliseName(fileInfo.strFile));

		//在旧版本中的是否有相同文件
		FILE_SEARCH_MAP::iterator itFileInOld = mapSearch.find(pairString);
		if(itFileInOld != mapSearch.end())	//在旧版本中存在
		{
			//新旧版本同时存在的文件，做上不用删除的标记
			oldVer.theFiles[itFileInOld->second].bNeedDel = false;

			//判断文件内容是否相同
			FILE_INFO& oldFile = oldVer.theFiles[itFileInOld->second];
			if(oldFile.nFileSize == fileInfo.nFileSize && oldFile.nCRC == fileInfo.nCRC)
			{
				//文件相同，略过该文件
				continue;
			}
		}

		//文件在旧版本中不存在或者有差异，需要更新...

		if(!fileInfo.strPakName.empty())	//是pak内的文件
		{
			//该pak包在旧版本中是否存在
			if(oldVer.pakFiles.find(normaliseName(fileInfo.strPakName)) == 
				oldVer.pakFiles.end()) //是全新的pak包
			{
				//是全新的pak包而且已经记录，忽略
				if(newPakComplete.find(normaliseName(fileInfo.strPakName)) != 
						newPakComplete.end()) continue;

				//!按照普通文件添加pak文件
				PATCH_FILE newPatchFile;
				newPatchFile.process = UP_NEW_FILE;
				newPatchFile.strPakFileName = "";
				newPatchFile.strFileName = fileInfo.strPakName;
				newPatchFile.strSourcePath = newVer.strPath;
				patchInfo.patchFiles.push_back(newPatchFile);

				//记录
				newPakComplete.insert(normaliseName(fileInfo.strPakName));
				continue;
			}
		}

		//!更新文件
		PATCH_FILE newPatchFile;
		newPatchFile.process = UP_NEW_FILE;
		newPatchFile.strPakFileName = fileInfo.strPakName;
		newPatchFile.strFileName = fileInfo.strFile;
		newPatchFile.strSourcePath = newVer.strPath;
		patchInfo.patchFiles.push_back(newPatchFile);
	}

	//1. 分析旧版本中需要删除的pak文件
	PAKFILE_SET oldPakNeedDel;
	PAKFILE_SET::iterator itPakInOld;
	for(itPakInOld=oldVer.pakFiles.begin(); itPakInOld!=oldVer.pakFiles.end(); itPakInOld++)
	{
		std::string strPakFile = *itPakInOld;

		//如果新版本中该pak包已经不存在，删除
		if(newVer.pakFiles.find(normaliseName(strPakFile)) == newVer.pakFiles.end())
		{
			//!按照普通文件方式删除该pak文件
			PATCH_FILE newPatchFile;
			newPatchFile.process = UP_DEL_FILE;
			newPatchFile.strPakFileName = "";
			newPatchFile.strFileName = strPakFile;
			newPatchFile.strSourcePath = "";
			patchInfo.patchFiles.push_back(newPatchFile);

			oldPakNeedDel.insert(normaliseName(strPakFile));
		}
	}

	//2. 分析需要删除的文件
	for(int i=0; i<(int)oldVer.theFiles.size(); i++)
	{
		const FILE_INFO& fileOld = oldVer.theFiles[i];
		if(!fileOld.bNeedDel) continue; //新版本中仍然存在

		//如果是pak包中的文件，并且该包已经被完全删除，则不用再处理
		if(!fileOld.strPakName.empty() && oldPakNeedDel.find(normaliseName(fileOld.strPakName)) != oldPakNeedDel.end())
			continue;

		//! 删除文件
		PATCH_FILE newPatchFile;
		newPatchFile.process = UP_DEL_FILE;
		newPatchFile.strPakFileName = fileOld.strPakName;
		newPatchFile.strFileName = fileOld.strFile;
		newPatchFile.strSourcePath = "";
		patchInfo.patchFiles.push_back(newPatchFile);
	}

	//3. 添加(version)文件本身
	PATCH_FILE newPatchFile;
	newPatchFile.process = UP_NEW_FILE;
	newPatchFile.strPakFileName = "";
	newPatchFile.strFileName = VERSION_FILENAME;
	newPatchFile.strSourcePath = newVer.strPath;
	patchInfo.patchFiles.push_back(newPatchFile);

	//4. 生成patch包
	if(!_generatePatchZip(patchInfo, szPatchFile, callBackFunc)) return false;

	return true;
}

void PatchMaker::_generateSearchMap(const VERSION_INFO& verInfo, FILE_SEARCH_MAP& mapSearch)
{
	mapSearch.clear();

	for(int i=0; i<(int)verInfo.theFiles.size(); i++)
	{
		const FILE_INFO& theFile = verInfo.theFiles[i];

		//检索key
		std::pair< std::string, std::string > pairString = 
			std::make_pair(
				normaliseName(theFile.strPakName), 
				normaliseName(theFile.strFile));

		mapSearch[pairString] = i;
	}
}

//生成Patch包
bool PatchMaker::_generatePatchZip(const PATCH_INFO& patchInfo, const char* szPatchFile,
						AXP_PATCHMAKER_CALLBACK callBackFunc)
{
	//参数检查
	assert(szPatchFile);
	if(!szPatchFile || szPatchFile[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//创建zip文件
	HZIP hz = CreateZip((void *)szPatchFile, 0, ZIP_FILENAME);
	if(!hz) 
	{
		setLastError(AXP_ERR_FILE_ACCESS, "%s", szPatchFile);
		return false;
	}

	//创建临时command文件
	char szTempPath[MAX_PATH] = {0};
	char szCommandFile[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szTempPath);
	::GetTempFileName(szTempPath, "~axp", MAX_PATH, szCommandFile);

	//产生版本文件
	HANDLE hCommandFile = ::CreateFile( szCommandFile, 
						GENERIC_READ|GENERIC_WRITE,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						0,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_ARCHIVE,
						0);

	if(hCommandFile == INVALID_HANDLE_VALUE)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "WinErr=%d", ::GetLastError());
		return false;
	}

	//加入patch命令
	for(int i=0; i<(int)patchInfo.patchFiles.size(); i++)
	{
		const PATCH_FILE& patchFile = patchInfo.patchFiles[i];

		//回调函数
		if(callBackFunc)
		{
			if(!callBackFunc(patchFile.strPakFileName.c_str(), patchFile.strFileName.c_str()))
			{
				continue;
			}
		}

		//需要更新的文件
		if(patchFile.process == UP_NEW_FILE)
		{
			//加入资源到zip中

			//如果是散文件
			if(patchFile.strPakFileName.empty())
			{
				char szTemp[MAX_PATH];
				strncpy(szTemp, patchFile.strSourcePath.c_str(), MAX_PATH);
				PathAppend(szTemp, patchFile.strFileName.c_str());

				ZRESULT zRet;
				if(ZR_OK != (zRet=ZipAdd(hz, normaliseName(patchFile.strFileName, false, true).c_str(), (void*)szTemp, 
								0, ZIP_FILENAME)))
				{
					setLastError(AXP_ERR_ZIPFILE, "XZIP=%d", zRet);
					CloseHandle(hCommandFile);
					CloseZip(hz); hz=0;
					return false;
				}
			}
			//如果是包内的文件
			else
			{
				char szPakFile[MAX_PATH];
				strncpy(szPakFile, patchFile.strSourcePath.c_str(), MAX_PATH);
				PathAppend(szPakFile, patchFile.strPakFileName.c_str());

				char szFileInZip[MAX_PATH];
				strncpy(szFileInZip, patchFile.strPakFileName.c_str(), MAX_PATH);
				PathAppend(szFileInZip, patchFile.strFileName.c_str());

				if(!_insertFileInPakToZip(szPakFile, patchFile.strFileName.c_str(), szFileInZip, (void*)hz))
				{
					CloseHandle(hCommandFile);
					CloseZip(hz); hz=0;
					return false;
				}
			}
		}

		//记录command
		char szCommand[MAX_PATH*4];
		int nLen = _snprintf(szCommand, MAX_PATH*4, "%d|%s|%s\n", 
			(int)patchFile.process,
			patchFile.strPakFileName.c_str(),
			patchFile.strFileName.c_str());

		DWORD dwWriteBytes;
		if(!WriteFile(hCommandFile, szCommand, nLen, &dwWriteBytes, 0) ||
			dwWriteBytes != nLen)
		{
			setLastError(AXP_ERR_FILE_WRITE, "WinErr=%d", ::GetLastError());
			CloseHandle(hCommandFile);
			CloseZip(hz);
			return false;
		}
	}
	CloseHandle(hCommandFile); hCommandFile=0;

	//Add command file in zip
	ZipAdd(hz, PATCH_COMMAND, szCommandFile, 0, ZIP_FILENAME);
	CloseZip(hz); hz=0;

	//del temp file
	DeleteFile(szCommandFile);

	//encrypt zip file
	if(!encryptFile(szPatchFile, false)) return false;

	//add crc flag at zip tail
	if(!attachCRCFlag(szPatchFile)) return false;

	return true;
}

//将Pak包中的文件加入到zip中
bool PatchMaker::_insertFileInPakToZip(const char* szPakFile, const char* szFileName, const char* szFileInZip, void* hZip)
{
	PackFile filePak;

	//打开pak文件
	if(!filePak.openPakFile(szPakFile, true)) return false;

	IStream* pFileStream = filePak.openFile(szFileName);
	if(!pFileStream)
	{
		filePak.closePakFile();
		return false;
	}

	unsigned int nFileSize = pFileStream->size();
	if(nFileSize == 0)
	{
		//空文件
		pFileStream->close(); pFileStream=0;
		filePak.closePakFile();
		return true;
	}
	
	char* pTempMemory = new char[nFileSize];
	pFileStream->read(pTempMemory, nFileSize);
	pFileStream->close(); pFileStream=0;
	filePak.closePakFile();

	//加入内存到zip中
	ZRESULT zRet = ZipAdd((HZIP)hZip, normaliseName(szFileInZip, false, true).c_str(), pTempMemory, nFileSize, ZIP_MEMORY);
	if(ZR_OK != zRet)
	{
		setLastError(AXP_ERR_ZIPFILE, "XZIP=%d", zRet);
		delete[] pTempMemory; pTempMemory=0;
		return false;
	}

	delete[] pTempMemory; pTempMemory=0;
	return true;
}

/************************************
			API接口
*************************************/

//创建一个PatchMaker接口
IPatchMaker* createPatchMaker(void)
{
	return new PatchMaker;
}

//删除一个PatchMaker接口
void destroyPatchMaker(IPatchMaker* pPatchMaker)
{
	if(!pPatchMaker) return;

	delete ((PatchMaker*)pPatchMaker);
}

}