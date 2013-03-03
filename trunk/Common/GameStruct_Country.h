/********************************************************************************
*	�ļ�����	GameStruct_Country.h
*	ȫ·����	d:\Prj\Common\GameStruct_Country.h
*	�����ˣ�	��һ��
*	����ʱ�䣺	2008 �� 4 �� 10 ��	16:12
*
*	����˵����	������ص����ݽṹ����Ϣ�ṹ
*				
*				
*	�޸ļ�¼��  2008-5-20  ������ӪPK��ض���
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
    INT         m_nMin;         //����
    INT         m_nMax;         //����
    INT         m_nValue;       //ֵ

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


enum    CAMP_ENUM                       //      ��Ӫ����
{
    CAMP_A          = 0,                //00	A�������Ӫ���ɹ���ս��NPC���ɹ�����ͬ������Ӫ���
    CAMP_B,                             //01	B�������Ӫ���ɹ���ս��NPC���ɹ�����ͬ������Ӫ���
    CAMP_C,                             //02	C�������Ӫ���ɹ���ս��NPC���ɹ�����ͬ������Ӫ���
    CAMP_D,                             //03	D�������Ӫ���ɹ���ս��NPC���ɹ�����ͬ������Ӫ���
    CAMP_FREE,                          //04	������Ӫ���ɹ���ս��NPC���ɹ���ͬ��Ӫ�벻ͬ������Ӫ���
    CAMP_5,                             //05	�����չ��Ӫ����6�жԣ�
    CAMP_6,                             //06	�����չ��Ӫ����5�жԣ�
    CAMP_HAOHUAI,                       //07	�����չ��Ӫ
    CAMP_8,                             //08	�����չ��Ӫ
    CAMP_9,                             //09	�����չ��Ӫ
    CAMP_10,                            //10	�����չ��Ӫ
    CAMP_11,                            //11	�����չ��Ӫ
    CAMP_12,                            //12	�����չ��Ӫ
    CAMP_13,                            //13	�����չ��Ӫ
    CAMP_14,                            //14	�����չ��Ӫ
    CAMP_15,                            //15	�����չ��Ӫ

    CAMP_PLAYER_END,

    CAMP_17          = 17,              //17    A��NPC���ᱻ����������ҹ���
    CAMP_18,                            //18    B��NPC���ᱻ����������ҹ���
    CAMP_19,                            //19    C��NPC���ᱻ����������ҹ���
    CAMP_20,                            //20    D��NPC���ᱻ����������ҹ���
    CAMP_21,                            //21    ��ͨNPC��Ӫ�������ܵ��κι���
    CAMP_22,                            //22    ����ս��NPC��Ӫ
    CAMP_23,                            //23    NPC��չ��Ӫ����21�жԣ�
    CAMP_24,                            //24    NPC��չ��Ӫ
    CAMP_25,                            //25    NPC��չ��Ӫ
    CAMP_26,                            //26    NPC��չ��Ӫ
    CAMP_27,                            //27    NPC��չ��Ӫ
    CAMP_28,                            //28    NPC��չ��Ӫ
    CAMP_29,                            //29    NPC��չ��Ӫ
    CAMP_30,                            //30    NPC��չ��Ӫ
    CAMP_31,                            //31    NPC��չ��Ӫ
    CAMP_32,                            //32    NPC��չ��Ӫ

    CAMP_NPC_END,

	// by soongxl 
    CAMP_GUILD      = 40,               //40    ���ģʽ
    CAMP_TEAM,                          //41    ����ģʽ
    CAMP_FAMILY,                        //42    ����ģʽ

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
        COUNTRY_GUOWANG  = 0,       //����
        COUNTRY_ZAIXIANG,           //����
        COUNTRY_YUANSHUAI,          //Ԫ˧
        COUNTRY_YUSHIDAFU,          //��ʷ���
        COUNTRY_BUTOU,              //��ͷ
    };

public:

    BYTE                    m_cCountryID; 
    GUID_t                  m_aUser[MAX_WORLD_USER];            //���߹��ҳ�Ա
    INT                     m_nCurUser;                         //��ǰ��������
    INT                     m_nGold;                            //ļ����
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



//���󵱹���
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

//�������ҹ�ְ
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

//������ҹ�ְ
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

//����ļ��
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


//���ҹ�Ա�б�
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

    COUNTRY_RETURN_REQUEST_KINDOM   = 0,        //���󵱹���
    COUNTRY_RETURN_APPOINT,                     //�����ְ
    COUNTRY_RETURN_REMOVE,                      //�����ְ
    COUNTRY_RETURN_DELATE,                      //����ͶƱ
    COUNTRY_RETURN_DELATE_SCUESS,               //����ͶƱ�ɹ�
    COUNTRY_RETURN_REQUEST_KINDOM_FAIL,         //���󵱹���ʧ��


    COUNTRY_ERROR_COUNTRY_ID        = 30,       //���Ҳ���
    COUNTRY_ERROR_USER_NOT_FIND,                //��Ҳ�����

    COUNTRY_RETURN_SIZE,
};


struct _COUNTRY_RETURN
{
    CHAR					m_ReturnType;
    CHAR                    m_cPosition;                        //��ְ
    CHAR                    m_cCountry;                         //����
    CHAR                    m_szName[MAX_CHARACTER_NAME];       //����
	//UINT                    m_nAP;                            //������Ϊ�� [ ZG ��� ]


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
