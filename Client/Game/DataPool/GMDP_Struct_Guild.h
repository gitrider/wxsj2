
/**	GMDP_Struct_Guild.h
 *
 *	功能：	帮会相关的数据
 *
 *	修改记录：	
 *			080307	GuildFamilyInfo 描述帮会中的家族
 *			080310	GuildMemberInfo_t 添加 m_iFamilyID 所属家族ID
 *					
 */

#pragma once


#include "Type.h"



//可任命的帮派职位
struct GuildAppointPos_t
{
	STRING		m_szPosName;			//职位名
	INT			m_PosID;				//职位ID

	GuildAppointPos_t()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		m_PosID			= -1;
		m_szPosName		= "";
	}
};

//帮派的详细信息
struct GuildDetailInfo_t
{
	STRING			m_GuildName;
	STRING			m_GuildCreator;
	STRING			m_GuildChairMan;
	STRING			m_CityName;
	BYTE			m_nLevel;
	INT				m_nPortSceneID;				//入口场景
	INT				m_MemNum;					//人数
	INT				m_Longevity;				//资历 
	INT				m_Contribute;				//贡献度
	INT				m_Honor;					//人气
	INT				m_FoundedMoney;				//帮派资金
	INT				m_nIndustryLevel;			//工业度
	INT				m_nAgrLevel;				//农业度
	INT				m_nComLevel;				//商业度
	INT				m_nDefLevel;				//防卫度
	INT				m_nTechLevel;				//科技度
	INT				m_nAmbiLevel;				//扩张度

	GuildDetailInfo_t()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		m_GuildName			= "";
		m_GuildCreator		= "";
		m_GuildChairMan		= "";
		m_CityName			= "";
		m_nLevel			=	0;
		m_nPortSceneID		=	0;			//入口场景
		m_MemNum			=	0;			//人数
		m_Longevity			=	0;			//资历 
		m_Contribute		=	0;			//贡献度
		m_Honor				=	0;			//人气
		m_FoundedMoney		=	0;			//帮派资金
		m_nIndustryLevel	=	0;			//工业度
		m_nAgrLevel			=	0;			//农业度
		m_nComLevel			=	0;			//商业度
		m_nDefLevel			=	0;			//防卫度
		m_nTechLevel		=	0;			//科技度
		m_nAmbiLevel		=	0;			//扩张度
	}

};

//帮众信息
struct GuildMemberInfo_t
{
	STRING		m_szName;			// 名称
	GUID_t		m_Guid;				// guid
	BYTE		m_bLevel;			// 等级
	BYTE		m_bMenPaiID;		// 职业
	INT			m_iCurContribute;	// 贡献
	INT			m_iMaxContribute;	// 最大贡献
	INT			m_iJoinTime;
	INT			m_iLogOutTime;
	BYTE		m_bIsOnline;		// 是否离线
	BYTE		m_bPosition;		// 职位
	INT			m_iFamilyID;		// 所属家族
	BYTE		m_iSex;				// 
	STRING		m_szTitleName;		// 
	
	GuildMemberInfo_t()
	{
		CleanUp();
	}
	VOID CleanUp()
	{
		m_szName			=	"";
		m_Guid				=	INVALID_ID;
		m_bLevel			=	0;
		m_bMenPaiID			=	0;
		m_iCurContribute	=	0;
		m_iMaxContribute	=	0;
		m_iJoinTime			=	0;
		m_iLogOutTime		=	0;
		m_bIsOnline			=	0;
		m_bPosition			=	0;
		m_iFamilyID			=	INVALID_ID;
	}

};

//工会列表中每一项的内容
struct GuildInfo_t
{
	INT					m_GuildID;
	STRING				m_ChiefName;
	STRING				m_GuildName;
	STRING				m_GuildDesc;
	STRING				m_CityName;
	INT					m_PortSceneID;
	UCHAR				m_uGuildStatus;
	UCHAR				m_uGuildUserCount;
	BYTE				m_bGuildLevel;
	INT					m_nFoundedTime;

	GuildInfo_t()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		m_GuildID			= INVALID_ID;
		m_ChiefName			=	"";
		m_GuildName			=	"";
		m_GuildDesc			=	"";
		m_CityName			=	"";
		m_uGuildStatus		=	0;
		m_uGuildUserCount	=	0;
		m_bGuildLevel		=	0;
		m_PortSceneID		=	-1;
		m_nFoundedTime		=	0;
	}
};



// 帮会中家族的信息
struct GuildFamilyInfo
{
	INT			m_iFamilyID;		// 家族在帮会中的ID
	STRING		m_sFamilyChiefName;	// 家族长的名字
	STRING		m_sFamilyName;		// 家族名字
	STRING		m_sFamilyDesc;		// 家族的宗旨
	INT			m_aFamilyUser[USER_FAMILY_SIZE];
	INT			m_iFamilyUserNum;

	GuildFamilyInfo()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		m_iFamilyID			= INVALID_ID;
		m_sFamilyChiefName	= "";
		m_sFamilyName		= "";
		m_sFamilyDesc		= "";
		m_iFamilyUserNum	= INVALID_ID;

		memset( (VOID*)m_aFamilyUser, 0, sizeof(INT) * USER_FAMILY_SIZE );
	};

};


