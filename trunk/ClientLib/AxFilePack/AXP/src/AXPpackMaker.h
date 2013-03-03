/*
		Axia|FilePack
			ȫ�´���ļ�����
*/

#pragma once

#include "AXP.h"

namespace AXP
{

 //Pak�ļ������߽ӿ�,��������ȫ�µ�Pak�ļ�
class PakMaker : public IPakMaker
{
public:
	/********************************************
					�ⲿ�ӿ�
	*********************************************/
	//��������һ��Ŀ¼���뵽����
	virtual bool addDiskFold(const char* szFoldInDisk, const char* szFoldInPak, const char* szExtFilter, bool bRecursive=true);

	//��������ϵ����ļ�������
	virtual bool addDiskFile(const char* szFileInDisk, const char* szFileInPak);

	//��������ļ������pak�ļ�,
	virtual bool savePakFile(const char* szPakFileName, AXP_PAKMAKER_SAVECALLBACK callbackFunc);

	//��������ļ������е�pak�ļ������ݱȽϣ����ڳ������
	virtual bool comparePakFile(const char* szPakFileName, AXP_PAKMAKER_COMPARECALLBACK callbackFunc);

public:
	/********************************************
					����/����
	*********************************************/
	PakMaker();
	virtual ~PakMaker();

private:
	//�ݹ����һ���ļ�, szFoldInDisk=·���� sFilter=������, strFoldInPak=Pak�е�·�� 
	bool _addDiskFold(const std::string& strFoldInDisk, 
			const std::set< std::string >& sFilter, 
			const std::string& strFoldInPak,
			bool bRecursive);
	
	//�����ļ��б�(list)�ļ�
	bool _genrateListFile(std::string& strTempListFile, int nActFileCount, unsigned int nTotalCRC);

protected:
	/********************************************
					�ڲ�����
	*********************************************/

	//�ļ���ʱ�������ݽṹ
	struct FileNode
	{
		bool bExcute;				//���ų�
		std::string	strFileInPak;	//Pak�е��ļ���
		std::string strFileInDisk;	//�����ϵ��ļ���
		unsigned int nFileSize;		//�ļ���С
		unsigned int nCRC;			//�ļ�У����
	};

	//key=�����淶����ļ���
	typedef std::map< std::string, FileNode > FileNodeBuf;

	FileNodeBuf m_AllFiles;	//�ļ�������
};


};
