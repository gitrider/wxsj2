/********************************************************************************
 *	�����ˣ�	���ı�
 *	����ʱ�䣺	2008 �� 5 �� 20 ��	
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/


#include "stdafx.h"
#include "GWCancelHorseFindMateInfo.h"
#include "Log.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "OnlineUser.h"
#include "HorseFindMateManager.h"
#include "WGOperateResult.h"
#include "WGReturnHorseFindMateInfo.h"

namespace Packets
{

uint	GWCancelHorseFindMateInfoHandler::Execute(GWCancelHorseFindMateInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	Assert( pServerPlayer );

	PET_GUID_t guid = pPacket->GetGUID();

	INT OResult;
	if(HorseFindMateInfoManager::GetInstance().CancelHorseFindMateInfo(guid)){
		OResult = OR_CANCELHORSEFINDMATE_SUCC;
	}
	else{
		OResult = OR_CANCELHORSEFINDMATE_FAIL;
	}

	WGOperateResult packet;
	packet.SetPlayerID(pPacket->GetPlayerID());
	packet.setResult(OResult);
	pServerPlayer->SendPacket(&packet);

    //ȡ���ɹ���ǿ�н���һ�μ�¼��ѯ����
	if(OResult == OR_CANCELHORSEFINDMATE_SUCC){
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

		BOOL bRet = HorseFindMateInfoManager::GetInstance().QueryHorseFindMateInfo(
			pPacket->GetGUID().GetHighSection(), 0, iReturnPos, Output, bContinue);

		WGReturnHorseFindMateInfo packet2;
		HorseFindMateInfoPerPage result;
		result.m_iCount = Output.GetSize();

		Assert(result.m_iCount <= HorseFindMateInfoPerPage::MAX_COUNT_PER_PAGE);

		for(INT i=0; i<result.m_iCount; ++i){
			result.m_aList[i] = Output[i];
		}

		packet2.SetSearch(FALSE);
		packet2.SetContinue(bContinue);
		packet2.SetReturnPostion(iReturnPos);
		packet2.SetHorseFindMateResult(result);
		packet2.SetPlayerID(pPacket->GetPlayerID());
		pServerPlayer->SendPacket(&packet2);
	}

	Log::SaveLog( WORLD_LOGFILE, "GWCancelHorseFindMateInfoHandler: ok guid=%d", guid);

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}


}
