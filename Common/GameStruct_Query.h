#ifndef _GAMESTRUCT_HORSEFINDMATE_H_
#define _GAMESTRUCT_HORSEFINDMATE_H_

#include "Type.h"
#include "DB_Struct.h"
#include "Packet.h"

//交易/放生/交配/还童
enum HorseStateChangeType{
	ST_TRADE = 0,
	ST_MATING = 1,
	ST_FREE = 2,
	ST_RETURNTOCHILD = 3,

	ST_COUNT
};

struct HorseFindMateInfo
{
	PET_GUID_t m_HorseGuid;
	INT m_iHorseDataId;	//骑乘在表中的数据ID
	UINT m_uIssueTime;	//骑乘发布的时间戳
	CHAR m_szHorseName[MAX_CHARACTER_NAME];
	CHAR m_szOwnerName[MAX_CHARACTER_NAME];
	CHAR m_szLeaveWords[MAX_BBS_MESSAGE_LENGTH];
	INT m_iHorseLevel;
	INT m_iSex;			//Default: -1, female: 0, male: 1
	INT m_iGrowRate;	//成长率
	INT m_iMatchPoint;  //速配指数

	HorseFindMateInfo& operator=(const HorseFindMateInfo& value){
		m_HorseGuid = value.m_HorseGuid;
		m_iHorseDataId = value.m_iHorseDataId;
		m_uIssueTime = value.m_uIssueTime;
		tsnprintf_s(m_szHorseName, MAX_CHARACTER_NAME-1, "%s", value.m_szHorseName);
		tsnprintf_s(m_szOwnerName, MAX_CHARACTER_NAME-1, "%s", value.m_szOwnerName);
		tsnprintf_s(m_szLeaveWords, MAX_BBS_MESSAGE_LENGTH-1, "%s", value.m_szLeaveWords);
		m_iHorseLevel = value.m_iHorseLevel;
		m_iSex = value.m_iSex;
		m_iGrowRate = value.m_iGrowRate;
		m_iMatchPoint = value.m_iMatchPoint;

		return *this;
	}

	HorseFindMateInfo(){
		m_iHorseLevel = INVALID_ID;
		m_iSex = INVALID_SEX;
		m_iGrowRate = INVALID_ID;
		m_iGrowRate = INVALID_ID;
		m_iMatchPoint = INVALID_ID;
		m_uIssueTime = 0;
		tsnprintf_s(m_szHorseName, MAX_CHARACTER_NAME-1, "%s", "");
		tsnprintf_s(m_szOwnerName, MAX_CHARACTER_NAME-1, "%s", "");
		tsnprintf_s(m_szLeaveWords, MAX_BBS_MESSAGE_LENGTH-1, "%s", "");
	}

	UINT	GetSize( ) const
	{
		return sizeof(m_HorseGuid) + sizeof(m_iHorseDataId) + 
			sizeof(m_uIssueTime) + sizeof(m_szHorseName) + 
			sizeof(m_szOwnerName) + sizeof(m_szLeaveWords) +
			sizeof(m_iHorseLevel) + sizeof(m_iSex) + 
			sizeof(m_iGrowRate) + sizeof(m_iMatchPoint);
	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read((CHAR*)(&m_szHorseName), sizeof(m_szHorseName));
		iStream.Read((CHAR*)(&m_szOwnerName), sizeof(m_szOwnerName));
		iStream.Read((CHAR*)(&m_szLeaveWords), sizeof(m_szLeaveWords));
		iStream.Read((CHAR*)(&m_HorseGuid), sizeof(m_HorseGuid));
		iStream.Read((CHAR*)(&m_iHorseDataId), sizeof(m_iHorseDataId));
		iStream.Read((CHAR*)(&m_iHorseLevel), sizeof(m_iHorseLevel));
		iStream.Read((CHAR*)(&m_iSex), sizeof(m_iSex));
		iStream.Read((CHAR*)(&m_iGrowRate), sizeof(m_iGrowRate));
		iStream.Read((CHAR*)(&m_uIssueTime), sizeof(m_uIssueTime));
		iStream.Read((CHAR*)(&m_iMatchPoint), sizeof(m_iMatchPoint));
		
	}

	VOID	Write(SocketOutputStream& oStream) const
	{
		oStream.Write((CHAR*)(&m_szHorseName), sizeof(m_szHorseName));
		oStream.Write((CHAR*)(&m_szOwnerName), sizeof(m_szOwnerName));
		oStream.Write((CHAR*)(&m_szLeaveWords), sizeof(m_szLeaveWords));
		oStream.Write((CHAR*)(&m_HorseGuid), sizeof(m_HorseGuid));
		oStream.Write((CHAR*)(&m_iHorseDataId), sizeof(m_iHorseDataId));
		oStream.Write((CHAR*)(&m_iHorseLevel), sizeof(m_iHorseLevel));
		oStream.Write((CHAR*)(&m_iSex), sizeof(m_iSex));
		oStream.Write((CHAR*)(&m_iGrowRate), sizeof(m_iGrowRate));
		oStream.Write((CHAR*)(&m_uIssueTime), sizeof(m_uIssueTime));
		oStream.Write((CHAR*)(&m_iMatchPoint), sizeof(m_iMatchPoint));

	}
};

struct HorseFindMateInfoPerPage
{
	enum { MAX_COUNT_PER_PAGE = 6 };

	INT m_iCount;
	HorseFindMateInfo m_aList[MAX_COUNT_PER_PAGE];

	HorseFindMateInfoPerPage& operator=(const HorseFindMateInfoPerPage& value){
		m_iCount = value.m_iCount;
		for(INT i=0; i<MAX_COUNT_PER_PAGE; ++i)
			m_aList[i] = value.m_aList[i];

		return *this;
	}

	UINT	GetSize( ) const
	{
		return sizeof(m_iCount) + m_aList[0].GetSize()*MAX_COUNT_PER_PAGE;
	}
	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read((CHAR*)&m_iCount, sizeof(m_iCount));
		for(INT i=0; i<MAX_COUNT_PER_PAGE; ++i){
			m_aList[i].Read(iStream);
		}
	}
	VOID	Write(SocketOutputStream& oStream) const
	{
		oStream.Write((CHAR*)&m_iCount, sizeof(m_iCount));
		for(INT i=0; i<MAX_COUNT_PER_PAGE; ++i){
			m_aList[i].Write(oStream);
		}
	}

};

//-----------------------------------------------------------------

struct OnlineUser_SameCity
{
	GUID_t				m_PlayerGUID;
	CHAR				m_szName[MAX_CHARACTER_NAME];
	CHAR				m_szGuildName[MAX_GUILD_NAME_SIZE];
	INT					m_iLevel;
	INT					m_iCountry;
	INT					m_iProfession;
	POSTCODE			m_PostCode;	//在线玩家邮编号,根据此号就可以匹配对应的城市和区域

	OnlineUser_SameCity& operator=(const OnlineUser_SameCity& value){
		m_PlayerGUID = value.m_PlayerGUID;
		tsnprintf_s(m_szName, MAX_CHARACTER_NAME-1, "%s", value.m_szName);
		tsnprintf_s(m_szGuildName, MAX_CHARACTER_NAME-1, "%s", value.m_szGuildName);
		m_iLevel = value.m_iLevel;
		m_iCountry = value.m_iCountry;
		m_iProfession = value.m_iProfession;
		m_PostCode = value.m_PostCode;

		return *this;
	}

	OnlineUser_SameCity(){
		m_PlayerGUID = INVALID_GUID;
		m_iLevel = INVALID_ID;
		m_iCountry = INVALID_ID;
		m_iProfession = INVALID_ID;
		m_PostCode = INVALID_ID;
		tsnprintf_s(m_szName, MAX_CHARACTER_NAME-1, "%s", "");
		tsnprintf_s(m_szGuildName, MAX_CHARACTER_NAME-1, "%s", "");
	}

	UINT	GetSize( ) const
	{
		return sizeof(m_PlayerGUID) + sizeof(m_szName) + sizeof(m_szGuildName) + 
			sizeof(m_iLevel) + sizeof(m_iCountry) + 
			sizeof(m_iProfession) + sizeof(m_PostCode);
	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read((CHAR*)(&m_PlayerGUID), sizeof(m_PlayerGUID));
		iStream.Read((CHAR*)(&m_szName), sizeof(m_szName));
		iStream.Read((CHAR*)(&m_szGuildName), sizeof(m_szGuildName));
		iStream.Read((CHAR*)(&m_iLevel), sizeof(m_iLevel));
		iStream.Read((CHAR*)(&m_iCountry), sizeof(m_iCountry));
		iStream.Read((CHAR*)(&m_iProfession), sizeof(m_iProfession));
		iStream.Read((CHAR*)(&m_PostCode), sizeof(m_PostCode));
	}

	VOID	Write(SocketOutputStream& oStream) const
	{
		oStream.Write((CHAR*)(&m_PlayerGUID), sizeof(m_PlayerGUID));
		oStream.Write((CHAR*)(&m_szName), sizeof(m_szName));
		oStream.Write((CHAR*)(&m_szGuildName), sizeof(m_szGuildName));
		oStream.Write((CHAR*)(&m_iLevel), sizeof(m_iLevel));
		oStream.Write((CHAR*)(&m_iCountry), sizeof(m_iCountry));
		oStream.Write((CHAR*)(&m_iProfession), sizeof(m_iProfession));
		oStream.Write((CHAR*)(&m_PostCode), sizeof(m_PostCode));
	}
};

struct OnlineUser_SameCity_PerPage
{
	enum {MAX_ONLINEUSER_NUMBER = 20};

	INT m_iCount;
	OnlineUser_SameCity m_aList[MAX_ONLINEUSER_NUMBER];

	OnlineUser_SameCity_PerPage& operator=(const OnlineUser_SameCity_PerPage& value){
		m_iCount = value.m_iCount;
		for(INT i=0; i<MAX_ONLINEUSER_NUMBER; ++i)
			m_aList[i] = value.m_aList[i];

		return *this;
	}

	UINT	GetSize( ) const
	{
		return sizeof(m_iCount) + m_aList[0].GetSize()*MAX_ONLINEUSER_NUMBER;
	}
	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read((CHAR*)&m_iCount, sizeof(m_iCount));
		for(INT i=0; i<MAX_ONLINEUSER_NUMBER; ++i){
			m_aList[i].Read(iStream);
		}
	}
	VOID	Write(SocketOutputStream& oStream) const
	{
		oStream.Write((CHAR*)&m_iCount, sizeof(m_iCount));
		for(INT i=0; i<MAX_ONLINEUSER_NUMBER; ++i){
			m_aList[i].Write(oStream);
		}
	}
};


#endif

