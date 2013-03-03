/********************************************************************************
 *	文件名：	GameStruct_TopList.h
 *	创建人：	王勇鸷
 *	创建时间：	2008 年 4 月 8 日	20:00
 *
 *	功能说明：	排行榜的一些数据结构、消息结构
 *	修改记录：
 *	修改人：	yyz
 *	修改时间：	2010 年	7 月 12日

*********************************************************************************/

#ifndef __GAMESTRUCT_TOPLIST_H__
#define __GAMESTRUCT_TOPLIST_H__

#include "Type.h"
#include "Packet.h"

#define LEVEL_TOP_MEMBER_NUM  5000

enum TOP_LIST_TYPE
{//这里的ID是和toplist表里的id向对应的，修改这里也得修改表中排行榜的id
	LEVEL_TOP_LIST = 0,			//等级排行榜
	MONEY_TOP_LIST ,			//游戏币排行榜
	REPUTE_TOP_LIST,			//声望排行榜
	PET_TOP_LIST,				//宠物排行榜
	ECTYPE_TOP_LIST,			//副本排行榜
	ERRANTRY_TOP_LIST,			//侠义排行榜
	CRIME_TOP_LIST,				//罪恶排行榜
	BINDMONEY_TOP_LIST,			//绑定游戏币排行榜
	RMB_TOP_LIST,				//人民币排行榜
	BINDRMB_TOP_LIST,			//绑定人民币排行榜
	FAVOR_TOP_LIST,				//人气排行榜
	GUILD_TOP_LIST,				//帮派排行榜
	
	TOP_LIST_NUM,				//排行榜总数

};

//排行榜基本信息
struct TOP_LIST_ATTR
{
	INT			m_TopListID;				//排行榜ID
	CHAR		m_Name[MAX_TOP_LIST_NAME];	
	UINT		m_UpdateInterval;			//刷新的时间间隔
	INT			m_MemberCount ;				//排行榜人数上限
	INT			m_RecordCount;				//当前记录数
	INT			m_EnterMinKey;				//允许进入排行榜的最小等级
};


//榜单上要显示的玩家基本数据
struct _TOP_DATA
{			
	GUID_t	m_Guid;							//玩家GUID
	INT		m_Level;						//等级
	INT		m_Country;						//国家
	CHAR	m_Name[MAX_CHARACTER_NAME];		//名称
	INT		m_NameSize;
	MenPaiID_t	 m_MenPai;					//职业

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(m_Guid) + sizeof(m_Level) + sizeof(m_Country)
					 + m_NameSize + sizeof(m_NameSize) + sizeof(m_MenPai);
		return uSize;
	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_Guid,sizeof(m_Guid) );
		iStream.Read( (CHAR*)&m_Level,sizeof(m_Level) );
		iStream.Read( (CHAR*)&m_Country,sizeof(m_Country) );
		iStream.Read( (CHAR*)&m_NameSize,sizeof(m_NameSize) );

		
		if( m_NameSize > 0 && m_NameSize < MAX_CHARACTER_NAME )
		{
			iStream.Read(m_Name,m_NameSize );
		}	
		iStream.Read( (CHAR*)&m_MenPai,sizeof(m_MenPai) );
	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_Guid,sizeof(m_Guid) );
		oStream.Write( (CHAR*)&m_Level, sizeof(m_Level) );
		oStream.Write( (CHAR*)&m_Country,sizeof(m_Country) );
		oStream.Write( (CHAR*)&m_NameSize,sizeof(m_NameSize) );
		
		if( m_NameSize > 0 && m_NameSize < MAX_CHARACTER_NAME )
		{
			oStream.Write( m_Name, m_NameSize );
		}

		oStream.Write( (CHAR*)&m_MenPai,sizeof(m_MenPai) );
	}
};


//等级排行榜上要显示的玩家属性: 基本属性 + 扩展属性
struct _LEVEL_TOP_DATA
{
	_LEVEL_TOP_DATA()
	{
		memset(&m_UserData,0,sizeof(_TOP_DATA));
		m_Exp	 = INVALID_ID;
		m_OnlineTime = 0;
		m_GuildNameSize  = 0;
		memset(m_GuildName,0,MAX_GUILD_NAME_SIZE);
		m_OldRank = 0;
	}

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(m_Exp) + 
					 sizeof(m_OldRank)+
					 sizeof(m_OnlineTime)+
					 sizeof(BYTE) + 
					sizeof(CHAR) * m_GuildNameSize+ 
					 m_UserData.GetSize();
		return uSize;
	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_Exp, sizeof(m_Exp) );
		iStream.Read( (CHAR*)&m_OnlineTime, sizeof(m_OnlineTime) );
		iStream.Read( (CHAR*)&m_OldRank, sizeof(m_OldRank) );
		iStream.Read( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			iStream.Read(m_GuildName,m_GuildNameSize );
		}	
		m_UserData.Read(iStream);
	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_Exp, sizeof(m_Exp) );
		oStream.Write( (CHAR*)&m_OnlineTime, sizeof(m_OnlineTime) );
		oStream.Write( (CHAR*)&m_OldRank, sizeof(m_OldRank) );
		oStream.Write( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			oStream.Write(m_GuildName,m_GuildNameSize );
		}	
		m_UserData.Write(oStream);
	}

	_TOP_DATA	m_UserData;				//玩家基本属性

	//下面是扩展属性
	INT			m_Exp;					//经验值，在此做为副键
	UINT		m_OnlineTime;			//在线时间，在此做次副键
	CHAR		m_GuildName[MAX_GUILD_NAME_SIZE];		//帮会名称
	BYTE		m_GuildNameSize;
	INT			m_OldRank;			//之前的排名
};

//游戏币排行榜上要显示的玩家属性: 基本属性 + 扩展属性
struct _MONEY_TOP_DATA
{
	_MONEY_TOP_DATA()
	{
		memset(&m_UserData,0,sizeof(_TOP_DATA));
		m_Money  = 0;
		m_OnLineTime = 0;
		m_OldRank = 0;
		m_GuildNameSize  = 0;
		memset(m_GuildName,0,MAX_GUILD_NAME_SIZE);
	
	}

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(m_Money) +
			sizeof(m_OldRank)+
			sizeof(m_OnLineTime)+
			sizeof(BYTE) + 
			sizeof(CHAR) * m_GuildNameSize+ 
			m_UserData.GetSize();
		return uSize;

	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_Money, sizeof(m_Money) );
		iStream.Read( (CHAR*)&m_OnLineTime, sizeof(m_OnLineTime) );
		iStream.Read( (CHAR*)&m_OldRank, sizeof(m_OldRank) );
		iStream.Read( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			iStream.Read(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Read(iStream);
	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_Money, sizeof(m_Money) );
		oStream.Write( (CHAR*)&m_OnLineTime, sizeof(m_OnLineTime) );
		oStream.Write( (CHAR*)&m_OldRank, sizeof(m_OldRank) );
		oStream.Write( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			oStream.Write(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Write(oStream);
	}

	_TOP_DATA	m_UserData;				//玩家基本属性

	//下面是扩展属性
	UINT	m_Money;	
	UINT	m_OnLineTime;				//玩家在线总时间,作为副键
	INT		m_OldRank;					//玩家之前的排名
	CHAR	m_GuildName[MAX_GUILD_NAME_SIZE];		//帮会名称
	BYTE	m_GuildNameSize;

};

//声望排行榜上要显示的玩家的属性：基本属性 + 扩展属性
struct _REPUTE_TOP_DATA
{
	_REPUTE_TOP_DATA()
	{
		memset(&m_UserData,0,sizeof(_TOP_DATA));
		m_Repute = 0;
		m_GuildNameSize  = 0;
		memset(m_GuildName,0,MAX_GUILD_NAME_SIZE);
	}

	UINT	GetSize() const
	{
		UINT uSize = sizeof(m_Repute) +
					 sizeof(BYTE) + 
					 sizeof(CHAR) * m_GuildNameSize+ 
					 m_UserData.GetSize();
		return uSize;
	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_Repute, sizeof(m_Repute) );
		iStream.Read( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			iStream.Read(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Read(iStream);
	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_Repute, sizeof(m_Repute) );
		oStream.Write( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			oStream.Write(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Write(oStream);
	}

	_TOP_DATA	m_UserData;				//玩家基本属性

	//下面是扩展属性
	UINT	m_Repute;					//玩家的声望

	CHAR	m_GuildName[MAX_GUILD_NAME_SIZE];		//帮会名称
	BYTE	m_GuildNameSize;

};



//宠物排行榜上要显示的玩家的属性: 基本属性 + 扩展属性
struct	_PET_TOP_DATA
{
	_PET_TOP_DATA()
	{
		memset(&m_UserData,0,sizeof(_TOP_DATA));
		m_PetLevel  = 0;
		m_GuildNameSize  = 0;
		memset(m_GuildName,0,MAX_GUILD_NAME_SIZE);
	}

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(m_PetLevel) +
			sizeof(BYTE) + 
			sizeof(CHAR) * m_GuildNameSize+ 
			m_UserData.GetSize();
		return uSize;

	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_PetLevel, sizeof(m_PetLevel) );
		iStream.Read( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			iStream.Read(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Read(iStream);
	}


	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_PetLevel, sizeof(m_PetLevel) );
		oStream.Write( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			oStream.Write(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Write(oStream);
	}

	_TOP_DATA	m_UserData;			//玩家基本属性

	//下面是扩展属性
	UINT	m_PetLevel;				//宠物等级

	CHAR	m_GuildName[MAX_GUILD_NAME_SIZE];		//帮会名称
	BYTE	m_GuildNameSize;

};

//副本排行榜上要显示的玩家的属性: 基本属性 + 扩展属性
struct  _ECTYPE_TOP_DATA
{
	_ECTYPE_TOP_DATA()
	{
		memset(&m_UserData,0,sizeof(_TOP_DATA));
		EctypeLevel  = 0;
		EctypeNum = 0;
		m_GuildNameSize  = 0;
		memset(m_GuildName,0,MAX_GUILD_NAME_SIZE);

	}

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(EctypeLevel) +
			sizeof(EctypeNum)+
			sizeof(BYTE) + 
			sizeof(CHAR) * m_GuildNameSize+ 
			m_UserData.GetSize();
		return uSize;

	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&EctypeLevel, sizeof(EctypeLevel) );
		iStream.Read( (CHAR*)&EctypeNum, sizeof(EctypeNum) );
		iStream.Read( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			iStream.Read(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Read(iStream);
	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&EctypeLevel, sizeof(EctypeLevel) );
		oStream.Write( (CHAR*)&EctypeNum, sizeof(EctypeNum) );
		oStream.Write( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			oStream.Write(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Write(oStream);
	}

	_TOP_DATA	m_UserData;				//玩家基本属性

	//下面是扩展属性
	UINT	EctypeNum;					//副本数量主键	
	UINT	EctypeLevel;				//副本评级副键
	
	CHAR	m_GuildName[MAX_GUILD_NAME_SIZE];		//帮会名称
	BYTE	m_GuildNameSize;
};

//侠义排行榜上要显示的玩家属性: 基本属性 + 扩展属性
struct _ERRANTRY_TOP_DATA 
{
	_ERRANTRY_TOP_DATA()
	{
		memset(&m_UserData,0,sizeof(_TOP_DATA));
		m_Errantry  = 0;
		m_GuildNameSize  = 0;
		memset(m_GuildName,0,MAX_GUILD_NAME_SIZE);

	}

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(m_Errantry) +
			sizeof(BYTE) + 
			sizeof(CHAR) * m_GuildNameSize+ 
			m_UserData.GetSize();
		return uSize;

	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_Errantry, sizeof(m_Errantry) );
		iStream.Read( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			iStream.Read(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Read(iStream);
	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_Errantry, sizeof(m_Errantry) );
		oStream.Write( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			oStream.Write(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Write(oStream);
	}

	_TOP_DATA	m_UserData;				//玩家基本属性

	//下面是扩展属性
	UINT	 m_Errantry;				//玩家的侠义值

	CHAR	m_GuildName[MAX_GUILD_NAME_SIZE];		//帮会名称
	BYTE	m_GuildNameSize;

};

//罪恶排行榜上要显示的玩家属性: 基本属性 + 扩展属性
struct _CRIME_TOP_DATA 
{
	_CRIME_TOP_DATA()
	{
		memset(&m_UserData,0,sizeof(_TOP_DATA));
		m_Crime  = 0;
		m_GuildNameSize  = 0;
		memset(m_GuildName,0,MAX_GUILD_NAME_SIZE);

	}

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(m_Crime) +
			sizeof(BYTE) + 
			sizeof(CHAR) * m_GuildNameSize+ 
			m_UserData.GetSize();
		return uSize;

	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_Crime, sizeof(m_Crime) );
		iStream.Read( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			iStream.Read(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Read(iStream);
	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_Crime, sizeof(m_Crime) );
		oStream.Write( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			oStream.Write(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Write(oStream);
	}

	_TOP_DATA	m_UserData;				//玩家基本属性

	//下面是扩展属性
	UINT	 m_Crime;					//玩家的罪恶值

	CHAR	m_GuildName[MAX_GUILD_NAME_SIZE];		//帮会名称
	BYTE	m_GuildNameSize;
};


//人气排行榜要显示的属性FAVOR_TOP_LIST
struct _FAVOR_TOP_DATA
{
	_FAVOR_TOP_DATA()
	{
		memset(&m_UserData,0,sizeof(_TOP_DATA));
		m_Favor  = 0;
		m_OnLineTime = 0;
		m_GuildNameSize  = 0;
		memset(m_GuildName,0,MAX_GUILD_NAME_SIZE);

	}

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(m_Favor) +
			sizeof(m_OnLineTime)+
			sizeof(BYTE) + 
			sizeof(CHAR) * m_GuildNameSize+ 
			m_UserData.GetSize();
		return uSize;

	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_Favor, sizeof(m_Favor) );
		iStream.Read( (CHAR*)&m_OnLineTime, sizeof(m_OnLineTime) );
		iStream.Read( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			iStream.Read(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Read(iStream);
	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_Favor, sizeof(m_Favor) );
		oStream.Write( (CHAR*)&m_OnLineTime, sizeof(m_OnLineTime) );
		oStream.Write( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			oStream.Write(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Write(oStream);
	}

	_TOP_DATA	m_UserData;				//玩家基本属性

	//下面是扩展属性
	UINT	m_Favor;					//玩家的人气值	
	UINT	m_OnLineTime;				//玩家在线总时间,作为副键

	CHAR	m_GuildName[MAX_GUILD_NAME_SIZE];		//帮会名称
	BYTE	m_GuildNameSize;

};

//帮派排行榜要显示的属性GUILD_TOP_LIST
struct _GUILD_TOP_DATA
{
	_GUILD_TOP_DATA()
	{
		m_GuildID = 0;
		m_GuildLevel = 0;
		m_GuildNum = 0;
		m_GuildResource = 0;
		m_GuildMoney = 0;

		m_CDRNameSize = 0;
		memset(m_CDRName,0,MAX_CHARACTER_NAME);
		m_GuildNameSize  = 0;
		memset(m_GuildName,0,MAX_GUILD_NAME_SIZE);
	}

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(m_GuildID)+
			sizeof(m_GuildLevel) +
			sizeof(m_GuildNum)+
			sizeof(m_GuildResource)+
			sizeof(m_GuildMoney)+
			sizeof(BYTE) *2 + 
			sizeof(CHAR) * m_GuildNameSize+ 
			sizeof(CHAR) * m_CDRNameSize;
		return uSize;
	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_GuildID, sizeof(m_GuildID) );
		iStream.Read( (CHAR*)&m_GuildLevel, sizeof(m_GuildLevel) );
		iStream.Read( (CHAR*)&m_GuildNum, sizeof(m_GuildNum) );
		iStream.Read( (CHAR*)&m_GuildResource, sizeof(m_GuildResource) );
		iStream.Read( (CHAR*)&m_GuildMoney, sizeof(m_GuildMoney) );

		iStream.Read( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			iStream.Read(m_GuildName,m_GuildNameSize );
		}
		iStream.Read( (CHAR*)&m_CDRNameSize, sizeof(m_CDRNameSize) );
		if( m_CDRNameSize > 0 && m_CDRNameSize < MAX_CHARACTER_NAME )
		{
			iStream.Read(m_CDRName,m_CDRNameSize );
		}
	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_GuildID, sizeof(m_GuildID) );
		oStream.Write( (CHAR*)&m_GuildLevel, sizeof(m_GuildLevel) );
		oStream.Write( (CHAR*)&m_GuildNum, sizeof(m_GuildNum) );
		oStream.Write( (CHAR*)&m_GuildResource, sizeof(m_GuildResource) );
		oStream.Write( (CHAR*)&m_GuildMoney, sizeof(m_GuildMoney) );

		oStream.Write( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			oStream.Write(m_GuildName,m_GuildNameSize );
		}
		oStream.Write( (CHAR*)&m_CDRNameSize, sizeof(m_CDRNameSize) );
		if( m_CDRNameSize > 0 && m_CDRNameSize < MAX_CHARACTER_NAME )
		{
			oStream.Write(m_CDRName,m_CDRNameSize );
		}
	}

	//下面是扩展属性
	UINT	m_GuildID;					//帮派ID
	UINT	m_GuildLevel;				//帮派等级
	UINT	m_GuildNum;					//帮派人数
	UINT	m_GuildResource;			//帮派资源数量
	UINT	m_GuildMoney;				//帮派金钱

	CHAR	m_CDRName[MAX_CHARACTER_NAME];		//会长名称
	BYTE	m_CDRNameSize;

	CHAR	m_GuildName[MAX_GUILD_NAME_SIZE];	//帮会名称
	BYTE	m_GuildNameSize;
};


//发送给游戏服务器的等级排行榜
struct WORLD_TOP_LIST
{
	UCHAR				m_Type;
	INT					m_DataCount;	

	_LEVEL_TOP_DATA		m_LevelDataEx[TOP_USDR_NUM];	//等级排行榜
	_MONEY_TOP_DATA		m_MoneyDataEx[TOP_USDR_NUM];	//财富排行榜
	_REPUTE_TOP_DATA	m_ReputeDataEx[TOP_USDR_NUM];	//声望排行榜
	_PET_TOP_DATA		m_PetDataEx[TOP_USDR_NUM];		//宠物排行榜
	_ECTYPE_TOP_DATA	m_EctypeDataEx[TOP_USDR_NUM];	//副本排行榜
	_ERRANTRY_TOP_DATA	m_ErrantryDataEx[TOP_USDR_NUM];	//侠义排行榜
	_CRIME_TOP_DATA		m_CrimeDataEx[TOP_USDR_NUM];	//罪恶排行榜
	_MONEY_TOP_DATA		m_BindMoneyDataEx[TOP_USDR_NUM];//绑定游戏币排行榜
	_MONEY_TOP_DATA		m_RMBDataEx[TOP_USDR_NUM];		//人民币排行榜
	_MONEY_TOP_DATA		m_BindRMBDataEx[TOP_USDR_NUM];	//绑定人民币排行榜
	_FAVOR_TOP_DATA		m_FavorDataEx[TOP_USDR_NUM];	//人气排行榜
	_GUILD_TOP_DATA		m_GuildDataEx[TOP_USDR_NUM];	//帮派排行榜


	VOID	CleanUp( )
	{
		m_Type = TOP_LIST_NUM;
		m_DataCount = 0;
		memset(m_LevelDataEx,0,sizeof(_LEVEL_TOP_DATA)*TOP_USDR_NUM);
		memset(m_MoneyDataEx,0,sizeof(_MONEY_TOP_DATA)*TOP_USDR_NUM);
		memset(m_ReputeDataEx,0,sizeof(_REPUTE_TOP_DATA)*TOP_USDR_NUM);
		memset(m_PetDataEx,0,sizeof(_PET_TOP_DATA)*TOP_USDR_NUM);
		memset(m_EctypeDataEx,0,sizeof(_ECTYPE_TOP_DATA)*TOP_USDR_NUM);
		memset(m_ErrantryDataEx,0,sizeof(_ERRANTRY_TOP_DATA)*TOP_USDR_NUM);
		memset(m_CrimeDataEx,0,sizeof(_CRIME_TOP_DATA)*TOP_USDR_NUM);

		memset(m_BindMoneyDataEx,0,sizeof(_MONEY_TOP_DATA)*TOP_USDR_NUM);
		memset(m_RMBDataEx,0,sizeof(_MONEY_TOP_DATA)*TOP_USDR_NUM);
		memset(m_BindRMBDataEx,0,sizeof(_MONEY_TOP_DATA)*TOP_USDR_NUM);
		memset(m_FavorDataEx,0,sizeof(_FAVOR_TOP_DATA)*TOP_USDR_NUM);
		memset(m_GuildDataEx,0,sizeof(_GUILD_TOP_DATA)*TOP_USDR_NUM);

	}

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(m_Type);
		uSize += sizeof(m_DataCount);

		switch(m_Type)
		{
		case LEVEL_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				uSize += m_LevelDataEx[i].GetSize();
			}
			break;

		case MONEY_TOP_LIST:
		case BINDMONEY_TOP_LIST:
		case RMB_TOP_LIST:
		case BINDRMB_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				uSize += m_MoneyDataEx[i].GetSize();
			}
			break;
		case REPUTE_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				uSize += m_ReputeDataEx[i].GetSize();
			}
			break;
		case PET_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				uSize += m_PetDataEx[i].GetSize();
			}
			break;
		case ECTYPE_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				uSize += m_EctypeDataEx[i].GetSize();
			}
			break;
		case ERRANTRY_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				uSize += m_ErrantryDataEx[i].GetSize();
			}
			break;
		case CRIME_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				uSize += m_CrimeDataEx[i].GetSize();
			}
			break;
		case FAVOR_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				uSize += m_FavorDataEx[i].GetSize();
			}
			break;
		case GUILD_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				uSize += m_ErrantryDataEx[i].GetSize();
			}
			break;

		default:
			Assert(FALSE);
			break;
		}

		return uSize;
	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_Type, sizeof(m_Type) );
		iStream.Read( (CHAR*)&m_DataCount, sizeof(m_DataCount) );

		switch(m_Type)
		{
		case LEVEL_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_LevelDataEx[i].Read(iStream);
			}
			break;

		case MONEY_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_MoneyDataEx[i].Read(iStream);
			}
			break;

		case REPUTE_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_ReputeDataEx[i].Read(iStream);
			}
			break;
		case PET_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_PetDataEx[i].Read(iStream);
			}
			break;
		case ECTYPE_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_EctypeDataEx[i].Read(iStream);
			}
			break;
		case ERRANTRY_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_ErrantryDataEx[i].Read(iStream);
			}
			break;
		case CRIME_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_CrimeDataEx[i].Read(iStream);
			}
			break;
		case BINDMONEY_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_BindMoneyDataEx[i].Read(iStream);
			}
			break;
		case RMB_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_RMBDataEx[i].Read(iStream);
			}
			break;
		case BINDRMB_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_BindRMBDataEx[i].Read(iStream);
			}
			break;
		case FAVOR_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_FavorDataEx[i].Read(iStream);
			}
			break;
		case GUILD_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_GuildDataEx[i].Read(iStream);
			}
			break;

		default:
			Assert(FALSE);
			break;
		}

	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_Type, sizeof(m_Type) );
		oStream.Write( (CHAR*)&m_DataCount, sizeof(m_DataCount) );

		switch(m_Type)
		{
		case LEVEL_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_LevelDataEx[i].Write(oStream);
			}
			break;

		case MONEY_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_MoneyDataEx[i].Write(oStream);
			}
			break;
		case REPUTE_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_ReputeDataEx[i].Write(oStream);
			}
			break;
		case PET_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_PetDataEx[i].Write(oStream);
			}
			break;
		case ECTYPE_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_EctypeDataEx[i].Write(oStream);
			}
			break;
		case ERRANTRY_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_ErrantryDataEx[i].Write(oStream);
			}
			break;
		case CRIME_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_CrimeDataEx[i].Write(oStream);
			}
			break;
		case BINDMONEY_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_BindMoneyDataEx[i].Write(oStream);
			}
			break;
		case RMB_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_RMBDataEx[i].Write(oStream);
			}
			break;
		case BINDRMB_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_BindRMBDataEx[i].Write(oStream);
			}
			break;
		case FAVOR_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_FavorDataEx[i].Write(oStream);
			}
			break;
		case GUILD_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_GuildDataEx[i].Write(oStream);
			}
			break;

		default:
			Assert(FALSE);
			break;
		}
	}

};

#endif 
