/////////////////////////////////////////////////////////////////////////////////
//文件名：Country.h
//功能描述 ：国家数据和逻辑功能
//修改记录：2008-04-21 创建 
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
        RET_REPEAT = 0,     // 重复投票
        RET_SCUESS,         
        RET_FAIL,

        RET_OK =3,          // 开始发动弹劾投票
        RET_FULL,
    };

    Country();
    ~Country();

    VOID    Init();

	BOOL	HeartBeat( DWORD dwTime=0 ) ;

public:

    // 任免官职
    VOID        AppointPosition(INT nPosition, GUID_t guid, const CHAR *pName);
    // 罢免国王
    VOID        RemoveKindom();

    //////////////////////////////////////////////////////////////////////////
    // 国家信息查询
    VOID        GetCountryInfo(_COUNTRY &Info);
    _COUNTRY*   GetCountryInfo() { return &m_CountryData ; };
    CHAR*       GetKindomName() { return m_CountryData.m_szGuoWang; }
    CHAR*       GetCountryName();
    VOID        SetCountryName(INT nCountryID);

    //////////////////////////////////////////////////////////////////////////
    // 国王行动力
    VOID        AddActionPoint(INT nAP);
    VOID        SubActionPoint(INT nAP);
    INT         GetActionPoint();

    //////////////////////////////////////////////////////////////////////////
    // 发起弹劾
    INT         Delate(GUID_t guid);
    VOID        CleanUpDelate();            //一周清空一次
    
    // 弹劾投票
    VOID        VoteOnBegin();
    BOOL        VoteOn(BOOL bResult, GUID_t guid);
    BOOL        VoteOnEnd( DWORD dwTime );
    // 取得当前投票数
    INT         GetVoteCount() { return m_nCurVoteNum; }
    INT         GetGuildMasterVote() { return m_nCurGuildMasterVote%3; }
    DWORD       GetStartTime () { return m_nStartTime; } ;
    // 清空投票信息
    VOID        VoteInfoCleanUp();
    // 投票结果
    VOID        GetVoteResult(INT& nCurVoteCnt, INT& nAgreeCnt, INT& nPercent);
    

private:

    // 国家基本数据
    _COUNTRY    m_CountryData;
    CHAR        m_szCountryName[MAX_COUNTRY_NAME];

    //弹劾投票统计
    VOTE_INFO   m_GuildMasterVote[MAX_GUILD_MASTER_VOTE];   // 发起弹劾投票
    INT         m_nCurGuildMasterVote;                      // 当前投票数

    VOTE_INFO   m_Vote[MAX_VOTE_USER];                      // 弹劾投票
    INT         m_nCurVoteNum;                              // 当前投票数
    INT         m_nCurAgreeNum;                             // 当前赞成票数
    DWORD       m_nStartTime;                               // 开投票的时间点
    time_t      m_nTime;                                    // 用于一周清空

};

#endif