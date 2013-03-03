/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "GWQueryHorseFindMateInfo.h"
#include "Log.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "OnlineUser.h"
#include "HorseFindMateManager.h"
#include "WGReturnHorseFindMateInfo.h"

namespace Packets
{

uint	GWQueryHorseFindMateInfoHandler::Execute(GWQueryHorseFindMateInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	Assert( pServerPlayer );

	BOOL bContinue = FALSE;
	INT iReturnPos = 0;

	//////////////////////////////////////////////////////////////////////////
	// Allocates memory on the stack for Output
	//_alloca(uSize),放弃使用_alloca，该api在linux下强烈建议不要使用，另外在windows下还要进行SEH处理
	UINT uSize = (UINT)(sizeof(HorseFindMateInfo)*HorseFindMateInfoPerPage::MAX_COUNT_PER_PAGE);
	Assert(uSize < 1024);
	CHAR szAlloc[1024] = {0};

	HorseFindMateInfoTable::HorseFindMateInfoVector Output(
		HorseFindMateInfoPerPage::MAX_COUNT_PER_PAGE,
		(VOID*)(szAlloc)
		);

	if(HorseFindMateInfoManager::GetInstance().QueryHorseFindMateInfo(pPacket->GetPlayerGUID(), 
		pPacket->GetMaxPostion(), iReturnPos, Output, bContinue) ){

		WGReturnHorseFindMateInfo packet;

		HorseFindMateInfoPerPage result;
		result.m_iCount = Output.GetSize();

		Assert(result.m_iCount <= HorseFindMateInfoPerPage::MAX_COUNT_PER_PAGE);

		for(INT i=0; i<result.m_iCount; ++i){
			result.m_aList[i] = Output[i];
		}

		packet.SetSearch(FALSE);
		packet.SetReturnPostion(iReturnPos);
		packet.SetContinue(bContinue);
		packet.SetHorseFindMateResult(result);
		packet.SetPlayerID(pPacket->GetPlayerID());
		pServerPlayer->SendPacket(&packet);

	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

}
