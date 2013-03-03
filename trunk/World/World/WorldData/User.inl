/********************************************************************************
 *	�ļ�����	User.inl
 *	ȫ·����	d:\Prj\World\World\WorldData\User.inl
 *	�����ˣ�	����
 *	����ʱ�䣺	2006 �� 3 �� 3 ��	12:18
 *
 *	����˵����	һЩ USER �� inline ����
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef __USER_INL__
#define __USER_INL__

#include "Config.h"

// ȡ����ҵ� UserID
ID_t USER::GetUserID() const
{
	return m_UserID;
}

// ������ҵ� UserID
VOID USER::SetUserID( ID_t id )
{
	m_UserID = id;
}

// ȡ�����״̬
UINT USER::UserStatus() const
{
	return m_UserStatus;
};

// �������״̬
VOID USER::SetUserStatus( UINT uStatus )
{
	if( uStatus > US_NONE && uStatus < US_SIZE )
	{
		m_UserStatus = uStatus;
	}
	else
	{
		Assert( FALSE && "uStatus Exception!" );
	}
}

// ȡ�������������
FULLUSERDATA* USER::GetFullUserData() const
{
	return m_pFullUserData;
}

// ���������������
VOID USER::SetFullUserData( FULLUSERDATA* pData )
{
	m_pFullUserData = pData;
}

// ȡ����ҵ� ServerID
ID_t USER::GetServerID() const
{
	if( GetSceneID()<0 ) return INVALID_ID ;
	return g_Config.SceneID2ServerID(GetSceneID()) ;
}

// ȡ����� PlayerID
PlayerID_t USER::GetPlayerID() const
{
	return m_PlayerID;
}

// ������� PlayerID
VOID USER::SetPlayerID( PlayerID_t pid )
{
	m_PlayerID = pid;
}

// ȡ����� GUID
GUID_t USER::GetGUID() const
{
	return m_SimpleData.m_GUID;
}

// ������� GUID
VOID USER::SetGUID( GUID_t guid )
{
	m_SimpleData.m_GUID = guid;
}

// ȡ����� SceneID
SceneID_t USER::GetSceneID() const
{
	return m_SceneID;
}

// ������� SceneID
VOID USER::SetSceneID( SceneID_t SceneID )
{
	m_SceneID = SceneID;
}

// �õ���ҳ�������
const USER_SIMPLE_DATA* USER::GetUserSimpleData() const
{
	return &m_SimpleData;
}

// ȡ���������
const CHAR* USER::GetName() const
{
	return m_SimpleData.m_Name;
}

// �����������
VOID USER::SetName( const CHAR* szName )
{
	Assert(szName);
	strncpy( m_SimpleData.m_Name, szName, sizeof(m_SimpleData.m_Name)-1 );
}

// ȡ������Ա�
UINT USER::GetSex() const
{
	return m_SimpleData.m_uSex;
}

// ��������Ա�
VOID USER::SetSex( UINT uSex )
{
	m_SimpleData.m_uSex = uSex;
}

// ȡ�ý�ɫ���ڵĹ���
INT USER::GetCountry() const
{
	return m_SimpleData.m_nCountry;
}

// ���ý�ɫ���ڵĹ���
VOID USER::SetCountry(INT nCountry)
{
	m_SimpleData.m_nCountry = nCountry;
}

// ȡ����ҵ����ɺ�
INT USER::GetMenpai() const
{
	return m_SimpleData.m_uMenPai;
}

// ������ҵ����ɺ�
VOID USER::SetMenpai(INT nMenpai)
{
	m_SimpleData.m_uMenPai = nMenpai;
}

// �õ���ҵȼ�
INT USER::GetLevel() const
{
	return m_SimpleData.m_nLevel;
}

// ������ҵȼ�
VOID USER::SetLevel(INT nLevel)
{
	m_SimpleData.m_nLevel = nLevel;
}

// �õ���ҵ�ͷ��
INT USER::GetPortrait() const
{
	return m_SimpleData.m_nPortrait;
}

// �������ͷ��
VOID USER::SetPortrait(INT nPortrait)
{
	m_SimpleData.m_nPortrait = nPortrait;
}

// �õ�����ģ��
UCHAR USER::GetFaceModel() const
{
	return m_SimpleData.m_uFaceMeshID;
}

// ��������ģ��
VOID USER::SetFaceModel(UCHAR uFaceMeshID)
{
	m_SimpleData.m_uFaceMeshID = uFaceMeshID;
}

// �õ�ͷ��ģ��
UCHAR USER::GetHairModel() const
{
	return m_SimpleData.m_uHairMeshID;
}

// ����ͷ��ģ��
VOID USER::SetHairModel(UCHAR uHairMeshID)
{
	m_SimpleData.m_uHairMeshID = uHairMeshID;
}

// �õ���ɫ
UINT USER::GetHairColor() const
{
	return m_SimpleData.m_uHairColor;
}

// ���÷�ɫ
VOID USER::SetHairColor(UINT uHairColor)
{
	m_SimpleData.m_uHairColor = uHairColor;
}

// ��������Money
VOID USER::SetMoney(UINT Money)
{
	m_SimpleData.m_uMoney = Money;
}

// �õ�����Money
UINT USER::GetMoney() const
{
	return m_SimpleData.m_uMoney;
}

// ��������Money
VOID USER::SetBankMoney(UINT Money)
{
	m_SimpleData.m_uBankMoney = Money;
}

// �õ�����Money
UINT USER::GetBankMoney() const
{
	return m_SimpleData.m_uBankMoney;
}
// ȡ����ҵĶ����
TeamID_t USER::GetTeamID() const
{
	return m_TeamID;
}

// ������Ҷ����
VOID USER::SetTeamID( TeamID_t tid )
{
	m_TeamID = tid;
}

// ȡ����ҵĳƺ�
const CHAR* USER::GetTitle() const
{
	return m_SimpleData.m_szTitle;
}

// ������ҵĳƺ�
VOID USER::SetTitle(const CHAR* szTitle)
{
	Assert(szTitle);
	strncpy( m_SimpleData.m_szTitle, szTitle, MAX_CHARACTER_TITLE-1 );
}

// �õ���Ұ�� ID
GuildID_t USER::GetGuildID() const
{
	return m_SimpleData.m_GuildID;
}

// ������Ұ�� ID
VOID USER::SetGuildID(GuildID_t gid)
{
	m_SimpleData.m_GuildID = gid;
}

// ������Ұ������
VOID USER::SetGuildName(const CHAR *szGuildName)
{
    memcpy(&m_SimpleData.m_szGuildName[0], szGuildName, MAX_GUILD_NAME_SIZE);
}

// �õ���Ұ������
const CHAR* USER::GetGuildName() const
{
    return m_SimpleData.m_szGuildName;
}

// ������Ҽ�������
VOID USER::SetFamilyName(const CHAR *szFamilyName)
{
    memcpy(&m_SimpleData.m_szFamilyName[0], szFamilyName, MAX_GUILD_FAMILY_NAME_SIZE);
}

// ���ð��������֤��Ϣ
VOID USER::SetGuildInviteKey(const USER_GUILD_DATA &Key)
{
    m_GuildData.m_bInvit = Key.m_bInvit;
    m_GuildData.m_GuildId= Key.m_GuildId;
    m_GuildData.m_iTime  = Key.m_iTime;
}

// �жϴ�ǰ�Ƿ��ѱ�����
BOOL USER::BeInGuildInvite(uint uTime)
{

    if (m_GuildData.m_bInvit)
    {
        if ((uTime-m_GuildData.m_iTime)> USER_GUILD_DATA::TIME_LIMIT)//��ʱ
        {
            CleanUpGuildInviteKey();
            return TRUE;
        }
    }

    return m_GuildData.m_bInvit;
}

// ���հ��������֤��Ϣ
BOOL USER::CompareGuildInviteKey(const GuildID_t GuildId)
{
    return (GuildId == m_GuildData.m_GuildId)?TRUE:FALSE; 
}

// ��հ��������֤��Ϣ
VOID USER::CleanUpGuildInviteKey()
{
    m_GuildData.m_bInvit = FALSE;
    m_GuildData.m_GuildId= INVALID_GUILD_ID;
    m_GuildData.m_iTime  = -1;
}

// �õ���Ҵ���������Ƶ����
ChannelID_t USER::GetChannelID() const
{
	return m_ChannelID;
}

// ������Ҵ���������Ƶ����
VOID USER::SetChannelID( ChannelID_t cid )
{
	m_ChannelID = cid;
}

// �õ��û���֤��
UINT USER::GetKey() const
{
	return m_uKey;
}

// �����û���֤��
VOID USER::SetKey( UINT uKey )
{
	m_uKey = uKey;
}

// ����������
VOID USER::CleanUpGuildData()
{
	m_SimpleData.m_GuildID = INVALID_ID;
	m_GuildData.CleanUp();
}

// �����ҹ�ϵ����
VOID USER::CleanUpRelationData()
{
	m_RelationData.CleanUp();
}

// ȡ����ҵ�����
const CHAR* USER::GetMood() const
{
	return m_RelationData.m_szMood;
}

// ��������
VOID USER::SetMood( const CHAR* szMood )
{
	Assert( szMood );
	strncpy( m_RelationData.m_szMood, szMood, MOOD_DATA_SIZE-1 );
}

// ��������
VOID USER::SetSettings(USHORT uSettings)
{
	m_RelationData.m_Flag.m_wFlags = uSettings;
}

// �Ƿ�ܾ�����Ϊ����
BOOL USER::IsCannotAddFriend()
{
	return m_RelationData.m_Flag.isSetBit(GSF_CANNOT_ADD_FRIEND);
}

// �Ƿ�ܾ��շ��ʼ�
BOOL USER::IsCannotRecvMail()
{
	return m_RelationData.m_Flag.isSetBit(GSF_CANNOT_RECV_MAIL);
}

// �Ƿ�ܾ�İ���˵��ʼ�
BOOL USER::IsCannotRecvStrangerMail()
{
	return m_RelationData.m_Flag.isSetBit(GSF_CANNOT_RECV_STRANGER_MAIL);
}

// �Ƿ�ܾ��������
BOOL USER::IsRefuseTeamInvite()
{
	return m_RelationData.m_Flag.isSetBit(GSF_REFUSE_TEAM_INVITE);
}


//ȡ����˺�
const CHAR*		USER::GetAccount()	const
{
	return m_SimpleData.m_Account;
}
//��������˺�
VOID	USER::SetAccount(const CHAR* pAccount)
{
	Assert(pAccount);
	strncpy(m_SimpleData.m_Account,pAccount,MAX_ACCOUNT_LENGTH-1);
}

#endif // __USER_INL__
