
#include "StdAfx.h"
#include "Global.h"
#include "NetWork\NetManager.h"
#include "GIException.h"
#include "GITimeSystem.h"
#include "GIVariable.h"
#include "AxProfile.h"
#include "Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMyself.h"
#include "Object\Logic\Object.h"
#include "GameCommand.h"
#include "Object\ObjectCommandDef.h"
#include "GCCharBaseAttrib.h"
#include "DataPool\GMUIDataPool.h"
#include "DataPool\DataPoolCommandDef.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Event\GMEventSystem.h"
#include "Object\GMObjectLoadQueue.h"
#include "Interface\GMGameInterFace.h"
#include "../../dbc/GMDataBase.h"



uint GCCharBaseAttribHandler::Execute( GCCharBaseAttrib* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	if( CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain )
	{
//		AxProfile::AxProfile_PushNode("CharBaseAttrib");

		CObjectManager* pObjectManager = CObjectManager::GetMe();

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
		if( pObj == NULL || !g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)) )
			return PACKET_EXE_CONTINUE;

		CObject_Character* pChar = (CObject_Character*)pObj;

		// ˢ�����ݳ�
		CCharacterData* pCharacterData = ((CObject_Character*)pObj)->GetCharacterData();


		// ְҵ��Ҫ��֤���ڵ�һλ������
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_JOB ) )
		{
			pCharacterData->Set_MenPai( pPacket->getJob() );
		}
		// ����
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_COUNTRY ) )
		{
			pCharacterData->Set_Country( pPacket->getCountry() );
		}

		// ��ɫ���壨����ģ�ͣ�
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_DATA_ID ) )
		{
			pCharacterData->Set_RaceID( pPacket->getDataID() );
			if( pObj == CObjectManager::GetMe()->GetMySelf() )
			{//laim  ����Ф����߼� ��Ӧ�÷�������ɣ�Ȼ�󴫵��ͻ��ˣ�GCDetailAttribHandler.cpp����Ŀǰûд����ʱ������
			 //yangfei[2010/02/06]:�����ע�ͽ��Ͳ���ȷ����Ϣ����ʵ�ɿͻ��˽���
				pCharacterData->Set_PortraitID(pPacket->getPortraitID());
			}


	
		}
		// ����
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_NAME ) )
		{
			pCharacterData->Set_Name( pPacket->getName() );				
		}
		// ͷ��
		{
			// ����ͷ��
			if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_COUNTRYTITLE ) )
				pCharacterData->Set_CurTitle( pPacket->getCountryTitle(), _TITLE::GUOJIA_TITLE );
			// ���ͷ��
			if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_GUILDTITLE ) )
				pCharacterData->Set_CurTitle( pPacket->getGuildTitle(), _TITLE::BANGPAI_TITLE );
			// �淨ͷ��
			if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_NORMALTITLE ) )
				pCharacterData->Set_CurTitle( pPacket->getNormalTitle(), _TITLE::WANFA_TITLE );

//			pCharacterData->Set_CurTitle( pPacket->getTitle(), pPacket->getTitleType() );
//			pCharacterData->Set_CurTitleType( pPacket->getTitleType() );
			
			// ��ǰ����ĳƺŸı���.
			CEventSystem::GetMe()->PushEvent( GE_CUR_TITLE_CHANGEED );
		}
		// �ȼ�
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_LEVEL ) )
		{
			INT nLevel = pCharacterData->Get_Level();

			pCharacterData->Set_Level( pPacket->getLevel() );	

			// �������Լ������ˣ�������Χ����������ɫ		20100531 BLL
			if ( pChar == CObjectManager::GetMe()->GetMySelf() && ( pPacket->getLevel() - nLevel ) == 1 )
			{
				CObjectManager::GetMe()->OnMySelfLevelUp();	// �����Լ����µȼ���������Χ�����������ɫ
			}
		}
		// HP�ٷֱ�
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_HP_PERCENT ) )
		{
			pCharacterData->Set_HPPercent( pPacket->getHPPercent() / 100.0f );	
		}
		// MP�ٷֱ�
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_MP_PERCENT ) )
		{
			pCharacterData->Set_MPPercent( pPacket->getMPPercent() / 100.0f );
		}
		// Ԫ���ٷֱ�
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_PNEUMA_PERCENT ) )
		{
			pCharacterData->Set_PenumaPercent( pPacket->getPneumaPercent() / 100.0f );
		}
		// ŭ��
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_RAGE ) )
		{
			pCharacterData->Set_Rage( pPacket->getRage() );					
		}
		// ������
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_STEALTH_LEVEL ) )
		{
			pCharacterData->Set_StealthLevel( pPacket->getStealthLevel() );	
		}
		// �Ƿ�Ϊ����״̬
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_SIT ) )
		{
			//pCharacterData->Set_Sit(pPacket->IsSit() );					
		}
		// �ƶ��ٶ�
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_MOVE_SPEED ) )
		{
			pCharacterData->Set_MoveSpeed( pPacket->getMoveSpeed() );			
		}
		// �����ٶ�
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_ATTACK_SPEED ) )
		{
			// ����������
			if( pObj == CObjectManager::GetMe()->GetMySelf() )
				pCharacterData->Set_AttackSpeed( (INT)pPacket->getAttackSpeed() );	
		}
		// ��ӪID
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_CAMP_ID ) )
		{
			pCharacterData->Set_CampData( pPacket->getCampData() );			
		}
		// ӵ����
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_OWNER ) )
		{
			pCharacterData->Set_OwnerID( pPacket->getOwnerID() );				
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_BUSOBJID))
		{
			pCharacterData->Set_BusObjID(pPacket->GetBusObjID());
		}
		// ռ����
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_OCCUPANT ) )
		{
			pCharacterData->Set_OccupantGUID( pPacket->getOccupantGUID() );	
		}
		// ͷ��ID
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_PORTRAIT_ID ) )
		{
//laim  ����Ф����߼� ��Ӧ�÷�������ɣ�Ȼ�󴫵��ͻ��ˣ�GCDetailAttribHandler.cpp����Ŀǰûд����ʱ������
			//yangfei[2010/02/06]:�����ע�ͽ��Ͳ���ȷ����Ϣ����ʵ�ɿͻ��˽���
			if( pObj != CObjectManager::GetMe()->GetMySelf() )
			pCharacterData->Set_PortraitID(pPacket->getPortraitID());		
		}
		// ģ��ID
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_MODEL_ID ) )
		{
			pCharacterData->Set_ModelID( pPacket->getModelID() );				
		}
		// ����ID
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_MOUNT_ID ) )
		{
			if (pChar->GetCharacterType() == CObject_Character::CT_MONSTER)
			{
				CObject_PlayerNPC* pNpc = (CObject_PlayerNPC*)pChar;
				// 
				if (pNpc->GetNpcType() == NPC_TYPE_MOUNT)
				{
					// �������
					DBC_DEFINEHANDLE(s_pCharMountDBC, DBC_CHARACTER_MOUNT)
					const _DBC_CHARACTER_MOUNT *pMount = 
					(const _DBC_CHARACTER_MOUNT*)( s_pCharMountDBC->Search_Index_EQU(pPacket->getMountID()) );
					if (pMount)
					{
						pCharacterData->Set_ModelID(pMount->m_nModelID);
						pNpc->SetNpcMountEffect(pPacket->getMountID(), NULL);
					}
				}
				else
				{
					pCharacterData->Set_MountID( pPacket->getMountID() );
				}
			}
			else
			{
				pCharacterData->Set_MountID( pPacket->getMountID() );
			}			
		}
		// ����INDEX   [7/23/2010 �¾���]
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_MOUNT_INDEX ) )
		{
			pCharacterData->Set_MountIndex( pPacket->getMountIndex() );	
		}
		// ����ͷ����ͷ����ɫ
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_PLAYER_DATA ) )
		{
			INT xx = pPacket->getFaceMeshID();
			pCharacterData->Set_FaceMesh( pPacket->getFaceMeshID() );

			INT xx2 = pPacket->getHairMeshID();
			pCharacterData->Set_HairMesh( pPacket->getHairMeshID() );

			INT xx3 = pPacket->getHairMatrixID();
			pCharacterData->Set_HairColor( pPacket->getHairMatrixID() );
		}
		// �Ƿ��̯״̬
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_IS_IN_STALL ) )
		{
			BOOL bIsStall = pCharacterData->Get_IsInStall();
			pCharacterData->Set_IsInStall( pPacket->getIsOpen() );

			// ��̯�󣬻ָ���̯ǰ��ͷ����Ϣ��״̬   20100527 BLL
			if ( pPacket->getIsOpen() == FALSE && bIsStall == TRUE )
			{
				pChar->RestoreInfoBoard();
			}
		}
		// ̯λ��
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_STALL_NAME ) )
		{
			pCharacterData->Set_StallName(pPacket->getStallName());			
		}

		// ����̯λ�Ƿ���	20100706 BLL
		if ( pPacket->IsUpdateAttrib( UPDATE_CHAR_STALL_PUBLIC ) )
		{
			pCharacterData->Set_StallOpenPublic( pPacket->getIsOpenPublic() );
		}

		pObj->PushDebugString("GCCharBaseAttrib");
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());

		//�����������Լ�
		if(pObj != (CObject*)CObjectManager::GetMe()->GetMySelf() && pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_AITYPE))
		{
			pCharacterData->Set_AIType(pPacket->getAIType());
		}
	}
	else
	{
		//KLAssert( FALSE );
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

