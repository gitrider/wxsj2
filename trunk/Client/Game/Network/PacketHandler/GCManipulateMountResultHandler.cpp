//[7/8/2010 �¾���]
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
                        
                        // �������б��������
						CDataPool::GetMe()->UserMount_SetItem(mountIndex, itemMount, FALSE);
						CActionSystem::GetMe()->UserMount_Update();
                        // ɾ�������е�����
						CDataPool::GetMe()->UserBag_SetItem(bagIndex, NULL, FALSE);
						CActionSystem::GetMe()->UserBag_Update();

						// ����ӵ�е�����
						CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
						pCharacterData->Set_Mount(mountIndex, itemMount->GetMountCharID());
						
						itemMount->SetMountObtainDate(pPacket->getMountObtainDate());
                        itemMount->SetMountIsBinded(pPacket->getMountIsBinded());

						// ��������UI������ģ��
						CDataPool::GetMe()->Mount_SetMountFakeModel(mountIndex, itemMount->GetMountModelID());

						// ˢ�½���
						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
						CEventSystem::GetMe()->PushEvent(GE_MOUNT_ATTR_UPDATE);
						CEventSystem::GetMe()->PushEvent(GE_TOGLE_MOUNT_PAGE);

						// ��ʾ�������
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

						// ��ʾ�������
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

						// ��ʾ�������
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

						// ��������UI������ģ��
						CDataPool::GetMe()->Mount_SetMountFakeModel(mountIndex, INVALID_ID);
						CDataPool::GetMe()->UserMount_SetItem(mountIndex, NULL , TRUE);   

						// ����ӵ�е�����
						CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
						pCharacterData->Set_Mount(mountIndex, INVALID_ID);

						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
						CEventSystem::GetMe()->PushEvent(GE_MOUNT_ATTR_UPDATE);

						// ��ʾ�������
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

							// ����ǰ�������ڱ�������ٻػָ��ٶ�
							if (itemMount->GetMountCharID() == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID())
							{
								CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_MountID(INVALID_ID);
								CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_MountIndex(INVALID_INDEX);
								FLOAT currentSpeed = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MoveSpeed();
								CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_MoveSpeed(currentSpeed - itemMount->GetMountAddtionSpeed());
							}
						}

						// ��������UI������ģ��
						CDataPool::GetMe()->Mount_SetMountFakeModel(mountIndex, INVALID_ID);
						CDataPool::GetMe()->UserMount_SetItem(mountIndex, NULL , TRUE);  

						// ����ӵ�е�����
						CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
						pCharacterData->Set_Mount(mountIndex, INVALID_ID);

						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
						CEventSystem::GetMe()->PushEvent(GE_MOUNT_ATTR_UPDATE);

						// ��ʾ�������
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