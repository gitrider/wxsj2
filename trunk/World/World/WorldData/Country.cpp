
#include "stdafx.h"
#include "Country.h"
#include "Packet.h"
#include "CountryManager.h"
#include "OnlineUser.h"
#include "Config.h"
#include "TimeManager.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "ShareMemManager.h"
#include "SMUManager.h"
#include "MailCenter.h"
#include "ServerManager.h"

#include "WGChat.h"
#include "ChatCenter.h"
#include "WGCountryReturn.h"



// Function name   : Country::Country
// Description     : 
// Return type     : 
Country::Country()
{
    m_nCurVoteNum           =    0;
    m_nCurAgreeNum          =    0;
    m_nStartTime            =    0;
    m_nTime                 =    0;

    m_nCurGuildMasterVote   =    0;
    memset(m_szCountryName, 0, MAX_CHARACTER_NAME);

    CleanUpDelate();
    VoteInfoCleanUp();
}



// Function name   : Country::~Country
// Description     : 
// Return type     : 
Country::~Country()
{

}


// Function name   : Country::Init
// Description     : 
// Return type     : 

VOID Country::Init()
{

}

// Function name   :   Country::HeartBeat
// Description     : 
// Return type     : BOOL  
// Argument        : DWORD dwTime=0

BOOL    Country::HeartBeat( DWORD dwTime )
{
    __ENTER_FUNCTION

        time_t t    = time(NULL);
        tm* pTm     = localtime(&t);
        tm* pTmOld  = localtime(&m_nTime);
        
        if (pTm->tm_wday != pTmOld->tm_wday)
        {
            if (1 == pTm->tm_wday && 0 == pTmOld->tm_wday)//每周一清空
            {
                CleanUpDelate();
            }
            m_nTime = t;
        }
        
        return VoteOnEnd(dwTime);

    __LEAVE_FUNCTION

        return FALSE;
}

// Function name   :   Country::AppointPosition
// Description     : 任免官职
// Return type     : VOID  
// Argument        : INT nPosition
// Argument        : CHAR *szName
VOID    Country::AppointPosition(INT nPosition, GUID_t guid, const CHAR *pName)
{
    __ENTER_FUNCTION

    m_CountryData.AppointPosition(nPosition, guid, pName);

    __LEAVE_FUNCTION
}

// Function name   :   GetCountryInfo
// Description     : 国家信息查询
// Return type     : VOID  
// Argument        : _COUNTRY &Info

VOID    Country::GetCountryInfo(_COUNTRY &Info)
{
    __ENTER_FUNCTION

    memcpy(&Info, &m_CountryData, sizeof(_COUNTRY));

    __LEAVE_FUNCTION
}



// Function name   :   Country::AddActionPoint
// Description     : 增加国王行动力
// Return type     : VOID  
// Argument        : INT nAP

VOID    Country::AddActionPoint(INT nAP)
{
    __ENTER_FUNCTION

    if ((m_CountryData.m_nAP+nAP) <= MAX_KINDOM_ACTION_POINT)
    {
        m_CountryData.m_nAP += nAP;
    }

    __LEAVE_FUNCTION
}


// Function name   :  Country::GetCountryName
// Description     : 
// Return type     : CHAR*  

CHAR*   Country::GetCountryName()
{
    __ENTER_FUNCTION

    return m_szCountryName;

    __LEAVE_FUNCTION
}

VOID    Country::SetCountryName(INT nCountryID)
{
    __ENTER_FUNCTION

        switch (m_CountryData.m_cCountryID)
        {
        case COUATTRIBUTE_XINYUE://新月汗国 
            sprintf(m_szCountryName, "新月汗国");
            break;
        case COUATTRIBUTE_DAMO://大漠汗国
            sprintf(m_szCountryName, "大漠汗国");
            break;
        case COUATTRIBUTE_JINZHANG://金帐汗国
            sprintf(m_szCountryName, "金帐汗国");
            break;
        case COUATTRIBUTE_XIYU://西域汗国
            sprintf(m_szCountryName, "西域汗国");
            break;
        }

    __LEAVE_FUNCTION
}


// Function name   :   Country::SubActionPoint
// Description     : 减少国王行动力
// Return type     : VOID  
// Argument        : INT nAP

VOID    Country::SubActionPoint(INT nAP)
{
    __ENTER_FUNCTION

    m_CountryData.m_nAP -= nAP;

    if (m_CountryData.m_nAP < 0)
        m_CountryData.m_nAP = 0;

    //开除国王
    if (0 == m_CountryData.m_nAP)
    {
        CHAR szName[MAX_CHARACTER_NAME] = {0};
        memcpy(szName, m_CountryData.m_szGuoWang, MAX_CHARACTER_NAME);

        m_CountryData.m_GuoWang = INVALID_GUID;
        memset(m_CountryData.m_szGuoWang, 0, MAX_CHARACTER_NAME);


        CHAR szCountryName[32] = {0};
        switch (m_CountryData.m_cCountryID)
        {
        case COUATTRIBUTE_XINYUE://新月汗国 
            sprintf(szCountryName, "新月汗国");
            break;
        case COUATTRIBUTE_DAMO://大漠汗国
            sprintf(szCountryName, "大漠汗国");
            break;
        case COUATTRIBUTE_JINZHANG://金帐汗国
            sprintf(szCountryName, "金帐汗国");
            break;
        case COUATTRIBUTE_XIYU://西域汗国
            sprintf(szCountryName, "西域汗国");
            break;
        }

        CHAR szContex[MAX_CHAT_SIZE] = {0};
        sprintf(szContex, "%s国王%s因国王行动力丧失殆尽，失去王位。", szCountryName, m_CountryData.m_szGuoWang);

        WGChat Msg ;
        Msg.SetChatType( CHAT_TYPE_COUNTRY ) ;
        Msg.SetContexSize( (BYTE)strlen(szContex) ) ;
        Msg.SetContex( szContex ) ;
        Msg.SetSourNameSize( (BYTE)strlen(szName) ) ;
        Msg.SetSourName( szName ) ;
        Msg.SetWorldChatID( g_pChatCenter->NextWorldChatID() ) ;

        //发送聊天消息
        Msg.SetCountryID( m_CountryData.m_cCountryID );
        g_pServerManager->BroadCastServer( &Msg ) ;

        //给前国王发右邮件
        CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
        sprintf(szMailContent, "你的国王行动力被减少到0，因此被贬为平民。不再拥有国王的荣威。", szName);
        g_pMailCenter->SendNormalMail(szName, szMailContent);

        //Todo:如果在线的话，还得通知客户端移植国家职位，离线的话只有等待强制邮件完成后再实现
        USER* pUser = g_pOnlineUser->FindUser( szName );
        if (pUser)
        {
            ID_t ServerID = g_Config.SceneID2ServerID(pUser->GetSceneID());
            ServerPlayer *pSPlayer = g_pServerManager->GetServerPlayer(ServerID);

            WGCountryReturn Msg;
            Msg.SetPlayerID( pUser->GetPlayerID() );
            _COUNTRY_RETURN CountryReturn;
            CountryReturn.m_ReturnType        = COUNTRY_RETURN_REMOVE;
            CountryReturn.m_cPosition         = _COUNTRY::COUNTRY_GUOWANG;
            CountryReturn.m_cCountry          = (CHAR)m_CountryData.m_cCountryID;
            Msg.SetCountryReturn( &CountryReturn );

            pSPlayer->SendPacket( &Msg );
        }
    }

    __LEAVE_FUNCTION
}


// Function name   :   Country::RemoveKindom
// Description     : 罢免国王
// Return type     : VOID  

VOID    Country::RemoveKindom()
{
    USER* pUser = g_pOnlineUser->FindUser( m_CountryData.m_GuoWang );
    if (pUser)
    {
        ID_t ServerID = g_Config.SceneID2ServerID(pUser->GetSceneID());
        ServerPlayer *pSPlayer = g_pServerManager->GetServerPlayer(ServerID);

        WGCountryReturn Msg;
        Msg.SetPlayerID( pUser->GetPlayerID() );
        _COUNTRY_RETURN CountryReturn;
        CountryReturn.m_ReturnType        = COUNTRY_RETURN_REMOVE;
        CountryReturn.m_cPosition         = _COUNTRY::COUNTRY_GUOWANG;
        CountryReturn.m_cCountry          = (CHAR)m_CountryData.m_cCountryID;
        Msg.SetCountryReturn( &CountryReturn );

        pSPlayer->SendPacket( &Msg );

        // 罢免国王
        m_CountryData.m_GuoWang = INVALID_GUID;
        memset(m_CountryData.m_szGuoWang, 0, MAX_CHARACTER_NAME);
        m_CountryData.m_nAP     = 0;
    }
}


// Function name   :    Country::GetActionPoint
// Description     : 获取国王行动力
// Return type     : INT  

INT     Country::GetActionPoint()
{
    __ENTER_FUNCTION

    return m_CountryData.m_nAP;

    __LEAVE_FUNCTION

    return 0;
}


// Function name   :   Country::VoteOnBegin
// Description     : 
// Return type     : VOID  

VOID    Country::VoteOnBegin()
{
    __ENTER_FUNCTION

        m_nStartTime    =   g_pTimeManager->CurrentTime();

    __LEAVE_FUNCTION
}

BOOL    Country::VoteOnEnd(DWORD dwTime)
{
    __ENTER_FUNCTION

        // 这里时间需要重取
        dwTime = g_pTimeManager->CurrentTime();
        if (m_nStartTime != 0)
        {
            if (dwTime - m_nStartTime > 4*3600*1000)
            {
                return TRUE;
            }
        }
        return FALSE;

    __LEAVE_FUNCTION

        return FALSE;
}

// Function name   :   Country::VoteOn
// Description     : 弹劾投票
// Return type     : VOID  
// Argument        : BOOL bResult
// Argument        : GUID_t guid

BOOL    Country::VoteOn(BOOL bResult, GUID_t guid)
{
    __ENTER_FUNCTION

        if (m_nCurVoteNum >= MAX_VOTE_USER)
            return FALSE;

        // 不能重复投票
        for (INT i=0; i<m_nCurVoteNum; ++i)
        {
            if (m_Vote[i].m_Guid == guid )
            {
                //邮件
                USER* pUser = g_pOnlineUser->FindUser( guid );
                if (pUser)
                {
                    CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                    sprintf(szMailContent, "你已经投过一票，无法继续投票。", pUser->GetName());
                    g_pMailCenter->SendNormalMail(pUser->GetName(), szMailContent);
                }
                return FALSE;
            }
        }

       
        if (INVALID_GUID == m_Vote[m_nCurVoteNum].m_Guid)
        {
            m_Vote[m_nCurVoteNum].m_bResult     =   bResult;
            m_Vote[m_nCurVoteNum].m_Guid        =   guid;
            ++m_nCurVoteNum;

            if (bResult)
                ++m_nCurAgreeNum;
        }
        return TRUE;

    __LEAVE_FUNCTION

        return FALSE;
}


// Function name   :   Country::VoteInfoCleanUp
// Description     : 
// Return type     : VOID  

VOID    Country::VoteInfoCleanUp()
{
    __ENTER_FUNCTION

        for (INT i=0; i<MAX_VOTE_USER; ++i)
        {
            m_Vote[i].m_bResult =   FALSE;
            m_Vote[i].m_Guid    =   INVALID_GUID;
        }

        m_nCurVoteNum = 0;
        m_nCurAgreeNum= 0;
        m_nStartTime  = 0;

    __LEAVE_FUNCTION
}



// Function name   :   Country::CleanUpDelate
// Description     : 一周清空一次
// Return type     : VOID  

VOID    Country::CleanUpDelate()
{
    __ENTER_FUNCTION

        for (INT i=0; i<MAX_GUILD_MASTER_VOTE; ++i)
        {
            m_GuildMasterVote[i].m_bResult  =   FALSE;
            m_GuildMasterVote[i].m_Guid     =   INVALID_GUID;
        }
        m_nCurGuildMasterVote = 0;

    __LEAVE_FUNCTION
}


// Function name   :   Country::GetVoteResult
// Description     : 投票结果
// Return type     : VOID  
// Argument        : INT& nAgreeCnt
// Argument        : INT& nPercent

VOID    Country::GetVoteResult(INT& nCurVoteCnt, INT& nAgreeCnt, INT& nPercent)
{
    __ENTER_FUNCTION

        nCurVoteCnt =   m_nCurVoteNum;
        nAgreeCnt   =   m_nCurAgreeNum;

        if (m_nCurVoteNum > 0)
        {
            nPercent    =   m_nCurAgreeNum / m_nCurVoteNum * 100;
        }
        else
            nPercent    =   0;


        if (nPercent >= 66 && nCurVoteCnt >= 100)
        {
            //TODO: 弹劾成功，参与联名弹劾的帮主返回游戏币60J.
            //系统信件：弹劾国王PlayerName成功，返回弹劾经费60J

            for (INT i=m_nCurGuildMasterVote-1; i<m_nCurGuildMasterVote-3 ; --i)
            {
                GUID_t guid = m_GuildMasterVote[i].m_Guid;

                USER* pUser = g_pOnlineUser->FindUser( guid );
                if (pUser)
                {
                    ID_t ServerID = g_Config.SceneID2ServerID(pUser->GetSceneID());
                    ServerPlayer *pSPlayer = g_pServerManager->GetServerPlayer(ServerID);

                    WGCountryReturn Msg;
                    Msg.SetPlayerID( pUser->GetPlayerID() );
                    _COUNTRY_RETURN CountryReturn;
                    CountryReturn.m_ReturnType        = COUNTRY_RETURN_DELATE_SCUESS;
                    Msg.SetCountryReturn( &CountryReturn );

                    pSPlayer->SendPacket( &Msg );
                }
            }
        }

    __LEAVE_FUNCTION
}

// Function name   :    Country::Delate
// Description     : 发起弹劾
// Return type     : INT  
// Argument        : GUID_t guid

INT     Country::Delate(GUID_t guid)
{

    __ENTER_FUNCTION
        if (m_nStartTime != 0)
            return RET_FAIL;
            

        for (INT i=0; i<MAX_GUILD_MASTER_VOTE; ++i)
        {
            if (m_GuildMasterVote[i].m_Guid == guid)
                return  RET_REPEAT;
        }


        for (INT i=0; i<MAX_GUILD_MASTER_VOTE; ++i)
        {
            if (INVALID_GUID == m_GuildMasterVote[i].m_Guid )
            {
                m_GuildMasterVote[i].m_bResult  =   TRUE;
                m_GuildMasterVote[i].m_Guid     =   guid;
                ++m_nCurGuildMasterVote;

                return ((i+1)%(g_Config.m_ConfigInfo.m_nDelateNum) == 0)?RET_OK:RET_SCUESS;
            }
        }
        return  RET_FULL;


    __LEAVE_FUNCTION

        return RET_FAIL;
}
