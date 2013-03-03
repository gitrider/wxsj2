#include "stdafx.h"
#include "CGAskLockObj.h"

#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"

#include "GCAddLockObj.h"
#include "GCItemInfo.h"

uint	CGAskLockObjHandler::Execute(CGAskLockObj* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	
	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	
	BYTE LockType = pPacket->GetLockType();
	BYTE lockObj  = pPacket->GetLockObj();

	GCAddLockObj msg;

	// ���Ҫִ�м����Ȳ�ѯ����ǲ��ǻ����㹻�ľ�����Ϊһ����Ʒ���߳���ӱ�������Ҫ10�㾫����
	if(pPacket->GetLockType() == CGAskLockObj::OPR_LOCK)
	{
		if( LOCK_A_OBJ_NEED_ENERGY > pHuman->GetEnergy() )
		{
			msg.SetLockObj(GCAddLockObj::LOCK_ITEM);
			msg.SetResult(GCAddLockObj::RESULT_NO_JINGLI);

			pHuman->GetPlayer()->SendPacket(&msg);

			return PACKET_EXE_CONTINUE ;
		}
	}

	switch(lockObj)
	{
	case CGAskLockObj::LOCK_ITEM:	//��Ʒ
		{
			msg.SetLockObj(GCAddLockObj::LOCK_ITEM);

			INT nItemIndex = HumanItemLogic::GetBagItemPosByGUID( pHuman, pPacket->GetItemGUID() );

			if(INVALID_ID == nItemIndex)
			{
				return PACKET_EXE_CONTINUE ;
			}

			Item* pItem = HumanItemLogic::GetBagItem( pHuman, nItemIndex );
			if(!pItem)
			{
				return PACKET_EXE_CONTINUE ;
			}

			ItemContainer* pCon = HumanItemLogic::GetBagContainer( pHuman, nItemIndex );
			if(!pCon)
			{
				return PACKET_EXE_CONTINUE ;
			}

			if(LockType == CGAskLockObj::OPR_LOCK)			//����
			{
				if(g_ItemOperator.SetItemPWLock(pCon, pCon->BagIndex2ConIndex(nItemIndex), TRUE))
				{
					//�۳�����
					pHuman->SetEnergy(pHuman->GetEnergy() - LOCK_A_OBJ_NEED_ENERGY);

					msg.SetResult(GCAddLockObj::RESULT_LOCK_OK);

					GCItemInfo  MsgItem;
					MsgItem.setIsNull(FALSE);
					MsgItem.setID((WORD)nItemIndex);
					pItem->SaveValueTo(MsgItem.getItem());
					pHuman->GetPlayer()->SendPacket(&MsgItem);
				}
			}
			else if(LockType == CGAskLockObj::OPR_UNLOCK)	//ȥ��
			{
				if(g_ItemOperator.SetItemPWLock(pCon, pCon->BagIndex2ConIndex(nItemIndex), FALSE))
				{
					msg.SetResult(GCAddLockObj::RESULT_UNLOCK_OK);

					GCItemInfo  MsgItem;
					MsgItem.setIsNull(FALSE);
					MsgItem.setID((WORD)nItemIndex);
					pItem->SaveValueTo(MsgItem.getItem());
					pHuman->GetPlayer()->SendPacket(&MsgItem);
				}
			}
		}
		break;
	case CGAskLockObj::LOCK_PET:	//����
		{
			msg.SetLockObj(GCAddLockObj::LOCK_PET);

		}
		break;
	default:
		break;
	}

	pHuman->GetPlayer()->SendPacket(&msg);

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}