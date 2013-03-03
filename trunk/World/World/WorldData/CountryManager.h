/////////////////////////////////////////////////////////////////////////////////
//�ļ�����CountryManager.h
//�������������ҹ����߼�
//�޸ļ�¼��2008-04-21 ���� 
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

    // �����ְ
    VOID        AppointPosition(INT nCountryID, INT nPosition, GUID_t guid,const CHAR *pName);
    // ������Ϣ��ѯ
    VOID        GetCountryInfo(INT nCountryID, _COUNTRY &Info);
    _COUNTRY*   GetCountryInfo(INT nCountryID);

    Country*    GetCountry(INT nCountryID);

    // ������ߵĹ��ҳ�Ա
    BOOL        AddCountryUser(COUNTRY_ATTRIBUTE countryID, const  GUID_t guid );
    // �Ƴ����ߵĹ��ҳ�Ա
    VOID        RemoveCountryUser(COUNTRY_ATTRIBUTE countryID, const  GUID_t guid );
    
    // �����ж���
    VOID        AddActionPoint(COUNTRY_ATTRIBUTE countryID, INT nAP);
    VOID        SubActionPoint(COUNTRY_ATTRIBUTE countryID, INT nAP);
    INT         GetActionPoint(COUNTRY_ATTRIBUTE countryID);

    // �㲥����ͶƱ���
    VOID        BroadcastVoteResult(Country *pCountry, BOOL bDuring = FALSE);

private:
    INT         m_mDay;  //0-31 ����

    //���º��� 
        Country m_XinYue;
    //��Į����
        Country m_DaMo;
    //���ʺ���
        Country m_JinZhang;
    //���򺹹�
        Country m_XiYu;

};

extern CountryManager* g_pCountryManager;


#endif
