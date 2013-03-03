/*
		Axia|FilePack
			����ļ�ϵͳ
*/

/*
|
|		-------------------------
|		|    AXP�ļ���ʽ(1.1)  |  
|		-------------------------
|
|		Offset |	Type  |  Description  
|		-------+----------+------------------
|>	Head
|		0X000	  UINT		��ʾ0X4B415054  'AXPK'
|		0X004	  UINT      �汾(Major|Minor)
|		0X010     UINT      �༭��־,���������Ϊ1ʱ����ʾ���ļ����ڱ��༭
|		0X014     UINT      Hash�����ļ��е�ƫ��     -> OH
|		0X018     UINT      Block�����ļ��е�ƫ��    -> OB
|		0X01C     UINT      Block�����ݵĸ���
|		0X020     UINT      Block�����������С(bytes)
|		0X024     UINT      ���ݿ����ļ��е�ƫ��     -> OD
|       0X028     UINT      ���ݿ�Ĵ�С(bytes)
|       0X02C     UINT      ���пն����ݿ�Ĵ�С(bytes)
|		...       ...       ...
|
|>  Hash��ṹ
|       ------[HASH0]
|	OH+ 0X000     UINT      HashA - ��ϣֵ A������У��
|		0X004     UINT      HashB - ��ϣֵ B������У��
|		0X008     UINT      Data  - ��������
|                                   00000000 00000000 00000000 00000000
|                                   |||_______________________________|__ ��Ӧ�����ݿ�BlockIndex
|                                   ||___________________________________ <����>
|                                   |____________________________________ ��Ӧ��Block�Ƿ���� 0-������ 1-����	
|
|       -------[HASH1]
|		0X00C     UINT      HashA - ��ϣֵ A������У��
|		...       ...       ...
|
|>  Block��ṹ
|		-------[BLOCK0]
|   OB+ 0X000     UINT      ��Ӧ�����ݿ����ļ��е�ƫ��
|		0X004     UINT      �����ݿ��С(bytes)
|		0X008     UINT      ���ݿ��־
|                           00000000 00000000 00000000 00000000
|                           |____________________________________ Free��־ 1-free
|
|		-------[BLOCK1]
|		0X00C     UINT      ��Ӧ�����ݿ����ļ��е�ƫ��
|		...       ...       ...
|
|>  Data����
|   OD+ 0x000               ������
|       ...                 ...
|
*/
#pragma once

#include "AXP.h"

namespace AXP
{

class BlockTable;

//TPK�ļ�ʵ����
class PackFile : public IPakFile
{
public:
	static const char* PAKFILE_EXT;	//!< �ļ���չ��
	static const char* LIST_FILENAME;	//!< �ļ����б��ļ�

	/********************************************
					�ⲿ�ӿ�
	*********************************************/

	//��һ�����ڵ�pak�ļ�
	virtual bool openPakFile(const char* szPackFileName, bool bConst);

	//����һ���µ�pak�ļ�
	virtual bool createNewPakFile(const char* szPackFileName);
	
	//�ر��ļ�
	virtual void closePakFile(void);

	//ĳ�ļ��ڰ����Ƿ����
	virtual bool isFileExists(const char* szFileName) const;

	//���ļ�����ʽ�򿪰��е�һ���ļ�
	virtual IStream* openFile(const char* szFileName);
	
	//�õ�����ĳ���ļ��Ĵ�С
	virtual unsigned int getFileSize(const char* szFileName) const;

	//����/����һ�������ļ������ڴ��ļ�������
	virtual bool insertContents(const char* szContents, unsigned int nContentsLen, 
					const char* szFileInPak, AXP_CONTENTS sourceType, bool bSaveAtOnce);

	//ɾ�����ڵ�һ���ļ�
	virtual bool removeFile(const char* szFileInPak, bool bSaveAtOnce);

	//�ļ��Լ칦��
	virtual bool selfCheck(SELFCHECK_CALLBACK callBack);

	/********************************************
					���������ӿ�
	*********************************************/
	/// �ر��������ļ�, ��Stream����
	void closeFile(IStream* pStream);

public:
	/********************************************
					����/����
	*********************************************/
	PackFile();
	virtual ~PackFile();

public:
	/********************************************
				�ڲ����ݽṹ
	*********************************************/
	//hash�������
	enum 
	{
		HASH_TABLE_SIZE = 0X8000, //֧��32768���ļ�, ռ��384K�ڴ�
	};

	//BlockTable�������
	enum
	{
		BLOCK_TABLE_MAXSIZE = 1024*1024,	//1M�ռ�, ���֧��8ǧ���ݿ�
	};

	//Block�ߴ�����
	enum
	{
		BLOCK_NODE_GSIZE = 256,		//256bytes, ע�⣬��ֵ������Ķ����㷨���!!
	};

	//�ļ�Ԥ���ߴ��׼
	enum
	{
		STREAM_PREREAD_SIZE = 500*1024,		//500k���µ��ļ�Ԥ�ȶ����ڴ�
	};
	
	//�ļ�ͷ
	struct FILE_HEAD
	{
		unsigned int	nIdentity;					//��ʾ0X4B505841  'AXPK'
		unsigned int	nVersion;					//�汾(Major|Minor)
		unsigned int	nHashTable_Offset;			//Hash�����ļ��е�ƫ��
		unsigned int	nBlockTable_Offset;			//Block�����ļ��е�ƫ��
		unsigned int	nBlockTable_Count;			//Block�����ݵĸ���
		unsigned int	nBlockTable_MaxSize;		//Block�����������С(bytes)
		unsigned int	nEditFlag;					//�༭��־,���������Ϊ1ʱ����ʾ���ļ����ڱ��༭
		unsigned int	nData_Size;					//���ݿ�Ĵ�С,�����ն�(bytes)
		unsigned int	nData_HoleSize;				//���пն����ݿ�Ĵ�С(bytes)
		unsigned int	nData_Offset;				//���ݿ����ļ��е�ƫ��
	};

	//HashNode
	struct FILE_HASHNODE
	{
		unsigned int nHashA;	//��ϣֵ A������У��
		unsigned int nHashB;	//��ϣֵ B������У��
		unsigned int nData;		//����
	};

	//BlockNode
	struct FILE_BLOCKNODE
	{
		unsigned int	nDataOffset;	  //��Ӧ�����ݿ����ļ��е�ƫ��
		unsigned int	nBlockSize;		  //�����ݿ�����Ӧ���ļ���С(bytes)
		unsigned int	nFlags;			  //���ݿ��־
	};

	//Hash��
	typedef FILE_HASHNODE FILE_HASHTABLE[HASH_TABLE_SIZE];

	//Block Table
	typedef std::vector< FILE_BLOCKNODE > FILE_BLOCKTABLE;

protected:
	/********************************************
					�ڲ�����
	*********************************************/
	std::string			m_strFileName;	//!< �ļ���
	bool				m_bConst;		//!< �Ƿ�֧�ֱ༭ģʽ

	//��������
	HANDLE				m_hPakFile;		//!< Windows�����ļ����
	FILE_HEAD			m_fileHead;		//!< �ļ�ͷ
	FILE_HASHTABLE		m_hashTable;	//!< Hash��
	FILE_BLOCKTABLE		m_blockTable;	//!< Block��

	//������¼���е�BlockIndex,�Կ��СΪ��������constģʽ�²�������
	typedef std::multimap< unsigned int, unsigned int > FreeBlockMap;
	FreeBlockMap		m_mapFreeBlock;

	//�����߳�ID����򿪵��ļ����
	typedef std::map< unsigned int, HANDLE > FileHandleMap;
	FileHandleMap		m_mapFileHandle;

	//��¼���д򿪵��ļ�
	typedef std::list< IStream* > StreamList;
	StreamList			m_listStream;

	//�̹߳�����(ֻ����ֻ��ģʽ���༭ģʽ�²�֧�ֶ��߳�)
	CRITICAL_SECTION	m_secFile;

	//�Զ����ṹ
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
				Hash�����
	*********************************************/

	//�����ַ����õ�λ��
	int getStringPosInHashTable(const char* szString) const;

	//��Hash���д���һ���ַ�����λ�ã�����Ѿ����ڣ���ᱻ�滻
	int allocStringPosInHashTable(const char* szString);

	//Bit���ݲ���
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
				Block�����
	*********************************************/

	//�϶������ݿ�ߴ�,����0,����256
	inline static unsigned int upBoundBlockSize(unsigned int nSize)
	{
		if((nSize&0xFF) == 0 && nSize!=0) return nSize;
		else return (nSize&(~0XFF))+0X100;
	}

	//����һ�����Ҫ���Block,����޷����䣬���� first=�����BlockIndex, second=�����ĸ�BlockIndex
	std::pair< int, int > allocFreeBlock(unsigned int nSize);

	//Bit���ݲ���
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
					���̱����
	*********************************************/

	//д�ļ�ͷ
	bool writeFileHead(void);
	//дHash�ڵ�
	bool writeHashNode(unsigned int nHashIndex);
	//дBlock�ڵ�
	bool writeBlockNode(unsigned int nBlockIndex);
	//��һ���ļ�д��
	bool writeDiskFile(unsigned int nOffset, unsigned int nWriteSize, const char* szDiskFile);
	//��һ���ڴ���Ϊ�ļ�д��
	bool writeMemory(unsigned int nOffset, unsigned int nWriteSize, 
				const char* szMemory, unsigned int nMemorySize); 
};

}
