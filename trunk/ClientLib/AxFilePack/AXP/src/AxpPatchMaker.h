/*
		Axia|FilePack
			����������
*/

#pragma once

#include "AXP.h"

namespace AXP
{

//���������ߣ����ڸ��������汾֮�������������
class PatchMaker : public IPatchMaker
{
public:
	/********************************************
					�ⲿ�ӿ�
	*********************************************/
	static const char* VERSION_FILENAME;	//!< �汾�ļ���
	static const char* PATCH_COMMAND;		//!< �汾���������ļ�

	//����Ŀ¼, �������szVersion���ڣ�������´����汾�ļ�"(version)"
	virtual bool analysePath(const char* szPath, const char* szVersion);

	//�õ�ĳ��Ŀ¼�İ汾����Ŀ¼���뾭������������᷵�� ""
	virtual const char* getPathVersion(const char* szPath);

	//��������Ŀ¼�ķ�������������汾����Patch
	virtual bool generatePatch(const char* szOldVersionPath, const char* szNewVersionpath, 
					const char* szPatchFile, AXP_PATCHMAKER_CALLBACK callBackFunc);

public:
	/********************************************
					����/����
	*********************************************/
	PatchMaker();
	virtual ~PatchMaker();

protected:
	/********************************************
					�ڲ�����
	*********************************************/
	
	//�ļ�����
	struct FILE_INFO
	{
		std::string		strFile;		//�ļ���,����ڰ汾Ŀ¼
		unsigned int	nFileSize;		//�ļ���С(bytes)
		unsigned int	nCRC;			//�ļ�У����
		std::string		strPakName;		//������axp�������ƣ������ɢ�ļ�����Ϊ��
		bool			bNeedDel;		//��Ҫɾ��������Patch��ʹ��
	};
	typedef std::vector< FILE_INFO > FILE_INFO_COLLECT;

	//pak������
	typedef std::set< std::string > PAKFILE_SET;

	/// һ�������İ汾Ŀ¼����
	struct VERSION_INFO
	{
		std::string			strVersion;		//�汾��
		std::string			strPath;		//���ڵĴ���Ŀ¼
		FILE_INFO_COLLECT	theFiles;		//�ļ���Ϣ
		PAKFILE_SET			pakFiles;		//pak�ļ�������
	};

	//�Ѿ���������Ŀ¼����, key=Ŀ¼��(�����淶��)
	typedef std::map< std::string, VERSION_INFO > VERSION_INFO_COLLECT;
	VERSION_INFO_COLLECT	m_verCollects;

	//�汾�Ƚ�����
	struct PATCH_FILE
	{
		UPDATE_PROCESS	process;		//�ļ���������
		std::string		strPakFileName;	//��Ӧ�İ��ļ�
		std::string		strFileName;	//��Ӧ���ļ�
		std::string		strSourcePath;	//�°汾�ļ��ڴ����ϵ�·��
	};
	typedef std::vector< PATCH_FILE > PATCH_FILES;

	//һ�������İ汾����
	struct PATCH_INFO
	{
		std::string strOldVersion;
		std::string strNewVersion;
		PATCH_FILES	patchFiles;
	};

protected:
	/********************************************
					�ڲ��ӿ�
	*********************************************/
	
	//ͨ��Version�ļ��ռ�����
	bool _collectFromVersionFile(const char* szVersionFile, VERSION_INFO& verInfo);
	//ͨ��Ŀ¼�ռ�����,���Ҳ����µ�version�ļ�
	bool _collectFromPath(const char* szPath, const char* szVersionFile, const char* szVersion, VERSION_INFO& verInfo);
	//�ݹ��ռ�·��
	bool _collectFromPathRecursive(const char* szPath, const char* szPathRelative, VERSION_INFO& verInfo);
	//���뵥���ļ�,������ļ�Ϊ����ļ�,���������е�С�ļ�
	bool _insertFile(const char* szFileName, const char* szRelativeName, VERSION_INFO& verInfo);


	//key=(pakName, fileName), value=index
	typedef std::map< std::pair< std::string, std::string >, unsigned int > FILE_SEARCH_MAP;
	//�����ļ���������
	void _generateSearchMap(const VERSION_INFO& verInfo, FILE_SEARCH_MAP& mapSearch);
	//����Patch��
	bool _generatePatchZip(const PATCH_INFO& patchInfo, const char* szPatchFile, AXP_PATCHMAKER_CALLBACK callBackFunc);
	//��Pak���е��ļ����뵽zip��
	bool _insertFileInPakToZip(const char* szPakFile, const char* szFileName, const char* szFileInZip, void* hZip);

};

}