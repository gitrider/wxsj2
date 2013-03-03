//[7/8/2010 陈军龙]
#include "StdAfx.h"
#include "GCManipulateMountResult.h"
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
#include "..\Game\Interface\GMGameInterface_Script_SouXia.h"
#include "DBC\GMDataBase.h"
#include "Object\Logic\Item\Obj_Item_Mount.h"
#include "..\..\Interface\GMInterface_Script_Talk.h"
#include "GITimeSystem.h"

using namespace Packets;


uint GCManipulateMountResultHandler::Execute(GCManipulateMountResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	BOOL bLog = (CGameProcedure::s_pUISystem != NULL) ? TRUE : FALSE;
	BYTE bagIndex = pPacket->getBagIndex();
	BYTE mountIndex = pPacket->getMountIndex();

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
	    switch(pPacket->getMountOperator())
		{
			case GCManipulateMountResult::MANIPULATE_EQUIP_MOUNT:
				{
					if (pPacket->getResult())
					{
					    CObject_Item_Mount* itemMount = static_cast<CObject_Item_Mount*>( CDataPool::GetMe()->UserBag_GetItem(bagIndex) );                      
						KLAssert( (NULL != itemMount) && (ICLASS_MOUNT == itemMount->GetItemClass()) );
                        
                        // 在坐骑列表添加坐骑
						CDataPool::GetMe()->UserMount_SetItem(mountIndex, itemMount, FALSE);
						CActionSystem::GetMe()->UserMount_Update();
                        // 删除背包中的坐骑
						CDataPool::GetMe()->UserBag_SetItem(bagIndex, NULL, FALSE);
						CActionSystem::GetMe()->UserBag_Update();

						// 更新拥有的坐骑
						CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
						pCharacterData->Set_Mount(mountIndex, itemMount->GetMountCharID());
						
						itemMount->SetMountObtainDate(pPacket->getMountObtainDate());
                        itemMount->SetMountIsBinded(pPacket->getMountIsBinded());

						// 更新坐骑UI的坐骑模型
						CDataPool::GetMe()->Mount_SetMountFakeModel(mountIndex, itemMount->GetMountModelID());

						// 刷新界面
						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
						CEventSystem::GetMe()->PushEvent(GE_MOUNT_ATTR_UPDATE);
						CEventSystem::GetMe()->PushEvent(GE_TOGLE_MOUNT_PAGE);

						// 显示到聊天框
						if(bLog)
						{
						    STRING szMsg = COLORMSGFUNC("Mount_Equip_Success", itemMount->GetName());;
						    ADDTALKMSG(szMsg);
						}
					}
				}
				break;

			case GCManipulateMountResult::MANIPULATE_CALL_MOUNT:
				{
					if (pPacket->getResult())
					{
						CObject_Item_Mount* itemMount = static_cast<CObject_Item_Mount*>( CDataPool::GetMe()->UserMount_GetItem(mountIndex) );      

                        if (NULL != itemMount)
                        {
							CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_MountID(itemMount->GetMountCharID());
							CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_MountIndex(mountIndex);
						    FLOAT currentSpeed = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MoveSpeed();
							CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_MoveSpeed(currentSpeed + itemMount->GetMountAddtionSpeed());
                        }

						// 显示到聊天框
						if(bLog)
						{
							STRING szMsg = COLORMSGFUNC("Mount_Call_Success", itemMount->GetName());;
							ADDTALKMSG(szMsg);
						}
					}
				}
				break;

		    case GCManipulateMountResult::MANIPULATE_RECALL_MOUNT:
				{
					if (pPacket->getResult())
					{
						CObject_Item_Mount* itemMount = static_cast<CObject_Item_Mount*>( CDataPool::GetMe()->UserMount_GetItem(mountIndex) );      

						if (NULL != itemMount)
						{
							CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_MountID(INVALID_ID);
							CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_MountIndex(INVALID_INDEX);
							FLOAT currentSpeed = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MoveSpeed();
							CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_MoveSpeed(currentSpeed - itemMount->GetMountAddtionSpeed());
						}

						// 显示到聊天框
						if(bLog)
						{
							STRING szMsg = COLORMSGFUNC("Mount_Recall_Success", itemMount->GetName());;
							ADDTALKMSG(szMsg);
						}
					}
				}
				break;

		    case GCManipulateMountResult::MANIPULATE_DISCARD_MOUNT:
				{
					if (pPacket->getResult())
					{
						CObject_Item_Mount* itemMount = static_cast<CObject_Item_Mount*>( CDataPool::GetMe()->UserMount_GetItem(mountIndex) );      
						LPCTSTR mountName = itemMount->GetName();

						// 更新坐骑UI的坐骑模型
						CDataPool::GetMe()->Mount_SetMountFakeModel(mountIndex, INVALID_ID);
						CDataPool::GetMe()->UserMount_SetItem(mountIndex, NULL , TRUE);   

						// 更新拥有的坐骑
						CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
						pCharacterData->Set_Mount(mountIndex, INVALID_ID);

						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
						CEventSystem::GetMe()->PushEvent(GE_MOUNT_ATTR_UPDATE);

						// 显示到聊天框
						if(bLog)
						{
							STRING szMsg = COLORMSGFUNC("Mount_Discard_Success", mountName);;
							ADDTALKMSG(szMsg);
						}
					}
				}
				break;

			case GCManipulateMountResult::MANIPULATE_DESTROY_MOUNT:
				{
					if (pPacket->getResult())
					{
						CObject_Item_Mount* itemMount = static_cast<CObject_Item_Mount*>( CDataPool::GetMe()->UserMount_GetItem(mountIndex) );      
						LPCTSTR mountName;

						if (NULL != itemMount)
						{
							mountName = itemMount->GetName();

							// 若当前坐骑正在被乘骑，则召回恢复速度
							if (itemMount->GetMountCharID() == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID())
							{
								CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_MountID(INVALID_ID);
								CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_MountIndex(INVALID_INDEX);
								FLOAT currentSpeed = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MoveSpeed();
								CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_MoveSpeed(currentSpeed - itemMount->GetMountAddtionSpeed());
							}
						}

						// 更新坐骑UI的坐骑模型
						CDataPool::GetMe()->Mount_SetMountFakeModel(mountIndex, INVALID_ID);
						CDataPool::GetMe()->UserMount_SetItem(mountIndex, NULL , TRUE);  

						// 更新拥有的坐骑
						CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
						pCharacterData->Set_Mount(mountIndex, INVALID_ID);

						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
						CEventSystem::GetMe()->PushEvent(GE_MOUNT_ATTR_UPDATE);

						// 显示到聊天框
						if(bLog)
						{
							STRING szMsg = COLORMSGFUNC("Mount_Delete_For_Due", mountName);;
							ADDTALKMSG(szMsg);
						}
					}
				}
				break;

		    default:break;
		}


	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}