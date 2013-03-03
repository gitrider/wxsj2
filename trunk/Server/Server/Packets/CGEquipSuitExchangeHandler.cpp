#include "stdafx.h"
#include "CGEquipSuitExchange.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "GCCharEquipment.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCEquipSuitExchangeResult.h"
#include "ItemRuler.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"
#include "GCDetailEquipList.h"


using namespace Packets;

uint CGEquipSuitExchangeHandler::Execute(CGEquipSuitExchange* pPacket, Player* pPlayer )
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

	//����״̬���ɲ���
	if(pHuman->m_ExchangBox.m_Status > 0)
	{//����
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: ObjID=%d, ExchangBox::m_Status>0" ,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}
	//��̯״̬���ɲ���
	if(pHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{//����
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: ObjID=%d, ServerStallBox::STALL_OPEN" ,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}

	//�г���ʱ������װ��Ϣ
	if(pGamePlayer->GetPlayerStatus()!=PS_SERVER_NORMAL ||
		!pHuman->IsActiveObj() )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipSuitExchangeHandler: change scene") ;
		return PACKET_EXE_CONTINUE;
	}
	//��̯���ɲ���
	if(pHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{//����
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: ObjID=%d, ServerStallBox::STALL_OPEN" ,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}

	UINT  nSuitNum = pPacket->getEquipSuitNum();

	if( nSuitNum > MAX_EQUIP_SUIT_NUM )
	{
		return PACKET_EXE_ERROR ;
	}

	ItemContainer* pEquipContainer = pHuman->GetEquipContain();
	
	Assert(pEquipContainer);
	_SUIT_SETTING suitSetting = pHuman->GetEquipSuitSetting(nSuitNum);

	GCEquipSuitExchangeResult Msg;
	GCDetailEquipList	SelfMsg;
	GCCharEquipment OtherMsg;
	SelfMsg.setObjID( pHuman->GetID() );
	BOOL bChange = FALSE;
	UINT RetPart = 0;

	//����������װ
	for(INT i=0; i<HEQUIP_NUMBER; ++i)
	{
		//������װ����
		if(!suitSetting.m_EquipData[i].isNull())
		{
			//���װ����
			Item* pEquip = pEquipContainer->GetItem(i);
			Assert(pEquip);

			//װ���б仯��װ���������뱣��Ķ�Ӧλ����װ��Ϣ��һ�£���Ҫ����������
			if(!(pEquip->GetGUID() == suitSetting.m_EquipData[i]))
			{
				INT nPos = INVALID_INDEX;
				ItemContainer* pBagContainer = HumanItemLogic::GetBagItemContain(pHuman, &suitSetting.m_EquipData[i], nPos);
				//������Ҳ�Ҳ��������¼�����������ˣ������ˣ�������
				if(nPos == INVALID_INDEX)
				{
					EXCHANGE_EQUIP_INFO ExchangeInfo;
					ExchangeInfo.m_BagIndex = INVALID_ITEM_POS;
					ExchangeInfo.m_EquipPoint	= INVALID_ITEM_POS;
					Msg.setEquipInfo(ExchangeInfo, i);
					continue;
				}
				
				//�ҵ���
				Item*	pExchangeItem = HumanItemLogic::GetBagItem(pHuman, nPos);
				
				Assert(pExchangeItem->GetItemClass() == ICLASS_EQUIP);
				if(pExchangeItem->GetEquipPoint() != i)
				{
					if(pExchangeItem->GetEquipPoint() == HEQUIP_RING1)
					{
						Assert(i == HEQUIP_RING2);
					}
					else if(pExchangeItem->GetEquipPoint() == HEQUIP_ADORN1)
					{
						Assert(i == HEQUIP_ADORN2);
					}
					else
						Assert(0);
				}
				//��ҿ��ܵȼ�����
				if(pExchangeItem->GetRequireLevel() > pHuman->GetLevel() )
				{
					EXCHANGE_EQUIP_INFO ExchangeInfo;
					ExchangeInfo.m_BagIndex = INVALID_ITEM_POS;
					ExchangeInfo.m_EquipPoint	= i;
					Msg.setEquipInfo(ExchangeInfo, i);
					g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipSuitExchangeHandler: Equiplevel , pEquip->GetRequireLevel()=%d", pEquip->GetRequireLevel() ) ;
					continue;
				}
				
				//���ܻ�ְҵ��
				if(!pExchangeItem->IsWuMenPai())
				{
					if(!pExchangeItem->InReqJob(pHuman->GetMenPai()))
					{
						EXCHANGE_EQUIP_INFO ExchangeInfo;
						ExchangeInfo.m_BagIndex = INVALID_ITEM_POS;
						ExchangeInfo.m_EquipPoint	= i;
						Msg.setEquipInfo(ExchangeInfo, i);
						g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipSuitExchangeHandler: ReqJob") ;
						continue;
					}
				}
				if (pEquip->IsEmpty())
				{
					g_ItemOperator.MoveItem( pBagContainer,
						pBagContainer->BagIndex2ConIndex(nPos),
						pEquipContainer,
						i ) ;
				}
				
				else
				{
					if (pBagContainer->IsCanUse())
					{
						//��װ������Ʒ���н���
						g_ItemOperator.ExchangeItem(pBagContainer,
							pBagContainer->BagIndex2ConIndex(nPos),
							pEquipContainer,
							i);
					}
					else //�����Ѿ�����
					{
						EXCHANGE_EQUIP_INFO ExchangeInfo;
						ExchangeInfo.m_BagIndex = INVALID_ITEM_POS;
						ExchangeInfo.m_EquipPoint	= INVALID_ITEM_POS;
						Msg.setEquipInfo(ExchangeInfo, i);
						continue;
					}
				}
				EXCHANGE_EQUIP_INFO ExchangeInfo;
				ExchangeInfo.m_BagIndex = nPos;
				ExchangeInfo.m_EquipPoint	= i;
				Msg.setEquipInfo(ExchangeInfo, i);

				//�������������ˣ����»�ȡһ��
				Item* pEquipItem = HumanItemLogic::GetEquip(pHuman, (HUMAN_EQUIP)i);
				if(!pEquipItem)
				{
					Assert(FALSE);
					return PACKET_EXE_CONTINUE;
				}
				Assert(!pEquipItem->IsEmpty()); 

				pEquipItem->SaveValueTo(SelfMsg.GetEquipData((HUMAN_EQUIP)i));
				RetPart|=(1<<((UINT)i));
				SelfMsg.SetAskPart(RetPart);
				RetPart = 0;
				//����ɼ�
				if(pHuman->IsVisualPart((HUMAN_EQUIP)i))
				{	
					OtherMsg.setObjID(pHuman->GetID());
					OtherMsg.setID((HUMAN_EQUIP)i, pEquipItem->GetItemTableIndex());
				}

				//��װ��
				bChange = TRUE;
			}

			//װ���������أ�����Ҫ֪ͨ
			continue;
		}
		else
		{
			//����û��ѽ
			EXCHANGE_EQUIP_INFO ExchangeInfo;
			ExchangeInfo.m_BagIndex = INVALID_ITEM_POS;
			ExchangeInfo.m_EquipPoint	= i;
			Msg.setEquipInfo(ExchangeInfo, i);
		}
	}

	
	if(bChange)
	{
		Msg.setResult(EQUIPSUIT_SUCCESS);
		pGamePlayer->SendPacket(&Msg);
		pGamePlayer->SendPacket( &SelfMsg ) ;
		pScene->BroadCast(&OtherMsg,pHuman,TRUE);
		pHuman->SetEquipVer(pHuman->GetEquipVer()+1);
		//ˢ��װ�������������Ӱ��

		pHuman->ItemEffectFlush();
	}
	else
	{
		Msg.setResult(EQUIPSUIT_SUCCESS);
		pGamePlayer->SendPacket(&Msg);
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipSuitExchangeHandler: nSuitNum=%d", nSuitNum ) ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}