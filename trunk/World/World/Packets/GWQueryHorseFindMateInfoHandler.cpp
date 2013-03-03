/********************************************************************************
 *	�����ˣ�	���ı�
 *	����ʱ�䣺	2008 �� 5 �� 20 ��	
 *
 *	����˵����	
 *	�޸ļ�¼��
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
	//_alloca(uSize),����ʹ��_alloca����api��linux��ǿ�ҽ��鲻Ҫʹ�ã�������windows�»�Ҫ����SEH����
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
