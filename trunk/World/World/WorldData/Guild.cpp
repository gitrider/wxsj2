
#include "stdafx.h"
#include "Guild.h"
#include "Packet.h"
#include "GuildManager.h"
#include "OnlineUser.h"
#include "Config.h"
#include "TimeManager.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "WGGuildReturn.h"
#include "ShareMemManager.h"
#include "SMUManager.h"
#include "MailCenter.h"
#include "WorldTable.h"

Guild::Guild( )
{
__ENTER_FUNCTION

	CleanUp( );
	
	_WORLD_INFO& WorldInfo = g_Config.m_WorldInfo;
	
	if(WorldInfo.m_EnableShareMem)
	{
		m_pGuildSmu = g_GuildSMUPool.NewObj();
		Assert(m_pGuildSmu);
	}
	else
	{
		m_pGuildSmu = NULL;
	}
__LEAVE_FUNCTION
}

Guild::~Guild( )
{
__ENTER_FUNCTION

	CleanUp( );
__LEAVE_FUNCTION
}

BOOL Guild::Init( _GUILD_INIT* pInit )
{
__ENTER_FUNCTION

	m_GuildID = pInit->m_GuildID;                                                   //���ID
	memcpy( m_GuildName,pInit->m_GuildName, MAX_GUILD_NAME_SIZE);                   //�������
	m_Status = pInit->m_Status;
	m_ChieftainGUID = pInit->m_ChieftainGUID;
	m_UserCount = pInit->m_UserCount;		                                        //����û�����
	m_MaxUserSize = pInit->m_MaxUserSize;	                                        //����˿�����
	m_GuildPoint = pInit->m_GuildPoint;		                                        //��ṱ�׵�
	m_GuildMoney = pInit->m_GuildMoney;		                                        //����ʽ�
	memcpy( m_aGuildUser, pInit->m_aGuildUser, sizeof(GUILDUSER)*USER_ARRAY_SIZE);  //����Ա
    memcpy( &m_FamilyUser, &pInit->m_FamilyUser, sizeof(FAMILYUSER));               //������Ϣ
    //Todo: Amos ���ݳ�ʼ��
    m_FamilyCount = 0;

	g_pGuildManager->RegisterGuildName( this );

	for( INT i=0; i<USER_ARRAY_SIZE; ++i )
	{
		if( m_aGuildUser[i].m_UserGUID == INVALID_ID )
		{
			m_nUserMallocIndex = i;
			break;
		}
	}

	m_AdminLayOut.CleanUp();
	
	//��������ṹ��ʼ��

	sprintf(m_AdminLayOut.Get(GUILD_ADMIN_t::POS_CHAIR_MAN)->szPosName, "����"); 
	m_AdminLayOut.Get(GUILD_ADMIN_t::POS_CHAIR_MAN)->MaxNumPos = 1;

	sprintf(m_AdminLayOut.Get(GUILD_ADMIN_t::POS_ASS_CHAIR_MAN)->szPosName, "������"); 
	m_AdminLayOut.Get(GUILD_ADMIN_t::POS_ASS_CHAIR_MAN)->MaxNumPos = 3;

	sprintf(m_AdminLayOut.Get(GUILD_ADMIN_t::POS_NORMAL_FAMILY_BOSS)->szPosName, "���峤"); 
	m_AdminLayOut.Get(GUILD_ADMIN_t::POS_NORMAL_FAMILY_BOSS)->MaxNumPos = 3;

    sprintf(m_AdminLayOut.Get(GUILD_ADMIN_t::POS_ELITE_MEMBER)->szPosName, "��Ӣ����"); 

	sprintf(m_AdminLayOut.Get(GUILD_ADMIN_t::POS_MEMBER)->szPosName, "��ͨ����"); 


	return TRUE;


__LEAVE_FUNCTION

	return FALSE;
}

BOOL Guild::InitFromShareMem()
{
	__ENTER_FUNCTION
	if(!m_pGuildSmu)
		return FALSE;

	if(m_pGuildSmu->m_GuildSM.m_GuildID == INVALID_ID)
		return FALSE;

	m_GuildID = m_pGuildSmu->m_GuildSM.m_GuildID; //���ID
	memcpy( m_GuildName,m_pGuildSmu->m_GuildSM.m_GuildName, MAX_GUILD_NAME_SIZE); //�������
	memcpy( m_GuildDesc,m_pGuildSmu->m_GuildSM.m_GuildDesc, MAX_GUILD_DESC_SIZE); //�����ּ
	m_Status = m_pGuildSmu->m_GuildSM.m_Status;
	m_ChieftainGUID = m_pGuildSmu->m_GuildSM.m_ChieftainGUID;
	m_UserCount = m_pGuildSmu->m_GuildSM.m_UserCount; //����û�����
	m_MaxUserSize = m_pGuildSmu->m_GuildSM.m_MaxUserSize; //����˿�����
	m_GuildPoint = m_pGuildSmu->m_GuildSM.m_GuildPoint; //��ṱ�׵�
	m_GuildMoney = m_pGuildSmu->m_GuildSM.m_GuildMoney; //����ʽ�
	m_Longevity	=	m_pGuildSmu->m_GuildSM.m_Longevity;					//����
	m_Contribute	=	m_pGuildSmu->m_GuildSM.m_Contribute;			//���׶�
	m_Honor	=	m_pGuildSmu->m_GuildSM.m_Honor;							//����

    //������Ϣ
    for (INT i=0; i<GUILD_FAMILY_SIZE; ++i)
    {
        for (INT j=0; j<USER_FAMILY_SIZE ; ++j)
        {
            m_FamilyUser.m_aFamilyUser[i][j] = m_pGuildSmu->m_GuildSM.m_FamilyUser.m_aFamilyUser[i][j];
        }
    }

	for( INT i=0; i<USER_ARRAY_SIZE; ++i )
	{
		m_aGuildUser[i].m_Position = m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_Position ;
		m_aGuildUser[i].m_uAccess = m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_uAccess;
		m_aGuildUser[i].m_uLastLoginTime  = m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_uLastLoginTime ;
		m_aGuildUser[i].m_UserGUID = m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_UserGUID ;
		m_aGuildUser[i].m_uLevel	=	m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_uLevel;							//���Ｖ��
		m_aGuildUser[i].m_bMenPaiID	=	m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_bMenPaiID;						    //����
		m_aGuildUser[i].m_iCurContribute	=	m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_iCurContribute;			//����
		m_aGuildUser[i].m_iMaxContribute	=	m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_iMaxContribute;			//�����
		m_aGuildUser[i].m_iJoinTime	=	m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_iJoinTime;						    //����ʱ��
		memcpy(m_aGuildUser[i].m_szUserName,m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_szUserName,MAX_CHARACTER_NAME);
	}

	g_pGuildManager->RegisterGuildName( this );

	for( INT i=0; i<USER_ARRAY_SIZE; ++i )
	{
		if( m_aGuildUser[i].m_UserGUID == INVALID_ID )
		{
			m_nUserMallocIndex = i;
			break;
		}
	}
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

VOID Guild::ValidateShareMem()
{
	__ENTER_FUNCTION

	if(!m_pGuildSmu)
			return;
	if(this->m_GuildID == INVALID_ID)
		return ;

	m_pGuildSmu->Lock(SM_W_WRITE);
	
	m_pGuildSmu->m_GuildSM.m_GuildID = m_GuildID; 
	memcpy( m_pGuildSmu->m_GuildSM.m_GuildName,m_GuildName, MAX_GUILD_NAME_SIZE); //�������
	memcpy( m_pGuildSmu->m_GuildSM.m_GuildDesc,m_GuildDesc, MAX_GUILD_DESC_SIZE); //�����ּ
	m_pGuildSmu->m_GuildSM.m_Status = m_Status;
	m_pGuildSmu->m_GuildSM.m_ChieftainGUID = m_ChieftainGUID  ;
	m_pGuildSmu->m_GuildSM.m_UserCount = m_UserCount  ;					//����û�����
	m_pGuildSmu->m_GuildSM.m_MaxUserSize = m_MaxUserSize;				//����˿�����
	m_pGuildSmu->m_GuildSM.m_GuildPoint = m_GuildPoint;					//��ṱ�׵�
	m_pGuildSmu->m_GuildSM.m_GuildMoney = m_GuildMoney;					//����ʽ�
	m_pGuildSmu->m_GuildSM.m_Longevity	=	m_Longevity;				//����
	m_pGuildSmu->m_GuildSM.m_Contribute	=	m_Contribute;				//���׶�
	m_pGuildSmu->m_GuildSM.m_Honor		=	m_Honor;					//����

	m_pGuildSmu->m_GuildSM.m_GuildMoney	=	m_GuildMoney;				//����ʽ�
	m_pGuildSmu->m_GuildSM.m_AdminLayout=	m_AdminLayOut;				//��������ͼ

    //������Ϣ
    for (INT i=0; i<GUILD_FAMILY_SIZE; ++i)
    {
        for (INT j=0; j<USER_FAMILY_SIZE ; ++j)
        {
            m_pGuildSmu->m_GuildSM.m_FamilyUser.m_aFamilyUser[i][j] = m_FamilyUser.m_aFamilyUser[i][j];
        }
    }

	for( INT i=0; i<USER_ARRAY_SIZE; ++i )
	{
		m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_Position = m_aGuildUser[i].m_Position;
		m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_uAccess = m_aGuildUser[i].m_uAccess;
		m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_uLastLoginTime = m_aGuildUser[i].m_uLastLoginTime;
		m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_UserGUID = m_aGuildUser[i].m_UserGUID;
		m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_uLevel = m_aGuildUser[i].m_uLevel;
		m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_bMenPaiID = m_aGuildUser[i].m_bMenPaiID;
		m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_iCurContribute = m_aGuildUser[i].m_iCurContribute;
		m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_iMaxContribute = m_aGuildUser[i].m_iMaxContribute;
		m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_iJoinTime = m_aGuildUser[i].m_iJoinTime;
		memcpy(m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_szUserName,m_aGuildUser[i].m_szUserName,MAX_CHARACTER_NAME);
	}
	m_pGuildSmu->UnLock(SM_W_WRITE);

	__LEAVE_FUNCTION

	if(m_pGuildSmu)
		m_pGuildSmu->UnLock(SM_W_WRITE);
}

VOID Guild::CleanUp( )
{
__ENTER_FUNCTION

	m_GuildID = INVALID_ID;
	memset( m_GuildName, 0, MAX_GUILD_NAME_SIZE*sizeof(CHAR) );
	memset( m_GuildDesc, 0, MAX_GUILD_DESC_SIZE*sizeof(CHAR) );
	memset( m_GuildChiefName, 0, MAX_CHARACTER_NAME*sizeof(CHAR) );
	
	m_UserCount = 0;
	m_nProposerCount = 0;
	m_MaxUserSize = 0;
	m_GuildPoint = 0;
	m_GuildMoney = 0;
    m_FamilyUser.CleanUp();

	for( INT i=0; i<USER_ARRAY_SIZE; i++ )
	{
		m_aGuildUser[i].CleanUp();
	}

	m_CityID = INVALID_ID;
	m_pCity = NULL;
	m_uTimeStamp = 0;
	m_IsLock = FALSE;
	m_GuildLevel = 0;
	m_Longevity		=	0;			//����
	m_Contribute	=	0;			//���׶�
	m_Honor			=	0;			//����


	m_AdminLayOut.CleanUp();

__LEAVE_FUNCTION
}

BOOL Guild::HeartBeat( UINT uTime )
{ // ��δ���ɰ����м�飬�Ƿ������������Ƿ�ʱ�䵽����������Ҫ��������ȵ�
__ENTER_FUNCTION

	switch( m_Status )
	{
	case GUILD_STATUS_NORMAL:
		{
		}
		break;
	default:
		Assert(FALSE);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// ȡ�ð���
const GUILDUSER* Guild::GetGuildUser( GUID_t userGUID )
{
__ENTER_FUNCTION

	for( INT i=0; i<USER_ARRAY_SIZE; ++i )
	{
		if( m_aGuildUser[i].m_UserGUID == userGUID )
		{
			return &(m_aGuildUser[i]);
		}
	}

	return NULL;

__LEAVE_FUNCTION

	return NULL;
}

VOID Guild::SetGuildUserTitleName( GUID_t userGUID, const CHAR *pszTitleName)
{
    if (pszTitleName)
    {
        UCHAR cSize = (UCHAR)strlen(pszTitleName);
        if (cSize > 0)
        {
            INT index = 0;
            GetGuildUser(userGUID, index);

            strncpy( m_aGuildUser[index].m_szTitleName, pszTitleName, cSize);
        }
    }
}

const GUILDUSER* Guild::GetGuildUser( GUID_t userGUID, INT &iIndex)
{
__ENTER_FUNCTION

    for( INT i=0; i<USER_ARRAY_SIZE; ++i )
    {
        if( m_aGuildUser[i].m_UserGUID == userGUID )
        {
             iIndex  = i;
             return &(m_aGuildUser[i]);
        }
    }

    return NULL;

__LEAVE_FUNCTION
}

// �ж��Ƿ���ĳ��Ȩ��
BOOL Guild::IsAuthorized( GUID_t userGUID, GUILD_AUTHORITY authority )
{
__ENTER_FUNCTION

	const GUILDUSER* pGuildUser = GetGuildUser( userGUID );
	if( pGuildUser == NULL )
	{
		return FALSE;
	}

	return ((authority & pGuildUser->m_uAccess) != 0);

__LEAVE_FUNCTION

	return FALSE;
}

// ������ᣬ��ʽ����
BOOL Guild::OnGuildCreate( const USER* pCreater, const CHAR* szName )
{
__ENTER_FUNCTION

	if( pCreater == NULL )
	{
		Assert(pCreater);
		return FALSE;
	}
	m_Status = GUILD_STATUS_NORMAL;
	strncpy( m_GuildName, szName, sizeof(m_GuildName)-1 ); //�������
	m_ChieftainGUID		= pCreater->GetGUID();
	m_MaxUserSize		= g_Config.m_ConfigInfo.m_nDefaultMaxMemberCount;
    m_MaxUser           = g_GuildLevelInfoTbl.m_TableInfo[0].m_MaxGuildUser;
	m_aGuildUser[0].m_UserGUID	= pCreater->GetGUID();
	strncpy( m_aGuildUser[0].m_szUserName, pCreater->GetName(), sizeof(m_aGuildUser[0].m_szUserName) - 1 );
	m_aGuildUser[0].m_uAccess		    = GUILD_AUTHORITY_CHIEFTAIN;
	m_aGuildUser[0].m_bOnlineFlag	    = TRUE;
	m_aGuildUser[0].m_uLastLoginTime    = g_pTimeManager->Time2DWORD();
	m_aGuildUser[0].m_iJoinTime		    = m_aGuildUser[0].m_uLastLoginTime;
    m_aGuildUser[0].m_uLevel            = pCreater->GetLevel();
    m_aGuildUser[0].m_cSex              = pCreater->GetSex();
    m_aGuildUser[0].m_bMenPaiID         = pCreater->GetMenpai();
	memcpy(m_GuildChiefName, m_aGuildUser[0].m_szUserName, MAX_CHARACTER_NAME);
	memcpy(m_GuildCreatorName, m_aGuildUser[0].m_szUserName, MAX_CHARACTER_NAME);

    //Todo:�Զ����ɻƽ����  �߻��ĵ�������
    //m_FamilyUser.Add(0, 0);

	//�Զ���Ϊ����
	OnUserPositionChange(m_aGuildUser[0].m_UserGUID, GUILD_POSITION_CHIEFTAIN);

	m_UserCount = 1;

	g_pGuildManager->RegisterGuildName( this );

	OnGuildFound();

	ValidateShareMem();

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// ��ᴴ��ʧ��
VOID Guild::OnGuildCreateFailed()
{
__ENTER_FUNCTION

	AssertEx( FALSE, "A guild create failed(Skip it)." );

	ValidateShareMem();

__LEAVE_FUNCTION
}

// �������������İ�������������ʱ
VOID Guild::OnGuildFound()
{
__ENTER_FUNCTION

	m_Status = GUILD_STATUS_NORMAL;

    time_t t = time(NULL);
    tm* pTm = localtime(&t);

    INT nYear = pTm->tm_year + 1900;
    INT nDay  = pTm->tm_mday;
    INT nMonth= pTm->tm_mon + 1;

    m_nTime = nYear%100*10000 + nMonth*100 + nDay;

	// ��Ϣ�㲥��ȥ
	WGGuildReturn Msg;


	_GUILD_RETURN GuildReturn;
	GuildReturn.m_ReturnType = GUILD_RETURN_FOUND;
	Msg.SetGuildReturn( &GuildReturn );

	BroadCast_Return( &Msg );

	ValidateShareMem();

__LEAVE_FUNCTION
}


BOOL Guild::OnGuildDestroy( GUID_t createrGUID )//���ٰ��
{
__ENTER_FUNCTION
	if( createrGUID != m_ChieftainGUID )
	{
		return FALSE;
	}

	USER* pUser;

	for( INT i=0; i<USER_ARRAY_SIZE; ++i )
	{ // ������������ҵ� GuildID ��ɾ��
		if( m_aGuildUser[i].m_UserGUID != INVALID_ID)
		{
			if(m_aGuildUser[i].m_bOnlineFlag)
			{
				pUser = g_pOnlineUser->FindUser(m_aGuildUser[i].m_UserGUID);
				if( pUser != NULL )
				{
					pUser->CleanUpGuildData();
				}
			}
			else
			{
				CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
				sprintf(szMailContent, "��İ���Ѿ��ڽ����ϳ�����");
				g_pMailCenter->SendNormalMail( m_aGuildUser[i].m_szUserName, szMailContent);

				//��һ���ִ���ʼ�
				//���´δ��������ʱ����Serverִ�д��ʼ���Ӧ�Ľű�������ֻ�Ǹ��Ĵ��˵Ĺ���ID
				g_pMailCenter->SendScriptMail( m_aGuildUser[i].m_szUserName,
					MAIL_UPDATE_ATTR, MAIL_ATTR_GUILD, INVALID_ID);
			}
		}
	}

	m_AdminLayOut.CleanUp();

	// ��Ϣ�㲥��ȥ
	WGGuildReturn Msg;

	_GUILD_RETURN GuildReturn;
	GuildReturn.m_ReturnType = GUILD_RETURN_DISMISS;
	Msg.SetGuildReturn( &GuildReturn );

	BroadCast_Return( &Msg );
	
	ValidateShareMem();

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// 0001	����ְ��
GUILD_ADMIN_t::ORESULT Guild::OnUserPositionChange( GUID_t userGUID, GUILD_POSITION position )
{
__ENTER_FUNCTION

	GUILDUSER* pGuildUser = ILGetGuildUser( userGUID );
	if( pGuildUser == NULL )
	{
		//Assert(0);
		return	GUILD_ADMIN_t::RET_NO_USER;
	}

	INT iUserIndex = ILGetGuildUserIndex( userGUID );

	GUILD_POSITION OldPos = pGuildUser->m_Position;

	switch(OldPos)
	{
	case GUILD_POSITION_CHIEFTAIN:
		{
			GUILD_ADMIN_t::ORESULT oResult = m_AdminLayOut.Remove(GUILD_ADMIN_t::POS_CHAIR_MAN, iUserIndex);

			if(GUILD_ADMIN_t::RET_NOT_EXIST == oResult)
			{
				//Assert(0);
				return GUILD_ADMIN_t::RET_NOT_EXIST;
			}
			pGuildUser->m_uAccess = GUILD_AUTHORITY_MEMBER;
			pGuildUser->m_Position = GUILD_POSITION_MEMBER;
		}
		break;

	case GUILD_POSITION_ASS_CHIEFTAIN:
		{
			GUILD_ADMIN_t::ORESULT oResult = m_AdminLayOut.Remove(GUILD_ADMIN_t::POS_ASS_CHAIR_MAN, iUserIndex);
			
			if(GUILD_ADMIN_t::RET_NOT_EXIST == oResult)
			{
				//Assert(0);
				return GUILD_ADMIN_t::RET_NOT_EXIST;
			}
			pGuildUser->m_uAccess = GUILD_AUTHORITY_MEMBER;
			pGuildUser->m_Position = GUILD_POSITION_MEMBER;
		}
		break;

	default:
		break;
	}
	
	switch(position)
	{

	case GUILD_POSITION_CHIEFTAIN:
		{
			GUILD_ADMIN_t::ORESULT oResult = m_AdminLayOut.Add(GUILD_ADMIN_t::POS_CHAIR_MAN, iUserIndex);
			
			if(GUILD_ADMIN_t::RET_POS_FULL == oResult)
			{
				//Assert(0);
				return GUILD_ADMIN_t::RET_POS_FULL;
			}
			pGuildUser->m_uAccess = GUILD_AUTHORITY_CHIEFTAIN;
		}
		break;

	case GUILD_POSITION_ASS_CHIEFTAIN:
		{
			GUILD_ADMIN_t::ORESULT oResult = m_AdminLayOut.Add(GUILD_ADMIN_t::POS_ASS_CHAIR_MAN, iUserIndex);

			if(GUILD_ADMIN_t::RET_POS_FULL == oResult)
			{
				//Assert(0);
				return GUILD_ADMIN_t::RET_POS_FULL;
			}
			pGuildUser->m_uAccess = GUILD_AUTHORITY_ASSCHIEFTAIN;
		}
		break;

	case GUILD_POSITION_MEMBER:
		{
			pGuildUser->m_uAccess = GUILD_AUTHORITY_MEMBER;
		}
		break;

	default:
		break;
	}

	pGuildUser->m_Position	= position;

	ValidateShareMem();
	return GUILD_ADMIN_t::RET_SUCCESS;

__LEAVE_FUNCTION
	return GUILD_ADMIN_t::RET_UNKOWN;
}

// 0002	����Ȩ��
VOID Guild::OnUserAuthorityChange( GUID_t userGUID, GUILD_AUTHORITY authority )
{
__ENTER_FUNCTION

	GUILDUSER* pGuildUser = ILGetGuildUser( userGUID );
	if( pGuildUser == NULL )
	{
		return;
	}

	if( IsAuthorized( userGUID, authority ) )
	{ // ��Ȩ
		pGuildUser->m_uAccess &= ~((UINT)authority);
	}
	else
	{ // ��Ȩ
		pGuildUser->m_uAccess |= (UINT)authority;
	}

	ValidateShareMem();

__LEAVE_FUNCTION
}

// 0002	����Ȩ��
VOID Guild::OnPositionAuthorityChange( GUILD_POSITION position, GUILD_AUTHORITY authority )
{
}

// ������������Ӧһ�����
GUILD_ERROR_TYPE Guild::OnUserEnter( const USER* pUser )
{
__ENTER_FUNCTION
	if( pUser == NULL )
	{
		Assert(pUser);
		return GUILD_ERROR;
	}

	//if( pUser->GetGuildID() != INVALID_ID )
	//{
	//	Assert(FALSE && "pUser have been in a guild.");
	//	return GUILD_ERROR;
	//}

	GUILDUSER GuildUser;

	switch( m_Status )
	{
	case GUILD_STATUS_NORMAL:
		{
			if( IsProposerFull() )
			{
//				Assert(FALSE && "The guild can't hold more proposers.");
                // �������������������뼰ʱ����
                CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                sprintf(szMailContent, "�������������������뼰ʱ����" );

                for( INT i=0; i<USER_ARRAY_SIZE; ++i )
                {
                    if ((GUILD_AUTHORITY_RECRUIT & m_aGuildUser[i].m_uAccess) != 0)
                    {
                        g_pMailCenter->SendNormalMail(m_aGuildUser[i].m_szUserName, szMailContent);
                    }
                }
				return GUILD_ERROR_PROPOSER_FULL;
			}

			if( IsMemberFull() )
			{
//				Assert(FALSE && "The guild can't hold more users.");
				return GUILD_ERROR_MEMBER_FULL;
			}

            if ( m_UserCount >= m_MaxUser )
            {
                return GUILD_ERROR_MEMBER_FULL;
            }

            if (pUser->GetLevel() < 10)
            {
                return GUILD_ERROR_LEVEL_TO_JOIN;
            }
            
			if(ILGetGuildUser(pUser->GetGUID()))
			{
			//	Assert(FALSE && "Already in the guild.");
                const GUILDUSER *pGuildUser =  GetGuildUser(pUser->GetGUID());
                if (pGuildUser)
                {
                    if (pGuildUser->m_Position == GUILD_POSITION_TRAINEE)
                        return GUILD_ERROR_WAITTING;
                }

				return GUILD_ERROR_IN_GUILD;
			}

			GuildUser.m_UserGUID = pUser->GetGUID();
			strncpy( GuildUser.m_szUserName, pUser->GetName(), sizeof(GuildUser.m_szUserName) - 1 );
			GuildUser.m_uAccess		= GUILD_AUTHORITY_LEAVE;
			GuildUser.m_bOnlineFlag = TRUE;
			GuildUser.m_uLastLoginTime = g_pTimeManager->Time2DWORD();
            GuildUser.m_uLevel      = pUser->GetLevel();
            GuildUser.m_cSex        = pUser->GetSex();
            GuildUser.m_bMenPaiID   = pUser->GetMenpai();
			AddGuildUser( GuildUser );
			OnUserPositionChange(GuildUser.m_UserGUID, GUILD_POSITION_TRAINEE);
			++m_nProposerCount;

		}
		break;
	default:
		Assert(FALSE);
		return GUILD_ERROR;
	}
	
	ValidateShareMem();

	return GUILD_ERROR_NOTHING;

__LEAVE_FUNCTION

	return GUILD_ERROR;
}

// ���������
GUILD_ERROR_TYPE Guild::OnUserInvite( const USER* pUser )
{
__ENTER_FUNCTION
    if( pUser == NULL )
    {
        Assert(pUser);
        return GUILD_ERROR;
    }

    GUILDUSER GuildUser;
    switch( m_Status )
    {
    case GUILD_STATUS_NORMAL:
        {
            if( IsProposerFull() )
            {
                return GUILD_ERROR_PROPOSER_FULL;
            }

            if( IsMemberFull() )
            {
                return GUILD_ERROR_MEMBER_FULL;
            }
            GuildUser.m_UserGUID = pUser->GetGUID();
            strncpy( GuildUser.m_szUserName, pUser->GetName(), sizeof(GuildUser.m_szUserName) - 1 );
            GuildUser.m_uAccess		= GUILD_AUTHORITY_MEMBER;
            GuildUser.m_bOnlineFlag = TRUE;
            GuildUser.m_uLastLoginTime = g_pTimeManager->Time2DWORD();
            GuildUser.m_uLevel      = pUser->GetLevel();
            GuildUser.m_cSex        = pUser->GetSex();
            GuildUser.m_bMenPaiID   = pUser->GetMenpai();
            AddGuildUser( GuildUser );
            OnUserPositionChange(GuildUser.m_UserGUID, GUILD_POSITION_MEMBER);
			++m_UserCount;
        }
        break;
    default:
        Assert(FALSE);
        return GUILD_ERROR;
    }

    ValidateShareMem();
    return GUILD_ERROR_NOTHING;
__LEAVE_FUNCTION
    return GUILD_ERROR;
}

VOID Guild::SetFamilyID(INT iGuildIndex, INT iID)
{
__ENTER_FUNCTION
    if (iGuildIndex < USER_ARRAY_SIZE)
    {
        m_aGuildUser[iGuildIndex].m_iFamilyID = iID;
    }
__LEAVE_FUNCTION
}

// ȡ�ü�����
INT Guild::GetFamilyID(GUID_t userGUID)
{
__ENTER_FUNCTION
    INT iIndex;
    const GUILDUSER *pUser = GetGuildUser(userGUID, iIndex);
    Assert(pUser);

    if (pUser)
    {
        return m_aGuildUser[iIndex].m_iFamilyID;     
    }
__LEAVE_FUNCTION
	return INVALID_ID;
}

// ���ð��ڹ��׶�   
VOID Guild::SetGuildPoint(GUID_t userGUID, INT nGP)
{
    __ENTER_FUNCTION

    GUILDUSER *pUser = ILGetGuildUser(userGUID);
    Assert(pUser);

    if (pUser)
    {
        pUser->m_nGP    =   nGP;     
    }

    __LEAVE_FUNCTION
}

// ȡ�ð��ڹ��׶�   
INT Guild::GetGuildPoint(GUID_t userGUID)
{
    __ENTER_FUNCTION

    GUILDUSER *pUser = ILGetGuildUser(userGUID);
    Assert(pUser);

    if (pUser)
    {
        return pUser->m_nGP;     
    }
    __LEAVE_FUNCTION
    return 0;
}

// ���ð���ܹ��׶�   
VOID Guild::SetTotalGuildPoint(INT nGP)
{
    __ENTER_FUNCTION

        m_TotalGuildPoint   =   nGP;

    __LEAVE_FUNCTION
}

// ȡ�ð���ܹ��׶�   
INT Guild::GetTotalGuildPoint()
{
    __ENTER_FUNCTION

        return m_TotalGuildPoint;

    __LEAVE_FUNCTION
        return 0;
}

// ��������
INT Guild::OnCreateFamily(const GUID_t CreatorGuid,const INT iShaikhIndex, const GUID_t ShaikhGuid,
                          const CHAR* szFamilyName, const INT iFamilyNameSize,
                          const CHAR* szFamilyDesc, const INT iFamilyDescSize, CHAR &cFamilyID )
{
__ENTER_FUNCTION
    //�ǰ������ܴ���
    if (CreatorGuid != m_aGuildUser[0].m_UserGUID)
        return FAMILYUSER::RET_UNKOWN;

    for (BYTE i=0; i<GUILD_FAMILY_SIZE; ++i)
    {//0�ż����Աһ���Ǽ��峤
        INT index = m_FamilyUser.m_aFamilyUser[i][0];
        if (INVALID_ID == index)
        {
            //�õȼ��ܲ����ٴ�������ļ���
            if ((index + 1) > m_MaxFamily)
            {
                return FAMILYUSER::RET_POS_FULL;
            }

            cFamilyID = i;
            //��Ӽ����Ա
            if (m_aGuildUser[iShaikhIndex].m_UserGUID == ShaikhGuid)//У����峤
            {
                INT ret = m_FamilyUser.Add(i, iShaikhIndex);
                SetFamilyID(iShaikhIndex, i);

                if (FAMILYUSER::RET_SUCCESS == ret)
                {
                    //�������ֺ���ּ
                    INT iShaikhNameSize = (INT)strlen(m_aGuildUser[iShaikhIndex].m_szUserName);
                    m_FamilyUser.ModifyFamilyName(i, szFamilyName, iFamilyNameSize);
                    m_FamilyUser.ModifyFamilyDesc(i, szFamilyDesc, iFamilyDescSize);
                    m_FamilyUser.ModifyShaikhName(i, &m_aGuildUser[iShaikhIndex].m_szUserName[0], iShaikhNameSize);
                }
                ++m_FamilyCount;
                return FAMILYUSER::RET_SUCCESS;
            }
            return FAMILYUSER::RET_NO_EXIST;
        }
    }
__LEAVE_FUNCTION
    return FAMILYUSER::RET_POS_FULL;
}

// ɾ�������Ա
INT Guild::OnFamilyDismiss(CHAR cFamilyIndex)
{
__ENTER_FUNCTION

    if (cFamilyIndex >= 0 && cFamilyIndex < GUILD_FAMILY_SIZE)
    {
        m_FamilyUser.ModifyFamilyDesc(cFamilyIndex, NULL, 0);
        m_FamilyUser.ModifyFamilyName(cFamilyIndex, NULL, 0);
        m_FamilyUser.ModifyShaikhName(cFamilyIndex, NULL, 0);
        
        for (INT i=0; i<USER_FAMILY_SIZE ; ++i)
        {
            {
                m_FamilyUser.m_aFamilyUser[cFamilyIndex][i] = INVALID_ID;
            }
        }
        return FAMILYUSER::RET_SUCCESS;
    }
__LEAVE_FUNCTION

    return FAMILYUSER::RET_UNKOWN;
}

// ��Ӽ����Ա
INT Guild::OnAddFamilyUser(const GUID_t InvitorGuid, CHAR cFamilyIndex, INT iGuildIndex, GUID_t guid)
{
__ENTER_FUNCTION
    if ( IsAuthorized(InvitorGuid, GUILD_AUTHORITY_ADDFAMILYUSER) )
        return PACKET_EXE_CONTINUE;

    //У��GUID
    if (iGuildIndex < USER_ARRAY_SIZE)
    {
        if (m_aGuildUser[iGuildIndex].m_UserGUID == guid)
        {//ͨ��,���
            INT ret = m_FamilyUser.Add(cFamilyIndex, iGuildIndex, m_MaxGoldFamilyUser);
            SetFamilyID(iGuildIndex, cFamilyIndex);

            if (FAMILYUSER::RET_SUCCESS  != ret)
            {
                Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X addFamilyUser failed.GUILD_ID=%d)!", 
                    guid, GetGuildID());

                return FAMILYUSER::RET_UNKOWN;
            }
            return FAMILYUSER::RET_SUCCESS;
        }

        Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X  addFamilyUser error,Guid in msg != Guid in GuildUser!", 
                      guid);
    }

    Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X OnAddFamilyUser iGuildIndex is overflow!", 
        guid);
__LEAVE_FUNCTION
    return PACKET_EXE_ERROR;
}

// ɾ�������Ա
INT Guild::OnRemoveFamilyUser(const GUID_t BossGuid,CHAR cFamilyIndex, INT iGuildIndex, GUID_t guid)
{
__ENTER_FUNCTION
    if (IsAuthorized(BossGuid, GUILD_AUTHORITY_DELFAMILYUSER))
        PACKET_EXE_CONTINUE;

    //У��GUID
    if (iGuildIndex < USER_ARRAY_SIZE)
    {
        if (m_aGuildUser[iGuildIndex].m_UserGUID == guid)
        {//ͨ��,del
            INT ret = m_FamilyUser.Remove(cFamilyIndex, iGuildIndex);
            SetFamilyID(iGuildIndex, INVALID_ID);

            if (FAMILYUSER::RET_SUCCESS != ret)
            {
                Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X RemoveFamilyUser failed.GUILD_ID=%d)!", 
                    guid, GetGuildID());
            }
            return FAMILYUSER::RET_SUCCESS;
        }

        Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X  RemoveFamilyUser error,Guid in msg != Guid in GuildUser!", 
            guid);
    }
    
    Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X OnRemoveFamilyUser iGuildIndex is overflow!", 
        guid);
__LEAVE_FUNCTION
    return PACKET_EXE_ERROR;
}

// ��ȡ�����Ա��Ϣ�б�
INT Guild::GetFamilyUser(CHAR cFamilyIndex, INT* pFamilyUser)
{
__ENTER_FUNCTION
    INT UserCount = 0;
    for (INT i=0,j=0; i<USER_FAMILY_SIZE ; ++i)
    {
        if (INVALID_ID != m_FamilyUser.m_aFamilyUser[cFamilyIndex][i])
            UserCount++;

        pFamilyUser[j++] = m_FamilyUser.m_aFamilyUser[cFamilyIndex][i];
    }
    return UserCount;
__LEAVE_FUNCTION
}

// ��ȡ��������
const CHAR * Guild::GetFamilyName(CHAR cFamilyIndex)
{
__ENTER_FUNCTION

    return m_FamilyUser.GetFamilyName(cFamilyIndex);

__LEAVE_FUNCTION

    return NULL;
}

// ��ȡ������ּ
const CHAR * Guild::GetFamilyDesc(CHAR cFamilyIndex)
{
__ENTER_FUNCTION

    return m_FamilyUser.GetFamilyDesc(cFamilyIndex);

__LEAVE_FUNCTION

    return NULL;
}

// ��ȡ���峤����
const CHAR * Guild::GetShaikhName(CHAR cFamilyIndex)
{
__ENTER_FUNCTION

    return m_FamilyUser.GetShaikhName(cFamilyIndex);

__LEAVE_FUNCTION
    return NULL;
}

// ȡ�ü�������
UINT Guild::GetFamilyCount()
{
__ENTER_FUNCTION

    return m_FamilyCount;

__LEAVE_FUNCTION

    return INVALID_ID;
}


// 0003 ���հ���
GUILD_ERROR_TYPE Guild::OnRecruitUser( GUID_t userGUID )
{
__ENTER_FUNCTION

	if( m_UserCount >= m_MaxUserSize )
	{
		Assert(FALSE && "The guild can't hold more users.");
		return GUILD_ERROR_MEMBER_FULL;
	}
	INT i;
	for( i=0; i<USER_ARRAY_SIZE; ++i )
	{
		if( m_aGuildUser[i].m_UserGUID == userGUID )
		{
			m_aGuildUser[i].m_uAccess = GUILD_AUTHORITY_MEMBER;
			OnUserPositionChange(m_aGuildUser[i].m_UserGUID, GUILD_POSITION_MEMBER);
			++m_UserCount;
			--m_nProposerCount;
			break;
		}
	}

	if(i == USER_ARRAY_SIZE)
	{
		return GUILD_ERROR_MEMBER_FULL;
	}

	ValidateShareMem();

	return GUILD_ERROR_NOTHING;

__LEAVE_FUNCTION

	return GUILD_ERROR;
}

// 0004	��������
// 0008	�뿪���
GUILD_ERROR_TYPE Guild::OnUserLeave( GUID_t userGUID )
{
__ENTER_FUNCTION

	for( INT i=0; i<USER_ARRAY_SIZE; ++i )
	{
		if( m_aGuildUser[i].m_UserGUID == userGUID )
		{
			if( m_aGuildUser[i].m_Position == GUILD_POSITION_TRAINEE )
			{ // �������뿪
				--m_nProposerCount;
			}
			else
			{ // ��ʽ�����뿪
				--m_UserCount;
			}
			OnUserPositionChange(m_aGuildUser[i].m_UserGUID, GUILD_POSITION_INVALID);

			m_aGuildUser[i].CleanUp();

			ValidateShareMem();

			return GUILD_ERROR_NOTHING;
		}
	}

	return GUILD_ERROR_MEMBER_NOT_EXIST;

__LEAVE_FUNCTION

	return GUILD_ERROR;
}

// ���������뿪���
GUILD_ERROR_TYPE Guild::OnUserLeaveByIndex( INT iIndex )
{
	__ENTER_FUNCTION
	if( m_aGuildUser[iIndex].m_UserGUID != INVALID_ID )
	{
		if( m_aGuildUser[iIndex].m_Position == GUILD_POSITION_TRAINEE )
		{ // �������뿪
			--m_nProposerCount;
		}
		else
		{ // ��ʽ�����뿪
			--m_UserCount;
		}
		OnUserPositionChange(m_aGuildUser[iIndex].m_UserGUID, GUILD_POSITION_INVALID);

		m_aGuildUser[iIndex].CleanUp();

		ValidateShareMem();

		return GUILD_ERROR_NOTHING;
	}
	__LEAVE_FUNCTION
	return GUILD_ERROR;
}


// 0005	���ã�������ְ��ͬ���ǣ�������Ҫ������ְ����˼���ȥ
GUILD_ERROR_TYPE Guild::OnDemise()
{
__ENTER_FUNCTION

	GUILDUSER* pOldChieftain = ILGetGuildUser( m_ChieftainGUID );
	if( pOldChieftain == NULL )
	{
		Assert( FALSE && "This guild does not have a Chieftain!" );
		return GUILD_ERROR_MEMBER_NOT_EXIST;
	}

	INT iIndex = m_AdminLayOut.Get(GUILD_ADMIN_t::POS_ASS_CHAIR_MAN)->Posidx[0];
	if(iIndex == -1)
	{
		Assert( FALSE && "No Ass Chief" );
		return GUILD_ERROR_NO_ASS_CHIEF;
	}

	GUILDUSER* pNewChieftain = &m_aGuildUser[iIndex];
	GUID_t newChieftainGUID = pNewChieftain->m_UserGUID;

	// ���Ӳ���
	OnUserPositionChange(m_ChieftainGUID, GUILD_POSITION_MEMBER);
	pOldChieftain->m_uAccess	= GUILD_AUTHORITY_MEMBER;

	OnUserPositionChange(newChieftainGUID, GUILD_POSITION_CHIEFTAIN);
	pNewChieftain->m_uAccess	= GUILD_AUTHORITY_CHIEFTAIN;

	//OnUserPositionChange(m_ChieftainGUID, GUILD_POSITION_ASS_CHIEFTAIN);
	//pOldChieftain->m_uAccess	= GUILD_AUTHORITY_ASSCHIEFTAIN;

	m_ChieftainGUID = newChieftainGUID;

	memcpy(m_GuildChiefName, pNewChieftain->m_szUserName, MAX_CHARACTER_NAME);

	ValidateShareMem();
	return GUILD_ERROR_NOTHING;
__LEAVE_FUNCTION
	return GUILD_ERROR;
}

// 0006	֧ȡ���
// 0007	������
VOID Guild::OnGuildMoneyChange( GUID_t userGUID, INT nMoney )//����ʽ����仯
{
__ENTER_FUNCTION

	m_GuildMoney += nMoney;

	if( m_GuildMoney < 0 )
	{
		m_GuildMoney = 0;
		Assert(FALSE);
	}
	
	ValidateShareMem();

__LEAVE_FUNCTION
}

VOID Guild::OnUserAccessChange( GUID_t userGUID, INT newAccess )//���Ȩ�ޱ仯
{
__ENTER_FUNCTION

	ValidateShareMem();

__LEAVE_FUNCTION
}

VOID Guild::OnGuildPointChange( GUID_t userGUID, INT nMoney )//��ṱ�׵㷢���仯
{
__ENTER_FUNCTION

	ValidateShareMem();

__LEAVE_FUNCTION
}

// ����������
VOID Guild::OnUserLogin( GUID_t userGUID )
{
__ENTER_FUNCTION

	GUILDUSER* pGuildUser = ILGetGuildUser( userGUID );
	if( pGuildUser == NULL )
	{
		return;
	}

	pGuildUser->m_bOnlineFlag = TRUE;
	pGuildUser->m_uLastLoginTime = g_pTimeManager->Time2DWORD();
	
	ValidateShareMem();

__LEAVE_FUNCTION
}

// ����������
VOID Guild::OnUserLogoff( GUID_t userGUID )
{
__ENTER_FUNCTION

	GUILDUSER* pGuildUser = ILGetGuildUser( userGUID );
	if( pGuildUser == NULL )
	{
		return;
	}

	pGuildUser->m_bOnlineFlag = FALSE;
	
__LEAVE_FUNCTION
}

VOID Guild::BroadCast_Return( Packet* pPacket, GUILD_POSITION positon )
{
__ENTER_FUNCTION

	Assert( pPacket );

	USER* pUser;
	ServerPlayer* pServerPlayer;
	WGGuildReturn* pPacketReturn = (WGGuildReturn*)pPacket;
	Assert( pPacketReturn );

	for( INT i=0; i<USER_ARRAY_SIZE; ++i )
	{ // ������������ҵ� GuildID ��ɾ��
		if( m_aGuildUser[i].m_UserGUID != INVALID_ID
		 && m_aGuildUser[i].m_bOnlineFlag
		 && m_aGuildUser[i].m_Position >= positon
		 )
		{
			pUser = g_pOnlineUser->FindUser(m_aGuildUser[i].m_UserGUID);
			if( pUser != NULL )
			{
				pServerPlayer = g_pServerManager->GetServerPlayer( pUser->GetServerID() );

				if( pServerPlayer != NULL )
				{
					pPacketReturn->SetPlayerID( pUser->GetPlayerID() );
					pServerPlayer->SendPacket( pPacket );
				}
			}
		}
	}
__LEAVE_FUNCTION
}

VOID Guild::AddGuildUser( GUILDUSER& GuildUser )
{
__ENTER_FUNCTION

	for( INT i=0; i<USER_ARRAY_SIZE; ++i, ++m_nUserMallocIndex )
	{
		if( m_nUserMallocIndex >= USER_ARRAY_SIZE )
		{
			m_nUserMallocIndex = 0;
		}

		if( m_aGuildUser[i].m_UserGUID == INVALID_ID )
		{
			m_aGuildUser[i].m_UserGUID	    = GuildUser.m_UserGUID;
			strncpy( m_aGuildUser[i].m_szUserName, GuildUser.m_szUserName, sizeof(GuildUser.m_szUserName) - 1 );
			m_aGuildUser[i].m_Position  	= GuildUser.m_Position;
			m_aGuildUser[i].m_uAccess   	= GuildUser.m_uAccess;
			m_aGuildUser[i].m_bOnlineFlag   = GuildUser.m_bOnlineFlag;
			m_aGuildUser[i].m_uLastLoginTime= GuildUser.m_uLastLoginTime;
            m_aGuildUser[i].m_uLevel        = GuildUser.m_uLevel;
            m_aGuildUser[i].m_cSex          = GuildUser.m_cSex;
            m_aGuildUser[i].m_bMenPaiID     = GuildUser.m_bMenPaiID;
			++m_nUserMallocIndex;
			break;
		}
	}

__LEAVE_FUNCTION
}

GUILDUSER* Guild::ILGetGuildUser( GUID_t userGUID )
{
__ENTER_FUNCTION

	for( INT i=0; i<USER_ARRAY_SIZE; ++i )
	{
		if( m_aGuildUser[i].m_UserGUID == userGUID )
		{
			return &(m_aGuildUser[i]);
		}
	}
	return NULL;

__LEAVE_FUNCTION

	return NULL;
}

// ͨ��������ð���
GUILDUSER*	Guild::GetGuildUserByIndex( INT nIndex)
{
__ENTER_FUNCTION
	if(nIndex >= 0 && nIndex < USER_ARRAY_SIZE)
	{
		return &(m_aGuildUser[nIndex]);
	}
	return NULL;
__LEAVE_FUNCTION
	return NULL;
}

//����ְλ��Ȩ�޲����ж�����
BOOL	Guild::CanDoIt(OPT_TYPE Opt, GUILD_POSITION position, GUILD_AUTHORITY authority)
{
	__ENTER_FUNCTION

	switch(Opt)
	{
	case OPT_NONE:
		{
			Assert(0);
		}
		break;
	case OPT_ASK_MEMBER_LIST:
		{
			if(position > GUILD_POSITION_TRAINEE)
				return TRUE;
			else
				return FALSE;
		}
		break;
	case OPT_ASK_DETAIL_INFO:
		{
			if(position > GUILD_POSITION_TRAINEE)
				return TRUE;
			else
				return FALSE;
		}
		break;
	case OPT_ASK_APPOINT_POS_INFO:
		{
			return TRUE;
		}
		break;

	case OPT_ASK_SELF_INFO:
		{
			if(position > GUILD_POSITION_TRAINEE)
				return TRUE;
			else
                return FALSE; 
		}
	default:
		Assert(0);
		break;
	}
	return FALSE;

	__LEAVE_FUNCTION
	return FALSE;

}

//�Ƿ��ܱ���������
BOOL Guild::CanAppointedBy(GUILD_POSITION position, const GUILDUSER* pUser)
{
__ENTER_FUNCTION

	if(pUser->m_uAccess&GUILD_AUTHORITY_ASSIGN)
	{//������Ȩ��
		if(pUser->m_Position>position)
		{//���ڴ�ְλ
			return	TRUE;
		}
	}
	return FALSE;

__LEAVE_FUNCTION
}

//��ô˰���е�ǰְλ������
CHAR*	Guild::GetPosName(INT iPosition)
{
__ENTER_FUNCTION

	switch(iPosition)
	{
	case GUILD_POSITION_ELITE_MEMBER:
		{
			return m_AdminLayOut.Get(GUILD_ADMIN_t::POS_ELITE_MEMBER)->szPosName;
		}
		break;

	case GUILD_POSITION_FAMILY_BOSS:
		{
			return m_AdminLayOut.Get(GUILD_ADMIN_t::POS_NORMAL_FAMILY_BOSS)->szPosName;
		}
		break;

	case GUILD_POSITION_ASS_CHIEFTAIN:
		{
			return m_AdminLayOut.Get(GUILD_ADMIN_t::POS_ASS_CHAIR_MAN)->szPosName;
		}
		break;

	case GUILD_POSITION_CHIEFTAIN:
		{
			return m_AdminLayOut.Get(GUILD_ADMIN_t::POS_CHAIR_MAN)->szPosName;
		}
		break;

	case GUILD_POSITION_MEMBER:
		{
			return m_AdminLayOut.Get(GUILD_ADMIN_t::POS_MEMBER)->szPosName;
		}
		break;
	}
__LEAVE_FUNCTION
	return NULL;
}

INT	Guild::ILGetGuildUserIndex( GUID_t userGUID )
{
__ENTER_FUNCTION

	for( INT i=0; i<USER_ARRAY_SIZE; ++i )
	{
		if( m_aGuildUser[i].m_UserGUID == userGUID )
		{
			return i;
		}
	}
__LEAVE_FUNCTION
	return -1;
}

VOID Guild::SetGuildMaxByLevel(INT nMaxUser, INT nMaxFamily, INT nMaxGoldFamilyUser)
{
    m_MaxUser               =   nMaxUser;         
    m_MaxFamily             =   nMaxFamily;       
    m_MaxGoldFamilyUser     =   nMaxGoldFamilyUser;
}
