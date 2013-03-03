
/********************************************************************************
*	�ļ�����	GMDP_Struct_QuestLog.h
*	�����ˣ�	Nick
*	����ʱ�䣺	2008 �� 4 �� 11 �� 17:00
*
*	����˵����	����questlog.tab�������
*	�޸ļ�¼��
*********************************************************************************/


#pragma once

#include "DB_Struct.h"
#include "GIDBC_Struct.h"
#include "DataBase\WXSJ_DBC.h"


class QuestLogDataMgr
{
protected:

	static QuestLogDataMgr* s_pMe;

public:

	QuestLogDataMgr( VOID );
	~QuestLogDataMgr( VOID );

	static QuestLogDataMgr* GetMe(void) { return s_pMe; }


public:

	BOOL Init( const DBC::DBCFile *pDataBase );

	void SetFilter( INT iLevel );

	// �жϸ�NPC�Ƿ�������
	BOOL IsNpcMission( LPCTSTR szNpcName );

	// �������NPC������
	STRING GetMissionNpcName( INT nMissId );

public:

	struct CompFunc
	{
		bool operator()(_DBC_QUEST_LOG p1, _DBC_QUEST_LOG p2) const
		{
			if( p1.nMinLevel == p2.nMinLevel )
				return p1.nScriptID < p2.nScriptID;
			
			return p1.nMinLevel < p2.nMinLevel;
		}
	};

	typedef std::set< _DBC_QUEST_LOG, CompFunc > QusetLogSet;
	typedef std::vector< _DBC_QUEST_LOG* > QusetLogVec;

	// ������questlog.tab�����������
	QusetLogSet s_allMissions;

	// ������˺����������
	QusetLogVec s_FilteredMissions;

protected:

	// �ȼ���������
	INT			m_iLevelFilter;
};
