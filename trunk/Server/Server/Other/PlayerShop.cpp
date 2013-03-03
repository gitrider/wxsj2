/************************************************************************/
/*
��������:	2006��2��13��
����ʱ��:	11:54
�ļ�����:	PlayerShop.h
�ļ�·��:	d:\Prj\Server\Server\Other\PlayerShop.cpp
������:		sunyu

�ļ����ܣ�	����̵�
�޸ļ�¼��
*/
/************************************************************************/

#include "stdafx.h"
#include "PlayerShop.h"
#include "TimeManager.h"//g_pTimeManager
#include "SMUManager.h"
#include "ShareMemManager.h"
#include "ServerManager.h"
#include "RecordOpt.h"
#include "GUIDManager.h"
#include "Obj_Human.h"

PlayerShop::PlayerShop()
{
	__ENTER_FUNCTION
		
		m_pPlayerShopDB = new PlayerShopDB_t;
		m_pPlayerShopSM = NULL;
		m_PlayerShopRT.m_PlayerShopChanged = FALSE;

	__LEAVE_FUNCTION

}
PlayerShop::~PlayerShop()
{
	__ENTER_FUNCTION
	SAFE_DELETE(m_pPlayerShopDB);
	__LEAVE_FUNCTION
}

VOID PlayerShop::Init()
{
	__ENTER_FUNCTION

	CleanUp();

	if(RecordOpt::InitRecordContainer(&(m_pPlayerShopDB->m_ExchangeRecord), MAX_EXCHANGE_RECORD) == FALSE)
	{
		Assert(0);
		return;
	}
	if(RecordOpt::InitRecordContainer(&(m_pPlayerShopDB->m_ManagerRecord), MAX_MANAGER_RECORD) == FALSE)
	{
		Assert(0);
		return;
	}

	//���ӵ�����ʱ����
	for(UINT i = 0; i<MAX_STALL_NUM_PER_SHOP; i++)
	{
		m_PlayerShopRT.m_StallList[i].Init(&m_pPlayerShopDB->m_StallListData[i], this);		//��̨�б�
	}
	
	__LEAVE_FUNCTION
}

VOID PlayerShop::CleanUp()
{
	__ENTER_FUNCTION

	m_pPlayerShopDB->CleanUp();

	//��ʼ��̯λ���������
	sprintf(m_pPlayerShopDB->m_szShopName, "�ӻ�̯λ");

	m_PlayerShopRT.m_bSerial = 0;
	DataChanged();
	__LEAVE_FUNCTION
}

PlayerStallBox*	PlayerShop::GetPlayerStallBoxByIndex(UINT uIndex)
{
	__ENTER_FUNCTION
	if( uIndex>=0 && uIndex<MAX_STALL_NUM_PER_SHOP )
	{
		return &m_PlayerShopRT.m_StallList[uIndex];
	}
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}

BOOL PlayerShop::HeartBeat(UINT uTime, FLOAT fComFactor)
{
	__ENTER_FUNCTION
	BYTE	nDay		=	(BYTE)g_pTimeManager->GetDay();
	BYTE	nHour		=	(BYTE)g_pTimeManager->GetHour();

	FLOAT		fPayMent	= 0.0;
	static BYTE	CurrentDay	= 0;
	static BYTE	CurrentHour	= 0;

	if(CurrentDay == 0 && CurrentHour == 0)
	{//��һ�γ�ʼ��Ϊ��ǰʱ��
		CurrentDay	= nDay;
		CurrentHour = nHour;
		return TRUE;
	}

	if(CurrentDay != nDay)
	{//��ҹ0ʱ
		CurrentDay = nDay;
		fPayMent = 50000*fComFactor*m_pPlayerShopDB->m_NumStallOpened;
		if(MinusMoney((UINT)fPayMent))
		{//�ɹ�

		}
		else
		{//����
			//��һ���߼���������б��رյ��̵�
			SetShopStatus(PLAYER_SHOP::STATUS_PLAYER_SHOP_SHUT_DOWN);
		}
	}

	if(CurrentHour != nHour)
	{//ÿСʱ
		CurrentHour = nHour;
		fPayMent = 2000*fComFactor*m_pPlayerShopDB->m_NumStallOnSale;
		if(MinusMoney((UINT)fPayMent))
		{//�ɹ�

		}
		else
		{//����
			//��һ���߼���������б��رյ��̵�
			SetShopStatus(PLAYER_SHOP::STATUS_PLAYER_SHOP_SHUT_DOWN);
		}
	}

	if(m_PlayerShopRT.m_PlayerShopChanged)
	{//д��sharememory
		if(m_pPlayerShopSM)
		{
			m_pPlayerShopSM->Lock(SM_S_WRITE);
			memcpy(&(m_pPlayerShopSM->m_PlayerShop),m_pPlayerShopDB,sizeof(PlayerShopDB_t));
			m_pPlayerShopSM->UnLock(SM_S_WRITE);
		}
		m_PlayerShopRT.m_PlayerShopChanged = FALSE;
	}

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL PlayerShop::MinusMoney(UINT uMoney)
{
	if(m_pPlayerShopDB->m_ProfitMoney>uMoney)
	{
		m_pPlayerShopDB->m_ProfitMoney -= uMoney;
		DataChanged();
		return TRUE;
	}
	else if(m_pPlayerShopDB->m_ProfitMoney + m_pPlayerShopDB->m_BaseMoney>uMoney)
	{
		m_pPlayerShopDB->m_BaseMoney -= uMoney-m_pPlayerShopDB->m_ProfitMoney;
		m_pPlayerShopDB->m_ProfitMoney = 0;
		UpdateMoneyEvent();
		DataChanged();
		return TRUE;
	}
	else//������
	{
		UpdateMoneyEvent();
		DataChanged();
		return FALSE;
	}
}

VOID PlayerShop::AddMoney(UINT uMoney)
{
	UINT fBaseMoney		= GetBaseMoney();
	UINT fMaxBaseMoney	= GetMaxBaseMoney();

	if( fBaseMoney < fMaxBaseMoney )
	{//���뱾��
		if((fMaxBaseMoney-fBaseMoney)<uMoney)
		{//��������
			SetBaseMoney((UINT)fMaxBaseMoney);
			SetProfitMoney((UINT)(GetProfitMoney() + (uMoney - (fMaxBaseMoney-fBaseMoney))));
		}
		else
		{
			SetBaseMoney((UINT)(fBaseMoney+uMoney));
		}
	}
	else
	{//ֱ�ӳ���ӯ����
		SetProfitMoney((UINT)(GetProfitMoney() + uMoney) );
	}
}

VOID PlayerShop::UpdateMoneyEvent()
{
	if((FLOAT)GetBaseMoney()<((FLOAT)GetMaxBaseMoney())/2)
	{//֪ͨ�ͻ��˱��𲻹���

	}
}

UINT PlayerShop::UpdateMaxBaseMoney(FLOAT fComFactor)
{
	m_pPlayerShopDB->m_MaxBaseMoney = (UINT)(300000*fComFactor*GetNumStallOpened());
	DataChanged();
	return m_pPlayerShopDB->m_MaxBaseMoney;
}

//-------------------------------------------------------------------------------------------------
//��Ӻ������
RET_TYPE_PARTNER PlayerShop::AddPartner(GUID_t Guid)
{
	__ENTER_FUNCTION
	Partner_t* PartnerList = m_pPlayerShopDB->m_PartnerList;

	if(m_pPlayerShopDB->m_PartnerNum == MAX_PARTNER_PER_SHOP)
	{
		return RET_TYPE_LIST_FULL;
	}

	for(INT i = 0; i<MAX_PARTNER_PER_SHOP; i++)
	{
		if(PartnerList[i].m_Guid == Guid)
		{
			return RET_TYPE_ALREADY_IN_LIST;
		}
	}

	Obj_Human* pSearchHuman = (Obj_Human*)g_pGUIDManager->Get(Guid) ;

	if(pSearchHuman == NULL)
	{
		return RET_TYPE_NOT_FIND_IN_WORLD;
	}

	for(INT i = 0; i<MAX_PARTNER_PER_SHOP; i++)
	{
		if(PartnerList[i].m_Guid == INVALID_GUID)
		{
			PartnerList[i].m_Guid = Guid;
			memset(PartnerList[i].m_szName, 0, MAX_CHARACTER_NAME);
			memcpy(PartnerList[i].m_szName, pSearchHuman->GetName(), MAX_CHARACTER_NAME);
			m_pPlayerShopDB->m_PartnerNum++;
			break;
		}
	}
	return RET_TYPE_SUCCESS;
	__LEAVE_FUNCTION
	return RET_TYPE_INVALID;
}

//-------------------------------------------------------------------------------------------------
//ɾ���������
RET_TYPE_PARTNER PlayerShop::RemovePartner(GUID_t Guid)
{
	__ENTER_FUNCTION
	Partner_t* PartnerList = m_pPlayerShopDB->m_PartnerList;

	if(m_pPlayerShopDB->m_PartnerNum == 0)
	{
		return RET_TYPE_LIST_EMPTY;
	}

	INT	iPartnerIndex = -1;
	INT i;
	for( i = 0; i<MAX_PARTNER_PER_SHOP; i++)
	{
		if(PartnerList[i].m_Guid == Guid)
		{
			iPartnerIndex = i;
			break;
		}
	}

	if(iPartnerIndex == -1)
	{
		return RET_TYPE_NOT_FIND_IN_LIST;
	}

	PartnerList[iPartnerIndex].m_Guid = INVALID_GUID;
	m_pPlayerShopDB->m_PartnerNum--;
	return RET_TYPE_SUCCESS;

	__LEAVE_FUNCTION
	return RET_TYPE_INVALID;
}

//-------------------------------------------------------------------------------------------------
//�Ƿ��Ǻ������
BOOL PlayerShop::IsPartner(GUID_t Guid)
{
	Partner_t* PartnerList = m_pPlayerShopDB->m_PartnerList;
	for(INT i = 0; i<MAX_PARTNER_PER_SHOP; i++)
	{
		if(PartnerList[i].m_Guid == Guid)
		{
			return TRUE;
		}
	}
	return FALSE;
}
