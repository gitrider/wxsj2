
//-----------------------------------------------------------------------------
// �ļ��� : TeamRecruitManager.h
// ģ��	:	��������������
// ����	 :
// �޸���ʷ:
//����:		�����
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
	//��ȡ��Ϣ
	VOID*					Get( uint id,UCHAR type ) ;
	//ɾ������
	VOID					Remove( uint id,UCHAR type ) ;

	LEADER_INFO_NODE		GetLeaderRecruitInfo(INT index);
	MEMBER_INFO_NODE		GetMemberRecruitInfo(INT index);

	LEADER_INFO_NODE*		GetLeaderNullNode();
	MEMBER_INFO_NODE*		GetMemberNullNode();

protected:
	//�ӳ���������������������
	IDTable					m_LeaderIDTable;
	LEADER_INFO_NODE		m_LeaderData[MAX_TEAMS];

	//��Ա��������������������
	IDTable					m_MemberIDTable;	
	MEMBER_INFO_NODE		m_MemberData[MAX_TEAMS];
};

extern TeamRecruitManager* g_pTeamRecruitManager;

#endif
