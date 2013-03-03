#include "StdAfx.h"
#include "GCNewObjItem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\TripperObj\GMTripperObj_ItemDrop.h"
#include "Object\Logic\TripperObj\GMTripperObj_Resource.h"
#include "World\WorldManager.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "GITimeSystem.h"
#include "..\..\Global.h"
#include "..\..\GameCommand.h"
#include "Object\ObjectCommandDef.h"
#include "GIException.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "..\..\DataPool\GMDataPool.h"
#include "Sound\GMSoundSystem.h"
#include "Event\GMEventSystem.h"
#include "..\..\Interface\GMInterface_Script_Talk.h"
#include "..\..\Object\Logic\Character\Obj_PlayerNPC.h"

using namespace Packets;
uint GCNewObjItemHandler::Execute(GCNewObjItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	 

		//当前流程是主流程
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
 			CObjectManager* pObjectManager = CObjectManager::GetMe();

			//检查位置是否合法
			if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ)))
			{
				KLThrow("ERROR POSITION @ GCNewItemBoxHandler");
			}

		 
			if(ITYPE_MONSTERDROP_OBJ_ITEM == pPacket->getObjType())//ObjItem的类型
			{
				BOOL bMustCreater = TRUE;
				//CObject* pObj = (CObject*)(pObjectManager->FindServerObject( pPacket->getObjID() ));//产生掉落包的怪物id
				//if ( pObj != NULL)// && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)) 
				{

					//CObject_Character *pCharacter		= (CObject_Character*)pObj;
				//	if(!pCharacter->IsDie())
					{

				     		ObjID_t		idItemDrop		= pPacket->getObjID();

							//产生ui事件
							char szMsg[MAX_PATH];
							//_snprintf(szMsg, MAX_PATH, "目前的id: %d  位置 ： %f  %f", idItemDrop, pPacket->getWorldPos().m_fX,pPacket->getWorldPos().m_fZ);
							CGameProcedure::s_pEventSystem->PushEvent( GE_CHAT_MESSAGE, "PACKET", szMsg, 4 );
						
							STRING strTemp = COLORMSGFUNC("battle_get_myself", szMsg);
							//ADDTALKMSG(strTemp);
							 
							GUID_t idOwner[MAX_TEAM_MEMBER];

							for (  UINT i  =0 ;i< MAX_TEAM_MEMBER ;++i)
							{
								idOwner[i] = -1;
							}

							INT  num =pPacket->getOwnerCount(); //拥有者

							for (UINT i  =0 ;i< num ;++i)
							{
								idOwner[i] =  pPacket->getOwner(i);

							}
							 
							WORLD_POS	posCreate		= pPacket->getWorldPos();  //物品的位置

							BOOL  isAttributive = FALSE;
							CObject* pCharacterObj ;
							for (UINT i  =0 ;i< num ;++i)
							{
							    //pCharacterObj = (CObject*)(pObjectManager->FindServerObject(idOwner[i] ));

								//if( pCharacterObj == (CObject*)CObjectManager::GetMe()->GetMySelf() )
								CObject_PlayerNPC* pMyObj = (CObject_PlayerNPC*)CObjectManager::GetMe()->GetMySelf();
								GUID_t MyGUID = pMyObj->GetServerGUID();
								if (idOwner[i] == MyGUID)

								//if(CObjectManager::GetMe()->GetMySelf()->GetServerID() == idOwner[i])  
								{
									isAttributive =TRUE;
									break;

								}
							}
					 				
								CObject_Item* pItemObj = CObject_Item::NewItem(pPacket->getItem());
								if(!pItemObj) return PACKET_EXE_CONTINUE;
								pItemObj->SetTypeOwner(tObject_Item::IO_ITEMDROP); 

				
								//CDataPool::GetMe()->ItemDrop_SetItem(pPacket->getItem(),pItemObj);
								CDataPool::GetMe()->ItemDrop_SetItem(pPacket->getObjID(),pItemObj);
					
							//	pCharacter->AddDropItemEvent(idItemDrop, idOwner, &posCreate,pPacket->getProtectTime(),isAttributive);
								bMustCreater = FALSE;
								CSoundSystemFMod::_PlayUISoundFunc(67);

								// 产生掉落包
 
									// 创建掉落物品
								//CTripperObject_ItemDrop* pDrop = (CTripperObject_ItemDrop*)CObjectManager::GetMe()->NewObject( "CTripperObject_ItemDrop", pPacket->getItem());//idItemDrop
								CTripperObject_ItemDrop* pDrop = (CTripperObject_ItemDrop*)CObjectManager::GetMe()->NewObject( "CTripperObject_ItemDrop", pPacket->getObjID());
								pDrop->SetDropCanPick(isAttributive);

								pDrop->Initial(NULL);	

									// 设置位置
								pDrop->SetMapPosition( fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ));

									// 设置掉落箱的归属
								pDrop->SetOwnerGUID(idOwner);

								pDrop->SetProtectTime(pPacket->getProtectTime());
								pDrop->SetStartTime(timeGetTime());
								 

						}
					
				} 
				
				if(bMustCreater)
					{
							//	//创建ItemDrop
							//CTripperObject_ItemDrop* pDrop = (CTripperObject_ItemDrop*)CObjectManager::GetMe()->NewObject( "CTripperObject_ItemDrop", pPacket->getObjID());
							//pDrop->Initial(NULL);	
							//	//设置位置
							//pDrop->SetMapPosition( fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ));

							//GUID_t idOwner[MAX_TEAM_MEMBER];
							//for (int i  =0 ;i< MAX_TEAM_MEMBER ;++i)
							//{
							//	idOwner[i] = -1;
							//}

							//INT  num =pPacket->getOwnerCount(); //拥有者

							//for (int i  =0 ;i< num ;++i)
							//{
							//	idOwner[i] =  pPacket->getOwner(i);
							//}
							//	//设置掉落箱的归属
							//pDrop->SetOwnerGUID(idOwner);
							//pDrop->SetProtectTime(pPacket->getProtectTime());
							//pDrop->SetStartTime(timeGetTime());
					}

				}

		
			}

		return PACKET_EXE_CONTINUE ;

		__LEAVE_FUNCTION

			return PACKET_EXE_ERROR ;
}