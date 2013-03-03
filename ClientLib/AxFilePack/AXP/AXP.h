/*
		Axia|FilePack
			文件包系统总接口

	注意:
	  所有功能都没有考虑多线程应用!!!
*/

#pragma once

namespace AXP
{
/************************************
			API函数定义
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
			预定义
*************************************/
class IPakFile;
class IStream;
class IPakMaker;
class IPatchMaker;
class IUpdater;


/************************************
			错误代码
*************************************/
enum AXP_ERRORS
{
	AXP_ERR_SUCCESS = 0,		//成功

	AXP_ERR_FILE_NOTFOUND,		//文件/路径不存在
	AXP_ERR_FILE_FORMAT,		//文件格式错误
	AXP_ERR_FILE_ACCESS,		//无访问权限
	AXP_ERR_FILE_READ,			//读文件错误
	AXP_ERR_FILE_WRITE,			//写入文件错误
	AXP_ERR_FILE_POINT,			//移动文件指针错误
	AXP_ERR_FILE_EDITMODE,		//编辑模式错误
	AXP_ERR_FILE_DIFF,			//文件不同
	AXP_ERR_MEMORY,				//没有足够的内存
	AXP_ERR_PARAM,				//参数错误
	AXP_ERR_HASHFULL,			//Hash表满了
	AXP_ERR_BLOCKFULL,			//Block表满了
	AXP_ERR_ANALYSENEED,		//目录必须经过分析
	AXP_ERR_ZIPFILE,			//ZIP文件操作错误
	AXP_ERR_CRCFAILED,			//CRC校验失败
};

/************************************
		 数据内容类型
*************************************/
enum AXP_CONTENTS
{ 
	AC_DISKFILE,	//磁盘文件内容
	AC_MEMORY,		//内存
};

/************************************
		版本差异操作
*************************************/
enum UPDATE_PROCESS
{
	UP_NEW_FILE = 0,		//文件更新
	UP_DEL_FILE = 1,		//文件删除
};

/************************************
			API接口
*************************************/

//创建一个Pak文件接口
IPakFile*		AXP_API	createPakFile(void);
//删除一个pak文件接口
void			AXP_API	destroyPakFile(IPakFile* pFile);

//创建一个PakMaker接口
IPakMaker*		AXP_API	createPakMaker(void);
//删除一个PakMaker接口
void			AXP_API	destroyPakMaker(IPakMaker* pMaker);

//创建一个PatchMaker接口
IPatchMaker*	AXP_API	createPatchMaker(void);
//删除一个PatchMaker接口
void			AXP_API	destroyPatchMaker(IPatchMaker* pPatchMaker);

//创建一个Updater接口
IUpdater*		AXP_API	createUpdater(void);
//删除一个Updater接口
void			AXP_API	destroyUpdater(IUpdater* pUpdater);

//得到上一个错误
AXP_ERRORS		AXP_API	getLastError(void);
//得到上一个错误的描述
const char*		AXP_API	getLastErrorDesc(void);


/************************************
			打包文件接口
*************************************/
class IPakFile
{
public:
	/*!
	\brief
		打开一个存在的pak文件

	\param szFileName
		文件名

	\param bConst
		是否以只读方式打开文件

	\return 
		是否成功
	*/
	virtual bool openPakFile(const char* szPackFileName, bool bConst) = 0;

	/*!
	\brief
		创建一个新的pak文件

	\param szPackFileName
		文件名

	\return 
		是否成功
	*/
	virtual bool createNewPakFile(const char* szPackFileName) = 0;

	/*!
	\brief
		关闭文件

	\return
		None
	*/
	virtual void closePakFile(void) = 0;

	/*!
	\brief
		某文件在包中是否存在

	\param szFileName
		文件名，该文件名会被自动规范化(字符变成小写, '\'转化为'/')
		外部不需要做规范化

	\return
		文件是否存在
	*/
	virtual bool isFileExists(const char* szFileName) const = 0;

	/*!
	\brief
		以文件流方式打开包中的一个文件

	\param szFileName
		文件名，该文件名会被自动规范化(字符变成小写, '\'转化为'/')
		外部不需要做规范化

	\return
		数据流指针,如果失败, 返回0
	*/
	virtual IStream* openFile(const char* szFileName) = 0;

	/*!
	\brief
		得到包中某个文件的大小

	\param szFileName
		文件名，该文件名会被自动规范化(字符变成小写, '\'转化为'/')
		外部不需要做规范化

	\return
		文件大小(bytes), 如果文件不存在，返回0
	*/
	virtual unsigned int getFileSize(const char* szFileName) const = 0;

	/*!
	\brief
		加入/更新一个磁盘文件或者内存文件到包中
	
	\param szContents
		磁盘文件名,或者内存地址

	\param nContentsLen
		内存大小(bytes),对于加入磁盘文件方式，该参数无效

	\param szFileInPak
		加入到包中后，在包中的文件名

	\param sourceType
		数据源类型

	\param bSaveAtOnce
		是否需要马上存入磁盘

	\return 
		是否成功
	*/
	virtual bool insertContents(const char* szContents, unsigned int nContentsLen, 
					const char* szFileInPak, AXP_CONTENTS sourceType, bool bSaveAtOnce) = 0;

	/*!
	\brief
		将一个文件从包中删除

	\param szFileInPak
		在包中的文件名

	\return 
		是否成功gone
	*/
	virtual bool removeFile(const char* szFileInPak, bool bSaveAtOnce) = 0;

	/*!
	\brief
		自检回调函数

	\param szFileName
		文件名

	\param pFileStream
		文件内容流

	\param
		文件是否合法
	*/
	typedef bool (__stdcall *SELFCHECK_CALLBACK)(const char* szFileName, IStream* pFileStream);

	/*
	\brief
		文件自检功能

	\remark
		检测内容包括
			1. 文件状态是否正确，如果是编辑状态则错误
			2. 打开"(list)"文件，检测列表中的文件的大小,crc是否和实际内容是否对应
			3. 检测hash表和Block表是否有多余的内容
			4. 每个文件由
	*/
	virtual bool selfCheck(SELFCHECK_CALLBACK callBack) = 0;
};


/************************************
	文件流接口，用于读取包中的文件
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
 Pak文件制作者接口,用于制作全新的Pak文件
********************************************/
class IPakMaker
{
public:
	/*!
	\brief
		将磁盘上一个目录加入到包中

	\param szFoldInDisk
		磁盘路径名

	\param szFoldInPak
		在包中的路径名，加入的文件在pak包中的名称是相对于该路径的,
		如果加在包中的根目录下,该参数为为空字符串""

	\param szExtFilter
		文件后缀过滤器,使用';'分割多个后缀，例如 "bmp;jpg", 如果为空，表示所有文件

	\param bRecursive
		是否递归

	\return 
		是否成功加入，通过getLastError获得错误原因
	*/
	virtual bool addDiskFold(const char* szFoldInDisk, const char* szFoldInPak, const char* szExtFilter, bool bRecursive=true) = 0;

	/*!
	\brief
		加入磁盘上单个文件到包中

	\param szFileInDisk
		文件在磁盘上的路径

	\param szFileInPak
		文件在包中的名称, 这个名称中的'\'符号会被自动转化为'/', 但大小写不会转化

	\return
		是否成功加入，通过getLastError获得错误原因
	*/
	virtual bool addDiskFile(const char* szFileInDisk, const char* szFileInPak) = 0;

	/*!
	\brief
		保存Pak文件时文件名回调函数

	\param szNameInDiskOrg
		磁盘上需要打包的文件名

	\param szNameInPak
		在包中的文件名

	\param szNameInDisk
		Callback返回的，经过内容转化的磁盘文件名

	\param szNameBufLen
		szNameInDisk的长度(bytes)

	\return
		该文件是否需要加入到包中
	*/
	typedef bool (__stdcall *AXP_PAKMAKER_SAVECALLBACK)(const char *szNameInDiskOrg, 
								const char* szNameInPak, char* szNameInDisk, int nNameBufLen);

	/*!
	\brief
		将加入的文件保存成pak文件,

	\param szPakName
		打包文件的文件名

	\param AXP_PAKMAKER_SAVECALLBACK
		回调函数

	\return
		是否成功加入，通过getLastError获得错误原因
	*/
	virtual bool savePakFile(const char* szPakFileName, AXP_PAKMAKER_SAVECALLBACK callbackFunc) = 0;

	/*!
	\breif
		文件比较时的回调函数

	\param szFileNameInDisk
		磁盘上的文件名

	\param pDataStream
		在包中的数据，有可能使0

	\return 
		文件是否相同

	\remark
		有可能有部分文件不需要打入pak包内，这时pDataStream为0
	*/
	typedef bool (__stdcall *AXP_PAKMAKER_COMPARECALLBACK)(
								const char* szFileNameInDisk, IStream* pDataStream);

	/*
	\brief
		将加入的文件和现有的pak文件作内容比较，用于程序测试

	\param szPakFileName
		现有的pak文件

	\param callbackFunc
		比较回调函数

	\return
		是否完全相同，使用getLastError获得原因

	*/
	virtual bool comparePakFile(const char* szPakFileName, AXP_PAKMAKER_COMPARECALLBACK callbackFunc) = 0;
};

/*******************************************
补丁制作者，用于根据两个版本之间差异制作补丁
*******************************************/
class IPatchMaker
{
public:
	/*!
	\brief
		分析目录, 如果参数szVersion存在，则会重新创建版本文件"(version)"

	\param szPath
		需要分析的目录路径

	\param szVersion
		设置的版本号，
			如果该参数存在，则会重新创建版本文件"(version)",

			如果该参数不存在，则会打开已经存在的"(version)"文件，这种情况下"(version)"
			必须存在，如果不存在，会返回错误

	\return
		是否成功,如果不成功,使用getLastError得到错误值
	*/
	virtual bool analysePath(const char* szPath, const char* szVersion) = 0;

	/*!
	\brief
		得到某个目录的版本，该目录必须经过分析，否则会返回 ""

	\param szPath
		目录路径

	\return
		该目录的版本号，如果目录没有经过分析，返回空字符串
	*/
	virtual const char* getPathVersion(const char* szPath) = 0;

	/*!
	\brief
		生成Patch包时的回调函数

	\param szPakName
		Pak包文件名

	\param szFileName
		文件名

	\return
		是否需要加入到Patch中
	*/
	typedef bool (__stdcall *AXP_PATCHMAKER_CALLBACK)(const char *szPakName, const char* szFileName);

	/*!
	\brief
		根据两个目录的分析结果，产生版本差异Patch

	\param szOldVersionPath
		旧版本目录, 该目录必须经过分析

	\param szNewVersionpath
		新版本目录, 该目录必须经过分析

	\param szPatchFile
		产生的Patch文件名

	\return
		是否成功
	*/
	virtual bool generatePatch(const char* szOldVersionPath, const char* szNewVersionpath, 
					const char* szPatchFile, AXP_PATCHMAKER_CALLBACK callBackFunc) = 0;
};

/*******************************************
  版本升级接口，用于将旧版本升级到新版本
*******************************************/
class IUpdater
{
public:
	/*!
	\brief
		检查一个文件末尾的crc校验码是否正确

	\param szPatchFile
		文件名

	\return 
		是否成功
	*/
	virtual bool checkPatchFileCRC(const char* szPatchFile) = 0;

	/*!
	\brief
		添加一个Patch升级包到升级操作中，后添加的后操作

	\param szPatchFile
		Patch升级包文件

	\return
		是否成功
	*/
	virtual bool addPatchFile(const char* szPatchFile) = 0;

	/*!
	\brief
		返回需要更新的文件数量

	\return
		需要更新的文件数量
	*/
	virtual unsigned int getUpdateFile(void) const = 0;

	/*!
	\brief
		升级时的回调函数

	\param szPakName
		Pak包文件名

	\param szFileName
		文件名

	\param process
		所执行的操作

	\return
		是否需要加入到Patch中
	*/
	typedef void (__stdcall *AXP_UPDATE_CALLBACK)(const char *szPakName, const char* szFileName, 
									UPDATE_PROCESS process);

	/*!
	\brief
		按照所加入的Patch包，升级某个目录

	\param szPath
		需要升级的版本目录

	\param callBack
		升级回调函数

	\return
		是否成功
	*/
	virtual bool updateVersion(const char* szPath, AXP_UPDATE_CALLBACK callBack) = 0;

	/*!
	\brief
		检测一个文件的crc校验码

	\param szFileName 
		文件名

	\param dwCrc32
		返回的crc码

	\param bIgnoreTail
		是否忽略最后四个字节

	\return 
		是否成功
	*/
	virtual bool crcFile(const char* szFilename, unsigned int &dwCrc32, bool bIgnoreTail=false) = 0;

	/*!
	\brief 
		根据zip文件获得一个由zzip库调用zzip_dir_open_ext_io时读取zip文件的io接口

	\param szZipFile
		zip文件名

	\return 
		返回的io接口(zziplib 中的 zzip_plugin_io_t 指针
	*/
	virtual void* getEncryptZipPluginIO(const char* szZipFile) const = 0;
	/*!
	\brief
		加密/解密zip文件, 并自动加上crc校验尾

	\param szPatchFile
		需要处理的文件

	\param szPatchFile
		处理后的文件

	\return 
		是否成功

	\remark
		函数会自动判断该文件是否经过了crc校验,不会重复校验
	*/
	virtual bool encryptZipFile(const char* szPatchFile, const char* szNewPatchFile) const = 0;
};

}