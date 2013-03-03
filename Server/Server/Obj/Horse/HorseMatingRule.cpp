/********************************************************************
	created:	2008/04/02
	created:	2:4:2008   19:50
	filename: 	MapServer\Server\Obj\Horse\HorseMatingRule.cpp
	file path:	MapServer\Server\Obj\Horse
	file base:	HorseMatingRule
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "HorseMatingRule.h"
#include "Obj_Human.h"
#include "Team.h"
#include "GCManipulatePetRet.h"
#include "Scene.h"
#include "TimeManager.h"
#include "HorseManager.h"
#include "ItemOperator.h"

#define TestResult(a) if((a)!=GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS){return a;}

BOOL HorseMatingRule::AskHorseMating( Obj_Character* pMe, Obj_Character* pChar, const Obj_Character* pNPC, PET_GUID_t guidMyHorse, PET_GUID_t guidOtherHorse )
{
	__ENTER_FUNCTION

	Obj_Human* pHumanMe		= (Obj_Human*)pMe;
	Obj_Human* pHumanChar	= (Obj_Human*)pChar;

	if( !IsBothHuman(pHumanMe, pHumanChar) )
	{
		return FALSE;
	}

	TeamInfo* pTeamInf = GetTeamInfo(pHumanMe);
	if( pTeamInf == NULL )
	{
		return FALSE;
	}

	INT iRet = HorseMatingImp( pHumanMe, pHumanChar, pNPC, pTeamInf, guidMyHorse, guidOtherHorse );
	if( iRet == GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS )
	{
		StartMating( pHumanMe, pHumanChar, guidMyHorse, guidOtherHorse );
		SpendMoney( pHumanMe );
	}
	else
	{
		SendMatingResult( pHumanMe, pHumanChar, iRet );
		return FALSE;
	}
	SendMatingResult( pHumanMe, pHumanChar, iRet );
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

INT HorseMatingRule::HorseMatingImp( Obj_Human* pMe, Obj_Human* pChar, const Obj_Character* pNPC, const TeamInfo* pTeamInf, PET_GUID_t guidMyHorse, PET_GUID_t guidOtherHorse )
{
	__ENTER_FUNCTION

	//是否组队人数为2
	TestResult( IsSuitabilityMemCount( pTeamInf ) );
	//判断是否组队
	TestResult( IsInTeam( pTeamInf, pMe, pChar ) );
	//是否是队长
	TestResult( IsTeamLeader( pTeamInf ) );
	//判断2人与NPC的距离必须小于5
	TestResult( IsTeamInArea( pMe, pChar, pNPC, pTeamInf ) );
	//队长现金大于N=2金币
	TestResult( IsHeaderHaveEnoughMoney( pMe ) );
	//判断两匹参与交配的马是否为可交配
	TestResult( IsBothHorseCanMating( pMe, pChar, guidMyHorse, guidOtherHorse ) );
	//是否异性
	TestResult( IsTwoHorseOppositeSex( pMe, pChar, guidMyHorse, guidOtherHorse ) );
	//是否有其它马在交配
	TestResult( IsAnyHorseMating(pMe, pChar) );
	//是否正在出战
	TestResult( IsCallup(pMe, guidMyHorse, pChar, guidOtherHorse) );

	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	__LEAVE_FUNCTION
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

INT HorseMatingRule::IsInTeam( const TeamInfo* pTeamInf, Obj_Human* pMe, Obj_Human* pChar )
{	
	__ENTER_FUNCTION

	if( !pTeamInf->HasTeam() )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_NO_TEAM;
	}

	const TEAMMEMBER* pTeamMember1 = pTeamInf->GetTeamMember( 0 );
	const TEAMMEMBER* pTeamMember2 = pTeamInf->GetTeamMember( 1 );

	if( pTeamMember1->m_SceneID != pTeamMember2->m_SceneID )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_HORSE_SCENE;
	}

	if( (pMe->GetID()!=pTeamMember1->m_ObjID && pMe->GetID()!=pTeamMember2->m_ObjID) ||
		(pChar->GetID()!=pTeamMember1->m_ObjID && pChar->GetID()!=pTeamMember2->m_ObjID) )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_NO_TEAM;
	}
	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;

	__LEAVE_FUNCTION

	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

INT HorseMatingRule::IsSuitabilityMemCount( const TeamInfo* pTeamInf )
{
	__ENTER_FUNCTION

	// 得到同场景队友的数量（不包含自己）
	if( pTeamInf->GetSceneMemberCount() != 1 )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_TEAM_MEM_COUNT;
	}
	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	__LEAVE_FUNCTION

	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

INT HorseMatingRule::IsTeamLeader( const TeamInfo* pTeamInf )
{
	__ENTER_FUNCTION

	if( !pTeamInf->IsLeader() )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_TEAM_LEADER;
	}
	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;

	__LEAVE_FUNCTION

	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

INT HorseMatingRule::IsTeamInArea( Obj_Human* pHuman, Obj_Human* pOther, const Obj_Character* pNPC, const TeamInfo* pTeamInf )
{
	__ENTER_FUNCTION

	FLOAT fDisArea = 5.0f * 5.0f;
	if( pOther == NULL )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_AREA;
	}
	FLOAT fDistance = MyLengthSq( pOther->getWorldPos(), pNPC->getWorldPos() );
	if( fDistance > fDisArea )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_AREA;
	}
	fDistance += MyLengthSq( pHuman->getWorldPos(), pNPC->getWorldPos() );
	if( fDistance > fDisArea )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_AREA;
	}
	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	__LEAVE_FUNCTION

	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

INT HorseMatingRule::IsBothHorseCanMating( Obj_Human* pHuman, Obj_Human* pOther, PET_GUID_t guidMyHorse, PET_GUID_t guidOtherHorse )
{
	__ENTER_FUNCTION

	Item* pMyHorse = const_cast<Obj_Human*>(pHuman)->GetHorseItem( guidMyHorse );
	Item* pOrHorse = const_cast<Obj_Human*>(pOther)->GetHorseItem( guidOtherHorse );

	if( pMyHorse==NULL || pOrHorse==NULL )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
	}
	if( pMyHorse->IsLock() || pOrHorse->IsLock() )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_HORSE_LOCK;
	}

	//是否有足够的等级
	TestResult( IsEnoughLevel(pMyHorse, pOrHorse) );

	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	__LEAVE_FUNCTION
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

INT HorseMatingRule::IsTwoHorseOppositeSex( Obj_Human* pHuman, Obj_Human* pOther, PET_GUID_t guidMyHorse, PET_GUID_t guidOtherHorse )
{
	__ENTER_FUNCTION

	INT iIndex = const_cast<Obj_Human*>(pHuman)->GetHorseIndexByGUID( guidMyHorse );
	_PET_DB* pMyPetDB = pHuman->GetHorseDB( iIndex );

	iIndex = const_cast<Obj_Human*>(pOther)->GetHorseIndexByGUID( guidOtherHorse );
	_PET_DB* pOrPetDB = pOther->GetHorseDB( iIndex );
	if( pMyPetDB==NULL || pOther == NULL )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
	}

	if( pMyPetDB->m_iSex == pOrPetDB->m_iSex )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_HORSE_SEX_SAME;
	}
	
	//是否是变异
	TestResult( IsVariation(pMyPetDB, pOrPetDB) );
	TestResult( IsCanMating(pMyPetDB, pOrPetDB) );
	//繁殖代数
	TestResult( IsGeneration(pMyPetDB, pOrPetDB) );
	//和上次交配时相比是否有20级的等级提升
	TestResult( IsEnoughMatingLevelup(pMyPetDB, pOrPetDB) );

	return IsBothHorseGeneration( pMyPetDB, pOrPetDB );
	__LEAVE_FUNCTION
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

INT HorseMatingRule::IsHeaderHaveEnoughMoney( Obj_Human* pHuman )
{
	__ENTER_FUNCTION

	INT iMoney = pHuman->GetMoney();
	if( iMoney < g_Config.m_ConfigInfo.m_nHorseMattingSpend )
	{
		pHuman->SendOperateResultMsg(OR_NOT_ENOUGH_MONEY);
		return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
	}
	
	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;

	__LEAVE_FUNCTION
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

INT HorseMatingRule::SpendMoney( Obj_Human* pHuman )
{
	__ENTER_FUNCTION

	INT iMoney = pHuman->GetMoney();

	iMoney -= g_Config.m_ConfigInfo.m_nHorseMattingSpend;
	pHuman->SetMoney( iMoney );
	
	pHuman->SaveMoneyLogByHuman( MONEY_HORSE_MATTING, g_Config.m_ConfigInfo.m_nHorseMattingSpend );
	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;

	__LEAVE_FUNCTION
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

INT HorseMatingRule::IsBothHorseGeneration( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB )
{
	__ENTER_FUNCTION

	if( !(pMyPetDB->m_byGeneration==1 && pOrPetDB->m_byGeneration==1) )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_HORSE_GENERATION;
	}
	return IsBothHorseHappinesEnough( pMyPetDB, pOrPetDB );

	__LEAVE_FUNCTION

	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

INT HorseMatingRule::IsBothHorseHappinesEnough( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB )
{
	__ENTER_FUNCTION

	if( !(pMyPetDB->m_byHappiness>=100 && pOrPetDB->m_byHappiness>=100) )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_HORSE_HAPPINESS;
	}
	return IsLevelSuitability( pMyPetDB, pOrPetDB );

	__LEAVE_FUNCTION

	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;

}

INT HorseMatingRule::IsLevelSuitability( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB )
{
	__ENTER_FUNCTION

	if( abs(pMyPetDB->m_nLevel - pOrPetDB->m_nLevel) > 5 )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_HORSE_LEVEL;
	}
	return IsSpoused( pMyPetDB, pOrPetDB );

	__LEAVE_FUNCTION

	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

INT HorseMatingRule::IsSpoused( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB )
{
	__ENTER_FUNCTION

	if( pMyPetDB->m_SpouseGUID==pOrPetDB->m_GUID &&
		pOrPetDB->m_SpouseGUID==pMyPetDB->m_GUID )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	}
	else if( pMyPetDB->m_SpouseGUID.IsNull() && pOrPetDB->m_SpouseGUID.IsNull() )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	}
	return GCManipulatePetRet::MANIPULATEPET_RET_HORSE_SPOUSED;

	__LEAVE_FUNCTION
	
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

Obj_Character* HorseMatingRule::GetTeammate( const TeamInfo* pTeamInf, Obj_Character* pHuman )
{
	__ENTER_FUNCTION

	INT iTeamCount = pTeamInf->GetSceneMemberCount();
	for( INT i=0; i<iTeamCount; ++i )
	{
		const TEAMMEMBER* pTeamMember = pTeamInf->GetTeamMember( i );
		if( pTeamMember->m_ObjID != pHuman->GetID() )
		{
			return (Obj_Character*)pHuman->getScene()->GetObjManager()->GetObj(pTeamMember->m_ObjID);
		}
	}	
	return NULL;

	__LEAVE_FUNCTION

	return NULL;
}

TeamInfo* HorseMatingRule::GetTeamInfo( Obj_Human* pMe )
{
	__ENTER_FUNCTION

	return pMe->GetTeamInfo();

	__LEAVE_FUNCTION
	
	return NULL;
}

BOOL HorseMatingRule::IsHuman( Obj_Character* pChar )
{
	__ENTER_FUNCTION

	return pChar->GetObjType() == Obj::OBJ_TYPE_HUMAN;

	__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL HorseMatingRule::IsBothHuman( Obj_Human* pMe, Obj_Character* pChar )
{
	__ENTER_FUNCTION

	Assert( pMe!=NULL && pChar!=NULL && "HorseMatingRule 马主人非法" );
	if( !IsHuman(pMe) || !IsHuman(pChar) )
	{
		return FALSE;
	}
	return TRUE;

	__LEAVE_FUNCTION
	return FALSE;
}

VOID HorseMatingRule::StartMating( Obj_Human* pMe, Obj_Human* pCh, PET_GUID_t guidMyHorse, PET_GUID_t guidOtherHorse )
{
	__ENTER_FUNCTION

	INT iIndex = pMe->GetHorseIndexByGUID( guidMyHorse );
	_PET_DB* pMyPetDB = pMe->GetHorseDB( iIndex );

	iIndex = pCh->GetHorseIndexByGUID( guidOtherHorse );
	_PET_DB* pOrPetDB = pCh->GetHorseDB( iIndex );

	if( pMyPetDB==NULL || pOrPetDB == NULL )
	{
		return;
	}

	INT iIndexMyHorse = pMe->GetHorseIndexByGUID( pMyPetDB->m_GUID );
	INT iIndexChHorse = pCh->GetHorseIndexByGUID( pOrPetDB->m_GUID );
	g_ItemOperator.SetMatingFlag( pMe->GetPetContain(), iIndexMyHorse, TRUE );
	g_ItemOperator.SetMatingFlag( pCh->GetPetContain(), iIndexChHorse, TRUE );
	
	UINT uTime = (UINT)g_pTimeManager->GetANSITime();

	g_ItemOperator.SetMatingStartTime( pMe->GetPetContain(), iIndexMyHorse, uTime );
	g_ItemOperator.SetMatingStartTime( pCh->GetPetContain(), iIndexChHorse, uTime );

	g_ItemOperator.SetMatingLevel( pMe->GetPetContain(), iIndexMyHorse, pMyPetDB->m_nLevel );
	g_ItemOperator.SetMatingLevel( pCh->GetPetContain(), iIndexChHorse, pOrPetDB->m_nLevel );

	g_ItemOperator.SetMatingFinishFlag( pMe->GetPetContain(), iIndexMyHorse, FALSE );
	g_ItemOperator.SetMatingFinishFlag( pCh->GetPetContain(), iIndexChHorse, FALSE );

	g_ItemOperator.SetSpouseGUID( pMe->GetPetContain(), iIndexMyHorse, pOrPetDB->m_GUID );
	g_ItemOperator.SetSpouseGUID( pCh->GetPetContain(), iIndexChHorse, pMyPetDB->m_GUID );

	//const_cast<_HUMAN_DB_LOAD*>(pMe->GetDB()->GetHumanDB())->m_bHorseMating = TRUE;
	//const_cast<_HUMAN_DB_LOAD*>(pCh->GetDB()->GetHumanDB())->m_bHorseMating = TRUE;
	pMe->GetDB()->SetHorseMattingFlag( TRUE );
	pCh->GetDB()->SetHorseMattingFlag( TRUE );

	SendHorseMatingSuccessMSG( pMe );
	SendHorseMatingSuccessMSG( pCh );
	__LEAVE_FUNCTION

}

VOID HorseMatingRule::SendHorseMatingSuccessMSG( Obj_Human* pHuman )
{
	//通知邮件
	CHAR  szName[MAX_CHARACTER_NAME] = {0};
	memcpy(szName, pHuman->GetName(), MAX_CHARACTER_NAME);
	CHAR szMsgContent[MAX_MAIL_CONTEX] = {0};
	sprintf(szMsgContent, "%s 恭喜！你们的骑乘已经在我这里开始繁殖了，繁殖时间大约为 %d 小时，繁殖成功后我们将会第一时间通知你们", 
		pHuman->GetName(), HorseMatingFinishTimeInHour);
	pHuman->getScene()->SendNormalMail(pHuman, szName, szMsgContent);
}

VOID HorseMatingRule::SendMatingResult( Obj_Human* pHuman, Obj_Human* pOther, INT iResult )
{
	__ENTER_FUNCTION

	GCManipulatePetRet msg;
	msg.SetManipulateRet( iResult );
	pHuman->GetPlayer()->SendPacket(&msg);
	pOther->GetPlayer()->SendPacket(&msg);

	__LEAVE_FUNCTION

}

HorseMatingRule::HorseMatingRule()
{

}

HorseMatingRule::~HorseMatingRule()
{

}


INT HorseMatingRule::IsAnyHorseMating( Obj_Human* pHuman, Obj_Human* pOther )
{
	__ENTER_FUNCTION

	if ( pHuman->GetDB()->GetHumanDB()->m_bHorseMating ||
		 pOther->GetDB()->GetHumanDB()->m_bHorseMating )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING;
	}

	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	__LEAVE_FUNCTION
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

INT HorseMatingRule::IsCallup( Obj_Human* pHuman, PET_GUID_t guidMyHorse, Obj_Human* pOther, PET_GUID_t guidOtherHorse )
{
	__ENTER_FUNCTION

	if( pHuman->GetGUIDOfCallUpHorse()==guidMyHorse ||
		pOther->GetGUIDOfCallUpHorse()==guidOtherHorse )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_HORSE_CALLUP;
	}
	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	__LEAVE_FUNCTION
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

INT HorseMatingRule::IsEnoughLevel( Item* pMyHorse, Item* pOrHorse )
{
	__ENTER_FUNCTION

	if( pMyHorse->GetLevel()<15 || pOrHorse->GetLevel()<15 )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_HORSE_LEVEL15;
	}
	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	__LEAVE_FUNCTION
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;

}

INT HorseMatingRule::IsEnoughMatingLevelup( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB )
{
	__ENTER_FUNCTION

	if( pMyPetDB->m_iMatingLevel != 0 )
	{
		if( pMyPetDB->m_nLevel-pMyPetDB->m_iMatingLevel < 20 )
		{
			return GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATINGLEVEL;
		}
	}

	if( pOrPetDB->m_iMatingLevel != 0 )
	{
		if( pOrPetDB->m_nLevel-pOrPetDB->m_iMatingLevel < 20 )
		{
			return GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATINGLEVEL;
		}
	}
	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	__LEAVE_FUNCTION
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

INT HorseMatingRule::IsGeneration( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB )
{
	__ENTER_FUNCTION

	if(	 g_HorseAttrTbl.m_aHorseTbl[pMyPetDB->m_nDataID].m_byGeneration>1 ||
		g_HorseAttrTbl.m_aHorseTbl[pOrPetDB->m_nDataID].m_byGeneration>1 )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_HORSE_GENERATION;
	}

	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	__LEAVE_FUNCTION
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

INT HorseMatingRule::IsVariation( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB )
{
	__ENTER_FUNCTION

	if( g_HorseAttrTbl.m_aHorseTbl[pMyPetDB->m_nDataID].m_bVariation ||
		g_HorseAttrTbl.m_aHorseTbl[pOrPetDB->m_nDataID].m_bVariation )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_HORSE_VARIATION;
	}
	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	__LEAVE_FUNCTION
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

VOID HorseMatingRule::RetrieveHorse( Obj_Human* pMe, Obj_Human* pCh, Obj_Character* pNPC )
{
	__ENTER_FUNCTION

	TeamInfo* pTeamInf = GetTeamInfo(pMe);
	if( pTeamInf == NULL )
	{
		return;
	}
	INT iRet = IsTeamLeader( pTeamInf ) ;

	if( iRet != GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS )
	{
		SendMatingResult( pMe, pCh, iRet );
		return;
	}

	iRet = IsCanRetrieveHorse( pMe, pCh, pNPC );
	if( iRet != GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS )
	{
		SendMatingResult( pMe, pCh, iRet );
		return;
	}
	iRet = MakingChild( pMe, pCh );


	SendMatingResult( pMe, pCh, GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_RETRIEVE );

	//const_cast<_HUMAN_DB_LOAD*>(pMe->GetDB()->GetHumanDB())->m_bHorseMating = FALSE;
	//const_cast<_HUMAN_DB_LOAD*>(pCh->GetDB()->GetHumanDB())->m_bHorseMating = FALSE;
	pMe->GetDB()->SetHorseMattingFlag( FALSE );
	pCh->GetDB()->SetHorseMattingFlag( FALSE );

	SendHorseRetrieveSuccessMSG(pMe);
	SendHorseRetrieveSuccessMSG(pCh);
	__LEAVE_FUNCTION

}

INT HorseMatingRule::MakingChild( Obj_Human* pMe, Obj_Human* pCh )
{
	__ENTER_FUNCTION

	_PET_DB* pMyPetDB			= GetMatingHorseDB( pMe );
	_PET_DB* pOrPetDB			= GetMatingHorseDB( pCh );

	INT iIndex = pMe->GetHorseIndexByGUID( pMyPetDB->m_GUID );
	g_ItemOperator.SetMatingFlag( pMe->GetPetContain(), iIndex, FALSE );

	iIndex = pCh->GetHorseIndexByGUID( pOrPetDB->m_GUID );
	g_ItemOperator.SetMatingFlag( pCh->GetPetContain(), iIndex, FALSE );

	pMe->RefeshHorseDetailAttrib( pMyPetDB->m_GUID );
	pCh->RefeshHorseDetailAttrib( pOrPetDB->m_GUID );
	TestResult( InitChild(pMyPetDB, pMe) );
	TestResult( InitChild(pOrPetDB, pCh) );

	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	__LEAVE_FUNCTION
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

VOID HorseMatingRule::SendHorseRetrieveSuccessMSG( Obj_Human* pHuman )
{
	//通知邮件
	CHAR  szName[MAX_CHARACTER_NAME] = {0};
	memcpy(szName, pHuman->GetName(), MAX_CHARACTER_NAME);
	CHAR szMsgContent[MAX_MAIL_CONTEX] = {0};
	sprintf(szMsgContent, "%s 恭喜你，你的骑乘交配成功，请您悉心照料它们", pHuman->GetName() );
	pHuman->getScene()->SendNormalMail(pHuman, szName, szMsgContent);
}

INT HorseMatingRule::InitChild( _PET_DB* pPetDB, Obj_Human* pHuman )
{
	__ENTER_FUNCTION

	_HORSE_ATTR* pHorseAttr = g_HorseAttrTbl.GetAttr( pPetDB->m_nDataID );
	if( pHorseAttr->m_iChildIDCount <= 0 )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
	}
	INT iRand = pHuman->getScene()->GetRand100() % pHorseAttr->m_iChildIDCount;
	if( iRand >= pHorseAttr->m_iChildIDCount )
	{
		iRand -= 1;
	}

	INT iChildID = pHorseAttr->m_aChildID[iRand];

	INT iIndex = pHuman->GetPetContain()->GetEmptyItemIndex();
	_PET_DB oPetDB;

	pHuman->getScene()->GetHorseManager()->InitHorseDB( &oPetDB, iChildID );
	oPetDB.m_iSex   = pPetDB->m_iSex;
	oPetDB.m_nLevel = 1;
	oPetDB.m_GUID.Init(pHuman->GetGUID(), g_pTimeManager->CurrentTime());
	g_ItemOperator.SetItemValue(pHuman->GetPetContain(), iIndex, &oPetDB);
	pHuman->RefeshHorseDetailAttrib( oPetDB.m_GUID );
	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	__LEAVE_FUNCTION
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;

}

INT HorseMatingRule::IsCanRetrieveHorse( Obj_Human* pMe, Obj_Human* pCh, Obj_Character* pNPC )
{
	__ENTER_FUNCTION

	_PET_DB* pMyPetDB = GetMatingHorseDB( pMe );
	_PET_DB* pOrPetDB = GetMatingHorseDB( pCh );
	if( pMyPetDB==NULL || pOrPetDB==NULL )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
	}

	TeamInfo* pTeamInf = GetTeamInfo(pMe);
	TestResult( IsSuitabilityMemCount( pTeamInf ) );

	TestResult( IsInTeam( pTeamInf, pMe, pCh ) );

	TestResult( IsTeamInArea( pMe, pCh, pNPC, pTeamInf ) );

	TestResult( IsSpouse(pMyPetDB, pOrPetDB) );

	TestResult( IsMatingFinished(pMyPetDB, pOrPetDB) );

	_PET_DB* pMyEmptyDB = GetEmptyHorseDB( pMe );
	_PET_DB* pOrEmptyDB = GetEmptyHorseDB( pCh );

	TestResult( IsHaveEmptySolt(pMyEmptyDB, pOrEmptyDB) );
	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	__LEAVE_FUNCTION
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

INT HorseMatingRule::IsHaveEmptySolt( _PET_DB* pMyEmptyDB, _PET_DB* pOrEmptyDB )
{
	__ENTER_FUNCTION

	if( pMyEmptyDB==NULL || pOrEmptyDB==NULL )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_HORSE_NO_SOLT;
	}
	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	__LEAVE_FUNCTION
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

INT HorseMatingRule::IsMatingFinished( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB )
{
	__ENTER_FUNCTION

	if( !pMyPetDB->m_bMatingMsgSend )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_FINISH;
	}

	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	__LEAVE_FUNCTION
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

INT HorseMatingRule::IsSpouse( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB )
{
	__ENTER_FUNCTION

	if( pMyPetDB->m_SpouseGUID != pOrPetDB->m_GUID )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_HORSE_SPOUSE;
	}
	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	__LEAVE_FUNCTION
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}

_PET_DB* HorseMatingRule::GetMatingHorseDB( Obj_Human* pHuman )
{
	__ENTER_FUNCTION

	for ( INT i = 0; i < HUMAN_PET_MAX_COUNT; i++ )
	{
		_PET_DB* pPetDB =  pHuman->GetHorseDB( i );
		if( pPetDB == NULL )
		{
			continue;
		}
		if( pPetDB->m_GUID.IsNull() )
		{
			continue;
		}
		if( pPetDB->m_bMating )
		{
			return pPetDB;
		}
	}
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}

_PET_DB* HorseMatingRule::GetEmptyHorseDB( Obj_Human* pHuman )
{
	__ENTER_FUNCTION

	_PET_DB* pPetDB = pHuman->GetEmptyHorseDB();
	return pPetDB;
	__LEAVE_FUNCTION
	return NULL;
}

INT HorseMatingRule::IsCanMating( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB )
{
	__ENTER_FUNCTION

	if( !g_HorseAttrTbl.m_aHorseTbl[pMyPetDB->m_nDataID].m_bCanMating ||
		!g_HorseAttrTbl.m_aHorseTbl[pOrPetDB->m_nDataID].m_bCanMating )
	{
		return GCManipulatePetRet::MANIPULATEPET_RET_HORSE_VARIATION;
	}
	return GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS;
	__LEAVE_FUNCTION
	return GCManipulatePetRet::MANIPULATEPET_RET_UNKNOWN;
}
