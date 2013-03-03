// AbilityManager.cpp
#include "stdafx.h"
#include "AbilityManager.h"
#include "AbilityGeneric.h"
#include "AbilityInlay.h"
#include "AbilityFoundry.h"
#include "AbilityTailor.h"
#include "AbilityGatherFish.h"
#include "AbilityCooking.h"

#include "WXSJ_DBC.h"
#include "DB_Struct.h"
#include "Log.h"
#include "FileDef.h"

using namespace DBC;

AbilityManager* g_pAbilityManager = NULL;

AbilityManager::AbilityManager()
{
__ENTER_FUNCTION

	CleanUp();

	m_pPrescriptionList = new _ABILITY_PRESCRIPTION_TBL;

__LEAVE_FUNCTION
}

AbilityManager::~AbilityManager()
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_ABILITY; ++i )
	{
		if( m_pAbility[i] != NULL )
		{
			SAFE_DELETE( m_pAbility[i] );
		}
	}

	SAFE_DELETE_ARRAY( m_pAbilityExpTbl );

	SAFE_DELETE( m_pPrescriptionList );

	m_AbilityLevelUpConfigManager.CleanUp();

__LEAVE_FUNCTION
}

VOID AbilityManager::CleanUp()
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_ABILITY; ++i )
	{
		m_pAbility[i] = NULL;
	}

	m_AbilityLevelUpConfigManager.CleanUp();

__LEAVE_FUNCTION
}

BOOL AbilityManager::Init()
{
__ENTER_FUNCTION

	BOOL ret;

	Log::SaveLog( SERVER_LOGFILE, "Load AbilityLevelUpDir..." );
	ret = m_AbilityLevelUpConfigManager.Init(DIR_ABILITYLEVELUP_CONFIG);
	AssertEx( ret, "Init AbilityLevelUpConfig failed." );
	DiskLog( SERVER_LOGFILE, "Init AbilityLevelUpConfig Done." );
	
	DiskLog( SERVER_LOGFILE, "Load Ability..." );
	ret = InitAbility( FILE_ABILITY );
	AssertEx( ret, "Init Ability failed." );
	Log::SaveLog( SERVER_LOGFILE, "Load Ability Done." );

	Log::SaveLog( SERVER_LOGFILE, "Load Ability Prescription..." );
	ret = InitPrescriptionList( FILE_PRESCRIPTION );
	AssertEx( ret, "Init Prescription failed." );
	Log::SaveLog( SERVER_LOGFILE, "Load Ability Prescription Done." );

	Log::SaveLog( SERVER_LOGFILE, "Load Ability Exp Table..." );
	ret = InitAbilityExpTable( FILE_ABILITY_EXP );
	AssertEx( ret , "Load Ability Exp Table failed.");
	Log::SaveLog( SERVER_LOGFILE, "Load Ability Exp Table Done." );

	Log::SaveLog( SERVER_LOGFILE, "LoadAll AbilityLevelUpConfig..." );
	ret = m_AbilityLevelUpConfigManager.LoadAllLevelUpConfig();
	AssertEx( ret, "LoadAll AbilityLevelUpConfig failed." );
	Log::SaveLog( SERVER_LOGFILE, "LoadAll AbilityLevelUpConfig Done." );
	
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// 读入生活技能配置文件
BOOL AbilityManager::InitAbility( const CHAR* filename )
{
__ENTER_FUNCTION

	DBCFile ThirdFile(UINT_MAX);
	BOOL ret;

	ret = ThirdFile.OpenFromTXT( filename );
	if( !ret )
		return FALSE;

	INT iTableCount = ThirdFile.GetRecordsNum();
	INT iColumnCount = ThirdFile.GetFieldsNum();

	Assert( iColumnCount );
	Assert( iTableCount );

	Ability TempAbility;
	Ability &rTempAbility = TempAbility;

	// 通过配置文件来配置生成生活技能
	for( INT i=0; i<iTableCount; i++ )
	{ // 技能表的条目数必须等于 ABILITY_CLASS_SIZE 标明的数目
		rTempAbility.AbilityID(ThirdFile.Search_Posistion(i, Ability::TB_ABILITY_ID)->iValue);
		rTempAbility.AbilityName(ThirdFile.Search_Posistion(i, Ability::TB_ABILITY_NAME)->pString);
		rTempAbility.LevelDemand(ThirdFile.Search_Posistion(i, Ability::TB_ABILITY_LEVEL_DEMAND)->iValue);
		rTempAbility.AbilityLevelLimit(ThirdFile.Search_Posistion(i, Ability::TB_ABILITY_LEVEL_LIMIT)->iValue);
		const CHAR *pszConfigFileName = ThirdFile.Search_Posistion(i, Ability::TB_ABILITY_LEVELUP_CONFIG)->pString;
		INT nLevelUpConfigIndex = -1;
		if(!m_AbilityLevelUpConfigManager.AddConfigName(pszConfigFileName, &nLevelUpConfigIndex))
		{
			AssertEx(FALSE, "m_AbilityLevelUpConfigManager.AddConfigName");
		}
		rTempAbility.LevelUpConfigIndex(nLevelUpConfigIndex);
		rTempAbility.OperationTime(ThirdFile.Search_Posistion(i, Ability::TB_ABILITY_OPERATION_TIME)->iValue);
		rTempAbility.OperationToolID(ThirdFile.Search_Posistion(i, Ability::TB_ABILITY_OPERATION_TOOL)->iValue);
		rTempAbility.PlatformDistance(ThirdFile.Search_Posistion(i, Ability::TB_ABILITY_PLATFORM_DISTANCE)->fValue);
		rTempAbility.PlatformID(ThirdFile.Search_Posistion(i, Ability::TB_ABILITY_PLATFORM)->iValue);
		rTempAbility.OperationActionID(ThirdFile.Search_Posistion(i, Ability::TB_ABILITY_OPERATION_ACTION)->iValue);

		INT nAbilityID = rTempAbility.AbilityID();

		switch( nAbilityID )
		{
		case ABILITY_CLASS_INVALID:
			continue;
		case ABILITY_CLASS_NOUSE:
			m_pAbility[nAbilityID] = new AbilityGeneric(rTempAbility);
			Assert(m_pAbility[nAbilityID]);
			break;

		case ABILITY_CLASS_GATHERMINE:
			m_pAbility[nAbilityID] = new AbilityGenericGather(rTempAbility);
			Assert(m_pAbility[nAbilityID]);
			break;

		case ABILITY_CLASS_REAP:
			m_pAbility[nAbilityID] = new AbilityGenericGather(rTempAbility);
			Assert(m_pAbility[nAbilityID]);
			break;

		case ABILITY_CLASS_CATCH:
			m_pAbility[nAbilityID] = new AbilityGenericGather(rTempAbility);
			Assert(m_pAbility[nAbilityID]);
			break;
		/*case ABILITY_CLASS_FISH:
			m_pAbility[ABILITY_CLASS_FISH] = new AbilityGatherFish(rTempAbility);
			Assert(m_pAbility[ABILITY_CLASS_FISH]);
			break;*/

		case ABILITY_CLASS_ARTWORK:
			m_pAbility[nAbilityID] = new AbilityGenericCompound(rTempAbility);
			Assert(m_pAbility[nAbilityID]);
			break;
		case ABILITY_CLASS_FOUNDRY:
			m_pAbility[ABILITY_CLASS_FOUNDRY] = new AbilityFoundry(rTempAbility);
			Assert(m_pAbility[ABILITY_CLASS_FOUNDRY]);
			break;
		case ABILITY_CLASS_TAILOR:
			m_pAbility[ABILITY_CLASS_TAILOR] = new AbilityTailor(rTempAbility);
			Assert(m_pAbility[ABILITY_CLASS_TAILOR]);
			break;
		case ABILITY_CLASS_COOKING:
			m_pAbility[ABILITY_CLASS_COOKING] = new AbilityCooking(rTempAbility);
			Assert(m_pAbility[ABILITY_CLASS_COOKING]);
			break;

		default:
			continue;
		}
		rTempAbility.CleanUp();
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL AbilityManager::InitPrescriptionList( const CHAR* filename )	
{
__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( filename );
	if( !ret )
		return FALSE;

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	Assert(iTableCount<MAX_ABILITY_PRESCRIPTION_NUM);

	INT iValue = 0;
	INT Column = 0;
	
	INT i;
	for(i =0; i<iTableCount; ++i)
	{
		_PRESCRIPTION_DATA *pData;
		pData = &(m_pPrescriptionList->m_Table[i]);

		pData->m_PrescriptionID = ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_PrescriptionID)->iValue;

		iValue = ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_CompoundID)->iValue;
		if( iValue != INVALID_ID && iValue > 0 )
		{
			pData->m_CompoundID = iValue;
		}
		else
		{
			pData->m_CompoundID = INVALID_ID;
		}

		pData->m_CompoundNum = ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_CompoundNum)->iValue;
		pData->m_CompoundQuality = ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_CompoundQuality)->iValue;
		pData->m_AbilityID = ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_AbilityID)->iValue;
		pData->m_AbilityLevel = ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_AbilityLevel)->iValue;

		Column = _PRESCRIPTION_DATA::TB_Stuff_BEGIN;

		for( INT j=0; j<MAX_PRESCRIPTION_STUFF; ++j )
		{
			iValue = ThirdFile.Search_Posistion(i, Column++)->iValue;
			if( iValue != INVALID_ID && iValue > 0 )
			{
				pData->m_Stuff_ID[j] = iValue;
				pData->m_Stuff_Num[j] = ThirdFile.Search_Posistion(i, Column++)->iValue;
			}
			else
			{
				pData->m_Stuff_ID[j] = INVALID_ID;
			}
		}

		pData->m_Attr_Vigor = ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_Attr_VIGOR)->iValue;
		pData->m_Attr_Energy = ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_Attr_ENERGY)->iValue;
		pData->m_Attr_Reserved = ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_Attr_RESERVED)->iValue;
		pData->m_Attr_Reserved2 = ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_Attr_RESERVED2)->iValue;

		pData->m_ColddownTime = ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_ColddownTime)->iValue;

		iValue = ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_ToolID)->iValue;
		if( iValue != INVALID_ID && iValue > 0 )
		{
			pData->m_ToolID = iValue;
		}
		else
		{
			pData->m_ToolID = INVALID_ID;
		}

		pData->m_ColddownID = (CooldownID_t)ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_ColddownID)->iValue;
		pData->m_ExpRequired = ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_ExpRequired)->iValue;
		pData->m_ExpIncrement = ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_ExpIncrement)->iValue;
		pData->m_nOperaTime = ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_OperaTime)->iValue;
		pData->m_ScriptID = ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_SCRIPTID)->iValue;

		iValue = ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_REPLICAID)->iValue;
		if( iValue != INVALID_ID && iValue > 0 )
		{
			pData->m_ReplicaID	= iValue;
		}
		else
		{
			pData->m_ReplicaID	= INVALID_ID;
		}

		pData->m_nSpecStuffSubClass	=
			ThirdFile.Search_Posistion(i, _PRESCRIPTION_DATA::TB_SubClass)->iValue;
	}

	m_pPrescriptionList->m_Count	= i;

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL AbilityManager::InitAbilityExpTable( const CHAR* filename )
{
__ENTER_FUNCTION

	DBCFile AbilityExpTableFile(0);
	BOOL ret = AbilityExpTableFile.OpenFromTXT(filename);

	AssertEx(ret,filename);
	if( !ret )
		return FALSE;

	INT iTableCount		= AbilityExpTableFile.GetRecordsNum();
	INT iTableColumn	= AbilityExpTableFile.GetFieldsNum();
	
	AssertEx(iTableCount>0,filename);
	if(iTableCount <= 0 )
		return FALSE;

	AssertEx( iTableColumn == _AbilityExpTable::ETE_ElementNum , "文件列数不匹配");
	if( iTableColumn != _AbilityExpTable::ETE_ElementNum)
		return FALSE;

	m_AbilityExpTblCount	=	iTableCount;
	m_pAbilityExpTbl		=	new _AbilityExpTable[m_AbilityExpTblCount];
	for(INT i =0;i<iTableCount;i++)
	{
		m_pAbilityExpTbl[i].m_AbilityObjectName		= 		AbilityExpTableFile.Search_Posistion(i,_AbilityExpTable::ETE_ObjectName)->pString;
		m_pAbilityExpTbl[i].m_AbilityBaseExp		=		AbilityExpTableFile.Search_Posistion(i,_AbilityExpTable::ETE_BaseExp)->iValue;
		m_pAbilityExpTbl[i].m_AbilityLevel			= 		AbilityExpTableFile.Search_Posistion(i,_AbilityExpTable::ETE_Level)->iValue;
		m_pAbilityExpTbl[i].m_AbilityCostPercent	=		AbilityExpTableFile.Search_Posistion(i,_AbilityExpTable::ETE_CostPercent)->iValue;
		m_pAbilityExpTbl[i].m_AbilityId				= 		AbilityExpTableFile.Search_Posistion(i,_AbilityExpTable::ETE_AbilityId)->iValue;
		m_pAbilityExpTbl[i].m_AbilityItemTypeSn		= 		AbilityExpTableFile.Search_Posistion(i,_AbilityExpTable::ETE_ItemTypeSn)->iValue;

	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}
