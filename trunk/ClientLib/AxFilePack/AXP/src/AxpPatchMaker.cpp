#include "StdAfx.h"
#include "AxpPatchMaker.h"
#include "AxpErrors.h"
#include "AxpUtil.h"
#include "AxpPackFile.h"
#include "XZip.h"
#include "AxpPackMaker.h"

namespace AXP
{

//�汾�ļ���
const char* PatchMaker::VERSION_FILENAME	= "(version)";
const char* PatchMaker::PATCH_COMMAND		= "(command)";

/********************************************
				����/����
*********************************************/
PatchMaker::PatchMaker()
{
}

PatchMaker::~PatchMaker()
{
}

//����Ŀ¼, �������szVersion���ڣ�������´����汾�ļ�"(version)"
bool PatchMaker::analysePath(const char* szPath, const char* szVersion)
{
	assert(szPath);
	if(!szPath || szPath[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//�Ƿ��Ѿ�������
	std::string strPathName = normaliseName(szPath);
	VERSION_INFO_COLLECT::iterator itFind = m_verCollects.find(strPathName);
	if(itFind != m_verCollects.end())
	{
		//ɾ�������·���
		m_verCollects.erase(itFind);
	}

	//������������
	m_verCollects.insert(std::make_pair(strPathName, VERSION_INFO()));
	itFind = m_verCollects.find(strPathName);
	assert(itFind != m_verCollects.end());

	VERSION_INFO& verInfo = itFind->second;

	//·����
	verInfo.strPath = szPath;

	//�汾�ļ�
	char szVersionFile[MAX_PATH] = {0};
	strncpy(szVersionFile, szPath, MAX_PATH);
	PathAppend(szVersionFile, VERSION_FILENAME);

	//ʹ�þɰ汾
	if(!szVersion || szVersion[0] == 0)
	{
		//����version�ļ�
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
		//������Ŀ¼
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

//ͨ��Ŀ¼�ռ�����,���Ҳ����µ�version�ļ�
bool PatchMaker::_collectFromPath(const char* szPath, const char* szVersionFile, const char* szVersion, VERSION_INFO& verInfo)
{
	//�������
	assert(szPath && szVersionFile && szVersion);
	if(!szPath || !szVersionFile ||!szVersion || szPath[0]==0 || szVersionFile[0]==0 || szVersion[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//ɾ���ɵİ汾�ļ�
	if(!DeleteFile(szVersionFile) && ERROR_FILE_NOT_FOUND != GetLastError())
	{
		setLastError(AXP_ERR_FILE_ACCESS, "%s", szVersionFile);
		return false;
	}

	//�ռ�Ŀ¼�е��ļ�
	if(!_collectFromPathRecursive(szPath, "", verInfo)) return false;

	//�����汾�ļ�
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

	//д�汾��Ϣ
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

	//д�ļ���Ϣ
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

	//�ر��ļ����
	CloseHandle(hPakFile); hPakFile=0;
	return true;
}

//�ݹ��ռ�·��
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

			//�ļ���
			if(theFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(!_collectFromPathRecursive(szPathFileName, szNameRelative, verInfo)) return false;
			}
			//�ļ�
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

//���뵥���ļ�,������ļ�Ϊ����ļ�,���������е�С�ļ�
bool PatchMaker::_insertFile(const char* szFileName, const char* szRelativeName, VERSION_INFO& verInfo)
{
	//�ļ��Ƿ����
	if(_access(szFileName, 4))
	{
		setLastError(AXP_ERR_FILE_NOTFOUND, "%s", szFileName);
		return false;
	}

	//�Ƿ��Ǵ���ļ�
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

			//�����ļ�����
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
		
		//����list�ļ������crcֵ
		unsigned int nListCRC;
		pListStream->seek(0);
		unsigned int nListSize = pListStream->size();

		char* pTempBuf = new char[nListSize];
		pListStream->read(pTempBuf, nListSize);
		crcMemory32(pTempBuf, nListSize, nListCRC);
		delete[] pTempBuf; pTempBuf=0;

		pListStream->close(); pListStream = 0;

		//����list�ļ�����
		FILE_INFO newFile;
		newFile.strFile = PackFile::LIST_FILENAME;
		newFile.nFileSize = nListSize;
		newFile.nCRC = nListCRC;
		newFile.strPakName = szRelativeName;
		newFile.bNeedDel = false;

		verInfo.theFiles.push_back(newFile);

		//����pak�ļ�
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

//ͨ��Version�ļ��ռ�����
bool PatchMaker::_collectFromVersionFile(const char* szVersionFile, VERSION_INFO& verInfo)
{
	//�������
	assert(szVersionFile);
	if(!szVersionFile || szVersionFile[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//ͨ��c��ʽ�򿪰汾�ļ�
	FILE* fpVersion = fopen(szVersionFile, "r");
	if(!fpVersion)
	{
		setLastError(AXP_ERR_FILE_NOTFOUND, "%s", szVersionFile);
		return false;
	}

	char szTemp[MAX_PATH*4] = {0};
	//��ȡ��һ����Ϣ
	fgets(szTemp, MAX_PATH*4, fpVersion);

	//Trim right
	std::string strLine = szTemp;
	std::string::size_type posTrim = strLine.find_last_not_of("\r\n");
	strLine = strLine.substr(0, posTrim==std::string::npos ? posTrim : posTrim+1);

	//�ָ�
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

	//��ʼ���ļ�����
	do
	{
		//��ȡһ��
		if(0==fgets(szTemp, MAX_PATH*4, fpVersion)) break;

		//Trim right
		strLine = szTemp;
		std::string::size_type posTrim = strLine.find_last_not_of("\r\n");
		strLine = strLine.substr(0, posTrim==std::string::npos ? posTrim : posTrim+1);

		//�ָ�
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

		//����pak�ļ�
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

//�õ�ĳ��Ŀ¼�İ汾����Ŀ¼���뾭������������᷵�� ""
const char* PatchMaker::getPathVersion(const char* szPath)
{
	//�������
	assert(szPath);
	if(!szPath || szPath[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//�Ƿ��Ѿ�������
	std::string strPathName = normaliseName(szPath);
	VERSION_INFO_COLLECT::iterator itFind = m_verCollects.find(strPathName);
	if(itFind == m_verCollects.end())
	{
		//δ��������
		setLastError(AXP_ERR_ANALYSENEED, "%s", szPath);

		static std::string BLANK_STRING("");
		return BLANK_STRING.c_str();
	}

	return itFind->second.strVersion.c_str();
}

//��������Ŀ¼�ķ�������������汾����Patch
bool PatchMaker::generatePatch(const char* szOldVersionPath, const char* szNewVersionpath, 
						const char* szPatchFile, AXP_PATCHMAKER_CALLBACK callBackFunc)
{
	//�������
	assert(szOldVersionPath && szNewVersionpath && szPatchFile);
	if(	!szOldVersionPath || szOldVersionPath[0]==0 || 
		!szNewVersionpath || szNewVersionpath[0]==0 ||
		!szPatchFile || szPatchFile[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//�Ƿ��Ѿ�������
	std::string strOldPath = normaliseName(szOldVersionPath);
	VERSION_INFO_COLLECT::iterator itOld = m_verCollects.find(strOldPath);
	if(itOld == m_verCollects.end())
	{
		//δ��������
		setLastError(AXP_ERR_ANALYSENEED, "%s", szOldVersionPath);
		return false;
	}

	std::string strNewPath = normaliseName(szNewVersionpath);
	VERSION_INFO_COLLECT::iterator itNew = m_verCollects.find(strNewPath);
	if(itNew == m_verCollects.end())
	{
		//δ��������
		setLastError(AXP_ERR_ANALYSENEED, "%s", szNewVersionpath);
		return false;
	}

	VERSION_INFO& oldVer = itOld->second;
	VERSION_INFO& newVer = itNew->second;

	//�����ɰ汾��������������
	FILE_SEARCH_MAP mapSearch;
	_generateSearchMap(oldVer, mapSearch);

	//Patch����
	PATCH_INFO patchInfo;
	patchInfo.strNewVersion = newVer.strVersion;
	patchInfo.strOldVersion = oldVer.strVersion;

	//������¼�ھɰ汾����ȫ�����ڵ�pak��
	PAKFILE_SET newPakComplete;

	//0. ��ʼ������Ҫ�����°汾��ӵ��ļ�
	for(int i=0; i<(int)newVer.theFiles.size(); i++)
	{
		const FILE_INFO& fileInfo = newVer.theFiles[i];

		//����key
		std::pair< std::string, std::string > pairString = 
			std::make_pair(
				normaliseName(fileInfo.strPakName), 
				normaliseName(fileInfo.strFile));

		//�ھɰ汾�е��Ƿ�����ͬ�ļ�
		FILE_SEARCH_MAP::iterator itFileInOld = mapSearch.find(pairString);
		if(itFileInOld != mapSearch.end())	//�ھɰ汾�д���
		{
			//�¾ɰ汾ͬʱ���ڵ��ļ������ϲ���ɾ���ı��
			oldVer.theFiles[itFileInOld->second].bNeedDel = false;

			//�ж��ļ������Ƿ���ͬ
			FILE_INFO& oldFile = oldVer.theFiles[itFileInOld->second];
			if(oldFile.nFileSize == fileInfo.nFileSize && oldFile.nCRC == fileInfo.nCRC)
			{
				//�ļ���ͬ���Թ����ļ�
				continue;
			}
		}

		//�ļ��ھɰ汾�в����ڻ����в��죬��Ҫ����...

		if(!fileInfo.strPakName.empty())	//��pak�ڵ��ļ�
		{
			//��pak���ھɰ汾���Ƿ����
			if(oldVer.pakFiles.find(normaliseName(fileInfo.strPakName)) == 
				oldVer.pakFiles.end()) //��ȫ�µ�pak��
			{
				//��ȫ�µ�pak�������Ѿ���¼������
				if(newPakComplete.find(normaliseName(fileInfo.strPakName)) != 
						newPakComplete.end()) continue;

				//!������ͨ�ļ����pak�ļ�
				PATCH_FILE newPatchFile;
				newPatchFile.process = UP_NEW_FILE;
				newPatchFile.strPakFileName = "";
				newPatchFile.strFileName = fileInfo.strPakName;
				newPatchFile.strSourcePath = newVer.strPath;
				patchInfo.patchFiles.push_back(newPatchFile);

				//��¼
				newPakComplete.insert(normaliseName(fileInfo.strPakName));
				continue;
			}
		}

		//!�����ļ�
		PATCH_FILE newPatchFile;
		newPatchFile.process = UP_NEW_FILE;
		newPatchFile.strPakFileName = fileInfo.strPakName;
		newPatchFile.strFileName = fileInfo.strFile;
		newPatchFile.strSourcePath = newVer.strPath;
		patchInfo.patchFiles.push_back(newPatchFile);
	}

	//1. �����ɰ汾����Ҫɾ����pak�ļ�
	PAKFILE_SET oldPakNeedDel;
	PAKFILE_SET::iterator itPakInOld;
	for(itPakInOld=oldVer.pakFiles.begin(); itPakInOld!=oldVer.pakFiles.end(); itPakInOld++)
	{
		std::string strPakFile = *itPakInOld;

		//����°汾�и�pak���Ѿ������ڣ�ɾ��
		if(newVer.pakFiles.find(normaliseName(strPakFile)) == newVer.pakFiles.end())
		{
			//!������ͨ�ļ���ʽɾ����pak�ļ�
			PATCH_FILE newPatchFile;
			newPatchFile.process = UP_DEL_FILE;
			newPatchFile.strPakFileName = "";
			newPatchFile.strFileName = strPakFile;
			newPatchFile.strSourcePath = "";
			patchInfo.patchFiles.push_back(newPatchFile);

			oldPakNeedDel.insert(normaliseName(strPakFile));
		}
	}

	//2. ������Ҫɾ�����ļ�
	for(int i=0; i<(int)oldVer.theFiles.size(); i++)
	{
		const FILE_INFO& fileOld = oldVer.theFiles[i];
		if(!fileOld.bNeedDel) continue; //�°汾����Ȼ����

		//�����pak���е��ļ������Ҹð��Ѿ�����ȫɾ���������ٴ���
		if(!fileOld.strPakName.empty() && oldPakNeedDel.find(normaliseName(fileOld.strPakName)) != oldPakNeedDel.end())
			continue;

		//! ɾ���ļ�
		PATCH_FILE newPatchFile;
		newPatchFile.process = UP_DEL_FILE;
		newPatchFile.strPakFileName = fileOld.strPakName;
		newPatchFile.strFileName = fileOld.strFile;
		newPatchFile.strSourcePath = "";
		patchInfo.patchFiles.push_back(newPatchFile);
	}

	//3. ���(version)�ļ�����
	PATCH_FILE newPatchFile;
	newPatchFile.process = UP_NEW_FILE;
	newPatchFile.strPakFileName = "";
	newPatchFile.strFileName = VERSION_FILENAME;
	newPatchFile.strSourcePath = newVer.strPath;
	patchInfo.patchFiles.push_back(newPatchFile);

	//4. ����patch��
	if(!_generatePatchZip(patchInfo, szPatchFile, callBackFunc)) return false;

	return true;
}

void PatchMaker::_generateSearchMap(const VERSION_INFO& verInfo, FILE_SEARCH_MAP& mapSearch)
{
	mapSearch.clear();

	for(int i=0; i<(int)verInfo.theFiles.size(); i++)
	{
		const FILE_INFO& theFile = verInfo.theFiles[i];

		//����key
		std::pair< std::string, std::string > pairString = 
			std::make_pair(
				normaliseName(theFile.strPakName), 
				normaliseName(theFile.strFile));

		mapSearch[pairString] = i;
	}
}

//����Patch��
bool PatchMaker::_generatePatchZip(const PATCH_INFO& patchInfo, const char* szPatchFile,
						AXP_PATCHMAKER_CALLBACK callBackFunc)
{
	//�������
	assert(szPatchFile);
	if(!szPatchFile || szPatchFile[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//����zip�ļ�
	HZIP hz = CreateZip((void *)szPatchFile, 0, ZIP_FILENAME);
	if(!hz) 
	{
		setLastError(AXP_ERR_FILE_ACCESS, "%s", szPatchFile);
		return false;
	}

	//������ʱcommand�ļ�
	char szTempPath[MAX_PATH] = {0};
	char szCommandFile[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szTempPath);
	::GetTempFileName(szTempPath, "~axp", MAX_PATH, szCommandFile);

	//�����汾�ļ�
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

	//����patch����
	for(int i=0; i<(int)patchInfo.patchFiles.size(); i++)
	{
		const PATCH_FILE& patchFile = patchInfo.patchFiles[i];

		//�ص�����
		if(callBackFunc)
		{
			if(!callBackFunc(patchFile.strPakFileName.c_str(), patchFile.strFileName.c_str()))
			{
				continue;
			}
		}

		//��Ҫ���µ��ļ�
		if(patchFile.process == UP_NEW_FILE)
		{
			//������Դ��zip��

			//�����ɢ�ļ�
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
			//����ǰ��ڵ��ļ�
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

		//��¼command
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

//��Pak���е��ļ����뵽zip��
bool PatchMaker::_insertFileInPakToZip(const char* szPakFile, const char* szFileName, const char* szFileInZip, void* hZip)
{
	PackFile filePak;

	//��pak�ļ�
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
		//���ļ�
		pFileStream->close(); pFileStream=0;
		filePak.closePakFile();
		return true;
	}
	
	char* pTempMemory = new char[nFileSize];
	pFileStream->read(pTempMemory, nFileSize);
	pFileStream->close(); pFileStream=0;
	filePak.closePakFile();

	//�����ڴ浽zip��
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
			API�ӿ�
*************************************/

//����һ��PatchMaker�ӿ�
IPatchMaker* createPatchMaker(void)
{
	return new PatchMaker;
}

//ɾ��һ��PatchMaker�ӿ�
void destroyPatchMaker(IPatchMaker* pPatchMaker)
{
	if(!pPatchMaker) return;

	delete ((PatchMaker*)pPatchMaker);
}

}