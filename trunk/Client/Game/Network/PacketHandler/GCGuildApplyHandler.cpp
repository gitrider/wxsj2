
/**	
 *	
 *	功能：	处理和NPC对话， 选择创建帮会
 *	
 *	修改记录：
 *			080228	增加创建条件检测
 *			080304	修改检测帮主令条件
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

	// 判断主流程
	if( CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain )
	{
		ObjID_t NpcId = pPacket->GetNpcId();

		if( CObjectManager::GetMe()->FindServerObject( (INT)NpcId ) )
		{
			CObject_PlayerMySelf* mySelf = CObjectManager::GetMe()->GetMySelf();
			if( !mySelf )
				return PACKET_EXE_ERROR;


			// 判断是否符合条件创建帮派


			// 判断是否以加入了帮会
			if( INVALID_ID != mySelf->GetCharacterData()->Get_Guild() )
			{
				CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "请先离开你现在的帮会再来申请建帮吧。" );
				return PACKET_EXE_CONTINUE;
			}

			// 判断等级
			if( mySelf->GetCharacterData()->Get_Level() < 20 )
			{
				CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "等级不足，请到 20 以后再来申请建帮吧。" );
				return PACKET_EXE_CONTINUE;
			}

			// 判断背包中是否有“帮主令”
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
					CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "请携带“帮主令”再来申请建帮吧！" );
					return PACKET_EXE_CONTINUE;
				}
			}


// server 检测
/*
			// 玩家国籍与所在场景所属国家相同
			if( mySelf->GetCharacterData()->Get_Country() )
			{
				// 获取场景的国家

				CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, "请到自己的国家申请建帮。" );
			}

			// 判断离开上个帮会的间隔时间
			if(  )
			{
				CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, "离开帮会不足24小时，请稍后再来申请建帮吧。" );
				return PACKET_EXE_CONTINUE;
			}

*/

			// 符合条件， 打开创建帮派的界面
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

