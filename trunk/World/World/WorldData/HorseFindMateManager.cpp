#include "stdafx.h"
#include "HorseFindMateManager.h"
#include "TimeManager.h"
#include "User.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "WGOperateResult.h"

HorseFindMateInfoTable::HorseFindMateInfoTable()
{
	m_HorseFindMateInfoVector.Reserve(MAX_HORSEFINDMATEINFO_COUNT);
}

HorseFindMateInfoTable::~HorseFindMateInfoTable()
{
}

BOOL HorseFindMateInfoTable::Add(HorseFindMateInfo& entry)
{
__ENTER_FUNCTION
	if(Find(entry.m_HorseGuid)){
		char szMsg[256] = {0};
		tsnprintf(szMsg, 255, "HorseFindMateInfo.m_HorseGuid=%llu already exist.", entry.m_HorseGuid);
		AssertEx(false, szMsg);
		return FALSE;
	}

	entry.m_uIssueTime = (INT)g_pTimeManager->CurrentDate();

	m_HorseFindMateInfoVector.Add(entry);
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL HorseFindMateInfoTable::Remove(const PET_GUID_t& HorseGuid)
{
__ENTER_FUNCTION
	INT i = 0;
	for(; i < m_HorseFindMateInfoVector.GetSize(); ++i){
		if(m_HorseFindMateInfoVector[i].m_HorseGuid == HorseGuid){
			m_HorseFindMateInfoVector.Remove(i);
			return TRUE;
		}
	}

	return FALSE;
__LEAVE_FUNCTION
	return FALSE;

}

BOOL HorseFindMateInfoTable::Find(const CHAR* szHorseName, 
								  INT HorseLevel, 
								  INT HorseSex, 
								  INT HorseGrowRate, 
								  INT iPostion,		 /// 输入的起始Pos
								  BOOL& bContinue,	 /// 是否有下一页
								  INT& iReturnPos,	 /// 返回的结束Pos
								  HorseFindMateInfoVector& Output,	/// 返回的结果集
								  GUID_t exceptGuid/* = INVALID_GUID*/	/// 被排除的玩家GUID
								  ) const
{
__ENTER_FUNCTION
	CHAR szMsg[128] = {0};
	tsnprintf_s(szMsg, 127, "iPositioin=%d", iPostion);

	AssertEx(iPostion>=0, szMsg);
	if(m_HorseFindMateInfoVector.GetSize() > 0){
		tsnprintf_s(szMsg, 127, "iPosition=%d, exp=iPostion<m_HorseFindMateInfoVector.GetSize()", iPostion);
		AssertEx(iPostion<m_HorseFindMateInfoVector.GetSize(), szMsg);
	}

	INT i = 0;
	for(i = iPostion; i < m_HorseFindMateInfoVector.GetSize(); ++i){
		if(strncmp(szHorseName, "", strlen(szHorseName)) != 0 && 
			strncmp(m_HorseFindMateInfoVector[i].m_szHorseName, szHorseName, strlen(szHorseName) != 0 ))
		{
			continue;
		}
		if(INVALID_ID != HorseLevel && 
			m_HorseFindMateInfoVector[i].m_iHorseLevel != HorseLevel)
		{
			continue;
		}
		if(INVALID_SEX != HorseSex && 
			m_HorseFindMateInfoVector[i].m_iSex != HorseSex)
		{
			continue;
		}
		if(INVALID_ID != HorseGrowRate &&
			m_HorseFindMateInfoVector[i].m_iGrowRate != HorseGrowRate)
		{
			continue;
		}

		if(INVALID_GUID != exceptGuid){
			if(m_HorseFindMateInfoVector[i].m_HorseGuid.GetHighSection() == exceptGuid){
				continue;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 终于找到符合条件的记录了
		Output.Add(m_HorseFindMateInfoVector[i]);

		if(Output.GetSize() >= HorseFindMateInfoPerPage::MAX_COUNT_PER_PAGE){
			break;
		}
	}

	if(i >= m_HorseFindMateInfoVector.GetSize() ){
		bContinue = FALSE;
	}
	else{
		bContinue = TRUE;
	}

	iReturnPos = i;

	return (Output.GetSize() > 0);
__LEAVE_FUNCTION
	return FALSE;
}

//BOOL HorseFindMateInfoTable::FindByHorseName(const CHAR* szHorseName, 
//								  const HorseFindMateInfoVector& Input, 
//								  HorseFindMateInfoVector& Output) const
//{
//	if(strncmp(szHorseName, "", strlen(szHorseName)) == 0){
//		Output = Input;
//		return TRUE;
//	}
//
//	HorseFindMateInfoVector::const_iterator it = Input.begin();
//	for(; it != Input.end(); ++it){
//		if(strncmp(it->m_szHorseName, szHorseName, strlen(it->m_szHorseName)) == 0){
//			Output.push_back(*it);
//		}
//	}
//	return (!Output.empty());
//}
//
//BOOL HorseFindMateInfoTable::FindByHorseLevel(INT HorseLevel, 
//								  const HorseFindMateInfoVector& Input, 
//								  HorseFindMateInfoVector& Output) const
//{
//	if(INVALID_ID == HorseLevel){
//		Output = Input;
//		return TRUE;
//	}
//
//	HorseFindMateInfoVector::const_iterator it = Input.begin();
//	for(; it != Input.end(); ++it){
//		if(it->m_iHorseLevel == HorseLevel){
//			Output.push_back(*it);
//		}
//	}
//	return (!Output.empty());
//}
//
//BOOL HorseFindMateInfoTable::FindByHorseSex(INT HorseSex, 
//								  const HorseFindMateInfoVector& Input, 
//								  HorseFindMateInfoVector& Output) const
//{
//	if(INVALID_SEX == HorseSex){
//		Output = Input;
//		return TRUE;
//	}
//
//	HorseFindMateInfoVector::const_iterator it = Input.begin();
//	for(; it != Input.end(); ++it){
//		if(it->m_iSex == HorseSex){
//			Output.push_back(*it);
//		}
//	}
//	return (!Output.empty());
//}
//
//BOOL HorseFindMateInfoTable::FindByHorseGrowRate(INT HorseGrowRate, 
//								  const HorseFindMateInfoVector& Input, 
//								  HorseFindMateInfoVector& Output) const
//{
//	if(INVALID_ID == HorseGrowRate){
//		Output = Input;
//		return TRUE;
//	}
//
//	HorseFindMateInfoVector::const_iterator it = Input.begin();
//	for(; it != Input.end(); ++it){
//		if(it->m_iGrowRate == HorseGrowRate){
//			Output.push_back(*it);
//		}
//	}
//	return (!Output.empty());
//}

BOOL HorseFindMateInfoTable::Find(const PET_GUID_t& HorseGuid) const
{
__ENTER_FUNCTION
	INT i = 0;
	for(; i < m_HorseFindMateInfoVector.GetSize(); ++i){
		if(m_HorseFindMateInfoVector[i].m_HorseGuid == HorseGuid)
			return TRUE;
	}
	return FALSE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL HorseFindMateInfoTable::Find(GUID_t PlayerGuid, HorseFindMateInfo& Output) const
{
__ENTER_FUNCTION
	INT i = 0;
	for(; i < m_HorseFindMateInfoVector.GetSize(); ++i){
		if(m_HorseFindMateInfoVector[i].m_HorseGuid.GetHighSection() == PlayerGuid){
			Output = m_HorseFindMateInfoVector[i];
			return TRUE;
		}
	}
	return FALSE;
__LEAVE_FUNCTION
	return FALSE;
}

//=======================================================
const INT UPDATEINTERVAL = 60000;
const INT MAX_HOLD_TIME = 3600000*48;

HorseFindMateInfoManager::HorseFindMateInfoManager()
{
	m_UpdateTimer.BeginTimer(UPDATEINTERVAL, g_pTimeManager->CurrentTime());
}

VOID HorseFindMateInfoManager::HeartBeat(UINT uTime)
{
__ENTER_FUNCTION
	if(m_UpdateTimer.CountingTimer(uTime)){
		for(INT i=0; i<m_HorseFindMateInfoTable.Size(); ++i){
			UINT uDiff = g_pTimeManager->DiffTime(m_HorseFindMateInfoTable.GetEntry(i).m_uIssueTime,
				g_pTimeManager->CurrentDate());

			if(uDiff  >= MAX_HOLD_TIME)
			{
				BOOL bRet = CancelHorseFindMateInfo(m_HorseFindMateInfoTable.GetEntry(i).m_HorseGuid);
				if(bRet){
					USER* pUser = g_pOnlineUser->FindUser(m_HorseFindMateInfoTable.GetEntry(i).m_HorseGuid.GetHighSection());
					if(pUser){
						ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer(pUser->GetServerID());
						if(pServerPlayer){
							WGOperateResult packet;
							packet.SetPlayerID(m_HorseFindMateInfoTable.GetEntry(i).m_HorseGuid.GetHighSection());
							packet.setResult(OR_CANCELHORSEFINDMATE_SUCC);

							pServerPlayer->SendPacket(&packet);
						}
					}
				}
			}
		}

	}
__LEAVE_FUNCTION
}

BOOL HorseFindMateInfoManager::IssueHorseFindMateInfo(const HorseFindMateInfo& entry)
{
	return m_HorseFindMateInfoTable.Add(const_cast<HorseFindMateInfo&>(entry));
}

BOOL HorseFindMateInfoManager::CancelHorseFindMateInfo(const PET_GUID_t& HorseGuid)
{
	return m_HorseFindMateInfoTable.Remove(HorseGuid);
}

BOOL HorseFindMateInfoManager::SearchHorseFindMateInfo(
	const CHAR* szHorseName, INT HorseLevel, INT HorseSex, INT HorseGrowRate, /// 搜索条件
	INT iPostion,		 /// 输入的起始Pos
	BOOL& bContinue,	 /// 是否有下一页
	INT& iReturnPos,	 /// 返回的结束Pos
	HorseFindMateInfoTable::HorseFindMateInfoVector& Output /// 返回的结果集
	)
{
	return m_HorseFindMateInfoTable.Find(szHorseName, HorseLevel, HorseSex, HorseGrowRate, 
		iPostion, bContinue, iReturnPos, Output);
}

BOOL HorseFindMateInfoManager::QueryHorseFindMateInfo(
	GUID_t PlayerGuid, INT iMaxPos, INT& iReturnPos,
	HorseFindMateInfoTable::HorseFindMateInfoVector& Output, BOOL& bContinue) const
{
	GUID_t exceptGuid = INVALID_GUID;
	BOOL bHaveMySelfHorse = FALSE;
	if(0 == iMaxPos){
		HorseFindMateInfo result;
		if(m_HorseFindMateInfoTable.Find(PlayerGuid, result)){
			Output.Add(result);
			bHaveMySelfHorse = TRUE;
			exceptGuid = PlayerGuid;
		}
	}

	m_HorseFindMateInfoTable.Find("", INVALID_ID, INVALID_SEX, INVALID_ID, 
		iMaxPos, bContinue, iReturnPos, Output, exceptGuid);

	/// 如果包含自己的骑乘信息，并且Output.size > HorseFindMateInfoPerPage::MAX_COUNT_PER_PAGE
	/// 那么需要将Output的最后一项Remove掉
	if(bHaveMySelfHorse && Output.GetSize() > HorseFindMateInfoPerPage::MAX_COUNT_PER_PAGE){
		Output.Remove(Output.GetSize()-1);
	}

	if(Output.GetSize() == 0)
		return FALSE;

	return TRUE;
}

BOOL HorseFindMateInfoManager::OnHorseChangeState(const PET_GUID_t& HorseGuid, INT iStateChangeType)
{
	switch(iStateChangeType){
		case ST_TRADE:
		case ST_FREE:
		case ST_MATING:
		case ST_RETURNTOCHILD:
			{
				return CancelHorseFindMateInfo(HorseGuid);
			}
			break;
		default:
			break;
	}
	return FALSE;
}


