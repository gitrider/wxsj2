/********************************************************************************
 *	�����ˣ�	���ı�
 *	����ʱ�䣺	2008 �� 5 �� 20 ��	
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/


#include "stdafx.h"
#include "WGOperateResult.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "PlayerPool.h"

namespace Packets
{

uint	WGOperateResultHandler::Execute(WGOperateResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID() ;
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID) ;
	if( pGamePlayer==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_CONTINUE ;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	// �Է�������������������� [6/16/2008 wuwenbin]
	switch(pPacket->getResult()){
		case OR_CANCELHORSEFINDMATE_SUCC:
			{
				// ���õ�ǰ���ѵ����GUID [6/16/2008 wuwenbin]
				PET_GUID_t petGuid;
				pHuman->SetFindingMateHorseGUID(petGuid);
			}
			break;
		case OR_ISSUEHORSE_FAIL:
			{
				// ���õ�ǰ���ѵ����GUID [6/16/2008 wuwenbin]
				PET_GUID_t petGuid;
				pHuman->SetFindingMateHorseGUID(petGuid);
				// ������ѷ���ʧ���򷵻���ҵĽ�Ǯ [6/16/2008 wuwenbin]
				UINT uMoney = pHuman->GetMoney() ;
				uMoney += g_TableInit.m_PetConfig.m_Money_FindingHorseMate;
				pHuman->SetMoney(uMoney);
				pHuman->SaveMoneyLogByHuman(MONEY_HORSE_FINDINGMATING, g_TableInit.m_PetConfig.m_Money_FindingHorseMate);
			}

			break;
		default:
			break;
	}

	pHuman->SendOperateResultMsg(pPacket->getResult());

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
}
