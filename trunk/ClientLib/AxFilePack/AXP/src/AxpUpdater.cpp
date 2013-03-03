#include "StdAfx.h"
#include "AxpUpdater.h"
#include "AxpErrors.h"
#include "AxpPatchMaker.h"
#include "AxpUtil.h"
#include "AxpPackFile.h"

#include <zzip/zzip.h>
#include <zzip/lib.h>

namespace AXP
{
//////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////

// ���ܷ�ʽ��zip�ļ�
static struct zzip_plugin_io g_encryptZipIO;
int __cdecl _zzip_encrypt_read (
        int fh,
        void *buf,
        unsigned cnt
        )
{
	extern DWORD g_arrdwCrc32Table[256];

	int pos = _tell(fh);
	int ret = _zzip_read(fh, buf, cnt);
	
	for(int i=0; i<ret; i++)
	{
		((unsigned char*)buf)[i] ^= getZipEncryptXORKey(pos+i);
	}

	return ret;
}

void init_encrypt_io(void)
{
	memcpy(&g_encryptZipIO, zzip_get_default_io(), sizeof(zzip_plugin_io));

	g_encryptZipIO.read = _zzip_encrypt_read;
}

//////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////

/********************************************
				����/����
*********************************************/
Updater::Updater()
{
	init_encrypt_io();
}

Updater::~Updater()
{
	//�رմ򿪵�zip�ļ�
	PATCHFILE_MAP::iterator it;
	for(it=m_mapPatchFile.begin(); it!=m_mapPatchFile.end(); it++)
	{
		ZZIP_DIR* mZzipDir = (ZZIP_DIR*)(it->second);
		if(mZzipDir)
		{
			zzip_dir_close(mZzipDir);
			mZzipDir = 0;
		}
	}
	m_mapPatchFile.clear();
}

/********************************************
	���һ���ļ�ĩβ��crcУ�����Ƿ���ȷ
*********************************************/
bool Updater::checkPatchFileCRC(const char* szPatchFile)
{
	//�������
	assert(szPatchFile);
	if(!szPatchFile || szPatchFile[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	return checkCRCFlag(szPatchFile);
}

/********************************************
���һ��Patch�����������������У�����ӵĺ����
*********************************************/
bool Updater::addPatchFile(const char* szPatchFile)
{
	//�������
	assert(szPatchFile);
	if(!szPatchFile || szPatchFile[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//�Ѿ������
	if(m_mapPatchFile.find(normaliseName(szPatchFile)) != m_mapPatchFile.end()) 
	{
		//Log something...
		return true;
	}

	//��zip�ļ�,�����Ƿ��Ǽ��ܺ��ļ��Զ�ѡ���Ƿ���Ҫʹ���Զ����ļ���ȡ�ӿ�
	zzip_error_t zzipError;
	ZZIP_DIR* mZzipDir = zzip_dir_open_ext_io(szPatchFile, &zzipError, 0, 
		(zzip_plugin_io_t)getEncryptZipPluginIO(szPatchFile));

	if (zzipError != ZZIP_NO_ERROR) 
	{
		setLastError(AXP_ERR_ZIPFILE, "ziperr=%d", zzipError);
		return false;
	}

	//����zip�ļ����
	m_mapPatchFile.insert(std::make_pair(normaliseName(szPatchFile), (void*)mZzipDir));

	//��(command)�ļ�
	char* pCmdFile=0;
	unsigned int nCmdFileSize = 0;
	if(!_readContentsFromZip(szPatchFile, PatchMaker::PATCH_COMMAND, 0, &pCmdFile, nCmdFileSize))
		return false;

	//�ֽ�command�ļ�
	std::vector< std::string > vCommand;
	convertStringToVector(pCmdFile, vCommand, "\r\n", true, true);

	//�յĸ��°��ļ�
	if(vCommand.empty())
	{
		//Log some thing...
		return true;
	}

	//���з�������
	for(int i=0; i<(int)vCommand.size(); i++)
	{
		const char* szCmdLine = vCommand[i].c_str();

		std::vector< std::string > vFileCmd;
		convertStringToVector(vCommand[i].c_str(), vFileCmd, "|", true, false);

		if(vFileCmd.size() != 3)
		{
			//Log some thing...
			continue;
		}

		UPDATE_FILE fileCmd;
		fileCmd.process = (UPDATE_PROCESS)atoi(vFileCmd[0].c_str());
		fileCmd.strPakFileName = vFileCmd[1];
		fileCmd.strFileName = vFileCmd[2];
		fileCmd.strSourceZip = szPatchFile;

		//�淶����ļ���
		std::string strNorFileName = normaliseName(fileCmd.strFileName);
		std::string strNorPakFile = normaliseName(fileCmd.strPakFileName);
		const char* szNorExt = ::PathFindExtension(strNorFileName.c_str());

		//ɢ�ļ�
		if(fileCmd.strPakFileName.empty())
		{
			m_scatteredFiles[strNorFileName] = fileCmd;

			//�������ļ�������һ��pak�ļ�
			if(szNorExt && strcmp(szNorExt, PackFile::PAKFILE_EXT) == 0)
			{
				//˵���ð���ɾ�������������, ���öԸ�pak�������в���
				m_pakFiles.erase(strNorFileName);
			}
		}
		//pak�����ļ�
		else
		{
			//���Ŀǰ�ð��Ĳ�������
			UPDATE_PAKFILES::iterator itPak = m_pakFiles.find(strNorPakFile);
			if(itPak == m_pakFiles.end())
			{
				//��δ�Ը�pak�����κβ���������
				m_pakFiles.insert(std::make_pair(strNorPakFile, UPDATE_FILES()));
				itPak = m_pakFiles.find(strNorPakFile);
			}
			assert(itPak != m_pakFiles.end());

			//�������
			itPak->second[strNorFileName] = fileCmd;
		}
	}

	return true;
}

//������Ҫ���µ��ļ�����
unsigned int Updater::getUpdateFile(void) const
{
	unsigned int nRet = (unsigned int)m_scatteredFiles.size();

	UPDATE_PAKFILES::const_iterator itPakUpdate;
	for(itPakUpdate=m_pakFiles.begin(); itPakUpdate!=m_pakFiles.end(); itPakUpdate++)
	{
		const UPDATE_FILES& pakUpdate = itPakUpdate->second;

		nRet += (unsigned int)pakUpdate.size();
	}

	return nRet;
}

//�����������Patch��������ĳ��Ŀ¼
bool Updater::updateVersion(const char* szPath, AXP_UPDATE_CALLBACK callBack)
{
	//�������
	assert(szPath);
	if(!szPath || szPath[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//0. ����axpɢ�ļ�
	UPDATE_FILES::iterator itFile;
	for(itFile=m_scatteredFiles.begin(); itFile!=m_scatteredFiles.end(); itFile++)
	{
		const UPDATE_FILE& fileUpdate = itFile->second;

		char* szExt = ::PathFindExtension(fileUpdate.strFileName.c_str());
		if(szExt && stricmp(szExt, ".axp") == 0) 
		{
			if(!_updateScatteredFile(szPath, fileUpdate, callBack)) return false;
		}
	}

	//ȷ�Ͽ���
	Sleep(500);

	//1. ����PAK�����ļ�
	UPDATE_PAKFILES::iterator itPakUpdate;
	for(itPakUpdate=m_pakFiles.begin(); itPakUpdate!=m_pakFiles.end(); itPakUpdate++)
	{
		const UPDATE_FILES& pakUpdate = itPakUpdate->second;

		//�Ը��·�ʽ��pak�ļ�
		char szPakFile[MAX_PATH] = {0};
		strncpy(szPakFile, szPath, MAX_PATH);
		PathAppend(szPakFile, itPakUpdate->first.c_str());

		PackFile pakFile;
		if(!pakFile.openPakFile(szPakFile, false)) return false;
		
		UPDATE_FILES::const_iterator itFile;

		//1.1 ����ִ��ɾ������
		for(itFile=pakUpdate.begin(); itFile!=pakUpdate.end(); itFile++)
		{
			const UPDATE_FILE& fileUpdate = itFile->second;
			if(fileUpdate.process != UP_DEL_FILE) continue;

			if(!_updatePakFile(pakFile, fileUpdate, callBack)) return false;
		}

		//1.2 ��ִ�и��²���
		for(itFile=pakUpdate.begin(); itFile!=pakUpdate.end(); itFile++)
		{
			const UPDATE_FILE& fileUpdate = itFile->second;
			if(fileUpdate.process != UP_NEW_FILE) continue;

			if(!_updatePakFile(pakFile, fileUpdate, callBack)) return false;
		}
	}

	//2. ����ɢ�ļ�
	UPDATE_FILES::iterator itVersionFile=m_scatteredFiles.end();
	for(itFile=m_scatteredFiles.begin(); itFile!=m_scatteredFiles.end(); itFile++)
	{
		const UPDATE_FILE& fileUpdate = itFile->second;

		//.axp�����Ѿ����¹�
		char* szExt = ::PathFindExtension(fileUpdate.strFileName.c_str());
		if(szExt && stricmp(szExt, ".axp") == 0) continue;

		//�汾���ļ�����������
		if(fileUpdate.strFileName == PatchMaker::VERSION_FILENAME) 
		{
			itVersionFile = itFile;
			continue;
		}

		if(!_updateScatteredFile(szPath, fileUpdate, callBack)) return false;
	}

	//�����°汾���ļ�"(version)"
	if(itVersionFile != m_scatteredFiles.end())
	{
		if(!_updateScatteredFile(szPath, itVersionFile->second, callBack)) return false;
	}

	return true;
}

//��zip�ļ��ж�ȡȫ���ļ����ݵ��ļ��л����ڴ���, 
//zip�ļ�������ͨ��addPatchFile�����patch�ļ�,
bool Updater::_readContentsFromZip(const char* szZipFile, const char* szFileName, 
					const char* szDiskFileName, char** ppMemoryBuf, unsigned int& nFileSize)
{
	//�������
	assert(szZipFile && szFileName && (szDiskFileName || ppMemoryBuf) );
	if(!szZipFile || szZipFile[0]==0 || !szFileName || szFileName[0]==0 || (!szDiskFileName && !ppMemoryBuf))
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//���������zip�ļ�
	PATCHFILE_MAP::iterator itPatch = 
		m_mapPatchFile.find(normaliseName(szZipFile));

	//�޷��ҵ�zip�ļ�
	assert(itPatch != m_mapPatchFile.end());
	if(itPatch == m_mapPatchFile.end()) 
	{
		setLastError(AXP_ERR_PARAM, "%s not inserted", szZipFile);
		return false;
	}

	//���ZIP���
	ZZIP_DIR* mZzipDir = (ZZIP_DIR*)(itPatch->second);
	assert(mZzipDir);

	std::string norFileName = normaliseName(szFileName, false, true);

	//�õ��ļ���Ϣ
	ZZIP_STAT zstat;
	memset(&zstat, 0, sizeof(ZZIP_STAT));

	//���ļ�,����򲻿����ǿ��ļ�
	ZZIP_FILE* zzipFile = zzip_file_open(mZzipDir, norFileName.c_str(), ZZIP_ONLYZIP | ZZIP_CASELESS);
	if(zzipFile)
	{
		int zip_err = zzip_dir_stat(mZzipDir, norFileName.c_str(), &zstat, ZZIP_CASEINSENSITIVE);
		if(zip_err!=0)
		{
			zzip_file_close(zzipFile);

			setLastError(AXP_ERR_ZIPFILE, "ziperr=%d", mZzipDir->errcode);
			return false;
		}
	}

	//�����Ҫд���ļ�
	if(szDiskFileName)
	{
		//ȷ���ļ�����Ŀ¼����
		char szDiskFilePath[MAX_PATH] = {0};
		strncpy(szDiskFilePath, szDiskFileName, MAX_PATH);
		PathRemoveFileSpec(szDiskFilePath);
		if(szDiskFilePath[0]!=0 && !forceCreatePath(szDiskFilePath))
		{
			if(zzipFile)zzip_file_close(zzipFile);

			setLastError(AXP_ERR_FILE_ACCESS, "Path=%s, WinErr=%d", szDiskFilePath, ::GetLastError());
			return false;
		}

		//�������ļ�
		HANDLE hDiskFile = ::CreateFile(szDiskFileName, 
							GENERIC_READ|GENERIC_WRITE,
							FILE_SHARE_READ|FILE_SHARE_WRITE,
							0,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_ARCHIVE,
							0);

		if(hDiskFile == INVALID_HANDLE_VALUE)
		{
			if(zzipFile)zzip_file_close(zzipFile);

			setLastError(AXP_ERR_FILE_ACCESS, "File=%s, WinErr=%d", szDiskFileName, ::GetLastError());
			return false;
		}

		if(zstat.st_size > 0)
		{
			const int MAX_BUFFER_SIZE = 4096;
			char buffer[MAX_BUFFER_SIZE] = {0};

			zzip_seek(zzipFile, 0, SEEK_SET);
			zzip_ssize_t zReadSize = zzip_file_read(zzipFile, buffer, sizeof(buffer));
		
			//ʵ���Ѿ�д��ĳߴ�
			unsigned int nActWriteSize = 0;

			//�ֿ��д�ļ�����
			do
			{
				//�ļ�����
				if(zReadSize==0) break;

				//д������ļ�
				DWORD dwBytesWrite;
				if(!WriteFile(hDiskFile, buffer, (DWORD)zReadSize, &dwBytesWrite, 0) || 
					dwBytesWrite != (DWORD)zReadSize)
				{
					zzip_file_close(zzipFile);
					CloseHandle(hDiskFile);

					setLastError(AXP_ERR_FILE_WRITE, "File=%s, WinErr: %d", szDiskFileName, GetLastError());
					return false;
				}

				//�ļ�����
				if(zzip_tell(zzipFile) >=zstat.st_size) break;

				zReadSize = zzip_file_read(zzipFile, buffer, sizeof(buffer));
			}while(true);
		}
		//�رվ��
		CloseHandle(hDiskFile); hDiskFile=0;
	}

	//�����Ҫ�����ڴ�
	if(ppMemoryBuf)
	{
		//����Ҫ���ڴ�
		unsigned int nMemoryNeed = (unsigned int)zstat.st_size+1;
		while(nMemoryNeed%4)nMemoryNeed++;	//upbound 4

		//����̬�ڴ��С
		static std::vector< unsigned char > s_autoMemory;
		if(s_autoMemory.size() < nMemoryNeed) 
		{
			s_autoMemory.resize(nMemoryNeed);
		}
		s_autoMemory.assign(s_autoMemory.size(), 0);

		//�����ļ�����
		if(zstat.st_size > 0)
		{
			zzip_seek(zzipFile, 0, SEEK_SET);
			zzip_ssize_t nZipSize = zzip_file_read(zzipFile, (char*)&(s_autoMemory[0]), zstat.st_size);
			if(nZipSize != zstat.st_size)
			{
				zzip_file_close(zzipFile);
				setLastError(AXP_ERR_ZIPFILE, "ziperr=%d", mZzipDir->errcode);
				return false;
			}
		}
		//��������
		*ppMemoryBuf = (char *)(&(s_autoMemory[0]));
	}

	//�رվ��
	if(zzipFile)zzip_file_close(zzipFile);	zzipFile=0;

	nFileSize = (unsigned int)zstat.st_size;
	return true;
}

//ִ��һ��pak���ڵ�һ������
bool Updater::_updatePakFile(PackFile& pakFile, const UPDATE_FILE& fileUpdate, AXP_UPDATE_CALLBACK callBack)
{
	if(fileUpdate.process == UP_DEL_FILE)
	{
		if(callBack)
		{
			callBack(fileUpdate.strPakFileName.c_str(), fileUpdate.strFileName.c_str(), 
				fileUpdate.process);
		}

		if(!pakFile.removeFile(fileUpdate.strFileName.c_str(), true) && 
			AXP_ERR_FILE_NOTFOUND != getLastError())
		{
			return false;
		}

		return true;
	}
	else if(fileUpdate.process == UP_NEW_FILE)
	{
		if(callBack)
		{
			callBack(fileUpdate.strPakFileName.c_str(), fileUpdate.strFileName.c_str(), 
				fileUpdate.process);
		}

		//zip�е��ļ���
		char szFileInZip[MAX_PATH] = {0};
		strncpy(szFileInZip, fileUpdate.strPakFileName.c_str(), MAX_PATH);
		PathAppend(szFileInZip, fileUpdate.strFileName.c_str());

		//�����ļ�����
		char* pFileBuf=0;
		unsigned int nFileSize=0;
		if(!_readContentsFromZip(fileUpdate.strSourceZip.c_str(), szFileInZip, 
			0, &pFileBuf, nFileSize)) 
		{
			pakFile.closePakFile();
			return false;
		}

		//д���ļ�
		if(!pakFile.insertContents(pFileBuf, nFileSize, fileUpdate.strFileName.c_str(), 
			AC_MEMORY, true))
		{
			pakFile.closePakFile();
			return false;
		}
		return true;
	}

	assert(false);
	setLastError(AXP_ERR_PARAM);
	return false;
}

//ִ��һ��ɢ�ļ�����
bool Updater::_updateScatteredFile(const char* szPath, const UPDATE_FILE& fileUpdate, AXP_UPDATE_CALLBACK callBack)
{
	if(callBack)
	{
		callBack(fileUpdate.strPakFileName.c_str(), fileUpdate.strFileName.c_str(), 
			fileUpdate.process);
	}

	char szFileName[MAX_PATH] = {0};
	strncpy(szFileName, szPath, MAX_PATH);
	PathAppend(szFileName, fileUpdate.strFileName.c_str());

	//�����ļ�
	if(fileUpdate.process == UP_NEW_FILE)
	{
		//��zip�е��ļ�д������ļ�
		unsigned int nFileSize;
		if(!_readContentsFromZip(fileUpdate.strSourceZip.c_str(), fileUpdate.strFileName.c_str(), 
			szFileName, 0, nFileSize)) return false;

		return true;
	}
	//ɾ���ļ�
	else if(fileUpdate.process == UP_DEL_FILE)
	{
		DeleteFile(szFileName);
		return true;
	}

	assert(false);
	setLastError(AXP_ERR_PARAM);
	return false;
}

//���һ���ļ���crcУ����
bool Updater::crcFile(const char* szFilename, unsigned int &dwCrc32, bool bIgnoreTail)
{
	return crcFile32(szFilename, dwCrc32, bIgnoreTail);
}

void* Updater::getEncryptZipPluginIO(const char* szZipFile) const
{
	return isEncryptedZipFile(szZipFile) ?  (void*)&g_encryptZipIO : (void*)zzip_get_default_io();
}

bool Updater::encryptZipFile(const char* szPatchFile, const char* szNewPatchFile) const
{
	//�������
	assert(szPatchFile);
	if(!szPatchFile || szPatchFile[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//�����ļ�
	if(compareTwoFileName(szPatchFile, szNewPatchFile) != 0)
	{
		if(!CopyFile(szPatchFile, szNewPatchFile, FALSE))
		{
			setLastError(AXP_ERR_FILE_WRITE, "File=%s, WinErr: %d", szNewPatchFile, GetLastError());
			return false;
		}
	}

	bool isCrced = checkCRCFlag(szNewPatchFile);

	//����/����
	if(!encryptFile(szNewPatchFile, isCrced)) return false;

	//����У��β
	return attachCRCFlag(szNewPatchFile);
}

/************************************
			API�ӿ�
*************************************/

IUpdater* createUpdater(void)
{
	return new Updater;
}

//ɾ��һ��PatchMaker�ӿ�
void destroyUpdater(IUpdater* pUpdater)
{
	if(!pUpdater) return;

	delete ((Updater*)pUpdater);
}

}