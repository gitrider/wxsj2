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

// 加密方式读zip文件
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
				构造/析构
*********************************************/
Updater::Updater()
{
	init_encrypt_io();
}

Updater::~Updater()
{
	//关闭打开的zip文件
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
	检查一个文件末尾的crc校验码是否正确
*********************************************/
bool Updater::checkPatchFileCRC(const char* szPatchFile)
{
	//参数检查
	assert(szPatchFile);
	if(!szPatchFile || szPatchFile[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	return checkCRCFlag(szPatchFile);
}

/********************************************
添加一个Patch升级包到升级操作中，后添加的后操作
*********************************************/
bool Updater::addPatchFile(const char* szPatchFile)
{
	//参数检查
	assert(szPatchFile);
	if(!szPatchFile || szPatchFile[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//已经加入过
	if(m_mapPatchFile.find(normaliseName(szPatchFile)) != m_mapPatchFile.end()) 
	{
		//Log something...
		return true;
	}

	//打开zip文件,根据是否是加密后文件自动选择是否需要使用自定义文件读取接口
	zzip_error_t zzipError;
	ZZIP_DIR* mZzipDir = zzip_dir_open_ext_io(szPatchFile, &zzipError, 0, 
		(zzip_plugin_io_t)getEncryptZipPluginIO(szPatchFile));

	if (zzipError != ZZIP_NO_ERROR) 
	{
		setLastError(AXP_ERR_ZIPFILE, "ziperr=%d", zzipError);
		return false;
	}

	//保存zip文件句柄
	m_mapPatchFile.insert(std::make_pair(normaliseName(szPatchFile), (void*)mZzipDir));

	//打开(command)文件
	char* pCmdFile=0;
	unsigned int nCmdFileSize = 0;
	if(!_readContentsFromZip(szPatchFile, PatchMaker::PATCH_COMMAND, 0, &pCmdFile, nCmdFileSize))
		return false;

	//分解command文件
	std::vector< std::string > vCommand;
	convertStringToVector(pCmdFile, vCommand, "\r\n", true, true);

	//空的更新包文件
	if(vCommand.empty())
	{
		//Log some thing...
		return true;
	}

	//逐行分析命令
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

		//规范后的文件名
		std::string strNorFileName = normaliseName(fileCmd.strFileName);
		std::string strNorPakFile = normaliseName(fileCmd.strPakFileName);
		const char* szNorExt = ::PathFindExtension(strNorFileName.c_str());

		//散文件
		if(fileCmd.strPakFileName.empty())
		{
			m_scatteredFiles[strNorFileName] = fileCmd;

			//如果这个文件本身是一个pak文件
			if(szNorExt && strcmp(szNorExt, PackFile::PAKFILE_EXT) == 0)
			{
				//说明该包被删除或者重新添加, 重置对该pak包的所有操作
				m_pakFiles.erase(strNorFileName);
			}
		}
		//pak包内文件
		else
		{
			//获得目前该包的操作集合
			UPDATE_PAKFILES::iterator itPak = m_pakFiles.find(strNorPakFile);
			if(itPak == m_pakFiles.end())
			{
				//尚未对该pak进行任何操作，创建
				m_pakFiles.insert(std::make_pair(strNorPakFile, UPDATE_FILES()));
				itPak = m_pakFiles.find(strNorPakFile);
			}
			assert(itPak != m_pakFiles.end());

			//加入操作
			itPak->second[strNorFileName] = fileCmd;
		}
	}

	return true;
}

//返回需要更新的文件数量
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

//按照所加入的Patch包，升级某个目录
bool Updater::updateVersion(const char* szPath, AXP_UPDATE_CALLBACK callBack)
{
	//参数检查
	assert(szPath);
	if(!szPath || szPath[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//0. 更新axp散文件
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

	//确认拷贝
	Sleep(500);

	//1. 更新PAK包内文件
	UPDATE_PAKFILES::iterator itPakUpdate;
	for(itPakUpdate=m_pakFiles.begin(); itPakUpdate!=m_pakFiles.end(); itPakUpdate++)
	{
		const UPDATE_FILES& pakUpdate = itPakUpdate->second;

		//以更新方式打开pak文件
		char szPakFile[MAX_PATH] = {0};
		strncpy(szPakFile, szPath, MAX_PATH);
		PathAppend(szPakFile, itPakUpdate->first.c_str());

		PackFile pakFile;
		if(!pakFile.openPakFile(szPakFile, false)) return false;
		
		UPDATE_FILES::const_iterator itFile;

		//1.1 首先执行删除操作
		for(itFile=pakUpdate.begin(); itFile!=pakUpdate.end(); itFile++)
		{
			const UPDATE_FILE& fileUpdate = itFile->second;
			if(fileUpdate.process != UP_DEL_FILE) continue;

			if(!_updatePakFile(pakFile, fileUpdate, callBack)) return false;
		}

		//1.2 再执行更新操作
		for(itFile=pakUpdate.begin(); itFile!=pakUpdate.end(); itFile++)
		{
			const UPDATE_FILE& fileUpdate = itFile->second;
			if(fileUpdate.process != UP_NEW_FILE) continue;

			if(!_updatePakFile(pakFile, fileUpdate, callBack)) return false;
		}
	}

	//2. 更新散文件
	UPDATE_FILES::iterator itVersionFile=m_scatteredFiles.end();
	for(itFile=m_scatteredFiles.begin(); itFile!=m_scatteredFiles.end(); itFile++)
	{
		const UPDATE_FILE& fileUpdate = itFile->second;

		//.axp更新已经更新过
		char* szExt = ::PathFindExtension(fileUpdate.strFileName.c_str());
		if(szExt && stricmp(szExt, ".axp") == 0) continue;

		//版本号文件放在最后更新
		if(fileUpdate.strFileName == PatchMaker::VERSION_FILENAME) 
		{
			itVersionFile = itFile;
			continue;
		}

		if(!_updateScatteredFile(szPath, fileUpdate, callBack)) return false;
	}

	//最后更新版本号文件"(version)"
	if(itVersionFile != m_scatteredFiles.end())
	{
		if(!_updateScatteredFile(szPath, itVersionFile->second, callBack)) return false;
	}

	return true;
}

//从zip文件中读取全部文件内容到文件中或者内存中, 
//zip文件必须是通过addPatchFile加入的patch文件,
bool Updater::_readContentsFromZip(const char* szZipFile, const char* szFileName, 
					const char* szDiskFileName, char** ppMemoryBuf, unsigned int& nFileSize)
{
	//参数检查
	assert(szZipFile && szFileName && (szDiskFileName || ppMemoryBuf) );
	if(!szZipFile || szZipFile[0]==0 || !szFileName || szFileName[0]==0 || (!szDiskFileName && !ppMemoryBuf))
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//搜索加入的zip文件
	PATCHFILE_MAP::iterator itPatch = 
		m_mapPatchFile.find(normaliseName(szZipFile));

	//无法找到zip文件
	assert(itPatch != m_mapPatchFile.end());
	if(itPatch == m_mapPatchFile.end()) 
	{
		setLastError(AXP_ERR_PARAM, "%s not inserted", szZipFile);
		return false;
	}

	//获得ZIP句柄
	ZZIP_DIR* mZzipDir = (ZZIP_DIR*)(itPatch->second);
	assert(mZzipDir);

	std::string norFileName = normaliseName(szFileName, false, true);

	//得到文件信息
	ZZIP_STAT zstat;
	memset(&zstat, 0, sizeof(ZZIP_STAT));

	//打开文件,如果打不开，是空文件
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

	//如果需要写入文件
	if(szDiskFileName)
	{
		//确认文件所在目录存在
		char szDiskFilePath[MAX_PATH] = {0};
		strncpy(szDiskFilePath, szDiskFileName, MAX_PATH);
		PathRemoveFileSpec(szDiskFilePath);
		if(szDiskFilePath[0]!=0 && !forceCreatePath(szDiskFilePath))
		{
			if(zzipFile)zzip_file_close(zzipFile);

			setLastError(AXP_ERR_FILE_ACCESS, "Path=%s, WinErr=%d", szDiskFilePath, ::GetLastError());
			return false;
		}

		//创建该文件
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
		
			//实际已经写入的尺寸
			unsigned int nActWriteSize = 0;

			//分块读写文件内容
			do
			{
				//文件结束
				if(zReadSize==0) break;

				//写入磁盘文件
				DWORD dwBytesWrite;
				if(!WriteFile(hDiskFile, buffer, (DWORD)zReadSize, &dwBytesWrite, 0) || 
					dwBytesWrite != (DWORD)zReadSize)
				{
					zzip_file_close(zzipFile);
					CloseHandle(hDiskFile);

					setLastError(AXP_ERR_FILE_WRITE, "File=%s, WinErr: %d", szDiskFileName, GetLastError());
					return false;
				}

				//文件结束
				if(zzip_tell(zzipFile) >=zstat.st_size) break;

				zReadSize = zzip_file_read(zzipFile, buffer, sizeof(buffer));
			}while(true);
		}
		//关闭句柄
		CloseHandle(hDiskFile); hDiskFile=0;
	}

	//如果需要读入内存
	if(ppMemoryBuf)
	{
		//所需要的内存
		unsigned int nMemoryNeed = (unsigned int)zstat.st_size+1;
		while(nMemoryNeed%4)nMemoryNeed++;	//upbound 4

		//扩大静态内存大小
		static std::vector< unsigned char > s_autoMemory;
		if(s_autoMemory.size() < nMemoryNeed) 
		{
			s_autoMemory.resize(nMemoryNeed);
		}
		s_autoMemory.assign(s_autoMemory.size(), 0);

		//读入文件内容
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
		//返回内容
		*ppMemoryBuf = (char *)(&(s_autoMemory[0]));
	}

	//关闭句柄
	if(zzipFile)zzip_file_close(zzipFile);	zzipFile=0;

	nFileSize = (unsigned int)zstat.st_size;
	return true;
}

//执行一个pak包内的一个更新
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

		//zip中的文件名
		char szFileInZip[MAX_PATH] = {0};
		strncpy(szFileInZip, fileUpdate.strPakFileName.c_str(), MAX_PATH);
		PathAppend(szFileInZip, fileUpdate.strFileName.c_str());

		//读入文件内容
		char* pFileBuf=0;
		unsigned int nFileSize=0;
		if(!_readContentsFromZip(fileUpdate.strSourceZip.c_str(), szFileInZip, 
			0, &pFileBuf, nFileSize)) 
		{
			pakFile.closePakFile();
			return false;
		}

		//写入文件
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

//执行一个散文件更新
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

	//更新文件
	if(fileUpdate.process == UP_NEW_FILE)
	{
		//将zip中的文件写入磁盘文件
		unsigned int nFileSize;
		if(!_readContentsFromZip(fileUpdate.strSourceZip.c_str(), fileUpdate.strFileName.c_str(), 
			szFileName, 0, nFileSize)) return false;

		return true;
	}
	//删除文件
	else if(fileUpdate.process == UP_DEL_FILE)
	{
		DeleteFile(szFileName);
		return true;
	}

	assert(false);
	setLastError(AXP_ERR_PARAM);
	return false;
}

//检测一个文件的crc校验码
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
	//参数检查
	assert(szPatchFile);
	if(!szPatchFile || szPatchFile[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//拷贝文件
	if(compareTwoFileName(szPatchFile, szNewPatchFile) != 0)
	{
		if(!CopyFile(szPatchFile, szNewPatchFile, FALSE))
		{
			setLastError(AXP_ERR_FILE_WRITE, "File=%s, WinErr: %d", szNewPatchFile, GetLastError());
			return false;
		}
	}

	bool isCrced = checkCRCFlag(szNewPatchFile);

	//加密/解密
	if(!encryptFile(szNewPatchFile, isCrced)) return false;

	//加上校验尾
	return attachCRCFlag(szNewPatchFile);
}

/************************************
			API接口
*************************************/

IUpdater* createUpdater(void)
{
	return new Updater;
}

//删除一个PatchMaker接口
void destroyUpdater(IUpdater* pUpdater)
{
	if(!pUpdater) return;

	delete ((Updater*)pUpdater);
}

}