
/////////////////////////////////////////////////////////////////////////////////
//文件名：WorldTable.h
//功能描述：世界数据服务器所用到的资源表格
//修改记录：2005-11-28 创建
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __WROLDTABLE_H__
#define __WROLDTABLE_H__

#include "Type.h"
#include "TopList.h"

//帮派，城市 【职位】－【权限】对照表
#define MAX_MEMBER_TYPES 100
struct _GUILD_MEMBER_ACCESS
{
	INT		m_MemberTypeCount ;
	INT		m_Table[MAX_MEMBER_TYPES][GUILD_AUTHORITY_NUMBER] ;

	_GUILD_MEMBER_ACCESS( )
	{
		m_MemberTypeCount = 0 ;
		memset( m_Table, 0, sizeof(INT)*MAX_MEMBER_TYPES*GUILD_AUTHORITY_NUMBER ) ;
	};
	INT		Get( uint membertype, uint accesstype ){
		Assert( membertype<MAX_MEMBER_TYPES && accesstype<GUILD_AUTHORITY_NUMBER ) ;
		return m_Table[membertype][accesstype] ;
	};
};
extern _GUILD_MEMBER_ACCESS g_GuildMemberAccessTbl ;

struct _DBC_CITY_BUILDING
{
	INT		nID;					//id
	INT		nCityID;				//城市id(DBC_CITY_DEINFE)
	INT		nBuildingID;			//建筑物id(DBC_BUILDING_DEINFE)
};

struct _DBC_CITY_DEFINE
{
	INT					m_Num;
	_DBC_CITY_BUILDING	m_Table[MAX_CITY_BUILDING];

	_DBC_CITY_DEFINE()
	{
		m_Num = 0;
		memset(m_Table,0,sizeof(_DBC_CITY_BUILDING)*MAX_CITY_BUILDING);
	}
};
extern _DBC_CITY_DEFINE g_CityBuildingTbl;


struct _DEFAULT_CHARDATA_TBL
{
	INT		m_Table[MAX_COUNTRY_NUM][MAX_MENPAI_NUM][CHAR_ININUM] ;

	_DEFAULT_CHARDATA_TBL( )
	{
		memset( m_Table, 0, sizeof(INT)*MAX_MENPAI_NUM*CHAR_ININUM*MAX_COUNTRY_NUM ) ;
	};
	INT		Get( uint Country, uint ProfessionType, uint CharParam )
	{
		Assert( Country<MAX_COUNTRY_NUM && ProfessionType<MAX_MENPAI_NUM && CharParam<CHAR_ININUM ) ;
		return m_Table[Country][ProfessionType][CharParam] ;
	};
	VOID	Set(uint Country,  uint MenPai ,uint Attr, INT iValue){
		Assert( Country<MAX_COUNTRY_NUM && MenPai<MATTRIBUTE_NUMBER && Attr<CHAR_ININUM  ) ;
		m_Table[Country][MenPai][Attr] = iValue;	
	}
};
extern _DEFAULT_CHARDATA_TBL g_DefaultCharDataTbl ;


//------------------------------------------------------------
//排行榜相关
//------------------------------------------------------------
#define MAX_TOP_LIST 256

struct _TOP_LIST_ATTR_TBL
{
	uint						m_Count ;
	TOP_LIST_ATTR				m_TableExt[MAX_TOP_LIST] ;

	_TOP_LIST_ATTR_TBL( )
	{
		memset( m_TableExt, 0, sizeof(TOP_LIST_ATTR)*MAX_TOP_LIST ) ;
		m_Count = 0 ;
	}

	INT		Count( ){ return m_Count ; } 

};

extern _TOP_LIST_ATTR_TBL g_TopListAttrTbl ;

//帮会升级信息表
struct _GUILD_LEVEL_INFO_TBL
{
    _GUILD_LEVEL_INFO       m_TableInfo[MAX_GUILD_LEVEL];

    _GUILD_LEVEL_INFO_TBL()
    {
    };

};
extern _GUILD_LEVEL_INFO_TBL g_GuildLevelInfoTbl ;


class TableInit
{
public :
	TableInit() ;
	~TableInit() ;

	BOOL			Init( ) ;

protected :
	VOID			Init_GuildMemberAccess() ;
	VOID			Init_CityBuilding() ;
	VOID			Init_DefaultChar();
	VOID			Init_TopListTable();
    VOID            InitGuildLevelInfoTable();

};
extern TableInit g_TableInit ;


#endif
