#ifndef __TEAM_H__
#define __TEAM_H__

#include "Type.h"

#define LEADER_ID 0



struct TEAMMEMBER
{
	GUID_t				m_Member ;

	//是否暂离开队伍，断线等行为（玩家不申请离队、不被队长请离）都视为暂时离队
	UCHAR			m_IsMomentLeave;	

	GUID_t				m_GUIDEx ;							//PlayerID
	SceneID_t			m_SceneID ;

	CHAR				m_Name[MAX_CHARACTER_NAME];			// 队员的名字，在队员入队的时候用
	INT					m_nPortrait;						// 头像
	USHORT				m_uDataID;							// 队员的性别
	UINT				m_uFamily;							// 2.门派
	INT					m_Level;							// 等级

	void			CleanUp( )
	{
		m_Member = INVALID_ID ;
		m_IsMomentLeave = FALSE;
		m_GUIDEx = INVALID_ID;
		m_SceneID = INVALID_ID;
		memset(m_Name,0,MAX_CHARACTER_NAME) ;
		m_nPortrait = INVALID_ID;
		m_uDataID = 0;
		m_uFamily = INVALID_ID;
		m_Level	  = 0;
	}
};

class Team
{
public :
	Team( ) ;
	~Team( ) ;

	void			CleanUp( ) ;

	BOOL			IsActive( ) ;
	BOOL			AddMember( TEAMMEMBER* pMember ) ;
	BOOL			DelMember( TEAMMEMBER* pMember ) ;
	BOOL			IsMember( TEAMMEMBER* pMember ) ;
	TEAMMEMBER*		Leader( ) ;
	TEAMMEMBER*		Member( int iIndex ) ;
	int				MemberCount( ){ return m_iNumber ; }
	BOOL			IsFull( ){ return (BOOL)(m_iNumber==MAX_TEAM_MEMBER) ; }
	void			Appoint( TEAMMEMBER* pNewLeader ) ;
	
	void			SetIsMomentLeave(TEAMMEMBER* pMember,BOOL IsMomentLeave)
	{
		for( int i=0; i<m_iNumber; i++ )
		{
			if( m_aMember[i].m_Member == pMember->m_Member )
			{
				m_aMember[i].m_IsMomentLeave = IsMomentLeave;
				return  ;
			}
		}

	}

	BOOL			GetIsMomentLeave(int index)
	{
		Assert( index>=0 && index<MAX_TEAM_MEMBER ) ;
		
		return m_aMember[index].m_IsMomentLeave;
	}

	void			SetTeamID( TeamID_t tid )
	{
		Assert( tid>=0 && tid<MAX_TEAMS ) ;
		m_TeamID = tid ;
	};
	TeamID_t		GetTeamID( )
	{
		return m_TeamID ;
	}
	BOOL			IsEmpty( )
	{
		return m_Empty ;
	}
	void			SetEmpty( BOOL bEmpty=TRUE )
	{
		m_Empty = bEmpty ;
	}

	void			AddFollowedMember(GUID_t guid)
	{
		Assert( m_nFollowedMemberCount<=MAX_TEAM_MEMBER );
		m_aFollowedMember[m_nFollowedMemberCount++] = guid;
	}

	void			DelFollowedMember(GUID_t guid);

	void			ClearFollowedMembers()
	{
		m_nFollowedMemberCount = 0;
	}

	INT				GetFollowedmemberCount()
	{
		return m_nFollowedMemberCount;
	}

	GUID_t*			GetFollowedMembers()
	{
		return m_aFollowedMember;
	}

	VOID	SetDistribMode(BYTE nMode)
	{
		Assert(nMode<ITEM_DISTRIB_NUM);
		m_nMode = nMode;
	}

	BYTE   GetDistribMode(){ return m_nMode;}
public :
	TEAMMEMBER		m_aMember[MAX_TEAM_MEMBER] ;
	INT				m_iNumber ;

	TeamID_t		m_TeamID ;
	BOOL			m_Empty ;

	INT				m_nFollowedMemberCount;
	GUID_t			m_aFollowedMember[MAX_TEAM_MEMBER];

	BYTE			m_nMode;
};


/////////////////////////////////////////////////////////////////////////////////
class TeamList
{
public :
	TeamList( ) ;
	~TeamList( ) ;

	void			CleanUp( ) ;
	BOOL			Init( ) ;

	BOOL			HeartBeat( DWORD dwTime ) ;


	Team*			GetTeam( TeamID_t tid )
	{
		Assert( tid>=0 && tid<MAX_TEAMS ) ;
		if (0 > tid || MAX_TEAMS <= tid){	// avertable hacker add by gh 2010/04/06 
			return NULL;
		}
		return &(m_pTeams[tid]) ;
	};
	TeamID_t		CreateTeam( ) ;
	BOOL			DestoryTeam( TeamID_t tid ) ;


public :
	Team*			m_pTeams ;
	int				m_iCount ;
	int				m_iCreateOffset ;	

};

extern TeamList* g_pTeamList ;





#endif
