
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

	m_GuildID = pInit->m_GuildID;                                                   //帮会ID
	memcpy( m_GuildName,pInit->m_GuildName, MAX_GUILD_NAME_SIZE);                   //帮会名称
	m_Status = pInit->m_Status;
	m_ChieftainGUID = pInit->m_ChieftainGUID;
	m_UserCount = pInit->m_UserCount;		                                        //帮会用户数量
	m_MaxUserSize = pInit->m_MaxUserSize;	                                        //帮会人口上限
	m_GuildPoint = pInit->m_GuildPoint;		                                        //帮会贡献点
	m_GuildMoney = pInit->m_GuildMoney;		                                        //帮会资金
	memcpy( m_aGuildUser, pInit->m_aGuildUser, sizeof(GUILDUSER)*USER_ARRAY_SIZE);  //帮会成员
    memcpy( &m_FamilyUser, &pInit->m_FamilyUser, sizeof(FAMILYUSER));               //家族信息
    //Todo: Amos 数据初始化
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
	
	//帮会行政结构初始化

	sprintf(m_AdminLayOut.Get(GUILD_ADMIN_t::POS_CHAIR_MAN)->szPosName, "帮主"); 
	m_AdminLayOut.Get(GUILD_ADMIN_t::POS_CHAIR_MAN)->MaxNumPos = 1;

	sprintf(m_AdminLayOut.Get(GUILD_ADMIN_t::POS_ASS_CHAIR_MAN)->szPosName, "副帮主"); 
	m_AdminLayOut.Get(GUILD_ADMIN_t::POS_ASS_CHAIR_MAN)->MaxNumPos = 3;

	sprintf(m_AdminLayOut.Get(GUILD_ADMIN_t::POS_NORMAL_FAMILY_BOSS)->szPosName, "家族长"); 
	m_AdminLayOut.Get(GUILD_ADMIN_t::POS_NORMAL_FAMILY_BOSS)->MaxNumPos = 3;

    sprintf(m_AdminLayOut.Get(GUILD_ADMIN_t::POS_ELITE_MEMBER)->szPosName, "精英帮众"); 

	sprintf(m_AdminLayOut.Get(GUILD_ADMIN_t::POS_MEMBER)->szPosName, "普通帮众"); 


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

	m_GuildID = m_pGuildSmu->m_GuildSM.m_GuildID; //帮会ID
	memcpy( m_GuildName,m_pGuildSmu->m_GuildSM.m_GuildName, MAX_GUILD_NAME_SIZE); //帮会名称
	memcpy( m_GuildDesc,m_pGuildSmu->m_GuildSM.m_GuildDesc, MAX_GUILD_DESC_SIZE); //帮会宗旨
	m_Status = m_pGuildSmu->m_GuildSM.m_Status;
	m_ChieftainGUID = m_pGuildSmu->m_GuildSM.m_ChieftainGUID;
	m_UserCount = m_pGuildSmu->m_GuildSM.m_UserCount; //帮会用户数量
	m_MaxUserSize = m_pGuildSmu->m_GuildSM.m_MaxUserSize; //帮会人口上限
	m_GuildPoint = m_pGuildSmu->m_GuildSM.m_GuildPoint; //帮会贡献点
	m_GuildMoney = m_pGuildSmu->m_GuildSM.m_GuildMoney; //帮会资金
	m_Longevity	=	m_pGuildSmu->m_GuildSM.m_Longevity;					//资历
	m_Contribute	=	m_pGuildSmu->m_GuildSM.m_Contribute;			//贡献度
	m_Honor	=	m_pGuildSmu->m_GuildSM.m_Honor;							//人气

    //家族信息
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
		m_aGuildUser[i].m_uLevel	=	m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_uLevel;							//人物级别
		m_aGuildUser[i].m_bMenPaiID	=	m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_bMenPaiID;						    //门派
		m_aGuildUser[i].m_iCurContribute	=	m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_iCurContribute;			//贡献
		m_aGuildUser[i].m_iMaxContribute	=	m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_iMaxContribute;			//最大贡献
		m_aGuildUser[i].m_iJoinTime	=	m_pGuildSmu->m_GuildSM.m_aGuildUser[i].m_iJoinTime;						    //加入时间
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
	memcpy( m_pGuildSmu->m_GuildSM.m_GuildName,m_GuildName, MAX_GUILD_NAME_SIZE); //帮会名称
	memcpy( m_pGuildSmu->m_GuildSM.m_GuildDesc,m_GuildDesc, MAX_GUILD_DESC_SIZE); //帮会宗旨
	m_pGuildSmu->m_GuildSM.m_Status = m_Status;
	m_pGuildSmu->m_GuildSM.m_ChieftainGUID = m_ChieftainGUID  ;
	m_pGuildSmu->m_GuildSM.m_UserCount = m_UserCount  ;					//帮会用户数量
	m_pGuildSmu->m_GuildSM.m_MaxUserSize = m_MaxUserSize;				//帮会人口上限
	m_pGuildSmu->m_GuildSM.m_GuildPoint = m_GuildPoint;					//帮会贡献点
	m_pGuildSmu->m_GuildSM.m_GuildMoney = m_GuildMoney;					//帮会资金
	m_pGuildSmu->m_GuildSM.m_Longevity	=	m_Longevity;				//资历
	m_pGuildSmu->m_GuildSM.m_Contribute	=	m_Contribute;				//贡献度
	m_pGuildSmu->m_GuildSM.m_Honor		=	m_Honor;					//人气

	m_pGuildSmu->m_GuildSM.m_GuildMoney	=	m_GuildMoney;				//帮会资金
	m_pGuildSmu->m_GuildSM.m_AdminLayout=	m_AdminLayOut;				//行政管理图

    //家族信息
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
	m_Longevity		=	0;			//资历
	m_Contribute	=	0;			//贡献度
	m_Honor			=	0;			//人气


	m_AdminLayOut.CleanUp();

__LEAVE_FUNCTION
}

BOOL Guild::HeartBeat( UINT uTime )
{ // 对未建成帮会进行检查，是否人数已满、是否时间到人数不足需要撤销申请等等
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

// 取得帮众
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

// 判断是否有某项权限
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

// 创建帮会，正式成立
BOOL Guild::OnGuildCreate( const USER* pCreater, const CHAR* szName )
{
__ENTER_FUNCTION

	if( pCreater == NULL )
	{
		Assert(pCreater);
		return FALSE;
	}
	m_Status = GUILD_STATUS_NORMAL;
	strncpy( m_GuildName, szName, sizeof(m_GuildName)-1 ); //帮会名称
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

    //Todo:自动生成黄金家族  策划文档待补完
    //m_FamilyUser.Add(0, 0);

	//自动升为帮主
	OnUserPositionChange(m_aGuildUser[0].m_UserGUID, GUILD_POSITION_CHIEFTAIN);

	m_UserCount = 1;

	g_pGuildManager->RegisterGuildName( this );

	OnGuildFound();

	ValidateShareMem();

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// 帮会创建失败
VOID Guild::OnGuildCreateFailed()
{
__ENTER_FUNCTION

	AssertEx( FALSE, "A guild create failed(Skip it)." );

	ValidateShareMem();

__LEAVE_FUNCTION
}

// 帮会成立，创建的帮会满足成立条件时
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

	// 消息广播出去
	WGGuildReturn Msg;


	_GUILD_RETURN GuildReturn;
	GuildReturn.m_ReturnType = GUILD_RETURN_FOUND;
	Msg.SetGuildReturn( &GuildReturn );

	BroadCast_Return( &Msg );

	ValidateShareMem();

__LEAVE_FUNCTION
}


BOOL Guild::OnGuildDestroy( GUID_t createrGUID )//销毁帮会
{
__ENTER_FUNCTION
	if( createrGUID != m_ChieftainGUID )
	{
		return FALSE;
	}

	USER* pUser;

	for( INT i=0; i<USER_ARRAY_SIZE; ++i )
	{ // 将所有在线玩家的 GuildID 都删掉
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
				sprintf(szMailContent, "你的帮会已经在江湖上除名。");
				g_pMailCenter->SendNormalMail( m_aGuildUser[i].m_szUserName, szMailContent);

				//发一封可执行邮件
				//在下次次玩家上线时会由Server执行此邮件对应的脚本，这里只是更改此人的工会ID
				g_pMailCenter->SendScriptMail( m_aGuildUser[i].m_szUserName,
					MAIL_UPDATE_ATTR, MAIL_ATTR_GUILD, INVALID_ID);
			}
		}
	}

	m_AdminLayOut.CleanUp();

	// 消息广播出去
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

// 0001	任免职务
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

// 0002	调整权限
VOID Guild::OnUserAuthorityChange( GUID_t userGUID, GUILD_AUTHORITY authority )
{
__ENTER_FUNCTION

	GUILDUSER* pGuildUser = ILGetGuildUser( userGUID );
	if( pGuildUser == NULL )
	{
		return;
	}

	if( IsAuthorized( userGUID, authority ) )
	{ // 夺权
		pGuildUser->m_uAccess &= ~((UINT)authority);
	}
	else
	{ // 授权
		pGuildUser->m_uAccess |= (UINT)authority;
	}

	ValidateShareMem();

__LEAVE_FUNCTION
}

// 0002	调整权限
VOID Guild::OnPositionAuthorityChange( GUILD_POSITION position, GUILD_AUTHORITY authority )
{
}

// 申请入帮，或者响应一个帮会
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
                // 贵帮的申请人数已满，请及时处理！
                CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                sprintf(szMailContent, "贵帮的申请人数已满，请及时处理！" );

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

// 邀请加入帮会
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

// 取得家族编号
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

// 设置帮众贡献度   
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

// 取得帮众贡献度   
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

// 设置帮会总贡献度   
VOID Guild::SetTotalGuildPoint(INT nGP)
{
    __ENTER_FUNCTION

        m_TotalGuildPoint   =   nGP;

    __LEAVE_FUNCTION
}

// 取得帮会总贡献度   
INT Guild::GetTotalGuildPoint()
{
    __ENTER_FUNCTION

        return m_TotalGuildPoint;

    __LEAVE_FUNCTION
        return 0;
}

// 创建家族
INT Guild::OnCreateFamily(const GUID_t CreatorGuid,const INT iShaikhIndex, const GUID_t ShaikhGuid,
                          const CHAR* szFamilyName, const INT iFamilyNameSize,
                          const CHAR* szFamilyDesc, const INT iFamilyDescSize, CHAR &cFamilyID )
{
__ENTER_FUNCTION
    //非帮主不能创建
    if (CreatorGuid != m_aGuildUser[0].m_UserGUID)
        return FAMILYUSER::RET_UNKOWN;

    for (BYTE i=0; i<GUILD_FAMILY_SIZE; ++i)
    {//0号家族成员一定是家族长
        INT index = m_FamilyUser.m_aFamilyUser[i][0];
        if (INVALID_ID == index)
        {
            //该等级能不能再创建更多的家族
            if ((index + 1) > m_MaxFamily)
            {
                return FAMILYUSER::RET_POS_FULL;
            }

            cFamilyID = i;
            //添加家族成员
            if (m_aGuildUser[iShaikhIndex].m_UserGUID == ShaikhGuid)//校验家族长
            {
                INT ret = m_FamilyUser.Add(i, iShaikhIndex);
                SetFamilyID(iShaikhIndex, i);

                if (FAMILYUSER::RET_SUCCESS == ret)
                {
                    //更新名字和宗旨
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

// 删除家族成员
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

// 添加家族成员
INT Guild::OnAddFamilyUser(const GUID_t InvitorGuid, CHAR cFamilyIndex, INT iGuildIndex, GUID_t guid)
{
__ENTER_FUNCTION
    if ( IsAuthorized(InvitorGuid, GUILD_AUTHORITY_ADDFAMILYUSER) )
        return PACKET_EXE_CONTINUE;

    //校验GUID
    if (iGuildIndex < USER_ARRAY_SIZE)
    {
        if (m_aGuildUser[iGuildIndex].m_UserGUID == guid)
        {//通过,添加
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

// 删除家族成员
INT Guild::OnRemoveFamilyUser(const GUID_t BossGuid,CHAR cFamilyIndex, INT iGuildIndex, GUID_t guid)
{
__ENTER_FUNCTION
    if (IsAuthorized(BossGuid, GUILD_AUTHORITY_DELFAMILYUSER))
        PACKET_EXE_CONTINUE;

    //校验GUID
    if (iGuildIndex < USER_ARRAY_SIZE)
    {
        if (m_aGuildUser[iGuildIndex].m_UserGUID == guid)
        {//通过,del
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

// 获取家族成员信息列表
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

// 获取家族名字
const CHAR * Guild::GetFamilyName(CHAR cFamilyIndex)
{
__ENTER_FUNCTION

    return m_FamilyUser.GetFamilyName(cFamilyIndex);

__LEAVE_FUNCTION

    return NULL;
}

// 获取家族宗旨
const CHAR * Guild::GetFamilyDesc(CHAR cFamilyIndex)
{
__ENTER_FUNCTION

    return m_FamilyUser.GetFamilyDesc(cFamilyIndex);

__LEAVE_FUNCTION

    return NULL;
}

// 获取家族长名字
const CHAR * Guild::GetShaikhName(CHAR cFamilyIndex)
{
__ENTER_FUNCTION

    return m_FamilyUser.GetShaikhName(cFamilyIndex);

__LEAVE_FUNCTION
    return NULL;
}

// 取得家族数量
UINT Guild::GetFamilyCount()
{
__ENTER_FUNCTION

    return m_FamilyCount;

__LEAVE_FUNCTION

    return INVALID_ID;
}


// 0003 招收帮众
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

// 0004	开除帮众
// 0008	离开帮会
GUILD_ERROR_TYPE Guild::OnUserLeave( GUID_t userGUID )
{
__ENTER_FUNCTION

	for( INT i=0; i<USER_ARRAY_SIZE; ++i )
	{
		if( m_aGuildUser[i].m_UserGUID == userGUID )
		{
			if( m_aGuildUser[i].m_Position == GUILD_POSITION_TRAINEE )
			{ // 申请者离开
				--m_nProposerCount;
			}
			else
			{ // 正式帮众离开
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

// 开除帮众离开帮会
GUILD_ERROR_TYPE Guild::OnUserLeaveByIndex( INT iIndex )
{
	__ENTER_FUNCTION
	if( m_aGuildUser[iIndex].m_UserGUID != INVALID_ID )
	{
		if( m_aGuildUser[iIndex].m_Position == GUILD_POSITION_TRAINEE )
		{ // 申请者离开
			--m_nProposerCount;
		}
		else
		{ // 正式帮众离开
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


// 0005	禅让，和任免职务不同的是，禅让需要将现有职务的人挤下去
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

	// 过河拆桥
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

// 0006	支取金额
// 0007	存入金额
VOID Guild::OnGuildMoneyChange( GUID_t userGUID, INT nMoney )//帮会资金发生变化
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

VOID Guild::OnUserAccessChange( GUID_t userGUID, INT newAccess )//玩家权限变化
{
__ENTER_FUNCTION

	ValidateShareMem();

__LEAVE_FUNCTION
}

VOID Guild::OnGuildPointChange( GUID_t userGUID, INT nMoney )//帮会贡献点发生变化
{
__ENTER_FUNCTION

	ValidateShareMem();

__LEAVE_FUNCTION
}

// 帮会玩家上线
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

// 帮会玩家下线
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
	{ // 将所有在线玩家的 GuildID 都删掉
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

// 通过索引获得帮众
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

//根据职位，权限操作判定操作
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

//是否能被此人任命
BOOL Guild::CanAppointedBy(GUILD_POSITION position, const GUILDUSER* pUser)
{
__ENTER_FUNCTION

	if(pUser->m_uAccess&GUILD_AUTHORITY_ASSIGN)
	{//有任免权限
		if(pUser->m_Position>position)
		{//高于此职位
			return	TRUE;
		}
	}
	return FALSE;

__LEAVE_FUNCTION
}

//获得此帮会中当前职位的名字
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
