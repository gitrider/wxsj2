#include "stdafx.h"
#include "CGEquipAdsorbMagic.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "HumanItemLogic.h"

#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"

#include "GCCharEquipment.h"
#include "GCDetailEquipList.h"
#include "GCItemInfo.h"
#include "GCEquipAdsorbMagicResult.h"

using namespace Packets;

UINT CGEquipAdsorbMagicHandler::Execute( CGEquipAdsorbMagic* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GamePlayer*pGamePlayer = (GamePlayer*)pPlayer;
	Assert(pGamePlayer);

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert(pHuman);

	Scene* pScene = pHuman->getScene();
	if (pScene == NULL)
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	// ����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID() == pScene->m_ThreadID );

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

	GCEquipAdsorbMagicResult Msg;
	EQUIPADSORMAGIC_RESULT	 Result = EQUIPADSORMAGIC_EQUIP_FAIL;

	UINT EquipPoint			= INVALID_ITEM_POS;
	UINT BagIndex			= INVALID_ITEM_POS;

	Item*	pEquipItem		= NULL;
	Item*   pBeadItem		= NULL;
	switch(pPacket->GetEquipPosType())
	{
	case ENUM_EQUIP_POINT:
		{
			EquipPoint = pPacket->GetEquipPos();
			if(EquipPoint>HEQUIP_ADORN2)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler: EquipPoint error, EquipPoint=%d", EquipPoint) ;
				return PACKET_EXE_ERROR;
			}

			ItemContainer* pEquipContainer = pHuman->GetEquipContain();
			if(NULL==pEquipContainer)
			{
				AssertEx(FALSE,"[CGEquipAdsorbMagic]: NULL EquipContainer pointer found!");
			}
			pEquipItem = pEquipContainer->GetItem((UINT)EquipPoint);
			Assert( pEquipItem );

			if ( pEquipItem->IsEmpty() )
			{
				//���ⷵ�ظ�ħʧ����Ϣ
				Result = EQUIPADSORMAGIC_EQUIP_FAIL;
				Msg.setResult( Result );
				pGamePlayer->SendPacket( &Msg );

				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler pEquipItem is Null ") ;
				return PACKET_EXE_CONTINUE;
			}
			
			// �������ϵ�һ����װ����������ǣ��򱨴�
			Assert( pEquipItem->GetItemClass() == ICLASS_EQUIP );

		}
		break;
	case ENUM_BAG:
		{
			BagIndex = pPacket->GetEquipPos();
			ItemContainer* pItemContainer = HumanItemLogic::GetBagContainer(pHuman,BagIndex);
			if( pItemContainer == NULL )
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler: BagIndex error") ;
				return PACKET_EXE_ERROR;
			}

			if (!pItemContainer->IsCanUse())
			{
				//���ⷵ�ظ�ħʧ����Ϣ
				Result = EQUIPADSORMAGIC_EQUIP_FAIL;
				Msg.setResult( Result );
				pGamePlayer->SendPacket( &Msg );

				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler: ItemContainer is invalid") ;
				return PACKET_EXE_ERROR;
			}
			
			pEquipItem	= pItemContainer->GetItem(pItemContainer->BagIndex2ConIndex(BagIndex));
			Assert(pEquipItem);

			if ( pEquipItem->IsEmpty() )
			{
				//���ⷵ�ظ�ħʧ����Ϣ
				Result = EQUIPADSORMAGIC_EQUIP_FAIL;
				Msg.setResult( Result );
				pGamePlayer->SendPacket( &Msg );

				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler pEquipItem is Null ") ;
				return PACKET_EXE_CONTINUE;
			}
			
			// �ڱ��������Ʒ�п��ܲ���װ�������Բ�ֱ�ӱ���������ͻ��˷���ʧ����Ϣ��
			if (pEquipItem->GetItemClass() != ICLASS_EQUIP)
			{
				//���ⷵ�ظ�ħʧ����Ϣ
				Result = EQUIPADSORMAGIC_EQUIP_FAIL;
				Msg.setResult( Result );
				pGamePlayer->SendPacket( &Msg );

				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler GetItemClass is not ICLASS_EQUIP ") ;
				return PACKET_EXE_CONTINUE;
			}
			if (pEquipItem->GetItemType() > EQUIP_ADORN)
			{
				//���ⷵ�ظ�ħʧ����Ϣ
				Result = EQUIPADSORMAGIC_EQUIP_FAIL;
				Msg.setResult( Result );
				pGamePlayer->SendPacket( &Msg );

				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler EquipAdsorbmagic GUID is defferent ") ;
				return PACKET_EXE_CONTINUE;
			}
		}
		break;
	default:
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler: EquipFromType error, EquipFromType=%d", pPacket->GetEquipPosType()) ;
			return PACKET_EXE_ERROR;
		}
		break;
	}

	// ���Ҫװ���Ļ��飬����һ�����ڱ�����
	UINT nBeadPos = pPacket->GetBeadPos();

	ItemContainer* pItemContainer = HumanItemLogic::GetBagContainer(pHuman,nBeadPos);
	if( pItemContainer == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler: BagIndex error") ;
			return PACKET_EXE_ERROR;
	}
	if (!pItemContainer->IsCanUse())
	{
		//���ⷵ�ظ�ħʧ����Ϣ
		Result = EQUIPADSORMAGIC_EQUIP_FAIL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler: ItemContainer is invalid") ;
			return PACKET_EXE_ERROR;
	}

	pBeadItem = pItemContainer->GetItem(pItemContainer->BagIndex2ConIndex(nBeadPos));
	Assert(pBeadItem);

	if ( pBeadItem->IsEmpty() )
	{
		//���ⷵ�ظ�ħʧ����Ϣ
		Result = EQUIPADSORMAGIC_EQUIP_FAIL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler pEquipItem is Null ") ;
		return PACKET_EXE_CONTINUE;
	}

	// �����õĶ����ǻ��飬����ʧ����Ϣ
	if (pBeadItem->GetItemClass() != ICLASS_SOUL_BEAD)
	{
		Result = EQUIPADSORMAGIC_EQUIP_FAIL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler GetItemClass is not ICLASS_SOUL_BEAD ") ;
		return PACKET_EXE_CONTINUE;
	}
	
	// �Ƿ�����ȷ��װ������
	switch(pEquipItem->GetItemType())
	{
	// ��ȷ����
	case EQUIP_CAP:
	case EQUIP_SHOULDER:
	case EQUIP_ARMOR:
	case EQUIP_HAND:
	case EQUIP_BOOT:
		{

		}
	break;
	default:
		{
			// ����װ�����Ͳ���ȷ��Ϣ
			Result = EQUIPADSORMAGIC_EQUIPBYTE_ERROR;
			Msg.setResult( Result );
			pGamePlayer->SendPacket( &Msg );

			return PACKET_EXE_CONTINUE;
		}
	}

	// �ж�װ���Ƿ��Ѿ���ħ
	if ( pEquipItem->IsAdsorbMagic() ) 
	{
		Result = EQUIPADSORMAGIC_EQUIP_HAVE_MAGIC;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler Equip Is Have Maigc ") ;
		return PACKET_EXE_CONTINUE;
	}
	
	// �ж����ӵĵȼ��Ƿ������ҵĵȼ�
	if ( pHuman->GetLevel() < pBeadItem->GetSoulBeadNeedLevel() )
	{
		Result = EQUIPADSORMAGIC_BEAMLEVEL_BIG_HUMANLEVEL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler BeadLevel Big Then HumanLevel ") ;
		return PACKET_EXE_CONTINUE;
	}

	// �ж����ӵĵȼ��Ƿ����������ʹ�õȼ�
	if ( pEquipItem->GetRequireLevel() < pBeadItem->GetSoulBeadNeedLevel() )
	{
		Result = EQUIPADSORMAGIC_BEAMLEVEL_BIG_EQUIPLEVEL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler BeadLevel Big Then EquipLevel ") ;
		return PACKET_EXE_CONTINUE;
	}

///////////////////////////////////////////////////////////////
	//���Ľ�Ǯ���㷽���������1.8�η� ����10 �ټ�2
	// ��ħ���ĵĽ�Ǯ����,��ʱ����ƽ��������,�Ժ���޸�
	INT UseMoney = 0;
	INT nEquipLevel = (pEquipItem->GetRequireLevel())/10;
	nEquipLevel *= nEquipLevel;
	UseMoney = nEquipLevel/10+2;
///////////////////////////////////////////////////////////////	


	// �ж���ҵ���Ϸ���Ƿ���
	if ( pHuman->GetMoney() < UseMoney )
	{
		Result = EQUIPADSORMAGIC_MONEY_NOT_ENOUGH;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler Not Enough Money ") ;
		return PACKET_EXE_CONTINUE;
	}

	// ��ħ����
	if (pHuman->EquipAdsorbMagic(pEquipItem, pBeadItem) == EQUIPADSORMAGIC_SUCCESS)
	{
		// ɾ������
		ITEM_LOG_PARAM		ItemLogParam;
		ItemLogParam.OpType	= ITEM_ADSORBMAGIC_CONSUME;
		ItemLogParam.CharGUID = pHuman->GetGUID();

		BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, nBeadPos);
		if(bRet)
		{
			SaveItemLog(&ItemLogParam);
		}

		// ɾ����ħ���ĵĽ�Ǯ
		pHuman->SetMoney( pHuman->GetMoney()-UseMoney );

		MONEY_LOG_PARAM MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_ADSORBMAIGC_REDUCE;
		MoneyLogParam.Count		=	UseMoney;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		SaveMoneyLog( &MoneyLogParam );

		Result = EQUIPADSORMAGIC_SUCCESS;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );


		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler::EquipAdsorbMagic success") ;
	}
	else
	{
		// ����ʧ����Ϣ
		Result = EQUIPADSORMAGIC_EQUIP_FAIL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );
		
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler: EquipAdsorbMagic Fail") ;
		return PACKET_EXE_CONTINUE;
	}

	GCItemInfo EquipMsg;
	EquipMsg.setIsNull(FALSE);
	EquipMsg.setID( pPacket->GetEquipPos() );
	pEquipItem->SaveValueTo(EquipMsg.getItem());
	pHuman->GetPlayer()->SendPacket(&EquipMsg);

return PACKET_EXE_CONTINUE;
	__LEAVE_FUNCTION
	return PACKET_EXE_ERROR;
}