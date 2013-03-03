
#include "stdafx.h"
#include "MailCenter.h"
#include "OnlineUser.h"
#include "MailPool.h"
#include "Config.h"
#include "OnlineUser.h"
#include "TimeManager.h"


#define MAIL_HEARTBEAT_TIME 600000
const UINT uMaxHoldMailTick = 864000 ;//���������Ա����ʼ����ʱ��

MailCenter*	g_pMailCenter = NULL ;

MailCenter::MailCenter( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

MailCenter::~MailCenter( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

BOOL MailCenter::Init( )
{
__ENTER_FUNCTION

	BOOL bRet = m_UserHashTable.Init( ) ;
	if( bRet==FALSE )
		return FALSE ;

	bRet = m_UserChain.Init( ) ;
	if( bRet==FALSE )
		return FALSE ;

	_WORLD_INFO& WorldInfo = g_Config.m_WorldInfo;

	if(WorldInfo.m_EnableShareMem)
	{
		MailNode* pNode;

		for(UINT i=0;i<MAX_MAILNODE_SIZE;i++)
		{
			pNode = g_pMailNodePool->GetNode(i);		
			if(pNode)
			{
				RegisterMailNode(pNode);
			}
		}
	}
	

	return bRet ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID MailCenter::CleanUp( )
{
__ENTER_FUNCTION

	m_UserHashTable.CleanUp( ) ;
	m_UserChain.CleanUp( ) ;

__LEAVE_FUNCTION
}

BOOL MailCenter::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

	if( !m_LogicTime.IsSetTimer() )
	{
		m_LogicTime.BeginTimer( MAIL_HEARTBEAT_TIME, uTime ) ;
	}
	if( !m_LogicTime.CountingTimer(uTime) )
	{
		return TRUE ;
	}

	UserNode* pUserNode = (UserNode*)(m_UserChain.m_ChainUser.First()) ;
	while( pUserNode!=NULL )
	{
		if( pUserNode->m_ChainMail.GetNum() <= 0 &&
			pUserNode->m_ChainScriptMail.GetNum() <= 0)
		{//����û��ڵ���û���ʼ���������û��ڵ�

			m_UserHashTable.Del( pUserNode->m_szName ) ;
			m_UserChain.m_ChainUser.OutChain() ;
			g_pUserNodePool->DeleteNode( pUserNode ) ;

			pUserNode = (UserNode*)(m_UserChain.m_ChainUser.Current()) ;
			continue ;
		}

		if( pUserNode->m_uTickCount > uMaxHoldMailTick )
		{//����û��ڵ��ڷ����������ĺܳ�ʱ�䣬������û��ڵ�����
		 //���һ��յ�ǰ�û��ڵ��������������ʼ��ڵ�����

			pUserNode->m_ChainMail.First() ;
			while( pUserNode->m_ChainMail.GetNum()>0 )
			{
				MailNode* pMailNode = (MailNode*)(pUserNode->m_ChainMail.OutChain()) ;
				g_pMailNodePool->DeleteNode( pMailNode ) ;
			};

			m_UserHashTable.Del( pUserNode->m_szName ) ;
			m_UserChain.m_ChainUser.OutChain() ;
			g_pUserNodePool->DeleteNode( pUserNode ) ;

			pUserNode = (UserNode*)(m_UserChain.m_ChainUser.Current()) ;
			continue ;
		}
		
		pUserNode->m_uTickCount ++ ;
		pUserNode = (UserNode*)(m_UserChain.m_ChainUser.Next()) ;
	};




	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT MailCenter::RegisterMailNode(MailNode* pMailNode)
{
	__ENTER_FUNCTION

	if(!pMailNode)
	{
		Assert(FALSE);
		return 0;
	}

	const MAIL* pMail = pMailNode->GetMail();
	if(pMail->m_GUID == INVALID_ID)
	{
		return 0;
	}

	UserNode* pUserNode = NULL ;
	CHAR szDestName[MAX_CHARACTER_NAME];
	
	memset(szDestName, 0, MAX_CHARACTER_NAME);
	strncpy(szDestName, pMail->m_szDestName, MAX_CHARACTER_NAME - 1);
	pUserNode = m_UserHashTable.Get( szDestName );

	if( pUserNode == NULL )
	{//��ǰû��Ŀ����ҵ��ʼ�����
		pUserNode = g_pUserNodePool->NewNode( ) ;//����һ���û��ڵ�

		strncpy( pUserNode->m_szName, pMail->m_szDestName, MAX_CHARACTER_NAME-1 ) ;
		//���û��ڵ�ע�ᵽHash����
		m_UserHashTable.Add( pUserNode->m_szName, pUserNode ) ;
		//���û��ڵ���ӵ��ڵ�������
		m_UserChain.m_ChainUser.Add( pUserNode ) ;
	}
	pMailNode->m_nUserNodePoolIndex = pUserNode->m_nPoolIndex ;//�ʼ��ڵ��б��������û��ڵ�ֵ

	//�����ʼ����ͣ����ʼ��ڵ���ӵ��û����ʼ��б���
	switch( pMail->m_uFlag )
	{
	case MAIL_TYPE_SCRIPT:
		{
			pUserNode->m_ChainScriptMail.Add( pMailNode ) ;
		}
		break ;
	case MAIL_TYPE_NORMAL:
	case MAIL_TYPE_SYSTEM:
	case MAIL_TYPE_NORMAL2:
	default :
		{
			if ( pUserNode->m_ChainMail.GetNum() >= MAX_MAIL )
			{ // ����ż����� MAX_MAIL ������ɾ��
				//���ʼ���������Ͽ�
				MailNode* pOldMailNode = (MailNode*)(pUserNode->m_ChainMail.OutChain());
				if( pOldMailNode!=NULL )
				{
					//ɾ���ʼ��ڵ�
					g_pMailNodePool->DeleteNode( pOldMailNode );
				}
				else
				{
					Assert( FALSE );
				}
			}

			pUserNode->m_ChainMail.Add( pMailNode ) ;
		}
		break ;
	};


	//������û��ڵ��ﱣ����ʼ������������ֵ������
	//...δ��
	OnUserSendMail( pMail ) ;

	return pUserNode->GetMailCount()+pUserNode->GetScriptMailCount() ;

	__LEAVE_FUNCTION

		return 0 ;
}

//����mail��Ŀ�꣨Ŀ����Ϣ��mail�����
UINT MailCenter::MailTo( const MAIL* pMail )
{
__ENTER_FUNCTION

	UserNode* pUserNode = NULL ;
	CHAR szDestName[MAX_CHARACTER_NAME];

	memset(szDestName, 0, MAX_CHARACTER_NAME);
	strncpy(szDestName, pMail->m_szDestName, MAX_CHARACTER_NAME - 1);
	pUserNode = m_UserHashTable.Get( szDestName );

	if( pUserNode == NULL )
	{//��ǰû��Ŀ����ҵ��ʼ�����
		pUserNode = g_pUserNodePool->NewNode( ) ;//����һ���û��ڵ�

		strncpy( pUserNode->m_szName, pMail->m_szDestName, MAX_CHARACTER_NAME-1 ) ;
		//���û��ڵ�ע�ᵽHash����
		m_UserHashTable.Add( pUserNode->m_szName, pUserNode ) ;
		//���û��ڵ���ӵ��ڵ�������
		m_UserChain.m_ChainUser.Add( pUserNode ) ;
	}

	MailNode* pMailNode = g_pMailNodePool->NewNode()  ;//����һ���ʼ��ڵ�
	pMailNode->SetMail( pMail ) ;//�����ʼ�����
	pMailNode->m_nUserNodePoolIndex = pUserNode->m_nPoolIndex ;//�ʼ��ڵ��б��������û��ڵ�ֵ

	//�����ʼ����ͣ����ʼ��ڵ���ӵ��û����ʼ��б���
	switch( pMail->m_uFlag )
	{
	case MAIL_TYPE_SCRIPT:
		{
			pUserNode->m_ChainScriptMail.Add( pMailNode ) ;
		}
		break ;
	case MAIL_TYPE_NORMAL:
	case MAIL_TYPE_SYSTEM:
	case MAIL_TYPE_NORMAL2:
	default :
		{
			if ( pUserNode->m_ChainMail.GetNum() > MAX_MAIL )
			{ // ����ż����� MAX_MAIL ������ɾ��
				//���ʼ���������Ͽ�
				MailNode* pOldMailNode = (MailNode*)(pUserNode->m_ChainMail.OutChain());
				if( pOldMailNode!=NULL )
				{
					//ɾ���ʼ��ڵ�
					g_pMailNodePool->DeleteNode( pOldMailNode );
				}
				else
				{
					Assert( FALSE );
				}
			}

			pUserNode->m_ChainMail.Add( pMailNode ) ;
		}
		break ;
	};


	//������û��ڵ��ﱣ����ʼ������������ֵ������
	//...δ��

	OnUserSendMail( pMail ) ;

	return pUserNode->GetMailCount()+pUserNode->GetScriptMailCount() ;

__LEAVE_FUNCTION

	return 0 ;
}

//UINT MailCenter::MailToOnlineUser( MAIL* pMail, USER* pSender )
//{
//__ENTER_FUNCTION
//
//
//	return TRUE ;
//
//__LEAVE_FUNCTION
//
//	return FALSE ;
//}
//
//UINT MailCenter::MailToOfflineUser( MAIL* pMail, USER* pSender, USER* pRecver )
//{
//__ENTER_FUNCTION
//
//
//	return TRUE ;
//
//__LEAVE_FUNCTION
//
//	return FALSE ;
//}


//�������ֽ�szName��ҵ��ʼ���Ϣ��pList�����ʼ��б�, ����ֵΪ�ʼ�����
UINT MailCenter::AskMail( const CHAR* szName, MAIL_LIST* pListOut )
{
__ENTER_FUNCTION
	
	if( pListOut==NULL || szName==NULL )
		return 0 ;

	pListOut->CleanUp( ) ;

	CHAR szTemp[MAX_CHARACTER_NAME];
	memset((void*)szTemp, 0, MAX_CHARACTER_NAME);
	strncpy(szTemp, szName, MAX_CHARACTER_NAME-1);

	UserNode* pUserNode = m_UserHashTable.Get( szTemp ) ;
	if( pUserNode == NULL )
	{
		return 0 ;
	}

	pUserNode->m_ChainMail.First() ;
	while( pUserNode->m_ChainMail.GetNum()>0 )
	{
		//���ʼ���������Ͽ�
		MailNode* pMailNode = (MailNode*)(pUserNode->m_ChainMail.OutChain()) ;
		if( pMailNode==NULL )
		{
			Assert( FALSE ) ;
			break ;
		}
		//���ʼ���Ϣ�浽����ṹ��
		pListOut->m_aMail[pListOut->m_Count] = *pMailNode->GetMail() ;
		pListOut->m_Count++ ;
		//ɾ���ʼ��ڵ�
		g_pMailNodePool->DeleteNode( pMailNode ) ;

		//����һ���ʼ�
		break ;
	}
	//�����û���ӵ�е��ʼ�������������ṹ
	pListOut->m_TotalLeft = pUserNode->m_ChainMail.GetNum() ;

	OnUserRecvMail( pListOut ) ;

	return pListOut->m_Count ;

__LEAVE_FUNCTION

	return 0 ;
}

UINT MailCenter::CheckMail( const CHAR* szName )
{
__ENTER_FUNCTION
	
	if( szName==NULL )
		return 0 ;

	CHAR szTemp[MAX_CHARACTER_NAME];
	memset((void*)szTemp, 0, MAX_CHARACTER_NAME);
	strncpy(szTemp, szName, MAX_CHARACTER_NAME-1);

	UserNode* pUserNode = m_UserHashTable.Get( szTemp ) ;
	if( pUserNode == NULL )
	{
		return 0 ;
	}

	return pUserNode->GetMailCount() ;

__LEAVE_FUNCTION

	return 0 ;
}

//�������ֽ�szName��ҵ��ʼ���Ϣ��pList�����ʼ��б�, ����ֵΪ�ʼ�����
UINT MailCenter::AskScriptMail( const CHAR* szName, MAIL_LIST* pListOut )
{
__ENTER_FUNCTION
	
	if( pListOut==NULL || szName==NULL )
		return 0 ;

	pListOut->CleanUp( ) ;

	CHAR szTemp[MAX_CHARACTER_NAME];
	memset((void*)szTemp, 0, MAX_CHARACTER_NAME);
	strncpy(szTemp, szName, MAX_CHARACTER_NAME-1);

	UserNode* pUserNode = m_UserHashTable.Get( szTemp );
	if( pUserNode == NULL )
	{
		return 0 ;
	}

	pUserNode->m_ChainScriptMail.First() ;
	while( pUserNode->m_ChainScriptMail.GetNum()>0 )
	{
		//���ʼ���������Ͽ�
		MailNode* pMailNode = (MailNode*)(pUserNode->m_ChainScriptMail.OutChain()) ;
		if( pMailNode==NULL )
		{
			Assert( FALSE ) ;
			break ;
		}
		//���ʼ���Ϣ�浽����ṹ��
		pListOut->m_aMail[pListOut->m_Count] = *pMailNode->GetMail() ;
		pListOut->m_Count++ ;
		//ɾ���ʼ��ڵ�
		g_pMailNodePool->DeleteNode( pMailNode ) ;

		if( pListOut->m_Count>=MAX_MAIL_SIZE )
			break ;
	}
	//�����û���ӵ�е��ʼ�������������ṹ
	pListOut->m_TotalLeft = pUserNode->m_ChainScriptMail.GetNum() ;

	OnUserRecvMail( pListOut ) ;

	return pListOut->m_Count ;

__LEAVE_FUNCTION

	return 0 ;
}

//��ҽ��յ����ʼ�
VOID MailCenter::OnUserRecvMail( const MAIL_LIST* pList )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

//��ҷ������ʼ�
VOID MailCenter::OnUserSendMail( const MAIL* pMail )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

// �����ʼ�
VOID MailCenter::SendNormalMail( const CHAR* szReceiverName, const CHAR* szContent)
{
__ENTER_FUNCTION

	Assert( szReceiverName );
	Assert( szContent );

	MAIL mail;

	mail.m_uFlag = MAIL_TYPE_NORMAL2;
	mail.m_DestSize = (BYTE)strlen( szReceiverName );
	strncpy( mail.m_szDestName, szReceiverName, MAX_CHARACTER_NAME-1 );
	mail.m_ContexSize = (BYTE)strlen( szContent );
	strncpy( mail.m_szContex, szContent, MAX_MAIL_CONTEX-1 );

	mail.m_uCreateTime = g_pTimeManager->GetANSITime();

	SendMail(&mail);

__LEAVE_FUNCTION
}

// ���Ϳ�ִ���ʼ�
VOID MailCenter::SendScriptMail(const CHAR* szReceiverName, UINT uParam0, UINT uParam1, UINT uParam2, UINT uParam3 )
{
__ENTER_FUNCTION

	Assert( szReceiverName );

	MAIL mail;

	mail.m_uFlag = MAIL_TYPE_SCRIPT;
	mail.m_DestSize = (BYTE)strlen( szReceiverName );
	strncpy( mail.m_szDestName, szReceiverName, MAX_CHARACTER_NAME-1 );
	mail.m_uParam0 = uParam0;
	mail.m_uParam1 = uParam1;
	mail.m_uParam2 = uParam2;
	mail.m_uParam3 = uParam3;

	SendMail(&mail);

__LEAVE_FUNCTION
}

// �� SendNormalMail��SendScriptMail ����
VOID MailCenter::SendMail(const MAIL* pMail)
{
__ENTER_FUNCTION

	Assert( pMail );

	USER* pUser = g_pOnlineUser->FindUser( pMail->m_szDestName );
	if( pUser )
	{//��ǰ�û�����
		pUser->RecvMail( pMail );
	}
	else
	{
		MailTo( pMail );
	}

__LEAVE_FUNCTION
}
