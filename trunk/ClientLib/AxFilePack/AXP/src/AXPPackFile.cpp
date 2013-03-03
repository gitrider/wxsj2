#include "StdAfx.h"
#include "AXPPackFile.h"
#include "AXPErrors.h"
#include "AXPUtil.h"

namespace AXP
{

const unsigned int AXPK_FILE_FLAG	= 0X504B5A46;	// 'PKZF'
const char* PackFile::PAKFILE_EXT	= ".pkf";		// �ļ���չ��
const char* PackFile::LIST_FILENAME = "(lists)";		//List�ļ��ڰ��е��ļ���

//����/����
PackFile::PackFile() : 
	m_hPakFile(0),
	m_bConst(true)
{
	memset(&m_fileHead, 0, sizeof(FILE_HEAD));
	memset(m_hashTable, 0, sizeof(FILE_HASHTABLE));

	//�����̹߳�����
	::InitializeCriticalSection(&m_secFile);
}

PackFile::~PackFile()
{
	closePakFile();

	//��չ�����
	::DeleteCriticalSection(&m_secFile);
}

void PackFile::closePakFile(void)
{
	//������д򿪵��ļ�
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

	//���Stream�򿪵��ļ����
	FileHandleMap::iterator it;
	for(it=m_mapFileHandle.begin(); it!=m_mapFileHandle.end(); it++)
	{
		::CloseHandle(it->second);
	}
	m_mapFileHandle.clear();

	//��ձ�
	memset(&m_fileHead, 0, sizeof(FILE_HEAD));
	memset(m_hashTable, 0, sizeof(FILE_HASHNODE)*HASH_TABLE_SIZE);
	m_blockTable.clear();
	m_mapFreeBlock.clear();
	m_bConst = true;
}

//����һ���µ�pak�ļ�
bool PackFile::createNewPakFile(const char* szPackFileName)
{
	assert(szPackFileName);
	if(!szPackFileName || szPackFileName[0] == 0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//�رվ��ļ�
	closePakFile();

	//��������
	m_strFileName = szPackFileName;
	m_bConst = false;

	//�����ļ�
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

//��һ�����ڵ�pak�ļ�
bool PackFile::openPakFile(const char* szPackFileName, bool bConst)
{
	assert(szPackFileName);
	if(!szPackFileName || szPackFileName[0] == 0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//�رվ��ļ�
	closePakFile();	

	//��������
	m_strFileName = szPackFileName;
	m_bConst = bConst;

	//���ļ�
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

	//��ֻ��ģʽ��ʱ������ļ��Ƿ��ڱ༭״̬
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

	//������п�
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

	//����Ա༭ģʽ�򿪣����ϱ༭��־
	if(!m_bConst)
	{
		m_fileHead.nEditFlag = 1;
		m_fileHead.nVersion	= 1<<16|2;
		writeFileHead();
		FlushFileBuffers(m_hPakFile);
	}

	return true;
}

//����/����һ�������ļ�������
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

	//ֻ��ģʽ��֧��
	if(m_bConst)
	{
		setLastError(AXP_ERR_FILE_ACCESS);
		return false;
	}

	//�쿴�ļ��Ƿ����
	if(sourceType==AC_DISKFILE && !PathFileExists(szContents))
	{
		setLastError(AXP_ERR_FILE_NOTFOUND, "%s", szContents);
		return false;
	}

	//��Hash���з���λ��
	int nHashPos = allocStringPosInHashTable(normaliseName(szFileInPak, true, true).c_str());
	if(nHashPos < 0)
	{
		setLastError(AXP_ERR_HASHFULL);
		return false;
	}

	FILE_HASHNODE& hashNode = m_hashTable[nHashPos];

	//����ļ���С
	unsigned int nFileSize = sourceType==AC_DISKFILE ? getDiskFileSize(szContents) : nContentsLen;

	//���ͬ���ļ��Ѿ�����
	if(getHashNodeExists(hashNode))
	{
		//Block
		unsigned int nBlockIndex = getHashNodeBlockIndex(hashNode);
		FILE_BLOCKNODE& blockNode = m_blockTable[nBlockIndex];

		//����ߴ�ӽ�
		if(upBoundBlockSize(blockNode.nBlockSize) == upBoundBlockSize(nFileSize))
		{
			//ֱ���滻�ļ�����

			//###################################
			//!! ���̲�����ʼ �����ļ�
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
				//�ļ��ߴ��в��죬����BlockNode����
				blockNode.nBlockSize = nFileSize;
				if(!writeBlockNode(nBlockIndex)) return false;
			}
			// ȷ��д�����
			if(bSaveAtOnce)
			{
				FlushFileBuffers(m_hPakFile);
			}
			//###################################
			//!!���̲�������
			//###################################
		}
		//�ߴ緢���仯����Ҫɾ���ɿռ䣬�����¿ռ�
		else 
		{
			//�ɿ���������Ϊ����ʹ��
			setBlockNodeUsed(blockNode, false);

			//���ɿ������п��б�
			m_mapFreeBlock.insert(std::make_pair(upBoundBlockSize(blockNode.nBlockSize), nBlockIndex));
			m_fileHead.nData_HoleSize += upBoundBlockSize(blockNode.nBlockSize);

			//���³��Է���ռ�
			std::pair< int, int > nNewBlockPair = allocFreeBlock(nFileSize);
			int nNewBlockIndex = nNewBlockPair.first;
			if(nNewBlockIndex < 0) 
			{
				setLastError(AXP_ERR_BLOCKFULL);	//Block������
				return false;
			}

			//����Hash������
			setHashNodeExists(hashNode, true);
			setHashNodeBlockIndex(hashNode, nNewBlockIndex);

			//###################################
			//!! ���̲�����ʼ �����ļ�
			//###################################
			if(nNewBlockPair.second> 0)
			{
				//�ָ��ռ�ʱ�����ĸ��ռ�
				if(!writeBlockNode((unsigned int)(nNewBlockPair.second))) return false;
			}

			//�����ļ�����
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

			//�ļ����ڵ��µ�BlockNode
			if(!writeBlockNode((unsigned int)nNewBlockIndex)) return false;
			//ԭ�ļ����ڵ�BlockNode
			if(nBlockIndex != nNewBlockIndex)
			{
				if(!writeBlockNode((unsigned int)nBlockIndex)) return false;
			}
			//�ļ���HashNode
			if(!writeHashNode((unsigned int)nHashPos)) return false;
			//�ļ�ͷ
			if(!writeFileHead()) return false;

			// ȷ��д�����
			if(bSaveAtOnce)
			{
				FlushFileBuffers(m_hPakFile);
			}
			//###################################
			//!!���̲�������
			//###################################
		}
	}
	else
	{
		//��ȡһ����ʿռ�
		std::pair< int, int > nNewBlockPair = allocFreeBlock(nFileSize);
		int nBlockIndex = nNewBlockPair.first;
		if(nBlockIndex < 0) 
		{
			setLastError(AXP_ERR_BLOCKFULL);	//Block������
			return false;
		}

		//����Hash������
		setHashNodeBlockIndex(hashNode, nBlockIndex);
		setHashNodeExists(hashNode, true);
		
		//###################################
		//!! ���̲�����ʼ �����ļ�
		//###################################
		if(nNewBlockPair.second> 0)
		{
			//�ָ��ռ�ʱ�����ĸ��ռ�
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

		// ȷ��д�����
		if(bSaveAtOnce)
		{
			FlushFileBuffers(m_hPakFile);
		}
		//###################################
		//!!���̲�������
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

	//ֻ��ģʽ��֧��
	if(m_bConst)
	{
		setLastError(AXP_ERR_FILE_ACCESS);
		return false;
	}

	//��hash��������
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
	
	//�趨ֵ
	setBlockNodeUsed(blockNode, false);
	setHashNodeExists(hashNode, false);

	//������п�
	m_mapFreeBlock.insert(std::make_pair(upBoundBlockSize(blockNode.nBlockSize), nBlockPos));
	m_fileHead.nData_HoleSize += upBoundBlockSize(blockNode.nBlockSize);

	//###################################
	//!! ���̲�����ʼ �����ļ�
	//###################################
	if(!writeBlockNode((unsigned int)nBlockPos)) return false;
	if(!writeHashNode((unsigned int)nHashPos)) return false;
	if(!writeFileHead()) return false;

	if(bSaveAtOnce)
	{
		FlushFileBuffers(m_hPakFile);
	}
	//###################################
	//!!���̲�������
	//###################################

	return true;
}

std::pair< int, int > PackFile::allocFreeBlock(unsigned int nSize)
{
	unsigned int nBoundFileSize = upBoundBlockSize(nSize);

	//�ҵ������õĿ��еĿ�
	FreeBlockMap::iterator it = m_mapFreeBlock.lower_bound(nBoundFileSize);
	if(it != m_mapFreeBlock.end()) 
	{
		unsigned int nBoundBlockSize = it->first;

		int nRetIndex = it->second;	//��BlockIndex
		int nRetSecIndex = -1;		//�����Ҫ�ָ�,�ָ�����ĸ�BlockIndex

		//�ӿ����б���ɾ��
		m_mapFreeBlock.erase(it);
		m_fileHead.nData_HoleSize -= nBoundBlockSize;

		if(nBoundBlockSize > nBoundFileSize) //�������
		{
			//��Ҫ�ָ�Ƚϴ�Ŀ�
			FILE_BLOCKNODE newBlock;
			newBlock.nBlockSize = nBoundBlockSize-nBoundFileSize; //�¿�Ĵ�С
			newBlock.nDataOffset = m_blockTable[nRetIndex].nDataOffset + nBoundFileSize; //ʣ��Ŀռ�
			newBlock.nFlags = 0; //��ձ��

			setBlockNodeUsed(newBlock, false); //��ʼΪFree
			m_blockTable.push_back(newBlock);

			//������1
			m_fileHead.nBlockTable_Count += 1;

			//������п�
			m_mapFreeBlock.insert(std::make_pair(newBlock.nBlockSize, (unsigned int)(m_blockTable.size()-1)));
			m_fileHead.nData_HoleSize += newBlock.nBlockSize;

			nRetSecIndex = (int)m_blockTable.size()-1; 
		}

		//�����µ�����
		m_blockTable[nRetIndex].nBlockSize = nSize; //�����ļ�ʵ�ʴ�С(bytes)
		setBlockNodeUsed(m_blockTable[nRetIndex], true); //����Ϊʹ����

		return std::make_pair(nRetIndex, nRetSecIndex);
	}

	//û�к��ʵĿ��п�,�����¿�
	FILE_BLOCKNODE newBlock;
	newBlock.nBlockSize = nSize; //�����ļ�ʵ�ʴ�С(bytes)
	newBlock.nDataOffset = m_fileHead.nData_Offset + m_fileHead.nData_Size; //���ļ�β�����µĿռ�
	newBlock.nFlags = 0;

	setBlockNodeUsed(newBlock, true); //����Ϊʹ����
	m_blockTable.push_back(newBlock);

	//������1
	m_fileHead.nBlockTable_Count += 1;
	//����������
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

	//ֻ��ģʽ��֧��
	if(m_bConst)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "Const Mode");
		return false;
	}

	//��λָ��
	if(INVALID_SET_FILE_POINTER == 
		SetFilePointer(m_hPakFile, 0, 0, FILE_BEGIN))
	{
		setLastError(AXP_ERR_FILE_POINT, "point=0");
		return false;
	}

	//дBlock����
	DWORD dwBytesWrite;
	if(!WriteFile(m_hPakFile, &m_fileHead, sizeof(FILE_HEAD), &dwBytesWrite, 0) || 
		dwBytesWrite != sizeof(FILE_HEAD))
	{
		setLastError(AXP_ERR_FILE_WRITE, "winErr: %d", GetLastError());
		return false;
	}

	return true;
}

//дHash�ڵ�
bool PackFile::writeHashNode(unsigned int nHashIndex)
{
	assert(nHashIndex<HASH_TABLE_SIZE && m_hPakFile);
	if(nHashIndex>=HASH_TABLE_SIZE || 0 == m_hPakFile) 
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//ֻ��ģʽ��֧��
	if(m_bConst)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "Const Mode");
		return false;
	}

	//��λָ��
	if(INVALID_SET_FILE_POINTER == 
		SetFilePointer(m_hPakFile, 
		m_fileHead.nHashTable_Offset+nHashIndex*sizeof(FILE_HASHNODE), 
		0, FILE_BEGIN))
	{
		setLastError(AXP_ERR_FILE_POINT, "winErr=%d, point=%d", GetLastError(),
			m_fileHead.nHashTable_Offset+nHashIndex*sizeof(FILE_HASHNODE));
		return false;
	}

	//дBlock����
	DWORD dwBytesWrite;
	if(!WriteFile(m_hPakFile, &(m_hashTable[nHashIndex]), sizeof(FILE_HASHNODE), &dwBytesWrite, 0) || 
		dwBytesWrite != sizeof(FILE_HASHNODE))
	{
		setLastError(AXP_ERR_FILE_WRITE, "winErr: %d", GetLastError());
		return false;
	}

	return true;
}

//дBlock�ڵ�
bool PackFile::writeBlockNode(unsigned int nBlockIndex)
{
	assert(nBlockIndex<(int)m_blockTable.size() && m_hPakFile);
	if(nBlockIndex>=(int)m_blockTable.size() || 0==m_hPakFile) 
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//ֻ��ģʽ��֧��
	if(m_bConst)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "Const Mode");
		return false;
	}

	//��λָ��
	if(INVALID_SET_FILE_POINTER == 
		SetFilePointer(m_hPakFile, 
		m_fileHead.nBlockTable_Offset+nBlockIndex*sizeof(FILE_BLOCKNODE), 
		0, FILE_BEGIN))
	{
		setLastError(AXP_ERR_FILE_POINT, "winErr=%d, point=%d", GetLastError(),
			m_fileHead.nBlockTable_Offset+nBlockIndex*sizeof(FILE_BLOCKNODE));
		return false;
	}

	//дBlock����
	DWORD dwBytesWrite;
	if(!WriteFile(m_hPakFile, &(m_blockTable[nBlockIndex]), sizeof(FILE_BLOCKNODE), &dwBytesWrite, 0) || 
		dwBytesWrite != sizeof(FILE_BLOCKNODE))
	{
		setLastError(AXP_ERR_FILE_WRITE, "winErr: %d", GetLastError());
		return false;
	}

	return true;
}

//��һ���ļ�д��
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

	//ֻ��ģʽ��֧��
	if(m_bConst)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "Const Mode");
		return false;
	}

	//��λָ��
	if(INVALID_SET_FILE_POINTER == 
		SetFilePointer(m_hPakFile, nOffset, 0, FILE_BEGIN))
	{
		setLastError(AXP_ERR_FILE_POINT, "winErr=%d, point=%d", 
			GetLastError(), nOffset);
		return false;
	}

	//��Դ�����ļ�
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

	//ʵ���Ѿ�д��ĳߴ�
	unsigned int nActWriteSize = 0;

	do
	{
		//�ļ�����
		if(dwBytesRead==0 || (!bReadSuccess && ERROR_HANDLE_EOF==GetLastError())) break;

		if(!bReadSuccess)
		{
			setLastError(AXP_ERR_FILE_READ, "file: %s", szDiskFile);
			CloseHandle(hDataFile);
			return false;
		}

		//��������
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

	//�ر�Դ�ļ�
	CloseHandle(hDataFile); hDataFile=0;

	//���д��ĳߴ糤�Ȳ�������0����
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

//��һ���ڴ���Ϊ�ļ�д��
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

	//ֻ��ģʽ��֧��
	if(m_bConst)
	{
		setLastError(AXP_ERR_FILE_ACCESS, "Const Mode");
		return false;
	}

	//��λָ��
	if(INVALID_SET_FILE_POINTER == 
		SetFilePointer(m_hPakFile, nOffset, 0, FILE_BEGIN))
	{
		setLastError(AXP_ERR_FILE_POINT, "winErr=%d, point=%d", 
			GetLastError(), nOffset);
		return false;
	}

	//��������
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

	//���д��ĳߴ糤�Ȳ�������0����
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

//�ļ��Լ칦��
bool PackFile::selfCheck(SELFCHECK_CALLBACK callBack)
{
	if(0==m_hPakFile) 
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//�༭ģʽ����
	if(m_fileHead.nEditFlag != 0)
	{
		setLastError(AXP_ERR_FILE_EDITMODE);
		return false;
	}

	//��List�ļ�
	IStream* pListStream = openFile(LIST_FILENAME);
	if(!pListStream) return false;

	//skip first line
	pListStream->skipLine();

	//read second line
	char szTempLine[MAX_PATH*4] = {0};
	int nLineSize = pListStream->readLine(szTempLine, MAX_PATH*4);
	int nFileCount = atoi(szTempLine);

	//��¼�Ѿ�������HashNode����
	std::set< unsigned int > setHashNode;
	//��¼�Ѿ�������BlockNode����
	std::set< unsigned int > setBlockNode;

	//���ж��ļ�
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

		//�����ļ�����
		std::vector< std::string > vStringVec;
		convertStringToVector(szTempLine, vStringVec, "|", true, false);
		if(vStringVec.size() != 3) 
		{
			pListStream->close();
			setLastError(AXP_ERR_FILE_FORMAT, "list file=%s", szTempLine);
			return false;
		}

		//����ļ���Ϣ
		std::string& strFileName = vStringVec[0];
		unsigned int nFileSize, nFileCRC;
		sscanf(vStringVec[1].c_str(), "%08X", &(nFileSize));
		sscanf(vStringVec[2].c_str(), "%08X", &(nFileCRC));

		//�����ļ�
		IStream* pFileStream = openFile(strFileName.c_str());	//���ļ�
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

		//�����ļ�ʵ��crc
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

		//������������ļ�Hash��Block����

		//�õ�Hash����
		int nHashPos = getStringPosInHashTable(normaliseName(strFileName).c_str());
		setHashNode.insert(nHashPos);

		//�õ�Block����
		unsigned int nBlockIndex = getHashNodeBlockIndex(m_hashTable[nHashPos]);
		setBlockNode.insert(nBlockIndex);

		nFileListSize++;
	};
	pListStream->close(); pListStream=0;

	//�ļ��������
	if(nFileListSize != nFileCount)
	{
		setLastError(AXP_ERR_FILE_FORMAT, "file countdif(%d!=%d)", nFileListSize, nFileCount);
		return false;
	}

	//�õ�(list)��Hash����
	int nListFileHashPos = getStringPosInHashTable(LIST_FILENAME);
	//�õ�(list)��Block����
	unsigned int nListFileBlockIndex = getHashNodeBlockIndex(m_hashTable[nListFileHashPos]);

	//����Ƿ���δ���õ�Hash����
	for(int i=0; i<HASH_TABLE_SIZE; i++)
	{
		const FILE_HASHNODE& hashNode = m_hashTable[i];
		if(!getHashNodeExists(hashNode)) continue;

		if(setHashNode.find(i) != setHashNode.end()) continue;
		if(i==nListFileHashPos) continue;

		//δ���õ�Hash����
		setLastError(AXP_ERR_FILE_FORMAT, "unref hashnode(%d)", i);
		return false;
	}

	//����Ƿ���δ���õ�BlockNode����
	for(int i=0; i<(int)m_blockTable.size(); i++)
	{
		const FILE_BLOCKNODE& blockNode = m_blockTable[i];
		if(!getBlockNodeUsed(blockNode)) continue;
		if(setBlockNode.find(i) != setBlockNode.end()) continue;
		if(i==nListFileBlockIndex) continue;

		//δ���õ�Hash����
		setLastError(AXP_ERR_FILE_FORMAT, "unref blocknode(%d)", i);
		return false;
	}

	return true;
}


/************************************
			API�ӿ�
*************************************/

//����һ��Pak�ļ��ӿ�
IPakFile* createPakFile(void)
{
	return new PackFile;
}

//ɾ��һ��pak�ļ��ӿ�
void destroyPakFile(IPakFile* pFile)
{
	delete ((PackFile*)pFile);
}
	
}