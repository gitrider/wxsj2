
#include "stdafx.h"
#include "CGPackUpPacket.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "GCItemInfo.h"
#include "ItemOperator.h"
#include "HumanItemLogic.h"
#include "GCPackUpPacket.h"

uint CGPackUpPacketHandler::Execute(CGPackUpPacket* pPacket, Player* pPlayer )
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
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	GCPackUpPacket msg;
	pHuman->GetPlayer()->SendPacket(&msg);

	//// �������
	//INT nConIndex = pPacket->GetConIndex();

	//ItemContainer* pContainer;

	//switch(nConIndex)
	//{
	//case 0://����
	//	{
	//		//pContainer = HumanItemLogic::GetContainer(pHuman,1);
	//	}
	//	break;
	//case 1://����
	//	{
	//		//pContainer = HumanItemLogic::GetContainer(pHuman,21);
	//	}
	//	break;
	//case 2://����
	//	{
	//		//pContainer = HumanItemLogic::GetContainer(pHuman,41);
	//	}
	//	break;
	//default:
	//	{
	//		pContainer = NULL;
	//	}
	//	break;
	//}

	//if(!pContainer)
	//{
	//	Assert(FALSE);
	//}

	//// �Ƿ���Ҫ���͸������ݵı�־
	//BOOL  bHasChange = FALSE;

	//// ��ʼ����----------------------------------------------------------------------
	//// 1���ϲ������ܹ��ϲ�����Ʒ����һ�������п��ܻᷢ����Ʒ���ݵĸı䣬
	////    ���ǲ�������µ���Ʒ�����������µ�GUID
	//for(INT i=0; i<20; i++)
	//{
	//	Item *pItem1 = pContainer->GetItem(i);

	//	if(pItem1)
	//	{
	//		if(!pItem1->IsEmpty())
	//		if(pItem1->IsCanLay())
	//		{
	//			for(INT j=0; j<20; j++)
	//			{
	//				if(i != j)
	//				{
	//					Item *pItem2 = pContainer->GetItem(j);
	//					if(pItem2)
	//					{
	//						if(!pItem2->IsEmpty())
	//						if( pItem1->GetItemTableIndex() == pItem2->GetItemTableIndex() )
	//						{
	//							//������Ʒ��ͬ�������ǿ��Ե��ŵ���Ʒ
	//							if(pItem1->GetLayedNum() < pItem1->GetMaxLayedNum())
	//							{
	//								if(pItem2->GetLayedNum() < pItem2->GetMaxLayedNum())
	//								{
	//									g_ItemOperator.MoveSpliceItem(pContainer,(UINT)j,pContainer,(UINT)i);
	//									bHasChange = TRUE;
	//								}
	//							}
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	//// �ϲ���ɣ�

	//// 2������һ���������λ�ã������ﲻ���ٳ�����Ʒ�ڲ����ݸı�Ĳ���
	//// ���������ȴ��߻��ṩ��ϸ���򡭡�����


	////֪ͨ�ͻ��˸���
	//if(bHasChange)
	//{
	//	for(INT i=0;i<20;i++)
	//	{
	//		Item* pItem = pContainer->GetItem(i);

	//		GCItemInfo  Msg;
	//		Msg.setIsNull(pItem->IsEmpty());
	//		Msg.setID((WORD)pContainer->ConIndex2BagIndex(i));
	//		pItem->SaveValueTo(Msg.getItem());
	//		pHuman->GetPlayer()->SendPacket(&Msg);
	//	}
	//}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}