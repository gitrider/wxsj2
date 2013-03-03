

#include "stdafx.h"
#include "CGSplitItem.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"
#include "ItemOperator.h"
#include "HumanItemLogic.h"
#include "GCSplitItemResult.h"
#include "GCItemInfo.h"
#include "GCBankItemInfo.h"


uint CGSplitItemHandler::Execute( CGSplitItem* pPacket, Player* pPlayer )
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

	INT nCon = pPacket->GetContainer();
	INT nSplitNum = pPacket->GetNumber();
	if (nSplitNum <= 0)
	{
		return PACKET_EXE_CONTINUE;
	}
	GCSplitItemResult msg;

	switch(pPacket->GetContainer())
	{
	case CGSplitItem::BAG_CON:
		{
			Item* pBagItem = HumanItemLogic::GetBagItem(pHuman,pPacket->GetPosition()); 
			if (pBagItem == NULL)
			{
				return PACKET_EXE_ERROR;
			}

			if(pBagItem->IsEmpty())
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGSplitItemHandler: ok BagIndex=%d  occur null type ",	pPacket->GetPosition() ) ;

				//������Ϣ���߿ͻ��ˣ�û�пո���
				msg.setResult(GCSplitItemResult::RESULT_FALSE_NOGRID);
				pHuman->GetPlayer()->SendPacket(&msg);

				return PACKET_EXE_CONTINUE;
			}

			ItemContainer* pBagContainer = HumanItemLogic::GetBagContainer(pHuman,pPacket->GetPosition());
			if (!pBagContainer->IsCanUse())
			{
				msg.setResult(GCSplitItemResult::RESULT_FALSE_NOGRID);
				pHuman->GetPlayer()->SendPacket(&msg);

				return PACKET_EXE_CONTINUE;
			}
			//��Ҫ�ҵ�һ���ܹ����õĿո�
			INT nEmptyIndex = pBagContainer->GetEmptyItemIndex();
			if( INVALID_INDEX == nEmptyIndex)
			{
				msg.setResult(GCSplitItemResult::RESULT_FALSE_NOGRID);
				pHuman->GetPlayer()->SendPacket(&msg);

				return PACKET_EXE_CONTINUE;
			}

			if( TRUE == HumanItemLogic::SplitItem(pHuman,
									  pBagContainer,
									  pBagContainer->BagIndex2ConIndex(pPacket->GetPosition()),
									  pPacket->GetNumber(),
									  pBagContainer,
									  nEmptyIndex))
			{

				//���ͳɹ���Ϣ
				Item* pItem = pBagContainer->GetItem(nEmptyIndex);
				Assert(pItem);
				pItem->SaveValueTo(msg.getItem());
				msg.setResult(GCSplitItemResult::RESULT_SUCCEED);
				msg.setContainer(GCSplitItemResult::BAG_CON);
				msg.setIsNull(FALSE);
				msg.setConIndex(pBagContainer->ConIndex2BagIndex(nEmptyIndex));
				pHuman->GetPlayer()->SendPacket(&msg);
				

				Item* pItemOld = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(pPacket->GetPosition()));
				GCItemInfo  Msg;
				Msg.setIsNull(FALSE);
				Msg.setID((WORD)pPacket->GetPosition());
				pItemOld->SaveValueTo(Msg.getItem());
				pHuman->GetPlayer()->SendPacket(&Msg);

			}
			else
			{
				//����ʧ����Ϣ
				msg.setResult(GCSplitItemResult::RESULT_FALSE);
				pHuman->GetPlayer()->SendPacket(&msg);
			}
		}
		break;
	case CGSplitItem::BANK_CON:
		{
			ItemContainer* pBankContainer = pHuman->GetBankContain();

			INT nIndex = pPacket->GetPosition();

			INT nEmptyIndex = 0;
			//�Զ������ո�������1
			if( nIndex >= RENTBOX1_START_INDEX && nIndex < RENTBOX2_START_INDEX )
			{
				BYTE	indextemp = RENTBOX1_START_INDEX;
				//HumanItemLogic::GetBankItem(pHuman, indextemp);

				while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp))  
					&&  indextemp < RENTBOX2_START_INDEX
					)
				{
					indextemp++;
				}

				if(indextemp == RENTBOX2_START_INDEX)
				{
					//ʧ��
					nEmptyIndex = -1;
				}
				else
				{
					nEmptyIndex = indextemp;
				}
			}

			if( nIndex >= RENTBOX2_START_INDEX && nIndex < RENTBOX3_START_INDEX )
			{
				BYTE	indextemp = RENTBOX2_START_INDEX;
				while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp)) 
					&&  indextemp < RENTBOX3_START_INDEX
					)
				{
					indextemp++;
				}

				if(indextemp == RENTBOX3_START_INDEX)
				{
					//ʧ��
					nEmptyIndex = -1;
				}
				else
				{
					nEmptyIndex = indextemp;
				}
			}

			if( nIndex >= RENTBOX3_START_INDEX && nIndex < RENTBOX4_START_INDEX )
			{
				BYTE	indextemp = RENTBOX3_START_INDEX;
				while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp)) 
					&&  indextemp < RENTBOX4_START_INDEX
					)
				{
					indextemp++;
				}

				if(indextemp == RENTBOX4_START_INDEX)
				{
					//ʧ��
					nEmptyIndex = -1;
				}
				else
				{
					nEmptyIndex = indextemp;
				}
			}

			if( nIndex >= RENTBOX4_START_INDEX && nIndex < RENTBOX5_START_INDEX )
			{
				BYTE	indextemp = RENTBOX4_START_INDEX;
				while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp)) 
					&&  indextemp < RENTBOX5_START_INDEX
					)
				{
					indextemp++;
				}

				if(indextemp == RENTBOX5_START_INDEX)
				{
					//ʧ��
					nEmptyIndex = -1;
				}
				else
				{
					nEmptyIndex = indextemp;
				}
			}

			if( nIndex == RENTBOX5_START_INDEX && nIndex < MAX_BANK_SIZE )
			{
				BYTE	indextemp = RENTBOX5_START_INDEX;
				while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp)) 
					&&  indextemp < MAX_BANK_SIZE
					)
				{
					indextemp++;
				}

				if(indextemp == MAX_BANK_SIZE)
				{
					//ʧ��
					nEmptyIndex = -1;
				}
				else
				{
					nEmptyIndex = indextemp;
				}
			}

			// ֻ�ڵ�ǰ��ҳ���ڲ���һ���ո�
			//INT nEmptyIndex = pBankContainer->GetEmptyItemIndex();
			

			if( -1 == nEmptyIndex )
			{
				// ֪ͨ�ͻ��ˣ�û����Ҫ�Ŀո�
				msg.setResult(GCSplitItemResult::RESULT_FALSE_NOGRID);
				pHuman->GetPlayer()->SendPacket(&msg);

				return PACKET_EXE_CONTINUE;
			}
			
			ITEM_LOG_PARAM	ItemLogParam;
			if(g_ItemOperator.SplitItem(&ItemLogParam,pBankContainer,(UCHAR)pPacket->GetPosition(),
				(UCHAR)pPacket->GetNumber(),pBankContainer,nEmptyIndex)==ITEMOE_SUCCESS)
			{
				//���ͳɹ���Ϣ
				Item* pItem = pBankContainer->GetItem(nEmptyIndex);
				Assert(pItem);
				pItem->SaveValueTo(msg.getItem());
				msg.setResult(GCSplitItemResult::RESULT_SUCCEED);
				msg.setContainer(GCSplitItemResult::BANK_CON);
				msg.setIsNull(FALSE);
				msg.setConIndex(nEmptyIndex);
				pHuman->GetPlayer()->SendPacket(&msg);

				Item* pItemOld = pBankContainer->GetItem(pPacket->GetPosition());
				GCBankItemInfo  Msg;
				Msg.setIsNull(FALSE);
				Msg.setBankIndex((WORD)pPacket->GetPosition());
				pItemOld->SaveValueTo(Msg.getItem());
				pHuman->GetPlayer()->SendPacket(&Msg);

			}
			else
			{
				//����ʧ����Ϣ
				msg.setResult(GCSplitItemResult::RESULT_FALSE);
				pHuman->GetPlayer()->SendPacket(&msg);
			}
		}
		break;
	default:
		break;
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
