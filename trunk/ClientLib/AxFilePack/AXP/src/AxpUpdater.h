/*
		Axia|FilePack
			�汾����
*/

#pragma once

#include "AxpPackFile.h"

namespace AXP
{
//�汾����
class Updater : public IUpdater
{
public:
	/********************************************
					�ⲿ�ӿ�
	*********************************************/

	//���һ���ļ�ĩβ��crcУ�����Ƿ���ȷ
	bool checkPatchFileCRC(const char* szPatchFile);

	//���һ��Patch�����������������У�����ӵĺ����
	virtual bool addPatchFile(const char* szPatchFile);

	//������Ҫ���µ��ļ�����
	virtual unsigned int getUpdateFile(void) const;

	//�����������Patch��������ĳ��Ŀ¼
	virtual bool updateVersion(const char* szPath, AXP_UPDATE_CALLBACK callBack);

	//���һ���ļ���crcУ����
	virtual bool crcFile(const char* szFilename, unsigned int &dwCrc32, bool bIgnoreTail);

	//����zip�ļ����һ����zzip�����zzip_dir_open_ext_ioʱ��ȡzip�ļ���io�ӿ�
	virtual void* getEncryptZipPluginIO(const char* szZipFile) const;

	//����/����zip�ļ�, ���Զ�����crcУ��β
	virtual bool encryptZipFile(const char* szPatchFile, const char* szNewPatchFile) const;

public:
	/********************************************
					����/����
	*********************************************/
	Updater();
	virtual ~Updater();

protected:
	/********************************************
					�ڲ�����
	*********************************************/
	//һ���ļ��ĸ��²���
	struct UPDATE_FILE
	{
		UPDATE_PROCESS	process;
		std::string		strPakFileName;	//��Ӧ�İ��ļ�
		std::string		strFileName;	//��Ӧ���ļ�
		std::string		strSourceZip;	//�����ļ���Ӧ��zip�ļ�
	};

	//���²�������,key=�淶����ļ���
	typedef std::map< std::string, UPDATE_FILE > UPDATE_FILES;

	//�����ļ����¼���, key=�淶���pak����
	typedef std::map< std::string, UPDATE_FILES > UPDATE_PAKFILES;

	UPDATE_FILES		m_scatteredFiles;	//��ɢ�ļ�����
	UPDATE_PAKFILES		m_pakFiles;			//���ļ���������

	//���¹�����,���漰���ĸ���zip�ļ�
	typedef std::map< std::string, void* > PATCHFILE_MAP;
	PATCHFILE_MAP		m_mapPatchFile;

protected:
	/********************************************
					�ڲ��ӿ�
	*********************************************/

	/*!
	\brief
		��zip�ļ��ж�ȡȫ���ļ����ݵ��ļ��л����ڴ���, 
		zip�ļ�������ͨ��addPatchFile�����patch�ļ�,

	\param szZipFile
		zip�ļ���

	\param szFileName
		�����������ļ���

	\param szDiskFileName
		����ò������գ���Ὣ�ļ�����д��ò���ָ�����ļ���

	\param ppMemoryBuf
		����ò������գ���ò����᷵��һ���ɱ���������ľ�̬�ڴ棬�������ļ����ݣ�
		��㲻�ÿ����ͷţ�ģ���˳�ʱ�ᱻ�ͷ�
	
	\nReadSize nFileSize
		�ļ����ݳ���(bytes)

	\remark
		��д���ڴ�ʱ, ���ṩ���ڴ������һ�θú��������ú󱻸ı��������,
		�ڴ�ĩβ�Զ���0
	*/
	bool _readContentsFromZip(const char* szZipFile, const char* szFileName, 
					const char* szDiskFileName, char** ppMemoryBuf, unsigned int& nFileSize);

	/*!
	\brief
		ִ��һ��pak���ڵ�һ������

	\param pakFile
		��Ҫ���µİ�

	\param fileUpdate
		��Ҫִ�еĸ��²���

	\param callBack
		�����ص�����

	\return
		�Ƿ�ɹ�
	*/
	bool _updatePakFile(PackFile& pakFile, const UPDATE_FILE& fileUpdate, AXP_UPDATE_CALLBACK callBack);

	/*!
	\brief
		ִ��һ��ɢ�ļ�����

	\param szPath
		��Ҫ�����İ汾Ŀ¼

	\param fileUpdate
		��Ҫִ�еĸ��²���

	\param callBack
		�����ص�����

	\return
		�Ƿ�ɹ�
	*/
	bool _updateScatteredFile(const char* szPath, const UPDATE_FILE& fileUpdate, AXP_UPDATE_CALLBACK callBack);
};

}