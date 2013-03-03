
#include "stdafx.h"
#include "GuildManager.h"
#include "OnlineUser.h"
#include "Config.h"
#include "City.h"

GuildManager* g_pGuildManager=NULL;

GuildManager::GuildManager( )
{
__ENTER_FUNCTION

	CleanUp( );

__LEAVE_FUNCTION
}

GuildManager::~GuildManager( )
{
__ENTER_FUNCTION

	CleanUp( );

__LEAVE_FUNCTION
}

BOOL GuildManager::Load( CHAR* pGuildFile )
{
__ENTER_FUNCTION


	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

VOID GuildManager::Save( CHAR* pGuildFile )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

BOOL GuildManager::Init( )
{
__ENTER_FUNCTION

	m_NameTable.InitTable(MAX_GUILD_SIZE, MAX_GUILD_NAME_SIZE);
	
	_WORLD_INFO& WorldInfo = g_Config.m_WorldInfo;
	
	if(WorldInfo.m_EnableShareMem)
	{
		_GUILD_INIT	GuildInit;
		INT i;

		for( i=0; i<MAX_GUILD_SIZE; i++ )
		{
			Guild* pGuild = GetGuild( i );
			if( pGuild==NULL )
			{
				Assert(FALSE);
				continue;
			}

			pGuild->InitFromShareMem();
			
		}
		
	}

	
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GuildManager::Init( _GUILD_MANAGER_INIT* pInit )
{
__ENTER_FUNCTION

	m_GuildCount = pInit->m_GuildCount;
	INT i;
	for( i=0; i<pInit->m_GuildCount; i++ )
	{
		GuildID_t guildID = pInit->m_pGuildInit[i].m_GuildID;
		Guild* pGuild = GetGuild( guildID );
		if( pGuild==NULL )
		{
			Assert(FALSE);
			continue;
		}

		pGuild->Init( &(pInit->m_pGuildInit[i]) );
	}

	for( i=0; i<MAX_GUILD_SIZE; i++ )
	{
		Guild* pGuild = GetGuild( i );
		if( pGuild==NULL )
		{
			Assert(FALSE);
			continue;
		}

		if( pGuild->GetGuildID( ) == INVALID_ID )
		{
			m_GuildMallocIndex = i;
			break;
		}
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

VOID GuildManager::CleanUp( )
{
__ENTER_FUNCTION


	for( INT i=0; i<MAX_GUILD_SIZE; i++ )
	{
		m_aGuilds[i].CleanUp();
	}
	m_GuildCount = 0;
	m_GuildMallocIndex = 0;

__LEAVE_FUNCTION
}

BOOL GuildManager::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_GUILD_SIZE; i++ )
	{
		if( m_aGuilds[i].GetGuildID()==INVALID_ID )
			continue;

		m_aGuilds[i].HeartBeat( uTime );
	}


	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

GUILD_ERROR_TYPE GuildManager::CreateGuild( USER* pUser, const CHAR* szName )
{
__ENTER_FUNCTION

	Guild* pGuild = NULL;

	// ��һ�������Ƿ�Ϸ������Ϸ��ͷ��� GUILD_ERROR_INVALID_NAME
	// ������һ��Ҳ���Էŵ� Server ��
	
	if( m_GuildCount >= MAX_GUILD_SIZE )
	{ // û�пյط���
		return GUILD_ERROR_GUILD_FULL;
	}
	
	INT i;
	for( i=0; i<MAX_GUILD_SIZE; ++i, ++m_GuildMallocIndex )
	{
		if( m_GuildMallocIndex >= MAX_GUILD_SIZE )
		{
			m_GuildMallocIndex = 0;
		}

		if( m_aGuilds[m_GuildMallocIndex].GetGuildID() == INVALID_ID )
		{
			break;
		}
	}

	if (pUser->GetLevel() < GUILD_CREATE_LEVEL_LIMIT)
	{// �ȼ�����20
		return GUILD_ERROR_LEVEL_TOO_LOW;
	}

	if(i >= MAX_GUILD_SIZE )
	{ // û�пյط���
		return GUILD_ERROR_GUILD_FULL;
	}

	pGuild = GetGuild(szName);
	if( pGuild != NULL )
	{ // ������
		return GUILD_ERROR_DUPLICATED_NAME;
	}


	GuildID_t guildID = m_GuildMallocIndex;  // ��ʱ�����Ǽ������硢���������

	pGuild = GetGuild(guildID);
	if( pGuild == NULL )
	{
		Assert(FALSE);
		return GUILD_ERROR;
	}

	_GUILD_INIT GuildInit;
	GuildInit.m_GuildID = guildID;
	pGuild->Init(&GuildInit);

	if( pGuild->OnGuildCreate(pUser, szName) )
	{
		pUser->SetGuildID( guildID );
		++m_GuildMallocIndex;
		++m_GuildCount;
		return GUILD_ERROR_NOTHING;
	}
	else
	{
		pGuild->OnGuildCreateFailed();
		return GUILD_ERROR;
	}

__LEAVE_FUNCTION

	return GUILD_ERROR;
}

GUILD_ERROR_TYPE GuildManager::DestroyGuild( GUID_t userGUID, GuildID_t guildID )
{
__ENTER_FUNCTION

	Guild* pGuild = GetGuild(guildID);
	if( pGuild == NULL )
	{
		Assert(FALSE);
		return GUILD_ERROR_NOT_EXIST;
	}

	if( pGuild->OnGuildDestroy(userGUID) == FALSE )
	{
		return GUILD_ERROR;
	}

	UnregisterGuildName(pGuild);
	pGuild->CleanUp();
	pGuild->ValidateShareMem();
	--m_GuildCount;
	return GUILD_ERROR_NOTHING;

__LEAVE_FUNCTION

	return GUILD_ERROR;
}

Guild* GuildManager::GetGuild( GuildID_t guildID )
{
__ENTER_FUNCTION

	if( guildID >= 0 )
	{
		guildID %= MAX_GUILD_SIZE;
		return &(m_aGuilds[guildID]);
	}

__LEAVE_FUNCTION

	return NULL;
}

// ����һ�����
Guild* GuildManager::GetGuild( const CHAR* szName )
{
	return (Guild*)(m_NameTable.Get(szName));
}

// ����һ�� guildid ����õ�������֣�����������򷵻� ""
const CHAR* GuildManager::GetGuildName( GuildID_t guildID )
{
__ENTER_FUNCTION

	Guild* pGuild = GetGuild( guildID );
	if( pGuild != NULL )
	{
		return pGuild->GetGuildName();
	}

__LEAVE_FUNCTION

	return "";
}

// ��ȡ����б�
BOOL GuildManager::GetGuildList( GUILD_WGC_LIST* pGuildList, INT nStartIndex )
{
__ENTER_FUNCTION

	int nMaxListCount; // pGuildList ����ܷż����������
	int nNowIndex = 0; // �������ĵ� n �����

	Assert( nStartIndex >= 0 );
	Assert( nStartIndex <= m_GuildCount );
	//nMaxListCount = (sizeof(pGuildList->m_uGuild) / sizeof(GUILD_WGC_LIST::s));
	nMaxListCount = MAX_GUILD_LIST_COUNT;
	Assert( nMaxListCount > 0 );

	for( INT i=0; i<MAX_GUILD_SIZE; ++i )
	{
		if( m_aGuilds[i].GetGuildID() == INVALID_ID )
		{
			continue;
		}

		if( nNowIndex >= nStartIndex )
		{ // �����￪ʼ��¼
			if( nNowIndex >= nStartIndex + nMaxListCount )
			{ // �����˳�
				break;
			}

			GUILD_WGC_LIST::s* pGuildInfo = &(pGuildList->m_uGuild[pGuildList->m_uGuildListCount++]);
			//����ID
			pGuildInfo->m_GuildID = m_aGuilds[i].GetGuildID();
			//������
			strncpy(pGuildInfo->m_GuildName, m_aGuilds[i].GetGuildName(), MAX_GUILD_NAME_SIZE);
			//������ּ
			strncpy(pGuildInfo->m_GuildDesc, m_aGuilds[i].GetGuildDesc(), MAX_GUILD_DESC_SIZE);
			//������
			strncpy(pGuildInfo->m_ChiefName, m_aGuilds[i].GetGuildChiefName(), MAX_CHARACTER_NAME);
			//������
			if(m_aGuilds[i].GetCity())
			{
				strncpy(pGuildInfo->m_CityName, m_aGuilds[i].GetCity()->GetCityName(), MAX_CITY_NAME_SIZE);
				pGuildInfo->m_PortSceneID = m_aGuilds[i].GetCity()->GetPortSceneID();
			}
			else
			{
				memset(pGuildInfo->m_CityName, 0, MAX_CITY_NAME_SIZE);
				pGuildInfo->m_PortSceneID = -1;
			}
			//����״̬
			pGuildInfo->m_uGuildStatus		=	m_aGuilds[i].GetGuildStatus();
			//���ἶ��
			pGuildInfo->m_bGuildLevel		=	m_aGuilds[i].GetGuildLevel();
			//��������
			pGuildInfo->m_uGuildUserCount	=	m_aGuilds[i].GetGuildUserCount();
			//����ʱ��
			pGuildInfo->m_nFoundedTime		=	m_aGuilds[i].GetFoundedTime();
		}

		++nNowIndex;
	}

	pGuildList->m_uGuildCount = m_GuildCount;

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;

}

// ע��һ��������ƣ��������ظ�ʹ�ã�
VOID GuildManager::RegisterGuildName( Guild* pGuild )
{
	m_NameTable.Add( pGuild->GetGuildName(), pGuild );
}

// ע��һ��������ƣ��ɱ��ٴ�ʹ�ã�
VOID GuildManager::UnregisterGuildName( Guild* pGuild )
{
	m_NameTable.Remove( pGuild->GetGuildName() );
}
