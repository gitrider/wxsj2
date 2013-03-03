/********************************************************************************
 *	�ļ�����	GameStruct_Finger.h
 *	ȫ·����	d:\Prj\Common\GameStruct_Finger.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2006 �� 4 �� 17 ��	21:49
 *
 *	����˵����	��Ҳ�����Ϣ�ṹ
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef __GAMESTRUCT_FINGER_H__
#define __GAMESTRUCT_FINGER_H__

#include "Type.h"

class SocketInputStream;
class SocketOutputStream;

/////////////////////////////////////////////////////////////////////////////////

enum FINGER_REQUEST_TYPE
{
	FREQ_NONE							= 0,
	FREQ_GUID,												// GUID �������
	FREQ_NAME,												// �ǳƲ���
	FREQ_ADVANCED,											// �߼�����
};

enum FINGER_RETURN_TYPE
{
	FRET_NONE							= 0,
	FRET_PLAYERLIST,										// ��������ǿ�ƽ���ɹ�

	FRET_ERR_START,
	FRET_ERR_NORESULT,										// û���κ�ƥ����
};

enum ADVANCED_FINGER_TYPE
{
	AFT_INVALID							= -1,
	AFT_MENPAI,												// ͨ�����ɲ�ѯ
	AFT_GUILD,												// ͨ������ѯ
	AFT_SEX,												// ͨ���Ա��ѯ
	AFT_LEVEL,												// ͨ�������ѯ
	AFT_NUMBER,
};

// FREQ_GUID
struct FINGER_BY_GUID
{
	//����
	GUID_t								m_TargetGUID;		// ĳ��ҵ� GUID
	UCHAR								m_uOnlineFlag;		// �Ƿ�����

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	GUID_t								GetTargetGUID( ) { return m_TargetGUID; }
	VOID								SetTargetGUID( GUID_t guid ) { m_TargetGUID = guid; }

	UCHAR								GetOnlineFlag( ) { return m_uOnlineFlag; }
	VOID								SetOnlineFlag( UCHAR flag ) { m_uOnlineFlag = flag; }
};

// FREQ_NAME
struct FINGER_BY_NAME
{
	//����
	UCHAR								m_uNameSize;
	CHAR								m_szTargetName[MAX_CHARACTER_NAME];
	UCHAR								m_uOnlineFlag;		// �Ƿ�����
	UCHAR								m_uPreciseFlag;		// �Ƿ�ȷ����
	UINT								m_uPosition;		// ��ǰ����λ��

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	VOID								SetTargetName( const CHAR* pName )
	{
		strncpy( m_szTargetName, pName, sizeof(m_szTargetName)-1 );
		m_uNameSize = (UCHAR)strlen(m_szTargetName);
	}
	const CHAR*							GetTargetName( ) { return m_szTargetName; }

	UCHAR								GetOnlineFlag( ) { return m_uOnlineFlag; }
	VOID								SetOnlineFlag( UCHAR flag ) { m_uOnlineFlag = flag; }

	UCHAR								GetPreciseFlag( ) { return m_uPreciseFlag; }
	VOID								SetPreciseFlag( UCHAR flag ) { m_uPreciseFlag = flag; }

	UINT								GetPosition( ) { return m_uPosition; }
	VOID								SetPosition( UINT uPosition ) { m_uPosition = uPosition; }
};

// FREQ_ADVANCED
struct ADVANCED_FINGER
{
	//����
	UCHAR								m_Flags;			// ÿ��λ��ʾ���������Ƿ��ж� ADVANCED_FINGER_TYPE
	UINT								m_uPosition;		// ��ǰ����λ��
	INT									m_nMenPai;			// ���� MENPAI_ATTRIBUTE
	GuildID_t							m_GuildID;			// ���ID
	CHAR								m_Sex;				// �Ա�
	INT									m_nBottomLevel;		// �ȼ���Χ
	INT									m_nTopLevel;		// �ȼ���Χ

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	VOID								SetFingerCondition( ADVANCED_FINGER_TYPE eType, BOOL bFlag )
	{
		if ( bFlag )					{ m_Flags |= (1<<(INT)eType); }
		else							{ m_Flags &= (~(1<<(INT)eType)); }
	}
	BOOL								IsFingerCondition( ADVANCED_FINGER_TYPE eType ) const
	{
		if( m_Flags & (1<<(INT)eType) )	{ return TRUE; }

		return FALSE;
	}

	UINT								GetPosition( ) { return m_uPosition; }
	VOID								SetPosition( UINT uPosition ) { m_uPosition = uPosition; }

	INT									GetMenPai( ) const { return m_nMenPai; }
	VOID								SetMenPai( INT menpai )
	{
		m_nMenPai = menpai;
		SetFingerCondition( AFT_MENPAI, TRUE );
	}

	GuildID_t							GetGuildID( ) const { return m_GuildID; }
	VOID								SetGuildID( GuildID_t gid )
	{
		m_GuildID = gid;
		SetFingerCondition( AFT_GUILD, TRUE );
	}

	CHAR								GetSex( ) const { return m_Sex; }
	VOID								SetSex( CHAR sex )
	{
		m_Sex = sex;
		SetFingerCondition( AFT_SEX, TRUE );
	}

	INT									GetBottomLevel( VOID ) const { return m_nBottomLevel; }
	VOID								SetBottomLevel( INT nLevel )
	{
		m_nBottomLevel = nLevel;
		SetFingerCondition( AFT_LEVEL, TRUE );
	}

	INT									GetTopLevel( VOID ) const { return m_nTopLevel; }
	VOID								SetTopLevel( INT nLevel )
	{
		m_nTopLevel = nLevel;
		SetFingerCondition( AFT_LEVEL, TRUE );
	}
};

// e.g.
// setup a finger request
// CGFinger Msg;
// CG_FINGER* pFinger = Msg.GetFinger();
// pFinger->CleanUp();
// pFinger->m_Type = FREQ_GUID;
// pFinger->m_FingerByGUID.CleanUp();
// pFinger->m_FingerByGUID.SetTargetGUID( guid );

struct CG_FINGER
{
	UCHAR								m_Type;				// enum FINGER_REQUEST_TYPE

	union
	{
		FINGER_BY_GUID					m_FingerByGUID;
		FINGER_BY_NAME					m_FingerByName;
		ADVANCED_FINGER					m_AdvancedFinger;
	};

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

};

struct GW_FINGER : public CG_FINGER
{
	//����
	GUID_t								m_GUID;				// �Լ��� GUID

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	GUID_t								GetGUID( ) { return m_GUID; }
	VOID								SetGUID( GUID_t guid ) { m_GUID = guid; }
};

/////////////////////////////////////////////////////////////////////////////////

// �ǳ� + GUID + OnlineFlag + Level + Sex + MenpaiID + GuildID + ͷ��
struct FINGER_PLAYER_INFO
{
	//����
	GUID_t								m_GUID;								// GUID
	UCHAR								m_uNameSize;
	CHAR								m_szName[MAX_CHARACTER_NAME];		// ����
	UCHAR								m_uOnlineFlag;						// �Ƿ�����
	INT									m_nLevel;							// ��ɫ�ȼ�
	CHAR								m_Sex;								// �Ա�
	INT									m_nMenPai;							// ���� MENPAI_ATTRIBUTE
	GuildID_t							m_GuildID;							// ���ID�����ڷ�������ˣ�
	UCHAR								m_uGuildNameSize;					// 
	CHAR								m_szGuildName[MAX_GUILD_NAME_SIZE];	// ������ƣ����� Server �����ͻ��ˣ�
	INT									m_nPortrait;						// ͷ��

	VOID								CleanUp();
	UINT								GetSize() const;
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

	UCHAR								GetOnlineFlag( ) { return m_uOnlineFlag; }
	VOID								SetOnlineFlag( UCHAR flag ) { m_uOnlineFlag = flag; }

	INT									GetLevel( ) { return m_nLevel; }
	VOID								SetLevel( INT level ) { m_nLevel = level; }

	CHAR								GetSex( ) const { return m_Sex; }
	VOID								SetSex( CHAR sex ) { m_Sex = sex; }

	INT									GetMenPai( ) { return m_nMenPai; }
	VOID								SetMenPai( INT menpai ) { m_nMenPai = menpai; }

	GuildID_t							GetGuildID( ) { return m_GuildID; }
	VOID								SetGuildID( GuildID_t gid ) { m_GuildID = gid; }

	const CHAR*							GetGuildName( ) { return m_szGuildName; }
	VOID								SetGuildName( const CHAR* pName )
	{
		strncpy( m_szGuildName, pName, MAX_GUILD_NAME_SIZE-1 );
		m_uGuildNameSize = (UCHAR)strlen(m_szGuildName);
	}

	INT									GetPortrait( ) { return m_nPortrait; }
	VOID								SetPortrait( INT nPortrait ) { m_nPortrait = nPortrait; }
};

#define FINGER_PLAYER_COUNT	10

// FRET_PLAYERLIST
struct RETURN_FINGER_PLAYERLIST
{
	//����
	UCHAR								m_uPlayerCount;
	FINGER_PLAYER_INFO					m_FingerPlayerInfo[FINGER_PLAYER_COUNT];
	// ��������Ϊ�˼�¼��ҵ�����״̬����ʵ�� <��һҳ> ����
	UCHAR								m_FingerFlag;					// �Ƿ���ҹ�ȫ����Χ(û����һҳ)
	UINT								m_uPosition;					// ��ǰ����λ��

	VOID								CleanUp();
	UINT								GetSize() const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	UCHAR								GetPlayerCount( ) { return m_uPlayerCount; }

	FINGER_PLAYER_INFO*					GetPlayerInfo( INT idx )
	{
		Assert( idx < m_uPlayerCount );
		return &(m_FingerPlayerInfo[idx]);
	}

	FINGER_PLAYER_INFO*					GetLastPlayerInfo( )
	{
		return &(m_FingerPlayerInfo[m_uPlayerCount-1]);
	}

	// ���÷���
	// 1. World ��������
	VOID								AddPlayerInfo( const USER_SIMPLE_DATA* pPlayerInfo );

	// 2. World ���� Server ��Server ����ֱ�ӵ����������ѭ����ֵ
	VOID								AddPlayerInfo( const FINGER_PLAYER_INFO* pPlayerInfo )
	{
		Assert( pPlayerInfo && m_uPlayerCount < FINGER_PLAYER_COUNT );
		memcpy( (void*)&m_FingerPlayerInfo[m_uPlayerCount++], pPlayerInfo, sizeof(FINGER_PLAYER_INFO) );
	}

	UCHAR								GetFingerFlag( ) { return m_FingerFlag; }
	VOID								SetFingerFlag( UCHAR uFlag ) { m_FingerFlag = uFlag; }

	UINT								GetPosition( ) { return m_uPosition; }
	VOID								SetPosition( UINT uPosition ) { m_uPosition = uPosition; }
};

struct GC_FINGER
{
	UCHAR								m_Type;				// enum FINGER_RETURN_TYPE

	union
	{
		RETURN_FINGER_PLAYERLIST		m_FingerPlayerList;
	};

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

};

struct WG_FINGER : public GC_FINGER
{
	//����
	PlayerID_t							m_PlayerID;			// ���˵� PlayerID

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	PlayerID_t							GetPlayerID( ) { return m_PlayerID; }
	VOID								SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
};

//////////////////////////////////////////////////////////////////////////

#endif // __GAMESTRUCT_FINGER_H__
