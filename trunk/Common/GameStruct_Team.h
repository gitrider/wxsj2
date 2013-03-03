/********************************************************************************
 *	�ļ�����	GameStruct_Team.h
 *	ȫ·����	d:\Prj\Common\GameStruct_Team.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2006 �� 4 �� 24 ��	18:14
 *
 *	����˵����	�򵥵� Buff �б���������ͷ���µ� Buff ͼ����ʾ
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef GAMESTRUCT_TEAM_H
#define GAMESTRUCT_TEAM_H

#include "DB_Struct.h"
#include "PacketFactory.h"

//�ӳ����Ա����������Ϣ���õ������ݽṹ

//������Գ�������
#define TEAM_ENOUNCE_LEN 12

//ÿһ��������෵��50����¼
#define	ONCE_RECRUIT_LIST_LEN	50

//��Ϣ����
enum RECRUIT_TYPE
{
	LEADER_RECRUIT = 0,		//�ӳ��������ն�Ա��Ϣ
	MEMBER_RECRUIT ,		//��Ա�������ն�Ա��Ϣ
};

//���Ŀ��
enum TEAME_TARGET
{
	EXERCISE_LEVEL = 0,		//�Ƿ�����
	DO_MISSION,				//�Ƿ�������
	BOUT,					//�Ƿ��д�
	KILL_BOSS,				//�Ƿ��BOSS
	KILL_ENEMY,				//�Ƿ�й�ɱ��
	HANG,					//�Ƿ�һ�
	TARGET_NUMBER,
};


//�ӳ���������
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

	INT					m_MinLevel;				//ƥ�����С�ȼ�
	INT					m_MaxLevel;				//ƥ������ȼ�

	//ƥ����Щ����
	BOOL				m_IsInCountry[COUATTRIBUTE_NUMBER];			

	//���Ŀ��
	BOOL				m_IsTarget[TARGET_NUMBER];		

	//ְҵ��Χ
	BOOL				m_IsMenPaiID[MATTRIBUTE_NUMBER];

	//�������
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


//��Ա��������
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

	INT					m_MinLevel;				//ƥ�����С�ȼ�
	INT					m_MaxLevel;				//ƥ������ȼ�

	//ƥ����Щ����
	BOOL				m_IsInCountry[COUATTRIBUTE_NUMBER];			

	//���Ŀ��
	BOOL				m_IsTarget[TARGET_NUMBER];		

	//�������
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

//��������Ϣ
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
	UCHAR	m_Country;		//����
};


//���״̬
enum NODE_STATE
{
	NODE_NULL = 0 ,		//��
	NODE_USING,			//ʹ����
	STATE_NUM,
};


//�ӳ�����ʱ���浽�����������е���Ϣ����Ҳ�ѯ�������ʱ���ڹ������в���
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

//��Ա����ʱ���浽�����������е���Ϣ����Ҳ�ѯ�������ʱ���ڹ������в���
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



//�������õ�������������ظ�����ҵ���Ϣ�ṹ
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
	UCHAR	m_Country;		//����
	BOOL	m_TeamTarget[TARGET_NUMBER];	//���Ŀ��

	//�������
	CHAR				m_szTeamEnounce[TEAM_ENOUNCE_LEN];
};


class SimpleImpactList
{
	INT						m_nNumOfImpacts;				// BUFF ����
	ImpactID_t				m_aBuffID[MAX_IMPACT_NUM];		// BUFF �б�

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
		{ // �����Ƿ� Buff �б��Ѿ��ı䣬ÿ����ǰ ID ��ȥ Cache �бȽ�һ���Ƿ����
			INT j;
			for( j=0; j<m_nNumOfImpacts; ++j )
			{
				if( ImpactList.m_aImpacts[i].GetImpactID() == m_aBuffID[j] )
				{
					break;
				}
			}

			if ( j >= m_nNumOfImpacts )
			{ // ��� ID ������
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
