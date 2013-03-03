
#include "stdafx.h"
#include "ChatPipe.h"
#include "Scene.h"
#include "Packet.h"
#include "Obj_Human.h"
#include "GCChat.h"
#include "PacketFactoryManager.h"
#include "ScanOperator_Chat.h"



ChatPipe::ChatPipe( )
{
__ENTER_FUNCTION

	m_pScene = NULL ;
	m_nValidCount = 0 ;

	m_nHead = 0 ;
	m_nTail = 0 ;

	m_pChatItems = NULL ;

__LEAVE_FUNCTION
}

ChatPipe::~ChatPipe( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

	m_pScene = NULL ;
	SAFE_DELETE_ARRAY( m_pChatItems ) ;

__LEAVE_FUNCTION
}

VOID ChatPipe::CleanUp( )
{
__ENTER_FUNCTION

	m_Lock.Lock() ;

	m_nValidCount = 0 ;

	m_nHead = 0 ;
	m_nTail = 0 ;

	for( INT i=0; i<CHAT_ITEM_SIZE; i++ )
	{
		m_pChatItems[i].m_pPacket = NULL ;
		m_pChatItems[i].m_SourObjID = INVALID_ID ;
		m_pChatItems[i].m_DestObjID = INVALID_ID ;
	}

__LEAVE_FUNCTION

	m_Lock.Unlock() ;
}

BOOL ChatPipe::Init( Scene* pScene )
{
__ENTER_FUNCTION

	m_pChatItems = new CHAT_ITEM[CHAT_ITEM_SIZE] ;
	Assert( m_pChatItems ) ;
	

	m_pScene = pScene ;
	Assert( m_pScene ) ;

__LEAVE_FUNCTION

	return TRUE ;
}

//DestID:�����Ǳ�ʾ����Ƶ��ʱĿ��OBJ
//����������������ʱ�����ּ����Ǵ����ڰ��ģ���˽��м���ID����Ψһ��ʶ
//����ڼ�������ʱ��DestID��Ϊ���ID������һ���²�Ϊ��
BOOL ChatPipe::SendPacket( Packet* pPacket, ObjID_t SourID, ObjID_t DestID )
{
__ENTER_FUNCTION

	m_Lock.Lock() ;

	Assert( m_nHead>=0 && m_nHead<CHAT_ITEM_SIZE ) ;
	Assert( m_nTail>=0 && m_nTail<CHAT_ITEM_SIZE ) ;

	if( m_pChatItems[m_nHead].m_pPacket )//�ܵ��������Ѿ�����
	{
		m_Lock.Unlock() ;
		return FALSE ;
	}

	m_pChatItems[m_nHead].m_pPacket = pPacket ;
	m_pChatItems[m_nHead].m_SourObjID = SourID ;
	m_pChatItems[m_nHead].m_DestObjID = DestID ;

	m_nHead ++ ;
	if( m_nHead>= CHAT_ITEM_SIZE )
		m_nHead = 0 ;

__LEAVE_FUNCTION

	m_Lock.Unlock() ;
	return TRUE ;
}

BOOL ChatPipe::RecvPacket( Packet*& pPacket, ObjID_t* pSourID, ObjID_t* pDestID )
{
__ENTER_FUNCTION

	m_Lock.Lock() ;

	Assert( m_nHead>=0 && m_nHead<CHAT_ITEM_SIZE ) ;
	Assert( m_nTail>=0 && m_nTail<CHAT_ITEM_SIZE ) ;

	if( m_pChatItems[m_nTail].m_pPacket == NULL )//û������
	{
		m_Lock.Unlock() ;
		return FALSE ;
	}

	pPacket = m_pChatItems[m_nTail].m_pPacket ;
	*pSourID = m_pChatItems[m_nTail].m_SourObjID ;
	*pDestID = m_pChatItems[m_nTail].m_DestObjID ;

	m_pChatItems[m_nTail].m_pPacket = NULL ;
	m_pChatItems[m_nTail].m_SourObjID = INVALID_ID ;
	m_pChatItems[m_nTail].m_DestObjID = INVALID_ID ;

	m_nTail ++ ;
	if( m_nTail>= CHAT_ITEM_SIZE )
		m_nTail = 0 ;

__LEAVE_FUNCTION

	m_Lock.Unlock() ;
	return TRUE ;
}

BOOL ChatPipe::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

	m_nValidCount += COUNTS_PER_TICK ;

	Packet* pPacket = NULL ;
	ObjID_t SourID = INVALID_ID ;
	ObjID_t DestID = INVALID_ID ;

	for( INT i=0; i<COUNTS_PER_TICK; i++ )
	{
		BOOL bRet = RecvPacket( pPacket, &SourID, &DestID ) ;
		if( bRet==FALSE || pPacket==NULL )
		{
			break ;
		}

		GCChat* pChat = (GCChat*)pPacket ;
		_MY_TRY
		{
			switch( pChat->GetChatType() )
			{
			case CHAT_TYPE_NEAR:
				{
					Obj* pObj = GetScene()->GetObjManager()->GetObj(SourID) ;
					if( pObj==NULL 
						|| !IsCharacterObj(pObj->GetObjType()) 
						|| !pObj->IsActiveObj() )
					{
						break ;
					}
					Obj_Character* pCharacter = (Obj_Character*)pObj ;
					if( pCharacter->GetObjType()!=Obj::OBJ_TYPE_HUMAN &&
						pCharacter->GetObjType()!=Obj::OBJ_TYPE_MONSTER)
					{
						break ;
					}
					//Obj_Human* pHuman = (Obj_Human*)pObj ;

					SCANOPERATOR_CHAT_INIT init ;
					init.m_bScanHuman = TRUE ;
					init.m_pScene = GetScene() ;
					//init.m_ZoneID = pHuman->getZoneID() ;
					init.m_ZoneID = pCharacter->getZoneID();
					init.m_nZoneRadius = MAX_RADIUS ;
					init.m_nChatType = pChat->GetChatType() ;
					init.m_pPacket = pChat ;
					init.m_GuildID = INVALID_ID ;

					ScanOperator_Chat scan ;
					scan.Init( &init ) ;

					GetScene()->Scan( &scan ) ;
	//
	//				GetScene()->BroadCast_Chat( pChat, pHuman->getZoneID() ) ;
				}
				break ;
			case CHAT_TYPE_WORLD:
			case CHAT_TYPE_SYSTEM:
				{
					SCANOPERATOR_CHAT_INIT init ;
					init.m_bScanHuman = TRUE ;
					init.m_pScene = GetScene() ;
					init.m_ZoneID = INVALID_ID ;
					init.m_nZoneRadius = MAX_RADIUS ;
					init.m_nChatType = pChat->GetChatType() ;
					init.m_pPacket = pChat ;
					init.m_GuildID = INVALID_ID ;

					ScanOperator_Chat scan ;
					scan.Init( &init ) ;

					GetScene()->Scan( &scan ) ;
	//
	//				GetScene()->BroadCast_Chat( pChat, INVALID_ID ) ;
				}
				break ;
			case CHAT_TYPE_TEAM:
			case CHAT_TYPE_USER:
			case CHAT_TYPE_SELF:
				{
					Obj* pObj = GetScene()->GetObjManager()->GetObj(DestID) ;
					if( pObj==NULL 
						|| !IsCharacterObj(pObj->GetObjType()) 
						|| !pObj->IsActiveObj() )
					{
						break ;
					}
					Obj_Character* pCharacter = (Obj_Character*)pObj ;
					if( pCharacter->GetObjType()!=Obj::OBJ_TYPE_HUMAN )
					{
						break ;
					}
					Obj_Human* pHuman = (Obj_Human*)pObj ;

					pHuman->GetPlayer()->SendPacket( pChat ) ;

					m_nValidCount -- ;
				}
				break ;
			case CHAT_TYPE_GUILD:
				{
					SCANOPERATOR_CHAT_INIT init ;
					init.m_bScanHuman = TRUE ;
					init.m_pScene = GetScene() ;
					init.m_ZoneID = INVALID_ID ;
					init.m_nZoneRadius = MAX_RADIUS ;
					init.m_nChatType = pChat->GetChatType() ;
					init.m_pPacket = pChat ;
					init.m_GuildID = (GuildID_t)DestID ;//��Ҫ���ð���ID

					ScanOperator_Chat scan ;
					scan.Init( &init ) ;

					GetScene()->Scan( &scan ) ;
				}
				break ;
			case CHAT_TYPE_JIAZU:
				{
					SCANOPERATOR_CHAT_INIT init ;
					init.m_bScanHuman = TRUE ;
					init.m_pScene = GetScene() ;
					init.m_ZoneID = INVALID_ID ;
					init.m_nZoneRadius = MAX_RADIUS ;
					init.m_nChatType = pChat->GetChatType() ;
					init.m_pPacket = pChat ;
					init.m_GuildID = DestID;
					init.m_JiaZuID = (JiaZuID_t)SourID ;//��Ҫ���ð���ID

					ScanOperator_Chat scan ;
					scan.Init( &init ) ;

					GetScene()->Scan( &scan ) ;
				}
				break ;
			case CHAT_TYPE_COUNTRY:
				{
					SCANOPERATOR_CHAT_INIT init ;
					init.m_bScanHuman = TRUE ;
					init.m_pScene = GetScene() ;
					init.m_ZoneID = INVALID_ID ;
					init.m_nZoneRadius = MAX_RADIUS ;
					init.m_nChatType = pChat->GetChatType() ;
					init.m_pPacket = pChat ;
					init.m_CountryID = (CountryID_t)SourID ;//��Ҫ���ð���ID

					ScanOperator_Chat scan ;
					scan.Init( &init ) ;

					GetScene()->Scan( &scan ) ;
				}
				break ;
			default :
				{
					Assert(FALSE) ;
				}
				break ;
			};
		}
		_MY_CATCH
		{
			SaveCodeLog( ) ;
		}
		if( pChat )
		{
			g_pPacketFactoryManager->RemovePacket( pChat ) ;
		}

		if( m_nValidCount <=0 ) break ;
	}


	//�����֡û�з����㹻�����Ϣ�����������
	if( m_nValidCount>0 ) m_nValidCount = 0 ;

__LEAVE_FUNCTION

	return TRUE ;
}

