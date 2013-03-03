
/**	
 *	
 *	���ܣ�	�����NPC�Ի��� ѡ�񴴽����
 *	
 *	�޸ļ�¼��
 *			080228	���Ӵ����������
 *			080304	�޸ļ�����������
 *			
 */

#include "StdAfx.h"
#include "GCGuildApply.h"
#include "Procedure\GameProcedure.h"
#include "Event\GMEventSystem.h"
#include "Interface\GMInterface_Script_Talk.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "DataPool\GMDP_CharacterData.h"
#include "DataPool\GMDataPool.h"



uint GCGuildApplyHandler::Execute( GCGuildApply* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	// �ж�������
	if( CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain )
	{
		ObjID_t NpcId = pPacket->GetNpcId();

		if( CObjectManager::GetMe()->FindServerObject( (INT)NpcId ) )
		{
			CObject_PlayerMySelf* mySelf = CObjectManager::GetMe()->GetMySelf();
			if( !mySelf )
				return PACKET_EXE_ERROR;


			// �ж��Ƿ����������������


			// �ж��Ƿ��Լ����˰��
			if( INVALID_ID != mySelf->GetCharacterData()->Get_Guild() )
			{
				CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "�����뿪�����ڵİ���������뽨��ɡ�" );
				return PACKET_EXE_CONTINUE;
			}

			// �жϵȼ�
			if( mySelf->GetCharacterData()->Get_Level() < 20 )
			{
				CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "�ȼ����㣬�뵽 20 �Ժ��������뽨��ɡ�" );
				return PACKET_EXE_CONTINUE;
			}

			// �жϱ������Ƿ��С������
			{
				BOOL bRelue = FALSE;
				tObject_Item* pItemBag = NULL;

				for( INT i =0 ;i<  MAX_BAG_SIZE ;++i )
				{
					pItemBag =  CDataPool::GetMe()->UserBag_GetItem( i );
					if( pItemBag )
					{
						if( EQUIP_CHIEFTAIN_TAG == pItemBag->GetIdTable() )
						{
							bRelue = TRUE;
							break;
						}

					}
				}

				if( !bRelue )
				{
					CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "��Я����������������뽨��ɣ�" );
					return PACKET_EXE_CONTINUE;
				}
			}


// server ���
/*
			// ��ҹ��������ڳ�������������ͬ
			if( mySelf->GetCharacterData()->Get_Country() )
			{
				// ��ȡ�����Ĺ���

				CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, "�뵽�Լ��Ĺ������뽨�" );
			}

			// �ж��뿪�ϸ����ļ��ʱ��
			if(  )
			{
				CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, "�뿪��᲻��24Сʱ�����Ժ��������뽨��ɡ�" );
				return PACKET_EXE_CONTINUE;
			}

*/

			// ���������� �򿪴������ɵĽ���
			{
				INT careId = CObjectManager::GetMe()->FindServerObject(NpcId)->GetID();
				CEventSystem::GetMe()->PushEvent( GE_GUILD_CREATE, careId );
			}
		}
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

