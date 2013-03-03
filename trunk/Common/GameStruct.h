
#ifndef __GAMESTRUCT_H__
#define __GAMESTRUCT_H__

#include "Type.h"
#include "GameDefine.h"


//#pragma pack(push, 1)

//用来定义在世界的浮点位置
struct WORLD_POS
{
	FLOAT	 m_fX ;
	FLOAT	 m_fZ ;

	WORLD_POS(VOID)					: m_fX(0.0f), m_fZ(0.0f)	{}
	WORLD_POS(FLOAT fX, FLOAT fZ)	: m_fX(fX)	, m_fZ(fZ)		{}
	VOID	CleanUp( ){
		m_fX = 0.0f ;
		m_fZ = 0.0f ;
	};
	WORLD_POS& operator=(WORLD_POS const& rhs)
	{
		m_fX = rhs.m_fX;
		m_fZ = rhs.m_fZ;
		return *this;
	}
	BOOL	operator==(WORLD_POS& Ref)
	{
		return (fabs(m_fX-Ref.m_fX)+fabs(m_fZ-Ref.m_fZ))<0.0001f;

	}
	BOOL	operator==(const WORLD_POS& Ref)
	{
		return (fabs(m_fX-Ref.m_fX)+fabs(m_fZ-Ref.m_fZ))<0.0001f;

	}
};

//用来定义在世界的网格位置
struct MAP_POS
{
	Coord_t	 m_nX ;
	Coord_t	 m_nZ ;

	MAP_POS(VOID)					: m_nX(0)	, m_nZ(0)	{}
	MAP_POS(Coord_t nX, Coord_t nZ)	: m_nX(nX)	, m_nZ(nZ)	{}
	VOID	CleanUp( ){
		m_nX = 0 ;
		m_nX = 0 ;
	};
};




//效果状态
struct _EFFECT
{
	BOOL				m_bActive ;
	INT					m_Value ;	//效果值
	INT					m_Time ;	//效果时间

	_EFFECT( )
	{
		CleanUp( ) ;
	}

	VOID				CleanUp( ){
		m_bActive = FALSE ;
		m_Value = 0 ;
		m_Time = 0 ;
	};

	BOOL				IsActive( ){ return m_bActive ; } ;
	VOID				SetActive( BOOL bActive ){ m_bActive = bActive ; } ;
};

//怪物生成器初始化数据
struct _MONSTERCREATER_INIT
{
	CHAR				m_FileName[_MAX_PATH] ;
	WORLD_POS			m_Position ;
};


#define			DEFAULT_ITEMBOX_RECYCLE_TIME	300000		//300秒，5分钟



//装备定义
struct EQUIP_LIST
{
	GUID_t	m_GUID;			//装备类型ID
	UINT	m_uParam1;		//装备属性1
	UINT	m_uParam2;		//装备属性2
};
#define EQUIP_PLAYER_FIXNUM		(8)		//玩家身上最多可佩戴的装备数

//饰品定义
struct EMBELLISH_LIST
{
	GUID_t	m_GUID;			//饰品类型ID
	UINT	m_uParam1;		//饰品属性1
	UINT	m_uParam2;		//饰品属性2
};
#define EMBELLISH_PLAYER_FIXNUM		(6)		//玩家身上最多可佩戴的饰品数



//玩家基本序列化信息
struct PLAYER_OWN
{
	GUID_t    m_nGUID;						//玩家完全唯一ID
	CHAR	  m_szName[MAX_CHARACTER_NAME];	//玩家姓名
	
	Coord_t   m_nX;						//玩家位置X
	Coord_t   m_nZ;						//玩家位置Z

	FLOAT     m_fDir;						//玩家面朝的方向（范围：0~1.0）
										//
										//       0.25 
										//    \   |   /
										// 0.5  \ | /
										//   ----   ----  0.0 (1.0f)
										//      / | \
										//    /   |   \
										//       0.75

	PLAYER_OWN( )
	{
		m_nGUID = INVALID_ID ;
		memset( m_szName, 0, MAX_CHARACTER_NAME ) ;
		m_nX = 0 ;
		m_nZ = 0 ;
		m_fDir = 0.0 ;
	};
};

struct PLAYER_S
{
	GUID_t    m_nGUID;						//玩家完全唯一ID
	CHAR	  m_szName[MAX_CHARACTER_NAME];	//玩家姓名
	
	Coord_t   m_nX;						//玩家位置X
	Coord_t   m_nZ;						//玩家位置Z

	FLOAT     m_fDir;						//玩家面朝的方向（范围：0~1.0）
										//
										//       0.25 
										//    \   |   /
										// 0.5  \ | /
										//   ----   ----  0.0 (1.0f)
										//      / | \
										//    /   |   \
										//       0.75

	PLAYER_S( )
	{
		m_nGUID = INVALID_ID ;
		memset( m_szName, 0, MAX_CHARACTER_NAME ) ;
		m_nX = 0 ;
		m_nZ = 0 ;
		m_fDir = 0.0 ;
	};
};

struct VRECT
{
	INT nStartx ;
	INT nStartz ;
	INT nEndx ;
	INT nEndz ;

	VRECT( )
	{
		nStartx = 0 ;
		nStartz = 0 ;
		nEndx = 0 ;
		nEndz = 0 ;
	};

	BOOL IsContinue( INT x, INT z )const
	{
		if ( x < nStartx || x > nEndx || z < nStartz || z > nEndz )
			return FALSE;
		else
			return TRUE;
	}
};

//一级战斗属性结构
struct _ATTR_LEVEL1
{
	INT		m_pAttr[CATTR_LEVEL1_NUMBER] ;

	_ATTR_LEVEL1( )
	{
		CleanUp( ) ;
	};
	INT	Get( INT iAttr )const{
		Assert( iAttr>=0 && iAttr<CATTR_LEVEL1_NUMBER ) ;
		return m_pAttr[iAttr] ;
	};  
	VOID Set( INT iAttr, INT iValue ){
		Assert( iAttr>=0 && iAttr<CATTR_LEVEL1_NUMBER ) ;
		m_pAttr[iAttr] = iValue ;
	} ;
	VOID CleanUp()
	{
		memset( m_pAttr, 0, sizeof(INT)*CATTR_LEVEL1_NUMBER ) ;
	} ;
};

//二级战斗属性结构
struct _ATTR_LEVEL2
{
	INT		m_pAttr[CATTR_LEVEL2_NUMBER] ;

	_ATTR_LEVEL2( )
	{
		memset( m_pAttr, 0, sizeof(INT)*CATTR_LEVEL2_NUMBER ) ;
	}
	INT	Get( INT iAttr ){
		Assert( iAttr>=0 && iAttr<CATTR_LEVEL2_NUMBER ) ;
		return m_pAttr[iAttr] ;
	};
	VOID Set( INT iAttr, INT iValue ){
		Assert( iAttr>=0 && iAttr<CATTR_LEVEL2_NUMBER ) ;
		m_pAttr[iAttr] = iValue ;
	} ;
};

//角色所拥有的称号
//#开头的字符串代表是一个字符串资源ID,必须通过表格索引，服务器不用保留这个表格
#define IDTOSTRING(str, strid, strsize)	char str[strsize];\
										memset(str, 0, strsize);\
										sprintf(str, "#%d", strid);\

#define STRINGTOID(str, strid)			INT strid = atoi((CHAR*)(str+1));\
										

struct _TITLE
{
	enum
	{
		NO_TITLE	 = -1,

		GUOJIA_TITLE = 1,       //国家称号
		BANGPAI_TITLE,          //帮派称号
		WANFA_TITLE,            //玩法称号
		MOOD_TITLE,				//玩家心情

		MAX_NUM_TITLE,
	};

    struct TITLE_INFO
    {
        INT     m_iTitleID;
        INT     m_iSuitID;      //组合称号ID    单一称号填-1
        INT     m_iTitleType;   //称号类型     国家，帮会，玩法称号
        INT     m_iBuffID;      //称号的BUFFid
        UINT    m_uTime;        //时限title到期时间，无限期的用0  

		//CHAR				m_szFemaleName[32];
		//CHAR                m_szMaleName[32];

        TITLE_INFO()
        {
            m_iTitleID  = INVALID_ID;
            m_iSuitID   = INVALID_ID;
            m_iTitleType= INVALID_ID;
            m_iBuffID   = INVALID_ID;
			m_uTime     = 0;
			
			//memset(m_szFemaleName, 0, sizeof(m_szFemaleName));
			//memset(m_szMaleName, 0, sizeof(m_szMaleName));
		}
    };

    struct TITLE_COMBINATION                        //组成称号信息
    {
        INT m_iGroupID;
        INT m_comTitleID;
        INT m_arPart[MAX_TITLE_COMBINATION];        //组合成员上限10个

        TITLE_COMBINATION ()
        {
            m_comTitleID = INVALID_ID;
            memset(m_arPart, 0, sizeof(INT)*MAX_TITLE_COMBINATION);
        }
    };


public:

    INT                 m_CurCountryTitle;                              //当前国家称号ID  无效均为-1
    INT                 m_CurGuildTitle;                                //当前帮派称号ID  无效均为-1  
    INT                 m_CurNormalTitle;                               //当前普通称号ID  无效均为-1
    TITLE_INFO          m_TitleArray[MAX_TITLE_SIZE];

    CHAR                m_szCurCountryTitle[MAX_CHARACTER_TITLE];      //当前国家称号
    CHAR                m_szCurGuildTitle[MAX_CHARACTER_TITLE];        //当前帮会称号
    CHAR                m_szCurNormalTitle[MAX_CHARACTER_TITLE];       //当前玩法称号

    CHAR                m_szOfficialTitleName[MAX_CHARACTER_TITLE];    //自定义官职称号

	VOID	CleanUp()
	{
        m_CurCountryTitle   = -1;
        m_CurGuildTitle     = -1;  
        m_CurNormalTitle    = -1; 

        memset((void*)m_TitleArray, -1, sizeof(TITLE_INFO)*MAX_TITLE_SIZE);
        memset((void*)m_szOfficialTitleName, 0, MAX_CHARACTER_TITLE);
        memset((void*)m_szCurCountryTitle, 0, MAX_CHARACTER_TITLE);
        memset((void*)m_szCurGuildTitle, 0, MAX_CHARACTER_TITLE);
        memset((void*)m_szCurNormalTitle, 0, MAX_CHARACTER_TITLE);
	}
};
//物品拾取控制
struct	ITEM_PICK_CTL
{
	ObjID_t				OwnerID;			//最终的拾取者ID
	uint				uBetTime;			//系统赌博时间
	UCHAR				uMaxBetPoint;		//最大Bet点数
	PICK_RULER			ePickRuler;			//系统控制符号


	ITEM_PICK_CTL()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		OwnerID			=	INVALID_ID;		//无所有者
		ePickRuler		=	IPR_FREE_PICK;	//自由拾取
		uBetTime		=	0;				//可以拾取
		uMaxBetPoint	=	0;
	}
};

typedef	ITEM_PICK_CTL	IPC;

#define MAX_PICKER_COUNT	6


//队伍能参与拾取的人员列表
struct TEAM_PICKER
{
	UINT		m_uCount;
	ObjID_t		m_PickerID[MAX_PICKER_COUNT];

	TEAM_PICKER()
	{
		memset(this,0,sizeof(TEAM_PICKER));
	}

	VOID	AddPicker(ObjID_t id)
	{
		for(UINT nIndex=0;nIndex<m_uCount;nIndex++)
		{
			if(m_PickerID[nIndex]==id)
				return;
		}
		m_PickerID[m_uCount] = id;
		m_uCount++;

	}


};




//最大伤害纪录
#define		MAX_DAMAGE_REC_COUNT	10


//伤害纪录
struct DAMAGE_RECORD
{
	GUID_t		m_Killer;
	ObjID_t		m_KillerObjID;
	TeamID_t	m_TeamID;
	UINT		m_uDamage;
	
	DAMAGE_RECORD()
	{
		CleanUp();
	}
	void	CleanUp()
	{
		m_Killer	  = INVALID_ID;
		m_KillerObjID = INVALID_ID;
		m_TeamID	  = INVALID_ID;
		m_uDamage	  = 0;
	}
};



//伤害队列
struct DAMAGE_MEM_LIST
{
	UINT				m_uCount;
	DAMAGE_RECORD		m_DamageRec[MAX_DAMAGE_REC_COUNT];
	
	DAMAGE_MEM_LIST()
	{
		CleanUp();
	}

	void	CleanUp()
	{
		m_uCount	=	0;
		for(int i = 0;i<MAX_DAMAGE_REC_COUNT;i++)
			m_DamageRec[i].CleanUp();
	}
	
	void	AddMember(GUID_t	KillerID,  ObjID_t KillerObjID,  TeamID_t KillerTeam, UINT	Damage)
	{
		if(KillerTeam!=INVALID_ID)
		{
			m_DamageRec[m_uCount].m_Killer			=	KillerID;
			m_DamageRec[m_uCount].m_KillerObjID		=   KillerObjID;
			m_DamageRec[m_uCount].m_TeamID			=	KillerTeam;
			m_DamageRec[m_uCount].m_uDamage			=	Damage;
		}
		else
		{
			m_DamageRec[m_uCount].m_Killer			=	KillerID;
			m_DamageRec[m_uCount].m_KillerObjID		=   KillerObjID;
			m_DamageRec[m_uCount].m_TeamID			=	INVALID_ID;
			m_DamageRec[m_uCount].m_uDamage			=	Damage;
		}

		m_uCount++;
	}

	void	AddMember(DAMAGE_RECORD&	dRec)
	{
		if(dRec.m_TeamID!=INVALID_ID)
		{
			m_DamageRec[m_uCount].m_Killer	=	dRec.m_Killer;
			m_DamageRec[m_uCount].m_TeamID	=	dRec.m_TeamID;
			m_DamageRec[m_uCount].m_uDamage	=	dRec.m_uDamage;
		}
		else
		{
			m_DamageRec[m_uCount].m_Killer	=	dRec.m_Killer;
			m_DamageRec[m_uCount].m_TeamID	=	INVALID_ID;
			m_DamageRec[m_uCount].m_uDamage	=	dRec.m_uDamage;
		}
		m_uCount++;
	}
	
	DAMAGE_RECORD*	FindMember(GUID_t KillerID)
	{
		for(UINT i =0;i<m_uCount;i++)
		{
			if(m_DamageRec[i].m_Killer == KillerID && KillerID!=INVALID_ID)
			{
					return &m_DamageRec[i];
					
			}
		}
		return NULL;
	}
};


struct _OWN_ABILITY 
{
	// AbilityID_t	m_Ability_ID; 不需要 ID，索引就是 ID
	WORD		m_Level; // 技能等级
	WORD		m_Exp; // 技能熟练度
};


#define		MAX_MONSTER_DROP_TASK_ITEM		5		
#define		MAX_MONSTER_KILLER_NUM			18

struct CHAR_OWNER_DROP_LIST 
{
	ObjID_t		HumanID;
	UINT		DropItemIndex[MAX_MONSTER_DROP_TASK_ITEM];
	UINT		DropCount;

	CHAR_OWNER_DROP_LIST()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset(this,0,sizeof(*this));
	}

	VOID	AddItem(UINT ItemIndex)
	{
		Assert(DropCount<MAX_MONSTER_DROP_TASK_ITEM);
		DropItemIndex[DropCount] = ItemIndex;
		DropCount++;
	}
};

struct OWNERCHARACTER 
{
	GUID_t		m_Guid;
	ObjID_t		m_ObjID;
};

struct MONSTER_OWNER_LIST 
{
	OWNERCHARACTER		OwnerDropList[MAX_TEAM_MEMBER];
	UINT				OwnerCount;

	MONSTER_OWNER_LIST()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset(this,0,sizeof(*this));
	}

	VOID	AddOwner(GUID_t	 HumanID, ObjID_t HumanObjID)
	{
		if(OwnerCount == MAX_TEAM_MEMBER)
			return;
		BOOL bAlive = FALSE;
		for(UINT i=0; i<OwnerCount; ++i)
		{
			if(OwnerDropList[i].m_Guid == HumanID)
			{
				bAlive = TRUE;
			}
		}
		if(!bAlive)
		{
			OwnerDropList[OwnerCount].m_Guid	=	HumanID;
			OwnerDropList[OwnerCount].m_ObjID   =   HumanObjID;
			OwnerCount++;
		}
	}
};



struct RELATION_MEMBER
{
	GUID_t			m_MemberGUID ;
	CHAR			m_szMemberName[MAX_CHARACTER_NAME] ;
	INT				m_nLevel;							//角色等级
	INT				m_nMenPai;							//门派 MENPAI_ATTRIBUTE
	INT				m_nPortrait;						// 头像
	GuildID_t		m_GuildID;							//帮会ID
	
	struct ReMember_ExtData
	{
		INT			m_nLevel;							//角色等级
		INT			m_nMenPai;							//门派 MENPAI_ATTRIBUTE
		INT			m_nPortrait;						//头像
		GuildID_t	m_GuildID;							//帮会ID
	};

	RELATION_MEMBER( )
	{
		CleanUp( );
	};

	VOID CleanUp( )
	{
		m_MemberGUID = INVALID_ID;
		memset( m_szMemberName, 0, sizeof(m_szMemberName) );
		m_nLevel = 0;
		m_nMenPai = INVALID_JOB;
		m_nPortrait = -1;
		m_GuildID = INVALID_ID;
	};

	ReMember_ExtData	GetExtData()
	{
		ReMember_ExtData ExtData;
		
		ExtData.m_nLevel = m_nLevel;
		ExtData.m_nMenPai = m_nMenPai;
		ExtData.m_nPortrait = m_nPortrait;
		ExtData.m_GuildID = m_GuildID;
		return ExtData;
	}

	VOID			 SetExtData(ReMember_ExtData& ExtData)
	{
		m_nLevel = ExtData.m_nLevel;
		m_nMenPai = ExtData.m_nMenPai;
		m_nPortrait = ExtData.m_nPortrait;
		m_GuildID = ExtData.m_GuildID;
	}
};
//配偶信息
struct MarriageInfo
{
	GUID_t		m_SpouseGUID;								// 配偶的 GUID
//	UINT		m_uWeddingTime;								// 婚礼时间

	MarriageInfo()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		m_SpouseGUID = INVALID_ID;
	}
};
//徒弟信息
struct PrenticeInfo
{
//	UINT		m_uRecruitingTime;							// 收徒时间
	time_t		m_BetrayingTime;							// 最后一次叛师时间
	UINT		m_uMoralPoint;								// 师德点
	UCHAR		m_uPrenticeCount;							// 徒弟数量
	GUID_t		m_PrenticeGUID[MAX_PRENTICE_COUNT];			// 徒弟的 GUID

	PrenticeInfo()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		m_BetrayingTime = 0;
		m_uMoralPoint = 0;
		m_uPrenticeCount = 0;

		for( INT i=0; i<MAX_PRENTICE_COUNT; ++i )
		{
			m_PrenticeGUID[i] = INVALID_ID;
		}
	}

};
//师傅信息
struct MasterInfo
{
	GUID_t		m_MasterGUID;								// 师傅的 GUID
//	UINT		m_uApprenticingTime;						// 拜师时间
//	UINT		m_uBetrayingTime;							// 上次叛师时间
//	UINT		m_uBetrayTimes;								// 叛师次数

	MasterInfo()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		m_MasterGUID = INVALID_ID;
	}

};

class SocketOutputStream ;
class SocketInputStream ;
//邮件
struct MAIL
{

	struct MailInfo
	{
		GUID_t		m_GUID;								// 发信人 GUID
		BYTE		m_SourSize ;
		INT			m_nPortrait;						// 发信人头像
		BYTE		m_DestSize ;
		WORD		m_ContexSize ;
		UINT		m_uFlag ;							//邮件标志 enum MAIL_TYPE
		time_t		m_uCreateTime ;						//邮件创建时间
		//执行邮件应用参数
		UINT		m_uParam0 ;
		UINT		m_uParam1 ;
		UINT		m_uParam2 ;
		UINT		m_uParam3 ;
	};
	
	VOID	GetMailInfo(MailInfo& mInfo)
	{
		mInfo.m_GUID = m_GUID;
		mInfo.m_SourSize	 = m_SourSize;
		mInfo.m_nPortrait	 = m_nPortrait;
		mInfo.m_DestSize	 = m_DestSize;
		mInfo.m_ContexSize	 = m_ContexSize;
		mInfo.m_uFlag		 = m_uFlag;
		mInfo.m_uCreateTime	 = m_uCreateTime;
		mInfo.m_uParam0		 = m_uParam0;
		mInfo.m_uParam1		 = m_uParam1;
		mInfo.m_uParam2		 = m_uParam2;
		mInfo.m_uParam3		 = m_uParam3;
	}

	VOID	SetMailInfo(MailInfo& mInfo)
	{
		m_GUID			= 	mInfo.m_GUID;		
		m_SourSize		= 	mInfo.m_SourSize;	
		m_nPortrait	= 	mInfo.m_nPortrait;	
		m_DestSize		= 	mInfo.m_DestSize;	
		m_ContexSize	= 	mInfo.m_ContexSize;	
		m_uFlag		= 	mInfo.m_uFlag;		
		m_uCreateTime	= 	mInfo.m_uCreateTime;	
		m_uParam0		= 	mInfo.m_uParam0;		
		m_uParam1		= 	mInfo.m_uParam1;		
		m_uParam2		= 	mInfo.m_uParam2;		
		m_uParam3		= 	mInfo.m_uParam3;		
	}

	GUID_t		m_GUID;								// 发信人 GUID
	BYTE		m_SourSize ;
	CHAR		m_szSourName[MAX_CHARACTER_NAME] ;	//发信人

	INT			m_nPortrait;						// 发信人头像

	BYTE		m_DestSize ;
	CHAR		m_szDestName[MAX_CHARACTER_NAME] ;	//收信人

	WORD		m_ContexSize ;
	CHAR		m_szContex[MAX_MAIL_CONTEX] ;		//内容

	UINT		m_uFlag ;							//邮件标志 enum MAIL_TYPE
	time_t		m_uCreateTime ;						//邮件创建时间

	//执行邮件应用参数
	UINT		m_uParam0 ;
	UINT		m_uParam1 ;
	UINT		m_uParam2 ;
	UINT		m_uParam3 ;

	MAIL( )
	{
		CleanUp( ) ;
	};
	VOID CleanUp( )
	{
		m_GUID = INVALID_INDEX;
		m_SourSize = 0 ;
		memset( m_szSourName, 0, sizeof(CHAR)*MAX_CHARACTER_NAME ) ;

		m_nPortrait = -1;

		m_DestSize = 0 ;
		memset( m_szDestName, 0, sizeof(CHAR)*MAX_CHARACTER_NAME ) ;

		m_ContexSize = 0 ;
		memset( m_szContex, 0, sizeof(CHAR)*MAX_MAIL_CONTEX ) ;

		m_uFlag = MAIL_TYPE_NORMAL ;
		m_uCreateTime = 0 ;

		m_uParam0 = 0 ;
		m_uParam1 = 0 ;
		m_uParam2 = 0 ;
		m_uParam3 = 0 ;
	};
	VOID Read( SocketInputStream& iStream ) ;
	VOID Write( SocketOutputStream& oStream ) const ;
};


#define MAX_MAIL_SIZE 20
struct MAIL_LIST
{
	MAIL		m_aMail[MAX_MAIL_SIZE] ;
	BYTE		m_Count ;//邮件数量
	BYTE		m_TotalLeft ;//用户帐号里的邮件剩余数量

	MAIL_LIST( )
	{
		CleanUp( ) ;
	};
	VOID CleanUp( )
	{
		m_Count = 0 ;
		m_TotalLeft = 0 ;
		for( INT i=0;i<MAX_MAIL_SIZE; i++ )
		{
			m_aMail[i].CleanUp() ;
		}
	};
	VOID Read( SocketInputStream& iStream ) ;
	VOID Write( SocketOutputStream& oStream ) const ;
};

// 批量邮件，指发送给不同人的同内容邮件
#define MAX_RECEIVER 100
struct BATCH_MAIL
{
	GUID_t				m_GUID;								// GUID

	BYTE				m_SourSize;
	CHAR				m_szSourName[MAX_CHARACTER_NAME];	//发信人

	BYTE				m_ReceiverCount;					//收信人数量
	struct
	{
		BYTE			m_DestSize;
		CHAR			m_szDestName[MAX_CHARACTER_NAME];	//收信人
	}m_Receivers[MAX_RECEIVER];

	WORD				m_ContentSize;
	CHAR				m_szContent[MAX_MAIL_CONTEX];		//内容

	UCHAR				m_uFlag;							//邮件标志 enum MAIL_TYPE
	time_t				m_uCreateTime;						//邮件创建时间

	BATCH_MAIL()		{ CleanUp(); }

	GUID_t				GetGUID( )
	{
		return m_GUID;
	}
	VOID				SetGUID( GUID_t guid )
	{
		m_GUID = guid;
	}

	const CHAR*			GetSourName()
	{
		return m_szSourName;
	}
	VOID				SetSourName( const CHAR* szName )
	{
		strncpy(m_szSourName, szName, MAX_CHARACTER_NAME - 1);
		m_SourSize = (UCHAR)strlen(m_szSourName);
	}

	BYTE				GetReceiverCount()
	{
		return m_ReceiverCount;
	}
	const CHAR*			GetDestName(BYTE idx)
	{
		if( idx >= m_ReceiverCount )
		{
			Assert( idx );
			return NULL;
		}

		return m_Receivers[idx].m_szDestName;
	}
	VOID				AddDestName( const CHAR* szName )
	{
		strncpy(m_Receivers[m_ReceiverCount].m_szDestName, szName, MAX_CHARACTER_NAME - 1);
		m_Receivers[m_ReceiverCount].m_DestSize = (UCHAR)strlen(m_Receivers[m_ReceiverCount].m_szDestName);
		++m_ReceiverCount;
	}

	const CHAR*			GetMailContent()
	{
		return m_szContent;
	}
	VOID				SetMailContent( const CHAR* szContent )
	{
		strncpy(m_szContent, szContent, MAX_MAIL_CONTEX - 1);
		m_ContentSize = (UCHAR)strlen(m_szContent);
	}

	UCHAR				GetMailFlag()				
	{
		return m_uFlag;
	}
	VOID				SetMailFlag(UCHAR uFlag)
	{
		m_uFlag = uFlag;
	}

	time_t				GetCreateTime()
	{
		return m_uCreateTime;
	}
	VOID				SetCreateTime(time_t uCreateTime)
	{
		m_uCreateTime = uCreateTime;
	}

	VOID				CleanUp();
	UINT				GetSize() const;
	VOID				Read( SocketInputStream& iStream );
	VOID				Write( SocketOutputStream& oStream ) const;
};


struct USER_SIMPLE_DATA
{
	CHAR				m_Name[MAX_CHARACTER_NAME];		            // 此用户的角色名字
	CHAR				m_Account[MAX_ACCOUNT_LENGTH];	// 此角色所在账号
	GUID_t				m_GUID;							            // 此用户的唯一号
	INT                 m_nCountry;                                 // 国家
	UINT				m_uMenPai;						            // 门派
	INT					m_nPortrait;					            // 头像
	UCHAR				m_uFaceMeshID;					            // 脸部模型
	UCHAR				m_uHairMeshID;					            // 头发模型
	UINT				m_uHairColor;					            // 发色
	INT					m_nLevel;						            // 级别
	USHORT				m_uSex;							            // 性别
	CHAR				m_szTitle[MAX_CHARACTER_TITLE];	            // 称号
	GuildID_t			m_GuildID;						            // 帮会 ID
    CHAR                m_szGuildName[MAX_GUILD_NAME_SIZE];         // 帮会名字
    CHAR                m_szFamilyName[MAX_GUILD_FAMILY_NAME_SIZE]; // 家族名字
	INT					m_iPostCode;								// 邮编号

	UINT				m_uMoney;									//角色身上货币
	UINT				m_uBankMoney;								//角色银行货币

	USER_SIMPLE_DATA( )
	{
		CleanUp( ) ;
	}
	VOID CleanUp( )
	{
		memset( m_Name, 0, sizeof(m_Name) );
		memset( m_Account, 0, sizeof(m_Account) );		
		m_GUID = INVALID_ID;
		m_nCountry = INVALID_COUNTRY;
		m_uMenPai = INVALID_JOB;
		m_nPortrait = -1;
		m_nLevel = 0;
		m_uSex = 0;
		memset( m_szTitle, 0, sizeof(m_szTitle) );
        memset( m_szGuildName, 0, sizeof(m_szGuildName));
        memset( m_szFamilyName, 0, sizeof(m_szFamilyName));
		m_GuildID = INVALID_ID;
		m_iPostCode = 0;
		m_uMoney     = 0;
		m_uBankMoney = 0;
	}
};

#define		MAX_SQL_LENGTH				4096
#define		MAX_LONG_SQL_LENGTH			204800

struct DB_QUERY
{
	UCHAR	m_SqlStr[MAX_SQL_LENGTH];		//执行的Sql语句
	
	VOID	Clear()
	{
		memset(m_SqlStr,0,MAX_SQL_LENGTH);
	}

	VOID	Parse(const CHAR* pTemplate,...);
	
};

struct LONG_DB_QUERY
{
	UCHAR	m_SqlStr[MAX_LONG_SQL_LENGTH];		//执行的Sql语句

	VOID	Clear()
	{
		memset(m_SqlStr,0,MAX_LONG_SQL_LENGTH);
	}

	VOID	Parse(const CHAR* pTemplate,...);
};

struct DB_CHAR_EQUIP_LIST
{
	DB_CHAR_EQUIP_LIST()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset(this,0,sizeof(*this));
	}
	UINT		m_Equip[HEQUIP_NUMBER];			//装备
};

struct DB_CHAR_BASE_INFO
{

	DB_CHAR_BASE_INFO()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset(this,0,sizeof(*this));
		
		m_Menpai	= INVALID_JOB;						//角色门派
		m_Country	= INVALID_COUNTRY;                  //国家
		m_Sex		= INVALID_SEX;
	}

	GUID_t				m_GUID;							//角色全局编号
	BYTE				m_Sex;							//性别
	CHAR				m_Name[MAX_CHARACTER_NAME];		//角色名字
	INT					m_Level;						//角色等级
	UINT				m_HairColor;					//头发颜色	
	BYTE				m_FaceColor;					//脸形颜色
	BYTE				m_HairModel;					//头发模型
	BYTE				m_FaceModel;					//脸形模型
	SceneID_t			m_StartScene;					//角色所在场景
	INT					m_Menpai;						//角色门派
	INT					m_HeadID;						//头部编号
	DB_CHAR_EQUIP_LIST	m_EquipList;					//装备列表
	INT                 m_Country;                      //国家
};

// 队员列表中的队员信息项
struct TEAM_LIST_ENTRY
{
	GUID_t				m_GUID;							// GUID
	SceneID_t			m_SceneID;						// 场景ID
	ID_t				m_SceneResID;					// 场景资源 ID	
	UINT				m_ExtraID;						// 队员的 PlayerID(WG) 或 ObjID(GC)
	UCHAR				m_NameSize;						// 姓名长度
	CHAR				m_Name[MAX_CHARACTER_NAME];		// 队员的名字
	INT					m_nPortrait;					// 头像
	USHORT				m_uDataID;						// 队员的性别
	UINT				m_uFamily;						// 2.门派

	TEAM_LIST_ENTRY( )
	{
		CleanUp( );
	};

	VOID				CleanUp( )
	{
		m_GUID			= INVALID_ID;
		m_SceneID		= INVALID_ID;
		m_SceneResID	= INVALID_ID;
		m_ExtraID		= INVALID_ID;
		m_NameSize		= 0;
		memset			(m_Name, 0, sizeof(m_Name));
		m_nPortrait		= -1;
		m_uDataID		= 0;
		m_uFamily		= 0;
	};

	TEAM_LIST_ENTRY&	operator= ( const TEAM_LIST_ENTRY& entry )
	{
		m_GUID			= entry.m_GUID;
		m_SceneID		= entry.m_SceneID;
		m_SceneResID	= entry.m_SceneResID;
		m_ExtraID		= entry.m_ExtraID;
		m_NameSize		= entry.m_NameSize;
		strncpy			( m_Name, entry.m_Name, sizeof(m_Name) - 1 );
		m_nPortrait		= entry.m_nPortrait;
		m_uDataID		= entry.m_uDataID;
		m_uFamily		= entry.m_uFamily;
		return			*this;
	}

	VOID				SetGUID( GUID_t guid ) { m_GUID = guid; }
	GUID_t				GetGUID( ) const { return m_GUID; }

	VOID				SetSceneID( SceneID_t SceneID ) { m_SceneID = SceneID; }
	SceneID_t			GetSceneID( ) const { return m_SceneID; }

	VOID				SetSceneResID( ID_t SceneID ) { m_SceneResID = SceneID; }
	ID_t				GetSceneResID( ) const { return m_SceneResID; }

	VOID				SetExtraID( UINT id ) { m_ExtraID = id; }
	UINT				GetExtraID( ) const { return m_ExtraID; }

	VOID				SetName( const CHAR* pName )
	{
		strncpy			( m_Name, pName, MAX_CHARACTER_NAME-1 );
		m_NameSize		= (UCHAR)strlen(m_Name);
	}
	const CHAR*			GetName( ) const { return m_Name; }

	VOID				SetIcon( INT icon ) { m_nPortrait = icon; }
	INT					GetIcon( ) const { return m_nPortrait; }

	VOID				SetDataID(USHORT dataid) { m_uDataID = dataid; }
	USHORT				GetDataID() const { return m_uDataID; }

	VOID				SetFamily(UINT uFamily) { m_uFamily = uFamily; }
	UINT				GetFamily() const { return m_uFamily; }

	UINT				GetSize() const;
	VOID				Read( SocketInputStream& iStream );
	VOID				Write( SocketOutputStream& oStream ) const;
};

//ID List
typedef struct _ObjID_List
{
	enum
	{
		MAX_LIST_SIZE = 512,
	};
	_ObjID_List()
	{
		CleanUp();
	}
	VOID CleanUp(VOID)
	{
		m_nCount=0;
		memset((VOID*)m_aIDs, INVALID_ID, sizeof(m_aIDs));
	}
	INT m_nCount;
	ObjID_t m_aIDs[MAX_LIST_SIZE];
} ObjID_List;

//玩家商店的唯一ID
struct _PLAYERSHOP_GUID
{
	ID_t	m_World ;		//世界号: 
	ID_t	m_Server ;		//服务端程序号：
	ID_t	m_Scene ;		//场景号
	INT		m_PoolPos ;		//数据池位置

	_PLAYERSHOP_GUID()
	{
		Reset();
	}
	_PLAYERSHOP_GUID& operator=(_PLAYERSHOP_GUID const& rhs)
	{
		m_PoolPos	= rhs.m_PoolPos;
		m_Server	= rhs.m_Server;
		m_World		= rhs.m_World;
		m_Scene		= rhs.m_Scene;
		return *this;
	}
	BOOL	operator ==(_PLAYERSHOP_GUID& Ref)	const
	{
		return (Ref.m_Scene==m_Scene)&&(Ref.m_PoolPos==m_PoolPos)&&(Ref.m_Server==m_Server)&&(Ref.m_World==m_World);
	}
	BOOL	isNull() const
	{
		return (m_Scene ==INVALID_ID)&&(m_World ==INVALID_ID)&&(m_PoolPos==-1)&&(m_Server == INVALID_ID);
	}
	VOID	Reset()
	{
		m_PoolPos = -1;
		m_Server = INVALID_ID;
		m_World	 = INVALID_ID;	
		m_Scene	 = INVALID_ID;
	}
};

enum  SM_COMMANDS
{
	CMD_UNKNOW,
	CMD_SAVE_ALL,
	CMD_CLEAR_ALL,

};


struct SM_COMMANDS_STATE
{
	SM_COMMANDS	cmdType;	
	union 
	{
		INT		iParam[6];
		FLOAT	fParam[6];
		CHAR	cParam[24];
	};
};


struct GLOBAL_CONFIG
{
	GLOBAL_CONFIG()
	{
		Commands.cmdType = CMD_UNKNOW;
	}
	SM_COMMANDS_STATE		Commands;
};


//密保相关
#define MIBAOUNIT_NAME_LENGTH			2	//每个键值的长度
#define MIBAOUNIT_VALUE_LENGTH			2	//每个数值的长度

#define MIBAOUNIT_NUMBER				3	//密保卡一组有效数据的密保单元个数

//7×7
#define MIBAO_TABLE_ROW_MAX				7	//密保使用的表的最大行数
#define MIBAO_TABLE_COLUMN_MAX			7	//密保使用的表的最大列数

//密保单元
struct MiBaoUint
{
	BYTE			row;
	BYTE			column;

	CHAR			key[MIBAOUNIT_NAME_LENGTH+1];
	CHAR			value[MIBAOUNIT_VALUE_LENGTH+1];

	MiBaoUint()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		row = column = BYTE_MAX;
		memset(key,0,MIBAOUNIT_NAME_LENGTH+1);
		memset(value,0,MIBAOUNIT_VALUE_LENGTH+1);
	}

	BOOL	IsSame(const MiBaoUint& mu)		const
	{
		if(row >= MIBAO_TABLE_ROW_MAX || column >= MIBAO_TABLE_COLUMN_MAX)	return FALSE;
		return (row == mu.row && column == mu.column)?TRUE:FALSE;
	}
};

//创建人物验证码
struct CreateCode
{
	USHORT			code[ANASWER_LENGTH_1];

	CreateCode()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset(code,0,sizeof(USHORT)*ANASWER_LENGTH_1);
	}

	BOOL	IsSame(CreateCode* pcd)
	{
		//CreateCode tmp;
		//if(memcmp(code,tmp.code,sizeof(USHORT)*ANASWER_LENGTH_1) == 0)
		//	return FALSE;

		if(!pcd) return FALSE;
		return (memcmp(code,pcd->code,sizeof(USHORT)*ANASWER_LENGTH_1) == 0);
	}
};


//一组密保数据
struct MiBaoGroup
{
	MiBaoUint		unit[MIBAOUNIT_NUMBER];

	MiBaoGroup()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		for(INT i = 0; i < MIBAOUNIT_NUMBER; ++i)	unit[i].CleanUp();
	}

	BOOL	IsAlreadyHaveUnit(BYTE row,BYTE column)
	{
		if(/*row < 0 || */row >= MIBAO_TABLE_ROW_MAX || /*column < 0 || */column >= MIBAO_TABLE_COLUMN_MAX)	return TRUE;

		MiBaoUint	tu;
		tu.row = row;
		tu.column = column;

		for(INT i = 0; i < MIBAOUNIT_NUMBER; ++i)
		{
			if(TRUE == unit[i].IsSame(tu))	return TRUE;
		}

		return FALSE;
	}

	const CHAR*		GetMiBaoKey(INT idx)	const
	{
		if(idx < 0 || idx >= MIBAOUNIT_NUMBER)	return NULL;
		return unit[idx].key;
	}
};

//////////////////////////////////////////////////////
// 抽奖相关操作数据
//////////////////////////////////////////////////////
#define MAX_PRIZE_STRING	20		//参见《天龙八部推广员帐号领奖通信协议》中的奖品代码长度定义
#define MAX_PRIZE_NUMBER	30		//一次最多领30个不同种类的奖品
#define MAX_NEWUSER_CARD_SIZE	20	//新手卡长度

enum PRIZE_TYPE_ENUM
{
	PRIZE_TYPE_INVALID	= 0,
	PRIZE_TYPE_CDKEY	= 1,	//推广员
	PRIZE_TYPE_YUANBAO	= 2,	//元宝
	PRIZE_TYPE_NEWUSER	= 3,	//新手卡(财富卡)
	PRIZE_TYPE_ZENGDIAN	= 4,	//赠点
	PRIZE_TYPE_ITEM		= 5,	//物品
	PRIZE_TYPE_SPORTS	= 6,	//体育竞猜卡
	PRIZE_TYPE_JU		= 7,	//网聚活动卡
};

//Billing返回的奖品结构
struct _PRIZE_DATA
{
	CHAR	m_PrizeString[MAX_PRIZE_STRING];	//奖品代码
	BYTE	m_PrizeNum;							//奖品数量

	_PRIZE_DATA()
	{
		memset(m_PrizeString,0,MAX_PRIZE_STRING);
		m_PrizeNum = 0;
	}

	static UINT getSize()
	{
		return sizeof(CHAR)*MAX_PRIZE_STRING+sizeof(BYTE);
	}
};

//Billing返回的购买结构
struct _RETBUY_DATA
{
	UINT	m_BuyInt;						//商品代码(元宝)
	USHORT	m_BuyNumber;					//商品数量

	_RETBUY_DATA()
	{
		m_BuyInt	= 0;
		m_BuyNumber	= 0;
	}

	static UINT getSize()
	{
		return sizeof(UINT)+sizeof(USHORT);
	}
};

//商品结构
struct _BUY_DATA
{
	CHAR	m_BuyString[MAX_PRIZE_STRING];	//商品代码(CD-KEY)
	UINT	m_BuyPoint;						//商品消耗点数
	UINT	m_BuyInt;						//商品代码(元宝)
	UINT	m_BuyNumber;					//商品数量

	_BUY_DATA()
	{
		memset(m_BuyString,0,MAX_PRIZE_STRING);
		m_BuyPoint	= 0;
		m_BuyInt	= 0;
		m_BuyNumber	= 0;
	}

	BYTE	GetPrizeType();						//奖品类型
	UINT	GetPrizeSerial();					//奖品序列号
	BYTE	GetPrizeNum();						//奖品数量
	UINT	GetCostPoint();						//消耗点数

	VOID	GetSubString(INT nIdx,CHAR* pBuf,INT nBufLength);	//拆分奖品字串

	BYTE	GetGoodsType();						//商品类型
	USHORT	GetGoodsNum();						//商品数量
};


#define MAX_CHOOSE_SCENE_NUMBER 10

struct DB_CHOOSE_SCENE
{
	DB_CHOOSE_SCENE()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset(this,0,sizeof(*this));
	}

	CHAR		mSceneCount;
	SceneID_t	mSceneID[MAX_CHOOSE_SCENE_NUMBER];
};

struct CFG_CHOOSE_SCENE
{
	CFG_CHOOSE_SCENE()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset(this,0,sizeof(*this));
	}

	CHAR		mSceneCount;
	SceneID_t	mSceneID[MAX_CHOOSE_SCENE_NUMBER];
	WORLD_POS	mPos[MAX_CHOOSE_SCENE_NUMBER];
};

//帐号安全相关
enum _ACCOUNT_SAFE_FLAG
{
	ASF_BIND_NONE = 0,
	ASF_BIND_IP =2,						//IP绑定
	ASF_BIND_MIBAOKA = 4,				//密保卡绑定
	ASF_BIND_MOBILE_PHONE = 8,			//手机绑定
	ASF_BIND_MAC = 16,					//MAC绑定
	ASF_BIND_REALNAME = 32,				//实名验证过（公安局数据库验证过）
	ASF_BIND_INPUTNAME = 64,			//填写过实名（玩家在WEB上填写过实名的信息，但尚未通过公安局数据库验证）
};

typedef struct tag_AccountSafeSign
{
	INT				m_Sign;

	tag_AccountSafeSign()
	{
		CleanUp();
	}

	VOID			CleanUp()
	{
		m_Sign = ASF_BIND_NONE;
	}

	BOOL			IsBindSafeSign(INT nSign)
	{
		switch(nSign) 
		{
		case ASF_BIND_IP:
		case ASF_BIND_MIBAOKA:
		case ASF_BIND_MOBILE_PHONE:
		case ASF_BIND_MAC:
		case ASF_BIND_REALNAME:
		case ASF_BIND_INPUTNAME:
			return (m_Sign & nSign)?TRUE:FALSE;
		case ASF_BIND_NONE:
		default:
			return FALSE;
		}
		return FALSE;
	}

	VOID			SetBindSafeSign(INT nSign,BOOL bSet = TRUE)
	{
		switch(nSign) 
		{
		case ASF_BIND_IP:
		case ASF_BIND_MIBAOKA:
		case ASF_BIND_MOBILE_PHONE:
		case ASF_BIND_MAC:
		case ASF_BIND_REALNAME:
		case ASF_BIND_INPUTNAME:
			(bSet)?(m_Sign |= nSign):(m_Sign &= ~nSign);
			break;
		case ASF_BIND_NONE:
		default:
			break;
		}
	}
}AccountSafeSign;

//消费记录结构
struct _COST_LOG
{
	CHAR			m_SerialKey[MAX_PRIZE_SERIAL_LENGTH+1];		//消费序列号
	INT				m_WorldId;									//World号
	INT				m_ServerId;									//Server号
	INT				m_SceneId;									//Scene号
	GUID_t			m_UserGUID;									//用户GUID
	INT				m_UserLevel;								//用户等级
	LONG			m_CostTime;									//消费时间(自1970-01-01 的秒数)
	INT				m_YuanBao;									//消耗的元宝数

	CHAR			m_AccName[MAX_ACCOUNT+1];					//帐号
	CHAR			m_CharName[MAX_CHARACTER_NAME+1];			//角色
	CHAR			m_Host[IP_SIZE+1];							//IP
	CHAR			m_OtherInfo[MAX_COST_OTHER_SIZE+1];			//Log的备注信息

	_COST_LOG()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset( this, 0, sizeof(*this) ) ;
		m_UserGUID = INVALID_GUID;
	}

	BOOL	IsSame(const CHAR* pSerial)
	{
		return (0 == strcmp(pSerial,m_SerialKey))?TRUE:FALSE;
	}
};

//	add by gh for souxia 2010/05/10
//1 将 SOUXIA_DATA 中固定属性，退化为通过查表获取
struct  SouXia_Skill
{
	SkillID_t	StudySkillId[MAX_SKILL_COUNT]; // 学习过的Skill 索引
	BYTE		StudyCount;
	//加个赋值重载,用来做交换用
	SouXia_Skill&	operator =(const SouXia_Skill& other )
	{
		if(this == &other)
		{
			return *this;
		}
		for(int i=0; i<=other.StudyCount; ++i)
		{
			StudySkillId[i] = other.StudySkillId[i];
		}
		StudyCount = other.StudyCount;
		return *this;
	}
};

struct  SouXia_Product
{
	SkillID_t	StudyProductId[MAX_PRODUCT_COUNT]; // 学习过的神器配方 索引
	BYTE		StudyCount;

	//加个赋值重载,用来做交换用
	SouXia_Product&	operator =(const SouXia_Product& other )
	{
		if(this == &other)
		{
			return *this;
		}
		for(int i=0; i<=other.StudyCount; ++i)
		{
			StudyProductId[i] = other.StudyProductId[i];
		}
		StudyCount = other.StudyCount;
		return *this;
	}
};

struct ZhaoHuan
{
	SkillID_t	StudyZhaoHuan; // 学习过的神兽召唤技能索引
	SHORT		LeftUseTime;   // 还可以使用的次数

	//加个赋值重载,用来做交换用
	ZhaoHuan&	operator =(const ZhaoHuan& other )
	{
		if(this == &other)
		{
			return *this;
		}
		StudyZhaoHuan = other.StudyZhaoHuan;
		LeftUseTime = other.LeftUseTime;
		return *this;
	}
};

struct  SouXia_PetZhaoHuan
{
	ZhaoHuan	StudyPet[MAX_PET_ZHAOHUAN_COUNT]; // 学习过的神兽召唤技能索引
	BYTE		StudyCount;   // 当前学习的数量

	//加个赋值重载,用来做交换用
	SouXia_PetZhaoHuan&	operator =(const SouXia_PetZhaoHuan& other )
	{
		if(this == &other)
		{
			return *this;
		}
		for(int i=0; i<=other.StudyCount; ++i)
		{
			StudyPet[i] = other.StudyPet[i];
		}
		StudyCount = other.StudyCount;
		return *this;
	}
};

struct  SouXia_ZuojiZhaoHuan
{
	ZhaoHuan	StudyZuoji[MAX_ZUOJI_ZHAOHUAN_COUNT]; // 学习过的坐骑召唤技能索引
	BYTE		StudyCount;   // 当前学习的数量

	//加个赋值重载,用来做交换用
	SouXia_ZuojiZhaoHuan&	operator = (const SouXia_ZuojiZhaoHuan& other )
	{
		if(this == &other)
		{
			return *this;
		}
		for(int i=0; i<=other.StudyCount; ++i)
		{
			StudyZuoji[i] = other.StudyZuoji[i];
		}
		StudyCount = other.StudyCount;
		return *this;
	}
};
/*
注意:现在拷贝构造和赋值构造由系统默认提供位拷贝,已经实现了部分用到的暂时用不到的先没写
*/
enum 
{
	SKILL_PER_PAGE = 3,
	PRODUCT_PER_PAGE = 1,
	PET_ZHAOHUAN_PER_PAGE = 8,
	ZUOJI_ZHAOHUAN_PER_PAGE = PET_ZHAOHUAN_PER_PAGE,
};
struct  SOUXIA_DATA
{
	SHORT					m_CurPos;
	UINT					m_SouXiaID;	// 捜侠录索引
	SouXia_Skill			m_Skill;	// 包含的搜侠技能
	SouXia_Product			m_Product;	//  ...  神器配方
	SouXia_PetZhaoHuan		m_Pet;		//  ...  神兽召唤
	SouXia_ZuojiZhaoHuan	m_ZuoJi;	//  ...  坐骑召唤

	SOUXIA_DATA()
	{
		CleanUp();
	};

	void CleanUp()
	{
		memset(this, 0, sizeof(SOUXIA_DATA));
		m_CurPos = -1;
	}

	SOUXIA_DATA& operator = (const SOUXIA_DATA& other)
	{
		if(this == &other)
		{
			return *this;
		}
		m_CurPos	= other.m_CurPos;
		m_SouXiaID	= other.m_SouXiaID;
		m_Skill		= other.m_Skill;
		m_Product	= other.m_Product;
		m_Pet		= other.m_Pet;
		m_ZuoJi		= other.m_ZuoJi; 

		return *this;
	};

	SHORT	GetCurPos()			{ return m_CurPos; }
	VOID	SetCurPos(BYTE pos)	{ m_CurPos = pos; }

	BYTE	GetCurTypeCount()	{ return GetCurSkillCount()>0 ? 1:0 + 
									GetCurProductCount() >0?1:0 + 
									GetCurPetCount()>0?1:0 + 
									GetCurZuoJiCount()>0?1:0 ; }

	BOOL SkillIsFull()			
	{
		if(MAX_SKILL_COUNT/SKILL_PER_PAGE == GetCurSkillPage())
		{	
			return TRUE;
		}
		return FALSE;
	}

	BOOL ProductIsFull()			
	{
		if(MAX_PRODUCT_COUNT/PRODUCT_PER_PAGE == GetCurProductPage())
		{	
			return TRUE;
		}
		return FALSE;
	}

	BOOL PetIsFull()			
	{
		if(MAX_PET_ZHAOHUAN_COUNT/ZUOJI_ZHAOHUAN_PER_PAGE == GetCurPetZhaoHuanPage())
		{	
			return TRUE;
		}
		return FALSE;
	}


	BOOL ZuoJiIsFull()			
	{
		if(MAX_ZUOJI_ZHAOHUAN_COUNT/PET_ZHAOHUAN_PER_PAGE == GetCurZuoJiZhaoHuanPage() )
		{	
			return TRUE;
		}
		return FALSE;
	}

	BYTE	GetCurSkillCount()	{return m_Skill.StudyCount;}
	BYTE	GetCurProductCount(){return m_Product.StudyCount;}
	BYTE	GetCurPetCount()	{return m_Pet.StudyCount;}
	BYTE	GetCurZuoJiCount()	{return m_ZuoJi.StudyCount;}

	VOID	IncCurSkillCount()	{ m_Skill.StudyCount++; }
	VOID	IncCurProductCount(){ m_Product.StudyCount++; }
	VOID	IncCurPetCount()	{ m_Pet.StudyCount++; }
	VOID	IncCurZuoJiCount()	{ m_ZuoJi.StudyCount++; }

	//VOID	DecCurSkillCount()	{ m_Skill.StudyCount--; }
	//VOID	DecCurProductCount(){ m_Product.StudyCount--; }
	VOID	DecCurPetCount()	{ Assert(m_Pet.StudyCount>0); m_Pet.StudyCount--; }
	VOID	DecCurZuoJiCount()	{ Assert(m_ZuoJi.StudyCount>0); m_ZuoJi.StudyCount--; }

	// 取得当前各项总的页数
	BYTE	GetCurSkillPage()			{ return (BYTE)(((GetCurSkillCount()+SKILL_PER_PAGE)-1)/SKILL_PER_PAGE); }
	BYTE	GetCurProductPage()			{ return GetCurProductCount(); }
	BYTE	GetCurPetZhaoHuanPage()		{ return (BYTE)(((GetCurPetCount()+PET_ZHAOHUAN_PER_PAGE)-1)/PET_ZHAOHUAN_PER_PAGE); }
	BYTE	GetCurZuoJiZhaoHuanPage()	{ return (BYTE)(((GetCurZuoJiCount()+ZUOJI_ZHAOHUAN_PER_PAGE)-1)/ZUOJI_ZHAOHUAN_PER_PAGE); }

	VOID	ReadSouXiaVarAttr(SocketInputStream& iStream);
	VOID	WriteSouXiaVarAttr(SocketOutputStream& oStream) const;
};

//后面的文件会用到前面的定义
#include "GameStruct_Item.h"
#include "GameStruct_Skill.h"
#include "GameStruct_Scene.h"

#include "GameStruct_Relation.h"
#include "GameStruct_Guild.h"
#include "GameStruct_City.h"
#include "GameStruct_Script.h"

#include "GameStruct_MinorPasswd.h"
#include "GameStruct_Finger.h"
#include "GameStruct_Country.h"

#endif
