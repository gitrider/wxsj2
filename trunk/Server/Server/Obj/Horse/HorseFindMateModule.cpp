#include "stdafx.h"
#include "HorseFindMateModule.h"
#include "GameStruct_Query.h"
#include "Player.h"
#include "Obj_Human.h"
#include "GameTable.h"
#include "GCReturnHorseFindMateInfo.h"
#include "GWIssueHorseFindMateInfo.h"
#include "GWSearchHorseFindMateInfo.h"
#include "GWQueryHorseFindMateInfo.h"
#include "GWHorseChangeState.h"
#include "GWCancelHorseFindMateInfo.h"
#include "formula.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"

BOOL HorseFindMateModule::IssueHorseFindMateInfo(Obj_Human* pOwner, 
												 const _PET_DB* pHorseDB, 
												 const CHAR* pszLeaveWords) const
{
	__ENTER_FUNCTION

	if (CanIssueHorseFindMateInfo(pOwner, pHorseDB)){

		GWIssueHorseFindMateInfo* pPacket = (GWIssueHorseFindMateInfo*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_ISSUEHORSEFINDMATEINFO));

		HorseFindMateInfo entry;
		entry.m_HorseGuid = pHorseDB->m_GUID;
		tsnprintf_s(entry.m_szHorseName, MAX_CHARACTER_NAME, "%s", pHorseDB->m_szName);
		tsnprintf_s(entry.m_szOwnerName, MAX_CHARACTER_NAME, "%s", pOwner->GetName());
		tsnprintf_s(entry.m_szLeaveWords, MAX_CHARACTER_NAME, "%s", pszLeaveWords);
		entry.m_iGrowRate = formula::GetHorseGrowRateType((INT)(pHorseDB->m_fGrowRate));
		entry.m_iHorseDataId = pHorseDB->m_nDataID;
		entry.m_iHorseLevel = pHorseDB->m_nLevel;
		entry.m_iSex = pHorseDB->m_iSex;

		pPacket->SetPlayerID(pOwner->GetPlayerID());
		pPacket->SetHorseFindMateInfo(entry);

		//pOwner->GetPlayer()->SendPacket(&packet);


		g_pServerManager->SendPacket( pPacket, INVALID_ID );


		//���������ڷ������ѵ����GUID, �ȴ�World���ط����ɹ��ĵĽ������ȷ���Ƿ�����
		pOwner->SetFindingMateHorseGUID(pHorseDB->m_GUID);

		//�ȿ۳���Ǯ�����World����ʧ���ٻ������
		UINT uMoney = pOwner->GetMoney();
		uMoney -= g_TableInit.m_PetConfig.m_Money_FindingHorseMate;
		pOwner->SetMoney(uMoney);
		pOwner->SaveMoneyLogByHuman(MONEY_HORSE_FINDINGMATING, g_TableInit.m_PetConfig.m_Money_FindingHorseMate);
	}

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL HorseFindMateModule::CancelHorseFindMateInfo(Obj_Human* pOwner, const _PET_DB* pHorseDB) const
{
	__ENTER_FUNCTION

	Assert(pOwner);
	Assert(pHorseDB);

	GWCancelHorseFindMateInfo* pPacket = (GWCancelHorseFindMateInfo*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CANCELHORSEFINDMATEINFO));
	pPacket->SetGUID(pHorseDB->m_GUID);
	pPacket->SetPlayerID(pOwner->GetPlayerID());
	g_pServerManager->SendPacket( pPacket, INVALID_ID );

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL HorseFindMateModule::SearchHorseFindMateInfo(Obj_Human* pOwner,
												  const CHAR* szHorseName,
												  INT HorseLevel, 
												  INT HorseSex,
												  INT HorseGrowRate,
												  INT iMaxPos) const
{
	__ENTER_FUNCTION

	Assert(pOwner);

	GWSearchHorseFindMateInfo* pPacket = (GWSearchHorseFindMateInfo*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_SEARCHHORSEFINDMATEINFO));
	pPacket->SetName(szHorseName);
	pPacket->SetMaxPostion(iMaxPos);
	pPacket->SetHorseLevel(HorseLevel);
	pPacket->SetHorseSex(HorseSex);
	pPacket->SetHorseGrowRate(HorseGrowRate);
	pPacket->SetPlayerID(pOwner->GetPlayerID());

	g_pServerManager->SendPacket( pPacket, INVALID_ID );

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;

}

BOOL HorseFindMateModule::QueryHorseFindMateInfo(Obj_Human* pOwner, INT iPosition) const
{
	__ENTER_FUNCTION

	Assert(pOwner);

	GWQueryHorseFindMateInfo* pPacket = (GWQueryHorseFindMateInfo*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_QUERYHORSEFINDMATEINFO));
	pPacket->SetPlayerID(pOwner->GetPlayerID());
	pPacket->SetPlayerGUID(pOwner->GetGUID());
	pPacket->SetMaxPostion(iPosition);

	g_pServerManager->SendPacket( pPacket, INVALID_ID );

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;

}

VOID HorseFindMateModule::OnHorseChangeState(Obj_Human* pOwner, const PET_GUID_t& HorseGuid, INT StateType)
{
	__ENTER_FUNCTION

	Assert(pOwner);

	GWHorseChangeState* pPacket = (GWHorseChangeState*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_HORSECHANGESTATE));
	pPacket->SetGUID(HorseGuid);
	pPacket->SetStateChangeType(StateType);
	pPacket->SetPlayerID(pOwner->GetPlayerID());

	g_pServerManager->SendPacket( pPacket, INVALID_ID );

	__LEAVE_FUNCTION
}

BOOL HorseFindMateModule::CanIssueHorseFindMateInfo(Obj_Human* pOwner, const _PET_DB* pHorseDB) const
{	
	__ENTER_FUNCTION

	Assert(pOwner);
	Assert(pHorseDB);
	
	//1)	��˱�����ս���߱��������ܣ����Ǳ��죩
	_HORSE_ATTR* pHorseAttr = g_HorseAttrTbl.GetAttr( pHorseDB->m_nDataID );
	if(NULL == pHorseAttr){
		// ��Ӧ���ߵ����� [6/16/2008 wuwenbin]
		g_pLog->FastSaveLog(
			LOG_FILE_2,
			"HorseFindMateModule::CanIssueHorseFindMateInfo..pHorseAttr=NULL of pHorseDB->m_nDataID=%d",
			pHorseDB->m_nDataID
			);
		return FALSE;
	}
	if(pHorseAttr->m_bVariation) {
		pOwner->SendOperateResultMsg(OR_ISSUEHORSE_MUSTBE_ZHANMA);
		return FALSE;
	}
	if(FALSE == pHorseAttr->m_bCanMating){
		pOwner->SendOperateResultMsg(OR_ISSUEHORSE_MUSTBE_ZHANMA);
		return FALSE;
	}

	//2)	��˱���Ŀǰû����ż
	if(pHorseDB->m_iMatingLevel > 0) {
		pOwner->SendOperateResultMsg(OR_ISSUEHORSE_MUSTNOT_HAVEMATE);
		return FALSE;
	}

	//3)	ͬһ����ҵĶ�ƥ���������һƥ����ܹ���������״̬
	if(pOwner->IsHaveHorseFindingMate()) {
		pOwner->SendOperateResultMsg(OR_ISSUEHORSE_HAVE_ONLYONE);
		return FALSE;
	}

	//4)	��˱���15�ȼ�����
	if(pHorseDB->m_nLevel < g_TableInit.m_PetConfig.m_Level_FindingHorseMate) {
		pOwner->SendOperateResultMsg(OR_ISSUEHORSE_NOTENOUGH_LEVEL);
		return FALSE;
	}

	//5)	���ѵ���˿��ֶȱ���Ϊ100
	if(pHorseDB->m_byHappiness != g_TableInit.m_PetConfig.m_Happiness_FindingHorseMate) {
		pOwner->SendOperateResultMsg(OR_ISSUEHORSE_NOTENOUGH_HAPPINESS);
		return FALSE;
	}

	//6)	���˱���Я���㹻�Ľ�Ǯ���ߵ���
	if(pOwner->GetMoney() < (UINT)(g_TableInit.m_PetConfig.m_Money_FindingHorseMate) ) {
		pOwner->SendOperateResultMsg(OR_ISSUEHORSE_NOTENOUGH_MONEY);
		return FALSE;
	}

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

VOID HorseFindMateModule::OnGetQueryResults(Obj_Human* pOwner, const HorseFindMateInfoPerPage& Result)
{
	__ENTER_FUNCTION

	GCReturnHorseFindMateInfo packet;
	packet.SetHorseFindMateResult(Result);

	pOwner->GetPlayer()->SendPacket(&packet);

	__LEAVE_FUNCTION

}


