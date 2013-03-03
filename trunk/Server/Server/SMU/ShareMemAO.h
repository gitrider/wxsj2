/********************************************************************
	��������:	2005��11��2��
	����ʱ��:	13:58
	�ļ�����:	ShareMemAO.h
	�ļ�·��:	d:\Prj\Server\Server\Smu\ShareMemAO.h
	������:		������
	
	�ļ����ܣ� �����ڴ���ʶ���Ķ���
	�޸ļ�¼��
	
*********************************************************************/

#ifndef _SHARE_MEM_ACCESS_OBJECT_H_
#define _SHARE_MEM_ACCESS_OBJECT_H_

#include "Type.h"
#include "DB_Struct.h"


#define		SHMEM_LOG_PATH	"./Log/DBShare.log"
/*
 *		�����ڴ���ʶ���
 *		ShareMemory	Access	Object
 */
class ShareMemAO
{

	
public:
	ShareMemAO()
	{
		m_pDataPtr	=	0;
		m_hold		=	0;
		m_Size		=	0;
		m_pHeader	=	0;	
		memset(m_LogPath,0,sizeof(m_LogPath));
	}
	~ShareMemAO(){};
	
	
	BOOL InitLog(CHAR* LogPtr)
	{
		__ENTER_FUNCTION

			Assert(LogPtr);
			strncpy(m_LogPath,LogPtr,255);
			
			return TRUE;
		__LEAVE_FUNCTION

		return FALSE;
	}
	
	
	
	/*
	 *	����ShareMem ���ʶ���(�´���)
	 *
	 *  SM_KEY	key		���ʼ�ֵ
	 *
	 *	uint		Size	�����������ֽڸ���
	 *
	 */
	BOOL	Create(SM_KEY key,uint	Size);
	/*
	 *	���ٶ���
	 */
	VOID	Destory();

	/*
	 *		����ShareMem ���ʶ���(�����´���)
	 *		SM_KEY	key		���ʼ�ֵ
	 *
	 *		uint		Size	�����������ֽڸ���
	 *		
	 */
	BOOL	Attach(SM_KEY,uint Size);
	/*
	 *		ȡ������(������)
	 */
	BOOL	Detach();

	/*
	 *	 ���������ָ��
	 */
	CHAR*	GetDataPtr()
	{
		return m_pDataPtr;
	}

	/*
	 *	��� ��СΪtSize �ĵ�tIndex ��smu������
	 */
	CHAR*	GetTypePtr(uint tSize,uint tIndex)
	{
		Assert(tSize>0);
		Assert(tSize*tIndex<m_Size);
		if( tSize<=0 || tIndex>=m_Size )
			return NULL ;
		return m_pDataPtr+tSize*tIndex;
	}
	/*
	 *	����������ܴ�С
	 */
	uint	GetSize()
	{
		return m_Size;
	}

	BOOL	DumpToFile(CHAR* FilePath);
	BOOL	MergeFromFile(CHAR* FilePath);

	UINT	GetHeadVer();
	VOID	SetHeadVer(UINT ver);
	
	//����
	INT					m_CmdArg;
private:
	
	//ShareMemory	�ڴ��С
	uint				m_Size;
	// ShareMemory  ����ָ��
	CHAR*				m_pDataPtr;
	// ShareMemory	�ڴ�ͷָ��
	CHAR*				m_pHeader;
	// ShareMemory	���	
	SMHandle			m_hold;

	CHAR				m_LogPath[255];
	
};	





#endif