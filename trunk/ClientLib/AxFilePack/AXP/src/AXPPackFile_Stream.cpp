#include "StdAfx.h"
#include "AXPPackFile.h"
#include "AXPErrors.h"
#include "AXPUtil.h"

namespace AXP
{

//DataStream实例类
class DataStream : public IStream
{
public:
	enum { AXP_STREAM_TEMP_SIZE = 1024 };

	//Read the requisite number of bytes from the stream, 
	//stopping at the end of the file.
	virtual unsigned int __fastcall read(void* buf, unsigned int count)
	{
		assert(m_pPackFile);
		if(buf==0 || count==0 || m_nCurrentOffset>=m_nFileSize) return 0;

		register unsigned int readWant = (std::min)(count, (unsigned int)(m_nFileSize - m_nCurrentOffset));

		if(m_pPreReadMemory)
		{
			memcpy(buf, m_pPreReadMemory+m_nCurrentOffset, readWant);
			m_nCurrentOffset += readWant;
			return readWant;
		}
		else
		{
			if(INVALID_SET_FILE_POINTER == 
				SetFilePointer(m_hFileandle, m_nStartPosition+m_nCurrentOffset, 0, FILE_BEGIN))
			{
				setLastError(AXP_ERR_FILE_POINT, "pos=%d", m_nStartPosition+m_nCurrentOffset);
				return 0;
			}

			DWORD dwReadBytes;
			if(!ReadFile(m_hFileandle, buf, readWant, &dwReadBytes, 0) ||
				dwReadBytes != readWant)
			{
				setLastError(AXP_ERR_FILE_READ, "WinErr=%d", ::GetLastError());
				return 0;
			}
			m_nCurrentOffset += dwReadBytes;
			return dwReadBytes;
		}
	}

	//Get a single line from the stream.
	unsigned int readLine(char* retBuf, unsigned int maxCount)
	{
		assert(m_pPackFile);
		if(m_pPackFile==0 || maxCount==0 || m_nCurrentOffset>=m_nFileSize) return 0;
		
        char tmpBuf[AXP_STREAM_TEMP_SIZE];
        unsigned int chunkSize = min(maxCount, ( unsigned int)AXP_STREAM_TEMP_SIZE-1);
        unsigned int totalCount = 0;
        unsigned int readCount; 

		while(chunkSize && (readCount = read(tmpBuf, chunkSize)))
		{
            // Terminate
            tmpBuf[readCount] = '\0';

            // Find first delimiter
            unsigned int pos = (unsigned int)strcspn(tmpBuf, "\n");

			if (pos < readCount)
            {
                // Found terminator, reposition backwards
				m_nCurrentOffset -= readCount-(pos+1);
            }

            // Are we genuinely copying?
            if (retBuf)
            {
                memcpy(retBuf+totalCount, tmpBuf, pos);
            }
            totalCount += pos;

            if (pos < readCount)
            {
                // Trim off trailing CR if this was a CR/LF entry
                if (totalCount && tmpBuf[totalCount-1] == '\r')
                {
                    --totalCount;
                }
                // Found terminator, break out
                break;
            }

            // Adjust chunkSize for next time
            chunkSize = min(maxCount-totalCount, (unsigned int)AXP_STREAM_TEMP_SIZE-1);
        }

        // Terminate
		if(retBuf)
		{
			retBuf[totalCount] = '\0';
		}
		return totalCount;
	}

	//Skip a single line from the stream.
	unsigned int skipLine(void)
	{
		assert(m_pPackFile);
		if(m_pPackFile==0 || m_nCurrentOffset>=m_nFileSize) return 0;

        char tmpBuf[AXP_STREAM_TEMP_SIZE];
        unsigned int total = 0;
        unsigned int readCount;
        // Keep looping while not hitting delimiter
        while ((readCount = read(tmpBuf, AXP_STREAM_TEMP_SIZE-1)) != 0)
        {
            // Terminate string
            tmpBuf[readCount] = '\0';

            // Find first delimiter
            unsigned int pos = (unsigned int)strcspn(tmpBuf, "\n");

            if (pos < readCount)
            {
                // Found terminator, reposition backwards
                m_nCurrentOffset -= (readCount - (pos + 1));

                total += pos + 1;

                // break out
                break;
            }

            total += readCount;
        }

        return total;
	}

	//Repositions the read point to a specified byte.
	virtual void seek(unsigned int pos)
	{
		m_nCurrentOffset = pos;
	}

	//Returns the current byte offset from beginning 
	virtual unsigned int tell(void) const
	{
		return m_nCurrentOffset;
	}

	//Returns true if the stream has reached the end.
	virtual bool eof(void) const
	{
		return (m_nCurrentOffset>=m_nFileSize);
	}

	//	Return the crc checksum of this contents.
	virtual unsigned int crc(void)
	{
		//alread get.
		if(m_nCrc!=0xFFFFFFFF) return m_nCrc;

		//zero stream?
		if(size()==0) return m_nCrc=0;

		//keep old file point and move to head
		unsigned int nOldPos = tell();
		m_nCurrentOffset = 0;

		//prepair buf
		const unsigned int CRC_BUF_SIZE = 4096;
		char szStackBuf[CRC_BUF_SIZE] = {0};
		char* pBuf = (size()>=CRC_BUF_SIZE) ? new char[size()] : szStackBuf;

		//read to buf
		read(pBuf, size());

		//caculate crc
		crcMemory32(pBuf, size(), m_nCrc);

		//free temp memory
		if(size()>=CRC_BUF_SIZE) delete[] pBuf;

		m_nCurrentOffset = nOldPos;
		return m_nCrc;
	}

	//Returns the total size of the data to be read from the stream, 
	//or 0 if this is indeterminate for this stream. 
	virtual unsigned int size(void) const
	{
		return m_nFileSize;
	}

	//Close this file, can't call any function after now!
	virtual void close(void)
	{
		if(!m_pPackFile) return;
		PackFile* ptemp = m_pPackFile;

		m_pPackFile=0;
		m_hFileandle=0;
		m_nStartPosition=0;
		m_nCurrentOffset=0;
		m_nFileSize=0;

		ptemp->closeFile(this);
		// delete this...
	}

private:
	//构造/析构
	DataStream(PackFile* pPackFile, HANDLE hFileHandle, unsigned int nStartPos, unsigned int nFileSize, bool bPreRead) :
		m_pPackFile(pPackFile), 
		m_hFileandle(hFileHandle),
		m_nStartPosition(nStartPos),
		m_nFileSize(nFileSize),
		m_nCurrentOffset(0),
		m_nCrc(0xFFFFFFFF),
		m_pPreReadMemory(0)
	{
		assert(m_pPackFile && m_hFileandle);

		if(!bPreRead) return;

		m_pPreReadMemory = new unsigned char[m_nFileSize];
		if(!m_pPreReadMemory) return;

		//read total...
		if(INVALID_SET_FILE_POINTER == 
			SetFilePointer(m_hFileandle, m_nStartPosition, 0, FILE_BEGIN))
		{
			setLastError(AXP_ERR_FILE_POINT, "pos=%d", m_nStartPosition);
			delete[] m_pPreReadMemory;
			return;
		}

		DWORD dwReadBytes;
		if(!ReadFile(m_hFileandle, m_pPreReadMemory, m_nFileSize, &dwReadBytes, 0) ||
			dwReadBytes != m_nFileSize)
		{
			setLastError(AXP_ERR_FILE_READ, "WinErr=%d", ::GetLastError());
			delete[] m_pPreReadMemory;
			return;
		}
	}
	virtual ~DataStream()
	{
		if(m_pPreReadMemory)
		{
			delete[] m_pPreReadMemory;
			m_pPreReadMemory=0;
		}
	}

	friend IStream*	PackFile::openFile(const char* szFileName);
	friend void		PackFile::closeFile(IStream* pStream);

protected:
	///Pack file
	PackFile*		m_pPackFile;
	/// windows file handle
	HANDLE			m_hFileandle; 
	/// start offset when open
	unsigned int	m_nStartPosition;
	/// current read offset
	unsigned int	m_nCurrentOffset;
	/// file size
	unsigned int	m_nFileSize;
	/// crc check sum
	unsigned int	m_nCrc;
	/// preread memory buf
	unsigned char*	m_pPreReadMemory;
};

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

//以文件流方式打开包中的一个文件
IStream* PackFile::openFile(const char* szFileName)
{
	//进入关键段，考虑多线程访问
	AUTO_LOCK autoLock(&m_secFile);

	assert(szFileName && m_hPakFile);
	if(!szFileName || szFileName[0]==0 || m_hPakFile==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//在hash表中搜索
	int nHashPos = getStringPosInHashTable(normaliseName(szFileName, true, true).c_str());
	if(nHashPos < 0) 
	{
		setLastError(AXP_ERR_FILE_NOTFOUND, szFileName);
		return 0;
	}

	const FILE_HASHNODE& hashNode = m_hashTable[nHashPos];

	//得到Block数据
	unsigned int nBlockIndex = getHashNodeBlockIndex(hashNode);
	assert(nBlockIndex < (unsigned int)m_blockTable.size());

	const FILE_BLOCKNODE& blockNode = m_blockTable[nBlockIndex];

	//得到当前线程id
	unsigned int uCurrentThread = (unsigned int)::GetCurrentThreadId();
	FileHandleMap::iterator itHandle = m_mapFileHandle.find(uCurrentThread);	//是否已经有句柄
	if(itHandle == m_mapFileHandle.end())
	{
		//分配一个句柄
		HANDLE hFile = ::CreateFile(m_strFileName.c_str(), 
			GENERIC_READ,
			FILE_SHARE_READ|FILE_SHARE_WRITE,
			0,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN,
			0);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			setLastError(AXP_ERR_FILE_ACCESS, "File=%s, WinErr=%d", m_strFileName.c_str(), ::GetLastError());
			return 0;
		}

		//加入map
		m_mapFileHandle.insert(std::make_pair(uCurrentThread, hFile));
		itHandle = m_mapFileHandle.find(uCurrentThread);
		assert(itHandle != m_mapFileHandle.end());
	}

	//Alloc a datastream
	DataStream* pNewStream = new DataStream(this, 
		itHandle->second, blockNode.nDataOffset, blockNode.nBlockSize, 
		blockNode.nBlockSize<=STREAM_PREREAD_SIZE);
	m_listStream.push_back(pNewStream);

	return (IStream*)pNewStream;
}

//得到包中某个文件的大小
unsigned int PackFile::getFileSize(const char* szFileName) const
{
	assert(szFileName);
	if(!szFileName || szFileName[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return 0;
	}

	//在hash表中搜索
	int nHashPos = getStringPosInHashTable(normaliseName(szFileName, true, true).c_str());
	if(nHashPos < 0) 
	{
		setLastError(AXP_ERR_FILE_NOTFOUND, "%s", szFileName);
		return 0;
	}

	const FILE_HASHNODE& hashNode = m_hashTable[nHashPos];
	
	//得到Block数据
	unsigned int nBlockIndex = getHashNodeBlockIndex(hashNode);
	assert(nBlockIndex < (unsigned int)m_blockTable.size());

	const FILE_BLOCKNODE& blockNode = m_blockTable[nBlockIndex];

	return blockNode.nBlockSize;
}

//关闭数据流文件
void PackFile::closeFile(IStream* pStream)
{
	//进入关键段，考虑多线程访问
	AUTO_LOCK autoLock(&m_secFile);

	m_listStream.remove(pStream);
	delete ((DataStream*)pStream); pStream=0;
}

//某文件在包中是否存在
bool PackFile::isFileExists(const char* szFileName) const
{
	assert(szFileName);
	if(!szFileName || szFileName[0]==0)
	{
		setLastError(AXP_ERR_PARAM);
		return false;
	}

	//在hash表中搜索
	int nHashPos = getStringPosInHashTable(normaliseName(szFileName, true, true).c_str());

	return nHashPos >= 0;
}

}