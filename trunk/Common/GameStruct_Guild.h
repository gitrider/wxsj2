/********************************************************************************
 *	�ļ�����	GameStruct_Guild.h
 *	ȫ·����	d:\Prj\Common\GameStruct_Guild.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 12 �� 9 ��	17:12
 *
 *	����˵����	����һЩ���ݽṹ����Ϣ�ṹ
 *				GUILD_PACKET ����Ϣ���Ľӿ� GUILD_CGW_ASKLIST �Ǿ����
 *				_GUILD_PACKET ���ṩ����Ϣʹ�õ� struct
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef __GAMESTRUCT_GUILD_H__
#define __GAMESTRUCT_GUILD_H__

#include "Type.h"
#include "GameDefine_Guild.h"

class SocketInputStream;
class SocketOutputStream;


// ���ȼ���Ϣ
struct  _GUILD_LEVEL_INFO
{
    CHAR        m_Level;                //���ȼ�
    CHAR        m_MaxFamily;            //�����������
    INT         m_Gold;                 //�ü������Ǯ
    INT         m_Contribute;           //�ü����蹱��
    INT         m_MaxGuildUser;         //�������Ա����
    INT         m_MaxGoldFamilyUser;    //�ƽ������������

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

// �����Ϣ���Ľӿ�
struct GUILD_PACKET
{
	virtual UINT			GetPacketSize() const = 0;
	virtual VOID			Read(SocketInputStream& iStream) = 0;
	virtual VOID			Write(SocketOutputStream& oStream) const = 0;
};

// ѯ�ʰ���б�
struct GUILD_CGW_ASKLIST : public GUILD_PACKET
{
	UCHAR					m_SortType;
	USHORT					m_uStart;

	virtual UINT			GetPacketSize() const { return sizeof(m_SortType) + sizeof(m_uStart); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// �������
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

// �޸İ����ּ
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

// �޸���Ұ��ƺ�
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

// ������
struct GUILD_CGW_JOIN : public GUILD_PACKET
{
	GuildID_t				m_GuildGUID;
    GUID_t                  m_TargetGuid;

	virtual UINT			GetPacketSize() const { return sizeof(m_GuildGUID)+sizeof(m_TargetGuid); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// ���������
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

// ��������
struct GUILD_CGW_CREATE_FAMILY : public GUILD_PACKET
{
    GuildID_t				m_GuildGUID;
    INT                     m_ShaikhIndex;      //���峤Index
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

// ��Ӽ����Ա
struct GUILD_CGW_ADD_FAMILY_USER : public GUILD_PACKET
{
    CHAR        m_cFamilyIndex; 
    INT         m_iGuildIndex;
    GUID_t      m_TargetGuid;   //��ӳ�Ա��GUID

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

// ɾ�������Ա
struct GUILD_CGW_REMOVE_FAMILY_USER : public GUILD_PACKET
{
    CHAR        m_cFamilyIndex; 
    INT         m_iGuildIndex;
    GUID_t      m_TargetGuid;   //��ӳ�Ա��GUID

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

// ��ɢ����
struct GUILD_CGW_FAMILY_DISMISS : public GUILD_PACKET
{
    GuildID_t				m_GuildGUID;
    INT     				m_nFamilyID;				

    virtual UINT			GetPacketSize() const { return sizeof(m_GuildGUID)+sizeof(INT); }
    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;
};

// ѯ�ʰ����Ϣ
struct GUILD_CGW_ASKINFO : public GUILD_PACKET
{
	enum
	{
		GUILD_MEMBER_INFO = 0,	//������Ϣ
		GUILD_INFO,			//�����Ϣ
		GUILD_APPOINT_POS,	//����п�������ְλ
		GUILD_SELF_INFO,	//���˰�����Ϣ

	};

    GUILD_CGW_ASKINFO()
    {
        memset(m_Password, 0, MAX_PWD);
    }

	GuildID_t				m_GuildGUID;
	BYTE					m_Type;				//������Ϣ�����ǰ����Ϣ

    BYTE                    m_cPwdSize;             
    CHAR                    m_Password[MAX_PWD];    //���뱣��

	virtual UINT			GetPacketSize() const { return sizeof(m_GuildGUID)+sizeof(BYTE)+m_cPwdSize+sizeof(m_cPwdSize); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// �������ְ��
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

// ������Ȩ��
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

// �������
struct GUILD_CGW_LEVELUP : public GUILD_PACKET
{
    GuildID_t				m_GuildGUID;
    GUID_t					m_ChiefGUID;
    UCHAR					m_GuildLevel;
    INT 					m_GP;           //�ͻ��˲�����

    virtual UINT			GetPacketSize() const
    {
        return sizeof(m_GuildGUID) + sizeof(m_ChiefGUID) + sizeof(m_GuildLevel) +sizeof(INT);
    }
    virtual VOID			Read(SocketInputStream& iStream);
    virtual VOID			Write(SocketOutputStream& oStream) const;
};

// �ﹱ
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

// ���ȼ���Ϣ
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

// �����°���
struct GUILD_CGW_RECRUIT : public GUILD_PACKET
{
	GUID_t					m_ProposerGUID;

	virtual UINT			GetPacketSize() const { return sizeof(m_ProposerGUID); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// ��������
struct GUILD_CGW_EXPEL : public GUILD_PACKET
{
	GUID_t					m_GuildUserGUID;

	virtual UINT			GetPacketSize() const { return sizeof(m_GuildUserGUID); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// ��ȡ����
struct GUILD_CGW_WITHDRAW : public GUILD_PACKET
{
	UINT					m_MoneyAmount;

	virtual UINT			GetPacketSize() const { return sizeof(m_MoneyAmount); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// �������
struct GUILD_CGW_DEPOSIT : public GUILD_PACKET
{
	UINT					m_MoneyAmount;

	virtual UINT			GetPacketSize() const { return sizeof(m_MoneyAmount); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// �뿪���
struct GUILD_CGW_DEMISE : public GUILD_PACKET
{
	virtual UINT			GetPacketSize() const { return 0; }
	virtual VOID			Read(SocketInputStream& iStream) {}
	virtual VOID			Write(SocketOutputStream& oStream) const {}
};

// �뿪���
struct GUILD_CGW_LEAVE : public GUILD_PACKET
{
	virtual UINT			GetPacketSize() const { return 0; }
	virtual VOID			Read(SocketInputStream& iStream) {}
	virtual VOID			Write(SocketOutputStream& oStream) const {}
};

// ���İ����ּ
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

// ȷ������
struct GUILD_CGW_INVITE_CONFIRM : public GUILD_PACKET
{
	GuildID_t		m_GuildId;		// ���ID
    BOOL			m_AgreeInvite;	// ͬ������
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


//���Ŀ�������Ϣ
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

//�����Ϣ
struct GUILD_WGC_GUILD_INFO : public GUILD_PACKET
{
	CHAR			m_GuildName[MAX_GUILD_NAME_SIZE];
	CHAR			m_GuildCreator[MAX_CHARACTER_NAME];
	CHAR			m_GuildChairMan[MAX_CHARACTER_NAME];
	CHAR			m_CityName[MAX_CITY_NAME_SIZE];
	BYTE			m_nLevel;
	INT				m_nPortSceneID;				//��ڳ���
	INT				m_MemNum;					//����
	INT				m_Longevity;				//���� 
	INT				m_Contribute;				//���׶�
	INT				m_Honor;					//����
	INT				m_FoundedMoney;				//�����ʽ�
	INT				m_nIndustryLevel;			//��ҵ��
	INT				m_nAgrLevel;				//ũҵ��
	INT				m_nComLevel;				//��ҵ��
	INT				m_nDefLevel;				//������
	INT				m_nTechLevel;				//�Ƽ���
	INT				m_nAmbiLevel;				//���Ŷ�

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
		m_nPortSceneID		=	0;			//��ڳ���
		m_MemNum			=	0;			//����
		m_Longevity			=	0;			//���� 
		m_Contribute		=	0;			//���׶�
		m_Honor				=	0;			//����
		m_FoundedMoney		=	0;			//�����ʽ�
		m_nIndustryLevel	=	0;			//��ҵ��
		m_nAgrLevel			=	0;			//ũҵ��
		m_nComLevel			=	0;			//��ҵ��
		m_nDefLevel			=	0;			//������
		m_nTechLevel		=	0;			//�Ƽ���
		m_nAmbiLevel		=	0;			//���Ŷ�
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

// �����б�
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
            m_iSex              =   2; //����
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

// ����б�
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

//�޸İ����ҳƺ�����
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


// ת����������������
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


//���˰�������
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

//����������
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
	UCHAR					m_uPacketType;			// ��Ϣ������
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
	{ // ��������Ϣ������һ�������
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
	{ // ��������Ϣ������һ�������
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
	GUID_t					m_GUID; // ��Ҫ���� GuildID ����ҵ� GUID
	GUID_t					m_GUIDChanged; // ��Ҫ���� GuildID ����ҵ� GUID
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
    BOOL					m_bScuess;          //�Ƿ�ɹ�


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

//��ͬ�������Ӧ�Ĳ�ͬ�������ṹ
struct GUILD_ADMIN_t
{
	enum ORESULT
	{
		RET_SUCCESS = 0,
		RET_NOT_EXIST,	//�����ڱ�ְλ������
		RET_POS_FULL,	//ְλ�˶�����
		RET_NO_USER,	//��Ҳ�����

		RET_UNKOWN,
	};
	enum
	{
		POS_LEVEL0_BEGIN = 0,

		POS_CHAIR_MAN,				//����
		POS_ASS_CHAIR_MAN ,			//������
        POS_NORMAL_FAMILY_BOSS,     //���峤
        POS_ELITE_MEMBER,           //��Ӣ���� �ѷ������
		POS_MEMBER,					//δ�������
        
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
