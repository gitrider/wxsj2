/********************************************************************************
 *	文件名：	GameStruct_Guild.h
 *	全路径：	d:\Prj\Common\GameStruct_Guild.h
 *	创建人：	胡繁
 *	创建时间：	2005 年 12 月 9 日	17:12
 *
 *	功能说明：	帮会的一些数据结构、消息结构
 *				GUILD_PACKET 是消息包的接口 GUILD_CGW_ASKLIST 是具体包
 *				_GUILD_PACKET 是提供给消息使用的 struct
 *	修改记录：
*********************************************************************************/

#ifndef __GAMESTRUCT_GUILD_H__
#define __GAMESTRUCT_GUILD_H__

#include "Type.h"
#include "GameDefine_Guild.h"

class SocketInputStream;
class SocketOutputStream;


// 帮会等级信息
struct  _GUILD_LEVEL_INFO
{
    CHAR        m_Level;                //帮会等级
    CHAR        m_MaxFamily;            //家族个数上限
    INT         m_Gold;                 //该级所需金钱
    INT         m_Contribute;           //该级所需贡献
    INT         m_MaxGuildUser;         //帮会最大成员上限
    INT         m_MaxGoldFamilyUser;    //黄金家族人数上限

    _GUILD_LEVEL_INFO()
    {
        CHAR        m_Level             =   0;             
        CHAR        m_MaxFamily         =   0;         
        INT         m_Gold              =   0;              
        INT         m_Contribute        =   0;        
        INT         m_MaxGuildUser      =   0;      
        INT         m_MaxGoldFamilyUser =   0; 
    }
};

// 帮会消息包的接口
struct GUILD_PACKET
{
	virtual UINT			GetPacketSize() const = 0;
	virtual VOID			Read(SocketInputStream& iStream) = 0;
	virtual VOID			Write(SocketOutputStream& oStream) const = 0;
};

// 询问帮会列表
struct GUILD_CGW_ASKLIST : public GUILD_PACKET
{
	UCHAR					m_SortType;
	USHORT					m_uStart;

	virtual UINT			GetPacketSize() const { return sizeof(m_SortType) + sizeof(m_uStart); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 创建帮会
struct GUILD_CGW_CREATE : public GUILD_PACKET
{
	UCHAR					m_NameSize;
	CHAR					m_szGuildName[MAX_GUILD_NAME_SIZE];

	UCHAR					m_DescSize;
	CHAR					m_szGuildDesc[MAX_GUILD_DESC_SIZE];

	GUILD_CGW_CREATE()
	{
		m_NameSize = 0;
		memset((void*)m_szGuildName, 0, MAX_GUILD_NAME_SIZE);

		m_DescSize = 0;
		memset((void*)m_szGuildDesc, 0, MAX_GUILD_DESC_SIZE);
	}

	virtual UINT			GetPacketSize() const { return sizeof(m_NameSize) + m_NameSize + sizeof(m_DescSize) + m_DescSize; }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 修改帮会宗旨
struct GUILD_CGW_MODIFY_DESC : public GUILD_PACKET
{
    UCHAR					m_DescSize;
    CHAR					m_szGuildDesc[MAX_GUILD_DESC_SIZE];

    GUILD_CGW_MODIFY_DESC()
    {
        m_DescSize = 0;
        memset((void*)m_szGuildDesc, 0, MAX_GUILD_DESC_SIZE);
    }

    virtual UINT			GetPacketSize() const { return sizeof(m_DescSize) + m_DescSize; }
    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 修改玩家帮会称号
struct GUILD_CGW_MODIFY_TITLE_NAME : public GUILD_PACKET
{
    UCHAR					m_NameSize;
    CHAR					m_szTitleName[MAX_CHARACTER_TITLE];

    GUILD_CGW_MODIFY_TITLE_NAME()
    {
        m_NameSize = 0;
        memset((void*)m_szTitleName, 0, MAX_CHARACTER_TITLE);
    }

    virtual UINT			GetPacketSize() const { return sizeof(m_NameSize) + m_NameSize; }
    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 加入帮会
struct GUILD_CGW_JOIN : public GUILD_PACKET
{
	GuildID_t				m_GuildGUID;
    GUID_t                  m_TargetGuid;

	virtual UINT			GetPacketSize() const { return sizeof(m_GuildGUID)+sizeof(m_TargetGuid); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 邀请加入帮会
struct GUILD_CGW_INVITE : public GUILD_PACKET
{
    UCHAR					m_NameSize;
    CHAR					m_szTargetName[MAX_CHARACTER_NAME];    

	GUILD_CGW_INVITE()
	{
		m_NameSize      = 0;
		memset((void*)m_szTargetName, 0, MAX_CHARACTER_NAME);
	};


	virtual UINT			GetPacketSize() const { return sizeof(UCHAR) + m_NameSize; }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 创建家族
struct GUILD_CGW_CREATE_FAMILY : public GUILD_PACKET
{
    GuildID_t				m_GuildGUID;
    INT                     m_ShaikhIndex;      //家族长Index
    GUID_t                  m_ShaikhGuid;
    BYTE                    m_cDescSize;
    BYTE                    m_cNameSize;
    CHAR                    m_szFamilyDesc[MAX_GUILD_FAMILY_DESC_SIZE];
    CHAR                    m_szFamilyName[MAX_GUILD_FAMILY_NAME_SIZE];

    GUILD_CGW_CREATE_FAMILY()
    {
        m_GuildGUID      = INVALID_GUILD_ID;
        m_cDescSize      = 0;
        m_cNameSize      = 0;
        m_ShaikhIndex    = INVALID_ID;
        m_ShaikhGuid     = INVALID_GUID;
        memset((void*)m_szFamilyDesc, 0, MAX_GUILD_FAMILY_DESC_SIZE);
        memset((void*)m_szFamilyName, 0, MAX_GUILD_FAMILY_NAME_SIZE);
    };

    virtual UINT			GetPacketSize() const
    {
        return sizeof(m_GuildGUID) + sizeof(GUID_t) + sizeof(INT) +
               m_cDescSize+m_cNameSize + 
               sizeof(BYTE)*2;
    }
    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 添加家族成员
struct GUILD_CGW_ADD_FAMILY_USER : public GUILD_PACKET
{
    CHAR        m_cFamilyIndex; 
    INT         m_iGuildIndex;
    GUID_t      m_TargetGuid;   //添加成员的GUID

    GUILD_CGW_ADD_FAMILY_USER ()
    {
        m_cFamilyIndex  = INVALID_ID;
        m_iGuildIndex   = INVALID_ID;
        m_TargetGuid    = INVALID_GUID;      
    }

    virtual UINT			GetPacketSize() const
    {
        return sizeof(CHAR) + sizeof(INT) + sizeof(GUID_t);
    }
    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 删除家族成员
struct GUILD_CGW_REMOVE_FAMILY_USER : public GUILD_PACKET
{
    CHAR        m_cFamilyIndex; 
    INT         m_iGuildIndex;
    GUID_t      m_TargetGuid;   //添加成员的GUID

    GUILD_CGW_REMOVE_FAMILY_USER ()
    {
        m_cFamilyIndex  = INVALID_ID;
        m_iGuildIndex   = INVALID_ID;
        m_TargetGuid    = INVALID_GUID;      
    }

    virtual UINT			GetPacketSize() const
    {
        return sizeof(CHAR) + sizeof(INT) + sizeof(GUID_t);
    }
    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 解散家族
struct GUILD_CGW_FAMILY_DISMISS : public GUILD_PACKET
{
    GuildID_t				m_GuildGUID;
    INT     				m_nFamilyID;				

    virtual UINT			GetPacketSize() const { return sizeof(m_GuildGUID)+sizeof(INT); }
    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 询问帮会信息
struct GUILD_CGW_ASKINFO : public GUILD_PACKET
{
	enum
	{
		GUILD_MEMBER_INFO = 0,	//帮众信息
		GUILD_INFO,			//帮会信息
		GUILD_APPOINT_POS,	//帮会中可任命的职位
		GUILD_SELF_INFO,	//本人帮派信息

	};

    GUILD_CGW_ASKINFO()
    {
        memset(m_Password, 0, MAX_PWD);
    }

	GuildID_t				m_GuildGUID;
	BYTE					m_Type;				//帮众信息，还是帮会信息

    BYTE                    m_cPwdSize;             
    CHAR                    m_Password[MAX_PWD];    //密码保护

	virtual UINT			GetPacketSize() const { return sizeof(m_GuildGUID)+sizeof(BYTE)+m_cPwdSize+sizeof(m_cPwdSize); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 帮会任免职务
struct GUILD_CGW_APPOINT : public GUILD_PACKET
{
	GuildID_t				m_GuildGUID;
	GUID_t					m_CandidateGUID;
	UCHAR					m_NewPosition;

	virtual UINT			GetPacketSize() const
	{
		return sizeof(m_GuildGUID) + sizeof(m_CandidateGUID) + sizeof(m_NewPosition);
	}
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 帮会调整权限
struct GUILD_CGW_ADJUSTAUTH : public GUILD_PACKET
{
	GuildID_t				m_GuildGUID;
	GUID_t					m_CandidateGUID;
	UCHAR					m_NewAuthority;

	virtual UINT			GetPacketSize() const
	{
		return sizeof(m_GuildGUID) + sizeof(m_CandidateGUID) + sizeof(m_NewAuthority);
	}
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 帮会升级
struct GUILD_CGW_LEVELUP : public GUILD_PACKET
{
    GuildID_t				m_GuildGUID;
    GUID_t					m_ChiefGUID;
    UCHAR					m_GuildLevel;
    INT 					m_GP;           //客户端不用填

    virtual UINT			GetPacketSize() const
    {
        return sizeof(m_GuildGUID) + sizeof(m_ChiefGUID) + sizeof(m_GuildLevel) +sizeof(INT);
    }
    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 帮贡
struct GUILD_CGW_GP : public GUILD_PACKET
{
    GuildID_t				m_GuildGUID;
    INT 					m_GP;           

    virtual UINT			GetPacketSize() const
    {
        return sizeof(m_GuildGUID) + sizeof(m_GP);
    }
    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 帮会等级信息
struct GUILD_CGW_LEVELINFO : public GUILD_PACKET
{
    INT     				m_MaxUser;
    INT                     m_MaxFamily;
    INT 					m_MaxGoldFamilyUser;           

    virtual UINT			GetPacketSize() const
    {
        return sizeof(INT)*3;
    }
    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 招收新帮众
struct GUILD_CGW_RECRUIT : public GUILD_PACKET
{
	GUID_t					m_ProposerGUID;

	virtual UINT			GetPacketSize() const { return sizeof(m_ProposerGUID); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 开除帮众
struct GUILD_CGW_EXPEL : public GUILD_PACKET
{
	GUID_t					m_GuildUserGUID;

	virtual UINT			GetPacketSize() const { return sizeof(m_GuildUserGUID); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 提取帮资
struct GUILD_CGW_WITHDRAW : public GUILD_PACKET
{
	UINT					m_MoneyAmount;

	virtual UINT			GetPacketSize() const { return sizeof(m_MoneyAmount); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 存入帮资
struct GUILD_CGW_DEPOSIT : public GUILD_PACKET
{
	UINT					m_MoneyAmount;

	virtual UINT			GetPacketSize() const { return sizeof(m_MoneyAmount); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 离开帮会
struct GUILD_CGW_DEMISE : public GUILD_PACKET
{
	virtual UINT			GetPacketSize() const { return 0; }
	virtual VOID			Read(SocketInputStream& iStream) {}
	virtual VOID			Write(SocketOutputStream& oStream) const {}
};

// 离开帮会
struct GUILD_CGW_LEAVE : public GUILD_PACKET
{
	virtual UINT			GetPacketSize() const { return 0; }
	virtual VOID			Read(SocketInputStream& iStream) {}
	virtual VOID			Write(SocketOutputStream& oStream) const {}
};

// 更改帮会宗旨
struct GUILD_CGW_CHANG_DESC : public GUILD_PACKET
{
	CHAR					m_GuildDesc[MAX_GUILD_DESC_SIZE];
	BYTE					m_GuildDescLength;

	GUILD_CGW_CHANG_DESC()
	{
		memset(m_GuildDesc, 0, MAX_GUILD_DESC_SIZE);
		m_GuildDescLength	=	0;
	}

	virtual UINT			GetPacketSize() const { return sizeof(BYTE) + m_GuildDescLength; }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 确认邀请
struct GUILD_CGW_INVITE_CONFIRM : public GUILD_PACKET
{
	GuildID_t		m_GuildId;		// 帮会ID
    BOOL			m_AgreeInvite;	// 同意邀请
    BYTE			m_InvitorNameSize;
    CHAR			m_InvitorName[MAX_CHARACTER_NAME];


	GUILD_CGW_INVITE_CONFIRM()
	{
		m_GuildId       = INVALID_ID;
        m_AgreeInvite   = TRUE;
        m_InvitorNameSize   = 0;
        memset(&m_InvitorName[0], 0, MAX_CHARACTER_NAME);
	};

	virtual UINT			GetPacketSize() const
    { 
        return sizeof(GuildID_t) + sizeof(BOOL) + 
                      m_InvitorNameSize + sizeof(m_InvitorNameSize);
    }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};


//帮会的可任命信息
struct GUILD_WGC_APPOINT_INFO : public GUILD_PACKET
{
	struct s
	{
		CHAR	m_PosName[MAX_GUILD_POS_NAME_SIZE];
		BYTE	m_PosID;
		s()
		{
			memset (m_PosName, 0, MAX_GUILD_POS_NAME_SIZE);
			m_PosID			=	0;
		}	
	} m_PosList[GUILD_POSITION_SIZE];
	INT iPosNum;

	GUILD_WGC_APPOINT_INFO()
	{
		iPosNum = 0;
	}

	virtual UINT			GetPacketSize() const
	{
		return	sizeof(iPosNum)+
				sizeof(s)*iPosNum;
	}

	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;

};

//帮会信息
struct GUILD_WGC_GUILD_INFO : public GUILD_PACKET
{
	CHAR			m_GuildName[MAX_GUILD_NAME_SIZE];
	CHAR			m_GuildCreator[MAX_CHARACTER_NAME];
	CHAR			m_GuildChairMan[MAX_CHARACTER_NAME];
	CHAR			m_CityName[MAX_CITY_NAME_SIZE];
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

	GUILD_WGC_GUILD_INFO()
	{
		CleanUp();
	}
	VOID CleanUp()
	{
		memset(m_GuildName, 0, MAX_GUILD_NAME_SIZE);
		memset(m_GuildCreator, 0, MAX_CHARACTER_NAME);
		memset(m_GuildChairMan, 0, MAX_CHARACTER_NAME);
		memset(m_CityName, 0, MAX_CITY_NAME_SIZE);
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

	virtual UINT			GetPacketSize() const
	{
		return	sizeof(m_nLevel) +
				sizeof(m_nPortSceneID) +
				sizeof(m_MemNum) +
				sizeof(m_Longevity) +
				sizeof(m_Contribute) +
				sizeof(m_Honor) +
				sizeof(m_FoundedMoney) +
				sizeof(m_nIndustryLevel) +
				sizeof(m_nAgrLevel) +
				sizeof(m_nComLevel) +
				sizeof(m_nDefLevel) +
				sizeof(m_nTechLevel) +
				sizeof(m_nAmbiLevel) +
				MAX_CHARACTER_NAME + 
				MAX_CHARACTER_NAME + 
				MAX_CITY_NAME_SIZE + 
				MAX_GUILD_NAME_SIZE;
	}

	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;

};

// 帮众列表
struct GUILD_WGC_MEMBER_LIST : public GUILD_PACKET
{
	struct s
	{
		CHAR		m_szName[MAX_CHARACTER_NAME];
		GUID_t		m_Guid;
		BYTE		m_bLevel;
		BYTE		m_bMenPaiID;
		INT			m_iCurContribute;
		INT			m_iMaxContribute;
		INT			m_iJoinTime;
		INT			m_iLogOutTime;
		BYTE		m_bIsOnline;
		BYTE		m_bPosition;
        BYTE        m_iSex;
        INT         m_iFamilyID;
        CHAR        m_szTitleName[MAX_CHARACTER_TITLE];
		s()
		{
			memset(m_szName, 0, MAX_CHARACTER_NAME);
			memset(m_szTitleName, 0, MAX_CHARACTER_TITLE);
			m_Guid				=   INVALID_GUID;
			m_bLevel			=	0;
			m_bMenPaiID			=	MATTRIBUTE_UNKNOW;
			m_iCurContribute	=	0;
			m_iMaxContribute	=	0;
			m_iJoinTime			=	0;
			m_iLogOutTime		=	0;
			m_bIsOnline			=	0;
			m_bPosition			=	0;	
            m_iSex              =   2; //人妖
            m_iFamilyID         =   -1;
		}
	}	m_GuildMemberData[USER_ARRAY_SIZE];

    struct f
    {
        CHAR        m_iFamilyID;
        CHAR        m_szShaikhName[MAX_CHARACTER_NAME];
        CHAR		m_szFamilyName[MAX_GUILD_FAMILY_NAME_SIZE];
        CHAR		m_szFamilyDesc[MAX_GUILD_FAMILY_DESC_SIZE];
        INT         m_aFamilyUser[USER_FAMILY_SIZE]; 
		INT			m_iFamilyUserNum;

        f()
        {
            memset(m_szShaikhName, 0, MAX_CHARACTER_NAME);
            memset(m_szFamilyName, 0, MAX_GUILD_FAMILY_NAME_SIZE);
            memset(m_szFamilyDesc, 0, MAX_GUILD_FAMILY_DESC_SIZE);
            m_iFamilyID         =   INVALID_ID;
			m_iFamilyUserNum	=   INVALID_ID;

            for( INT i=0; i<USER_FAMILY_SIZE; ++i )
            {
                m_aFamilyUser[i] = INVALID_ID;
            }

        }
    }	m_GuildFamilyData[GUILD_FAMILY_SIZE];

	USHORT			m_uValidMemberCount;
	USHORT			m_uMemberCount;
	USHORT			m_uMemberMax;
    USHORT          m_uFamilyCount;
	CHAR			m_GuildDesc[MAX_GUILD_DESC_SIZE];
	CHAR			m_GuildName[MAX_GUILD_NAME_SIZE];
	BYTE			m_bPosition;
	BYTE			m_bAccess;

	GUILD_WGC_MEMBER_LIST()
	{
		m_uValidMemberCount	=	0;
		m_uMemberCount	=	0;
		m_uMemberMax	=	0;
		m_bPosition		=	0;
		m_bAccess		=	0;
        m_uFamilyCount  =   0;
		memset(m_GuildDesc, 0, MAX_GUILD_DESC_SIZE);
		memset(m_GuildName, 0, MAX_GUILD_NAME_SIZE);
	}

	virtual UINT			GetPacketSize() const
	{
		return	sizeof(m_uValidMemberCount) +
				sizeof(m_uMemberCount) + 
				sizeof(m_uMemberMax) + 
                sizeof(m_uFamilyCount) +
				sizeof(m_bPosition) +
				sizeof(m_bAccess) +
				MAX_GUILD_DESC_SIZE + 
				MAX_GUILD_NAME_SIZE + 
				m_uMemberCount * sizeof(s) +
                m_uFamilyCount * sizeof(f);
	}

	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;

};

// 帮会列表
struct GUILD_WGC_LIST : public GUILD_PACKET
{
	USHORT					m_uStartIndex;
	USHORT					m_uGuildCount;
	UCHAR					m_uGuildListCount;

	struct s
	{
		s()
		{
			m_GuildID = INVALID_ID;
			memset((void*)m_GuildName, 0, sizeof(m_GuildName));
			memset((void*)m_GuildDesc, 0, sizeof(m_GuildDesc));
			memset((void*)m_CityName, 0, sizeof(m_CityName));
			memset((void*)m_ChiefName, 0, sizeof(m_ChiefName));
			m_uGuildStatus		= 0;
			m_uGuildUserCount	= 0;
			m_bGuildLevel		= 0;
			m_PortSceneID		= -1;
			m_nFoundedTime		=	0;
		}

		GuildID_t			m_GuildID;
		CHAR				m_ChiefName[MAX_CHARACTER_NAME];
		CHAR				m_GuildName[MAX_GUILD_NAME_SIZE];
		CHAR				m_GuildDesc[MAX_GUILD_DESC_SIZE];
		CHAR				m_CityName[MAX_CITY_NAME_SIZE];
		INT					m_PortSceneID;
		UCHAR				m_uGuildStatus;
		UCHAR				m_uGuildUserCount;
		BYTE				m_bGuildLevel;
		INT					m_nFoundedTime;
	}		m_uGuild[MAX_GUILD_LIST_COUNT];

	GUILD_WGC_LIST()
	{
		m_uStartIndex = 0;
		m_uGuildCount = 0;
		m_uGuildListCount = 0;
	}

	virtual UINT			GetPacketSize() const
	{
		return sizeof(m_uStartIndex) + sizeof(m_uGuildCount) + sizeof(m_uGuildListCount)
			 + m_uGuildListCount * sizeof(s);
	}

	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

//修改帮会玩家称号名字
struct GUILD_WGC_MODIFY_TITLE_NAME : public GUILD_PACKET
{
    GUID_t					m_Guid;
    UCHAR					m_uTitleNameSize;
    CHAR                    m_szTitleName[MAX_CHARACTER_TITLE];

    GUILD_WGC_MODIFY_TITLE_NAME()
    {
        m_Guid = INVALID_GUID;
        m_uTitleNameSize = 0;
        memset(m_szTitleName, 0, MAX_CHARACTER_TITLE);
    }

    virtual UINT			GetPacketSize() const
    {
        return sizeof(GUID_t) + sizeof(UCHAR) + m_uTitleNameSize;
    }

    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;
};


// 转发邀请加入帮会的请求
struct GUILD_WGC_INVITE : public GUILD_PACKET
{
    GuildID_t		m_GuildId;
    BYTE			m_GuildNameSize;
    CHAR			m_GuildName[MAX_GUILD_NAME_SIZE];
    BYTE			m_InvitorNameSize;
    CHAR			m_InvitorName[MAX_CHARACTER_NAME];


    GUILD_WGC_INVITE()
    {
        m_GuildId           = INVALID_ID;
        m_GuildNameSize     = 0;
        m_InvitorNameSize   = 0;
        memset((void*)m_GuildName, 0, sizeof(m_GuildName));
        memset((void*)m_InvitorName, 0, sizeof(m_InvitorName));
    };


    virtual UINT			GetPacketSize() const
    {
        return sizeof(GuildID_t) + sizeof(BYTE)*2 + m_GuildNameSize + m_InvitorNameSize;
    }

    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;
};


//个人帮派数据
struct GUILD_WGC_SELF_GUILD_INFO : public GUILD_PACKET
{
	CHAR			m_GuildName[MAX_GUILD_NAME_SIZE];
	BYTE			m_GuildNameSize;

	GUILD_WGC_SELF_GUILD_INFO()
	{
		CleanUp();
	}
	VOID CleanUp()
	{
		memset(m_GuildName, 0, MAX_GUILD_NAME_SIZE);
		m_GuildNameSize = 0;
	}

	virtual UINT			GetPacketSize() const
	{
		return	sizeof(BYTE) + m_GuildNameSize;
	}

	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

//邀请加入帮派
struct GUILD_WGC_INVITE_INFO : public GUILD_PACKET
{
    GuildID_t		m_GuildId;
    GUID_t          m_TargetGuid;
    BYTE			m_TargetNameSize;
    CHAR			m_TargetName[MAX_CHARACTER_NAME];

    GUILD_WGC_INVITE_INFO()
    {
        CleanUp();
    }
    VOID CleanUp()
    {
        memset(m_TargetName, 0, MAX_CHARACTER_NAME);
        m_GuildId       = INVALID_ID;
        m_TargetGuid    = INVALID_GUID;
        m_TargetNameSize= 0;
    }

    virtual UINT			GetPacketSize() const
    {
        return	sizeof(BYTE) + sizeof(m_GuildId) + sizeof(m_TargetGuid) + m_TargetNameSize;
    }

    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;
};



struct _GUILD_PACKET
{
	UCHAR					m_uPacketType;			// 消息包类型
	virtual GUILD_PACKET*	GetPacket(INT nPacketType) const = 0;

	UINT					GetPacketSize() const
	{
		GUILD_PACKET*		pGuildPacket;

		pGuildPacket = GetPacket(m_uPacketType);
		if( pGuildPacket == NULL )
		{
			Assert( FALSE );
			return 0;
		}

		return (sizeof(m_uPacketType) + pGuildPacket->GetPacketSize());
	}

	VOID					Read(SocketInputStream& iStream);
	VOID					Write(SocketOutputStream& oStream) const;
};

struct _GUILD_CGW_PACKET : public _GUILD_PACKET
{
	GUILD_CGW_ASKLIST				m_PacketAskList;
	GUILD_CGW_CREATE				m_PacketCreate;
	GUILD_CGW_JOIN					m_PacketJoin;
	GUILD_CGW_ASKINFO				m_PacketAskInfo;
	GUILD_CGW_APPOINT				m_PacketAppoint;
	GUILD_CGW_ADJUSTAUTH			m_PacketJustAuthority;
    GUILD_CGW_LEVELUP               m_PacketLevelUp;
    GUILD_CGW_GP                    m_PacketGP;
    GUILD_CGW_LEVELINFO             m_PacketLevelInfo;
	GUILD_CGW_RECRUIT				m_PacketRecruit;
	GUILD_CGW_EXPEL					m_PacketExpel;
	GUILD_CGW_WITHDRAW				m_PacketWithdraw;
	GUILD_CGW_DEPOSIT				m_PacketDeposit;
	GUILD_CGW_LEAVE					m_PacketLeave;
	GUILD_CGW_DEMISE				m_PacketDemise;
	GUILD_CGW_CHANG_DESC			m_PacketChangeDesc;
	GUILD_CGW_INVITE				m_PacketInvite;
	GUILD_CGW_INVITE_CONFIRM		m_PacketInviteConfirm;
    GUILD_CGW_CREATE_FAMILY         m_PacketCreateFamily;
    GUILD_CGW_ADD_FAMILY_USER       m_PacketAddFamilyUser;
    GUILD_CGW_REMOVE_FAMILY_USER    m_PacketRemoveFamilyUser;
    GUILD_CGW_MODIFY_DESC           m_PacketModifyDesc;
    GUILD_CGW_MODIFY_TITLE_NAME     m_PacketModifyTitleName;
    GUILD_CGW_FAMILY_DISMISS        m_PacketFamilyDismiss;


	union u
	{ // 比最大的消息包长了一个虚表长度
		CHAR				u_AskListSize[sizeof(GUILD_CGW_ASKLIST)];
		CHAR				u_CreateSize[sizeof(GUILD_CGW_CREATE)];
		CHAR				u_JoinSize[sizeof(GUILD_CGW_JOIN)];
		CHAR				u_AskInfoSize[sizeof(GUILD_CGW_ASKINFO)];
		CHAR				u_AppointSize[sizeof(GUILD_CGW_APPOINT)];
		CHAR				u_JustAuthoritySize[sizeof(GUILD_CGW_ADJUSTAUTH)];
        CHAR                u_LevelUp[sizeof(GUILD_CGW_LEVELUP)];
        CHAR                u_GP[sizeof(GUILD_CGW_GP)];
        CHAR                u_LevelInfo[sizeof(GUILD_CGW_LEVELINFO)];
		CHAR				u_RecruitSize[sizeof(GUILD_CGW_RECRUIT)];
		CHAR				u_ExpelSize[sizeof(GUILD_CGW_EXPEL)];
		CHAR				u_WithdrawSize[sizeof(GUILD_CGW_WITHDRAW)];
		CHAR				u_DepositSize[sizeof(GUILD_CGW_DEPOSIT)];
		CHAR				u_LeaveSize[sizeof(GUILD_CGW_LEAVE)];
		CHAR				u_DemiseSize[sizeof(GUILD_CGW_DEMISE)];
		CHAR				u_ChangeDescSize[sizeof(GUILD_CGW_CHANG_DESC)];
		CHAR				u_InviteSize[sizeof(GUILD_CGW_INVITE)];
		CHAR				u_InviteConfirmSize[sizeof(GUILD_CGW_INVITE_CONFIRM)];
        CHAR                u_CreateFamilySize[sizeof(GUILD_CGW_CREATE_FAMILY)];
        CHAR                u_AddFamilyUserSize[sizeof(GUILD_CGW_ADD_FAMILY_USER)];
        CHAR                u_RemoveFamilyUserSize[sizeof(GUILD_CGW_REMOVE_FAMILY_USER)];
        CHAR                u_ModifyDescSize[sizeof(GUILD_CGW_MODIFY_DESC)];
        CHAR                u_ModifyTitleNameSize[sizeof(GUILD_CGW_MODIFY_TITLE_NAME)];
        CHAR                u_FamilyDismiss[sizeof(GUILD_CGW_FAMILY_DISMISS)];
	};

	_GUILD_CGW_PACKET() {}

	virtual GUILD_PACKET*	GetPacket(INT nPacketType) const
	{
		GUILD_PACKET*		pGuildPacket;

		switch(nPacketType)
		{
		case GUILD_PACKET_CG_ASKLIST:
		case GUILD_PACKET_GW_ASKLIST: pGuildPacket = (GUILD_PACKET*)&m_PacketAskList; break;

		case GUILD_PACKET_CG_CREATE:
		case GUILD_PACKET_GW_CREATE: pGuildPacket = (GUILD_PACKET*)&m_PacketCreate; break;
		
		case GUILD_PACKET_CG_JOIN:
		case GUILD_PACKET_GW_JOIN: pGuildPacket = (GUILD_PACKET*)&m_PacketJoin; break;
		
		case GUILD_PACKET_CG_ASKINFO:
		case GUILD_PACKET_GW_ASKINFO: pGuildPacket = (GUILD_PACKET*)&m_PacketAskInfo; break;
		
		case GUILD_PACKET_CG_APPOINT:
		case GUILD_PACKET_GW_APPOINT: pGuildPacket = (GUILD_PACKET*)&m_PacketAppoint; break;
		
		case GUILD_PACKET_CG_ADJUSTAUTHORITY:
		case GUILD_PACKET_GW_ADJUSTAUTHORITY: pGuildPacket = (GUILD_PACKET*)&m_PacketJustAuthority; break;
		
		case GUILD_PACKET_CG_RECRUIT:
		case GUILD_PACKET_GW_RECRUIT: pGuildPacket = (GUILD_PACKET*)&m_PacketRecruit; break;
		
		case GUILD_PACKET_CG_EXPEL:
		case GUILD_PACKET_GW_EXPEL: pGuildPacket = (GUILD_PACKET*)&m_PacketExpel; break;
		
		case GUILD_PACKET_CG_WITHDRAW:
		case GUILD_PACKET_GW_WITHDRAW: pGuildPacket = (GUILD_PACKET*)&m_PacketWithdraw; break;
		
		case GUILD_PACKET_CG_DEPOSIT:
		case GUILD_PACKET_GW_DEPOSIT: pGuildPacket = (GUILD_PACKET*)&m_PacketDeposit; break;
		
		case GUILD_PACKET_CG_LEAVE:
		case GUILD_PACKET_GW_LEAVE: pGuildPacket = (GUILD_PACKET*)&m_PacketLeave; break;
		
		case GUILD_PACKET_CG_DISMISS:
		case GUILD_PACKET_GW_DISMISS: pGuildPacket = (GUILD_PACKET*)&m_PacketLeave; break;
		
		case GUILD_PACKET_CG_DEMISE:
		case GUILD_PACKET_GW_DEMISE: pGuildPacket = (GUILD_PACKET*)&m_PacketDemise; break;
		
		case GUILD_PACKET_CG_CHANGEDESC:
		case GUILD_PACKET_GW_CHANGEDESC: pGuildPacket = (GUILD_PACKET*)&m_PacketChangeDesc; break;
		
		case GUILD_PACKET_CG_INVITE:
		case GUILD_PACKET_GW_INVITE: pGuildPacket = (GUILD_PACKET*)&m_PacketInvite; break;

		case GUILD_PACKET_CG_INVITE_CONFIRM:
		case GUILD_PACKET_GW_INVITE_CONFIRM: pGuildPacket = (GUILD_PACKET*)&m_PacketInviteConfirm;	break;

        case GUILD_PACKET_CG_CREATE_FAMILY:
        case GUILD_PACKET_GW_CREATE_FAMILY: pGuildPacket = (GUILD_PACKET*)&m_PacketCreateFamily; break;

        case GUILD_PACKET_CG_ADD_FAMILY_USER:
        case GUILD_PACKET_GW_ADD_FAMILY_USER: pGuildPacket = (GUILD_PACKET*)&m_PacketAddFamilyUser; break;

        case GUILD_PACKET_CG_REMOVE_FAMILY_USER:
        case GUILD_PACKET_GW_REMOVE_FAMILY_USER: pGuildPacket = (GUILD_PACKET*)&m_PacketRemoveFamilyUser; break;

        case GUILD_PACKET_GW_MODIFY_DESC:
        case GUILD_PACKET_CG_MODIFY_DESC: pGuildPacket = (GUILD_PACKET*)&m_PacketModifyDesc; break;

        case GUILD_PACKET_GW_MODIFY_TITLE_NAME: pGuildPacket = (GUILD_PACKET*)&m_PacketModifyTitleName; break;

        case GUILD_PACKET_GW_FAMILY_DISMISS:
        case GUILD_PACKET_CG_FAMILY_DISMISS: pGuildPacket = (GUILD_PACKET*)&m_PacketFamilyDismiss; break;

        case GUILD_PACKET_CG_LEVELUP:
        case GUILD_PACKET_GW_LEVELUP:   pGuildPacket    = (GUILD_PACKET*)&m_PacketLevelUp; break;

        case GUILD_PACKET_GW_ADD_GP:
        case GUILD_PACKET_GW_SUB_GP:    pGuildPacket    = (GUILD_PACKET*)&m_PacketGP; break;

        case GUILD_PACKET_GW_LEVEL_INFO:    pGuildPacket= (GUILD_PACKET*)&m_PacketLevelInfo; break;

		default:
			Assert( FALSE );
			pGuildPacket = NULL;
		}

		return pGuildPacket;
	}

	static UINT				GetPacketMaxSize()
	{
		return (sizeof(UCHAR) + sizeof(u) - sizeof(GUILD_PACKET));
	}

};

struct _GUILD_WGC_PACKET : public _GUILD_PACKET
{
	GUILD_WGC_LIST				m_PacketList;
	GUILD_WGC_MEMBER_LIST		m_PacketMemberList;
	GUILD_WGC_GUILD_INFO		m_PacketGuildInfo;
	GUILD_WGC_APPOINT_INFO		m_PacketAppointList;
	GUILD_WGC_SELF_GUILD_INFO	m_PacketSelfGuildInfo;
	GUILD_WGC_INVITE			m_PacketInvite;
    GUILD_WGC_MODIFY_TITLE_NAME m_PacketTitleName;

    
	union u
	{ // 比最大的消息包长了一个虚表长度
		CHAR				u_ListSize[sizeof(GUILD_WGC_LIST)];
		CHAR				u_MemberListSize[sizeof(GUILD_WGC_MEMBER_LIST)];
		CHAR				u_GuildInfoSize[sizeof(GUILD_WGC_GUILD_INFO)];
		CHAR				u_AppointInfoSize[sizeof(GUILD_WGC_APPOINT_INFO)];
		CHAR				u_SelfGuildInfoSize[sizeof(GUILD_WGC_SELF_GUILD_INFO)];
        CHAR                u_InviteSize[sizeof(GUILD_WGC_INVITE)];
        CHAR                u_TitleNameSize[sizeof(GUILD_WGC_MODIFY_TITLE_NAME)];
	};

	_GUILD_WGC_PACKET() {}

	virtual GUILD_PACKET*	GetPacket(INT nPacketType) const
	{
		GUILD_PACKET*		pGuildPacket;

		switch(nPacketType)
		{
		case GUILD_PACKET_WG_LIST:
		case GUILD_PACKET_GC_LIST: pGuildPacket = (GUILD_PACKET*)&m_PacketList; break;

		case GUILD_PACKET_WG_MEMBER_LIST:
		case GUILD_PACKET_GC_MEMBER_LIST: pGuildPacket = (GUILD_PACKET*)&m_PacketMemberList; break;

		case GUILD_PACKET_WG_GUILD_INFO:
		case GUILD_PACKET_GC_GUILD_INFO: pGuildPacket = (GUILD_PACKET*)&m_PacketGuildInfo; break;

		case GUILD_PACKET_WG_APPOINT_INFO:
		case GUILD_PACKET_GC_APPOINT_INFO: pGuildPacket = (GUILD_PACKET*)&m_PacketAppointList; break;

		case GUILD_PACKET_WG_SELF_GUILD_INFO:
		case GUILD_PACKET_GC_SELF_GUILD_INFO: pGuildPacket = (GUILD_PACKET*)&m_PacketSelfGuildInfo; break;

        case GUILD_PACKET_WG_CONFIRM_GUILD_INVITE:
        case GUILD_PACKET_GC_CONFIRM_GUILD_INVITE: pGuildPacket = (GUILD_PACKET*)&m_PacketInvite; break;
           
        case GUILD_PACKET_WG_MODIFY_TITLE_NAME:
        case GUILD_PACKET_GC_MODIFY_TITLE_NAME: pGuildPacket = (GUILD_PACKET*)&m_PacketTitleName; break;

		default:
			Assert( FALSE );
			pGuildPacket = NULL;
		}

		return pGuildPacket;
	}

	static UINT				GetPacketMaxSize()
	{
		return (sizeof(UCHAR) + sizeof(u) - sizeof(GUILD_PACKET));
	}

};

struct _GUILD_RETURN
{
	CHAR					m_ReturnType;
	GuildID_t				m_GuildID;
	GUID_t					m_GUID; // 需要重设 GuildID 的玩家的 GUID
	GUID_t					m_GUIDChanged; // 需要重设 GuildID 的玩家的 GUID
	BYTE					m_PosID;
	UCHAR					m_SourNameSize;
	CHAR					m_SourName[MAX_CHARACTER_NAME];
	UCHAR					m_DestNameSize;
	CHAR					m_DestName[MAX_CHARACTER_NAME];
	UCHAR					m_GuildNameSize;
	CHAR					m_GuildName[MAX_GUILD_NAME_SIZE];
	UCHAR					m_PositionNameSize;
	CHAR					m_PositionName[MAX_GUILD_POS_NAME_SIZE];
	UCHAR					m_ChangedPositionNameSize;
	CHAR					m_ChangedPositionName[MAX_GUILD_POS_NAME_SIZE];
    CHAR                    m_cFamilyID;
    UCHAR					m_FamilyNameSize;
    CHAR					m_FamilyName[MAX_GUILD_FAMILY_NAME_SIZE];
    UCHAR					m_FamilyDescSize;
    CHAR					m_FamilyDesc[MAX_GUILD_FAMILY_DESC_SIZE];
    UCHAR					m_GuildLevel;
    BOOL					m_bScuess;          //是否成功


	_GUILD_RETURN()
	{
		m_ReturnType	=	GUILD_RETURN_INVALID;
		m_GuildID		=	INVALID_ID;
		m_GUID			=	INVALID_ID;
		m_GUIDChanged	=	INVALID_ID;
		m_PosID			=	0;
		m_SourNameSize	=	0;
        m_cFamilyID     =   INVALID_ID;
        m_FamilyNameSize=   0;
        m_FamilyDescSize=   0;
        m_GuildLevel    =   0;
        m_bScuess       =   FALSE;

		memset((void*)m_SourName, 0, sizeof(m_SourName));
		m_DestNameSize = 0;
		memset((void*)m_DestName, 0, sizeof(m_DestName));
		m_GuildNameSize = 0;
		memset((void*)m_GuildName, 0, sizeof(m_GuildName));
		m_PositionNameSize	=	0;
		memset((void*)m_PositionName, 0, sizeof(m_PositionName));	
		m_ChangedPositionNameSize	=	0;
		memset((void*)m_ChangedPositionName, 0, sizeof(m_ChangedPositionName));	
        memset((void*)m_FamilyName, 0, sizeof(m_FamilyName));
        memset((void*)m_FamilyDesc, 0, sizeof(m_FamilyDesc));
	}

	UINT					GetPacketSize() const
	{
		UINT uSize = sizeof(m_ReturnType);

		switch( m_ReturnType )
		{
        case GUILD_RETURN_INVIT:
            {
                uSize +=	sizeof(m_GuildID)
                          + sizeof(m_GUID)
                          + sizeof(m_GuildNameSize)
                          + m_GuildNameSize;
            }
            break;
		case GUILD_RETURN_CREATE:
		case GUILD_RETURN_JOIN:
			{
				uSize +=	sizeof(m_GuildID)
							+ sizeof(m_GuildNameSize) + m_GuildNameSize;
			}
			break;
		case GUILD_RETURN_RESPONSE:
			{
				uSize += sizeof(m_GuildID) + sizeof(m_GUID) + sizeof(m_SourNameSize) + m_SourNameSize;
			}
			break;
		case GUILD_RETURN_EXPEL:
			{
				uSize +=  sizeof(m_SourNameSize) + m_SourNameSize
						+ sizeof(m_DestNameSize) + m_DestNameSize
						+ sizeof(m_GUID);
			}
			break;
		case GUILD_RETURN_REJECT:
			{
				uSize +=  sizeof(m_GuildNameSize) + m_GuildNameSize
					+ sizeof(m_DestNameSize) + m_DestNameSize
					+ sizeof(m_GUID);
			}
			break;
		case GUILD_RETURN_RECRUIT:
			{
				uSize += sizeof(m_GuildID)
						+ sizeof(m_GUIDChanged)
						+ sizeof(m_SourNameSize) + m_SourNameSize
						+ sizeof(m_DestNameSize) + m_DestNameSize
						+ sizeof(m_GuildNameSize) + m_GuildNameSize
						+ sizeof(m_PositionNameSize) + m_PositionNameSize
						+ sizeof(m_GUID);
			}
			break;
		case GUILD_RETURN_LEAVE:
			{
				uSize +=  sizeof(m_GUID) 
						+ sizeof(m_GuildNameSize) + m_GuildNameSize
						+ sizeof(m_DestNameSize) + m_DestNameSize;
			}
			break;
		case GUILD_RETURN_FOUND:
			{

			}
			break;
		case GUILD_RETURN_DISMISS:
			{
			
			}
			break;
        case GUILD_RETURN_ALREADY_IN:
            {

            }
            break;
        case GUILD_RETURN_ERASE_ITEM_FAIL:
            {
            }
            break;
		case GUILD_RETURN_PROMOTE:
			{
				uSize +=  sizeof(m_SourNameSize) + m_SourNameSize
						+ sizeof(m_DestNameSize) + m_DestNameSize
						+ sizeof(m_PositionNameSize) + m_PositionNameSize
						+ sizeof(m_GuildNameSize) + m_GuildNameSize
						+ sizeof(m_PosID)
						+ sizeof(m_GUID)
						+ sizeof(m_GuildID);
			}
			break;
		case GUILD_RETURN_DEMOTE:
			{
				uSize +=  sizeof(m_SourNameSize) + m_SourNameSize
						+ sizeof(m_DestNameSize) + m_DestNameSize
						+ sizeof(m_PositionNameSize) + m_PositionNameSize
						+ sizeof(m_GuildNameSize) + m_GuildNameSize
						+ sizeof(m_PosID)
						+ sizeof(m_GUID)
						+ sizeof(m_GuildID);
			}
			break;
		case GUILD_RETURN_AUTHORIZE:
		case GUILD_RETURN_DEPRIVE_AUTHORITY:
			{
			}
			break;
		case GUILD_RETURN_DEMISE:
			{
				uSize += sizeof(m_GuildID)
					+ sizeof(m_GUID)
					+ sizeof(m_GUIDChanged)
					+ sizeof(m_SourNameSize) + m_SourNameSize
					+ sizeof(m_DestNameSize) + m_DestNameSize
					+ sizeof(m_GuildNameSize) + m_GuildNameSize
					+ sizeof(m_PositionNameSize) + m_PositionNameSize
					+ sizeof(m_ChangedPositionNameSize) + m_ChangedPositionNameSize;
			}
			break;
		case GUILD_RETURN_NAME:
			{

			}
			break;
		case GUILD_RETURN_WITHDRAW:
		case GUILD_RETURN_DEPOSIT:
		case GUILD_RETURN_CREATE_FAILED:
			{
				
			}
			break;
        case GUILD_RETURN_CREATE_FAMILY:
            {
                uSize += sizeof(m_cFamilyID)
                    + m_FamilyNameSize
                    + m_FamilyDescSize
                    + sizeof(m_FamilyNameSize)
                    + sizeof(m_FamilyDescSize);
            }
            break;
        case GUILD_RETURN_FAMILY_DISMISS:
            {
                uSize += sizeof(m_cFamilyID);
            }
            break;
        case GUILD_RETURN_ADD_FAMILY_USER:
            {
                uSize += sizeof(m_cFamilyID) 
                    + m_SourNameSize
                    + sizeof(m_SourNameSize);
            }
            break;
        case GUILD_RETURN_LEVEL_UP:
            {
                uSize += sizeof(m_GuildLevel)+ sizeof(m_bScuess);
            }
            break;

        case GUILD_RETURN_BEADD_FAMILY:
            {
                uSize += sizeof(m_cFamilyID);
            }
            break;
        case GUILD_RETURN_REMOVE_FAMILY_USER:
            {
                uSize += sizeof(m_cFamilyID)
                    + m_SourNameSize
                    + sizeof(m_SourNameSize);
            }
            break;
        case GUILD_RETURN_MODIFY_DESC:
            {
                uSize += m_SourNameSize
                    + sizeof(m_SourNameSize);
            }
            break;
		default:
			Assert(FALSE);
			return 0;
		}

		return uSize;
	}

	VOID					Read(SocketInputStream& iStream);
	VOID					Write(SocketOutputStream& oStream) const;
};

struct GUILD_POS_t
{
	BYTE	NumPos;
	BYTE	MaxNumPos;
	CHAR	szPosName[MAX_GUILD_POS_NAME_SIZE];
	INT		Posidx[MAX_NUM_PER_POSITION];

	VOID CleanUp()
	{
		NumPos = 0;
		MaxNumPos = 0;
		memset(szPosName, 0, MAX_GUILD_POS_NAME_SIZE);
		for(INT i = 0; i<MAX_NUM_PER_POSITION; i++)
		{
			Posidx[i] = -1;
		}
	}
};

//不同级别帮会对应的不同的行政结构
struct GUILD_ADMIN_t
{
	enum ORESULT
	{
		RET_SUCCESS = 0,
		RET_NOT_EXIST,	//此人在本职位不存在
		RET_POS_FULL,	//职位人都满了
		RET_NO_USER,	//玩家不存在

		RET_UNKOWN,
	};
	enum
	{
		POS_LEVEL0_BEGIN = 0,

		POS_CHAIR_MAN,				//帮主
		POS_ASS_CHAIR_MAN ,			//副帮主
        POS_NORMAL_FAMILY_BOSS,     //家族长
        POS_ELITE_MEMBER,           //精英帮众 已分配帮众
		POS_MEMBER,					//未分配帮众
        
		POS_LEVEL_END,						
	};

	GUILD_POS_t		m_PosList[POS_LEVEL_END];

	GUILD_POS_t*	Get(INT iPos)
	{
		return &m_PosList[iPos];
	}

	ORESULT	Remove(INT iPos, INT iIndex)
	{
		GUILD_POS_t* pGuildPos = Get(iPos);

		for(INT i = 0; i<pGuildPos->MaxNumPos; i++)
		{
			if(pGuildPos->Posidx[i] == iIndex)
			{
				pGuildPos->Posidx[i] = pGuildPos->Posidx[pGuildPos->NumPos-1];
				pGuildPos->Posidx[pGuildPos->NumPos-1] = -1;
				pGuildPos->NumPos--;
				return RET_SUCCESS;
			}
		}
		return RET_NOT_EXIST;
	}


	ORESULT	Add(INT iPos, INT iIndex)
	{
		GUILD_POS_t* pGuildPos = Get(iPos);
		if(pGuildPos->NumPos == pGuildPos->MaxNumPos)
			return RET_POS_FULL;
		pGuildPos->Posidx[pGuildPos->NumPos] = iIndex;
		pGuildPos->NumPos++;
		return RET_SUCCESS;
	}

	VOID CleanUp(INT iLevelBegin = POS_LEVEL0_BEGIN)
	{
		for(INT i = iLevelBegin; i<POS_LEVEL_END; i++)
		{
			m_PosList[i].CleanUp();
		}
	}
};

#endif // __GAMESTRUCT_GUILD_H__
