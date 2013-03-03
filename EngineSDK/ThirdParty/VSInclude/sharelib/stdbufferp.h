#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*!	
	Author : dfhu
	Data: 2002-10
	Des: a slot memory pool
*/


#ifndef STD_LEN_BUFFER_POOL_H__
#define STD_LEN_BUFFER_POOL_H__


#define MAX_BUFFER_NUM_LENGTH 9600

#pragma warning ( disable : 4018 4244)

#pragma pack(push,4)


namespace SHARE_POOL
{
	struct StdLenBuffer
	{
		int m_nAllocateBufferNum;
		int m_nUsedBufferNum;
		int m_nOneBufferSize;
		char *m_pBuffer;
		CMyMutex m_mutexL;
		vector<char *> m_OtherBufferArray;

		struct NodeChar
		{
			NodeChar *pNext;
			int nUseNum;
			char *pData;
		};
	#define NodeCharMemoSize sizeof(NodeChar)

		typedef NodeChar * PNodeChar;
	#define PNodeCharMemSize sizeof(NodeChar *)

		PNodeChar m_pFreeList;

	private:
		NodeChar * _ToNode(char *buf)
		{
			return (PNodeChar)buf;
		}

		char * _ToChar(PNodeChar pNode)
		{
			return (char *)pNode;
		}

		NodeChar * _GetNode(int nLie)
		{
			return _ToNode(m_pBuffer+nLie*(m_nOneBufferSize+NodeCharMemoSize));
		}

		int _GetLieFromBuffer(char *buffer)
		{
			int nAll = (buffer-m_pBuffer);
			return nAll/(NodeCharMemoSize+m_nOneBufferSize);
		}

		PNodeChar _GetNodeFromBuffer(char *buffer)
		{
			return (PNodeChar)(buffer-NodeCharMemoSize);
		}

		PNodeChar BeginNode()
		{
			return ((PNodeChar)m_pBuffer);
		}

		bool NodeInBuffer(PNodeChar newNode)
		{
			return ((_ToChar(newNode)<(m_pBuffer+m_nAllocateBufferNum*(m_nOneBufferSize+NodeCharMemoSize)))&&(_ToChar(newNode)>=m_pBuffer));
		}

		bool BufferInPool(char *pBuffer)
		{
			return ((pBuffer<(m_pBuffer+m_nAllocateBufferNum*(m_nOneBufferSize+NodeCharMemoSize)))&&(pBuffer>=m_pBuffer));
		}

		void AddToFreeList(PNodeChar node)
		{
			node->pNext = m_pFreeList;
			m_pFreeList = node;
		}

		char *_PtrFromNode(PNodeChar node)
		{
			return ((char *)(node)+NodeCharMemoSize);
		}
	public:

		StdLenBuffer()
		{
			m_nAllocateBufferNum=m_nUsedBufferNum=m_nOneBufferSize=0;
			m_pBuffer=NULL;m_pFreeList=NULL;
		}

		StdLenBuffer( int nOneBufferSize,int nBufferNum )
		{
			m_nAllocateBufferNum=m_nUsedBufferNum=m_nOneBufferSize=0;
			m_pBuffer=NULL;m_pFreeList=NULL;
			InitBuffer( nOneBufferSize, nBufferNum );
		}

		int InitBuffer(int nOneBufferSize,int nBufferNum)
		{
			m_pBuffer=new char[(nOneBufferSize+NodeCharMemoSize)*nBufferNum];
			m_nOneBufferSize=nOneBufferSize;
			m_nAllocateBufferNum=nBufferNum;
			m_nUsedBufferNum=0;
			m_OtherBufferArray.resize(MAX_BUFFER_NUM_LENGTH-nBufferNum,NULL);

			NodeChar *pNode=NULL;
			for(int i=0;i<nBufferNum;i++)
			{
				pNode = _GetNode(i);
				pNode->pData = _PtrFromNode(pNode);
				pNode->nUseNum = 0;
				if(i<nBufferNum-1)
					pNode->pNext = _GetNode(i+1);
				else
					pNode->pNext = NULL;
			}
			m_pFreeList = (PNodeChar)m_pBuffer;
			return m_nAllocateBufferNum;
		}

		~StdLenBuffer()
		{
			Clear();
		}

		void Clear()
		{
			m_mutexL.Lock();
			m_nAllocateBufferNum=0;
			m_nUsedBufferNum=0;
			delete []m_pBuffer;
			m_pBuffer=NULL;
			for(int i=0;i<m_OtherBufferArray.size();i++)
			{
				delete [](m_OtherBufferArray[i]);
				m_OtherBufferArray[i]=NULL;
			}
			m_OtherBufferArray.clear();
			m_mutexL.Unlock();
		}

		char *GetBuffer()
		{
			char *pGBuffer=NULL;
			m_mutexL.Lock();
			if(m_pFreeList)
			{
				PNodeChar pGet = m_pFreeList;
				m_pFreeList = m_pFreeList->pNext;
				pGBuffer = pGet->pData;
				++pGet->nUseNum;
			}
			else //if(m_nUsedBufferNum<m_nAllocateBufferNum)
			{
				for(int i=0;i<m_OtherBufferArray.size();i++)
				{	
					pGBuffer = m_OtherBufferArray[i];
					if(pGBuffer == NULL)
					{
						pGBuffer = new char [m_nOneBufferSize];
						m_OtherBufferArray[i] = pGBuffer;
						break;
					}
					pGBuffer = NULL;
				}
				if( pGBuffer == NULL )
				{
					pGBuffer = new char[m_nOneBufferSize];
					m_OtherBufferArray.push_back( pGBuffer );
				}
				m_nUsedBufferNum++;
			}
			m_mutexL.Unlock();
			return pGBuffer;
		}

		void FreeBuffer(char *pBuffer)
		{
			m_mutexL.Lock();
			if(BufferInPool(pBuffer))
			{
				PNodeChar pNode = _GetNodeFromBuffer(pBuffer);
			//	if(NodeInBuffer(pNode))
			//	{
				--pNode->nUseNum;
				if(pNode->nUseNum == 0)
					AddToFreeList(pNode);
				else
				{
					OUT_LOG("Error occur when use memory\r\n");
					pNode->nUseNum = 0;
				//	MessageBox(NULL,"warning","memfree bug",MB_OK);
				//	ASSERT(FALSE);
				}
			
			//	}
			}
			else
			{
				char *tmpBuffer;
				for(int i=0;i<m_OtherBufferArray.size();i++)
				{
					tmpBuffer=m_OtherBufferArray[i];
					if(tmpBuffer==pBuffer)
					{
						m_OtherBufferArray[i]=NULL;
						delete []pBuffer;
						break;
					}
				}
			}
			m_mutexL.Unlock();
		}
	};

	/*
	#include <bitset>
	using namespace std;

	struct StdLenBuffer
	{
		bitset<MAX_BUFFER_NUM_LENGTH> m_bitMemoUse;
		int m_nAllocateBufferNum;
		int m_nUsedBufferNum;
		int m_nOneBufferSize;
		char *m_pBuffer;
		CMyMutex m_mutexL;
		vector<char *> m_OtherBufferArray;

		StdLenBuffer()
		{
			m_nAllocateBufferNum=m_nUsedBufferNum=m_nOneBufferSize=0;
			m_pBuffer=NULL;
		}

		int InitBuffer(int nOneBufferSize,int nBufferNum)
		{
			m_pBuffer=new char[nOneBufferSize*nBufferNum];
			m_nOneBufferSize=nOneBufferSize;
			m_nAllocateBufferNum=nBufferNum;
			m_nUsedBufferNum=0;
			m_OtherBufferArray.resize(MAX_BUFFER_NUM_LENGTH-nBufferNum,NULL);
			for(int i=0;i<MAX_BUFFER_NUM_LENGTH;i++)
				m_bitMemoUse.set(i,false);
			return m_nAllocateBufferNum;
		}

		~StdLenBuffer()
		{
			Clear();
		}

		void Clear()
		{
			m_mutexL.Lock();
			m_nAllocateBufferNum=0;
			m_nUsedBufferNum=0;
			delete []m_pBuffer;
			m_pBuffer=NULL;
			for(int i=0;i<m_OtherBufferArray.size();i++)
			{
				delete [](m_OtherBufferArray[i]);
				m_OtherBufferArray[i]=NULL;
			}
			m_OtherBufferArray.clear();
			m_mutexL.Unlock();
		}

		char *GetBuffer()
		{
			char *pGBuffer=NULL;
			m_mutexL.Lock();
			if(m_nUsedBufferNum>=m_nAllocateBufferNum)
			{
				char *pTmp=NULL;
				for(int i=0;i<m_OtherBufferArray.size();i++)
				{
					pTmp=m_OtherBufferArray[i];
					if(pTmp==NULL)
					{
						pTmp=new char[m_nOneBufferSize];
						m_OtherBufferArray[i]=pTmp;
						pGBuffer=pTmp;
						break;
					}
				}
			}
			else //if(m_nUsedBufferNum<m_nAllocateBufferNum)
			{
				for(int i=0;i<m_nAllocateBufferNum;i++)
				{
					if(!m_bitMemoUse.at(i))
					{
						pGBuffer=m_nOneBufferSize*i+m_pBuffer;
						m_bitMemoUse.set(i,true);
						break;
					}
				}
				m_nUsedBufferNum++;
			}
			m_mutexL.Unlock();
			return pGBuffer;
		}

		void FreeBuffer(char *pBuffer)
		{
			m_mutexL.Lock();
			int iLie=(pBuffer-m_pBuffer)/m_nOneBufferSize;
			if(iLie>-1&&iLie<m_nAllocateBufferNum)
			{
				m_bitMemoUse.set(iLie,false);
				m_nUsedBufferNum--;
			}
			else
			{
				char *tmpBuffer;
				for(int i=0;i<m_OtherBufferArray.size();i++)
				{
					tmpBuffer=m_OtherBufferArray[i];
					if(tmpBuffer==pBuffer)
					{
						m_OtherBufferArray[i]=NULL;
						delete []pBuffer;
						break;
					}
				}
			}
			m_mutexL.Unlock();
		}
	};
	*/
}
#pragma pack(pop)

#pragma warning ( default : 4018 4244)
#endif


