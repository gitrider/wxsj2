/*
		Axia|FilePack
			打包文件系统
*/

/*
|
|		-------------------------
|		|    AXP文件格式(1.1)  |  
|		-------------------------
|
|		Offset |	Type  |  Description  
|		-------+----------+------------------
|>	Head
|		0X000	  UINT		标示0X4B415054  'AXPK'
|		0X004	  UINT      版本(Major|Minor)
|		0X010     UINT      编辑标志,当这个整数为1时，表示该文件正在被编辑
|		0X014     UINT      Hash表在文件中的偏移     -> OH
|		0X018     UINT      Block表在文件中的偏移    -> OB
|		0X01C     UINT      Block表内容的个数
|		0X020     UINT      Block表最大容量大小(bytes)
|		0X024     UINT      数据块在文件中的偏移     -> OD
|       0X028     UINT      数据块的大小(bytes)
|       0X02C     UINT      其中空洞数据块的大小(bytes)
|		...       ...       ...
|
|>  Hash表结构
|       ------[HASH0]
|	OH+ 0X000     UINT      HashA - 哈希值 A，用于校验
|		0X004     UINT      HashB - 哈希值 B，用于校验
|		0X008     UINT      Data  - 附加数据
|                                   00000000 00000000 00000000 00000000
|                                   |||_______________________________|__ 对应的数据块BlockIndex
|                                   ||___________________________________ <废弃>
|                                   |____________________________________ 对应的Block是否存在 0-不存在 1-存在	
|
|       -------[HASH1]
|		0X00C     UINT      HashA - 哈希值 A，用于校验
|		...       ...       ...
|
|>  Block表结构
|		-------[BLOCK0]
|   OB+ 0X000     UINT      对应的数据块在文件中的偏移
|		0X004     UINT      本数据块大小(bytes)
|		0X008     UINT      数据块标志
|                           00000000 00000000 00000000 00000000
|                           |____________________________________ Free标志 1-free
|
|		-------[BLOCK1]
|		0X00C     UINT      对应的数据块在文件中的偏移
|		...       ...       ...
|
|>  Data数据
|   OD+ 0x000               数据区
|       ...                 ...
|
*/
#pragma once

#include "AXP.h"

namespace AXP
{

class BlockTable;

//TPK文件实现类
class PackFile : public IPakFile
{
public:
	static const char* PAKFILE_EXT;	//!< 文件扩展名
	static const char* LIST_FILENAME;	//!< 文件名列表文件

	/********************************************
					外部接口
	*********************************************/

	//打开一个存在的pak文件
	virtual bool openPakFile(const char* szPackFileName, bool bConst);

	//创建一个新的pak文件
	virtual bool createNewPakFile(const char* szPackFileName);
	
	//关闭文件
	virtual void closePakFile(void);

	//某文件在包中是否存在
	virtual bool isFileExists(const char* szFileName) const;

	//以文件流方式打开包中的一个文件
	virtual IStream* openFile(const char* szFileName);
	
	//得到包中某个文件的大小
	virtual unsigned int getFileSize(const char* szFileName) const;

	//加入/更新一个磁盘文件或者内存文件到包中
	virtual bool insertContents(const char* szContents, unsigned int nContentsLen, 
					const char* szFileInPak, AXP_CONTENTS sourceType, bool bSaveAtOnce);

	//删除包内的一个文件
	virtual bool removeFile(const char* szFileInPak, bool bSaveAtOnce);

	//文件自检功能
	virtual bool selfCheck(SELFCHECK_CALLBACK callBack);

	/********************************************
					其他公开接口
	*********************************************/
	/// 关闭数据流文件, 供Stream调用
	void closeFile(IStream* pStream);

public:
	/********************************************
					构造/析构
	*********************************************/
	PackFile();
	virtual ~PackFile();

public:
	/********************************************
				内部数据结构
	*********************************************/
	//hash表的容量
	enum 
	{
		HASH_TABLE_SIZE = 0X8000, //支持32768个文件, 占用384K内存
	};

	//BlockTable最大容量
	enum
	{
		BLOCK_TABLE_MAXSIZE = 1024*1024,	//1M空间, 最多支持8千数据块
	};

	//Block尺寸粒度
	enum
	{
		BLOCK_NODE_GSIZE = 256,		//256bytes, 注意，此值和下面的对齐算法相关!!
	};

	//文件预读尺寸标准
	enum
	{
		STREAM_PREREAD_SIZE = 500*1024,		//500k以下的文件预先读入内存
	};
	
	//文件头
	struct FILE_HEAD
	{
		unsigned int	nIdentity;					//标示0X4B505841  'AXPK'
		unsigned int	nVersion;					//版本(Major|Minor)
		unsigned int	nHashTable_Offset;			//Hash表在文件中的偏移
		unsigned int	nBlockTable_Offset;			//Block表在文件中的偏移
		unsigned int	nBlockTable_Count;			//Block表内容的个数
		unsigned int	nBlockTable_MaxSize;		//Block表最大容量大小(bytes)
		unsigned int	nEditFlag;					//编辑标志,当这个整数为1时，表示该文件正在被编辑
		unsigned int	nData_Size;					//数据块的大小,包括空洞(bytes)
		unsigned int	nData_HoleSize;				//其中空洞数据块的大小(bytes)
		unsigned int	nData_Offset;				//数据块在文件中的偏移
	};

	//HashNode
	struct FILE_HASHNODE
	{
		unsigned int nHashA;	//哈希值 A，用于校验
		unsigned int nHashB;	//哈希值 B，用于校验
		unsigned int nData;		//数据
	};

	//BlockNode
	struct FILE_BLOCKNODE
	{
		unsigned int	nDataOffset;	  //对应的数据块在文件中的偏移
		unsigned int	nBlockSize;		  //本数据块所对应的文件大小(bytes)
		unsigned int	nFlags;			  //数据块标志
	};

	//Hash表
	typedef FILE_HASHNODE FILE_HASHTABLE[HASH_TABLE_SIZE];

	//Block Table
	typedef std::vector< FILE_BLOCKNODE > FILE_BLOCKTABLE;

protected:
	/********************************************
					内部数据
	*********************************************/
	std::string			m_strFileName;	//!< 文件名
	bool				m_bConst;		//!< 是否支持编辑模式

	//磁盘数据
	HANDLE				m_hPakFile;		//!< Windows磁盘文件句柄
	FILE_HEAD			m_fileHead;		//!< 文件头
	FILE_HASHTABLE		m_hashTable;	//!< Hash表
	FILE_BLOCKTABLE		m_blockTable;	//!< Block表

	//用来记录空闲的BlockIndex,以快大小为索引，在const模式下不起作用
	typedef std::multimap< unsigned int, unsigned int > FreeBlockMap;
	FreeBlockMap		m_mapFreeBlock;

	//根据线程ID分配打开的文件句柄
	typedef std::map< unsigned int, HANDLE > FileHandleMap;
	FileHandleMap		m_mapFileHandle;

	//记录所有打开的文件
	typedef std::list< IStream* > StreamList;
	StreamList			m_listStream;

	//线程共享锁(只用于只读模式，编辑模式下不支持多线程)
	CRITICAL_SECTION	m_secFile;

	//自动锁结构
	struct AUTO_LOCK
	{
		AUTO_LOCK(LPCRITICAL_SECTION cs) : m_cs(cs)
		{
			::EnterCriticalSection(m_cs);
		}
		~AUTO_LOCK() 
		{
			::LeaveCriticalSection(m_cs); 
		}

		LPCRITICAL_SECTION m_cs;
	};

private:
	/********************************************
				Hash表操作
	*********************************************/

	//根据字符串得到位置
	int getStringPosInHashTable(const char* szString) const;

	//在Hash表中创建一个字符串的位置，如果已经存在，则会被替换
	int allocStringPosInHashTable(const char* szString);

	//Bit数据操作
	inline static unsigned int getHashNodeBlockIndex(const FILE_HASHNODE& hashNode) 
	{
		return (unsigned int)(hashNode.nData & 0X3FFFFFFF);
	}

	inline static void setHashNodeBlockIndex(FILE_HASHNODE& hashNode, unsigned int nBlockIndex)
	{
		register unsigned int temp = hashNode.nData & 0XC0000000;
		hashNode.nData = temp + (nBlockIndex & 0X3FFFFFFF);
	}

	inline static bool getHashNodeExists(const FILE_HASHNODE& hashNode)
	{
		return (hashNode.nData&0X80000000) != 0;
	}

	inline static void setHashNodeExists(FILE_HASHNODE& hashNode, bool bExists)
	{
		if(bExists)
			hashNode.nData = (hashNode.nData & 0X7FFFFFFF) + 0X80000000;
		else
			hashNode.nData = hashNode.nData & 0X7FFFFFFF;
	}

private:
	/********************************************
				Block表操作
	*********************************************/

	//上对齐数据块尺寸,对于0,扩大到256
	inline static unsigned int upBoundBlockSize(unsigned int nSize)
	{
		if((nSize&0xFF) == 0 && nSize!=0) return nSize;
		else return (nSize&(~0XFF))+0X100;
	}

	//分派一块符合要求的Block,如果无法分配，返回 first=分配的BlockIndex, second=产生的副BlockIndex
	std::pair< int, int > allocFreeBlock(unsigned int nSize);

	//Bit数据操作
	inline static bool getBlockNodeUsed(const FILE_BLOCKNODE& blockNode)
	{
		return (blockNode.nFlags & 0X80000000) != 0;
	}

	inline static void setBlockNodeUsed(FILE_BLOCKNODE& blockNode, bool bUsed)
	{
		if(bUsed)
			blockNode.nFlags = (blockNode.nFlags & 0X7FFFFFFF) + 0X80000000;
		else
			blockNode.nFlags = blockNode.nFlags & 0X7FFFFFFF;
	}

private:
	/********************************************
					磁盘表操作
	*********************************************/

	//写文件头
	bool writeFileHead(void);
	//写Hash节点
	bool writeHashNode(unsigned int nHashIndex);
	//写Block节点
	bool writeBlockNode(unsigned int nBlockIndex);
	//将一个文件写入
	bool writeDiskFile(unsigned int nOffset, unsigned int nWriteSize, const char* szDiskFile);
	//将一段内存作为文件写入
	bool writeMemory(unsigned int nOffset, unsigned int nWriteSize, 
				const char* szMemory, unsigned int nMemorySize); 
};

}
