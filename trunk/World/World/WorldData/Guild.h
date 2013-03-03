/////////////////////////////////////////////////////////////////////////////////
//文件名：Guild.h
//功能描述：帮会数据和逻辑功能
//修改记录：2005-11-28 创建 
//			2005-12-06 扩充帮会结构和功能，
//          2008-03-04 新增家族和修改帮会结构 modify by - amos
/////////////////////////////////////////////////////////////////////////////////


#ifndef __GUILD_H__
#define __GUILD_H__

#include "Type.h"
#include "GameDefine_Guild.h"
#include "DB_Struct.h"

#define MAX_USER_IN_GUILD			160
#define USER_ARRAY_SIZE				MAX_USER_IN_GUILD + MAX_PROPOSER_IN_GUILD

//帮会中成员的信息
struct GUILDUSER
{
	GUID_t				m_UserGUID;							//玩家GUID
	CHAR				m_szUserName[MAX_CHARACTER_NAME];	//玩家名字
	GUILD_POSITION		m_Position;							//职务
	UINT				m_uAccess;							//权限
	BOOL				m_bOnlineFlag;						//是否在线，此条不需要存储
	UINT				m_uLastLoginTime;					//最后上线时间
	UINT				m_uLevel;							//人物级别
	BYTE				m_bMenPaiID;						//门派
	INT					m_iCurContribute;					//贡献
	INT					m_iMaxContribute;					//最大贡献
	INT					m_iJoinTime;						//加入时间
	INT     			m_iFamilyID;						//所属家族编号
	BYTE     			m_cSex;     						//玩家性别
	CHAR				m_szTitleName[MAX_CHARACTER_TITLE];	//帮会称号名字
	INT 				m_nGP;	                            //帮会贡献度

	GUILDUSER( )
	{
		CleanUp( );
	};
	VOID CleanUp( )
	{	
		m_UserGUID		= INVALID_ID;
		memset( m_szUserName, 0, sizeof(CHAR)*MAX_CHARACTER_NAME );
        memset( m_szTitleName, 0, sizeof(CHAR)*MAX_CHARACTER_TITLE );
		m_Position		= GUILD_POSITION_TRAINEE;
		m_uAccess		= 0;
		m_bOnlineFlag	= FALSE;
		m_uLastLoginTime= 0;
		m_uLevel		=	0;
		m_bMenPaiID		=	0;
		m_iCurContribute=	0;
		m_iMaxContribute=	0;
		m_iJoinTime		=	0;
        m_nGP           =   0;
        m_iFamilyID     =   INVALID_ID;
	}
};

struct FAMILYUSER
{
    //家族成员 只记录m_aGuildUser下标
    INT                 m_aFamilyUser[GUILD_FAMILY_SIZE][USER_FAMILY_SIZE]; 
    //家族名字
    CHAR                m_aFamilyName[GUILD_FAMILY_SIZE][MAX_GUILD_FAMILY_NAME_SIZE];
    //家族宗旨
    CHAR                m_aFamilyDesc[GUILD_FAMILY_SIZE][MAX_GUILD_FAMILY_DESC_SIZE];
    //家族长的名字
    CHAR                m_szShaikhName[GUILD_FAMILY_SIZE][MAX_CHARACTER_NAME];

    enum ORESULT
    {
        RET_SUCCESS = 0,
        RET_POS_FULL,	//人都满了
        RET_NO_EXIST,	//不存在

        RET_UNKOWN,
    };

    FAMILYUSER &operator = ( const FAMILYUSER &in )
    {
        if ( &in != this )
        {
            for (INT i=0; i<GUILD_FAMILY_SIZE; ++i)
            {
                for (INT j=0; j<USER_FAMILY_SIZE ; ++j)
                {
                    m_aFamilyUser[i][j] = in.m_aFamilyUser[i][j];
                }
            }
        }
        return *this;
    }

    INT Add (INT iFamilyId, INT iIndex, INT nMax = USER_FAMILY_SIZE)   //家族ID，玩家在m_aGuildUser的下标
    {
        for (INT i=0; i<USER_FAMILY_SIZE ; ++i)
        {
            if (INVALID_ID == m_aFamilyUser[iFamilyId][i])
            {
                if (iFamilyId == 0)
                {
                    if (i+1 > nMax)
                        return RET_POS_FULL;
                }

                m_aFamilyUser[iFamilyId][i] = iIndex;
                return RET_SUCCESS;
            }
        }
        return RET_POS_FULL;
    }

    INT Remove (INT iFamilyId, INT iIndex)
    {
        for (INT i=0; i<USER_FAMILY_SIZE; ++i)
        {
            if (iIndex == m_aFamilyUser[iFamilyId][i])
            {
                m_aFamilyUser[iFamilyId][i] = INVALID_ID;
                return RET_SUCCESS;
            }
        }
        return RET_NO_EXIST;
    }

    VOID ModifyFamilyName(BYTE cFamilyIdex, const CHAR* szName, const INT size)
    {
        memset(&m_aFamilyName[cFamilyIdex][0], 0, MAX_GUILD_FAMILY_NAME_SIZE);
        strncpy((CHAR*)&m_aFamilyName[cFamilyIdex][0], szName, size);
    }

    VOID ModifyFamilyDesc(BYTE cFamilyIdex, const CHAR* szDesc, const INT size)
    {
        memset(&m_aFamilyDesc[cFamilyIdex][0], 0, MAX_GUILD_FAMILY_DESC_SIZE);
        strncpy((CHAR*)&m_aFamilyDesc[cFamilyIdex][0], szDesc, size);
    }

    VOID ModifyShaikhName(BYTE cFamilyIdex, const CHAR* szName, const INT size)
    {
        memset(&m_szShaikhName[cFamilyIdex][0], 0, MAX_CHARACTER_NAME);
        strncpy((CHAR*)&m_szShaikhName[cFamilyIdex][0], szName, size);
    }

    const CHAR * GetFamilyName(CHAR cFamilyIndex)
    {
        return &m_aFamilyName[cFamilyIndex][0];
    }

    const CHAR * GetFamilyDesc(CHAR cFamilyIndex)
    {
        return &m_aFamilyDesc[cFamilyIndex][0];
    }

    const CHAR * GetShaikhName(CHAR cFamilyIndex)
    {
        return &m_szShaikhName[cFamilyIndex][0];
    }

    FAMILYUSER( )
    {
        CleanUp( );
    };

    VOID CleanUp( )
    {
        for( INT i=0; i<GUILD_FAMILY_SIZE; ++i )
        {
            for( INT j=0; j<USER_FAMILY_SIZE; ++j )
            {
                m_aFamilyUser[i][j] = INVALID_ID;
            }
        }

        memset(m_aFamilyName, 0, GUILD_FAMILY_SIZE*MAX_GUILD_FAMILY_NAME_SIZE);
        memset(m_aFamilyDesc, 0 , GUILD_FAMILY_SIZE*MAX_GUILD_FAMILY_DESC_SIZE);
        memset(m_szShaikhName, 0, GUILD_FAMILY_SIZE*MAX_CHARACTER_NAME);
    }
};

struct _GUILD_INIT
{
	GuildID_t			m_GuildID;							                //帮会ID
	CHAR				m_GuildName[MAX_GUILD_NAME_SIZE];	                //帮会名称
	GUILD_STATUS		m_Status;							                //帮会的状态
	GUID_t				m_ChieftainGUID;					                //帮主 GUID
	INT					m_nProposerCount;					                //申请人数
	INT					m_UserCount;						                //帮会用户数量
	INT					m_MaxUserSize;						                //帮会人口上限
	INT					m_GuildPoint;						                //帮会贡献点
	INT					m_GuildMoney;						                //帮会资金
	INT					m_nTime;							                //成立时间或创建剩余时间
	GUILDUSER			m_aGuildUser[USER_ARRAY_SIZE];		                //帮会成员
    FAMILYUSER          m_FamilyUser;                                       //家族信息

	_GUILD_INIT( )
	{
		m_GuildID = INVALID_ID;
		memset( m_GuildName, 0, sizeof(CHAR)*MAX_GUILD_NAME_SIZE );
		m_Status = GUILD_STATUS_INVALID;
		m_nProposerCount = 0; // 申请人数
		m_UserCount = 0;
		m_MaxUserSize = 0;
		m_GuildPoint = 0;
		m_GuildMoney = 0;
        m_FamilyUser.CleanUp();

		for( INT i=0; i<USER_ARRAY_SIZE; i++ )
		{
			m_aGuildUser[i].CleanUp();
		}
	};
	~_GUILD_INIT( )
	{
	};
};

class Packet;
class City;
class USER;

class Guild
{
public :
	Guild( );
	~Guild( );
	//-------------------------------------------------------------------------------------------------
	//enum
	//-------------------------------------------------------------------------------------------------
	enum PacketType
	{
		PACKET_RETURN	= 1, // 返回操作
	};

	//操作类型
	enum OPT_TYPE
	{
		OPT_NONE	= 0,	            // 返回操作
		OPT_ASK_MEMBER_LIST,			//请求当前所有帮派成员列表
		OPT_ASK_DETAIL_INFO,			//请求帮派的详细信息
		OPT_ASK_APPOINT_POS_INFO,		//请求帮派的可任命信息
		OPT_ASK_SELF_INFO,				//请求本人帮派信息
	};

	//-------------------------------------------------------------------------------------------------
	//初始化
	//-------------------------------------------------------------------------------------------------
	BOOL				Init( _GUILD_INIT* pInit );
	BOOL				InitFromShareMem();
	VOID				ValidateShareMem();
	VOID				CleanUp( );

	//-------------------------------------------------------------------------------------------------
	//基本功能
	//-------------------------------------------------------------------------------------------------
	BOOL				HeartBeat( UINT uTime=0 );		//逻辑心跳接口
	BOOL				IsMemberFull( ) { return (m_UserCount >= m_MaxUserSize); }// 是否帮会已经满员
	BOOL				IsProposerFull( ) { return (m_nProposerCount >= MAX_PROPOSER_IN_GUILD); }// 是否已经有太多玩家申请
	BOOL				IsAuthorized( GUID_t userGUID, GUILD_AUTHORITY authority );	// 判断是否有某项权限

	//-------------------------------------------------------------------------------------------------
	// 帮会的事件接口
	//-------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------
	// 创建帮会，还不算正式帮会，FALSE 表示帮会名称与现有名称重名
	BOOL				OnGuildCreate( const USER* pCreater, const CHAR* szName );
	VOID				OnGuildCreateFailed();
	VOID				OnGuildFound();
	BOOL				OnGuildDestroy( GUID_t createrGUID );

	//-------------------------------------------------------------------------------------------------
	// 任免职务，权限变更，权限判定
	GUILD_ADMIN_t::ORESULT	OnUserPositionChange( GUID_t userGUID, GUILD_POSITION position );//任免职务的原则,一定是先免职为普通帮众,再升职为某个官员
	
	VOID				OnUserAuthorityChange( GUID_t userGUID, GUILD_AUTHORITY authority );
	VOID				OnPositionAuthorityChange( GUILD_POSITION position, GUILD_AUTHORITY authority );
	VOID				OnUserAccessChange( GUID_t userGUID, INT newAccess ); //玩家权限变化
	VOID				OnGuildPointChange( GUID_t userGUID, INT nMoney ); //帮会贡献点发生变化
	BOOL				CanDoIt(OPT_TYPE Opt, GUILD_POSITION position, GUILD_AUTHORITY authority);//根据职位，权限操作判定操作
	BOOL				CanAppointedBy(GUILD_POSITION position, const GUILDUSER* pUser);//是否能被此人任命
	CHAR*				GetPosName(INT iPosition);//获得此帮会中当前职位的名字

	//-------------------------------------------------------------------------------------------------
	// 申请入帮，或者响应一个帮会
	GUILD_ERROR_TYPE	OnUserEnter( const USER* pUser );
	GUILD_ERROR_TYPE	OnRecruitUser( GUID_t userGUID );// 招收帮众
	GUILD_ERROR_TYPE	OnUserLeave( GUID_t userGUID );// 开除帮众离开帮会
	GUILD_ERROR_TYPE	OnUserLeaveByIndex( INT iIndex );// 开除帮众离开帮会
	GUILD_ERROR_TYPE	OnDemise();// 禅让，和任免职务不同的是，禅让需要将现有职务的人挤下去
	VOID				OnGuildMoneyChange( GUID_t userGUID, INT nMoney ); //帮会资金发生变化// 支取金额存入金额

	//-------------------------------------------------------------------------------------------------
    // 邀请加入帮会
	GUILD_ERROR_TYPE	OnUserInvite( const USER* pUser );

    // 创建家族
    INT                 OnCreateFamily(const GUID_t CreatorGuid,const INT iShaikhIndex,
                            const GUID_t ShaikhGuid, const CHAR* szFamilyName, const INT iFamilyNameSize,
                            const CHAR* szFamilyDesc,  const INT iFamilyDescSize, CHAR &cFamilyID );

    // 添加家族成员
    INT                 OnAddFamilyUser(const GUID_t InvitorGuid, CHAR cFamilyIndex, INT iGuildIndex, GUID_t guid);

    // 删除家族成员
    INT                 OnRemoveFamilyUser(const GUID_t BossGuid, CHAR cFamilyIndex, INT iGuildIndex, GUID_t guid);

    // 解散家族
    INT                 OnFamilyDismiss(CHAR cFamilyIndex );

    // 修改家族名字

    // 修改家族宗旨

    // 获取家族成员信息列表,返回家族成员数量
    INT                 GetFamilyUser(CHAR cFamilyIndex, INT* pFamilyUser);

    // 获取家族名字
    const CHAR *        GetFamilyName(CHAR cFamilyIndex);

    // 获取家族宗旨
    const CHAR *        GetFamilyDesc(CHAR cFamilyIndex);

    // 获取家族长名字
    const CHAR *        GetShaikhName(CHAR cFamilyIndex);

    // 取得家族数量
    UINT                GetFamilyCount();

    // 设置家族编号
    VOID                SetFamilyID(INT iGuildIndex, INT iID);

    // 取得家族编号
    INT                 GetFamilyID(GUID_t userGUID);

    // 设置帮众贡献度   
    VOID                SetGuildPoint(GUID_t userGUID, INT nGP);

    // 取得帮众贡献度   
    INT                 GetGuildPoint(GUID_t userGUID);

    // 设置帮会总贡献度   
    VOID                SetTotalGuildPoint(INT nGP);

    // 取得帮会总贡献度   
    INT                 GetTotalGuildPoint();

	//-------------------------------------------------------------------------------------------------
	// 帮会玩家上下线
	VOID				OnUserLogin( GUID_t userGUID );
	VOID				OnUserLogoff( GUID_t userGUID );

	//-------------------------------------------------------------------------------------------------
	// 广播，对 position 以上职务的帮众广播操作返回消息
	VOID				BroadCast_Return( Packet* pPacket, GUILD_POSITION positon = GUILD_POSITION_MEMBER );

	//-------------------------------------------------------------------------------------------------
	//操作锁
	BOOL				IsLock(){return m_IsLock;}
	VOID				SetLock(BOOL bIsLock){m_IsLock	=	bIsLock;}

	//-------------------------------------------------------------------------------------------------
	//数据接口
	//-------------------------------------------------------------------------------------------------

	//此帮会的ID
	GuildID_t			GetGuildID( ){ return m_GuildID; }

	// 帮会名称
	const CHAR*			GetGuildName( ) { return m_GuildName; }

	// 帮会宗旨
	const CHAR*			GetGuildDesc( ) { return m_GuildDesc; }

	// 帮会宗旨
	VOID				SetGuildDesc(CHAR*	pszGuildDesc)
	{
		memset(m_GuildDesc, 0, MAX_GUILD_DESC_SIZE);
		memcpy(m_GuildDesc, pszGuildDesc, MAX_GUILD_DESC_SIZE); 
	}

	BYTE				GetGuildLevel(){ return m_GuildLevel;}
	VOID				SetGuildLevel(BYTE GuildLevel){m_GuildLevel = GuildLevel;}

	//帮主名
	CHAR*				GetGuildChiefName(){return m_GuildChiefName;}

	//创建者名
	CHAR*				GetCreatorName( ) { return m_GuildCreatorName; }

	//帮派的领地
	City*				GetCity(){ return m_pCity;}

	//成立时间
	INT					GetFoundedTime(){return m_nTime;}

	// 取得帮众
	const GUILDUSER*	GetGuildUser( GUID_t userGUID );

    const GUILDUSER*	GetGuildUser( GUID_t userGUID, INT &iIndex);

	// 通过索引获得帮众
	GUILDUSER*	GetGuildUserByIndex( INT nIndex);

	// 取得帮众数量
	INT					GetGuildUserCount( ) { return m_UserCount; }

	// 取得预备帮众数量
	INT					GetGuildProposerCount( ) { return m_nProposerCount; }

	// 取得帮众数量
	INT					GetGuildUserMax( ) { return m_MaxUserSize; }

	// 帮会状态
	GUILD_STATUS		GetGuildStatus( ) { return m_Status; }

	// 帮主 GUID
	GUID_t				GetChieftainGUID( ) { return m_ChieftainGUID; }

	//-------------------------------------------------------------------------------------------------
	//帮派属性接口
	
	//资历
	INT					GetLongevity(){ return m_Longevity;}
	VOID				SetLongevity(INT Longevity){ m_Longevity = Longevity;}

	//贡献度
	INT					GetContribute(){ return m_Contribute;}
	VOID				SetContribute(INT Contribute){ m_Contribute = Contribute;}

	//人气
	INT					GetHonor(){ return m_Honor;}
	VOID				SetHonor(INT Honor){ m_Honor = Honor;}

	//帮会资金
	INT					GetMoney(){ return m_GuildMoney;}
	VOID				SetMoney(INT GuildMoney){ m_GuildMoney = GuildMoney;}

    VOID            	SetGuildUserTitleName( GUID_t userGUID, const CHAR *pszTitleName);

    //设置和帮会等级挂钩的三个最大值
    VOID                SetGuildMaxByLevel(INT nMaxUser, INT nMaxFamily, INT nMaxGoldFamilyUser);
    INT                 GetMaxUser() { return m_MaxUser; }
    INT                 GetMaxFamily() { return m_MaxFamily; }
    INT                 GetMaxGoldFamilyUser() { return m_MaxGoldFamilyUser; }

protected :
	//-------------------------------------------------------------------------------------------------
	//帮会内部逻辑及功能接口
	VOID				AddGuildUser( GUILDUSER& GuildUser );

	
protected :
	inline GUILDUSER*	ILGetGuildUser( GUID_t userGUID );
	inline INT			ILGetGuildUserIndex( GUID_t userGUID );

	GuildID_t			m_GuildID; //帮会ID
	CHAR				m_GuildName[MAX_GUILD_NAME_SIZE];		    //帮会名称
	CHAR				m_GuildDesc[MAX_GUILD_DESC_SIZE];		    //帮会描述
    CHAR                m_FamilyDesc[MAX_GUILD_FAMILY_DESC_SIZE];   //家族宗旨
	CHAR				m_GuildChiefName[MAX_CHARACTER_NAME];	    //帮主名
	CHAR				m_GuildCreatorName[MAX_CHARACTER_NAME];	    //创建者名
	BYTE				m_GuildLevel;
	GUILD_STATUS		m_Status;								    //帮会的状态
	GUID_t				m_ChieftainGUID;						    //帮主的 GUID
	INT					m_nProposerCount;						    //申请人数
	INT					m_UserCount;							    //帮会用户数量
	INT					m_MaxUserSize;							    //帮会人口上限
	INT					m_GuildPoint;							    //帮会贡献点
    UINT    			m_FamilyCount;							    //家族数量
	GUILDUSER			m_aGuildUser[USER_ARRAY_SIZE];			    //帮会成员
    FAMILYUSER          m_FamilyUser;                               //家族信息
	BOOL				m_IsLock;								    //当前工会是否被锁住，某些操作，只能同时存在一个，如创建城市
	CityID_t			m_CityID;								    //绑定的城市ID
	City*				m_pCity;								    //绑定的城市指针
    INT                 m_TotalGuildPoint;                          //帮会总贡献度

    INT                 m_MaxUser;                                  //帮会人数上限          和级别挂钩
    INT                 m_MaxFamily;                                //家族上限              和级别挂钩
    INT                 m_MaxGoldFamilyUser;                        //黄金家族人数上限      和级别挂钩

	// GUILD_STATUS_UNCOMMITTED 时，表示剩余毫秒数
	// GUILD_STATUS_NORMAL 时，表示帮会成立时间
	INT					m_nTime;
	GuildSMU*			m_pGuildSmu;
protected:
	//-------------------------------------------------------------------------------------------------
	//帮派属性
	INT					m_Longevity;				//资历
	INT					m_Contribute;				//贡献度
	INT					m_Honor;					//人气
	INT					m_GuildMoney;				//帮会资金

	//-------------------------------------------------------------------------------------------------
	//管理结构
	GUILD_ADMIN_t		m_AdminLayOut;							


private :

	// 用于存储心跳传来的时间戳
	UINT				m_uTimeStamp;

	// 分配帮众
	INT					m_nUserMallocIndex;
};


#endif
