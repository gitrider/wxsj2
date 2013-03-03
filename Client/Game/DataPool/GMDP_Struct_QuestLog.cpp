//////////////////////////////////////////////////////
#include "StdAfx.h"
#include "GMDP_Struct_QuestLog.h"


QuestLogDataMgr *QuestLogDataMgr::s_pMe	= NULL;
QuestLogDataMgr::QuestLogDataMgr( VOID )
{
	s_pMe	= this;
	m_iLevelFilter = 0;
}

QuestLogDataMgr::~QuestLogDataMgr( VOID )
{
	QusetLogVec::iterator iter1 = s_FilteredMissions.begin();
	for( ; iter1 != s_FilteredMissions.end(); iter1++ )
	{
		s_FilteredMissions.erase( iter1 );
	}

	QusetLogSet::iterator iter = s_allMissions.begin();
	for( ; iter != s_allMissions.end(); iter++ )
	{
		s_allMissions.erase( iter );
	}

	s_pMe = NULL;
}


BOOL QuestLogDataMgr::Init( const DBC::DBCFile *pFile )
{
	enum QuestLogEnum
	{
		ScriptID		= 0,// 脚本ID
		MissionID,			// 任务MissionID
		MissionName,		// 任务名称
		SceneID,			// 接任务地图ID
		NPCName,			// 接任务NPC名称
		NPCCommit,			// 交任务NPC名称 // 20100511 AddCode
		NPC_X,				// 接任务NPC_X坐标
		NPC_Z,				// 接任务NPC_Z坐标
		MinLevel,			// 接任务最低等级
		MaxLevel,			// 接任务最高等级
		PrevMission1,		// 前置任务1
		PrevMission2,		// 前置任务2
		PrevMission3,		// 前置任务3
		PrevMission4,		// 前置任务4
		PrevMission5,		// 前置任务5
	};

	// 记录数
	INT iRecordCnt = pFile->GetRecordsNum();

	// 字段数
	INT iColumnCnt = pFile->GetFieldsNum();

	INT index = 0;
	INT iLastID = 0;
	for(INT i = 0; i < iRecordCnt; i++)
	{
		_DBC_QUEST_LOG questLog;

		questLog.nScriptID		= pFile->Search_Posistion(i, ScriptID)->iValue;
		questLog.nMissionID		= pFile->Search_Posistion(i, MissionID)->iValue;
		questLog.szName			= pFile->Search_Posistion(i, MissionName)->pString;
		questLog.nSceneID		= pFile->Search_Posistion(i, SceneID)->iValue;
		questLog.szNPCName		= pFile->Search_Posistion(i, NPCName)->pString;
		questLog.szNPCCommit	= pFile->Search_Posistion(i, NPCCommit)->pString; // 20100510 AddCode
		questLog.nNPC_X			= pFile->Search_Posistion(i, NPC_X)->iValue;
		questLog.nNPC_Z			= pFile->Search_Posistion(i, NPC_Z)->iValue;
		questLog.nMinLevel		= pFile->Search_Posistion(i, MinLevel)->iValue;
		questLog.nMaxLevel		= pFile->Search_Posistion(i, MaxLevel)->iValue;
		questLog.nPrevMission1	= pFile->Search_Posistion(i, PrevMission1)->iValue;
		questLog.nPrevMission2	= pFile->Search_Posistion(i, PrevMission2)->iValue;
		questLog.nPrevMission3	= pFile->Search_Posistion(i, PrevMission3)->iValue;
		questLog.nPrevMission4	= pFile->Search_Posistion(i, PrevMission4)->iValue;
		questLog.nPrevMission5	= pFile->Search_Posistion(i, PrevMission5)->iValue;

		s_allMissions.insert( questLog );
	}

	return TRUE;
}


void QuestLogDataMgr::SetFilter( INT iLevel )
{
	if( m_iLevelFilter == iLevel )
		return;

	m_iLevelFilter = iLevel;

	s_FilteredMissions.clear();

	QusetLogSet::iterator iter = s_allMissions.begin();
	for( ; iter != s_allMissions.end(); iter++ )
	{
		_DBC_QUEST_LOG* pMission = (_DBC_QUEST_LOG*)&(*iter);

		if( (pMission->nMinLevel <= m_iLevelFilter || pMission->nMinLevel == -1) && 
			(m_iLevelFilter <= pMission->nMaxLevel || pMission->nMaxLevel == -1) )
		{
			s_FilteredMissions.push_back( pMission );
		}
	}
}


// 判断npc身上是否有可接任务
BOOL QuestLogDataMgr::IsNpcMission( LPCTSTR szNpcName )
{
	QusetLogVec::iterator iter = s_FilteredMissions.begin();
	for( ; iter != s_FilteredMissions.end(); iter++ )
	{
		if( 0 == strcmp( szNpcName, (*iter)->szNPCName ) )
		{
			return TRUE;
		}
	}

	return FALSE;
}


// 获得任务NPC的名字
STRING QuestLogDataMgr::GetMissionNpcName( INT nMissId )
{
	if( nMissId < 0 || nMissId >= s_FilteredMissions.size() )
		return "";

	QusetLogVec::iterator iter = s_FilteredMissions.begin();
	for( ; iter != s_FilteredMissions.end(); iter++ )
	{
		if( nMissId == (*iter)->nMissionID )
		{
			return (*iter)->szNPCName;
		}
	}

	return "";
}

/*
// 获得任务的场景信息
INT QuestLogDataMgr::GetMissionSceneInfo( INT nMissId )
{
	return 0;
}
*/

// 获得npc身上的已接任务的完成情况
//INT QuestLogDataMgr::GetNpcMissionState( LPCTSTR szNpcName )
//{
	// 取得该npc身上的任务
//}


