#include "StdAfx.h"
#include "AXPPackFile.h"
#include "AXPErrors.h"
#include "AXPUtil.h"

namespace AXP
{

const unsigned int AXPK_FILE_FLAG	= 0X504B5A46;	// 'PKZF'
const char* PackFile::PAKFILE_EXT	= ".pkf";		// 文件扩展名
const char* PackFile::LIST_FILENAME = "(lists)";		//List文件在包中的文件名

//构造/析构
PackFile::PackFile() : 
	m_hPakFile(0),
	m_bConst(true)
{
	memset(&m_fileHead, 0, sizeof(FILE_HEAD));
	memset(m_hashTable, 0, sizeof(FILE_HASHTABLE));

	//创建线程共享锁
	::InitializeCriticalSection(&m_secFile);
}

PackFile::~PackFile()
{
	closePakFile();

	//清空共享锁
	::DeleteCriticalSection(&m_secFile);
}

void PackFile::closePakFile(void)
{
	//清空所有打开的文件
	while(!m_listStream.empty())
	{
		(*m_listStream.begin())->close();
	}

	//remove edit flag
	if(m_fileHead.nEditFlag != 0)
	{
		m_fileHead.nEditFlag = 0;
		writeFileHead();
	}

	//Close handle
	if(m_hPakFile)
	{
		FlushFileBuffers(m_hPakFile);
		CloseHandle(m_hPakFile); m_hPakFile = 0;
	}

	//清空Stream打开的文件句柄
	FileHandleMap::iterator it;
	for(it=m_mapFileHandle.begin(); it!=m_mapFileHandle.end(); it++)
	{
		::CloseHandle(it->second);
	}
	m_mapFileHandle.clear();

	//清空表
	memset(&m_fileHead, 0, sizeof(FILE_HEAD));
	memset(m_hashTable, 0, sizeof(FILE_HASHNODE)*HASH_TABLE_SIZE);
	m_blockTable.clear();
	m_mapFreeBlock.clear();
	m_bConst = true;
}

//创建一个新的pak文件
bool PackFile::createNewPakFile(const char* szPackFileName)
{
	assert(szPackFileName);
	if(!szPackFileName || szPackFileName[0] == 0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//关闭旧文件
	closePakFile();

	//生成数据
	m_strFileName = szPackFileName;
	m_bConst = false;

	//生成文件
	m_hPakFile = ::CreateFile( szPackFileName, 
						GENERIC_READ|GENERIC_WRITE,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						0,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_ARCHIVE,
						0);

	if(m_hPakFile == INVALID_HANDLE_VALUE)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "WinErr=%d", ::GetLastError());
		return false;
	}

	// Fill Head
	m_fileHead.nIdentity	= AXPK_FILE_FLAG;
	m_fileHead.nVersion		= 1<<16|2;
	m_fileHead.nEditFlag	= 1;

	m_fileHead.nHashTable_Offset	= sizeof(FILE_HEAD);

	m_fileHead.nBlockTable_Offset	= sizeof(FILE_HEAD)+sizeof(FILE_HASHTABLE);
	m_fileHead.nBlockTable_Count	= 0;
	m_fileHead.nBlockTable_MaxSize	= BLOCK_TABLE_MAXSIZE;

	m_fileHead.nData_Offset			= sizeof(FILE_HEAD)+sizeof(FILE_HASHTABLE)+BLOCK_TABLE_MAXSIZE;
	m_fileHead.nData_Size			= 0;
	m_fileHead.nData_HoleSize		= 0;

	DWORD dwWriteBytes=0;

	//Write Head
	if(!WriteFile(m_hPakFile, &m_fileHead, sizeof(FILE_HEAD), &dwWriteBytes, 0) ||
		dwWriteBytes != sizeof(FILE_HEAD))
	{
		setLastError(AXP_ERR_FILE_WRITE, "WinErr=%d", ::GetLastError());
		return false;
	}

	//Write HashTable
	if(!WriteFile(m_hPakFile, m_hashTable, sizeof(FILE_HASHTABLE), &dwWriteBytes, 0) ||
		dwWriteBytes != sizeof(FILE_HASHTABLE))
	{
		setLastError(AXP_ERR_FILE_WRITE, "WinErr=%d", ::GetLastError());
		return false;
	}

	//Write BlockTable
	char* pTempBuf = new char[BLOCK_TABLE_MAXSIZE];
	memset(pTempBuf, 0, BLOCK_TABLE_MAXSIZE);

	if(!WriteFile(m_hPakFile, pTempBuf, BLOCK_TABLE_MAXSIZE, &dwWriteBytes, 0) ||
		dwWriteBytes != BLOCK_TABLE_MAXSIZE)
	{
		delete[] pTempBuf; pTempBuf=0;
		setLastError(AXP_ERR_FILE_WRITE, "WinErr=%d", ::GetLastError());
		return false;
	}
	delete[] pTempBuf; pTempBuf=0;


	return true;
}

//打开一个存在的pak文件
bool PackFile::openPakFile(const char* szPackFileName, bool bConst)
{
	assert(szPackFileName);
	if(!szPackFileName || szPackFileName[0] == 0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//关闭旧文件
	closePakFile();	

	//生成数据
	m_strFileName = szPackFileName;
	m_bConst = bConst;

	//打开文件
	m_hPakFile = ::CreateFile(szPackFileName, 
						bConst ? GENERIC_READ : GENERIC_READ|GENERIC_WRITE,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						0,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN,
						0);

	if(m_hPakFile == INVALID_HANDLE_VALUE)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "File=%s, WinErr=%d", szPackFileName, ::GetLastError());
		return false;
	}

	//Read Head
	DWORD dwReadBytes;
	if(!ReadFile(m_hPakFile, &m_fileHead, sizeof(FILE_HEAD), &dwReadBytes, 0) ||
		dwReadBytes != sizeof(FILE_HEAD))
	{
		setLastError(AXP_ERR_FILE_READ, "WinErr=%d", ::GetLastError());
		return false;
	}

	//以只读模式打开时，检查文件是否处于编辑状态
	if(m_bConst && m_fileHead.nEditFlag != 0)
	{
		setLastError(AXP_ERR_FILE_EDITMODE);
		return false;
	}

	//Read HashTable
	if(INVALID_SET_FILE_POINTER == 
			SetFilePointer(m_hPakFile, m_fileHead.nHashTable_Offset, 0, FILE_BEGIN))
	{
		setLastError(AXP_ERR_FILE_POINT, "file: %s, point=%d", 
			szPackFileName, m_fileHead.nHashTable_Offset);
		return false;
	}

	if(!ReadFile(m_hPakFile, m_hashTable, sizeof(FILE_HASHTABLE), &dwReadBytes, 0) ||
		dwReadBytes != sizeof(FILE_HASHTABLE))
	{
		setLastError(AXP_ERR_FILE_READ, "WinErr=%d", ::GetLastError());
		return false;
	}

	//Read BlockTable
	if(INVALID_SET_FILE_POINTER == 
			SetFilePointer(m_hPakFile, m_fileHead.nBlockTable_Offset, 0, FILE_BEGIN))
	{
		setLastError(AXP_ERR_FILE_POINT, "file: %s, point=%d", 
			szPackFileName, m_fileHead.nBlockTable_Offset);
		return false;
	}

	m_blockTable.resize(m_fileHead.nBlockTable_Count);
	if(!ReadFile(m_hPakFile, &(m_blockTable[0]), sizeof(FILE_BLOCKNODE)*m_fileHead.nBlockTable_Count, &dwReadBytes, 0) ||
		dwReadBytes != sizeof(FILE_BLOCKNODE)*m_fileHead.nBlockTable_Count)
	{
		setLastError(AXP_ERR_FILE_READ, "WinErr=%d", ::GetLastError());
		return false;
	}

	//保存空闲块
	for(unsigned int i=0; i<(unsigned int)m_blockTable.size(); i++)
	{
		FILE_BLOCKNODE& blockNode = m_blockTable[i];
		if(blockNode.nDataOffset < m_fileHead.nData_Offset)
		{
			setLastError(AXP_ERR_FILE_FORMAT);
			return false;
		}

		if(!getBlockNodeUsed(blockNode))
		{
			m_mapFreeBlock.insert(std::make_pair(upBoundBlockSize(blockNode.nBlockSize), i));
		}
	}

	//如果以编辑模式打开，设上编辑标志
	if(!m_bConst)
	{
		m_fileHead.nEditFlag = 1;
		m_fileHead.nVersion	= 1<<16|2;
		writeFileHead();
		FlushFileBuffers(m_hPakFile);
	}

	return true;
}

//加入/更新一个磁盘文件到包中
bool PackFile::insertContents(const char* szContents, unsigned int nContentsLen, 
					const char* szFileInPak, AXP_CONTENTS sourceType, bool bSaveAtOnce)
{
	assert(szContents && szFileInPak);
	if(!szFileInPak || szFileInPak[0]==0 || !m_hPakFile)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	if(sourceType==AC_DISKFILE && (!szContents || szContents[0]==0))
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//只读模式不支持
	if(m_bConst)
	{
		setLastError(AXP_ERR_FILE_ACCESS);
		return false;
	}

	//察看文件是否存在
	if(sourceType==AC_DISKFILE && !PathFileExists(szContents))
	{
		setLastError(AXP_ERR_FILE_NOTFOUND, "%s", szContents);
		return false;
	}

	//在Hash表中分配位置
	int nHashPos = allocStringPosInHashTable(normaliseName(szFileInPak, true, true).c_str());
	if(nHashPos < 0)
	{
		setLastError(AXP_ERR_HASHFULL);
		return false;
	}

	FILE_HASHNODE& hashNode = m_hashTable[nHashPos];

	//获得文件大小
	unsigned int nFileSize = sourceType==AC_DISKFILE ? getDiskFileSize(szContents) : nContentsLen;

	//如果同名文件已经存在
	if(getHashNodeExists(hashNode))
	{
		//Block
		unsigned int nBlockIndex = getHashNodeBlockIndex(hashNode);
		FILE_BLOCKNODE& blockNode = m_blockTable[nBlockIndex];

		//如果尺寸接近
		if(upBoundBlockSize(blockNode.nBlockSize) == upBoundBlockSize(nFileSize))
		{
			//直接替换文件内容

			//###################################
			//!! 磁盘操作开始 保存文件
			//###################################
			if(sourceType==AC_DISKFILE)
			{
				if(!writeDiskFile(blockNode.nDataOffset, 
					upBoundBlockSize(blockNode.nBlockSize), szContents)) return false;
			}
			else
			{
				if(!writeMemory(blockNode.nDataOffset, 
					upBoundBlockSize(blockNode.nBlockSize), szContents, nContentsLen)) return false;
			}

			if(blockNode.nBlockSize != nFileSize)
			{
				//文件尺寸有差异，保存BlockNode数据
				blockNode.nBlockSize = nFileSize;
				if(!writeBlockNode(nBlockIndex)) return false;
			}
			// 确认写入磁盘
			if(bSaveAtOnce)
			{
				FlushFileBuffers(m_hPakFile);
			}
			//###################################
			//!!磁盘操作结束
			//###################################
		}
		//尺寸发生变化，需要删除旧空间，分配新空间
		else 
		{
			//旧块内容设置为不再使用
			setBlockNodeUsed(blockNode, false);

			//将旧块加入空闲块列表
			m_mapFreeBlock.insert(std::make_pair(upBoundBlockSize(blockNode.nBlockSize), nBlockIndex));
			m_fileHead.nData_HoleSize += upBoundBlockSize(blockNode.nBlockSize);

			//重新尝试分配空间
			std::pair< int, int > nNewBlockPair = allocFreeBlock(nFileSize);
			int nNewBlockIndex = nNewBlockPair.first;
			if(nNewBlockIndex < 0) 
			{
				setLastError(AXP_ERR_BLOCKFULL);	//Block表满了
				return false;
			}

			//设置Hash表数据
			setHashNodeExists(hashNode, true);
			setHashNodeBlockIndex(hashNode, nNewBlockIndex);

			//###################################
			//!! 磁盘操作开始 保存文件
			//###################################
			if(nNewBlockPair.second> 0)
			{
				//分割大空间时产生的副空间
				if(!writeBlockNode((unsigned int)(nNewBlockPair.second))) return false;
			}

			//保存文件内容
			if(sourceType==AC_DISKFILE)
			{
				if(!writeDiskFile(m_blockTable[nNewBlockIndex].nDataOffset, 
					upBoundBlockSize(m_blockTable[nNewBlockIndex].nBlockSize), szContents)) return false;
			}
			else
			{
				if(!writeMemory(m_blockTable[nNewBlockIndex].nDataOffset, 
					upBoundBlockSize(m_blockTable[nNewBlockIndex].nBlockSize), 
					szContents, nContentsLen)) return false;
			}

			//文件所在的新的BlockNode
			if(!writeBlockNode((unsigned int)nNewBlockIndex)) return false;
			//原文件所在的BlockNode
			if(nBlockIndex != nNewBlockIndex)
			{
				if(!writeBlockNode((unsigned int)nBlockIndex)) return false;
			}
			//文件的HashNode
			if(!writeHashNode((unsigned int)nHashPos)) return false;
			//文件头
			if(!writeFileHead()) return false;

			// 确认写入磁盘
			if(bSaveAtOnce)
			{
				FlushFileBuffers(m_hPakFile);
			}
			//###################################
			//!!磁盘操作结束
			//###################################
		}
	}
	else
	{
		//获取一块合适空间
		std::pair< int, int > nNewBlockPair = allocFreeBlock(nFileSize);
		int nBlockIndex = nNewBlockPair.first;
		if(nBlockIndex < 0) 
		{
			setLastError(AXP_ERR_BLOCKFULL);	//Block表满了
			return false;
		}

		//设置Hash表数据
		setHashNodeBlockIndex(hashNode, nBlockIndex);
		setHashNodeExists(hashNode, true);
		
		//###################################
		//!! 磁盘操作开始 保存文件
		//###################################
		if(nNewBlockPair.second> 0)
		{
			//分割大空间时产生的副空间
			if(!writeBlockNode((unsigned int)(nNewBlockPair.second))) return false;
		}

		if(sourceType==AC_DISKFILE)
		{
			if(!writeDiskFile(m_blockTable[nBlockIndex].nDataOffset, 
				upBoundBlockSize(m_blockTable[nBlockIndex].nBlockSize), szContents)) return false;
		}
		else
		{
			if(!writeMemory(m_blockTable[nBlockIndex].nDataOffset, 
					upBoundBlockSize(m_blockTable[nBlockIndex].nBlockSize), 
					szContents, nContentsLen)) return false;
		}

		if(!writeBlockNode((unsigned int)nBlockIndex)) return false;
		if(!writeHashNode((unsigned int)nHashPos)) return false;
		if(!writeFileHead()) return false;

		// 确认写入磁盘
		if(bSaveAtOnce)
		{
			FlushFileBuffers(m_hPakFile);
		}
		//###################################
		//!!磁盘操作结束
		//###################################
	}

	return true;
}

bool PackFile::removeFile(const char* szFileInPak, bool bSaveAtOnce)
{
	assert(szFileInPak);
	if(!szFileInPak || szFileInPak[0]==0 || !m_hPakFile)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//只读模式不支持
	if(m_bConst)
	{
		setLastError(AXP_ERR_FILE_ACCESS);
		return false;
	}

	//在hash表中搜索
	int nHashPos = getStringPosInHashTable(normaliseName(szFileInPak, true, true).c_str());
	if(nHashPos < 0)
	{
		setLastError(AXP_ERR_FILE_NOTFOUND, "%s", szFileInPak);
		return false;
	}

	FILE_HASHNODE& hashNode = m_hashTable[nHashPos];

	//Block Node
	unsigned int nBlockPos = getHashNodeBlockIndex(hashNode);
	FILE_BLOCKNODE& blockNode = m_blockTable[nBlockPos];
	
	//设定值
	setBlockNodeUsed(blockNode, false);
	setHashNodeExists(hashNode, false);

	//保存空闲快
	m_mapFreeBlock.insert(std::make_pair(upBoundBlockSize(blockNode.nBlockSize), nBlockPos));
	m_fileHead.nData_HoleSize += upBoundBlockSize(blockNode.nBlockSize);

	//###################################
	//!! 磁盘操作开始 保存文件
	//###################################
	if(!writeBlockNode((unsigned int)nBlockPos)) return false;
	if(!writeHashNode((unsigned int)nHashPos)) return false;
	if(!writeFileHead()) return false;

	if(bSaveAtOnce)
	{
		FlushFileBuffers(m_hPakFile);
	}
	//###################################
	//!!磁盘操作结束
	//###################################

	return true;
}

std::pair< int, int > PackFile::allocFreeBlock(unsigned int nSize)
{
	unsigned int nBoundFileSize = upBoundBlockSize(nSize);

	//找到可以用的空闲的块
	FreeBlockMap::iterator it = m_mapFreeBlock.lower_bound(nBoundFileSize);
	if(it != m_mapFreeBlock.end()) 
	{
		unsigned int nBoundBlockSize = it->first;

		int nRetIndex = it->second;	//本BlockIndex
		int nRetSecIndex = -1;		//如果需要分割,分割产生的副BlockIndex

		//从空闲列表中删除
		m_mapFreeBlock.erase(it);
		m_fileHead.nData_HoleSize -= nBoundBlockSize;

		if(nBoundBlockSize > nBoundFileSize) //如果过大
		{
			//需要分割比较大的块
			FILE_BLOCKNODE newBlock;
			newBlock.nBlockSize = nBoundBlockSize-nBoundFileSize; //新块的大小
			newBlock.nDataOffset = m_blockTable[nRetIndex].nDataOffset + nBoundFileSize; //剩余的空间
			newBlock.nFlags = 0; //清空标记

			setBlockNodeUsed(newBlock, false); //初始为Free
			m_blockTable.push_back(newBlock);

			//计数加1
			m_fileHead.nBlockTable_Count += 1;

			//加入空闲块
			m_mapFreeBlock.insert(std::make_pair(newBlock.nBlockSize, (unsigned int)(m_blockTable.size()-1)));
			m_fileHead.nData_HoleSize += newBlock.nBlockSize;

			nRetSecIndex = (int)m_blockTable.size()-1; 
		}

		//设置新的数据
		m_blockTable[nRetIndex].nBlockSize = nSize; //储存文件实际大小(bytes)
		setBlockNodeUsed(m_blockTable[nRetIndex], true); //设置为使用中

		return std::make_pair(nRetIndex, nRetSecIndex);
	}

	//没有合适的空闲快,分配新块
	FILE_BLOCKNODE newBlock;
	newBlock.nBlockSize = nSize; //储存文件实际大小(bytes)
	newBlock.nDataOffset = m_fileHead.nData_Offset + m_fileHead.nData_Size; //在文件尾分配新的空间
	newBlock.nFlags = 0;

	setBlockNodeUsed(newBlock, true); //设置为使用中
	m_blockTable.push_back(newBlock);

	//计数加1
	m_fileHead.nBlockTable_Count += 1;
	//数据区扩大
	m_fileHead.nData_Size += upBoundBlockSize(nSize);


	return std::make_pair((int)(m_blockTable.size()-1), -1);
}

bool PackFile::writeFileHead(void)
{
	if(0==m_hPakFile) 
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//只读模式不支持
	if(m_bConst)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "Const Mode");
		return false;
	}

	//定位指针
	if(INVALID_SET_FILE_POINTER == 
		SetFilePointer(m_hPakFile, 0, 0, FILE_BEGIN))
	{
		setLastError(AXP_ERR_FILE_POINT, "point=0");
		return false;
	}

	//写Block数据
	DWORD dwBytesWrite;
	if(!WriteFile(m_hPakFile, &m_fileHead, sizeof(FILE_HEAD), &dwBytesWrite, 0) || 
		dwBytesWrite != sizeof(FILE_HEAD))
	{
		setLastError(AXP_ERR_FILE_WRITE, "winErr: %d", GetLastError());
		return false;
	}

	return true;
}

//写Hash节点
bool PackFile::writeHashNode(unsigned int nHashIndex)
{
	assert(nHashIndex<HASH_TABLE_SIZE && m_hPakFile);
	if(nHashIndex>=HASH_TABLE_SIZE || 0 == m_hPakFile) 
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//只读模式不支持
	if(m_bConst)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "Const Mode");
		return false;
	}

	//定位指针
	if(INVALID_SET_FILE_POINTER == 
		SetFilePointer(m_hPakFile, 
		m_fileHead.nHashTable_Offset+nHashIndex*sizeof(FILE_HASHNODE), 
		0, FILE_BEGIN))
	{
		setLastError(AXP_ERR_FILE_POINT, "winErr=%d, point=%d", GetLastError(),
			m_fileHead.nHashTable_Offset+nHashIndex*sizeof(FILE_HASHNODE));
		return false;
	}

	//写Block数据
	DWORD dwBytesWrite;
	if(!WriteFile(m_hPakFile, &(m_hashTable[nHashIndex]), sizeof(FILE_HASHNODE), &dwBytesWrite, 0) || 
		dwBytesWrite != sizeof(FILE_HASHNODE))
	{
		setLastError(AXP_ERR_FILE_WRITE, "winErr: %d", GetLastError());
		return false;
	}

	return true;
}

//写Block节点
bool PackFile::writeBlockNode(unsigned int nBlockIndex)
{
	assert(nBlockIndex<(int)m_blockTable.size() && m_hPakFile);
	if(nBlockIndex>=(int)m_blockTable.size() || 0==m_hPakFile) 
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//只读模式不支持
	if(m_bConst)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "Const Mode");
		return false;
	}

	//定位指针
	if(INVALID_SET_FILE_POINTER == 
		SetFilePointer(m_hPakFile, 
		m_fileHead.nBlockTable_Offset+nBlockIndex*sizeof(FILE_BLOCKNODE), 
		0, FILE_BEGIN))
	{
		setLastError(AXP_ERR_FILE_POINT, "winErr=%d, point=%d", GetLastError(),
			m_fileHead.nBlockTable_Offset+nBlockIndex*sizeof(FILE_BLOCKNODE));
		return false;
	}

	//写Block数据
	DWORD dwBytesWrite;
	if(!WriteFile(m_hPakFile, &(m_blockTable[nBlockIndex]), sizeof(FILE_BLOCKNODE), &dwBytesWrite, 0) || 
		dwBytesWrite != sizeof(FILE_BLOCKNODE))
	{
		setLastError(AXP_ERR_FILE_WRITE, "winErr: %d", GetLastError());
		return false;
	}

	return true;
}

//将一个文件写入
bool PackFile::writeDiskFile(unsigned int nOffset, unsigned int nWriteSize, const char* szDiskFile)
{
	assert(szDiskFile);
	if(!szDiskFile || szDiskFile[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	if(0==m_hPakFile) 
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//只读模式不支持
	if(m_bConst)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "Const Mode");
		return false;
	}

	//定位指针
	if(INVALID_SET_FILE_POINTER == 
		SetFilePointer(m_hPakFile, nOffset, 0, FILE_BEGIN))
	{
		setLastError(AXP_ERR_FILE_POINT, "winErr=%d, point=%d", 
			GetLastError(), nOffset);
		return false;
	}

	//打开源数据文件
	HANDLE hDataFile = CreateFile(szDiskFile,
		GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | 
		FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);
	if(hDataFile == INVALID_HANDLE_VALUE)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "Open file: %s, WinErr=%d", szDiskFile, ::GetLastError());
		return false;
	}

	const int MAX_BUFFER_SIZE = 4096;

	BYTE buffer[MAX_BUFFER_SIZE] = {0};
	DWORD dwBytesRead;
	BOOL bReadSuccess = ReadFile(hDataFile, buffer, sizeof(buffer), &dwBytesRead, 0);

	//实际已经写入的尺寸
	unsigned int nActWriteSize = 0;

	do
	{
		//文件结束
		if(dwBytesRead==0 || (!bReadSuccess && ERROR_HANDLE_EOF==GetLastError())) break;

		if(!bReadSuccess)
		{
			setLastError(AXP_ERR_FILE_READ, "file: %s", szDiskFile);
			CloseHandle(hDataFile);
			return false;
		}

		//超过容量
		assert(nActWriteSize+dwBytesRead <= nWriteSize);
		if(nActWriteSize+dwBytesRead > nWriteSize)
		{
			dwBytesRead = nWriteSize-nActWriteSize;
		}

		DWORD dwBytesWrite;
		if(!WriteFile(m_hPakFile, buffer, dwBytesRead, &dwBytesWrite, 0) || 
			dwBytesRead != dwBytesWrite)
		{
			setLastError(AXP_ERR_FILE_WRITE, "winErr: %d", GetLastError());
			CloseHandle(hDataFile);
			return false;
		}

		nActWriteSize += dwBytesWrite;
		if(nActWriteSize >= nWriteSize)
		{
			break;
		}

		bReadSuccess = ReadFile(hDataFile, buffer, sizeof(buffer), &dwBytesRead, 0);
	}while(true);

	//关闭源文件
	CloseHandle(hDataFile); hDataFile=0;

	//如果写入的尺寸长度不够，用0补齐
	while(nActWriteSize < nWriteSize)
	{
		int nThisWrite = min(MAX_BUFFER_SIZE, nWriteSize-nActWriteSize);
		assert(nThisWrite <= BLOCK_NODE_GSIZE);
		memset(buffer, 0, sizeof(buffer));

		DWORD dwBytesWrite;
		if(!WriteFile(m_hPakFile, buffer, nThisWrite, &dwBytesWrite, 0) || 
			nThisWrite != dwBytesWrite)
		{
			setLastError(AXP_ERR_FILE_WRITE, "winErr: %d", GetLastError());
			return false;
		}

		nActWriteSize += dwBytesWrite;
	}

	return true;
}

//将一段内存作为文件写入
bool PackFile::writeMemory(unsigned int nOffset, unsigned int nWriteSize, 
					const char* szMemory, unsigned int nMemorySize)
{
	assert(szMemory);
	if(!szMemory)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	if(0==m_hPakFile) 
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//只读模式不支持
	if(m_bConst)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "Const Mode");
		return false;
	}

	//定位指针
	if(INVALID_SET_FILE_POINTER == 
		SetFilePointer(m_hPakFile, nOffset, 0, FILE_BEGIN))
	{
		setLastError(AXP_ERR_FILE_POINT, "winErr=%d, point=%d", 
			GetLastError(), nOffset);
		return false;
	}

	//超过容量
	assert(nMemorySize <= nWriteSize);
	if(nMemorySize > nWriteSize)
	{
		nMemorySize = nWriteSize;
	}

	DWORD dwBytesWrite;
	if(!WriteFile(m_hPakFile, szMemory, nMemorySize, &dwBytesWrite, 0) || 
		dwBytesWrite != nMemorySize)
	{
		setLastError(AXP_ERR_FILE_WRITE, "winErr: %d", GetLastError());
		return false;
	}

	//如果写入的尺寸长度不够，用0补齐
	if(nMemorySize < nWriteSize)
	{
		char szTemp[BLOCK_NODE_GSIZE];
		memset(szTemp, 0, sizeof(szTemp));

		int nThisWrite = nWriteSize-nMemorySize;
		assert(nThisWrite <= BLOCK_NODE_GSIZE);

		DWORD dwBytesWrite;
		if(!WriteFile(m_hPakFile, szTemp, nThisWrite, &dwBytesWrite, 0) || 
			nThisWrite != dwBytesWrite)
		{
			setLastError(AXP_ERR_FILE_WRITE, "winErr: %d", GetLastError());
			return false;
		}
	}

	return true;
}

//文件自检功能
bool PackFile::selfCheck(SELFCHECK_CALLBACK callBack)
{
	if(0==m_hPakFile) 
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//编辑模式错误
	if(m_fileHead.nEditFlag != 0)
	{
		setLastError(AXP_ERR_FILE_EDITMODE);
		return false;
	}

	//打开List文件
	IStream* pListStream = openFile(LIST_FILENAME);
	if(!pListStream) return false;

	//skip first line
	pListStream->skipLine();

	//read second line
	char szTempLine[MAX_PATH*4] = {0};
	int nLineSize = pListStream->readLine(szTempLine, MAX_PATH*4);
	int nFileCount = atoi(szTempLine);

	//记录已经检查过的HashNode索引
	std::set< unsigned int > setHashNode;
	//记录已经检查过的BlockNode索引
	std::set< unsigned int > setBlockNode;

	//逐行读文件
	int nFileListSize = 0;
	while (!pListStream->eof())
	{
		int nLineLength = pListStream->readLine(szTempLine, MAX_PATH*4);
		if(0 == nLineLength) 
		{
			pListStream->close();
			setLastError(AXP_ERR_FILE_FORMAT, "list file second line=0");
			return false;
		}

		//分析文件描述
		std::vector< std::string > vStringVec;
		convertStringToVector(szTempLine, vStringVec, "|", true, false);
		if(vStringVec.size() != 3) 
		{
			pListStream->close();
			setLastError(AXP_ERR_FILE_FORMAT, "list file=%s", szTempLine);
			return false;
		}

		//获得文件信息
		std::string& strFileName = vStringVec[0];
		unsigned int nFileSize, nFileCRC;
		sscanf(vStringVec[1].c_str(), "%08X", &(nFileSize));
		sscanf(vStringVec[2].c_str(), "%08X", &(nFileCRC));

		//读入文件
		IStream* pFileStream = openFile(strFileName.c_str());	//打开文件
		if(!pFileStream)
		{
			pListStream->close();
			return false;
		}

		unsigned int nStreamSize = pFileStream->size();
		if(nStreamSize != nFileSize)
		{
			pFileStream->close();
			pListStream->close();
			setLastError(AXP_ERR_FILE_FORMAT, "file=%s, sizedif(%d!=%d)", 
				strFileName.c_str(), nStreamSize, nFileSize);
			return false;
		}

		char* pTempBuf = new char[nStreamSize];
		if(nStreamSize != pFileStream->read(pTempBuf, nStreamSize))
		{
			pFileStream->close();
			pListStream->close();
			setLastError(AXP_ERR_FILE_READ, "file=%s", strFileName.c_str());
		}
		pFileStream->close(); pFileStream=0;

		//计算文件实际crc
		unsigned int  nStreamCRC;
		crcMemory32(pTempBuf, nStreamSize, nStreamCRC);
		delete[] pTempBuf; pTempBuf=0;

		if(nStreamCRC != nFileCRC)
		{
//			pFileStream->close();
			pListStream->close();
			setLastError(AXP_ERR_FILE_FORMAT, "file=%s, crcdif(%d!=%d)", 
				strFileName.c_str(), nStreamCRC, nFileCRC);
			return false;
		}

		//保存分析过的文件Hash和Block数据

		//得到Hash数据
		int nHashPos = getStringPosInHashTable(normaliseName(strFileName).c_str());
		setHashNode.insert(nHashPos);

		//得到Block数据
		unsigned int nBlockIndex = getHashNodeBlockIndex(m_hashTable[nHashPos]);
		setBlockNode.insert(nBlockIndex);

		nFileListSize++;
	};
	pListStream->close(); pListStream=0;

	//文件个数检查
	if(nFileListSize != nFileCount)
	{
		setLastError(AXP_ERR_FILE_FORMAT, "file countdif(%d!=%d)", nFileListSize, nFileCount);
		return false;
	}

	//得到(list)的Hash数据
	int nListFileHashPos = getStringPosInHashTable(LIST_FILENAME);
	//得到(list)的Block数据
	unsigned int nListFileBlockIndex = getHashNodeBlockIndex(m_hashTable[nListFileHashPos]);

	//检查是否有未引用的Hash数据
	for(int i=0; i<HASH_TABLE_SIZE; i++)
	{
		const FILE_HASHNODE& hashNode = m_hashTable[i];
		if(!getHashNodeExists(hashNode)) continue;

		if(setHashNode.find(i) != setHashNode.end()) continue;
		if(i==nListFileHashPos) continue;

		//未引用的Hash数据
		setLastError(AXP_ERR_FILE_FORMAT, "unref hashnode(%d)", i);
		return false;
	}

	//检查是否有未引用的BlockNode数据
	for(int i=0; i<(int)m_blockTable.size(); i++)
	{
		const FILE_BLOCKNODE& blockNode = m_blockTable[i];
		if(!getBlockNodeUsed(blockNode)) continue;
		if(setBlockNode.find(i) != setBlockNode.end()) continue;
		if(i==nListFileBlockIndex) continue;

		//未引用的Hash数据
		setLastError(AXP_ERR_FILE_FORMAT, "unref blocknode(%d)", i);
		return false;
	}

	return true;
}


/************************************
			API接口
*************************************/

//创建一个Pak文件接口
IPakFile* createPakFile(void)
{
	return new PackFile;
}

//删除一个pak文件接口
void destroyPakFile(IPakFile* pFile)
{
	delete ((PackFile*)pFile);
}
	
}