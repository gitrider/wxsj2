/*
		Axia|FilePack
			�ļ���ϵͳ�ܽӿ�

	ע��:
	  ���й��ܶ�û�п��Ƕ��߳�Ӧ��!!!
*/

#pragma once

namespace AXP
{
/************************************
			API��������
*************************************/
#ifdef AXP_STATIC_LIB
	#define AXP_API
#else
	#ifdef AXP_EXPORTS
		#define AXP_API __declspec(dllexport)
	#else
		#define AXP_API __declspec(dllimport)
	#endif
#endif



/************************************
			Ԥ����
*************************************/
class IPakFile;
class IStream;
class IPakMaker;
class IPatchMaker;
class IUpdater;


/************************************
			�������
*************************************/
enum AXP_ERRORS
{
	AXP_ERR_SUCCESS = 0,		//�ɹ�

	AXP_ERR_FILE_NOTFOUND,		//�ļ�/·��������
	AXP_ERR_FILE_FORMAT,		//�ļ���ʽ����
	AXP_ERR_FILE_ACCESS,		//�޷���Ȩ��
	AXP_ERR_FILE_READ,			//���ļ�����
	AXP_ERR_FILE_WRITE,			//д���ļ�����
	AXP_ERR_FILE_POINT,			//�ƶ��ļ�ָ�����
	AXP_ERR_FILE_EDITMODE,		//�༭ģʽ����
	AXP_ERR_FILE_DIFF,			//�ļ���ͬ
	AXP_ERR_MEMORY,				//û���㹻���ڴ�
	AXP_ERR_PARAM,				//��������
	AXP_ERR_HASHFULL,			//Hash������
	AXP_ERR_BLOCKFULL,			//Block������
	AXP_ERR_ANALYSENEED,		//Ŀ¼���뾭������
	AXP_ERR_ZIPFILE,			//ZIP�ļ���������
	AXP_ERR_CRCFAILED,			//CRCУ��ʧ��
};

/************************************
		 ������������
*************************************/
enum AXP_CONTENTS
{ 
	AC_DISKFILE,	//�����ļ�����
	AC_MEMORY,		//�ڴ�
};

/************************************
		�汾�������
*************************************/
enum UPDATE_PROCESS
{
	UP_NEW_FILE = 0,		//�ļ�����
	UP_DEL_FILE = 1,		//�ļ�ɾ��
};

/************************************
			API�ӿ�
*************************************/

//����һ��Pak�ļ��ӿ�
IPakFile*		AXP_API	createPakFile(void);
//ɾ��һ��pak�ļ��ӿ�
void			AXP_API	destroyPakFile(IPakFile* pFile);

//����һ��PakMaker�ӿ�
IPakMaker*		AXP_API	createPakMaker(void);
//ɾ��һ��PakMaker�ӿ�
void			AXP_API	destroyPakMaker(IPakMaker* pMaker);

//����һ��PatchMaker�ӿ�
IPatchMaker*	AXP_API	createPatchMaker(void);
//ɾ��һ��PatchMaker�ӿ�
void			AXP_API	destroyPatchMaker(IPatchMaker* pPatchMaker);

//����һ��Updater�ӿ�
IUpdater*		AXP_API	createUpdater(void);
//ɾ��һ��Updater�ӿ�
void			AXP_API	destroyUpdater(IUpdater* pUpdater);

//�õ���һ������
AXP_ERRORS		AXP_API	getLastError(void);
//�õ���һ�����������
const char*		AXP_API	getLastErrorDesc(void);


/************************************
			����ļ��ӿ�
*************************************/
class IPakFile
{
public:
	/*!
	\brief
		��һ�����ڵ�pak�ļ�

	\param szFileName
		�ļ���

	\param bConst
		�Ƿ���ֻ����ʽ���ļ�

	\return 
		�Ƿ�ɹ�
	*/
	virtual bool openPakFile(const char* szPackFileName, bool bConst) = 0;

	/*!
	\brief
		����һ���µ�pak�ļ�

	\param szPackFileName
		�ļ���

	\return 
		�Ƿ�ɹ�
	*/
	virtual bool createNewPakFile(const char* szPackFileName) = 0;

	/*!
	\brief
		�ر��ļ�

	\return
		None
	*/
	virtual void closePakFile(void) = 0;

	/*!
	\brief
		ĳ�ļ��ڰ����Ƿ����

	\param szFileName
		�ļ��������ļ����ᱻ�Զ��淶��(�ַ����Сд, '\'ת��Ϊ'/')
		�ⲿ����Ҫ���淶��

	\return
		�ļ��Ƿ����
	*/
	virtual bool isFileExists(const char* szFileName) const = 0;

	/*!
	\brief
		���ļ�����ʽ�򿪰��е�һ���ļ�

	\param szFileName
		�ļ��������ļ����ᱻ�Զ��淶��(�ַ����Сд, '\'ת��Ϊ'/')
		�ⲿ����Ҫ���淶��

	\return
		������ָ��,���ʧ��, ����0
	*/
	virtual IStream* openFile(const char* szFileName) = 0;

	/*!
	\brief
		�õ�����ĳ���ļ��Ĵ�С

	\param szFileName
		�ļ��������ļ����ᱻ�Զ��淶��(�ַ����Сд, '\'ת��Ϊ'/')
		�ⲿ����Ҫ���淶��

	\return
		�ļ���С(bytes), ����ļ������ڣ�����0
	*/
	virtual unsigned int getFileSize(const char* szFileName) const = 0;

	/*!
	\brief
		����/����һ�������ļ������ڴ��ļ�������
	
	\param szContents
		�����ļ���,�����ڴ��ַ

	\param nContentsLen
		�ڴ��С(bytes),���ڼ�������ļ���ʽ���ò�����Ч

	\param szFileInPak
		���뵽���к��ڰ��е��ļ���

	\param sourceType
		����Դ����

	\param bSaveAtOnce
		�Ƿ���Ҫ���ϴ������

	\return 
		�Ƿ�ɹ�
	*/
	virtual bool insertContents(const char* szContents, unsigned int nContentsLen, 
					const char* szFileInPak, AXP_CONTENTS sourceType, bool bSaveAtOnce) = 0;

	/*!
	\brief
		��һ���ļ��Ӱ���ɾ��

	\param szFileInPak
		�ڰ��е��ļ���

	\return 
		�Ƿ�ɹ�gone
	*/
	virtual bool removeFile(const char* szFileInPak, bool bSaveAtOnce) = 0;

	/*!
	\brief
		�Լ�ص�����

	\param szFileName
		�ļ���

	\param pFileStream
		�ļ�������

	\param
		�ļ��Ƿ�Ϸ�
	*/
	typedef bool (__stdcall *SELFCHECK_CALLBACK)(const char* szFileName, IStream* pFileStream);

	/*
	\brief
		�ļ��Լ칦��

	\remark
		������ݰ���
			1. �ļ�״̬�Ƿ���ȷ������Ǳ༭״̬�����
			2. ��"(list)"�ļ�������б��е��ļ��Ĵ�С,crc�Ƿ��ʵ�������Ƿ��Ӧ
			3. ���hash���Block���Ƿ��ж��������
			4. ÿ���ļ���
	*/
	virtual bool selfCheck(SELFCHECK_CALLBACK callBack) = 0;
};


/************************************
	�ļ����ӿڣ����ڶ�ȡ���е��ļ�
*************************************/
class IStream
{
public:
	/*!
	\brief
		Read the requisite number of bytes from the stream, 
		stopping at the end of the file.
	\param buf 
		Reference to a buffer pointer
	\param count 
		Number of bytes to read
	\returns 
		The number of bytes read
	*/
	virtual unsigned int __fastcall read(void* buf, unsigned int count) = 0;

	/*!
	\brief	
		Get a single line from the stream.

	\param retBuf 
		Reference to a buffer pointer

	\param maxCount 
		The maximum length of data to be read, excluding the terminating character

	\returns 
		The number of bytes read, excluding the terminating character
	*/
	virtual unsigned int readLine(char* retBuf, unsigned int maxCount) = 0;

	/*!
	\brief
		Skip a single line from the stream.

	\return
		The number of bytes skipped
	*/
	virtual unsigned int skipLine(void) = 0;

	/*!
	\brief
		Repositions the read point to a specified byte.
	*/
	virtual void seek(unsigned int pos) = 0;

	/*!
	\brief
		Returns the current byte offset from beginning 
	*/
	virtual unsigned int tell(void) const = 0;

	/*!
	\brief
		Returns true if the stream has reached the end.
	*/
	virtual bool eof(void) const = 0;

	/*!
	\brief
		Returns the total size of the data to be read from the stream, 
		or 0 if this is indeterminate for this stream. 
	*/
	virtual unsigned int size(void) const = 0;

	/*!
		Return the crc checksum of this contents.
	*/
	virtual unsigned int crc(void) = 0;

	/*!
	\brief
		Close this file, can't call any function after now!
	*/
	virtual void close(void) = 0;
};

/*******************************************
 Pak�ļ������߽ӿ�,��������ȫ�µ�Pak�ļ�
********************************************/
class IPakMaker
{
public:
	/*!
	\brief
		��������һ��Ŀ¼���뵽����

	\param szFoldInDisk
		����·����

	\param szFoldInPak
		�ڰ��е�·������������ļ���pak���е�����������ڸ�·����,
		������ڰ��еĸ�Ŀ¼��,�ò���ΪΪ���ַ���""

	\param szExtFilter
		�ļ���׺������,ʹ��';'�ָ�����׺������ "bmp;jpg", ���Ϊ�գ���ʾ�����ļ�

	\param bRecursive
		�Ƿ�ݹ�

	\return 
		�Ƿ�ɹ����룬ͨ��getLastError��ô���ԭ��
	*/
	virtual bool addDiskFold(const char* szFoldInDisk, const char* szFoldInPak, const char* szExtFilter, bool bRecursive=true) = 0;

	/*!
	\brief
		��������ϵ����ļ�������

	\param szFileInDisk
		�ļ��ڴ����ϵ�·��

	\param szFileInPak
		�ļ��ڰ��е�����, ��������е�'\'���Żᱻ�Զ�ת��Ϊ'/', ����Сд����ת��

	\return
		�Ƿ�ɹ����룬ͨ��getLastError��ô���ԭ��
	*/
	virtual bool addDiskFile(const char* szFileInDisk, const char* szFileInPak) = 0;

	/*!
	\brief
		����Pak�ļ�ʱ�ļ����ص�����

	\param szNameInDiskOrg
		��������Ҫ������ļ���

	\param szNameInPak
		�ڰ��е��ļ���

	\param szNameInDisk
		Callback���صģ���������ת���Ĵ����ļ���

	\param szNameBufLen
		szNameInDisk�ĳ���(bytes)

	\return
		���ļ��Ƿ���Ҫ���뵽����
	*/
	typedef bool (__stdcall *AXP_PAKMAKER_SAVECALLBACK)(const char *szNameInDiskOrg, 
								const char* szNameInPak, char* szNameInDisk, int nNameBufLen);

	/*!
	\brief
		��������ļ������pak�ļ�,

	\param szPakName
		����ļ����ļ���

	\param AXP_PAKMAKER_SAVECALLBACK
		�ص�����

	\return
		�Ƿ�ɹ����룬ͨ��getLastError��ô���ԭ��
	*/
	virtual bool savePakFile(const char* szPakFileName, AXP_PAKMAKER_SAVECALLBACK callbackFunc) = 0;

	/*!
	\breif
		�ļ��Ƚ�ʱ�Ļص�����

	\param szFileNameInDisk
		�����ϵ��ļ���

	\param pDataStream
		�ڰ��е����ݣ��п���ʹ0

	\return 
		�ļ��Ƿ���ͬ

	\remark
		�п����в����ļ�����Ҫ����pak���ڣ���ʱpDataStreamΪ0
	*/
	typedef bool (__stdcall *AXP_PAKMAKER_COMPARECALLBACK)(
								const char* szFileNameInDisk, IStream* pDataStream);

	/*
	\brief
		��������ļ������е�pak�ļ������ݱȽϣ����ڳ������

	\param szPakFileName
		���е�pak�ļ�

	\param callbackFunc
		�Ƚϻص�����

	\return
		�Ƿ���ȫ��ͬ��ʹ��getLastError���ԭ��

	*/
	virtual bool comparePakFile(const char* szPakFileName, AXP_PAKMAKER_COMPARECALLBACK callbackFunc) = 0;
};

/*******************************************
���������ߣ����ڸ��������汾֮�������������
*******************************************/
class IPatchMaker
{
public:
	/*!
	\brief
		����Ŀ¼, �������szVersion���ڣ�������´����汾�ļ�"(version)"

	\param szPath
		��Ҫ������Ŀ¼·��

	\param szVersion
		���õİ汾�ţ�
			����ò������ڣ�������´����汾�ļ�"(version)",

			����ò��������ڣ������Ѿ����ڵ�"(version)"�ļ������������"(version)"
			������ڣ���������ڣ��᷵�ش���

	\return
		�Ƿ�ɹ�,������ɹ�,ʹ��getLastError�õ�����ֵ
	*/
	virtual bool analysePath(const char* szPath, const char* szVersion) = 0;

	/*!
	\brief
		�õ�ĳ��Ŀ¼�İ汾����Ŀ¼���뾭������������᷵�� ""

	\param szPath
		Ŀ¼·��

	\return
		��Ŀ¼�İ汾�ţ����Ŀ¼û�о������������ؿ��ַ���
	*/
	virtual const char* getPathVersion(const char* szPath) = 0;

	/*!
	\brief
		����Patch��ʱ�Ļص�����

	\param szPakName
		Pak���ļ���

	\param szFileName
		�ļ���

	\return
		�Ƿ���Ҫ���뵽Patch��
	*/
	typedef bool (__stdcall *AXP_PATCHMAKER_CALLBACK)(const char *szPakName, const char* szFileName);

	/*!
	\brief
		��������Ŀ¼�ķ�������������汾����Patch

	\param szOldVersionPath
		�ɰ汾Ŀ¼, ��Ŀ¼���뾭������

	\param szNewVersionpath
		�°汾Ŀ¼, ��Ŀ¼���뾭������

	\param szPatchFile
		������Patch�ļ���

	\return
		�Ƿ�ɹ�
	*/
	virtual bool generatePatch(const char* szOldVersionPath, const char* szNewVersionpath, 
					const char* szPatchFile, AXP_PATCHMAKER_CALLBACK callBackFunc) = 0;
};

/*******************************************
  �汾�����ӿڣ����ڽ��ɰ汾�������°汾
*******************************************/
class IUpdater
{
public:
	/*!
	\brief
		���һ���ļ�ĩβ��crcУ�����Ƿ���ȷ

	\param szPatchFile
		�ļ���

	\return 
		�Ƿ�ɹ�
	*/
	virtual bool checkPatchFileCRC(const char* szPatchFile) = 0;

	/*!
	\brief
		���һ��Patch�����������������У�����ӵĺ����

	\param szPatchFile
		Patch�������ļ�

	\return
		�Ƿ�ɹ�
	*/
	virtual bool addPatchFile(const char* szPatchFile) = 0;

	/*!
	\brief
		������Ҫ���µ��ļ�����

	\return
		��Ҫ���µ��ļ�����
	*/
	virtual unsigned int getUpdateFile(void) const = 0;

	/*!
	\brief
		����ʱ�Ļص�����

	\param szPakName
		Pak���ļ���

	\param szFileName
		�ļ���

	\param process
		��ִ�еĲ���

	\return
		�Ƿ���Ҫ���뵽Patch��
	*/
	typedef void (__stdcall *AXP_UPDATE_CALLBACK)(const char *szPakName, const char* szFileName, 
									UPDATE_PROCESS process);

	/*!
	\brief
		�����������Patch��������ĳ��Ŀ¼

	\param szPath
		��Ҫ�����İ汾Ŀ¼

	\param callBack
		�����ص�����

	\return
		�Ƿ�ɹ�
	*/
	virtual bool updateVersion(const char* szPath, AXP_UPDATE_CALLBACK callBack) = 0;

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
	virtual bool crcFile(const char* szFilename, unsigned int &dwCrc32, bool bIgnoreTail=false) = 0;

	/*!
	\brief 
		����zip�ļ����һ����zzip�����zzip_dir_open_ext_ioʱ��ȡzip�ļ���io�ӿ�

	\param szZipFile
		zip�ļ���

	\return 
		���ص�io�ӿ�(zziplib �е� zzip_plugin_io_t ָ��
	*/
	virtual void* getEncryptZipPluginIO(const char* szZipFile) const = 0;
	/*!
	\brief
		����/����zip�ļ�, ���Զ�����crcУ��β

	\param szPatchFile
		��Ҫ������ļ�

	\param szPatchFile
		�������ļ�

	\return 
		�Ƿ�ɹ�

	\remark
		�������Զ��жϸ��ļ��Ƿ񾭹���crcУ��,�����ظ�У��
	*/
	virtual bool encryptZipFile(const char* szPatchFile, const char* szNewPatchFile) const = 0;
};

}