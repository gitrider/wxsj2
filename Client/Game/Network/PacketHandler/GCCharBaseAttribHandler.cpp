
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

		// 刷入数据池
		CCharacterData* pCharacterData = ((CObject_Character*)pObj)->GetCharacterData();


		// 职业（要保证放在第一位来处理）
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_JOB ) )
		{
			pCharacterData->Set_MenPai( pPacket->getJob() );
		}
		// 国家
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_COUNTRY ) )
		{
			pCharacterData->Set_Country( pPacket->getCountry() );
		}

		// 角色种族（更换模型）
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_DATA_ID ) )
		{
			pCharacterData->Set_RaceID( pPacket->getDataID() );
			if( pObj == CObjectManager::GetMe()->GetMySelf() )
			{//laim  人物肖像的逻辑 本应该服务器完成，然后传到客户端（GCDetailAttribHandler.cpp），目前没写，暂时先这样
			 //yangfei[2010/02/06]:上面的注释解释不正确。消息包事实由客户端解析
				pCharacterData->Set_PortraitID(pPacket->getPortraitID());
			}


	
		}
		// 名称
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_NAME ) )
		{
			pCharacterData->Set_Name( pPacket->getName() );				
		}
		// 头衔
		{
			// 国家头衔
			if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_COUNTRYTITLE ) )
				pCharacterData->Set_CurTitle( pPacket->getCountryTitle(), _TITLE::GUOJIA_TITLE );
			// 帮会头衔
			if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_GUILDTITLE ) )
				pCharacterData->Set_CurTitle( pPacket->getGuildTitle(), _TITLE::BANGPAI_TITLE );
			// 玩法头衔
			if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_NORMALTITLE ) )
				pCharacterData->Set_CurTitle( pPacket->getNormalTitle(), _TITLE::WANFA_TITLE );

//			pCharacterData->Set_CurTitle( pPacket->getTitle(), pPacket->getTitleType() );
//			pCharacterData->Set_CurTitleType( pPacket->getTitleType() );
			
			// 当前人物的称号改变了.
			CEventSystem::GetMe()->PushEvent( GE_CUR_TITLE_CHANGEED );
		}
		// 等级
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_LEVEL ) )
		{
			INT nLevel = pCharacterData->Get_Level();

			pCharacterData->Set_Level( pPacket->getLevel() );	

			// 如果玩家自己升级了，更新周围怪物名字颜色		20100531 BLL
			if ( pChar == CObjectManager::GetMe()->GetMySelf() && ( pPacket->getLevel() - nLevel ) == 1 )
			{
				CObjectManager::GetMe()->OnMySelfLevelUp();	// 根据自己的新等级，更新周围怪物的名字颜色
			}
		}
		// HP百分比
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_HP_PERCENT ) )
		{
			pCharacterData->Set_HPPercent( pPacket->getHPPercent() / 100.0f );	
		}
		// MP百分比
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_MP_PERCENT ) )
		{
			pCharacterData->Set_MPPercent( pPacket->getMPPercent() / 100.0f );
		}
		// 元气百分比
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_PNEUMA_PERCENT ) )
		{
			pCharacterData->Set_PenumaPercent( pPacket->getPneumaPercent() / 100.0f );
		}
		// 怒气
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_RAGE ) )
		{
			pCharacterData->Set_Rage( pPacket->getRage() );					
		}
		// 隐身级别
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_STEALTH_LEVEL ) )
		{
			pCharacterData->Set_StealthLevel( pPacket->getStealthLevel() );	
		}
		// 是否为坐下状态
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_SIT ) )
		{
			//pCharacterData->Set_Sit(pPacket->IsSit() );					
		}
		// 移动速度
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_MOVE_SPEED ) )
		{
			pCharacterData->Set_MoveSpeed( pPacket->getMoveSpeed() );			
		}
		// 攻击速度
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_ATTACK_SPEED ) )
		{
			// 必须是主角
			if( pObj == CObjectManager::GetMe()->GetMySelf() )
				pCharacterData->Set_AttackSpeed( (INT)pPacket->getAttackSpeed() );	
		}
		// 阵营ID
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_CAMP_ID ) )
		{
			pCharacterData->Set_CampData( pPacket->getCampData() );			
		}
		// 拥有者
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_OWNER ) )
		{
			pCharacterData->Set_OwnerID( pPacket->getOwnerID() );				
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_BUSOBJID))
		{
			pCharacterData->Set_BusObjID(pPacket->GetBusObjID());
		}
		// 占有者
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_OCCUPANT ) )
		{
			pCharacterData->Set_OccupantGUID( pPacket->getOccupantGUID() );	
		}
		// 头像ID
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_PORTRAIT_ID ) )
		{
//laim  人物肖像的逻辑 本应该服务器完成，然后传到客户端（GCDetailAttribHandler.cpp），目前没写，暂时先这样
			//yangfei[2010/02/06]:上面的注释解释不正确。消息包事实由客户端解析
			if( pObj != CObjectManager::GetMe()->GetMySelf() )
			pCharacterData->Set_PortraitID(pPacket->getPortraitID());		
		}
		// 模型ID
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_MODEL_ID ) )
		{
			pCharacterData->Set_ModelID( pPacket->getModelID() );				
		}
		// 座骑ID
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_MOUNT_ID ) )
		{
			if (pChar->GetCharacterType() == CObject_Character::CT_MONSTER)
			{
				CObject_PlayerNPC* pNpc = (CObject_PlayerNPC*)pChar;
				// 
				if (pNpc->GetNpcType() == NPC_TYPE_MOUNT)
				{
					// 检索马表
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
		// 座骑INDEX   [7/23/2010 陈军龙]
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_MOUNT_INDEX ) )
		{
			pCharacterData->Set_MountIndex( pPacket->getMountIndex() );	
		}
		// 脸、头发、头发颜色
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_ATT_PLAYER_DATA ) )
		{
			INT xx = pPacket->getFaceMeshID();
			pCharacterData->Set_FaceMesh( pPacket->getFaceMeshID() );

			INT xx2 = pPacket->getHairMeshID();
			pCharacterData->Set_HairMesh( pPacket->getHairMeshID() );

			INT xx3 = pPacket->getHairMatrixID();
			pCharacterData->Set_HairColor( pPacket->getHairMatrixID() );
		}
		// 是否摆摊状态
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_IS_IN_STALL ) )
		{
			BOOL bIsStall = pCharacterData->Get_IsInStall();
			pCharacterData->Set_IsInStall( pPacket->getIsOpen() );

			// 收摊后，恢复摆摊前的头顶信息板状态   20100527 BLL
			if ( pPacket->getIsOpen() == FALSE && bIsStall == TRUE )
			{
				pChar->RestoreInfoBoard();
			}
		}
		// 摊位名
		if( pPacket->IsUpdateAttrib( UPDATE_CHAR_STALL_NAME ) )
		{
			pCharacterData->Set_StallName(pPacket->getStallName());			
		}

		// 设置摊位是否开市	20100706 BLL
		if ( pPacket->IsUpdateAttrib( UPDATE_CHAR_STALL_PUBLIC ) )
		{
			pCharacterData->Set_StallOpenPublic( pPacket->getIsOpenPublic() );
		}

		pObj->PushDebugString("GCCharBaseAttrib");
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());

		//如果不是玩家自己
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

