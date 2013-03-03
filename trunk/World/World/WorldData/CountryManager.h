/////////////////////////////////////////////////////////////////////////////////
//文件名：CountryManager.h
//功能描述：国家管理逻辑
//修改记录：2008-04-21 创建 
//			
/////////////////////////////////////////////////////////////////////////////////


#ifndef __COUNTRYMANAGER_H__
#define __COUNTRYMANAGER_H__

#include "Type.h"
#include "Country.h"
#include "GameStruct_Country.h"
#include "StrTable.h"





class CountryManager
{
public:
    CountryManager();
    ~CountryManager();

public:

    BOOL    Init();
	BOOL	HeartBeat( DWORD dwTime=0 ) ;

    // 任免官职
    VOID        AppointPosition(INT nCountryID, INT nPosition, GUID_t guid,const CHAR *pName);
    // 国家信息查询
    VOID        GetCountryInfo(INT nCountryID, _COUNTRY &Info);
    _COUNTRY*   GetCountryInfo(INT nCountryID);

    Country*    GetCountry(INT nCountryID);

    // 添加上线的国家成员
    BOOL        AddCountryUser(COUNTRY_ATTRIBUTE countryID, const  GUID_t guid );
    // 移除下线的国家成员
    VOID        RemoveCountryUser(COUNTRY_ATTRIBUTE countryID, const  GUID_t guid );
    
    // 国王行动力
    VOID        AddActionPoint(COUNTRY_ATTRIBUTE countryID, INT nAP);
    VOID        SubActionPoint(COUNTRY_ATTRIBUTE countryID, INT nAP);
    INT         GetActionPoint(COUNTRY_ATTRIBUTE countryID);

    // 广播弹劾投票结果
    VOID        BroadcastVoteResult(Country *pCountry, BOOL bDuring = FALSE);

private:
    INT         m_mDay;  //0-31 天数

    //新月汗国 
        Country m_XinYue;
    //大漠汗国
        Country m_DaMo;
    //金帐汗国
        Country m_JinZhang;
    //西域汗国
        Country m_XiYu;

};

extern CountryManager* g_pCountryManager;


#endif
