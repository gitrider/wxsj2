/********************************************************************
	created:	2008/03/18
	created:	18:3:2008   14:45
	filename: 	MapServer\Server\Obj\Horse\HorseManager.cpp
	file path:	MapServer\Server\Obj\Horse
	file base:	HorseManager
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "Ini.h"
#include "Log.h"
#include "GameTable.h"
#include "Scene.h"
#include "Obj_Horse.h"
#include "ObjManager.h"
#include "HorseManager.h"
#include "ItemOperator.h"
#include "TimeManager.h"
#define SECSIZE 64

HorseManager::HorseManager( VOID )
{
	__ENTER_FUNCTION
	Cleanup();
	__LEAVE_FUNCTION
}

HorseManager::~HorseManager( VOID )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

Obj_Horse *HorseManager::GetHorse( ObjID_t idObj )
{
	Obj_Horse *pHorse = (Obj_Horse*)(GetScene()->GetObjManager()->GetObj( idObj ));
	if ( pHorse == NULL )
	{
		return NULL;
	}

	if ( pHorse->GetObjType() != Obj::OBJ_TYPE_HORSE )
	{
		Assert( (pHorse->GetObjType() == Obj::OBJ_TYPE_HORSE) && "HorseManager::GetHorse" );
		return NULL;
	}
	return pHorse;
}

BOOL HorseManager::RemoveAllHorse( VOID )
{
__ENTER_FUNCTION

	Scene *pScene;
	Obj_Horse *pHorse;
	UINT i,uCount,uMaxCount,uLength;

	pScene		= GetScene();
	uCount		= 0;
	uMaxCount	= GetCount();
	uLength	= GetLength();

	for ( i = 0; i <uLength; i++ )
	{
		pHorse = (Obj_Horse*)(GetObjByIndex( 0 ));
		if ( pHorse != NULL )
		{
			pHorse->SetActiveFlag( FALSE );
			pHorse->CleanUp();
			pScene->DeleteObject( pHorse );
			uCount++;
			if (uCount >=uMaxCount )
				break;
		}
	}
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL HorseManager::RemoveHorse( ObjID_t idObj )
{
__ENTER_FUNCTION

	Scene* pScene = GetScene();
	if (pScene)
	{
		Obj* pHorse = pScene->GetHorseManager()->GetHorse(idObj);
		Assert( pHorse->GetObjType() == Obj::OBJ_TYPE_HORSE && "HorseManager::RemoveHorse ObjType error" );
		if (pHorse && pHorse->GetObjType() == Obj::OBJ_TYPE_HORSE)
		{
			pHorse->SetActiveFlag( FALSE );
			pHorse->CleanUp();
			pScene->DeleteObject( pHorse );
		}
	}

	return TRUE;
__LEAVE_FUNCTION
	return TRUE;
}

BOOL HorseManager::CreateHorse( INT nDataID, WORLD_POS* pPos, ObjID_t& rPetID )
{
__ENTER_FUNCTION
	// create new OBJ of pet
	Scene* pScene = GetScene();
	Obj_Horse *pHorse = (Obj_Horse*)(pScene->NewObject( Obj::OBJ_TYPE_HORSE ));
	if ( !pHorse )
	{
		return FALSE;
	}
	// 将新创建宠物的ObjID返回
	rPetID = pHorse->GetID();

	_HORSE_ATTR* pPetAttr = g_HorseAttrTbl.GetAttr(nDataID);
	if (!pPetAttr)
		return FALSE;

	_OBJ_HORSE_INIT initPet;
	initPet.m_Pos		= *pPos;
	initPet.m_Dir		= 0.f;
	initPet.m_eLogic	= CHARACTER_LOGIC_IDLE;
	initPet.m_CreatorID	= INVALID_ID;

	InitHorseDB( &initPet.m_PetDB, nDataID );
	initPet.m_GUID		= INVALID_GUID;

	// 初始化Pet
	BOOL bResult = pHorse->Init( &initPet );
	if ( !bResult )
	{
		return FALSE;
	}
	
	// 激活该Pet
	pHorse->SetActiveFlag( TRUE );

	return TRUE;
__LEAVE_FUNCTION
	return TRUE;
}
// 生成一个捕捉后宠物的GUID
BOOL	HorseManager::CreateGUIDOfHorse(_PET_DB *pPetDB, ObjID_t idHuman, ObjID_t idPet)
{
	__ENTER_FUNCTION

	Assert(idHuman != INVALID_ID && idPet != INVALID_ID && "PetManager::CreateGUIDOfPet");
	if(idHuman == INVALID_ID || idPet == INVALID_ID)
	{
		return FALSE;
	}

	Obj_Human	*pHuman;
	Obj_Horse	*pHorse;

	pHuman	= GetScene()->GetHumanManager()->GetHuman(idHuman);
	pHorse	= GetHorse(idPet);

	Assert(pHuman != NULL && pHorse != NULL && "PetManager::CreateGUIDOfPet");
	if(pHuman == NULL || pHorse == NULL)
	{
		return FALSE;
	}

	PET_ATTR* pPetAttr = g_PetAttrTbl.GetAttr(pHorse->GetDataID());
	if (!pPetAttr)
		return FALSE;

	PET_GUID_t guidPet, guidSpouse;
	guidPet.Init(pHuman->GetGUID(), g_pTimeManager->CurrentTime());
	guidSpouse.Reset();

	pPetDB->m_GUID					= guidPet;
	pPetDB->m_SpouseGUID			= guidSpouse;

	
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL HorseManager::MakeCapturePetAttrib(_PET_DB *pPetDB, BOOL bNeedLevelFluctuate/*=TRUE*/, BOOL bReturnToChild/*=FALSE*/)
{
__ENTER_FUNCTION

	CreateAITypeOfPet(*pPetDB);
	return TRUE;

__LEAVE_FUNCTION

	return FALSE ;
}

// 通过DataID去付值一个PET DB
BOOL HorseManager::InitHorseDB( _PET_DB *pPetDB, INT nDataID )
{
__ENTER_FUNCTION

	if ( pPetDB == NULL )
		return FALSE;

	pPetDB->CleanUp();

	_HORSE_ATTR *pPetAttr = NULL;
	//MONSTER_EXT_ATTR *pMonsterAttr = NULL;
	if ( nDataID != INVALID_ID )
	{
		 pPetAttr = g_HorseAttrTbl.GetAttr( nDataID );

	}

	if ( pPetAttr != NULL )
	{
		//pPetDB->m_GUID;
		pPetDB->m_SpouseGUID.Reset();
		pPetDB->m_nDataID				= nDataID;
		strncpy( pPetDB->m_szName, pPetAttr->m_sName, sizeof( pPetAttr->m_sName ) - 1 );
		strncpy( pPetDB->m_szNick, pPetAttr->m_sName, sizeof( pPetAttr->m_sName ) - 1 );
		pPetDB->m_nLevel					= pPetAttr->m_iLevel;
		pPetDB->m_nTakeLevel				= pPetAttr->m_iTakeLevel;
		pPetDB->m_nAttackType				= -1;
		pPetDB->m_AIType					= 0;//pPetAttr->m_AI;
		pPetDB->m_CampData.CleanUp();
		//阵营
		//pPetDB->m_CampData.m_nCampID		= pMonsterAttr->m_Camp;
		//繁殖代数
		pPetDB->m_byGeneration				= pPetAttr->m_byGeneration;
		//快乐度
		pPetDB->m_byHappiness				= 100;
		pPetDB->m_iMatingLevel				= 0;
		pPetDB->m_iStrengthPerception		= GetRand(pPetAttr->m_iStrengthMinPer, pPetAttr->m_iStrengthMaxPer);
		pPetDB->m_iSmartnessPerception		= GetRand(pPetAttr->m_iSmartnessMinPer, pPetAttr->m_iSmartnessMaxPer);
		pPetDB->m_iMindPerception			= GetRand(pPetAttr->m_iMindMinPer, pPetAttr->m_iMindMaxPer);
		pPetDB->m_iConstitutionPerception	= GetRand(pPetAttr->m_iConstitutionMinPer, pPetAttr->m_iConstitutionMaxPer);

		pPetDB->m_iSavvy					= GetRand(pPetAttr->m_iSavvyMin, pPetAttr->m_iSavvyMax);
		pPetDB->m_nGenGu					= GetRand(pPetAttr->m_GenGuMin, pPetAttr->m_GenGuMax);
		pPetDB->m_fGrowRate					= (FLOAT)GetRand(pPetAttr->m_iPullulationRateMin, pPetAttr->m_iPullulationRateMax);
		pPetDB->m_nRemainPoint				= 0;
		pPetDB->m_nExp						= 0;

		pPetDB->m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_STR]		= GetRand(pPetAttr->m_iStrengthMin, pPetAttr->m_iStrengthMax);
		pPetDB->m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_DEX]		= GetRand(pPetAttr->m_iSmartnessMin, pPetAttr->m_iSmartnessMax);
		pPetDB->m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_INT]		= GetRand(pPetAttr->m_iMindMin, pPetAttr->m_iMindMax);
		pPetDB->m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_CON]		= GetRand(pPetAttr->m_iConstitutionMin, pPetAttr->m_iConstitutionMax);
		
		pPetDB->m_iSex						= GetScene()->GetRand100()>=50? 1 : 0;
		for( INT i=0; i<pPetAttr->m_iSkillSlotCount; ++i )
		{
			if( pPetAttr->m_oSkillSlot[i].m_iSkillCount == 0 )
			{
				continue;
			}
			INT iIndex = GetScene()->GetRand100() % pPetAttr->m_oSkillSlot[i].m_iSkillCount;
			if( iIndex >= pPetAttr->m_oSkillSlot[i].m_iSkillCount )
			{
				iIndex -= 1;
			}	
			pPetDB->m_SkillList[i].m_nSkillID = pPetAttr->m_oSkillSlot[i].m_aSkillSlot[iIndex];
			pPetDB->m_SkillList[i].m_bCanUse  = TRUE;
		}
		
	}
	return TRUE;

__LEAVE_FUNCTION

	return FALSE ;
}
VOID	HorseManager::CreateAITypeOfPet(_PET_DB& rPetDB)
{
__ENTER_FUNCTION
	INT idData = rPetDB.m_nDataID; 
	INT& rAIType = rPetDB.m_AIType;

	rAIType = RandGen::GetRand(0, 4);

__LEAVE_FUNCTION
}

VOID HorseManager::CreateTypeOfPet(_PET_DB& rPetDB, BOOL bNeedLevelFluctuate, BOOL bReturnToChild)
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

VOID HorseManager::CreateRemainPointOfPet(_PET_DB& rPetDB)
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

VOID HorseManager::CreateSkillOfPet(_PET_DB& rPetDB)
{
__ENTER_FUNCTION
#define MAX_RANG_VALUE (1000000)
#define RAND_COUNT (5)
	INT idData = rPetDB.m_nDataID;
	_PET_SKILL *paSkillList = rPetDB.m_SkillList;

	INT nIndex = 0;
	INT n;
	for (n = 0; n < PET_MAX_NUM; ++n)
	{
		if (g_PetSkillDistrTbl[n].m_PetID == idData)
		{
			nIndex = n;
			break;
		}
	}

    if (n == PET_MAX_NUM)
	{
		Assert(FALSE && "idData ... not found!");
		return ;
	}

	INT nRangeA = g_PetSkillDistrTbl[nIndex].m_Table[0].m_nSkillRange;
	INT nRangeB = nRangeA + g_PetSkillDistrTbl[nIndex].m_Table[1].m_nSkillRange;
	INT nRangeC = nRangeB + g_PetSkillDistrTbl[nIndex].m_Table[2].m_nSkillRange;
	INT nRangeD = nRangeC + g_PetSkillDistrTbl[nIndex].m_Table[3].m_nSkillRange;
	INT nRangeE = nRangeD + g_PetSkillDistrTbl[nIndex].m_Table[4].m_nSkillRange;

	
	for (INT i = 0; i < RAND_COUNT; ++i)
	{
		INT nRet = RandGen::GetRand(0, MAX_RANG_VALUE);
		if (nRet <= nRangeA)
		{// 0~a
			INT nSkillIndex = g_PetSkillDistrTbl[nIndex].m_Table[0].m_nSkillIndex;
			AddPetSkill(paSkillList, nSkillIndex);
		}
		else if (nRet <= nRangeB)
		{// a+1~b
			INT nSkillIndex = g_PetSkillDistrTbl[nIndex].m_Table[1].m_nSkillIndex;
			AddPetSkill(paSkillList, nSkillIndex);
		}
		else if (nRet <= nRangeC)
		{// a+b+1~c
			INT nSkillIndex = g_PetSkillDistrTbl[nIndex].m_Table[2].m_nSkillIndex;
			AddPetSkill(paSkillList, nSkillIndex);
		}
		else if (nRet <= nRangeD)
		{// a+b+c+1~d
			INT nSkillIndex = g_PetSkillDistrTbl[nIndex].m_Table[3].m_nSkillIndex;
			AddPetSkill(paSkillList, nSkillIndex);
		}
		else if (nRet <= nRangeE)
		{// a+b+c+d+1~e
			INT nSkillIndex = g_PetSkillDistrTbl[nIndex].m_Table[4].m_nSkillIndex;
			AddPetSkill(paSkillList, nSkillIndex);
		}
		else
		{// a+b+c+d+e~∑
		}
	}
__LEAVE_FUNCTION
}

VOID HorseManager::CreateGrowRateOfPet(_PET_DB& rPetDB)
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

VOID HorseManager::CreatePerceptionOfPet(_PET_DB& rPetDB)
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

// 向宠物的技能列表中增加新技能
VOID HorseManager::AddPetSkill(_PET_SKILL *paSkillList, INT& rSkillIndex)
{
__ENTER_FUNCTION

	if (rSkillIndex > PET_SKILL_NUM)
	{
		Assert(FALSE && "rSkillIndex > PET_SKILL_NUM");
		return ;
	}
	INT nSkillID = -1;
	INT nSkillType = -1;
	for (INT i = 0; i < PET_SKILL_NUM; ++i)
	{// 根据索引查找相应的SkillID
		if (g_PetSkillIndexTbl[i].m_nIndex == rSkillIndex)
		{
			nSkillID = g_PetSkillIndexTbl[i].m_nSkillID;
			// 根据技能ID判断该技能是否是手动技能，如果是手动技能则加入到手动技能槽中
			SkillTemplateData_T const* pSkillTemplateData = g_SkillTemplateDataMgr.GetInstanceByID(nSkillID);
			if (!pSkillTemplateData)
			{
				return ;
			}
			ID_t OperateModeOfPetSkill = pSkillTemplateData->GetOperateModeForPetSkill();
			if (PET_SKILL_OPERATE_NEEDOWNER == OperateModeOfPetSkill)
			{// 是手动技能
				paSkillList[PET_SKILL_INDEX_CONTROL_BY_PLAYER].m_nSkillID = nSkillID;
				return ;
			}
			break;
		}
	}

	for (INT j = PET_SKILL_INDEX_CONTROL_BY_AI0 ; j < PET_MAX_SKILL_COUNT; ++j) 
	{
		// 如果已经拥有该技能则直接返回
		if (paSkillList[j].m_nSkillID == nSkillID)
		{
			return ;
		}
        // 如果有可用的技能槽才加入技能列表
		if (paSkillList[j].m_nSkillID == INVALID_ID) 
		{
			paSkillList[j].m_nSkillID = nSkillID;
			break;
		}
	}
__LEAVE_FUNCTION
}

BOOL HorseManager::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

	if( !m_LogicTimer.CountingTimer(uTime ) )
		return TRUE;

#define MAX_REMOVE_SIZE 256
	INT aNeedRemovePet[MAX_REMOVE_SIZE] ;
	ClearRemoveArray(aNeedRemovePet);


	INT nRemoveCount=0 ;

	INT i;
	for ( i = 0; i < (INT)m_uCount; i++ )
	{
		if ( m_papObjs[i] != NULL )
		{
			_MY_TRY
			{
				if ( m_papObjs[i]->IsActiveObj() )
				{
					m_papObjs[i]->HeartBeat(uTime ) ;
				}
				else
				{
					aNeedRemovePet[nRemoveCount++] = i ;
					if( nRemoveCount >= MAX_REMOVE_SIZE-1 )
					{
						DeleteDeadHorse( nRemoveCount, aNeedRemovePet );
						nRemoveCount = 0;
						ClearRemoveArray( aNeedRemovePet );
					}
				}
			}
			_MY_CATCH
			{
				SaveCodeLog( ) ;
			}
		}
	}

	DeleteDeadHorse(nRemoveCount, aNeedRemovePet);

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

VOID HorseManager::DeleteDeadHorse( INT nRemoveCount, INT* aNeedRemovePet )
{
	__ENTER_FUNCTION

	for( INT i=nRemoveCount-1; i>=0; i-- )
	{
		_MY_TRY
		{
			Obj_Horse* pHorse = (Obj_Horse*)(m_papObjs[aNeedRemovePet[i]]) ;
			if( pHorse != NULL )
			{
				pHorse->SetActiveFlag( FALSE );
				pHorse->CleanUp();
				GetScene()->DeleteObject( pHorse ) ;
			}
		}
		_MY_CATCH
		{
			SaveCodeLog( ) ;
		}
	}	
	__LEAVE_FUNCTION
}

INT HorseManager::GetRand( INT iMin, INT iMax )
{
	__ENTER_FUNCTION

	FLOAT fRand = ((FLOAT)(m_pScene->GetRand100()) / 100.0f);
	return (INT)(iMin + ( (FLOAT)(iMax-iMin) * fRand +0.5f ));

	__LEAVE_FUNCTION
	return iMin;
}

VOID HorseManager::ClearRemoveArray( INT* aNeedRemovePet )
{
	__ENTER_FUNCTION

	memset( aNeedRemovePet, 0, sizeof(INT)*MAX_REMOVE_SIZE ) ;

	__LEAVE_FUNCTION

}

VOID HorseManager::Cleanup()
{
	m_pScene = NULL;
}