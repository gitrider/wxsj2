
#include "stdafx.h"
#include "Scene.h"
#include "GamePlayer.h"
#include "Obj_Pet.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "CGManipulatePet.h"
#include "GCManipulatePetRet.h"
#include "GCOperateResult.h"
#include "GCDetailAttrib_Pet.h"
#include "GCRemovePet.h"
#include "HumanItemLogic.h"
#include "HorseMatingRule.h"
#include "HorseFindMateModule.h"
#include "GameStruct_Query.h"
#include "ItemOperator.h"

using namespace Packets;

const INT HappinessMin = 60;

VOID ManipulateDeletePet( Obj_Human* pHuman, GamePlayer* pGamePlayer )
{
	__ENTER_FUNCTION

	ORESULT oResult = pHuman->ReCallPet();
	GCManipulatePetRet msg;
	if (OR_FAILED(oResult))
	{
		msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_RECALLFALID);
	}
	else if (OR_SUCCEEDED(oResult))
	{
		msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_RECALLSUCC);
	}
	pGamePlayer->SendPacket(&msg);
	__LEAVE_FUNCTION
}

VOID ManipulateFreePet( Obj_Human* pHuman, CGManipulatePet* pPacket, GamePlayer* pGamePlayer )
{
	__ENTER_FUNCTION

	PET_LOG_PARAM	PetLogParam;
	ORESULT oResult = pHuman->FreePetToNature(&PetLogParam,pPacket->GetGUID());
	GCManipulatePetRet msg;
	if (OR_FAILED(oResult))
	{
		msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_FREEFALID);
	}
	else if (OR_SUCCEEDED(oResult))
	{
		msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_FREESUCC);
	}
	pGamePlayer->SendPacket(&msg);

	__LEAVE_FUNCTION
}


VOID StartPassiveSkill( SkillID_t iSkillID, const Obj_Human* pHuman, INT iSkillLevel, INT iIndex )
{
	const SkillTemplateData_T* pTemplate = GetSkillTemplateByID( iSkillID );
	if( pTemplate->GetPassiveFlag() <= 0 )
	{
		return;
	}
	const SkillInstanceData_T* pInstance = GetSkillInstanceByID( iSkillID );
	if( pInstance == NULL )
	{
		return;
	}
	const Descriptor_T* pDescriptor =  pInstance->GetDescriptorByIndex( 0 );
	if( pDescriptor == NULL )
	{
		return;
	}
	INT iDataIndex = pDescriptor->GetValueBySkillLevel( iSkillLevel-1 );

	Obj_Human* pTemp = const_cast<Obj_Human*>(pHuman);
	pTemp->SetHorseDataIndex( iIndex, iDataIndex );
	g_ImpactCore.SendImpactToUnit( *pTemp, iDataIndex, pHuman->GetID() );
}

VOID AddPassiveSkillToOnwer( Obj_Human* pHuman, _PET_DB* pPetDB )
{
	for( INT i=0; i<6; ++i )
	{
		pHuman->SetHorseDataIndex( i, INVALID_ID );
	}
	for( INT i=0; i<PET_MAX_SKILL_COUNT; ++i )
	{
		if( pPetDB->m_SkillList[i].m_bCanUse==FALSE || pPetDB->m_SkillList[i].m_nSkillID==INVALID_ID )
		{
			continue;
		}
		StartPassiveSkill( pPetDB->m_SkillList[i].m_nSkillID, pHuman, 1, i );
	}
}

VOID ManipulateCreateHorse( Obj_Human* pHuman, CGManipulatePet* pPacket )
{
	__ENTER_FUNCTION
	//�Ѿ���������������
	if( !pHuman->GetGUIDOfCallUpHorse().IsNull() )
	{
		return;
	}
	INT iIndex = pHuman->GetHorseIndexByGUID( pPacket->GetGUID() );
	if( iIndex == INVALID_ID )
	{
		return;
	}
	_PET_DB* pPetDB = pHuman->GetHorseDB( iIndex );

	if( pPetDB->m_byHappiness < HappinessMin )
	{
		pHuman->SendOperateResultMsg(OR_NEED_HAPPINESS_60);
		return;
	}

	_HORSE_ATTR *pPetAttr = NULL;
	//MONSTER_EXT_ATTR *pMonsterAttr = NULL;
	if ( pPetDB->m_nDataID == INVALID_ID )
	{
		return;
	}
	pPetAttr = g_HorseAttrTbl.GetAttr( pPetDB->m_nDataID );
	if( pPetAttr == NULL )
	{
		return;
	}
	pHuman->SetMountID(pPetAttr->m_iMountID);
	pHuman->SetGUIDOfCallUpHorse( pPetDB->m_GUID );
	
	//TODO:��������Լӳɵȴ���
	AddPassiveSkillToOnwer( pHuman, pPetDB );
	__LEAVE_FUNCTION
}

VOID ManipulateDeleteHorse( Obj_Human* pHuman )
{
	__ENTER_FUNCTION
	pHuman->ReCallHorse();
	__LEAVE_FUNCTION
}

VOID ManipulateFreeHorse( Obj_Human* pHuman, CGManipulatePet* pPacket, GamePlayer* pGamePlayer )
{
	__ENTER_FUNCTION

	PET_GUID_t oPetGUID = pHuman->GetGUIDOfCallUpHorse();
	if( oPetGUID == pPacket->GetGUID() )
	{
		pHuman->SendOperateResultMsg(OR_CALLUP_HORSE_CAN_NOT_FREE);
		return;
	}
	INT iIndex = pHuman->GetHorseIndexByGUID( pPacket->GetGUID() );
	_PET_DB* pPetDB = pHuman->GetHorseDB( iIndex );
	if( pPetDB == NULL )
		return;

	HorseFindMateModule::GetInstance().OnHorseChangeState(pHuman, pPetDB->m_GUID, ST_FREE);

	pPetDB->CleanUp();
	g_ItemOperator.SetItemValue(pHuman->GetPetContain(), iIndex, pPetDB); //modified by wuwenbin

	GCRemovePet msgRemovePet;
	msgRemovePet.SetGUID(pPacket->GetGUID());
	pGamePlayer->SendPacket(&msgRemovePet);


	__LEAVE_FUNCTION
}

INT UseHorseSkillSoltItem( INT iUseItemNum, 
						  _HORSE_ITEM* pHorseItem, 
						  _PET_DB* pPetDB, 
						  BOOL &bUseItem, 
						  INT iIndex, 
						  Obj_Human* pHuman )
{
	__ENTER_FUNCTION

	INT iGirdNum = 0;
	INT iSolt = 6;
	_PET_SKILL oPetSkill;
	oPetSkill.CleanUp();
	oPetSkill.m_bCanUse = TRUE;
	
	if( iUseItemNum < 1 )
	{
		bUseItem = FALSE;
		return 0;
	}
	for( INT j=0; j<pHorseItem->m_iAttrChangePoint; ++j )
	{
		if( iSolt >= PET_MAX_SKILL_COUNT )
			break;
		//�Ը��ӽ��д��
		if( !pPetDB->m_SkillList[iSolt].m_bCanUse )
		{
			g_ItemOperator.SetSkill( pHuman->GetPetContain(), iIndex, iSolt, oPetSkill );
			++iGirdNum;
		}
		++iSolt;
	}

	if( iGirdNum > 0 )
	{
		iUseItemNum = 1;
		bUseItem = TRUE;
	}
	else
	{
		iUseItemNum = 0;
		bUseItem = FALSE;
	}

	return iUseItemNum;
	__LEAVE_FUNCTION
	return 0;
}

VOID HorseReturnToChildByItem( Obj_Human* pHuman, 
							  _PET_DB* pPetDB, 
							  BOOL &bUseItem, 
							  INT &iUseItemNum, 
							  _HORSE_ITEM* pHorseItem, 
							  INT iIndex )
{
	__ENTER_FUNCTION

	if( !pHuman->CanHorseReturnToChild( pPetDB->m_GUID ) )
	{
		bUseItem = FALSE;
		return;
	}
	PET_GUID_t iGUID = pPetDB->m_GUID;
	INT iSex = pPetDB->m_iSex;
	CHAR cNickName[MAX_NICK_NAME];
	memset( cNickName, 0, sizeof(CHAR)*MAX_NICK_NAME );
	strncpy( cNickName, pPetDB->m_szNick, MAX_NICK_NAME-1 );

	if( !pHuman->HorseReturnToChild( iGUID ) )
	{
		iUseItemNum = 0;
		return;
	}
	iUseItemNum = 1;
	g_ItemOperator.SetPetGUID( pHuman->GetPetContain(), iIndex, iGUID );
	FLOAT fGrowRate = (FLOAT)pHorseItem->m_iAttrChangePoint + pPetDB->m_fGrowRate;
	g_ItemOperator.SetGrowRate( pHuman->GetPetContain(), iIndex, fGrowRate );
	g_ItemOperator.SetPetSex( pHuman->GetPetContain(), iIndex, iSex );
	g_ItemOperator.SetNick( pHuman->GetPetContain(), iIndex, cNickName );

	HorseFindMateModule::GetInstance().OnHorseChangeState(pHuman, pPetDB->m_GUID, ST_RETURNTOCHILD);

	__LEAVE_FUNCTION
}

VOID HorseSkillStudyByItem( Obj_Human* pHuman, _HORSE_ITEM* pHorseItem, BOOL &bUseItem, _PET_DB* pPetDB, INT &iUseItemNum, INT iIndex )
{
	__ENTER_FUNCTION

	bUseItem = FALSE;
	if( pHuman->getScene()->GetRand100() > pHorseItem->m_iSuccessRate )
	{
		return;
	}
	BOOL bCanUseFlag = FALSE;
	_PET_SKILL oPetSkill;
	oPetSkill.CleanUp();
	oPetSkill.m_bCanUse = TRUE;
	//�ҵ��ո���
	for( INT i=6; i<PET_MAX_SKILL_COUNT; ++i )
	{
		if( !pPetDB->m_SkillList[i].m_bCanUse )
		{
			bCanUseFlag = TRUE;
			continue;
		}
		if( pPetDB->m_SkillList[i].m_nSkillID == INVALID_ID )
		{
			iUseItemNum = 1;
			bUseItem = TRUE;
			oPetSkill.m_nSkillID = pHorseItem->m_iStudySkillID;
			g_ItemOperator.SetSkill( pHuman->GetPetContain(), iIndex, i, oPetSkill );
			break;
		}
		if( i==PET_MAX_SKILL_COUNT-1 && bCanUseFlag )
		{
			//�Բ���,������û���㹻����ͨ��,���ȶ���ʹ�ü��ܲ۵��߰�
			pHuman->SendOperateResultMsg(OR_HORSE_NO_ENOUGH_SKILL_SOLT);
		}
		else if( i==PET_MAX_SKILL_COUNT-1 && !bCanUseFlag )
		{
			pHuman->SendOperateResultMsg(OR_HORSE_HAVE_TOO_MUCH_SKILL);
		}
	}
	__LEAVE_FUNCTION
}

VOID HorseSkillApperByItem( Obj_Human* pHuman, _HORSE_ITEM* pHorseItem, BOOL &bUseItem, _PET_DB* pPetDB, INT &iUseItemNum, INT iIndex )
{
	__ENTER_FUNCTION

	if( pHuman->getScene()->GetRand100() > pHorseItem->m_iSuccessRate )
	{
		bUseItem = FALSE;
		return;
	}
	_PET_SKILL oPetSkill;
	oPetSkill.CleanUp();
	oPetSkill.m_bCanUse = TRUE;
	//�ҵ��ո���
	for( INT i=0; i<6; ++i )
	{
		if( pPetDB->m_SkillList[i].m_nSkillID == INVALID_ID )
		{
			iUseItemNum = 1;
			oPetSkill.m_nSkillID = pHorseItem->m_iStudySkillID;
			g_ItemOperator.SetSkill( pHuman->GetPetContain(), iIndex, i, oPetSkill );
			break;
		}
	}
	__LEAVE_FUNCTION
}

VOID HorseForgetSkillByItem( _HORSE_ITEM* pHorseItem, _PET_DB* pPetDB, BOOL &bUseItem, INT &iUseItemNum, Obj_Human* pHuman, INT iIndex )
{
	__ENTER_FUNCTION
	_PET_SKILL oPetSkill;
	oPetSkill.CleanUp();
	oPetSkill.m_bCanUse = TRUE;
	Assert(pHorseItem->m_iAttrChangePoint<=PET_MAX_SKILL_COUNT && "�������Ǹ��ӺŷǷ�" );
	INT iTmpIndex = pHorseItem->m_iAttrChangePoint-1;
	if( iTmpIndex < 0 )
		iTmpIndex = 0;
	if( !pPetDB->m_SkillList[iTmpIndex].m_bCanUse )
	{
		bUseItem = FALSE;
		return;
	}
	oPetSkill.m_nSkillID = INVALID_ID;
	g_ItemOperator.SetSkill( pHuman->GetPetContain(), iIndex, iTmpIndex, oPetSkill );

	iUseItemNum = 1;
	__LEAVE_FUNCTION

}

BOOL HorseAddHappinessByItem( INT & iUseItemNum, _PET_DB* pPetDB, _HORSE_ITEM* pHorseItem, Obj_Human* pHuman, INT iIndex )
{
	__ENTER_FUNCTION
	if( pPetDB->m_byHappiness >= 100 )
	{
		iUseItemNum = 0;
		return FALSE;
	}
	INT iHappiness = pPetDB->m_byHappiness;
	INT iItemUsed = 0;
	for( INT i=0; i<iUseItemNum; ++i )
	{
		if( pHorseItem->m_iAttrChangePoint > 0 )
		{
			iHappiness += pHorseItem->m_iAttrChangePoint;
		}
		else
		{
			iHappiness = (INT)(iHappiness * ((pHorseItem->m_iAttrChangePercent+100)/100.0f));
		}
		++iItemUsed;
		if( iHappiness >= 100 )
		{
			iHappiness = 100;
			break;
		}
	}
	iUseItemNum = iItemUsed;

	INT iIndex = pHuman->GetPetContain()->GetIndexByGUID( &pPetDB->m_GUID );
	g_ItemOperator.SetHappiness( pHuman->GetPetContain(), iIndex, iHappiness );
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

INT CalcPerception( INT iPer, _PET_DB* pPetDB )
{
	return (INT)( ( iPer + pPetDB->m_nGenGu )*( 1.0f + (FLOAT)(pPetDB->m_iSavvy) / 1000.0f ) );
}

VOID UpdatePerception( _PET_DB* pPetDB, INT iIndex, Obj_Human* pHuman )
{
	__ENTER_FUNCTION
	INT iMindPerception			= CalcPerception( pPetDB->m_iMindPerception, pPetDB );
	INT iSmartnessPerception	= CalcPerception( pPetDB->m_iSmartnessPerception, pPetDB );
	INT iStrengthPerception		= CalcPerception( pPetDB->m_iStrengthPerception, pPetDB );
	INT iConstitutionPerception	= CalcPerception( pPetDB->m_iConstitutionPerception, pPetDB );

	g_ItemOperator.SetMindPer( pHuman->GetPetContain(), iIndex, iMindPerception );
	g_ItemOperator.SetSmartnessPer( pHuman->GetPetContain(), iIndex, iSmartnessPerception );
	g_ItemOperator.SetStrPer( pHuman->GetPetContain(), iIndex, iStrengthPerception );
	g_ItemOperator.SetConstitutionPer( pHuman->GetPetContain(), iIndex, iConstitutionPerception );

	__LEAVE_FUNCTION
}

VOID HorseAddGenGuByItem( INT & iUseItemNum, _PET_DB* pPetDB, _HORSE_ITEM* pHorseItem, INT iIndex, Obj_Human* pHuman )
{
	__ENTER_FUNCTION
	INT iGenGu = pPetDB->m_nGenGu;
	for( INT i=0; i<iUseItemNum; ++i )
	{
		if( pHorseItem->m_iAttrChangePoint > 0 )
		{
			iGenGu += pHorseItem->m_iAttrChangePoint;
		}
		else
		{
			iGenGu = (INT)(pPetDB->m_nGenGu * ((pHorseItem->m_iAttrChangePercent+100)/100.0f));
		}
	}
	g_ItemOperator.SetGenGu( pHuman->GetPetContain(), iIndex, iGenGu );
	UpdatePerception( pPetDB, iIndex, pHuman );
	__LEAVE_FUNCTION
}

VOID HorseAddSavvyByItem( INT & iUseItemNum, _PET_DB* pPetDB, _HORSE_ITEM* pHorseItem, INT iIndex, Obj_Human* pHuman )
{
	__ENTER_FUNCTION
	INT iSavvy = pPetDB->m_iSavvy;
	for( INT i=0; i<iUseItemNum; ++i )
	{
		if( pHorseItem->m_iAttrChangePoint > 0 )
		{
			iSavvy += pHorseItem->m_iAttrChangePoint;
		}
		else
		{
			iSavvy = (INT)(pPetDB->m_iSavvy * ((pHorseItem->m_iAttrChangePercent+100)/100.0f));
		}
	}
	g_ItemOperator.SetSavvy( pHuman->GetPetContain(), iIndex, iSavvy );
	UpdatePerception( pPetDB, iIndex, pHuman );
	__LEAVE_FUNCTION
}

BOOL UseHorseItem( _HORSE_ITEM* pHorseItem, INT &iUseItemNum, _PET_DB* pPetDB, Obj_Human* pHuman, INT iIndex )
{
	__ENTER_FUNCTION

	BOOL bUseItem = TRUE;
	switch(pHorseItem->m_iItemType)
	{
	case HORSE_ITEM_SAVVY:			//���Լӳ�
		HorseAddSavvyByItem(iUseItemNum, pPetDB, pHorseItem, iIndex, pHuman);
		break;
	case HORSE_ITEM_BASIC:			//���Ǽӳ�
		HorseAddGenGuByItem(iUseItemNum, pPetDB, pHorseItem, iIndex, pHuman);

		break;
	case HORSE_ITEM_HAPPINESS:		//���ֶ�����
		bUseItem = HorseAddHappinessByItem(iUseItemNum, pPetDB, pHorseItem, pHuman, iIndex);
		if( !bUseItem )
		{
			pHuman->SendOperateResultMsg(OR_HORSE_HAPPINESS_ENOUGH);
		}
		break;
	case HORSE_ITEM_SKILL_GIRD:		//���ܸ���
		//�ҵ����Դ�׵ļ��ܸ�
		iUseItemNum = UseHorseSkillSoltItem(iUseItemNum, pHorseItem, pPetDB, bUseItem, iIndex, pHuman );
		break;
	case HORSE_ITEM_SKILL_FORGET:	//��������
		HorseForgetSkillByItem(pHorseItem, pPetDB, bUseItem, iUseItemNum, pHuman, iIndex);

		break;
	case HORSE_ITEM_SKILL_APPER:	//�����������ǰ6��
		//����ɹ�����
		HorseSkillApperByItem(pHuman, pHorseItem, bUseItem, pPetDB, iUseItemNum, iIndex);

		break;
	case HORSE_ITEM_SKILL_STUDY:	//����ѧϰ����Ժ�6��
		//����ɹ�����
		HorseSkillStudyByItem(pHuman, pHorseItem, bUseItem, pPetDB, iUseItemNum, iIndex);

		break;
	case HORSE_ITEM_CHILD:			//��ͯ
		HorseReturnToChildByItem(pHuman, pPetDB, bUseItem, iUseItemNum, pHorseItem, iIndex);
		break;
	default:
		bUseItem = FALSE;
		break;
	}
	return bUseItem;
	__LEAVE_FUNCTION
	return FALSE;
}

UINT ManipulateUseHorseItem( Obj_Human* pHuman, CGManipulatePet* pPacket )
{
	__ENTER_FUNCTION
	UINT		uItemID		= pPacket->GetItemID();
	PET_GUID_t	iHorseID	= pPacket->GetGUID();
	//ȡ��ָ�����������Ʒ����
	Item* pItem = HumanItemLogic::GetBagItem( pHuman, pPacket->GetBagIndex() );
	//�����ƷID��client���Ͳ�һ���򷵻�
	if( pItem->GetItemTableIndex() != uItemID )
		return PACKET_EXE_CONTINUE;

	//�����ID
	if( iHorseID.IsNull() )
	{
		return PACKET_EXE_CONTINUE ;
	}
	//�����Ʒ����
	_HORSE_ITEM* pHorseItem = g_HorseItemTbl.GetHorseItemAttr( uItemID );
	if( pHorseItem == NULL )
	{
		return PACKET_EXE_CONTINUE ;
	}
	//���ָ���������Ʒ������
	INT iItemCount  = 	pItem->GetLayedNum();	//HumanItemLogic::CalcBagItemCount( pHuman, uItemID );
	INT iUseItemNum = pPacket->GetTypeData();
	//�������С��client����ʹ�õ�������ʵ������
	if( iItemCount < iUseItemNum )
	{
		iUseItemNum = iItemCount;
	}
	INT iIndex = pHuman->GetHorseIndexByGUID( pPacket->GetGUID() );
	if( iIndex == INVALID_ID )
	{
		return PACKET_EXE_CONTINUE;
	}
	_PET_DB* pPetDB = pHuman->GetHorseDB( iIndex );

	//ʹ�ô���Ʒ
	BOOL bUseItem = UseHorseItem(pHorseItem, iUseItemNum, pPetDB, pHuman, iIndex );

	//������Ʒ
	if( bUseItem && iUseItemNum>0 )
	{
		HumanItemLogic::DecBagItemLayCount( pHuman, pPacket->GetBagIndex(), iUseItemNum );
		pHuman->RefeshHorseDetailAttrib( pPetDB->m_GUID );
	}
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

VOID ManipulateCreatePet( Obj_Human* pHuman, CGManipulatePet* pPacket, GamePlayer* pGamePlayer )
{
	//�ٻ����Ѿ�����ͨ�����ˣ��˷���������Ч
	//ORESULT oResult = pHuman->TestCallUpPet(pPacket->GetGUID() );
	//if( OR_SUCCEEDED(oResult) )
	//{
	//	AI_Human* pHumanAI = (AI_Human*)(pHuman->GetAIObj());
	//	if (pHumanAI)
	//	{
	//		ObjID_t idSkill = 302;//CALL_UP_PET;
	//		oResult = pHumanAI->PushCommand_UseSkill(idSkill, -1, -1.f, -1.f, 0.f, INVALID_GUID);
	//	}
	//}
	//if( OR_FAILED(oResult) )
	//{
	//	GCManipulatePetRet msg;
	//	msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_CALLUPFALID);
	//	pGamePlayer->SendPacket(&msg);

	//	pHuman->SendOperateResultMsg(oResult);
	//	return;
	//}

	//pHuman->ReCallPet();
	//pHuman->SetGUIDOfCallUpPet(pPacket->GetGUID());
}

VOID ManipulateAskOtherPetInfoForMatingLock( ObjID_t objID, Scene* pScene, Obj_Human* pHuman, PET_GUID_t oHorseGUID )
{
	//�����Item
	Obj* pObj = pScene->GetObjManager()->GetObj(objID);
	if (pObj && pObj->GetObjType() == Obj::OBJ_TYPE_HUMAN)
	{
		Obj_Human* pHumanTemp = (Obj_Human*)pObj;

		Item* pPetItem = pHumanTemp->GetHorseItem(oHorseGUID);
		if(!pPetItem) 
		{
			return;
		}

		GCDetailAttrib_Pet msg;
		pHumanTemp->CalculatePetDetailAttrib(msg, pPetItem);
		msg.SetExtraInfoLength(1);
		BYTE bFlag;
		bFlag = TYPE_CONTEX_MENU_OTHER_PET;
		msg.SetExtraInfoData(&bFlag);
		msg.SetGUID( oHorseGUID );

		pHuman->GetPlayer()->SendPacket(&msg);
	}
	else
	{
		pHuman->SendOperateResultMsg(OR_CANNOT_ASK_PETDETIAL);
	}
}

VOID ManipulateAskOtherPetInfo( ObjID_t objID, PET_GUID_t guidOtherHorse, Scene* pScene, Obj_Human* pHuman )
{
	Obj* pObj = pScene->GetObjManager()->GetObj(objID);
	if (pObj && pObj->GetObjType() == Obj::OBJ_TYPE_HUMAN)
	{
		Obj_Human* pHumanTemp = (Obj_Human*)pObj;
		PET_GUID_t oHorseGUID;
		if( guidOtherHorse.IsNull() )
		{
			oHorseGUID = pHumanTemp->GetGUIDOfCallUpHorse();
			if( oHorseGUID.IsNull() )
			{
				return;
			}
		}
		else
		{
			oHorseGUID = guidOtherHorse;
		}

		Item* pPetItem = pHumanTemp->GetHorseItem(oHorseGUID);
		if(!pPetItem) 
		{
			return;
		}

		GCDetailAttrib_Pet msg;
		pHumanTemp->CalculatePetDetailAttrib(msg, pPetItem);
		msg.SetExtraInfoLength(1);
		BYTE bFlag;
		bFlag = TYPE_CONTEX_MENU_OTHER_PET;
		msg.SetExtraInfoData(&bFlag);
		msg.SetGUID( oHorseGUID );

		pHuman->GetPlayer()->SendPacket(&msg);
	}
	else
	{
		pHuman->SendOperateResultMsg(OR_CANNOT_ASK_PETDETIAL);
	}
}

VOID ManipulateHorseCheckUp( _PET_DB* pPetDB, Obj_Human* pHuman )
{
	//�ȼ�С��10���ܼ���
	if( pPetDB->m_nLevel < 10 )
	{
		pHuman->SendOperateResultMsg(OR_HORSE_CHECKUP_NEED_10LEVEL);
		return;
	}
	//��ս�������Լ���
	if( pHuman->GetGUIDOfCallUpHorse() == pPetDB->m_GUID )
	{
		pHuman->SendOperateResultMsg(OR_HORSE_CALLUP_CAN_NOT_CHECKUP);
		return;
	}
	INT iIndex = pHuman->GetPetContain()->GetIndexByGUID( &pPetDB->m_GUID );

	//���Ľ�Ǯ
	INT iMoney = pHuman->GetMoney();
	if( iMoney < g_Config.m_ConfigInfo.m_nHorseCheckUpSpend )
	{
		pHuman->SendOperateResultMsg(OR_NOT_ENOUGH_MONEY);
		return;
	}
	iMoney -= g_Config.m_ConfigInfo.m_nHorseCheckUpSpend;

	pHuman->SetMoney(iMoney);
	g_ItemOperator.SetCheckUpFlag( pHuman->GetPetContain(), iIndex, TRUE );
	pHuman->RefeshHorseDetailAttrib( pPetDB->m_GUID );
}

void ManipulateHorseNickName( _PET_DB* pPetDB, CGManipulatePet* pPacket, Obj_Human* pHuman )
{
	INT iIndex = pHuman->GetPetContain()->GetIndexByGUID( &pPetDB->m_GUID );

	g_ItemOperator.SetNick( pHuman->GetPetContain(), iIndex, pPacket->GetNickName() );
	pHuman->RefeshHorseDetailAttrib( pPetDB->m_GUID );
}

Obj_Human* GetTeamate( Obj_Human* pHuman, GamePlayer* pGamePlayer )
{
	GCManipulatePetRet msg;

	TeamInfo* pTeamInf = pHuman->GetTeamInfo();
	if( pTeamInf->GetSceneMemberCount() != 1 )
	{
		msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_HORSE_OPEN_MATING_UI_FAIL);
		pGamePlayer->SendPacket(&msg);
		return NULL;
	}
	Obj_Human* pOther = NULL;
	for( INT i=0; i<2; ++i )
	{
		const TEAMMEMBER* pMember = pTeamInf->GetTeamMember( i );
		if( pMember->m_ObjID != pHuman->GetID() )
		{
			pOther = (Obj_Human*)pHuman->getScene()->GetObjManager()->GetObj( pMember->m_ObjID );
			if( pOther->GetObjType() != Obj::OBJ_TYPE_HUMAN )
			{
				pOther = NULL;
			}
			break;
		}
	}

	return pOther;
}
VOID ManipulateHorseOpenMatingUI( Obj_Human* pHuman, GamePlayer* pGamePlayer, ObjID_t idNpc )
{
	__ENTER_FUNCTION

	GCManipulatePetRet msg;

	Obj_Human* pOther = GetTeamate(pHuman, pGamePlayer);

	if( pOther == NULL )
	{
		msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_HORSE_OPEN_MATING_UI_FAIL);
		pGamePlayer->SendPacket(&msg);
		return;
	}

	HorseMatingRule oMatingRule;
	Obj_Character* pNpc = (Obj_Character*)pHuman->getScene()->GetObjManager()->GetObj( idNpc );
	if( pNpc == NULL )
	{
		return;
	}

	INT iRet = oMatingRule.IsTeamInArea( pHuman, pOther, pNpc, pHuman->GetTeamInfo() );
	if(  iRet != GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS )
	{
		msg.SetManipulateRet( iRet );
	}
	else
	{
		msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_HORSE_OPEN_MATING_UI_OK);
	}

	msg.SetObjID( idNpc );
	pGamePlayer->SendPacket(&msg);
	pOther->GetPlayer()->SendPacket( &msg );

	__LEAVE_FUNCTION
}

void ManipulateHorseMatingLock( Obj_Human* pHuman, PET_GUID_t guidMatingHorse, GamePlayer* pGamePlayer, Scene* pScene )
{
	__ENTER_FUNCTION

	GCManipulatePetRet msg;

	Obj_Human* pOther = GetTeamate(pHuman, pGamePlayer);

	if( pOther == NULL )
	{
		msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_LOCK_FAIL);
		pGamePlayer->SendPacket(&msg);
		return;
	}
	pHuman->SetMatingHorseGuid( guidMatingHorse );
	msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_LOCK_OK);
	pGamePlayer->SendPacket(&msg);
	msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_HORSE_OTHER_MATING_LOCK);
	pOther->GetPlayer()->SendPacket( &msg );
	ManipulateAskOtherPetInfoForMatingLock( pHuman->GetID(), pScene, pOther, guidMatingHorse );
	__LEAVE_FUNCTION
}

VOID ManipulateHorseMatingUnlock( Obj_Human* pHuman, GamePlayer* pGamePlayer )
{
	__ENTER_FUNCTION

	GCManipulatePetRet msg;

	Obj_Human* pOther = GetTeamate(pHuman, pGamePlayer);

	if( pOther == NULL )
	{
		return;
	}
	PET_GUID_t guidNULL;
	pHuman->SetMatingHorseGuid( guidNULL );
	msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_UNLOCK);
	pGamePlayer->SendPacket(&msg);
	msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_HORSE_OTHER_MATING_UNLOCK);
	pOther->GetPlayer()->SendPacket( &msg );
	__LEAVE_FUNCTION
}

VOID ManipulateHorseMatingConfirm( Obj_Human* pHuman, GamePlayer* pGamePlayer, CGManipulatePet* pPacket )
{
	__ENTER_FUNCTION

	GCManipulatePetRet msg;

	Obj_Human* pOther = GetTeamate(pHuman, pGamePlayer);

	if( pOther == NULL )
	{
		return;
	}

	PET_GUID_t guidMyMatingHorse = pHuman->GetMatingHorseGuid();
	PET_GUID_t guidOrMatingHorse = pOther->GetMatingHorseGuid();

	//������κ�һ��û��������������ȷ��
	if( guidMyMatingHorse.IsNull() || guidOrMatingHorse.IsNull() )
	{
		msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_CONFIRM_FAIL);
		pGamePlayer->SendPacket(&msg);
		pOther->GetPlayer()->SendPacket( &msg );
	}
	//����ȷ�����
	pHuman->SetMatingHorseConfirm( TRUE );
	//����Է���û��ȷ��
	if( !pOther->IsMatingHorseConfirm() )
	{
		return;
	}

	HorseMatingRule oMatingRule;
	Obj_Character* pNpc = (Obj_Character*)pHuman->getScene()->GetObjManager()->GetObj( pPacket->GetCharObjID() );
	if( pNpc == NULL )
	{
		return;
	}

	//�Է���ȷ������˫������ȷ����Ϣ
	msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_CONFIRM);
	pGamePlayer->SendPacket(&msg);
	pOther->GetPlayer()->SendPacket( &msg );
	BOOL bRet;
	//��ʼ����
	if( pHuman->GetTeamInfo()->IsLeader() )
	{
		bRet = oMatingRule.AskHorseMating( pHuman, pOther, pNpc, pHuman->GetMatingHorseGuid(), pOther->GetMatingHorseGuid() );
	}
	else
	{
		bRet = oMatingRule.AskHorseMating( pOther, pHuman, pNpc, pOther->GetMatingHorseGuid(), pHuman->GetMatingHorseGuid() );
	}
	
	if( !bRet )
	{
		pHuman->SetMatingHorseConfirm( FALSE );
		pOther->SetMatingHorseConfirm( FALSE );
	}
	pHuman->RefeshHorseDetailAttrib( pHuman->GetMatingHorseGuid() );
	pOther->RefeshHorseDetailAttrib( pOther->GetMatingHorseGuid() );

	// �������ɹ���֪ͨ�������ģ�� [6/16/2008 wuwenbin]
	if(bRet){
		_PET_DB* pPetDB = pHuman->GetHorseDB(pHuman->GetHorseIndexByGUID(pHuman->GetMatingHorseGuid()) );
		_PET_DB* pOtherPetDB = pOther->GetHorseDB(pOther->GetHorseIndexByGUID(pOther->GetMatingHorseGuid()) );
		HorseFindMateModule::GetInstance().OnHorseChangeState(pHuman, pPetDB->m_GUID, ST_MATING);
		HorseFindMateModule::GetInstance().OnHorseChangeState(pOther, pOtherPetDB->m_GUID, ST_MATING);
	}

	__LEAVE_FUNCTION
}

VOID ManipulateHorseMatingCancel( Obj_Human* pHuman, GamePlayer* pGamePlayer )
{
	__ENTER_FUNCTION 

	GCManipulatePetRet msg;

	Obj_Human* pOther = GetTeamate(pHuman, pGamePlayer);

	if( pOther == NULL )
	{
		return;
	}
	msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_CANCEL);
	pGamePlayer->SendPacket(&msg);
	pOther->GetPlayer()->SendPacket( &msg );
	__LEAVE_FUNCTION
}

VOID ManipulateHorseMatingRetrieve( Obj_Human* pHuman, GamePlayer* pGamePlayer, ObjID_t iNpcID )
{
	__ENTER_FUNCTION
	HorseMatingRule oMatingRule;
	GCManipulatePetRet msg;

	Obj_Human* pOther = GetTeamate(pHuman, pGamePlayer);

	if( pOther == NULL )
	{
		msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_NO_TEAM);
		pGamePlayer->SendPacket(&msg);
		return;
	}
	Obj_Character* pNpc = (Obj_Character*)pHuman->getScene()->GetObjManager()->GetObj( iNpcID );
	if( pNpc == NULL )
	{
		return;
	}
	oMatingRule.RetrieveHorse( pHuman, pOther, pNpc );

	__LEAVE_FUNCTION
}

uint CGManipulatePetHandler::Execute( CGManipulatePet* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;
	
	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	if(pHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{//��̯��
		return PACKET_EXE_CONTINUE;
	}

	_PET_DB* pPetDB = NULL;
	INT nType = pPacket->GetManipulateType();
	if( nType!=MANIPULATE_ASKOTHERPETINFO &&
		nType!=MANIPULATE_HORSE_OPEN_MATING_UI &&
		!(nType>=MANIPULATE_HORSE_MATING_UNLOCK))
	{
		//GUIDΪ��
		if( pPacket->GetGUID().IsNull() )
			return PACKET_EXE_CONTINUE;
		//��ƷΪ��
		Item *pPetItem = pHuman->GetHorseItem(pPacket->GetGUID());
		if(pPetItem == NULL)
			return PACKET_EXE_CONTINUE;
		//��Ʒ����
		if(pPetItem->IsLock())
		{
			return PACKET_EXE_CONTINUE;
		}

		INT iIndex = pHuman->GetHorseIndexByGUID( pPacket->GetGUID() );
		if( iIndex == INVALID_ID )
		{
			return PACKET_EXE_CONTINUE;
		}
		pPetDB = pHuman->GetHorseDB( iIndex );
		if( pPetDB == NULL )
		{
			return PACKET_EXE_CONTINUE;
		}
		//���ڽ���״̬
		if( pPetDB->m_bMating )
		{
			return PACKET_EXE_CONTINUE;
		}
	}

	switch(nType)
	{
	case MANIPULATE_CREATEPET:
		{// �ٻ�����
			ManipulateCreatePet(pHuman, pPacket, pGamePlayer);
		}
		break;
	case MANIPULATE_DELETEPET:
		{// �ջس���
			ManipulateDeletePet(pHuman, pGamePlayer);
		}
		break;
	case MANIPULATE_FREEPET:
		{// ��������
			ManipulateFreePet(pHuman, pPacket, pGamePlayer);
		}
		break;
	case MANIPULATE_ASKOTHERPETINFO:
		{// �쿴������ҵĳ�����Ϣ
			ManipulateAskOtherPetInfo(pPacket->GetObjID(), pPacket->GetGUID(), pScene, pHuman);
		}
		break;
	case MANIPULATE_CREATEHORSE:	
		{// �ٻ�����
			ManipulateCreateHorse(pHuman, pPacket);
		}
		break;
	case MANIPULATE_DELETEHORSE:	
		{// �ջ�����
			ManipulateDeleteHorse(pHuman);
		}
		break;
	case MANIPULATE_FREEHORSE:
		{//����
			ManipulateFreeHorse(pHuman, pPacket, pGamePlayer);
		}
		break;

	case MANIPULATE_HORSE_ITEM:
		{//ʹ����Ʒ����
			return ManipulateUseHorseItem( pHuman, pPacket );
		}
		break;
	case MANIPULATE_HORSE_CHECKUP:
		{//����
			ManipulateHorseCheckUp(pPetDB, pHuman);
		}
		break;
	case MANIPULATE_HORSE_NICKNAME:
		{//�޸��ǳ�
			ManipulateHorseNickName(pPetDB, pPacket, pHuman);
		}
		break;
	case MANIPULATE_HORSE_OPEN_MATING_UI:
		{//�򿪽������
			ManipulateHorseOpenMatingUI(pHuman, pGamePlayer, pPacket->GetCharObjID());
		}
		break;
	case MANIPULATE_HORSE_MATING_LOCK:
		{//������������
			ManipulateHorseMatingLock( pHuman, pPacket->GetGUID(), pGamePlayer, pScene );
		}
		break;
	case MANIPULATE_HORSE_MATING_UNLOCK:
		{//�����������
			ManipulateHorseMatingUnlock( pHuman, pGamePlayer );
		}
		break;
	case MANIPULATE_HORSE_MATING_CONFIRM:	
		{//���ｻ��ȷ��
			ManipulateHorseMatingConfirm(pHuman, pGamePlayer, pPacket);
		}
		break;
	case MANIPULATE_HORSE_MATING_CANCEL:
		{//���ｻ��ȡ��
			ManipulateHorseMatingCancel(pHuman, pGamePlayer);
		}
		break;
	case MANIPULATE_HORSE_MATING_RETRIEVE:
		{
			ManipulateHorseMatingRetrieve(pHuman, pGamePlayer, pPacket->GetCharObjID());
		}
		break;
	default:
		break;
	}
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}