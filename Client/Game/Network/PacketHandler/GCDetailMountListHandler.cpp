//[7/8/2010 陈军龙]
#include "StdAfx.h"
#include "GCDetailMountList.h"
#include "Procedure\GameProcedure.h"
#include "GameCommand.h"
#include "DataPool\DataPoolCommandDef.h"
#include "DataPool\GMDataPool.h"
#include "Object\Manager\ObjectManager.h"
#include "Global.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "Object\Logic\Character\Obj_PlayerOther.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "Event\GMEventSystem.h"
#include "Action\GMActionSystem.h"
#include "DBC\GMDataBase.h"
#include "GIException.h"
#include "Object\Logic\Item\Obj_Item_Mount.h"


uint GCDetailMountListHandler::Execute(GCDetailMountList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//AxTrace(0, 2, "得到装备的详细信息.");
		CObject* pObj = (CObject*)CObjectManager::GetMe()->FindServerObject(pPacket->getObjID());
		if(!pObj || !g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
		{
			return PACKET_EXE_CONTINUE;
		}

		CCharacterData* pCharacterData = ((CObject_Character*)pObj)->GetCharacterData();

		////刷入数据池
		//UINT dwMountMask = pPacket->GetPartFlags();

		_ITEM* pItemBuf = pPacket->getMountData();
		UINT dwMountMask = pPacket->getMountIndexFlag();

		//if( pObj != (CObject*)CObjectManager::GetMe()->GetMySelf() )
		//{
			for(INT i=0; i<MAX_MOUNT_SIZE; i++)
			{
		
				if(dwMountMask &(1<<i))
				{
			        if (pItemBuf[i].ItemClass() != ICLASS_MOUNT)
			            continue;

					CObject_Item_Mount* pItemObj = static_cast<CObject_Item_Mount*>( CObject_Item::NewItem(pItemBuf[i].m_ItemIndex) );

					if(NULL != pItemObj)
					{
					//	CDataPool::GetMe()->OtherPlayerMount_SetItem((HUMAN_Mount)i, NULL, TRUE);
					//}
					//else
					//{ 
						pItemObj->SetGUID( pItemBuf[i].m_ItemGUID.m_World, pItemBuf[i].m_ItemGUID.m_Server, pItemBuf[i].m_ItemGUID.m_Serial);
						pItemObj->SetTypeOwner(tObject_Item::IO_MYSELF_MOUNT);
						pItemObj->SetPosIndex(i);
						pItemObj->SetMountObtainDate(pItemBuf[i].GetMountData()->m_nObtainDate);
						pItemObj->SetMountIsBinded(pItemBuf[i].GetMountData()->m_bIsBinded);
						pItemObj->SetMountRemainTime(pItemBuf[i].GetMountData()->m_nRemainTime);

						pCharacterData->Set_Mount(i,pItemObj->GetMountCharID());
						CDataPool::GetMe()->UserMount_SetItem(i, pItemObj,TRUE);
						CDataPool::GetMe()->Mount_SetMountFakeModel(i, pItemObj->GetMountModelID());
						//刷新界面
						CEventSystem::GetMe()->PushEvent(GE_MOUNT_ATTR_UPDATE);

						//CDataPool::GetMe()->OtherPlayerMount_SetItem((HUMAN_Mount)i, pItemObj);
					}
				}
				//}
				//else
				//{
				//	CDataPool::GetMe()->OtherPlayerMount_SetItem((HUMAN_Mount)i, NULL, TRUE);
				//}

			//}
			
			//CObject_PlayerOther* TargetAvatar = CObjectManager::GetMe()->GetTarget_Avatar();
			//if (NULL == TargetAvatar)
			//	return PACKET_EXE_CONTINUE;

			//// 设置当前目标职业，更新UI模型时会用到
			//CObject_Character* pCharObj = (CObject_Character*)(CObjectManager::GetMe()->GetMainTarget());
			//if (pCharObj)
			//{
			//	TargetAvatar->GetCharacterData()->Set_MenPai(pCharObj->GetCharacterData()->Get_MenPai());
			//}			

			//// 刷新目标UI模型
			//TargetAvatar->GetCharacterData()->Set_RaceID( ((CObject_Character*)pObj)->GetCharacterData()->Get_RaceID() );
			//for(INT i=0; i<HMount_NUMBER; i++)
			//{
			//	TargetAvatar->GetCharacterData()->Set_Mount((HUMAN_Mount)i,
			//		((CObject_Character*)pObj)->GetCharacterData()->Get_Mount((HUMAN_Mount)i));
			//}
			//TargetAvatar->SetFaceDir(0.0f);
			//
			//CDataPool::GetMe()->Copy_To_TargetMount(pObj);

			//// 更新其他玩家身上的装备到ActionSystem
			//CActionSystem::GetMe()->OtherPlayerMount_Update();

			//// 通知界面事件
			//CEventSystem::GetMe()->PushEvent(GE_OTHERPLAYER_UPDATE_Mount);
		//}
		//else
		//{
		//	for(INT i=0; i<HMount_NUMBER; i++)
		//	{
		//		if(dwMountMask&(1<<i))
		//		{
		//			CObject_Item* pItemObj = CObject_Item::NewItem(pItemBuf[i].m_ItemIndex);
		//			if(!pItemObj) continue;

		//			pItemObj->SetGUID(	pItemBuf[i].m_ItemGUID.m_World,
		//				pItemBuf[i].m_ItemGUID.m_Server,
		//				pItemBuf[i].m_ItemGUID.m_Serial);

		//			pItemObj->SetExtraInfo(pItemBuf+i);
		//			pItemObj->SetTypeOwner(tObject_Item::IO_MYSELF_Mount);
		//			pItemObj->SetPosIndex(i);

		//			pCharacterData->Set_Mount((HUMAN_Mount)i,pItemObj->GetIdTable());

		//			CDataPool::GetMe()->UserMount_SetItem((HUMAN_Mount)i, pItemObj);
		//		}
		//	}

			// 更新主角身上的装备到ActionSystem
     		CActionSystem::GetMe()->UserMount_Update();

			// 通知界面事件
			//CEventSystem::GetMe()->PushEvent(GE_UPDATE_Mount);
		}
		
		
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}