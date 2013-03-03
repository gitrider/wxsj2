#ifndef _SMU_MANAGER_H_
#define _SMU_MANAGER_H_

#include "ShareMemAO.h"
#include "Log.h"

#define MAX_MANAGER_SMU		5000
#define MAX_SM_OBJ_NUM		20

enum SMPOOL_TYPE
{
	SMPT_SHAREMEM,
	SMPT_SERVER,
	SMPT_WORLD
};

//ShareMemory ��Ԫ��
template<typename T>

class SMUPool
{

	
public:
	SMUPool()
	{
		__ENTER_FUNCTION
		m_pRefObjPtr	= NULL;	
		m_hObj			= NULL;
		m_nMaxSize		= -1;
		m_nPosition		= -1;
		__LEAVE_FUNCTION
	}
	~SMUPool()
	{
		__ENTER_FUNCTION
		
			SAFE_DELETE(m_pRefObjPtr);
			SAFE_DELETE_ARRAY(m_hObj);

		__LEAVE_FUNCTION
	}


	BOOL Init( uint nMaxCount,SM_KEY key,SMPOOL_TYPE SMPT,CHAR* LogPath)
	{
		__ENTER_FUNCTION

		
		m_pRefObjPtr = new ShareMemAO();
		Assert(m_pRefObjPtr);

		if(!m_pRefObjPtr)
			return FALSE;

		m_pRefObjPtr->InitLog(LogPath);
		m_pRefObjPtr->m_CmdArg = g_CmdArgv;

		BOOL ret;
		ret = m_pRefObjPtr->Attach(key,sizeof(T)*nMaxCount+sizeof(SMHead));

		if(SMPT == SMPT_SHAREMEM)
		{
			if(!ret)
			{
				ret = m_pRefObjPtr->Create(key,sizeof(T)*nMaxCount+sizeof(SMHead));
				
			}
		}
		else
		{
			if(!ret)
			{
				return FALSE;
			}
		}
		
		if(!ret)
		{
			if(m_pRefObjPtr->m_CmdArg == CMD_MODE_CLEARALL)
			{
				return TRUE;
			}

			Log::SaveLog(SHMEM_LOG_PATH,"Setup SMU block fail!");
			Assert(ret);
			return ret;

		}

		m_nMaxSize		= nMaxCount;
		m_nPosition		= 0;
		m_hObj			= new T* [m_nMaxSize];

		INT i;
		for( i = 0; i < m_nMaxSize; i++ )
		{
			m_hObj[i] = reinterpret_cast<T*>(m_pRefObjPtr->GetTypePtr(sizeof(T),i));
			if ( m_hObj[i] == NULL )
			{
				Assert( m_hObj[i] != NULL );
				return FALSE;
			}
		}

		m_key = key;

		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}

	BOOL Finalize()
	{
		__ENTER_FUNCTION
			
		Assert(m_pRefObjPtr);
		m_pRefObjPtr->Destory();
		return TRUE;

		__LEAVE_FUNCTION

		return FALSE;

	}

	T* NewObj( VOID )
	{
		__ENTER_FUNCTION

		Assert( m_nPosition < m_nMaxSize );
		if ( m_nPosition >= m_nMaxSize )
		{
			return NULL;
		}

		T *pObj = m_hObj[m_nPosition];
		pObj->SetPoolID( (UINT)m_nPosition );
		m_nPosition++;
		return pObj;
		__LEAVE_FUNCTION
		return NULL;
	}

	VOID DeleteObj( T *pObj )
	{
		__ENTER_FUNCTION

		Assert( pObj != NULL );	
		if ( pObj == NULL )
		{
			return ;
		}
		Assert( m_nPosition > 0 );	
		if ( m_nPosition <= 0 )
		{
			return ;
		}
		UINT uDelIndex = pObj->GetPoolID();
		Assert(uDelIndex < (UINT)m_nPosition );
		if (uDelIndex >= (UINT)m_nPosition )
		{
			return ;
		}
		m_nPosition--;
		T *pDelObj			= m_hObj[uDelIndex];
		m_hObj[uDelIndex]	= m_hObj[m_nPosition];
		m_hObj[m_nPosition]	= pDelObj;

		m_hObj[uDelIndex]->SetPoolID(uDelIndex );
		m_hObj[m_nPosition]->SetPoolID( INVALID_ID );
		__LEAVE_FUNCTION
	}

	T*				GetPoolObj(INT iIndex)
	{
		Assert(iIndex<m_nMaxSize);
		return m_hObj[iIndex];
	}

	INT				GetPoolMaxSize()
	{
		return m_nMaxSize;
	}

	INT				GetPoolSize()
	{
		return m_nPosition;
	}

	SM_KEY			GetKey()
	{
		return m_key;
	}

	BOOL			DumpToFile(CHAR* FilePath)
	{
		if(!m_pRefObjPtr)
		{
			Assert(m_pRefObjPtr);
			return FALSE;
		}

		return m_pRefObjPtr->DumpToFile(FilePath);
	}
	BOOL			MergeFromFile(CHAR* FilePath)			
	{
		if(!m_pRefObjPtr)
		{
			Assert(m_pRefObjPtr);
			return FALSE;
		}
		return m_pRefObjPtr->MergeFromFile(FilePath);
	}

	UINT	GetHeadVer()
	{
		Assert(m_pRefObjPtr);
		return m_pRefObjPtr->GetHeadVer();
	}
	VOID	SetHeadVer(UINT ver)
	{
		Assert(m_pRefObjPtr);
		return m_pRefObjPtr->SetHeadVer(ver);
	}
	

private:
	T				**m_hObj;			//�������SMU����
	INT				m_nMaxSize;			//�������
	INT				m_nPosition;		//��ǰʹ��ƫ��
	ShareMemAO*		m_pRefObjPtr;		//����SMObject
	SM_KEY			m_key;				//��Ӧ��ShareMemory Key
	
};




template<typename T>

class SMUManager
{
	
	// smu ����
	T				*m_pSMU[MAX_MANAGER_SMU];
	// smu ����
	INT				m_nCount;

public:

	SMUManager():
	  m_nCount(0)
	{
		memset(m_pSMU,0,sizeof(T*)*MAX_MANAGER_SMU);
	}

	~SMUManager()
	{}
	
	//��ʼ������
	VOID		Init()
	{

		__ENTER_FUNCTION
		m_nCount	= 0;
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;

	}
	
	BOOL		HeartBeat(UINT uTime=0);

	/*
	 *	���һ��SMUPool ���������ݵ�������
	 */

	BOOL		AddSMU(T*		pSMU)
	{
		__ENTER_FUNCTION

		Assert( m_nCount < MAX_MANAGER_SMU );
		if( m_nCount >= MAX_MANAGER_SMU )
			return FALSE;

		m_pSMU[m_nCount] = pSMU;
		m_pSMU->SetSMUID( m_nCount );
		m_nCount ++;
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	/*
	 *	ɾ��һ��SMUPool ���������ݵ�������
	 */
	BOOL		DelSMU(T* pSMU)
	{
		__ENTER_FUNCTION

		UINT uID = pSMU->GetSMUID();
		
		Assert(uID < (UINT)m_nCount );
		if (uID >= (UINT)m_nCount )
			return FALSE;

		Assert( m_pSMU[uID] == pSMU  );
		if ( m_pSMU[uID] != pSMU )
			return FALSE;

		m_pSMU[uID] = m_pSMU[m_nCount-1];
		m_pSMU[uID]->SetSMUID(uID); //���¸ı�����
		pSMU->SetSMUID( INVALID_ID );
		m_nCount--;
		
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	/*
	 *	����SMUId ����SMU
	 */
	T*			GetSMU(SMUID_t	SMUId)
	{
		Assert(SMUId<m_nCount);
		if( SMUId>=m_nCount )
			return NULL ;
		return m_pSMU[SMUId];
		
	}

};

#endif