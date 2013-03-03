// PetManager.cpp

#include "stdafx.h"
#include "Ini.h"
#include "Log.h"
#include "GameTable.h"
#include "Scene.h"
#include "Obj_Pet.h"
#include "ObjManager.h"
#include "PetManager.h"
#include "ItemOperator.h"
#include "TimeManager.h"
#define SECSIZE 64

PetManager::PetManager( VOID )
{
__ENTER_FUNCTION
	Cleanup();
__LEAVE_FUNCTION
}

PetManager::~PetManager( VOID )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

VOID PetManager::Cleanup()
{
	memset( &m_oInit, 0, sizeof(m_oInit) );
	m_pScene = NULL;
}

Obj_Pet *PetManager::GetPet( ObjID_t idObj )
{
	Obj_Pet *pPet = (Obj_Pet*)(GetScene()->GetObjManager()->GetObj( idObj ));
	if ( pPet == NULL )
	{
		return NULL;
	}

	if ( pPet->GetObjType() != Obj::OBJ_TYPE_PET )
	{
		Assert( (pPet->GetObjType() == Obj::OBJ_TYPE_PET) && "PetManager::GetPet" );
		return NULL;
	}
	return pPet;
}

VOID PetManager::SetInitData( _MONSTER_PET_ATTR& oInit )
{
	m_oInit = oInit;
}

const _MONSTER_PET_ATTR& PetManager::GetInitData()
{
	return m_oInit;
}

BOOL PetManager::RemoveAllPet( VOID )
{
__ENTER_FUNCTION

	Scene *pScene;
	Obj_Pet *pPet;
	UINT i,uCount,uMaxCount,uLength;

	pScene		= GetScene();
	uCount		= 0;
	uMaxCount	= GetCount();
	uLength	= GetLength();

	for ( i = 0; i <uLength; i++ )
	{
		pPet = (Obj_Pet*)(GetObjByIndex( 0 ));
		if ( pPet != NULL )
		{
			pPet->SetActiveFlag( FALSE );
			pPet->CleanUp();
			pScene->DeleteObject( pPet );
			uCount++;
			if (uCount >=uMaxCount )
				break;
		}
	}
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL PetManager::RemovePet( ObjID_t idObj )
{
__ENTER_FUNCTION

	Scene* pScene = GetScene();
	if (pScene)
	{
		Obj* pPet = pScene->GetPetManager()->GetPet(idObj);
		if (pPet && pPet->GetObjType() == Obj::OBJ_TYPE_PET)
		{
			pPet->SetActiveFlag( FALSE );
			pPet->CleanUp();
			pScene->DeleteObject( pPet );
		}
	}

	return TRUE;
__LEAVE_FUNCTION
	return TRUE;
}

BOOL PetManager::CreatePet( INT nDataID, WORLD_POS* pPos, ObjID_t& rPetID )
{
__ENTER_FUNCTION
	// create new OBJ of pet
	Obj_Pet *pPet = (Obj_Pet*)(GetScene()->NewObject( Obj::OBJ_TYPE_PET ));
	if ( !pPet )
	{
		return FALSE;
	}
	// 将新创建宠物的ObjID返回
	rPetID = pPet->GetID();

	MONSTER_EXT_ATTR* pMonsterExtAttr = g_MonsterAttrExTbl.GetExtAttr(nDataID);
	if (!pMonsterExtAttr)
		return FALSE;

	_OBJ_PET_INIT initPet;
	initPet.m_Pos		= *pPos;
	initPet.m_Dir		= 0.f;
	initPet.m_eLogic	= CHARACTER_LOGIC_IDLE;
	initPet.m_CreatorID	= INVALID_ID;

	InitPetDB( &initPet.m_PetDB, nDataID );
	initPet.m_GUID		= INVALID_GUID;

	// 初始化Pet
	BOOL bResult = pPet->Init( &initPet );
	if ( !bResult )
	{
		return FALSE;
	}
	
	// 激活该Pet
	pPet->SetActiveFlag( TRUE );

	return TRUE;
__LEAVE_FUNCTION
	return TRUE;
}
// 生成一个捕捉后宠物的GUID
BOOL	PetManager::CreateGUIDOfPet(_PET_DB *pPetDB, ObjID_t idHuman, ObjID_t idPet)
{
	__ENTER_FUNCTION

	Assert(idHuman != INVALID_ID && idPet != INVALID_ID && "PetManager::CreateGUIDOfPet");
	if(idHuman == INVALID_ID || idPet == INVALID_ID)
	{
		return FALSE;
	}

	Obj_Human	*pHuman;
	Obj_Pet		*pPet;

	pHuman	= GetScene()->GetHumanManager()->GetHuman(idHuman);
	pPet	= GetPet(idPet);

	Assert(pHuman != NULL && pPet != NULL && "PetManager::CreateGUIDOfPet");
	if(pHuman == NULL || pPet == NULL)
	{
		return FALSE;
	}

	PET_ATTR* pPetAttr = g_PetAttrTbl.GetAttr(pPet->GetDataID());
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

BOOL PetManager::MakeCapturePetAttrib(_PET_DB *pPetDB, BOOL bNeedLevelFluctuate/*=TRUE*/, BOOL bReturnToChild/*=FALSE*/)
{
__ENTER_FUNCTION

	CreateAITypeOfPet(*pPetDB);

	return TRUE;

__LEAVE_FUNCTION

	return FALSE ;
}

// 通过DataID去付值一个PET DB
BOOL PetManager::InitPetDB( _PET_DB *pPetDB, INT nDataID )
{
__ENTER_FUNCTION

	if ( pPetDB == NULL )
		return FALSE;

	pPetDB->CleanUp();

	//_HORSE_ATTR *pPetAttr = NULL;
	MONSTER_EXT_ATTR *pPetAttr = NULL;
	if ( nDataID != INVALID_ID )
	{
		 pPetAttr = g_MonsterAttrExTbl.GetExtAttr( nDataID );
	}

	if ( pPetAttr != NULL )
	{
		pPetDB->m_SpouseGUID.Reset();
		pPetDB->m_nDataID				= nDataID;
		strncpy( pPetDB->m_szNick, "", sizeof( pPetDB->m_szNick ) - 1 );

		pPetDB->m_nAttackType				= -1;
		pPetDB->m_AIType					= 0;//pPetAttr->m_AI;
		pPetDB->m_CampData.CleanUp();
		//阵营
		pPetDB->m_CampData.m_nCampID		= pPetAttr->m_Camp;
		//繁殖代数
		pPetDB->m_byGeneration				= 0;
		//快乐度
		pPetDB->m_byHappiness				= 100;
		
	}
	return TRUE;

__LEAVE_FUNCTION

	return FALSE ;
}
VOID	PetManager::CreateAITypeOfPet(_PET_DB& rPetDB)
{
__ENTER_FUNCTION
	INT idData = rPetDB.m_nDataID; 
	INT& rAIType = rPetDB.m_AIType;
	rAIType = RandGen::GetRand(0, 4);

__LEAVE_FUNCTION
}

VOID PetManager::CreateTypeOfPet(_PET_DB& rPetDB, BOOL bNeedLevelFluctuate, BOOL bReturnToChild)
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

VOID PetManager::CreateRemainPointOfPet(_PET_DB& rPetDB)
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

VOID PetManager::CreateSkillOfPet(_PET_DB& rPetDB)
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

VOID PetManager::CreateGrowRateOfPet(_PET_DB& rPetDB)
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

VOID PetManager::CreatePerceptionOfPet(_PET_DB& rPetDB)
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

// 向宠物的技能列表中增加新技能
VOID PetManager::AddPetSkill(_PET_SKILL *paSkillList, INT& rSkillIndex)
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

BOOL PetManager::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

	if( !m_LogicTimer.CountingTimer(uTime ) )
		return TRUE;

#define MAX_REMOVE_SIZE 256
	INT aNeedRemovePet[MAX_REMOVE_SIZE] ;
	memset( aNeedRemovePet, 0, sizeof(INT)*MAX_REMOVE_SIZE ) ;

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
					BOOL bRet = m_papObjs[i]->HeartBeat_OutZone(uTime );
					if( bRet==FALSE )
					{
						aNeedRemovePet[nRemoveCount++] = i ;
					}
				}
			}
			_MY_CATCH
			{
				SaveCodeLog( ) ;
			}
		}
	}

	for( i=nRemoveCount-1; i>=0; i-- )
	{
		_MY_TRY
		{
			Obj_Pet* pPet = (Obj_Pet*)(m_papObjs[aNeedRemovePet[i]]) ;
			if( pPet != NULL )
			{
				pPet->SetActiveFlag( FALSE );
				pPet->CleanUp();
				GetScene()->DeleteObject( pPet ) ;
			}
		}
		_MY_CATCH
		{
			SaveCodeLog( ) ;
		}
	}


	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

INT PetManager::GetRand( INT iMin, INT iMax )
{
	FLOAT fRand = ((FLOAT)(m_pScene->GetRand100()) / 100.0f);
	return (INT)(iMin + ( (FLOAT)(iMax-iMin) * fRand +0.5f ));
}

