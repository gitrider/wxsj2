/*
		Axia|FilePack
			版本升级
*/

#pragma once

#include "AxpPackFile.h"

namespace AXP
{
//版本升级
class Updater : public IUpdater
{
public:
	/********************************************
					外部接口
	*********************************************/

	//检查一个文件末尾的crc校验码是否正确
	bool checkPatchFileCRC(const char* szPatchFile);

	//添加一个Patch升级包到升级操作中，后添加的后操作
	virtual bool addPatchFile(const char* szPatchFile);

	//返回需要更新的文件数量
	virtual unsigned int getUpdateFile(void) const;

	//按照所加入的Patch包，升级某个目录
	virtual bool updateVersion(const char* szPath, AXP_UPDATE_CALLBACK callBack);

	//检测一个文件的crc校验码
	virtual bool crcFile(const char* szFilename, unsigned int &dwCrc32, bool bIgnoreTail);

	//根据zip文件获得一个由zzip库调用zzip_dir_open_ext_io时读取zip文件的io接口
	virtual void* getEncryptZipPluginIO(const char* szZipFile) const;

	//加密/解密zip文件, 并自动加上crc校验尾
	virtual bool encryptZipFile(const char* szPatchFile, const char* szNewPatchFile) const;

public:
	/********************************************
					构造/析构
	*********************************************/
	Updater();
	virtual ~Updater();

protected:
	/********************************************
					内部数据
	*********************************************/
	//一个文件的更新操作
	struct UPDATE_FILE
	{
		UPDATE_PROCESS	process;
		std::string		strPakFileName;	//对应的包文件
		std::string		strFileName;	//对应的文件
		std::string		strSourceZip;	//数据文件对应的zip文件
	};

	//更新操作集合,key=规范后的文件名
	typedef std::map< std::string, UPDATE_FILE > UPDATE_FILES;

	//包内文件更新集合, key=规范后的pak包名
	typedef std::map< std::string, UPDATE_FILES > UPDATE_PAKFILES;

	UPDATE_FILES		m_scatteredFiles;	//零散文件操作
	UPDATE_PAKFILES		m_pakFiles;			//包文件操作集合

	//更新过程中,所涉及到的更新zip文件
	typedef std::map< std::string, void* > PATCHFILE_MAP;
	PATCHFILE_MAP		m_mapPatchFile;

protected:
	/********************************************
					内部接口
	*********************************************/

	/*!
	\brief
		从zip文件中读取全部文件内容到文件中或者内存中, 
		zip文件必须是通过addPatchFile加入的patch文件,

	\param szZipFile
		zip文件名

	\param szFileName
		所读的数据文件名

	\param szDiskFileName
		如果该参数不空，则会将文件内容写入该参数指定的文件中

	\param ppMemoryBuf
		如果该参数不空，则该参数会返回一段由本函数分配的静态内存，里面是文件内容，
		外层不用考虑释放，模块退出时会被释放
	
	\nReadSize nFileSize
		文件内容长度(bytes)

	\remark
		当写入内存时, 所提供的内存会在下一次该函数被调用后被改变或者销毁,
		内存末尾自动置0
	*/
	bool _readContentsFromZip(const char* szZipFile, const char* szFileName, 
					const char* szDiskFileName, char** ppMemoryBuf, unsigned int& nFileSize);

	/*!
	\brief
		执行一个pak包内的一个更新

	\param pakFile
		需要更新的包

	\param fileUpdate
		需要执行的更新操作

	\param callBack
		升级回调函数

	\return
		是否成功
	*/
	bool _updatePakFile(PackFile& pakFile, const UPDATE_FILE& fileUpdate, AXP_UPDATE_CALLBACK callBack);

	/*!
	\brief
		执行一个散文件更新

	\param szPath
		需要升级的版本目录

	\param fileUpdate
		需要执行的更新操作

	\param callBack
		升级回调函数

	\return
		是否成功
	*/
	bool _updateScatteredFile(const char* szPath, const UPDATE_FILE& fileUpdate, AXP_UPDATE_CALLBACK callBack);
};

}