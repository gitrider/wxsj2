// AbilityCompound.cpp

#include "stdafx.h"
#include "AbilityCompound.h"
#include "AbilityManager.h"
#include "GameTable.h"
#include "Scene.h"
#include "GCAbilitySucc.h"
#include "GamePlayer.h"
#include "GCItemInfo.h"
#include "HumanItemLogic.h"
#include "ScriptDef.h"
#include "LogDefine.h"
#include "GCNotifyEquip.h"
#include "ItemOperator.h"

ORESULT AbilityCompound::CanUseAbility(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	Assert( pHuman );

	ORESULT res;
	res = Ability::CanUseAbility(pHuman);
	if( res != OR_OK )
	{
		return res;
	}

	AbilityOpera* pAbilityOpera;
	const _ABILITY_PRESCRIPTION_TBL* pPrescriptionList = g_pAbilityManager->GetPrescriptionList();

	pAbilityOpera = pHuman->GetAbilityOpera();
	Assert( pAbilityOpera );

	if( (pAbilityOpera->m_PresID > 0) && (pAbilityOpera->m_PresID <= pPrescriptionList->m_Count) )
	{ // �����䷽�ϳ����

		const _PRESCRIPTION_DATA* pPrescrData;
		pPrescrData = pPrescriptionList->Get_Prescription(pAbilityOpera->m_PresID);

		// �ж������䷽�Ƿ����������
		if( pPrescrData->m_AbilityID != m_AbilityID )
		{
			return OR_WARNING;
		}

		// �ж�����Ƿ��Ѿ�ѧ����䷽
		if( pHuman->__IsPrescrHaveLearned( pAbilityOpera->m_PresID ) == FALSE )
		{
			return OR_WARNING;
		}

		if( pPrescrData->m_nOperaTime > 0 )
		{ // ʹ��ÿ���䷽�Զ���Ĳ���ʱ��
			pAbilityOpera->m_nMaxTime = pPrescrData->m_nOperaTime;
		}

		if( pPrescrData->m_ScriptID != INVALID_ID )
		{ // ��ͨ�䷽��ִ��

			LuaInterface* pLuaInterface;
			pLuaInterface = pHuman->getScene()->GetLuaInterface();

			// ���ýű������жϲ�ȡ�÷���ֵ
			ORESULT res;
			res = (ORESULT)pLuaInterface->ExeScript_DD( pPrescrData->m_ScriptID, DEF_ABILITY_CHECK,
				(INT)pHuman->getScene()->SceneID(), (INT)pHuman->GetID() );

			// ���ط���ֵ
			return res;
		}
		else
		{
			return IsFitPrescr(pHuman, pPrescrData);
		}

	}
	// else if(...) // ���䷽�ϳ����

	return OR_WARNING;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AbilityCompound::IsFitPrescr(Obj_Human* pHuman, const _PRESCRIPTION_DATA* pPrescrData)
{
__ENTER_FUNCTION

	Assert( pHuman );
	Assert( pPrescrData );

	if( pPrescrData->m_ToolID!=INVALID_ID
	 && HumanItemLogic::CalcBagItemCount(pHuman,pPrescrData->m_ToolID)<1
	 )
	{
		return OR_NO_TOOL;
	}

	if( pPrescrData->m_ExpRequired > pHuman->__GetAbilityExp( pPrescrData->m_AbilityID ) )
	{
		return OR_EXP_LACK;
	}

	for( INT i=0; i<MAX_PRESCRIPTION_STUFF; ++i )
	{
		if( pPrescrData->m_Stuff_ID[i] == INVALID_ID )
		{
			continue;
		}
		
		if( HumanItemLogic::CalcBagItemCount(pHuman,pPrescrData->m_Stuff_ID[i])
		  < pPrescrData->m_Stuff_Num[i]
		  )
		{
			return OR_STUFF_LACK;
		}
	}

	if( pPrescrData->m_Attr_Vigor > 0 )
	{
		if( pHuman->GetVigor() - pPrescrData->m_Attr_Vigor < 0 )
		{
			return OR_NOT_ENOUGH_VIGOR;
		}
	}

	if( pPrescrData->m_Attr_Energy > 0 )
	{
		if( pHuman->GetEnergy() - pPrescrData->m_Attr_Energy < 0 )
		{
			return OR_NOT_ENOUGH_ENERGY;
		}
	}

	if( pPrescrData->m_Attr_Reserved > 0 )
	{
	}

	if( pPrescrData->m_Attr_Reserved2 > 0 )
	{
	}

	if( (pPrescrData->m_ColddownID >= 0))
	{
		if(FALSE==pHuman->IsCooldowned( pPrescrData->m_ColddownID ))
		{ // ��ȴʱ���Ƿ�û�н���
			return OR_COOL_DOWNING;
		}
	}

	// �ж��Ƿ����ϻ���ʣ��ռ�
	INT ns;
	if( HumanItemLogic::CalcBagItemSpace(pHuman, pPrescrData->m_CompoundID, pPrescrData->m_CompoundNum, ns)
	 == FALSE
	 )
	{
		return OR_BAG_OUT_OF_SPACE;
	}

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AbilityCompound::OnProcOver(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	Assert( pHuman );

	AbilityOpera* pAbilityOpera;
	const _ABILITY_PRESCRIPTION_TBL* pPrescriptionList = g_pAbilityManager->GetPrescriptionList();

	pAbilityOpera = pHuman->GetAbilityOpera();
	Assert( pAbilityOpera );

	if( (pAbilityOpera->m_PresID > 0) && (pAbilityOpera->m_PresID <= pPrescriptionList->m_Count) )
	{ // �����䷽�ϳ����
		const _PRESCRIPTION_DATA* pPrescrData;
		pPrescrData = pPrescriptionList->Get_Prescription(pAbilityOpera->m_PresID);

		ORESULT res;

		LuaInterface* pLuaInterface;
		pLuaInterface = pHuman->getScene()->GetLuaInterface();

		if( pPrescrData->m_ScriptID != INVALID_ID )
		{ // ��ͨ�䷽��ִ��
			// ���ýű������жϲ�ȡ�÷���ֵ
			res = (ORESULT)pLuaInterface->ExeScript_DD( pPrescrData->m_ScriptID, DEF_ABILITY_CHECK,
				(INT)pHuman->getScene()->SceneID(), (INT)pHuman->GetID() );
			if( res != OR_OK )
			{ // �ٴμ�飬����ϳɹ����г�����Ʒ�����ͱ����ռ�仯�ȵ�
				return res;
			}

			// ������Ʒ
			pLuaInterface->ExeScript_DD( pPrescrData->m_ScriptID, DEF_ABILITY_CONSUME,
				(INT)pHuman->getScene()->SceneID(), (INT)pHuman->GetID() );
		}
		else
		{
			res = IsFitPrescr(pHuman, pPrescrData);
			if( res != OR_OK )
			{ // �ٴμ�飬����ϳɹ����г�����Ʒ�����ͱ����ռ�仯�ȵ�
				return res;
			}

			// ������Ʒ
			AbilityConsume(pHuman, pPrescrData);
		}

		// ���������
		pLuaInterface->ExeScript_DDDD( ABILITY_LOGIC_SCRIPT, DEF_ABILITY_GAIN_EXPERIENCE,
			(INT)pHuman->getScene()->SceneID(), (INT)pHuman->GetID(),
			pPrescrData->m_AbilityID, pPrescrData->m_AbilityLevel );

		res = (ORESULT)pLuaInterface->ExeScript_DDDD( ABILITY_LOGIC_SCRIPT, DEF_ABILITY_SUCCESSFUL_CHECK,
			(INT)pHuman->getScene()->SceneID(), (INT)pHuman->GetID(),
			pPrescrData->m_AbilityID, pPrescrData->m_AbilityLevel );
		if( res == OR_FAILURE )
		{ // �ϳɽ���ʱ�ĳɹ���ͳһ�жϣ�����жϽ���Ǻϳ�ʧ�ܣ���...
			return OnProcFailure( pHuman, pPrescrData );
		}

		if( pPrescrData->m_ScriptID != INVALID_ID )
		{ // ��ͨ�䷽��ִ��
			// ���ýű������жϲ�ȡ�÷���ֵ
			res = (ORESULT)pLuaInterface->ExeScript_DD( pPrescrData->m_ScriptID, DEF_ABILITY_PRODUCE,
				(INT)pHuman->getScene()->SceneID(), (INT)pHuman->GetID() );

			// ���ط���ֵ
			return res;
		}
		else
		{
			return OnProcSuccess(pHuman, pPrescrData);
		}
	}
	

	return OR_ERROR;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AbilityCompound::OnProcSuccess(Obj_Human* pHuman, const _PRESCRIPTION_DATA* pPrescrData)
{
__ENTER_FUNCTION

	Assert( pHuman );
	Assert( pPrescrData );

	GamePlayer* pGamePlayer = (GamePlayer*)pHuman->GetPlayer();

	if( pPrescrData->m_CompoundNum > 0 )
	{
		// Quality ������һ���ű�������
		LuaInterface* pLuaInterface;
		pLuaInterface = pHuman->getScene()->GetLuaInterface();

		INT nQuality;

		nQuality = pLuaInterface->ExeScript_DDDDD( ABILITY_LOGIC_SCRIPT, DEF_ABILITY_CALC_QUALITY,
			pHuman->__GetScene()->SceneID(),
			pPrescrData->m_ReplicaID,
			pHuman->__GetAbilityLevel(pPrescrData->m_AbilityID),
            AbilityLevelLimit(),
			pPrescrData->m_CompoundID);

		CreateItemToHuman(pHuman, pPrescrData->m_CompoundID, pPrescrData->m_CompoundNum, nQuality);
	}

	// ���ͳɹ���Ϣ�Լ���Ʒ��״̬�仯
	GCAbilitySucc SuccMsg;
	SuccMsg.SetAbilityID( pPrescrData->m_AbilityID );
	SuccMsg.SetPrescriptionID( pPrescrData->m_PrescriptionID );
	SuccMsg.SetItemSerial( pPrescrData->m_CompoundID);
	pGamePlayer->SendPacket( &SuccMsg );

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AbilityCompound::OnProcFailure(Obj_Human* pHuman, const _PRESCRIPTION_DATA* pPrescrData)
{
__ENTER_FUNCTION

	Assert( pHuman );
	Assert( pPrescrData );

	GamePlayer* pGamePlayer = (GamePlayer*)pHuman->GetPlayer();

	if( pPrescrData->m_ReplicaID != INVALID_ID )
	{
		// ���Ʒ�� Quality ���� 1����ʹ����װҲ��ʱ���������ɴ����Ե���װ
		CreateItemToHuman(pHuman, pPrescrData->m_ReplicaID, 1, 1);
	}

	// ���ͳɹ���Ϣ�Լ���Ʒ��״̬�仯
	GCAbilitySucc SuccMsg;
	SuccMsg.SetAbilityID( pPrescrData->m_AbilityID );
	SuccMsg.SetPrescriptionID( pPrescrData->m_PrescriptionID );
	SuccMsg.SetItemSerial( pPrescrData->m_ReplicaID);
	pGamePlayer->SendPacket( &SuccMsg );

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AbilityCompound::AbilityConsume(Obj_Human* pHuman, const _PRESCRIPTION_DATA* pPrescrData)
{
__ENTER_FUNCTION

	Assert( pHuman );
	Assert( pPrescrData );

	// �۳���Ҫ��Ʒ
	for( INT i=0; i<MAX_PRESCRIPTION_STUFF; ++i )
	{
		if( pPrescrData->m_Stuff_ID[i] == INVALID_ID )
		{
			continue;
		}
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType	= ITEM_ABILITY_COMPOUND_LOST;
		HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, pPrescrData->m_Stuff_ID[i], pPrescrData->m_Stuff_Num[i]);
	}

	if( pPrescrData->m_Attr_Vigor > 0 )
	{
		pHuman->SetVigor( pHuman->GetVigor() - pPrescrData->m_Attr_Vigor );
	}

	if( pPrescrData->m_Attr_Energy > 0 )
	{
		pHuman->SetEnergy( pHuman->GetEnergy() - pPrescrData->m_Attr_Energy );
	}

	if( pPrescrData->m_Attr_Reserved > 0 )
	{
	}

	if( pPrescrData->m_Attr_Reserved2 > 0 )
	{
	}

	if( (pPrescrData->m_ColddownID >= 0)
	 && (pPrescrData->m_ColddownTime > 0)
	 )
	{ // ������ȴʱ��
		pHuman->SetCooldown( pPrescrData->m_ColddownID, pPrescrData->m_ColddownTime );
	}

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

BOOL AbilityCompound::CreateItemToHuman(Obj_Human* pHuman, UINT uItemSN, INT nCount, INT nQuality)
{
__ENTER_FUNCTION

	Assert( pHuman );
	Assert( uItemSN != INVALID_ID );
	Assert( nCount > 0 );

	if( nCount < 1 )
	{
		return FALSE;
	}

	if( uItemSN != INVALID_ID )
	{
		ITEM_LOG_PARAM	ItemLogParam;
		BOOL bRet;

		ItemLogParam.OpType		= ITEM_CREATE_ABILITY_COMPOUD;
		ItemLogParam.CharGUID	= pHuman->GetGUID();
		ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
		ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
		ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;

		if( nCount > 1 )
		{
			bRet = HumanItemLogic::CreateMultiItemToBag(&ItemLogParam, pHuman, uItemSN, nCount);
		}
		else
		{
			UINT uBagPos;

			bRet = HumanItemLogic::CreateItemToBag(&ItemLogParam, pHuman, uItemSN, uBagPos, nQuality);

			Item* pItem = HumanItemLogic::GetBagItem(pHuman, uBagPos);
			// �����װ�������������������modify by gh 2010/07/22
			if (pItem->IsEquip() && uBagPos>=0)
			{
				ItemContainer* pContainer;
				pContainer = HumanItemLogic::GetBagContainer( pHuman, uBagPos);
				g_ItemOperator.SetItemCreator( pContainer, uBagPos, pHuman->GetName() );
			}

			GCNotifyEquip Msg;
			Msg.SetBagIndex( uBagPos );
			pItem->SaveValueTo(Msg.GetItem());
			pHuman->GetPlayer()->SendPacket(&Msg);
		}

		if( bRet != FALSE )
		{
			SaveItemLog(&ItemLogParam);
		}
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}
