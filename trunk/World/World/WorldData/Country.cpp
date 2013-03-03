
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
            if (1 == pTm->tm_wday && 0 == pTmOld->tm_wday)//ÿ��һ���
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
// Description     : �����ְ
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
// Description     : ������Ϣ��ѯ
// Return type     : VOID  
// Argument        : _COUNTRY &Info

VOID    Country::GetCountryInfo(_COUNTRY &Info)
{
    __ENTER_FUNCTION

    memcpy(&Info, &m_CountryData, sizeof(_COUNTRY));

    __LEAVE_FUNCTION
}



// Function name   :   Country::AddActionPoint
// Description     : ���ӹ����ж���
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
        case COUATTRIBUTE_XINYUE://���º��� 
            sprintf(m_szCountryName, "���º���");
            break;
        case COUATTRIBUTE_DAMO://��Į����
            sprintf(m_szCountryName, "��Į����");
            break;
        case COUATTRIBUTE_JINZHANG://���ʺ���
            sprintf(m_szCountryName, "���ʺ���");
            break;
        case COUATTRIBUTE_XIYU://���򺹹�
            sprintf(m_szCountryName, "���򺹹�");
            break;
        }

    __LEAVE_FUNCTION
}


// Function name   :   Country::SubActionPoint
// Description     : ���ٹ����ж���
// Return type     : VOID  
// Argument        : INT nAP

VOID    Country::SubActionPoint(INT nAP)
{
    __ENTER_FUNCTION

    m_CountryData.m_nAP -= nAP;

    if (m_CountryData.m_nAP < 0)
        m_CountryData.m_nAP = 0;

    //��������
    if (0 == m_CountryData.m_nAP)
    {
        CHAR szName[MAX_CHARACTER_NAME] = {0};
        memcpy(szName, m_CountryData.m_szGuoWang, MAX_CHARACTER_NAME);

        m_CountryData.m_GuoWang = INVALID_GUID;
        memset(m_CountryData.m_szGuoWang, 0, MAX_CHARACTER_NAME);


        CHAR szCountryName[32] = {0};
        switch (m_CountryData.m_cCountryID)
        {
        case COUATTRIBUTE_XINYUE://���º��� 
            sprintf(szCountryName, "���º���");
            break;
        case COUATTRIBUTE_DAMO://��Į����
            sprintf(szCountryName, "��Į����");
            break;
        case COUATTRIBUTE_JINZHANG://���ʺ���
            sprintf(szCountryName, "���ʺ���");
            break;
        case COUATTRIBUTE_XIYU://���򺹹�
            sprintf(szCountryName, "���򺹹�");
            break;
        }

        CHAR szContex[MAX_CHAT_SIZE] = {0};
        sprintf(szContex, "%s����%s������ж���ɥʧ������ʧȥ��λ��", szCountryName, m_CountryData.m_szGuoWang);

        WGChat Msg ;
        Msg.SetChatType( CHAT_TYPE_COUNTRY ) ;
        Msg.SetContexSize( (BYTE)strlen(szContex) ) ;
        Msg.SetContex( szContex ) ;
        Msg.SetSourNameSize( (BYTE)strlen(szName) ) ;
        Msg.SetSourName( szName ) ;
        Msg.SetWorldChatID( g_pChatCenter->NextWorldChatID() ) ;

        //����������Ϣ
        Msg.SetCountryID( m_CountryData.m_cCountryID );
        g_pServerManager->BroadCastServer( &Msg ) ;

        //��ǰ���������ʼ�
        CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
        sprintf(szMailContent, "��Ĺ����ж��������ٵ�0����˱���Ϊƽ�񡣲���ӵ�й�����������", szName);
        g_pMailCenter->SendNormalMail(szName, szMailContent);

        //Todo:������ߵĻ�������֪ͨ�ͻ�����ֲ����ְλ�����ߵĻ�ֻ�еȴ�ǿ���ʼ���ɺ���ʵ��
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
// Description     : �������
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

        // �������
        m_CountryData.m_GuoWang = INVALID_GUID;
        memset(m_CountryData.m_szGuoWang, 0, MAX_CHARACTER_NAME);
        m_CountryData.m_nAP     = 0;
    }
}


// Function name   :    Country::GetActionPoint
// Description     : ��ȡ�����ж���
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

        // ����ʱ����Ҫ��ȡ
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
// Description     : ����ͶƱ
// Return type     : VOID  
// Argument        : BOOL bResult
// Argument        : GUID_t guid

BOOL    Country::VoteOn(BOOL bResult, GUID_t guid)
{
    __ENTER_FUNCTION

        if (m_nCurVoteNum >= MAX_VOTE_USER)
            return FALSE;

        // �����ظ�ͶƱ
        for (INT i=0; i<m_nCurVoteNum; ++i)
        {
            if (m_Vote[i].m_Guid == guid )
            {
                //�ʼ�
                USER* pUser = g_pOnlineUser->FindUser( guid );
                if (pUser)
                {
                    CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                    sprintf(szMailContent, "���Ѿ�Ͷ��һƱ���޷�����ͶƱ��", pUser->GetName());
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
// Description     : һ�����һ��
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
// Description     : ͶƱ���
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
            //TODO: �����ɹ����������������İ���������Ϸ��60J.
            //ϵͳ�ż�����������PlayerName�ɹ������ص�������60J

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
// Description     : ������
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
