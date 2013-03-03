
//-----------------------------------------------------------------------------
// 文件名 : TeamRecruitManager.h
// 模块	:	队伍征兵管理器
// 功能	 :
// 修改历史:
//创建:		王勇鸷
//-----------------------------------------------------------------------------

#ifndef __TEAM_RECRUIT_MANAGER__
#define __TEAM_RECRUIT_MANAGER__

#include "IDTable.h"
#include "GameStruct_Team.h"

class TeamRecruitManager
{
public:
	TeamRecruitManager();
	~TeamRecruitManager();

	BOOL					Init( );
	VOID					CleanUp( );
	IDTable*				GetLeaderTable();
	IDTable*				GetMemberTable( );
	BOOL					Add( uint id, VOID* pPtr,UCHAR type) ;
	//读取信息
	VOID*					Get( uint id,UCHAR type ) ;
	//删除表项
	VOID					Remove( uint id,UCHAR type ) ;

	LEADER_INFO_NODE		GetLeaderRecruitInfo(INT index);
	MEMBER_INFO_NODE		GetMemberRecruitInfo(INT index);

	LEADER_INFO_NODE*		GetLeaderNullNode();
	MEMBER_INFO_NODE*		GetMemberNullNode();

protected:
	//队长发布的征兵快速索引表
	IDTable					m_LeaderIDTable;
	LEADER_INFO_NODE		m_LeaderData[MAX_TEAMS];

	//队员发布的征兵快速索引表
	IDTable					m_MemberIDTable;	
	MEMBER_INFO_NODE		m_MemberData[MAX_TEAMS];
};

extern TeamRecruitManager* g_pTeamRecruitManager;

#endif
