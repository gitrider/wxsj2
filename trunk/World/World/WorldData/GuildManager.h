/////////////////////////////////////////////////////////////////////////////////
//�ļ�����GuildManager.h
//�����������������߼�
//�޸ļ�¼��2005-11-28 ���� 
//			2005-12-8 ����ṹ�͹��� 
/////////////////////////////////////////////////////////////////////////////////


#ifndef __GUILDMANAGER_H__
#define __GUILDMANAGER_H__

#include "Type.h"
#include "Guild.h"
#include "GameDefine_Guild.h"
#include "StrTable.h"




struct _GUILD_MANAGER_INIT
{
	INT					m_GuildCount;
	_GUILD_INIT*		m_pGuildInit;

	_GUILD_MANAGER_INIT( )
	{
		m_GuildCount = 0;
		m_pGuildInit = NULL;
	}
	~_GUILD_MANAGER_INIT( )
	{
		m_GuildCount = 0;
		SAFE_DELETE_ARRAY( m_pGuildInit );
	}
};


class GuildManager
{
public :
	GuildManager( );
	~GuildManager( );

	BOOL				Load( CHAR* pGuildFile );//�Ӵ��̶�ȡ�����Ϣ����ʱʹ�ã�
	VOID				Save( CHAR* pGuildFile );//����ǰ�����Ϣ���浽���̣���ʱʹ�ã�

	BOOL				Init( );
	BOOL				Init( _GUILD_MANAGER_INIT* pInit );//��ʼ�������Ϣ
	VOID				CleanUp( );

	BOOL				HeartBeat( UINT uTime=0 );//�߼�����

	GUILD_ERROR_TYPE	CreateGuild( USER* pUser, const CHAR* szName );//����һ�����
	GUILD_ERROR_TYPE	DestroyGuild( GUID_t userGUID, GuildID_t guildID );//ɾ��һ�����

	Guild*				GetGuild( GuildID_t guildID );//��ȡ���
	Guild*				GetGuild( const CHAR* szName ); // ����һ�����

	// ����һ�� guildid ����õ�������֣�����������򷵻� ""
	const CHAR*			GetGuildName( GuildID_t guildID );

	// ��ȡ����б�
	BOOL				GetGuildList( GUILD_WGC_LIST* pGuildList, INT nStartIndex );

	// ע��һ��������ƣ��������ظ�ʹ�ã�
	VOID				RegisterGuildName( Guild* pGuild );

	// ע��һ��������ƣ��ɱ��ٴ�ʹ�ã�
	VOID				UnregisterGuildName( Guild* pGuild );

protected :
	Guild				m_aGuilds[MAX_GUILD_SIZE];
	INT					m_GuildCount;
	INT					m_GuildMallocIndex;
	StrTable			m_NameTable;


};

extern GuildManager* g_pGuildManager;


#endif
