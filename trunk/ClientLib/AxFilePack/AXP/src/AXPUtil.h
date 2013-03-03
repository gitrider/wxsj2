/*
		Axia|FilePack
			���ߺ���
*/

#pragma once

namespace AXP
{

/*!
\brief
	�淶�ļ���

\param fileName
	��Ҫת�����ļ���

\param  toLowCase
	�Ƿ�ת��ΪСд������GBK���룬��Ҫ������ú�locale

\param norSlash
	�Ƿ� '\' ת��Ϊ '/'

\return 
	ת������ļ���
*/
std::string normaliseName(const std::string& fileName, bool toLowCase=true, bool norSlash=true);

/*!
\brief
	��ȡ�ļ���С, ��֧�ֳ���2G�Ĵ��ļ�

\param szFileName
	�ļ���

\return 
	�ļ���С������ļ������ڣ�����0
*/
unsigned int getDiskFileSize(const char* szFileName);

/*!
\brief
	���ַ�������ָ���ķָ����ָ�

\param strResource
	�ַ���

\param vRet
	���ص��ַ�������

\param szKey
	���ڷָ�Ĺؼ���

\param bOneOfKey
	�Ƿ�ʹ�ùؼ����е�����һ���ַ���Ϊ�ָ���

\param bIgnoreEmpty
	���Կս��

\return 
	�ļ���С������ļ������ڣ�����0
*/
int convertStringToVector(const char* strResource, std::vector< std::string >& vRet, const char* szKey, bool bOneOfKey, bool bIgnoreEmpty);

/*!
\brief
	���һ���ļ���crcУ����

\param szFileName 
	�ļ���

\param dwCrc32
	���ص�crc��

\param bIgnoreTail
	�Ƿ��������ĸ��ֽ�

\return 
	�Ƿ�ɹ�
*/
bool crcFile32(const char* szFilename, unsigned int &dwCrc32, bool bIgnoreTail=false);

/*!
\brief
	����һ���ڴ��crcУ����

\param pMemoryBuf
	�ڴ�ָ��

\param dwBufSize
	�ڴ��С(bytes)

\param dwCrc32
	���ص�crc��

\return 
	�Ƿ�ɹ�
*/
bool crcMemory32(const void* pMemoryBuf, unsigned int dwBufSize, unsigned int& dwCrc32);

/*!
\brief
	���ļ�β������crcУ����

\param szFileName
	�ļ���

\return 
	�Ƿ�ɹ�
*/
bool attachCRCFlag(const char* szFileName);

/*
\brief
	����ļ�β����CRCУ����

\param szFileName 
	�ļ���

\return 
	�Ƿ�ɹ�
*/
bool checkCRCFlag(const char* szFileName);

/*
\brief
	�ݹ鴴��Ŀ¼

\param szPath
	Ŀ¼��

\return
	�Ƿ�ɹ�
*/
bool forceCreatePath(const char* szPath);

/*!
\brief
	�Ƚ������ļ����Ƿ���ͬ�����Ǵ�Сд�ͳ����ļ���������

\param szFileName1
	�ļ���1

\param szFileName2
	�ļ���2

\return
	�ȽϽ���������ͬc�е�strcmp
*/
int compareTwoFileName(const char* szFileName1, const char* szFileName2);

/** �õ�����������
*/
unsigned char getZipEncryptXORKey(unsigned int pos);

/*
\brief
	��ָ���ļ���zip�ļ�����/����

\param szFileName
	�ļ���

\param ignoreTail
	�Ƿ�����������ĸ��ֽ�

\return 
	�Ƿ�ɹ�
*/
bool encryptFile(const char* szFileName, bool ignoreTail);

/*
\brief
	ĳ�ļ��Ƿ��Ǽ��ܺ�zip�ļ�

\param szFileName
	�ļ���

\return 
	�Ƿ��Ǽ��ܺ��zip�ļ�
*/
bool isEncryptedZipFile(const char* szFileName);

}
