
#include "StdAfx.h"
#include "GCEquipDecompoundResult.h"
#include "Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Item\Obj_Item_Equip.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "DataPool\GMUIDataPool.h"
#include "DataPool\GMDataPool.h"
#include "Action\GMActionSystem.h"
#include "Event\GMEventSystem.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Sound\GMSoundSystem.h"
 

using namespace Packets;
 

uint GCEquipDecompoundResultHandler::Execute(GCEquipDecompoundResult * pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CObjectManager* pObjectManager = CObjectManager::GetMe();

			EQUIPDECOMPOUND_RESULT ret = (EQUIPDECOMPOUND_RESULT)pPacket->GetResult();
			switch(ret)
			{

			case EQUIPDECOMPOUND_SUCCESS:
				{	

				   CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_EQUIP_DISASS);	
					
				   CGameProcedure::s_pGfxSystem->PushDebugString("装备拆解成功");
				   STRING strTemp = "装备拆解成功" ;
				   CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				
				}
				break;

			case EQUIPDECOMPOUND_EQUIP_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("拆解物品不存在");

					STRING strTemp = "拆解物品不存在" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;


			case EQUIPDECOMPOUND_BAG_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("玩家背包空间不足");

					STRING strTemp = "玩家背包空间不足" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;

			case EQUIPDECOMPOUND_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("拆借装备失败");

					STRING strTemp = "拆借装备失败" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;
			default:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("拆借装备失败");

					STRING strTemp = "拆借装备失败" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;
			}

			//解除被绑定物品的锁定
			INT nTypeOwner	= CDataPool::GetMe()->Dismantle_GetItemConta();
			INT nItemPos	= CDataPool::GetMe()->Dismantle_GetItemPos();
 
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

					if(pItem)
					{
						pItem->SetLock(FALSE);
					}


					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				}
				break;
			case tObject_Item::IO_MYSELF_EQUIP:		// 玩家的装备
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP (nItemPos));

					if(pItem)
					{
						pItem->SetLock(FALSE);

						if (EQUIPDECOMPOUND_SUCCESS == (EQUIPDECOMPOUND_RESULT)pPacket->GetResult())
						{
							// 改变物品的id, 以便更新新的action 按钮
							((CObject_Item*)pItem)->ChangeClientID();

							CDataPool::GetMe()->UserEquip_SetItem(HUMAN_EQUIP (nItemPos), NULL, FALSE);

							//---------------------------------------------------
							//逻辑层
							CObject_PlayerMySelf * pMySelf = CObjectManager::GetMe()->GetMySelf();
							//pMySelf->UnEquipItem( (HUMAN_EQUIP)pPacket->getEquipPoint() );

							//---------------------------------------
							//刷新角色属性
							CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_Equip(HUMAN_EQUIP (nItemPos), -1);

							// 更新主角身上的装备到ActionSystem
							CActionSystem::GetMe()->UserEquip_Update();

							// 通知界面事件
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);

							CSoundSystemFMod::_PlayUISoundFunc(67);
						}
						else
						{
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);
						}
					}
				}
				break;
			default:
				break;
			}
	 

			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);


			CDataPool::GetMe()->Dismantle_CleanUP();


		}

	return PACKET_EXE_CONTINUE ;


	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}