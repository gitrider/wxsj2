/********************************************************************************
 *	�����ˣ�	���ı�
 *	����ʱ�䣺	2008 �� 5 �� 20 ��	
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/


#include "stdafx.h"
#include "GWSearchHorseFindMateInfo.h"
#include "Log.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "OnlineUser.h"
#include "HorseFindMateManager.h"
#include "WGReturnHorseFindMateInfo.h"

namespace Packets
{

uint	GWSearchHorseFindMateInfoHandler::Execute(GWSearchHorseFindMateInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	Assert( pServerPlayer );

	BOOL bContinue = FALSE;
	INT iReturnPos = 0;

	//////////////////////////////////////////////////////////////////////////
	// Allocates memory on the stack for Output
	//_alloca(uSize),����ʹ��_alloca����api��linux��ǿ�ҽ��鲻Ҫʹ�ã�������windows�»�Ҫ����SEH����
	UINT uSize = (UINT)(sizeof(HorseFindMateInfo)*HorseFindMateInfoPerPage::MAX_COUNT_PER_PAGE);
	Assert(uSize < 1024);
	CHAR szAlloc[1024] = {0};

	HorseFindMateInfoTable::HorseFindMateInfoVector Output(
		HorseFindMateInfoPerPage::MAX_COUNT_PER_PAGE,
		(VOID*)(szAlloc)
		);

	BOOL bRet = HorseFindMateInfoManager::GetInstance().SearchHorseFindMateInfo(pPacket->GetName(),
		pPacket->GetHorseLevel(), pPacket->GetHorseSex(), pPacket->GetHorseGrowRate(),
		pPacket->GetMaxPostion(), bContinue, iReturnPos, Output);
	//if(bRet){
		INT i = 0;
		WGReturnHorseFindMateInfo packet;
		HorseFindMateInfoPerPage result;
		result.m_iCount = Output.GetSize();
		for(i=0; i<result.m_iCount; ++i){
			result.m_aList[i] = Output[i];
		}

		packet.SetReturnPostion(iReturnPos);
		packet.SetContinue(bContinue);
		packet.SetReturnPostion(iReturnPos);
		packet.SetHorseFindMateResult(result);
		packet.SetPlayerID(pPacket->GetPlayerID());
		pServerPlayer->SendPacket(&packet);
	//}

	Log::SaveLog( WORLD_LOGFILE, "GWIssueHorseFindMateInfoHandler: ok name=%s,level=%d,sex=%d,growrate=%d",
		pPacket->GetName(),	pPacket->GetHorseLevel(), pPacket->GetHorseSex(), pPacket->GetHorseGrowRate());

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

}

