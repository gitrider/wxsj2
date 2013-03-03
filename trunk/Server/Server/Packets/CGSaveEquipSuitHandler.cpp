#include "stdafx.h"
#include "CGSaveEquipSuit.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "GCCharEquipment.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCSaveEquipSuitResult.h"
#include "ItemRuler.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"
#include "GCDetailEquipList.h"


using namespace Packets;

uint CGSaveEquipSuitHandler::Execute(CGSaveEquipSuit* pPacket, Player* pPlayer )
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

	
	UINT nSuitNum = pPacket->getSuitNum();
	if(nSuitNum>MAX_EQUIP_SUIT_NUM)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSaveEquipSuitHandler: nSuitNum=%d", nSuitNum ) ;
		return PACKET_EXE_ERROR ;
	}
	ItemContainer* pItemContainer = pHuman->GetEquipContain();
	Assert(pItemContainer);
	BOOL bDataValid = TRUE;
	BYTE nResult = EQUIPSUIT_EQUIP_FAIL;
	GCSaveEquipSuitResult Msg;
	_SUIT_SETTING suitSetting = pPacket->getSuitSetting();
	for(INT i=0; i<HEQUIP_NUMBER; ++i)
	{
		//������
		if(!suitSetting.m_EquipData[i].isNull())
		{
			Item* pEquip = pItemContainer->GetItem(i);
			Assert(pEquip);

			//װ����������
			if(!pEquip->IsEmpty())
			{
				if(!(pEquip->GetGUID() == suitSetting.m_EquipData[i]))
				{
					//��鱳������û��
					UINT nPos = HumanItemLogic::GetBagItemPosByGUID(pHuman, suitSetting.m_EquipData[i]);

					//�Ҳ���
					if(nPos == INVALID_INDEX)
					{
						bDataValid = FALSE;
						break;
					}
					//�ӱ������ҵ�
					Item* pUseItem = HumanItemLogic::GetBagItem(pHuman ,nPos);

					//�ж�����
					if(pUseItem->GetItemClass() != ICLASS_EQUIP)
					{
						g_pLog->FastSaveLog( LOG_FILE_1, "CGSaveEquipSuitHandler: Equip is not ICLASS_EQUIP, EquipPoint=%d", i ) ;
						return PACKET_EXE_ERROR ;
					}
					//�ж�װ����
					if(pUseItem->GetEquipPoint() != i)
					{
						if(pUseItem->GetEquipPoint() == HEQUIP_RING1)
						{
							if((i != HEQUIP_RING1)&&(i != HEQUIP_RING2))
							{
								g_pLog->FastSaveLog( LOG_FILE_1, "CGSaveEquipSuitHandler: EquipPoint error , pUseItem->GetEquipPoint()=%d", pUseItem->GetEquipPoint() ) ;
								return PACKET_EXE_ERROR ;
							}
						}
						else if(pUseItem->GetEquipPoint() == HEQUIP_ADORN1)
						{
							if((i != HEQUIP_ADORN1)&&(i != HEQUIP_ADORN2))
							{
								g_pLog->FastSaveLog( LOG_FILE_1, "CGSaveEquipSuitHandler: EquipPoint error , pUseItem->GetEquipPoint()=%d", pUseItem->GetEquipPoint() ) ;
								return PACKET_EXE_ERROR ;
							}
						}
						else
						{
							g_pLog->FastSaveLog( LOG_FILE_1, "CGSaveEquipSuitHandler: EquipPoint error , pUseItem->GetEquipPoint()=%d", pUseItem->GetEquipPoint() ) ;
							return PACKET_EXE_ERROR ;
						}
					}
					//�жϵȼ�
					if(pUseItem->GetRequireLevel() > pHuman->GetLevel() )
					{
						nResult = EQUIPSUIT_LEVEL;
						Msg.setResult(nResult);
						Msg.setSuitNum(0);
						pGamePlayer->SendPacket(&Msg);
						g_pLog->FastSaveLog( LOG_FILE_1, "CGSaveEquipSuitHandler: Equiplevel , pUseItem->GetRequireLevel()=%d", pUseItem->GetRequireLevel() ) ;
						return PACKET_EXE_CONTINUE;
					}
					//�ж�ְҵ
					if(!pUseItem->IsWuMenPai())
					{
						if(!pUseItem->InReqJob(pHuman->GetMenPai()))
						{
							nResult = EQUIPSUIT_JOB_FAIL;
							Msg.setResult(nResult);
							Msg.setSuitNum(0);
							pGamePlayer->SendPacket(&Msg);
							g_pLog->FastSaveLog( LOG_FILE_1, "CGSaveEquipSuitHandler: ReqJob , pUseItem->GetReqJob ") ;

							return PACKET_EXE_CONTINUE;
						}
					}

					//���Ա�����������,������һ����ѯ
					continue;
				}
			}

			//װ����û�����ݣ�Ҫ��鱳��
			else
			{
				//��鱳������û��
				UINT nPos = HumanItemLogic::GetBagItemPosByGUID(pHuman, suitSetting.m_EquipData[i]);

				//�Ҳ���
				if(nPos == INVALID_INDEX)
				{
					bDataValid = FALSE;
					break;
				}
				//�ӱ������ҵ�
				Item* pUseItem = HumanItemLogic::GetBagItem(pHuman ,nPos);

				//�ж�����
				if(pUseItem->GetItemClass() != ICLASS_EQUIP)
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGSaveEquipSuitHandler: Equip is not ICLASS_EQUIP, EquipPoint=%d", i ) ;
					return PACKET_EXE_ERROR ;
				}
				//�ж�װ����
				if(pUseItem->GetEquipPoint() != i)
				{
					if(pUseItem->GetEquipPoint() == HEQUIP_RING1)
					{
						if((i != HEQUIP_RING1)&&(i != HEQUIP_RING2))
						{
							g_pLog->FastSaveLog( LOG_FILE_1, "CGSaveEquipSuitHandler: EquipPoint error , pUseItem->GetEquipPoint()=%d", pUseItem->GetEquipPoint() ) ;
							return PACKET_EXE_ERROR ;
						}
					}
					else if(pUseItem->GetEquipPoint() == HEQUIP_ADORN1)
					{
						if((i != HEQUIP_ADORN1)&&(i != HEQUIP_ADORN2))
						{
							g_pLog->FastSaveLog( LOG_FILE_1, "CGSaveEquipSuitHandler: EquipPoint error , pUseItem->GetEquipPoint()=%d", pUseItem->GetEquipPoint() ) ;
							return PACKET_EXE_ERROR ;
						}
					}
					else
					{
						g_pLog->FastSaveLog( LOG_FILE_1, "CGSaveEquipSuitHandler: EquipPoint error , pUseItem->GetEquipPoint()=%d", pUseItem->GetEquipPoint() ) ;
						return PACKET_EXE_ERROR ;
					}
				}
				//�жϵȼ�
				if(pUseItem->GetRequireLevel() > pHuman->GetLevel() )
				{
					nResult = EQUIPSUIT_LEVEL;
					Msg.setResult(nResult);
					Msg.setSuitNum(0);
					pGamePlayer->SendPacket(&Msg);
					g_pLog->FastSaveLog( LOG_FILE_1, "CGSaveEquipSuitHandler: Equiplevel , pUseItem->GetRequireLevel()=%d", pUseItem->GetRequireLevel() ) ;
					return PACKET_EXE_CONTINUE;
				}
				//�ж�ְҵ
				if(!pUseItem->IsWuMenPai())
				{
					if(!pUseItem->InReqJob(pHuman->GetMenPai()))
					{
						nResult = EQUIPSUIT_JOB_FAIL;
						Msg.setResult(nResult);
						Msg.setSuitNum(0);
						pGamePlayer->SendPacket(&Msg);
						g_pLog->FastSaveLog( LOG_FILE_1, "CGSaveEquipSuitHandler: ReqJob ") ;

						return PACKET_EXE_CONTINUE;
					}
				}
			}
		}
	}
	
	//������Ч����
	if(bDataValid == FALSE)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSaveEquipSuitHandler: EQUIP don't exsist " ) ;

		return PACKET_EXE_CONTINUE; ;
	}
	//����
	pHuman->SetEquipSuitSetting(suitSetting, nSuitNum);

	nResult = EQUIPSUIT_SUCCESS;

	Msg.setResult(nResult);
	Msg.setSuitNum(nSuitNum);
	Msg.setSuitSetting(pHuman->GetEquipSuitSetting(nSuitNum));
	pGamePlayer->SendPacket(&Msg);
	g_pLog->FastSaveLog( LOG_FILE_1, "CGSaveEquipSuitHandler: nSuitNum=%d", nSuitNum ) ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}