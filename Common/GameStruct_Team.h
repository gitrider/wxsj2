/********************************************************************************
 *	文件名：	GameStruct_Team.h
 *	全路径：	d:\Prj\Common\GameStruct_Team.h
 *	创建人：	胡繁
 *	创建时间：	2006 年 4 月 24 日	18:14
 *
 *	功能说明：	简单的 Buff 列表，可以用在头像下的 Buff 图标显示
 *	修改记录：
*********************************************************************************/

#ifndef GAMESTRUCT_TEAM_H
#define GAMESTRUCT_TEAM_H

#include "DB_Struct.h"
#include "PacketFactory.h"

//队长或队员发布征兵消息所用到的数据结构

//组队宣言长度上限
#define TEAM_ENOUNCE_LEN 12

//每一次请求最多返回50条记录
#define	ONCE_RECRUIT_LIST_LEN	50

//消息类型
enum RECRUIT_TYPE
{
	LEADER_RECRUIT = 0,		//队长发布征收队员消息
	MEMBER_RECRUIT ,		//队员发布征收队员消息
};

//组队目的
enum TEAME_TARGET
{
	EXERCISE_LEVEL = 0,		//是否练级
	DO_MISSION,				//是否做任务
	BOUT,					//是否切磋
	KILL_BOSS,				//是否打BOSS
	KILL_ENEMY,				//是否敌国杀人
	HANG,					//是否挂机
	TARGET_NUMBER,
};


//队长征兵条件
struct LEADER_RECRUIT_ATTRIB
{
	LEADER_RECRUIT_ATTRIB()
	{
		m_MinLevel = 0;
		m_MaxLevel = MAX_PLAYER_EXP_LEVEL;
		memset(m_IsInCountry,0,sizeof(m_IsInCountry));
		memset(m_IsTarget,0,sizeof(m_IsTarget));
		memset(m_IsMenPaiID,0,sizeof(m_IsMenPaiID));
		memset(m_szTeamEnounce,0,TEAM_ENOUNCE_LEN);
	}

	INT					m_MinLevel;				//匹配的最小等级
	INT					m_MaxLevel;				//匹配的最大等级

	//匹配哪些国家
	BOOL				m_IsInCountry[COUATTRIBUTE_NUMBER];			

	//组队目标
	BOOL				m_IsTarget[TARGET_NUMBER];		

	//职业范围
	BOOL				m_IsMenPaiID[MATTRIBUTE_NUMBER];

	//组队宣言
	CHAR				m_szTeamEnounce[TEAM_ENOUNCE_LEN];

	UINT	GetSize()	const
	{
		UINT size = sizeof(INT) * 2 + sizeof(m_IsInCountry) + sizeof(m_IsTarget) 
					+ sizeof(m_IsMenPaiID) + sizeof(m_szTeamEnounce);
		return size;
	}

	BOOL Read(SocketInputStream& iStream )
	{
		__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_MinLevel), sizeof(m_MinLevel) ) ;
		iStream.Read( (CHAR*)(&m_MaxLevel), sizeof(m_MaxLevel) ) ;

		iStream.Read( (CHAR*)m_IsInCountry, sizeof(m_IsInCountry) ) ;
		iStream.Read( (CHAR*)m_IsTarget, sizeof(m_IsTarget) ) ;
		iStream.Read( (CHAR*)m_IsMenPaiID,  sizeof(m_IsMenPaiID) ) ;

		iStream.Read( (CHAR*)m_szTeamEnounce, sizeof(m_szTeamEnounce) ) ;

		return TRUE;
	
		__LEAVE_FUNCTION

		return FALSE;
	}

	BOOL Write(SocketOutputStream& oStream )	const
	{
		__ENTER_FUNCTION

			oStream.Write( (CHAR*)(&m_MinLevel), sizeof(m_MinLevel) ) ;
			oStream.Write( (CHAR*)(&m_MaxLevel), sizeof(m_MaxLevel) ) ;

			oStream.Write( (CHAR*)m_IsInCountry,sizeof(m_IsInCountry)) ;
			oStream.Write( (CHAR*)m_IsTarget, sizeof(m_IsTarget) ) ;
			oStream.Write( (CHAR*)m_IsMenPaiID,  sizeof(m_IsMenPaiID) ) ;
		
			oStream.Write( (CHAR*)m_szTeamEnounce,sizeof(m_szTeamEnounce)) ;

		return TRUE;

		__LEAVE_FUNCTION

		return FALSE;
	}
};


//队员征兵条件
struct MEMBER_RECRUIT_ATTRIB
{
	MEMBER_RECRUIT_ATTRIB()
	{
		m_MinLevel = 0;
		m_MaxLevel = MAX_PLAYER_EXP_LEVEL;
		memset(m_IsInCountry,0,sizeof(m_IsInCountry));
		memset(m_IsTarget,0,sizeof(m_IsTarget));
		memset(m_szTeamEnounce,0,TEAM_ENOUNCE_LEN);
	}

	INT					m_MinLevel;				//匹配的最小等级
	INT					m_MaxLevel;				//匹配的最大等级

	//匹配哪些国家
	BOOL				m_IsInCountry[COUATTRIBUTE_NUMBER];			

	//组队目标
	BOOL				m_IsTarget[TARGET_NUMBER];		

	//组队宣言
	CHAR				m_szTeamEnounce[TEAM_ENOUNCE_LEN];

	UINT	GetSize()	const
	{
		return sizeof(INT) + sizeof(INT) + sizeof(m_IsInCountry) + 
			sizeof(m_IsTarget) + sizeof(m_szTeamEnounce);
	}

	BOOL Read(SocketInputStream& iStream )
	{
		__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_MinLevel), sizeof(m_MinLevel) ) ;
		iStream.Read( (CHAR*)(&m_MaxLevel), sizeof(m_MaxLevel) ) ;

		iStream.Read( (CHAR*)m_IsInCountry, sizeof(m_IsInCountry) ) ;
		iStream.Read( (CHAR*)m_IsTarget, sizeof(m_IsTarget) ) ;
		iStream.Read( (CHAR*)m_szTeamEnounce, sizeof(m_szTeamEnounce) ) ;
	
		return TRUE;
	
		__LEAVE_FUNCTION

		return FALSE;
	}

	BOOL Write(SocketOutputStream& oStream )	const
	{
		__ENTER_FUNCTION

			oStream.Write( (CHAR*)(&m_MinLevel), sizeof(m_MinLevel) ) ;
			oStream.Write( (CHAR*)(&m_MaxLevel), sizeof(m_MaxLevel) ) ;

			oStream.Write( (CHAR*)m_IsInCountry, sizeof(m_IsInCountry) ) ;
			oStream.Write( (CHAR*)m_IsTarget, sizeof(m_IsTarget) ) ;
			oStream.Write( (CHAR*)m_szTeamEnounce, sizeof(m_szTeamEnounce) ) ;	

		return TRUE;

		__LEAVE_FUNCTION

		return FALSE;
	}

};

//发布者信息
struct RECRUIT_OWNER_INFO
{
	RECRUIT_OWNER_INFO()
	{
		m_NameLen = 0;
		memset(m_szUserName,0,MAX_CHARACTER_NAME);
		m_Level =  INVALID_ID;
		m_ManPai =  INVALID_ID ;
		m_Country =  INVALID_ID;		
	}

	UCHAR	m_NameLen;
	CHAR	m_szUserName[MAX_CHARACTER_NAME];
	INT		m_Level;
	UCHAR	m_ManPai;
	UCHAR	m_Country;		//国家
};


//结点状态
enum NODE_STATE
{
	NODE_NULL = 0 ,		//空
	NODE_USING,			//使用中
	STATE_NUM,
};


//队长征兵时保存到征兵管理器中的信息，玩家查询征兵结果时，在管理器中查找
struct LEADER_INFO_NODE
{
	LEADER_INFO_NODE()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		memset(&m_OwnerInfo,0,sizeof(RECRUIT_OWNER_INFO));
		memset(&m_LeaderRecruit,0,sizeof(LEADER_RECRUIT_ATTRIB));
		m_NodeState = NODE_NULL;	
	}

	VOID	SetOwnerData(RECRUIT_OWNER_INFO& Data)
	{
		memcpy(&m_OwnerInfo,&Data,sizeof(RECRUIT_OWNER_INFO) );	
	}

	VOID	SetRecruitData(LEADER_RECRUIT_ATTRIB& Data)
	{
		memcpy( &m_LeaderRecruit,&Data,sizeof(LEADER_RECRUIT_ATTRIB) );
	}

	BYTE					m_NodeState;
	RECRUIT_OWNER_INFO		m_OwnerInfo;
	LEADER_RECRUIT_ATTRIB	m_LeaderRecruit;
};

//队员征兵时保存到征兵管理器中的信息，玩家查询征兵结果时，在管理器中查找
struct MEMBER_INFO_NODE
{
	MEMBER_INFO_NODE()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		memset(&m_OwnerInfo,0,sizeof(RECRUIT_OWNER_INFO));
		memset(&m_MemberRecruit,0,sizeof(MEMBER_RECRUIT_ATTRIB));
		m_NodeState = NODE_NULL;	
	}

	VOID	SetOwnerData(RECRUIT_OWNER_INFO& Data)
	{
		memcpy(&m_OwnerInfo,&Data,sizeof(RECRUIT_OWNER_INFO) );	
	}

	VOID	SetRecruitData(MEMBER_RECRUIT_ATTRIB& Data)
	{
		memcpy(&m_MemberRecruit,&Data,sizeof(MEMBER_RECRUIT_ATTRIB) );
	}

	BYTE					m_NodeState;
	RECRUIT_OWNER_INFO		m_OwnerInfo;
	MEMBER_RECRUIT_ATTRIB	m_MemberRecruit;
};



//玩家请求得到征兵结果，返回给该玩家的消息结构
struct RETURN_RECRUIT_INFO
{
	RETURN_RECRUIT_INFO()
	{
		m_Level	= INVALID_ID;
		m_ManPai	= INVALID_ID;
		m_Country	= INVALID_ID;		
		m_NameLen = 0;
		memset(m_TeamTarget,0,TARGET_NUMBER);
		memset(m_szTeamEnounce,0,TEAM_ENOUNCE_LEN);
	}

	UINT	GetSize()	const
	{
		UINT size = sizeof(m_Level) + sizeof(m_ManPai) + 
					sizeof(m_Country) + sizeof(m_TeamTarget) 
					+ sizeof(m_NameLen) + m_NameLen * sizeof(CHAR)
					+ sizeof(m_szTeamEnounce);
		return size;
	}

	BOOL Read(SocketInputStream& iStream )
	{
		__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_Level), sizeof(m_Level) ) ;
		iStream.Read( (CHAR*)(&m_ManPai), sizeof(m_ManPai) ) ;
		iStream.Read( (CHAR*)(&m_Country), sizeof(m_Country) ) ;
		iStream.Read( (CHAR*)m_TeamTarget,  sizeof(m_TeamTarget) ) ;
		iStream.Read( (CHAR*)m_szTeamEnounce,  sizeof(m_szTeamEnounce) ) ;
		iStream.Read( (CHAR*)(&m_NameLen), sizeof(m_NameLen) ) ;
		iStream.Read( (CHAR*)m_szUserName,  m_NameLen*sizeof(CHAR) ) ;

		return TRUE;
	
		__LEAVE_FUNCTION

		return FALSE;
	}

	BOOL Write(SocketOutputStream& oStream )	const
	{
		__ENTER_FUNCTION

			oStream.Write( (CHAR*)(&m_Level), sizeof(m_Level) ) ;
			oStream.Write( (CHAR*)(&m_ManPai), sizeof(m_ManPai) ) ;
			oStream.Write( (CHAR*)(&m_Country), sizeof(m_Country) ) ;
			oStream.Write( (CHAR*)m_TeamTarget, sizeof(m_TeamTarget) ) ;
			oStream.Write( (CHAR*)m_szTeamEnounce,  sizeof(m_szTeamEnounce) ) ;
			oStream.Write( (CHAR*)(&m_NameLen), sizeof(m_NameLen) ) ;
			oStream.Write( (CHAR*)m_szUserName,  m_NameLen*sizeof(CHAR) ) ;
		return TRUE;

		__LEAVE_FUNCTION

		return FALSE;
	}

	UCHAR	m_NameLen;
	CHAR	m_szUserName[MAX_CHARACTER_NAME];
	INT		m_Level;
	UCHAR	m_ManPai;
	UCHAR	m_Country;		//国家
	BOOL	m_TeamTarget[TARGET_NUMBER];	//组队目的

	//组队宣言
	CHAR				m_szTeamEnounce[TEAM_ENOUNCE_LEN];
};


class SimpleImpactList
{
	INT						m_nNumOfImpacts;				// BUFF 数量
	ImpactID_t				m_aBuffID[MAX_IMPACT_NUM];		// BUFF 列表

public:
	SimpleImpactList()		{ CleanUp(); }

	VOID					CleanUp() { m_nNumOfImpacts = 0; }

	INT						GetImpactsCount() { return m_nNumOfImpacts; }

	VOID					AddImpact( ImpactID_t impact )
	{
		Assert( m_nNumOfImpacts <= MAX_IMPACT_NUM );
		m_aBuffID[m_nNumOfImpacts++] = impact;
	}

	VOID					RemoveImpact( ImpactID_t impact )
	{
		Assert( m_nNumOfImpacts > 0 );
		for( INT i=0; i<m_nNumOfImpacts; ++i )
		{
			if( m_aBuffID[i] == impact )
			{
				--m_nNumOfImpacts;

				for( INT j=i; j<m_nNumOfImpacts; ++j )
				{
					m_aBuffID[j] = m_aBuffID[j+1];
				}

				return;
			}
		}
	}

	VOID					SetImpactList( const SimpleImpactList* pSimpleImpactList )
	{
		memcpy( this, pSimpleImpactList, sizeof(*this) );
	}

	VOID					SetImpactList( const _IMPACT_LIST* pImpactList )
	{
		m_nNumOfImpacts = pImpactList->m_Count;
		for( INT i=0; i<m_nNumOfImpacts; ++i )
		{
			m_aBuffID[i] = pImpactList->m_aImpacts[i].GetImpactID();
		}
	}

	BOOL					operator==( const _IMPACT_LIST& ImpactList ) const
	{
		if( m_nNumOfImpacts != ImpactList.m_Count )
		{
			return FALSE;
		}

		for( INT i=0; i<ImpactList.m_Count; ++i )
		{ // 查找是否 Buff 列表已经改变，每个当前 ID 拿去 Cache 中比较一下是否存在
			INT j;
			for( j=0; j<m_nNumOfImpacts; ++j )
			{
				if( ImpactList.m_aImpacts[i].GetImpactID() == m_aBuffID[j] )
				{
					break;
				}
			}

			if ( j >= m_nNumOfImpacts )
			{ // 这个 ID 不存在
				return FALSE;
			}
		}

		return TRUE;
	}

	BOOL					operator!=( const _IMPACT_LIST& ImpactList ) const
	{
		return (*this == ImpactList) == FALSE;
	}
};

#endif // GAMESTRUCT_TEAM_H
