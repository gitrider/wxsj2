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
		ScriptID		= 0,// �ű�ID
		MissionID,			// ����MissionID
		MissionName,		// ��������
		SceneID,			// �������ͼID
		NPCName,			// ������NPC����
		NPCCommit,			// ������NPC���� // 20100511 AddCode
		NPC_X,				// ������NPC_X����
		NPC_Z,				// ������NPC_Z����
		MinLevel,			// ��������͵ȼ�
		MaxLevel,			// ��������ߵȼ�
		PrevMission1,		// ǰ������1
		PrevMission2,		// ǰ������2
		PrevMission3,		// ǰ������3
		PrevMission4,		// ǰ������4
		PrevMission5,		// ǰ������5
	};

	// ��¼��
	INT iRecordCnt = pFile->GetRecordsNum();

	// �ֶ���
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


// �ж�npc�����Ƿ��пɽ�����
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


// �������NPC������
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
// �������ĳ�����Ϣ
INT QuestLogDataMgr::GetMissionSceneInfo( INT nMissId )
{
	return 0;
}
*/

// ���npc���ϵ��ѽ������������
//INT QuestLogDataMgr::GetNpcMissionState( LPCTSTR szNpcName )
//{
	// ȡ�ø�npc���ϵ�����
//}


