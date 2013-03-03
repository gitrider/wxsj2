/********************************************************************************
 *	�ļ�����	User.h
 *	ȫ·����	d:\Prj\World\World\WorldData\User.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2006 �� 4 �� 5 ��	15:56
 *
 *	����˵����	���������������
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef __WORLD_USER_H__
#define __WORLD_USER_H__

#include "Type.h"
#include "IDTable.h"

enum USER_STATUS
{
	US_NONE = 0 ,		//��
	US_HOLDUSERDATA ,	//ӵ�е�ǰ��ҵĵ�����Ϣ
	US_NORMAL ,			//���������Ϸ�У�һ������
	US_OFFLINE ,		//��Ҵ��ڶ���״̬��
	US_CRASH_DOWN,		//Server ���������
	US_WAIT_SHUTDOWN,	//Server �����˰�ȫͣ�������
	US_WAIT_KICK,		//�ȴ�����Server�����˵Ľ������

	US_SIZE,
};

enum SERVER_KICK_STATUS
{
	SK_NONE,			//��
	SK_KICK_SEND,		//�������󷢳�
	SK_KICK_OK,			//����OK
	SK_KICK_WAIT_SHM,	//���˵ȴ�ShareMemory���
	SK_KICK_NOUSR,		//��������û�д����
	SK_KICK_SRV_CRUSH,	//����������

	SK_SIZE,
};

struct SERVER_KICK
{
	PlayerID_t				m_ServerID;				//��Ӧ��������PlayerID
	BYTE					m_KickStatus;			//��Ӧ������������״̬

	SERVER_KICK()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		m_ServerID = INVALID_ID;
		m_KickStatus = SK_NONE;
	}
};

struct USER_GUILD_DATA
{
    enum GUILD_INVITE_TIME_LIMIT
    {
        TIME_LIMIT = 60*1000,
    };

	UINT					m_uAccess;					//Ȩ��
    BOOL                    m_bInvit;                   //�Ƿ�����
    GuildID_t               m_GuildId;                  //�������İ��ID
    INT                     m_iTime;                    //�յ������ʱ��

	USER_GUILD_DATA( )
	{
		CleanUp( );
	}
	VOID CleanUp( )
	{
        m_uAccess = 0;
        m_bInvit  = FALSE;
        m_GuildId = INVALID_GUILD_ID;
		m_iTime   = -1;
	}
};

struct _RELATION_USER
{
	GUID_t					m_UserGUID;
	RELATION_TYPE			m_RelationType;
	INT						m_nFriendPoint;				// Ŀǰ����ѺöȻἴʱ����
	CHAR					m_szGroupName[MAX_GROUP_NAME];	//��Ӧ����

	VOID					CleanUp()
	{
		m_UserGUID			= INVALID_ID;
		m_RelationType		= RELATION_TYPE_NONE;
		m_nFriendPoint		= 0;
		memset(m_szGroupName,0,sizeof(m_szGroupName));
	}
};

struct _RELATION_LIST
{
	INT						m_nCount;
	INT						m_nFriendCount;
	INT						m_nBlackCount;
	INT						m_nEnemyCount;
	_RELATION_USER			m_RelationUsers[MAX_RELATION_SIZE];

	VOID					CleanUp()
	{
		m_nCount			= 0;
		m_nFriendCount		= 0;
		m_nBlackCount		= 0;
		m_nEnemyCount		= 0;

		for( INT i=0; i<MAX_RELATION_SIZE; ++i )
		{
			m_RelationUsers[i].CleanUp();
		}
	}
};

struct _RELATION_DEL_POINT
{
	GUID_t					m_UserGUID;				// ��ɾ���˵�GUID
	INT						m_nFriendPoint;			// �뱻ɾ���˵ĺ��Ѷ�
	time_t					m_DelTime;				// ɾ��ʱ��ʱ��

	VOID					CleanUp()
	{
		m_UserGUID			= -1;
		m_nFriendPoint		= 0;
		m_DelTime			= 0;
	}
};

struct _RELATION_DEL_POINT_LIST
{
	INT						m_nCount;
	_RELATION_DEL_POINT		m_RelationDelPoint[MAX_FRIEND_SIZE];
	VOID					CleanUp()
	{
		m_nCount			= 0;

		for ( INT i=0; i<MAX_FRIEND_SIZE; ++i)
		{
			m_RelationDelPoint->CleanUp();
		}

	}
};

struct USER_RELATION_DATA
{
	Flag16					m_Flag;							//������Ϣ��־���� enum GAME_SETTING_FLAG
	CHAR					m_szMood[MOOD_DATA_SIZE];		// ����
	_RELATION_LIST			m_RelationList;					// ��ϵ������
	_RELATION_DEL_POINT_LIST m_RelationDelPointList;		// �Ѿ�ɾ���ĺ��Ѻ��Ѷȵݼ��б�

	IDTable					m_RelationGUIDTable;			// ���Ѻͺ��������ڴ˻���

	USER_RELATION_DATA()
	{
		CleanUp( );
		m_RelationGUIDTable.InitTable( MAX_RELATION_SIZE*2 );
	}
	VOID CleanUp( )
	{
		m_Flag.CleanUp( );
		memset( m_szMood, 0, sizeof(m_szMood) );
		m_RelationList.CleanUp();
		m_RelationGUIDTable.CleanUp();
		m_RelationDelPointList.CleanUp();
	}
};


struct FULLUSERDATA;
class USER
{
public:

	USER( )
	{
		m_UserID			= INVALID_ID;
		CleanUp( );
	}
	void CleanUp( )
	{
		m_UserStatus		= US_NONE;
		m_pFullUserData		= NULL;
		m_PlayerID			= INVALID_ID;
		m_TeamID			= INVALID_ID;
		m_SceneID			= INVALID_ID;
		m_ChannelID			= INVALID_ID;
		m_uKey				= 0;
		m_SimpleData.CleanUp( );
		m_GuildData.CleanUp( );
		m_RelationData.CleanUp( );
		m_KickTime			= 0;
		m_LastTime			= 0;
	}

	// ȡ����ҵ� UserID
	inline ID_t				GetUserID() const;

	// ������ҵ� UserID
	inline VOID				SetUserID( ID_t id );

	// ȡ�����״̬
	inline UINT				UserStatus() const;

	// �������״̬
	inline VOID				SetUserStatus( UINT uStatus );

	// ȡ�������������
	inline FULLUSERDATA*	GetFullUserData() const;

	// ���������������
	inline VOID				SetFullUserData( FULLUSERDATA* pData );

	// ȡ����ҵ� ServerID
	inline ID_t				GetServerID() const;

	// ȡ����� PlayerID
	inline PlayerID_t		GetPlayerID() const;

	// ������� PlayerID
	inline VOID				SetPlayerID( PlayerID_t pid );

	// ȡ����� GUID
	inline GUID_t			GetGUID() const;

	// ������� GUID
	inline VOID				SetGUID( GUID_t guid );

	// ȡ����� SceneID
	inline SceneID_t		GetSceneID() const;

	// ������� SceneID
	inline VOID				SetSceneID( SceneID_t SceneID );

	// �õ���ҳ�������
	inline const USER_SIMPLE_DATA* GetUserSimpleData() const;

	// ȡ���������
	inline const CHAR*		GetName() const;

	// �����������
	inline VOID				SetName( const CHAR* szName );

	// ȡ������Ա�
	inline UINT				GetSex() const;

	// ��������Ա�
	inline VOID				SetSex( UINT uSex );

	//ȡ�ý�ɫ���ڹ���
	inline INT				GetCountry() const;

	// ���ý�ɫ���ڵĹ���
	inline VOID				SetCountry(INT nCountry);

	// ȡ����ҵ����ɺ�
	inline INT				GetMenpai() const;

	// ������ҵ����ɺ�
	inline VOID				SetMenpai(INT nMenpai);

	// �õ���ҵȼ�
	inline INT				GetLevel() const;

	// ������ҵȼ�
	inline VOID				SetLevel(INT nLevel);

	// �õ���ҵ�ͷ��
	inline INT				GetPortrait() const;

	// �������ͷ��
	inline VOID				SetPortrait(INT nPortrait);

	// �õ�����ģ��
	inline UCHAR			GetFaceModel() const;

	// ��������ģ��
	inline VOID				SetFaceModel(UCHAR uFaceMeshID);

	// �õ�ͷ��ģ��
	inline UCHAR			GetHairModel() const;

	// ����ͷ��ģ��
	inline VOID				SetHairModel(UCHAR uHairMeshID);

	// �õ���ɫ
	inline UINT				GetHairColor() const;

	// ���÷�ɫ
	inline VOID				SetHairColor(UINT uHairColor);

	// ��������Money
	inline VOID				SetMoney(UINT Money);

	// �õ�����Money
	inline UINT				GetMoney() const;

	// ��������Money
	inline VOID				SetBankMoney(UINT Money);

	// �õ�����Money
	inline UINT				GetBankMoney() const;

	// ȡ����ҵĶ����
	inline TeamID_t			GetTeamID() const;

	// ������Ҷ����
	inline VOID				SetTeamID( TeamID_t tid );

	// ȡ����ҵĳƺ�
	inline const CHAR*		GetTitle() const;

	// ������ҵĳƺ�
	inline VOID				SetTitle(const CHAR* szTitle);

	// �õ���Ұ�� ID
	inline GuildID_t		GetGuildID() const;

	// ������Ұ�� ID
	inline VOID				SetGuildID(GuildID_t gid);

    // ������Ұ������
    inline VOID				SetGuildName(const CHAR *szGuildName);

    // �õ���Ұ������
    inline const CHAR*		GetGuildName() const;

    // ������Ҽ�������
    inline VOID				SetFamilyName(const CHAR *szFamilyName);

    // ���ð��������֤��Ϣ
    inline VOID             SetGuildInviteKey(const USER_GUILD_DATA &Key);

    // ���հ��������֤��Ϣ
    inline BOOL             CompareGuildInviteKey(const GuildID_t GuildId);

    // �жϴ�ǰ�Ƿ��ѱ�����
    inline BOOL             BeInGuildInvite(uint uTime);

    // ��հ��������֤��Ϣ
    inline VOID             CleanUpGuildInviteKey();

	// �õ���Ҵ���������Ƶ����
	inline ChannelID_t		GetChannelID() const;

	// ������Ҵ���������Ƶ����
	inline VOID				SetChannelID( ChannelID_t cid );

	// �õ��û���֤��
	inline UINT				GetKey() const;

	// �����û���֤��
	inline VOID				SetKey( UINT uKey );

	// ����������
	inline VOID				CleanUpGuildData();

	// �����ҹ�ϵ����
	inline VOID				CleanUpRelationData();

	// ����һ����ϵ��
	VOID					AddRelation( GUID_t guid, const _RELATION_USER* pUser );

	// �Ƴ�һ����ϵ��
	VOID					DelRelation( GUID_t guid );

	// �õ���ĳ��ҵĹ�ϵ״̬
	enum RELATION_TYPE		GetRelationType( GUID_t guid );

	// �ж� guid �Ƿ����ܺ���
	BOOL					IsGoodFriend(GUID_t guid);

	// ����һ�����ܺ���
	VOID					AddGoodFriend(GUID_t guid);

	// ȡ����ҵ�����
	inline const CHAR*		GetMood() const;

	// ��������
	inline VOID				SetMood( const CHAR* szMood );

	// ��������
	inline VOID				SetSettings(USHORT uSettings);

	// �Ƿ�ܾ�����Ϊ����
	inline BOOL				IsCannotAddFriend();

	// �Ƿ�ܾ��շ��ʼ�
	inline BOOL				IsCannotRecvMail();

	// �Ƿ�ܾ�İ���˵��ʼ�
	inline BOOL				IsCannotRecvStrangerMail();

	// �Ƿ�ܾ��������
	inline BOOL				IsRefuseTeamInvite();

	// �����������б�
	VOID					FillOnlineList( RETURN_ONLINE_LIST* pSend );

	// ֪ͨ����������Ϣ
	VOID					InformOnlineMsg();

	// ֪ͨ����������Ϣ
	VOID					InformOfflineMsg();

	// ������ҷ���ĳ�������ߵ���Ϣ
	VOID					SendRelationOnlineMsg(USER* pUser);

	// ������ҷ���ĳ�������ߵ���Ϣ
	VOID					SendRelationOfflineMsg(USER* pUser);

	// �����Ƿ���Է����ʼ���������ԵĻ����򷵻� 0�����򷵻ش���ţ�����Ųμ� enum RELATION_RETURN_TYPE
	UCHAR					ValidateMail(const MAIL* pMail);

	// �յ�һ���ʼ�
	BOOL					RecvMail(const MAIL* pMail);

	// ��������
	BOOL					HeartBeat( UINT uTime );

	// �����߳�ʱ��
	VOID					ResetKickTime( );
	VOID					ResetSMU_KickTime();

	// �����ҪKick�ķ������ͳ�ʼ״̬
	BOOL					AddKick(PlayerID_t serverId, BYTE initstatus);

	// �������Kick��Ϣ
	BOOL					ClearKick();

	// �޸�Kick��������״̬
	BOOL					ChangeKick(PlayerID_t serverId, BYTE changestatus);

	// �ж��Ƿ�Kick���
	BOOL					IsKickFinish();

	// ���Kick��Ϣ
	SERVER_KICK*			GetKick(INT idx);

	// ȡ����˺�
	inline const CHAR*		GetAccount()	const;

	// ��������˺�
	inline VOID				SetAccount(const CHAR* pAccount);

	// ����һ���ݼ����Ѷȵ�ǰ����,ֻ��guid�Ϳ������������ݴӺ����б�����,����ɾ�����ѵ�ʱ��
	time_t					AddRelationDelPoint( GUID_t guid, time_t nTime = 0);

	// ɾ��һ���ݼ����Ѷȵ�ǰ����
	VOID					DelRelationDelPoint( GUID_t guid );

	// �����б�ͨ��ʱ���������ĺ��Ѷȣ�ȥ�����Ѷ�<=0��ǰ����
	VOID					UpDataDelPointList();
	
	// ͨ��GUID��ü���֮��ĺ��Ѷ�
	INT						GetPoint( GUID_t guid );

	// ʵʱ���º��Ѷ�
	VOID					SetPoint( INT nPoint, GUID_t guid );
	

private:
	ID_t					m_UserID;			//������ҵ��û� ID, �������û��б�������±�
	UINT					m_UserStatus;		//������ҵ�״̬�� enum USER_STATUS
	FULLUSERDATA*			m_pFullUserData;	//������ҵ���������
	PlayerID_t				m_PlayerID;			//���û����ڳ�����PlayerID
	SceneID_t				m_SceneID;			//���û����ڳ���ID
	TeamID_t				m_TeamID;			//���û����ڵĶ���ID
	ChannelID_t				m_ChannelID;		//���û�����������Ƶ����
	UINT					m_uKey;				//�û���֤��
	USER_SIMPLE_DATA		m_SimpleData;		//���û��Ļ�����Ϣ
	USER_GUILD_DATA			m_GuildData;		//�����������
	USER_RELATION_DATA		m_RelationData;		//��ϵ���������
	INT						m_KickTime;			//û����Ϣʱ����߳�ʣ��ʱ��
	INT						m_SMU_KickTime;		//������ҵ������߳�ʣ��ʱ��
	UINT					m_LastTime;			//��һ�ε���ʱ��
    time_t                  m_LoginTime;        //����ʱ��

	SERVER_KICK				m_Kick[/*OVER_MAX_SERVER*/256];	//����SERVER������״̬
};

#include "User.inl"

#endif // __WORLD_USER_H__
