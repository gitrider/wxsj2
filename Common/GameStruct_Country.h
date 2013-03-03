/********************************************************************************
*	文件名：	GameStruct_Country.h
*	全路径：	d:\Prj\Common\GameStruct_Country.h
*	创建人：	李一立
*	创建时间：	2008 年 4 月 10 日	16:12
*
*	功能说明：	国家相关的数据结构、消息结构
*				
*				
*	修改记录：  2008-5-20  新增阵营PK相关定义
*********************************************************************************/


#ifndef __GAMESTRUCT_COUNTRY_H__
#define __GAMESTRUCT_COUNTRY_H__


#include "Type.h"

#define  MAX_WORLD_USER         3000
#define  DEFAULT_AP             1000
#define  MAX_VOTE_USER          1000
#define  MAX_GUILD_MASTER_VOTE  100

struct  PK_INFO
{
    INT         m_nID;          //ID
    INT         m_nMin;         //下限
    INT         m_nMax;         //上限
    INT         m_nValue;       //值

    PK_INFO()
    {
        m_nID       =   0;      
        m_nMin      =   0;     
        m_nMax      =   0;     
        m_nValue    =   0;   
    }
};

struct  VOTE_INFO
{
    BOOL        m_bResult;
    GUID_t      m_Guid;

    VOTE_INFO()
    {
        m_bResult   = FALSE;
        m_Guid      = INVALID_GUID;
    }
};


enum    CAMP_ENUM                       //      阵营分类
{
    CAMP_A          = 0,                //00	A国玩家阵营，可攻击战斗NPC，可攻击不同国家阵营玩家
    CAMP_B,                             //01	B国玩家阵营，可攻击战斗NPC，可攻击不同国家阵营玩家
    CAMP_C,                             //02	C国玩家阵营，可攻击战斗NPC，可攻击不同国家阵营玩家
    CAMP_D,                             //03	D国玩家阵营，可攻击战斗NPC，可攻击不同国家阵营玩家
    CAMP_FREE,                          //04	对立阵营，可攻击战斗NPC，可攻击同阵营与不同国家阵营玩家
    CAMP_5,                             //05	玩家扩展阵营（与6敌对）
    CAMP_6,                             //06	玩家扩展阵营（与5敌对）
    CAMP_HAOHUAI,                       //07	玩家扩展阵营
    CAMP_8,                             //08	玩家扩展阵营
    CAMP_9,                             //09	玩家扩展阵营
    CAMP_10,                            //10	玩家扩展阵营
    CAMP_11,                            //11	玩家扩展阵营
    CAMP_12,                            //12	玩家扩展阵营
    CAMP_13,                            //13	玩家扩展阵营
    CAMP_14,                            //14	玩家扩展阵营
    CAMP_15,                            //15	玩家扩展阵营

    CAMP_PLAYER_END,

    CAMP_17          = 17,              //17    A国NPC，会被其它国家玩家攻击
    CAMP_18,                            //18    B国NPC，会被其它国家玩家攻击
    CAMP_19,                            //19    C国NPC，会被其它国家玩家攻击
    CAMP_20,                            //20    D国NPC，会被其它国家玩家攻击
    CAMP_21,                            //21    普通NPC阵营，不会受到任何攻击
    CAMP_22,                            //22    正常战斗NPC阵营
    CAMP_23,                            //23    NPC扩展阵营（与21敌对）
    CAMP_24,                            //24    NPC扩展阵营
    CAMP_25,                            //25    NPC扩展阵营
    CAMP_26,                            //26    NPC扩展阵营
    CAMP_27,                            //27    NPC扩展阵营
    CAMP_28,                            //28    NPC扩展阵营
    CAMP_29,                            //29    NPC扩展阵营
    CAMP_30,                            //30    NPC扩展阵营
    CAMP_31,                            //31    NPC扩展阵营
    CAMP_32,                            //32    NPC扩展阵营

    CAMP_NPC_END,

	// by soongxl 
    CAMP_GUILD      = 40,               //40    帮会模式
    CAMP_TEAM,                          //41    队伍模式
    CAMP_FAMILY,                        //42    家族模式

	CAMP_UNKNOWN,
};


struct _COUNTRY
{
    _COUNTRY()
    {
        m_GuoWang   =   INVALID_GUID;
        m_ZaiXiang  =   INVALID_GUID;
        m_YuanShuai =   INVALID_GUID;
        m_YuShiDaFu =   INVALID_GUID;
        m_BuTou     =   INVALID_GUID;
        m_nCurUser  =   0;
        m_nGold     =   0;
        m_nAP       =   0;
        m_cCountryID=   0;

        for (INT i=0; i<MAX_WORLD_USER; ++i)
        {
            m_aUser[i] = INVALID_GUID;
        }
        
        memset(m_szGuoWang, 0, MAX_CHARACTER_NAME);
        memset(m_szZaiXiang, 0, MAX_CHARACTER_NAME);
        memset(m_szYuanShuai, 0, MAX_CHARACTER_NAME);
        memset(m_szYuShiDaFu, 0, MAX_CHARACTER_NAME);
        memset(m_szBuTou, 0, MAX_CHARACTER_NAME);
    }

    enum
    {
        COUNTRY_GUOWANG  = 0,       //国王
        COUNTRY_ZAIXIANG,           //宰相
        COUNTRY_YUANSHUAI,          //元帅
        COUNTRY_YUSHIDAFU,          //御史大夫
        COUNTRY_BUTOU,              //捕头
    };

public:

    BYTE                    m_cCountryID; 
    GUID_t                  m_aUser[MAX_WORLD_USER];            //在线国家成员
    INT                     m_nCurUser;                         //当前国家人数
    INT                     m_nGold;                            //募捐箱
    INT                     m_nAP;                              //Action Point

    GUID_t                  m_GuoWang;
    GUID_t                  m_ZaiXiang;
    GUID_t                  m_YuanShuai;
    GUID_t                  m_YuShiDaFu;    
    GUID_t                  m_BuTou;

    CHAR                    m_szGuoWang[MAX_CHARACTER_NAME];
    CHAR                    m_szZaiXiang[MAX_CHARACTER_NAME];
    CHAR                    m_szYuanShuai[MAX_CHARACTER_NAME];
    CHAR                    m_szYuShiDaFu[MAX_CHARACTER_NAME];
    CHAR                    m_szBuTou[MAX_CHARACTER_NAME];

    VOID                    AddGold(INT nGold)
    {
        m_nGold += nGold;
    }

    VOID                    SubGold(INT nGold)
    {
        if (m_nGold >=  nGold)
            m_nGold -= nGold;
        else
            m_nGold = 0;
    }

    INT                     GetGold()
    {
        return  m_nGold;
    }

    VOID                    AddUser(GUID_t guid) 
    {
        for (INT i=0; i<MAX_WORLD_USER; ++i)
        {
            if (m_aUser[i] == INVALID_GUID)
            {
                m_aUser[i] = guid;
                ++m_nCurUser;
                break;
            }
        }
    }

    VOID                    RemoveUser(GUID_t guid) 
    {
        for (INT i=0; i<MAX_WORLD_USER; ++i)
        {
            if (m_aUser[i] == guid)
            {
                m_aUser[i] = INVALID_GUID;
                --m_nCurUser;
                break;
            }
        }
    }

    INT                     GetCurUserNum () { return m_nCurUser; }

    VOID                    AppointPosition(INT nPosition, GUID_t guid, const CHAR *szName)
    {
        UCHAR cNameSize = (UCHAR)strlen(szName);
        switch (nPosition)
        {
        case COUNTRY_GUOWANG:
            {
                m_GuoWang       = guid;
                m_nAP           = DEFAULT_AP;

                if (cNameSize > 0)
                {
                    strncpy((CHAR*)m_szGuoWang,szName, cNameSize);
                }
                else
                {
                    memset(m_szGuoWang, 0, MAX_CHARACTER_NAME);
                }
            }
            break;
        case COUNTRY_ZAIXIANG:
            {
                m_ZaiXiang  = guid;

                if (cNameSize > 0)
                {
                    strncpy((CHAR*)m_szZaiXiang,szName, cNameSize);
                }
                else
                {
                    memset(m_szZaiXiang, 0, MAX_CHARACTER_NAME);
                }
            }
            break;
        case COUNTRY_YUANSHUAI:
            {
                m_YuanShuai = guid;

                if (cNameSize > 0)
                {
                    strncpy((CHAR*)m_szYuanShuai,szName, cNameSize);
                }
                else
                {
                    memset(m_szYuanShuai, 0, MAX_CHARACTER_NAME);
                }
             }
            break;
        case COUNTRY_YUSHIDAFU:
            {
                m_YuShiDaFu = guid;

                if (cNameSize > 0)
                {
                    strncpy((CHAR*)m_szYuShiDaFu,szName, cNameSize);
                }
                else
                {
                    memset(m_szYuShiDaFu, 0, MAX_CHARACTER_NAME);
                }
            }
            break;
        case COUNTRY_BUTOU:
            {
                m_BuTou     = guid;

                if (cNameSize > 0)
                {
                    strncpy((CHAR*)m_szBuTou,szName, cNameSize);
                }
                else
                {
                    memset(m_szBuTou, 0, MAX_CHARACTER_NAME);
                }
            }
            break;
        }
    }
};



//请求当国王
struct COUNTRY_CGW_KINDOM_REQUEST
{
    GUID_t                  m_Guid;

    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;

    UINT	    	    	GetPacketSize() const
    {
        return sizeof(GUID_t);
    }
};

//任命国家官职
struct COUNTRY_CGW_COUNTRY_APPOINT
{
    GUID_t                  m_KindomGuid;
    CHAR                    m_cNameSize;
    CHAR                    m_szTargetName[MAX_CHARACTER_NAME];
    INT                     m_nOfficialPosition;

    COUNTRY_CGW_COUNTRY_APPOINT()
    {
        memset(m_szTargetName, 0, MAX_CHARACTER_NAME);
    }

    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;

    UINT    			    GetPacketSize() const
    {
        return sizeof(GUID_t) + sizeof(INT) + sizeof(CHAR) + m_cNameSize;
    }
};

//罢免国家官职
struct COUNTRY_CGW_COUNTRY_REMOVE
{
    GUID_t                  m_KindomGuid;
    INT                     m_nOfficialPosition;
    CHAR                    m_cNameSize;
    CHAR                    m_szTargetName[MAX_CHARACTER_NAME];

    COUNTRY_CGW_COUNTRY_REMOVE()
    {
        memset(m_szTargetName, 0, MAX_CHARACTER_NAME);
    }

    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;

    UINT    		    	GetPacketSize() const
    {
        return sizeof(GUID_t) + sizeof(CHAR) + m_cNameSize + sizeof(INT);
    }
};

//国王募集
struct COUNTRY_CGW_COUNTRY_COLLECT
{
    GUID_t                  m_Guid;
    INT                     m_nGold;
    BYTE                    m_cKindomSize;
    BYTE                    m_cDescSize;
    CHAR                    m_szKindom[MAX_CHARACTER_NAME];
    CHAR                    m_szDesc[MAX_COUNTRY_COLLECT_DESC];

    COUNTRY_CGW_COUNTRY_COLLECT()
    {
        m_Guid          = INVALID_GUID;
        m_nGold         = 0;
        m_cKindomSize   = 0;
        m_cDescSize     = 0;

        memset(m_szKindom, 0, MAX_CHARACTER_NAME);
        memset(m_szDesc, 0, MAX_COUNTRY_COLLECT_DESC);
    }

    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;

    UINT    		    	GetPacketSize() const
    {
        return sizeof(m_nGold) + m_cKindomSize + m_cDescSize + 2 + sizeof(GUID_t);
    }
};


//国家官员列表
struct COUNTRY_CGW_COUNTRY_INFO
{
    INT                     m_CountryID;
    _COUNTRY                m_CountryInfo;

    COUNTRY_CGW_COUNTRY_INFO()
    {
        memset(&m_CountryInfo, 0, sizeof(_COUNTRY));
    }

    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;

    UINT    		    	GetPacketSize() const
    {
        return sizeof(INT) + sizeof(_COUNTRY);
    }
};

enum COUNTRY_RETURN_TYPE
{
    COUNTRY_RETURN_INVALID			= -1,

    COUNTRY_RETURN_REQUEST_KINDOM   = 0,        //请求当国王
    COUNTRY_RETURN_APPOINT,                     //任免官职
    COUNTRY_RETURN_REMOVE,                      //罢免官职
    COUNTRY_RETURN_DELATE,                      //弹劾投票
    COUNTRY_RETURN_DELATE_SCUESS,               //弹劾投票成功
    COUNTRY_RETURN_REQUEST_KINDOM_FAIL,         //请求当国王失败


    COUNTRY_ERROR_COUNTRY_ID        = 30,       //国家不对
    COUNTRY_ERROR_USER_NOT_FIND,                //玩家不存在

    COUNTRY_RETURN_SIZE,
};


struct _COUNTRY_RETURN
{
    CHAR					m_ReturnType;
    CHAR                    m_cPosition;                        //官职
    CHAR                    m_cCountry;                         //国家
    CHAR                    m_szName[MAX_CHARACTER_NAME];       //名字
	//UINT                    m_nAP;                            //国王行为力 [ ZG 添加 ]


    _COUNTRY_RETURN()
    {
        m_ReturnType	=	COUNTRY_RETURN_INVALID;
        memset(m_szName, 0, MAX_CHARACTER_NAME);
    }

    UINT					GetPacketSize() const
    {
        UINT uSize = sizeof(m_ReturnType);

        switch( m_ReturnType )
        {
        case COUNTRY_RETURN_REQUEST_KINDOM:
            {
            }
            break;
        case COUNTRY_RETURN_APPOINT:
            {
                uSize +=	sizeof(m_cPosition)
                            + sizeof(m_cCountry) + MAX_CHARACTER_NAME;                
            }
            break;
        case COUNTRY_RETURN_REMOVE:
            {
                uSize +=	sizeof(m_cPosition)
                            + sizeof(m_cCountry);      
            }
            break;
        case COUNTRY_ERROR_COUNTRY_ID:
            {
            }
            break;
        case COUNTRY_ERROR_USER_NOT_FIND:
            {
            }
            break;
        case COUNTRY_RETURN_DELATE_SCUESS:
            break;
        case COUNTRY_RETURN_DELATE:
            break;
        case COUNTRY_RETURN_REQUEST_KINDOM_FAIL:
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




#endif // __GAMESTRUCT_COUNTRY_H__
