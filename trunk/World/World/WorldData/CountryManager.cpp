#include "stdafx.h"
#include "CountryManager.h"
#include "OnlineUser.h"
#include "Config.h"
#include "City.h"
#include "TimeManager.h"
#include "MailCenter.h"
#include "ServerManager.h"

#include "WGChat.h"
#include "ChatCenter.h"
#include "WGCountryReturn.h"



CountryManager* g_pCountryManager=NULL;

#define AP_ONE_DAY  10

// Function name   : CountryManager::CountryManager
// Description     : 
// Return type     : 

CountryManager::CountryManager()
{
    __ENTER_FUNCTION

        m_mDay = -1;

    __LEAVE_FUNCTION
}


// Function name   : CountryManager::~CountryManager
// Description     : 
// Return type     : 

CountryManager::~CountryManager()
{
    __ENTER_FUNCTION

    __LEAVE_FUNCTION
}


// Function name   : CountryManager::Init
// Description     : 
// Return type     : BOOL 

BOOL CountryManager::Init( )
{
    __ENTER_FUNCTION

        //Todo:�ȴ����ݿ���ɣ�����ɳ�ʼ��
        m_XinYue.GetCountryInfo()->m_cCountryID     =   COUATTRIBUTE_XINYUE;
        m_DaMo.GetCountryInfo()->m_cCountryID       =   COUATTRIBUTE_DAMO;
        m_JinZhang.GetCountryInfo()->m_cCountryID   =   COUATTRIBUTE_JINZHANG;
        m_XiYu.GetCountryInfo()->m_cCountryID       =   COUATTRIBUTE_XIYU;

        m_XinYue.SetCountryName(COUATTRIBUTE_XINYUE);
        m_DaMo.SetCountryName(COUATTRIBUTE_DAMO);
        m_JinZhang.SetCountryName(COUATTRIBUTE_JINZHANG);
        m_XiYu.SetCountryName(COUATTRIBUTE_XIYU);

        return TRUE;

    __LEAVE_FUNCTION

        return FALSE;
}


// Function name   :   CountryManager::HeartBeat
// Description     : �߼�֡
// Return type     : BOOL  
// Argument        : DWORD dwTime=0

BOOL    CountryManager::HeartBeat( DWORD dwTime )
{
    __ENTER_FUNCTION

    time_t t = time(NULL);
    tm* pTm = localtime(&t);

    if (pTm)
    {
        //�µ�һ�죬�й����ͼ������ж���
        if (pTm->tm_mday != m_mDay)
        {
            if ( INVALID_GUID != m_XinYue.GetCountryInfo()->m_GuoWang)
            {
                m_XinYue.SubActionPoint(AP_ONE_DAY);
            }

            if ( INVALID_GUID != m_DaMo.GetCountryInfo()->m_GuoWang)
            {
                m_DaMo.SubActionPoint(AP_ONE_DAY);
            }

            if ( INVALID_GUID != m_JinZhang.GetCountryInfo()->m_GuoWang)
            {
                m_JinZhang.SubActionPoint(AP_ONE_DAY);
            }

            if ( INVALID_GUID != m_XiYu.GetCountryInfo()->m_GuoWang)
            {
                m_XiYu.SubActionPoint(AP_ONE_DAY);
            }

            m_mDay = pTm->tm_mday;
        }
    }

    // ͶƱͳ��
    if (m_XinYue.HeartBeat(dwTime))
    {
        BroadcastVoteResult(&m_XinYue);
    }

    if (m_DaMo.HeartBeat(dwTime))
    {
        BroadcastVoteResult(&m_DaMo);
    }

    if (m_JinZhang.HeartBeat(dwTime))
    {
        BroadcastVoteResult(&m_JinZhang);
    }

    if (m_XiYu.HeartBeat(dwTime))
    {
        BroadcastVoteResult(&m_XiYu);
    }

    return TRUE;
    __LEAVE_FUNCTION
    return FALSE;
}

// Function name   : CountryManager::AppointPosition
// Description     : 
// Return type     : 
// Argument        : INT nPosition
// Argument        : CHAR *szName

VOID CountryManager::AppointPosition(INT nCountryID, INT nPosition, GUID_t guid, const CHAR *pName)
{
    __ENTER_FUNCTION

        switch (nCountryID)
    {
        case COUATTRIBUTE_XINYUE://���º��� 
            m_XinYue.AppointPosition(nPosition, guid, pName);
            break;
        case COUATTRIBUTE_DAMO://��Į����
            m_DaMo.AppointPosition(nPosition, guid, pName);
            break;
        case COUATTRIBUTE_JINZHANG://���ʺ���
            m_JinZhang.AppointPosition(nPosition, guid, pName);
            break;
        case COUATTRIBUTE_XIYU://���򺹹�
            m_XiYu.AppointPosition(nPosition, guid, pName);
            break;
    };

    __LEAVE_FUNCTION
}


// Function name   :   CountryManager::GetCountryInfo
// Description     : ������Ϣ��ѯ
// Return type     : VOID  
// Argument        : INT nCountryID

VOID    CountryManager::GetCountryInfo(INT nCountryID, _COUNTRY &Info)
{
    __ENTER_FUNCTION

        switch (nCountryID)
    {
        case COUATTRIBUTE_XINYUE://���º��� 
            m_XinYue.GetCountryInfo(Info);
            break;
        case COUATTRIBUTE_DAMO://��Į����
            m_DaMo.GetCountryInfo(Info);
            break;
        case COUATTRIBUTE_JINZHANG://���ʺ���
            m_JinZhang.GetCountryInfo(Info);
            break;
        case COUATTRIBUTE_XIYU://���򺹹�
            m_XiYu.GetCountryInfo(Info);
            break;
    };

    __LEAVE_FUNCTION
}


// Function name   :   CountryManager::GetCountryInfo
// Description     : 
// Return type     : Country*  
// Argument        : INT nCountryID

Country*    CountryManager::GetCountry(INT nCountryID)
{
    __ENTER_FUNCTION

        switch (nCountryID)
    {
        case COUATTRIBUTE_XINYUE://���º��� 
            return &m_XinYue;
        case COUATTRIBUTE_DAMO://��Į����
            return &m_DaMo;
        case COUATTRIBUTE_JINZHANG://���ʺ���
            return &m_JinZhang;
        case COUATTRIBUTE_XIYU://���򺹹�
            return &m_XiYu;
    };

    __LEAVE_FUNCTION
        return NULL;
}


// Function name   :   CountryManager::GetCountryInfo
// Description     : ������Ϣ��ѯ
// Return type     : Country*  
// Argument        : INT nCountryID

_COUNTRY*    CountryManager::GetCountryInfo(INT nCountryID)
{
    __ENTER_FUNCTION

    switch (nCountryID)
    {
        case COUATTRIBUTE_XINYUE://���º��� 
            return m_XinYue.GetCountryInfo();
        case COUATTRIBUTE_DAMO://��Į����
            return m_DaMo.GetCountryInfo();
        case COUATTRIBUTE_JINZHANG://���ʺ���
            return m_JinZhang.GetCountryInfo();
        case COUATTRIBUTE_XIYU://���򺹹�
            return m_XiYu.GetCountryInfo()  ;
    };

    __LEAVE_FUNCTION
        return NULL;
}



// Function name   :   AddCountryUser
// Description     : ������ߵĹ��ҳ�Ա
// Return type     : BOOL  
// Argument        : COUNTRY_ATTRIBUTE countryID
// Argument        : GUID_t guid

BOOL    CountryManager::AddCountryUser(COUNTRY_ATTRIBUTE countryID, const GUID_t   guid )
{
    __ENTER_FUNCTION

    _COUNTRY *pCountry = GetCountryInfo(countryID);
    Assert(pCountry);
    pCountry->AddUser(guid);

    return TRUE;
    __LEAVE_FUNCTION
    return FALSE;
}


// Function name   :   RemoveCountryUser
// Description     : �Ƴ����ߵĹ��ҳ�Ա
// Return type     : VOID  
// Argument        : COUNTRY_ATTRIBUTE countryID
// Argument        : GUID_t guid

VOID    CountryManager::RemoveCountryUser(COUNTRY_ATTRIBUTE countryID,const GUID_t   guid )
{
    __ENTER_FUNCTION

    _COUNTRY *pCountry = GetCountryInfo(countryID);
    Assert(pCountry);
    pCountry->RemoveUser(guid);

    __LEAVE_FUNCTION
}



// Function name   :   CountryManager::AddActionPoint
// Description     : ���ӹ����ж���
// Return type     : VOID  
// Argument        : COUNTRY_ATTRIBUTE countryID
// Argument        : INT nAP

VOID    CountryManager::AddActionPoint(COUNTRY_ATTRIBUTE countryID, INT nAP)
{
    __ENTER_FUNCTION

        Country *pCountry = GetCountry(countryID);
        Assert(pCountry);
        pCountry->AddActionPoint(nAP);

    __LEAVE_FUNCTION
}


// Function name   :   CountryManager::SubActionPoint
// Description     : 
// Return type     : VOID  
// Argument        : COUNTRY_ATTRIBUTE countryID
// Argument        : INT nAP

VOID    CountryManager::SubActionPoint(COUNTRY_ATTRIBUTE countryID, INT nAP)
{
    __ENTER_FUNCTION

        Country *pCountry = GetCountry(countryID);
        Assert(pCountry);
        pCountry->SubActionPoint(nAP);

    __LEAVE_FUNCTION
}


// Function name   :    CountryManager::GetActionPoint
// Description     : 
// Return type     : INT  
// Argument        : COUNTRY_ATTRIBUTE countryID

INT     CountryManager::GetActionPoint(COUNTRY_ATTRIBUTE countryID)
{
    __ENTER_FUNCTION

        Country *pCountry = GetCountry(countryID);
        Assert(pCountry);
        return pCountry->GetActionPoint();

    __LEAVE_FUNCTION   
        return 0;
}


// Function name   :   CountryManager::BroadcastVoteResult
// Description     : �㲥����ͶƱ���
// Return type     : VOID  

VOID    CountryManager::BroadcastVoteResult(Country *pCountry, BOOL bDuring)
{
    __ENTER_FUNCTION

    INT nCurVoteCnt = 0, nAgreeCnt = 0, nPercent = 0;
    pCountry->GetVoteResult(nCurVoteCnt, nAgreeCnt, nPercent);
    INT nID = pCountry->GetCountryInfo()->m_cCountryID;

    //1.������������100�ˣ�ͶƱ�����Ч
    //2.�����ʸ���66%�������ɹ�������ͶƱ
    //- �����ʲ�����66%��һֱͶƱ��ֱ��ʱ�����
    if (nCurVoteCnt >= g_Config.m_ConfigInfo.m_nVoteNum)
    {
        if (nPercent >= 66)
        {
            CHAR szContex[MAX_CHAT_SIZE] = {0};
            sprintf(szContex, "%s���ҹ���%s��ȫ�������ɹ����������ġ�", pCountry->GetCountryName(), pCountry->GetKindomName());

            WGChat Msg ;
            Msg.SetChatType( CHAT_TYPE_COUNTRY ) ;
            Msg.SetContexSize( (BYTE)strlen(szContex) ) ;
            Msg.SetContex( szContex ) ;
            Msg.SetSourNameSize( (BYTE)strlen("����") ) ;
            Msg.SetSourName( "����" ) ;
            Msg.SetWorldChatID( g_pChatCenter->NextWorldChatID() ) ;

            //����������Ϣ
            Msg.SetCountryID( nID );
            g_pServerManager->BroadCastServer( &Msg ) ;

            //��ǰ���������ʼ�
            CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
            sprintf(szMailContent, "ͨ��ȫ��ͶƱ�����Ѿ���������ĿǰΪƽ��״̬��������ٴγ�Ϊ������ɽ�������ע�����������ع���");
            g_pMailCenter->SendNormalMail(pCountry->GetKindomName(), szMailContent);

            pCountry->RemoveKindom();
            pCountry->VoteInfoCleanUp();
            return;
        }
    }

    // ���ɹ�
    if (!bDuring)
    {
        CHAR szContex[MAX_CHAT_SIZE] = {0};
        sprintf(szContex, "%s���ҹ���%s��������ӵ�����ڸոս����ĵ��������Ĺ����У��ɹ���ס����λ��", pCountry->GetCountryName(), pCountry->GetKindomName());

        WGChat Msg ;
        Msg.SetChatType( CHAT_TYPE_COUNTRY ) ;
        Msg.SetContexSize( (BYTE)strlen(szContex) ) ;
        Msg.SetContex( szContex ) ;
        Msg.SetSourNameSize( (BYTE)strlen("����") ) ;
        Msg.SetSourName( "����" ) ;
        Msg.SetWorldChatID( g_pChatCenter->NextWorldChatID() ) ;

        //����������Ϣ
        Msg.SetCountryID( nID );
        g_pServerManager->BroadCastServer( &Msg ) ;

        pCountry->VoteInfoCleanUp();
    }

    __LEAVE_FUNCTION
}


