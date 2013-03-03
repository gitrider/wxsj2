
/********************************************************************************
*	文件名：	GMDP_Struct_QuestLog.h
*	创建人：	Nick
*	创建时间：	2008 年 4 月 11 日 17:00
*
*	功能说明：	保存questlog.tab表的数据
*	修改记录：
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

	// 判断该NPC是否有任务
	BOOL IsNpcMission( LPCTSTR szNpcName );

	// 获得任务NPC的名字
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

	// 保存了questlog.tab表的所有数据
	QusetLogSet s_allMissions;

	// 保存过滤后的任务数据
	QusetLogVec s_FilteredMissions;

protected:

	// 等级过滤条件
	INT			m_iLevelFilter;
};
