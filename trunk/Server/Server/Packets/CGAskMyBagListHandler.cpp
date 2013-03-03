#include "stdafx.h"
#include "CGAskMyBagList.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "GCMyBagList.h"
#include "HumanItemLogic.h"
#include "GCSouXiaList.h"
#include "SouXia.h"

using namespace Packets;


uint CGAskMyBagListHandler::Execute(CGAskMyBagList* pPacket, Player* pPlayer )
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

	
	ASK_BAG_MODE	mode		=	pPacket->GetAskMode();

	GCMyBagList Msg;

	switch(mode) 
	{
	case ASK_ALL:
		{
			_BAG_ITEM		ItemIterator;
			_ITEM_GUID		NullGuid;
			uint			AllAskCount = 0;
			uint            nItemCount  = 0;
			memset(&NullGuid,0,sizeof(_ITEM_GUID));
			

			Msg.SetAskMode(ASK_ALL);

			//��ȡ��չ����
			ItemContainer* pExtraContainer = pHuman->GetExtraContain();
			Assert(pExtraContainer);
			for( INT i=0; i<pExtraContainer->GetContainerSize(); i++ )
			{
				//��ʱ����
				Item*	pItem =	HumanItemLogic::GetExtraContainerItem(pHuman,pExtraContainer->ConIndex2BagIndex(i));
				Assert(pItem);

				if(!(pItem->IsEmpty()))
				{
					ItemIterator.m_nndex			=	pExtraContainer->ConIndex2BagIndex(i);
					pItem->SaveValueTo(&ItemIterator.m_nItemData);
					Msg.SetAskItemData(&ItemIterator,nItemCount);
					++nItemCount;
				}

			}
			//��ȡ��������
			ItemContainer* pBaseContainer = pHuman->GetBaseContain();
			Assert(pBaseContainer);
			for( INT i=0; i<pBaseContainer->GetContainerSize(); i++ )
			{
				//��ʱ����
				Item*	pItem =	pBaseContainer->GetItem(i);
				Assert(pItem);

				if(!(pItem->IsEmpty()))
				{
					ItemIterator.m_nndex			=	pBaseContainer->ConIndex2BagIndex(i);
					pItem->SaveValueTo(&ItemIterator.m_nItemData);
					Msg.SetAskItemData(&ItemIterator,nItemCount);
					++nItemCount;
				}
			}

			//��ȡ��չ����
			for (INT j=0; j<MAX_EXTRA_BAG_NUM; ++j)
			{
				ItemContainer* pBaseContainer = pHuman->GetExtraBagContain(j);
				Assert(pBaseContainer);
				if (pBaseContainer->IsValid())
				{
					for( INT i=0; i<pBaseContainer->GetContainerSize(); i++ )
					{
						//��ʱ����
						Item*	pItem =	pBaseContainer->GetItem(i);
						Assert(pItem);

						if(!(pItem->IsEmpty()))
						{
							ItemIterator.m_nndex			=	pBaseContainer->ConIndex2BagIndex(i);
							pItem->SaveValueTo(&ItemIterator.m_nItemData);
							Msg.SetAskItemData(&ItemIterator,nItemCount);
							++nItemCount;
						}
					}
				}
			}

			Msg.SetItemCount(nItemCount);

			pGamePlayer->SendPacket(&Msg);
			
		}
		break;
	case ASK_SET:
		{
			//_BAG_ITEM	ItemIterator;
			//BYTE		askIndex ;
			//Msg.SetAskMode(ASK_SET);
			////���õ�ǰ�����������С
			//if(askCount>MAX_BAG_SIZE)	askCount= MAX_BAG_SIZE;

			//Msg.SetAskCount(askCount);

			//for(INT i=0;i<askCount;i++)
			//{

			//	
			//	askIndex						=	pPacket->GetAskItemIndex(i);	
			//	ItemIterator.m_nndex			=	askIndex;

			//	Item*	pItem =	HumanItemLogic::GetBagItem(pHuman,i);
			//	Assert(pItem);

			//	ItemIterator.m_ItemID			=	pItem->GetGUID();
			//	ItemIterator.m_ItemTableIndex	=	pItem->GetItemTableIndex();
			//	ItemIterator.m_Count			=	pItem->GetLayedNum();
			//	Msg.SetAskItemData(&ItemIterator,i);
			//}

			//pGamePlayer->SendPacket(&Msg);
		}
		break;
	default:
		break;
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskMyBagListHandler: mode=%d ",
		mode) ;

	// ������ҵ�ǰ����¼�б�,��ʱ���ÿͻ�������
	GCSouXiaList	souXiaListMsg;
	BYTE curSouXia = pHuman->GetCurSouXiaCount();
	souXiaListMsg.SetSouXiaCount(curSouXia);

	BYTE  iCount = 0;
	for (int i=0; i<MAX_SOUXIA_CONTAINER; ++i)
	{
		SouXia souXia	= pHuman->GetSouXia(i);
		UINT   souXiaID = souXia.GetSouXiaData().m_SouXiaID;
		if (souXiaID > 0)
		{
			SHORT curPos = souXia.GetSouXiaData().GetCurPos();
			Assert(curPos >= 0 );

			SOUXIA_DATA	souXiaData = souXia.GetSouXiaData();
			souXiaListMsg.SetSouXiaData(&souXiaData, iCount);//ע��ȡ����������Ӧ�����
			iCount++;
		}
	}
	Assert(iCount == curSouXia);
	pGamePlayer->SendPacket(&souXiaListMsg);

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}