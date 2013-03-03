/////////////////////////////////////////////////////////////////////////////////
//�ļ�����Country.h
//�������� ���������ݺ��߼�����
//�޸ļ�¼��2008-04-21 ���� 
//			
//          
/////////////////////////////////////////////////////////////////////////////////


#ifndef __COUNTRY_H__
#define __COUNTRY_H__

#include "Type.h"
#include "GameStruct_Country.h"
#include "DB_Struct.h"

#define  MAX_COUNTRY_NAME 10
class Country
{
public:
    enum    RET_COUNTRY
    {   
        RET_REPEAT = 0,     // �ظ�ͶƱ
        RET_SCUESS,         
        RET_FAIL,

        RET_OK =3,          // ��ʼ��������ͶƱ
        RET_FULL,
    };

    Country();
    ~Country();

    VOID    Init();

	BOOL	HeartBeat( DWORD dwTime=0 ) ;

public:

    // �����ְ
    VOID        AppointPosition(INT nPosition, GUID_t guid, const CHAR *pName);
    // �������
    VOID        RemoveKindom();

    //////////////////////////////////////////////////////////////////////////
    // ������Ϣ��ѯ
    VOID        GetCountryInfo(_COUNTRY &Info);
    _COUNTRY*   GetCountryInfo() { return &m_CountryData ; };
    CHAR*       GetKindomName() { return m_CountryData.m_szGuoWang; }
    CHAR*       GetCountryName();
    VOID        SetCountryName(INT nCountryID);

    //////////////////////////////////////////////////////////////////////////
    // �����ж���
    VOID        AddActionPoint(INT nAP);
    VOID        SubActionPoint(INT nAP);
    INT         GetActionPoint();

    //////////////////////////////////////////////////////////////////////////
    // ������
    INT         Delate(GUID_t guid);
    VOID        CleanUpDelate();            //һ�����һ��
    
    // ����ͶƱ
    VOID        VoteOnBegin();
    BOOL        VoteOn(BOOL bResult, GUID_t guid);
    BOOL        VoteOnEnd( DWORD dwTime );
    // ȡ�õ�ǰͶƱ��
    INT         GetVoteCount() { return m_nCurVoteNum; }
    INT         GetGuildMasterVote() { return m_nCurGuildMasterVote%3; }
    DWORD       GetStartTime () { return m_nStartTime; } ;
    // ���ͶƱ��Ϣ
    VOID        VoteInfoCleanUp();
    // ͶƱ���
    VOID        GetVoteResult(INT& nCurVoteCnt, INT& nAgreeCnt, INT& nPercent);
    

private:

    // ���һ�������
    _COUNTRY    m_CountryData;
    CHAR        m_szCountryName[MAX_COUNTRY_NAME];

    //����ͶƱͳ��
    VOTE_INFO   m_GuildMasterVote[MAX_GUILD_MASTER_VOTE];   // ������ͶƱ
    INT         m_nCurGuildMasterVote;                      // ��ǰͶƱ��

    VOTE_INFO   m_Vote[MAX_VOTE_USER];                      // ����ͶƱ
    INT         m_nCurVoteNum;                              // ��ǰͶƱ��
    INT         m_nCurAgreeNum;                             // ��ǰ�޳�Ʊ��
    DWORD       m_nStartTime;                               // ��ͶƱ��ʱ���
    time_t      m_nTime;                                    // ����һ�����

};

#endif