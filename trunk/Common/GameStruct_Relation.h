// ������Ϣ�ṹ
// fancy

#ifndef __GAMESTRUCT_RELATION_H__
#define __GAMESTRUCT_RELATION_H__

#include "Type.h"

class SocketInputStream;
class SocketOutputStream;

/////////////////////////////////////////////////////////////////////////////////

// ��ϵ������
struct _RELATION
{
	GUID_t								m_GUID;								// GUID
	UCHAR								m_uNameSize;
	CHAR								m_szName[MAX_CHARACTER_NAME];		// ����
	INT									m_nLevel;							// ��ɫ�ȼ�
	INT									m_nMenPai;							// ���� MENPAI_ATTRIBUTE
	INT									m_nPortrait;						// ͷ��
	GuildID_t							m_GuildID;							// ���ID�����ڷ�������ˣ�
	UCHAR								m_uGuildNameSize;					// 
	CHAR								m_szGuildName[MAX_GUILD_NAME_SIZE];	// ������ƣ����� Server �����ͻ��ˣ�
	UCHAR								m_uOnlineFlag;						// �Ƿ����ߣ����²���ֻ�����߲ŷ����ͻ��ˣ�
	UCHAR								m_uMoodSize;						// ���鳤��
	CHAR								m_szMood[MOOD_DATA_SIZE];			// ����
	UCHAR								m_uTitleSize;						// �ƺ�����
	CHAR								m_szTitle[MAX_CHARACTER_TITLE];		// �ƺ�
	SceneID_t							m_SceneID;							// ���ڳ��������²����� Server ���ã�
	UCHAR								m_uTeamSize;						// ����������0 ��ʾδ��ӣ�
	UINT								m_point;							// ���Ѷ�

	VOID								CleanUp();
	UINT								GetSize() const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	UINT								GetPoint(){ return m_point; }
	VOID								SetPoint( UINT point ){ m_point = point; }

	GUID_t								GetGUID( ) { return m_GUID; }
	VOID								SetGUID( GUID_t guid ) { m_GUID = guid; }

	const CHAR*							GetName( ) { return m_szName; }
	VOID								SetName( const CHAR* pName )
	{
		strncpy( m_szName, pName, MAX_CHARACTER_NAME-1 );
		m_uNameSize = (UCHAR)strlen(m_szName);
	}

	INT									GetLevel( ) { return m_nLevel; }
	VOID								SetLevel( INT level ) { m_nLevel = level; }

	INT									GetMenPai( ) { return m_nMenPai; }
	VOID								SetMenPai( INT menpai ) { m_nMenPai = menpai; }

	INT									GetPortrait( ) { return m_nPortrait; }
	VOID								SetPortrait( INT nPortrait ) { m_nPortrait = nPortrait; }

	GuildID_t							GetGuildID( ) { return m_GuildID; }
	VOID								SetGuildID( GuildID_t gid ) { m_GuildID = gid; }

	const CHAR*							GetGuildName( ) { return m_szGuildName; }
	VOID								SetGuildName( const CHAR* pName )
	{
		strncpy( m_szGuildName, pName, MAX_GUILD_NAME_SIZE-1 );
		m_uGuildNameSize = (UCHAR)strlen(m_szGuildName);
	}

	UCHAR								GetOnlineFlag( ) { return m_uOnlineFlag; }
	VOID								SetOnlineFlag( UCHAR flag ) { m_uOnlineFlag = flag; }

	const CHAR*							GetMood( ) { return m_szMood; }
	VOID								SetMood( const CHAR* pMood )
	{
		strncpy( m_szMood, pMood, MOOD_DATA_SIZE-1 );
		m_uMoodSize = (UCHAR)strlen(m_szMood);
	}

	const CHAR*							GetTitle( ) { return m_szTitle; }
	VOID								SetTitle( const CHAR* pTitle )
	{
		strncpy( m_szTitle, pTitle, MAX_CHARACTER_TITLE-1 );
		m_uTitleSize = (UCHAR)strlen(m_szTitle);
	}

	SceneID_t							GetSceneID( ) { return m_SceneID; }
	VOID								SetSceneID( SceneID_t sid ) { m_SceneID = sid; }

	UCHAR								GetTeamSize( ) { return m_uTeamSize; }
	VOID								SetTeamSize( UCHAR TeamSize ) { m_uTeamSize = TeamSize; }
};

enum RELATION_REQUEST_TYPE
{
	REQ_NONE							= 0,
	REQ_RELATIONLIST,
	REQ_RELATIONINFO,
	REQ_VIEWPLAYER,										// �鿴���
	REQ_ADDFRIEND,
	REQ_ADDTOBLACKLIST,									// ��ҵ�������
	REQ_ADDTOENEMY,										// ��ӵ�����
	REQ_TEMPFRIEND_TO_FRIEND,
	REQ_TEMPFRIEND_ADDTO_BLACKLIST,
	REQ_TEMPFRIEND_ADDTO_ENEMY,							// ת�Ƶ�����
	REQ_TRANSITION,
	REQ_DELFRIEND,										// ɾ������
	REQ_DELFROMBLACKLIST,								// ɾ��������
	REQ_DELFROMENEMY,									// ɾ������
	REQ_NEWGOODFRIEND,									// ����һ�����ܺ���
	REQ_RELATIONONLINE,									// ������������б�
	REQ_MODIFYMOOD,										// �޸��Լ�������
	REQ_MODIFYSETTINGS,									// �޸���ϵ������
	REQ_NOTIFY_ADDTEMPFRIEND,							// ֪ͨ�Է�����Ϊ��ʱ����
	REQ_ADD_NEW_GROUP,									// ���󴴽�����
	REQ_DEL_GROUP,										// ����ɾ����
	REQ_SUREADDFRIEND,									// ȷ�ϼ�Ϊ����
	REQ_REJECTADDFRIEND,								// �ܾ���Ϊ��������
	REQ_DELFRIENDBYMAIL,								// ɾ�����ѣ�ͨ����ִ���ʼ�
	REQ_FRIENDADDPOINT,									// ��word���º��Ѷ���Ϣ
};

enum RELATION_RETURN_TYPE
{
	RET_NONE							= 0,
	RET_RELATIONLIST,
	RET_RELATIONINFO,
	RET_VIEWPLAYER,										// �鿴���
	RET_TARGETNOTONLINE,								// Ŀ�겻���ߣ������� World ѯ��ĳ��ϵ�˵���Ϣû���ҵ�ʱ�ķ�����
	RET_ADDFRIEND,
	RET_ADDTOBLACKLIST,
	RET_ADDENEMY,										// ֪ͨ��ҶԷ��Ѿ������뵽������
	RET_TEMPFRIEND_TO_FRIEND,
	RET_TEMPFRIEND_ADDTO_BLACKLIST,
	RET_TRANSITION,
	RET_DELFRIEND,
	RET_DELFROMBLACKLIST,								// ֪ͨɾ��������
	RET_DELFROMENEMY,									// ͬ־ɾ������
	RET_ADDFRIENDNOTIFY,								// ֪ͨ�����Ѿ�������
	RET_ASKADDFRIEND,									// ֪ͨ����Ƿ�ͬ�ⱻ����Ϊ����
	RET_ASKADDTOBLACKLIST,								// ֪ͨ����Ѿ��ɹ����Է��ƶ���������
	RET_ASKTHROWTOFRIENDLIST,							// ֪ͨ����Ѿ��ɹ����Է��ƶ��������б�
	RET_ONLINELIST,										// ���ߺ����б�
	RET_RELATIONONLINE,									// ��������
	RET_RELATIONOFFLINE,								// ��������
	RET_NEWMOOD,										// �µ�����
	RET_NOTIFY_ADDTEMPFRIEND,							// ֪ͨ�Է�����Ϊ��ʱ����

	RET_NEW_GROUP,										//	�������
	RET_REJECTADDFRIEND,								// ֪ͨ�����˾ܾ����ܼ�Ϊ��������
	RET_FRIENDADDPOINT,									// ֪ͨ���Ӻ��Ѷ�
	RET_DELFRIENDBYMAIL,								// ͨ����ִ���ʼ�ɾ������
	RET_DELFRIENDFORADDENEMY,							// �ڼ�Ϊ����ʱ��֪ͨserver�Ϳͻ���ɾ������

	RET_ERR_START,
	RET_ERR_TARGETNOTEXIST,								// Ŀ�겻���ڻ�����
	RET_ERR_GROUPISFULL,								// ������
	RET_ERR_ISFRIEND,									// �Ѿ��Ǻ���
	RET_ERR_ISBLACKNAME,								// �Ѿ������������
	RET_ERR_CANNOTTRANSITION,							// ����ת��
	RET_ERR_ISNOTFRIEND,								// ���Ǻ���
	RET_ERR_ISNOTINBLACKLIST,							// ���ں�����
	RET_ERR_SPOUSETOBLACKLIST,							// ����ż���������
	RET_ERR_MASTERTOBLACKLIST,							// ��ʦ�����������
	RET_ERR_PRENTICETOBLACKLIST,						// ��ͽ�ܼ��������
	RET_ERR_BROTHERTOBLACKLIST,							// ������ֵܼ��������
	RET_ERR_DELSPOUSE,									// ɾ����ż
	RET_ERR_DELMASTER,									// ɾ��ʦ��
	RET_ERR_DELPRENTICE,								// ɾ��ͽ��
	RET_ERR_DELBROTHER,									// ɾ�������ֵ�
	RET_ERR_PASSWDMISMATCH,								// ���벻ƥ��
	RET_ERR_CANNOT_ADDFRIEND,							// �ܾ���Ϊ����
	RET_ERR_CANNOTRECEIVEMAIL,							// �ܾ������κ��ʼ�
	RET_ERR_NOTRECVSTRANGEMAIL,							// ����İ�����ʼ�
	RET_ERR_FRIENDGROUPISFULL,							// �������Ѿ���Ա			
	RET_ERR_BLACKGROUPISFULL,							// ���������Ѿ���Ա

	RET_ERR_RELATIONUNKNOWN,							// δ֪����
};

// ����Ϊ GUID
struct RELATION_GUID
{
	//����
	GUID_t								m_TargetGUID;		// ĳ��ҵ� GUID

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	GUID_t								GetTargetGUID( ) { return m_TargetGUID; }
	VOID								SetTargetGUID( GUID_t guid ) { m_TargetGUID = guid; }
};

struct REQUEST_DEL_FRIEND_MAIL
{
	GUID_t								m_GUID;
	UINT								m_TIME;								// ʱ��

	VOID								CleanUp();
	UINT								GetSize() const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	// Ӧ�ýӿ�
	GUID_t								GetGUID() { return m_GUID; }
	VOID								SetGUID(GUID_t guid) { m_GUID = guid; }

	UINT								GetTIME() { return m_TIME; }
	VOID								SetTIME( UINT nTime ) { m_TIME = nTime; }
};


// ֪ͨ
struct RETURN_NOTIFY_FRIEND
{
	GUID_t								m_GUID;								// GUID
	UCHAR								m_uNameSize;
	CHAR								m_szName[MAX_CHARACTER_NAME];		// ����

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	GUID_t								GetGUID( ) { return m_GUID; }
	VOID								SetGUID( GUID_t guid ) { m_GUID = guid; }

	const CHAR*							GetName( ) { return m_szName; }
	VOID								SetName( const CHAR* pName )
	{
		strncpy( m_szName, pName, MAX_CHARACTER_NAME-1 );
		m_uNameSize = (UCHAR)strlen(m_szName);
	}
};

// ɾ������
struct REQUEST_DEL_FRIEND : public RETURN_NOTIFY_FRIEND
{
	//����

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
};

//���һ������
struct REQUEST_ADD_NEW_GROUP
{
	//����
	UCHAR								m_uNameSize;						// ��������
	CHAR								m_szGroupName[MAX_GROUP_NAME];		// ����
	INT									m_uGroupID;							// ��ID

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	const CHAR*							GetGroupName( ) { return m_szGroupName; }
	VOID								SetGroupName( const CHAR* GroupName )
	{
		if( GroupName == NULL || strlen(GroupName) > MAX_GROUP_NAME )
		{
			Assert( GroupName );
			Assert( strlen(GroupName) <= MAX_GROUP_NAME );
			return;
		}

		strncpy( m_szGroupName, GroupName, MOOD_DATA_SIZE-1 );
		m_uNameSize = (UCHAR)strlen(m_szGroupName);
	}
};


// �޸�����
struct REQUEST_MODIFY_MOOD
{
	//����
	UCHAR								m_uMoodSize;						// ���鳤��
	CHAR								m_szMood[MOOD_DATA_SIZE];			// ����

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	const CHAR*							GetMood( ) { return m_szMood; }
	VOID								SetMood( const CHAR* pMood )
	{
		if( pMood == NULL || strlen(pMood) > MAX_MOOD_SIZE )
		{
			Assert( pMood );
			Assert( strlen(pMood) <= MAX_MOOD_SIZE );
			return;
		}

		strncpy( m_szMood, pMood, MOOD_DATA_SIZE-1 );
		m_uMoodSize = (UCHAR)strlen(m_szMood);
	}
};

// ��ѯ��ϵ����ϸ��Ϣ
struct REQUEST_RELATION_INFO
{
	//����
	GUID_t								m_TargetGUID;		// ĳ��ҵ� GUID
	UCHAR								m_uNameSize;
	CHAR								m_szTargetName[MAX_CHARACTER_NAME];

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	GUID_t								GetTargetGUID( ) { return m_TargetGUID; }
	VOID								SetTargetGUID( GUID_t guid ) { m_TargetGUID = guid; }

	VOID								SetTargetName( const CHAR* pName )
	{
		strncpy( m_szTargetName, pName, MAX_CHARACTER_NAME-1 );
		m_uNameSize = (UCHAR)strlen(m_szTargetName);
	}
	const CHAR*							GetTargetName( ) { return m_szTargetName; }
};

// �ҽ��鿴���
struct REQUEST_VIEW_PLAYER
{
	//����
	UCHAR								m_uNameSize;
	CHAR								m_szTargetName[MAX_CHARACTER_NAME];

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	VOID								SetTargetName( const CHAR* pName )
	{
		strncpy( m_szTargetName, pName, MAX_CHARACTER_NAME-1 );
		m_uNameSize = (UCHAR)strlen(m_szTargetName);
	}
	const CHAR*							GetTargetName( ) { return m_szTargetName; }
};

// ����һ����ϵ��
struct REQUEST_ADD_RELATION
{
	//����
	GUID_t								m_TargetGUID;		// ĳ��ҵ� GUID
	UCHAR								m_uNameSize;
	CHAR								m_szTargetName[MAX_CHARACTER_NAME];
	UCHAR								m_uRelationType;	// ��ϵ����

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	GUID_t								GetTargetGUID( ) { return m_TargetGUID; }
	VOID								SetTargetGUID( GUID_t guid ) { m_TargetGUID = guid; }

	VOID								SetTargetName( const CHAR* pName )
	{
		strncpy( m_szTargetName, pName, MAX_CHARACTER_NAME-1 );
		m_uNameSize = (UCHAR)strlen(m_szTargetName);
	}
	const CHAR*							GetTargetName( ) { return m_szTargetName; }

	UCHAR								GetRelationType( ) { return m_uRelationType; }
	VOID								SetRelationType( UCHAR uRelationType ) { m_uRelationType = uRelationType; }
};

// ����һ����ϵ�ˣ����Ҵ������
struct REQUEST_ADD_RELATION_WITH_GROUP : public REQUEST_ADD_RELATION
{
	//����
	UCHAR								m_uGroup;			// ��

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	UCHAR								GetGroup( ) { return m_uGroup; }
	VOID								SetGroup( UCHAR uGroup ) { m_uGroup = uGroup; }
};

// ����Ϊ GUID �� UCHAR��UCHAR
struct RELATION_GUID_UCHAR_UCHAR
{
	//����
	GUID_t								m_TargetGUID;		// ĳ��ҵ� GUID
	UCHAR								m_uRelationType;	// ��ϵ����
	UCHAR								m_uGroup;			// ��

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	GUID_t								GetTargetGUID( ) { return m_TargetGUID; }
	VOID								SetTargetGUID( GUID_t guid ) { m_TargetGUID = guid; }

	UCHAR								GetRelationType( ) { return m_uRelationType; }
	VOID								SetRelationType( UCHAR uRelationType ) { m_uRelationType = uRelationType; }

	UCHAR								GetGroup( ) { return m_uGroup; }
	VOID								SetGroup( UCHAR uGroup ) { m_uGroup = uGroup; }
};

struct CG_RELATION
{
	UCHAR								m_Type;			//enum CG_TYPE

	union
	{
		RELATION_GUID					m_RelationGUID;
		REQUEST_RELATION_INFO			m_RequestInfo;
		REQUEST_VIEW_PLAYER				m_ViewPlayer;
		REQUEST_ADD_RELATION			m_AddRelation;
		REQUEST_ADD_RELATION_WITH_GROUP	m_AddRelationWithGroup;
		RELATION_GUID_UCHAR_UCHAR		m_RelationGUIDUCHARUCHAR;
		REQUEST_DEL_FRIEND				m_DelFriend;
		REQUEST_MODIFY_MOOD				m_ModifyMood;
		REQUEST_ADD_NEW_GROUP			m_AddNewGroup;
		
	};

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

};

/////////////////////////////////////////////////////////////////////////////////

// ����Ϊ GUID �� UCHAR
struct RELATION_GUID_UCHAR
{
	//����
	GUID_t								m_TargetGUID;		// ĳ��ҵ� GUID
	UCHAR								m_uRelationType;	// ��ϵ����

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	GUID_t								GetTargetGUID( ) { return m_TargetGUID; }
	VOID								SetTargetGUID( GUID_t guid ) { m_TargetGUID = guid; }

	UCHAR								GetRelationType( ) { return m_uRelationType; }
	VOID								SetRelationType( UCHAR uRelationType ) { m_uRelationType = uRelationType; }
};

// �޸���ϵ������
struct REQUEST_MODIFY_SETTINGS
{
	//����
	USHORT								m_uSettings;	// ��ϵ����

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	USHORT								GetSettings( ) { return m_uSettings; }
	VOID								SetSettings( UCHAR uSettings ) { m_uSettings = uSettings; }
};

//////////////////////////////////////////////////////////////////////////

struct GW_RELATION
{
	UCHAR								m_Type;
	GUID_t								m_GUID;		// �Լ��� GUID

	union
	{
		RELATION_GUID					m_RelationGUID;
		REQUEST_RELATION_INFO			m_RequestInfo;
		REQUEST_VIEW_PLAYER				m_ViewPlayer;
		REQUEST_ADD_RELATION_WITH_GROUP	m_AddRelationWithGroup;
		REQUEST_ADD_RELATION			m_AddRelation;
		RELATION_GUID_UCHAR				m_TransitionRelation;
		REQUEST_MODIFY_SETTINGS			m_Settings;
		REQUEST_MODIFY_MOOD				m_ModifyMood;
		REQUEST_ADD_NEW_GROUP			m_AddNewGroup;
		REQUEST_DEL_FRIEND				m_DelFriend;
		REQUEST_DEL_FRIEND_MAIL			m_DelFriendByMail;
	};

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	GUID_t								GetGUID( ) { return m_GUID; }
	VOID								SetGUID( GUID_t guid ) { m_GUID = guid; }
};

/////////////////////////////////////////////////////////////////////////////////

struct RETURN_ADD_RELATION
{
	UCHAR								m_uRelationType;	// ��ϵ����
	UCHAR								m_uGroup;			// ��

	_RELATION							m_Relation;			// ��ϵ����ϸ��Ϣ���������ɹ���Я��

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	UCHAR								GetRelationType( ) { return m_uRelationType; }
	VOID								SetRelationType( UCHAR uRelationType ) { m_uRelationType = uRelationType; }

	UCHAR								GetGroup( ) { return m_uGroup; }
	VOID								SetGroup( UCHAR uGroup ) { m_uGroup = uGroup; }

	_RELATION*							GetRelationData( ) { return &m_Relation; }
	VOID								SetRelationData( const _RELATION* pRelationData )
	{
		Assert( pRelationData );
		memcpy( (void*)&m_Relation, pRelationData, sizeof(m_Relation) );
	}
};

struct RETURN_RELATION_INFO
{
	UCHAR								m_uRelationType;	// ��ϵ����
	UCHAR								m_uGroup;			// ��
	INT									m_nFriendpoint;		// ��ϵֵ

	_RELATION							m_Relation;			// ��ϵ����ϸ��Ϣ

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	UCHAR								GetRelationType( ) { return m_uRelationType; }
	VOID								SetRelationType( UCHAR uRelationType ) { m_uRelationType = uRelationType; }

	UCHAR								GetGroup( ) { return m_uGroup; }
	VOID								SetGroup( UCHAR uGroup ) { m_uGroup = uGroup; }

	INT									GetFriendPoint( ) { return m_nFriendpoint; }
	VOID								SetFriendPoint( INT nFp ) { m_nFriendpoint = nFp; }

	_RELATION*							GetRelationData( ) { return &m_Relation; }
	VOID								SetRelationData( const _RELATION* pRelationData )
	{
		Assert( pRelationData );
		memcpy( (void*)&m_Relation, pRelationData, sizeof(m_Relation) );
	}
};

// �Ҽ��鿴�����Ϣ
struct RETURN_VIEW_PLAYER
{
	GUID_t								m_GUID;								// GUID
	UCHAR								m_uNameSize;
	CHAR								m_szName[MAX_CHARACTER_NAME];		// 1.����
	//UCHAR								m_uTitleSize;
	//CHAR								m_szTitle[MAX_CHARACTER_TITLE];		// 2.�ƺ�
	//INT									m_nLevel;							// 3.��ɫ�ȼ�
	//INT									m_nMenPai;							// 4.���� MENPAI_ATTRIBUTE
	//UCHAR								m_uGuildNameSize;
	//CHAR								m_szGuildName[MAX_GUILD_NAME_SIZE];	// 5.������ƣ����� Server �����ͻ��ˣ�
	//UCHAR								m_uSpouseNameSize;
	//CHAR								m_szSpouseName[MAX_CHARACTER_NAME];	// 6.��ż����
	//INT									m_nModelID;							// 7.ģ��
	//UINT								m_uHairColor;						// 8.ͷ����ɫ

	//UINT								m_WeaponID;							// 9.����
	//UINT								m_CapID;							// 10.ñ��
	//UINT								m_ArmourID;							// 11.�·�
	//UINT								m_CuffID;							// 12.����
	//UINT								m_FootID;							// 13.ѥ��

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	VOID								FillData( const RETURN_VIEW_PLAYER* pViewPlayer )
	{
		Assert( pViewPlayer );
		memcpy( (void*)this, pViewPlayer, sizeof(*this) );
	}

	GUID_t								GetGUID( ) { return m_GUID; }
	VOID								SetGUID( GUID_t guid ) { m_GUID = guid; }

	const CHAR*							GetName( ) { return m_szName; }
	VOID								SetName( const CHAR* pName )
	{
		strncpy( m_szName, pName, MAX_CHARACTER_NAME-1 );
		m_uNameSize = (UCHAR)strlen(m_szName);
	}
};

struct _RELATION_ONLINE
{
	GUID_t								m_GUID;								// ���ߵ����ܺ���
	UCHAR								m_uMoodSize;						// ���鳤��
	CHAR								m_szMood[MOOD_DATA_SIZE];			// ����

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	GUID_t								GetGUID( ) const { return m_GUID; }
	VOID								SetGUID( GUID_t guid ) { m_GUID = guid; }

	const CHAR*							GetMood( ) const { return m_szMood; }
	VOID								SetMood( const CHAR* pMood )
	{
		strncpy( m_szMood, pMood, MOOD_DATA_SIZE-1 );
		m_uMoodSize = (UCHAR)strlen(m_szMood);
	}
};

// ���ڸ�������������б�
struct RETURN_ONLINE_LIST
{
	UCHAR								m_uOnlineCount;						// ��������
	_RELATION_ONLINE					m_OnlineInfo[RELATION_BLACKNAME_OFFSET-RELATION_FRIEND_OFFSET];

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	UCHAR								GetOnlineCount() { return m_uOnlineCount; }

	const _RELATION_ONLINE*				GetOnlineRelation( UCHAR uIndex )
	{
		if( uIndex >= m_uOnlineCount )
		{
			Assert(uIndex < m_uOnlineCount);
			return NULL;
		}

		return &(m_OnlineInfo[uIndex]);
	}
	VOID								AddOnlineRelation( const _RELATION_ONLINE* pOnlineInfo )
	{
		if( m_uOnlineCount >= RELATION_BLACKNAME_OFFSET-RELATION_FRIEND_OFFSET )
		{
			Assert( m_uOnlineCount < RELATION_BLACKNAME_OFFSET-RELATION_FRIEND_OFFSET );
			return;
		}

		m_OnlineInfo[m_uOnlineCount++] = *pOnlineInfo;
	}
};

// ����֪ͨ����������
struct RETURN_NOTIFY_ONLINE
{
	UCHAR								m_uNameSize;
	CHAR								m_szTargetName[MAX_CHARACTER_NAME];
	UCHAR								m_uMoodSize;						// ���鳤��
	CHAR								m_szMood[MOOD_DATA_SIZE];			// ����

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	const CHAR*							GetTargetName( ) { return m_szTargetName; }
	VOID								SetTargetName( const CHAR* pName )
	{
		strncpy( m_szTargetName, pName, MAX_CHARACTER_NAME-1 );
		m_uNameSize = (UCHAR)strlen(m_szTargetName);
	}

	const CHAR*							GetMood( ) { return m_szMood; }
	VOID								SetMood( const CHAR* pMood )
	{
		strncpy( m_szMood, pMood, MOOD_DATA_SIZE-1 );
		m_uMoodSize = (UCHAR)strlen(m_szMood);
	}
};

struct WG_RELATION
{
	UCHAR								m_Type;
	PlayerID_t							m_PlayerID;			// ���˵� PlayerID

	union
	{
		RETURN_ADD_RELATION				m_AddRelation;
		RETURN_RELATION_INFO			m_RelationInfo;
		RETURN_VIEW_PLAYER				m_ViewPlayer;
		RELATION_GUID					m_RelationGUID;
		RETURN_NOTIFY_FRIEND			m_NotifyFriend;
		RETURN_ONLINE_LIST				m_RelationOnline;
		RETURN_NOTIFY_ONLINE			m_NotifyOnline;
		REQUEST_VIEW_PLAYER				m_PlayerName;
		REQUEST_ADD_NEW_GROUP			m_AddNewGroup;
	};

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	PlayerID_t							GetPlayerID( ) { return m_PlayerID; }
	VOID								SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
};

/////////////////////////////////////////////////////////////////////////////////

struct _FRIEND_INFO
{
	GUID_t								m_GUID;				// GUID
	UCHAR								m_uNameSize;
	CHAR								m_szTargetName[MAX_CHARACTER_NAME];
	UCHAR								m_uRelationType;	// ��ϵ����
	UCHAR								m_uGroup;			// ��
	
	// �Զ����������
	CHAR								m_szGroupName[MAX_GROUP_NAME];	

	INT									m_nFriendpoint;		// ��ϵֵ
	VOID								CleanUp()
	{
		m_GUID							= INVALID_ID;
		m_uNameSize						= 0;
		memset((void*)m_szTargetName, 0, sizeof(m_szTargetName));
		m_uGroup						= 0;
		memset((void*)m_szGroupName, 0, sizeof(m_szGroupName));
		m_nFriendpoint					= 0;
	}
	UINT								GetSize() const
	{
		UINT							uSize;
		uSize							= sizeof(m_GUID) + sizeof(m_uNameSize) + m_uNameSize
										+ sizeof(m_uRelationType) + sizeof(m_uGroup) 
										+ sizeof(m_szGroupName) + sizeof(m_nFriendpoint);
		return uSize;
	}
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;
};

struct _BLACKNAME_INFO
{
	GUID_t								m_GUID;				// GUID
	UCHAR								m_uNameSize;
	CHAR								m_szTargetName[MAX_CHARACTER_NAME];
	VOID								CleanUp()
	{
		m_GUID							= INVALID_ID;
		m_uNameSize						= 0;
		memset((void*)m_szTargetName, 0, sizeof(m_szTargetName));
	}
	UINT								GetSize() const
	{
		UINT							uSize;
		uSize							= sizeof(m_GUID) + sizeof(m_uNameSize) + m_uNameSize;
		return uSize;
	}
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;
};

// ������Ϣ
struct _ENEMY_INFO
{
	GUID_t								m_GUID;				// GUID
	UCHAR								m_uNameSize;
	CHAR								m_szEnemyName[MAX_CHARACTER_NAME];

	VOID								CleanUp()
	{
		m_GUID							= INVALID_ID;
		m_uNameSize						= 0;
		memset((void*)m_szEnemyName, 0, sizeof(m_szEnemyName));
	}

	UINT								GetSize() const
	{
		UINT							uSize;
		uSize							= sizeof(m_GUID) + sizeof(m_uNameSize) + m_uNameSize;
		return uSize;
	}

	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;
};


struct _OWN_RELATION;

// �����б��Լ�������
struct GC_RELATIONLIST
{
	//����
	UCHAR								m_uFriendCount;				// ������
	UCHAR								m_uBlackCount;				// ��������
	UCHAR								m_uEnemyCount;				//������
	UCHAR								m_uMoodSize;
	CHAR								m_szMood[MOOD_DATA_SIZE];	// �Լ�������
	_FRIEND_INFO						m_FriendInfo[RELATION_BLACKNAME_OFFSET - RELATION_FRIEND_OFFSET];
	_BLACKNAME_INFO						m_BlackNameInfo[RELATION_ENEMY_OFFSET - RELATION_BLACKNAME_OFFSET];
	_ENEMY_INFO							m_EnemyInfo[MAX_ENEMY_SIZE];

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	UCHAR								GetFriendCount( ) { return m_uFriendCount; }

	UCHAR								GetBlackCount( ) { return m_uBlackCount; }
	UCHAR								GetEnemyCount( ) { return m_uEnemyCount; }

	const CHAR*							GetMood( ) { return m_szMood; }
	VOID								SetMood( const CHAR* pMood )
	{
		strncpy( m_szMood, pMood, MOOD_DATA_SIZE-1 );
		m_uMoodSize = (UCHAR)strlen(m_szMood);
	}

	VOID								AddFriend( const _OWN_RELATION* pRelation );
	const _FRIEND_INFO*					GetFriend( INT index );

	VOID								AddBlackName( const _OWN_RELATION* pRelation );
	const _BLACKNAME_INFO*				GetBlackName( INT index );

	VOID								AddEnemyName( const _OWN_RELATION* pRelation );
	const _ENEMY_INFO*					GetEnemyName( INT index );
};

struct GC_RELATION
{
	UCHAR								m_Type;

	union
	{
		GC_RELATIONLIST					m_RelationList;
		RETURN_ADD_RELATION				m_NewRelation;
		RETURN_RELATION_INFO			m_RelationInfo;
		RETURN_VIEW_PLAYER				m_ViewPlayer;
		RELATION_GUID_UCHAR_UCHAR		m_RelationGUIDUCHARUCHAR;
		RELATION_GUID					m_RelationGUID;
		RETURN_NOTIFY_FRIEND			m_NotifyFriend;
		RETURN_ONLINE_LIST				m_RelationOnline;
		RETURN_NOTIFY_ONLINE			m_NotifyOnline;
		REQUEST_MODIFY_MOOD				m_NewMood;
		REQUEST_VIEW_PLAYER				m_PlayerName;
		REQUEST_ADD_NEW_GROUP			m_AddNewGroup;
	};

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;
};

//////////////////////////////////////////////////////////////////////////

#endif
