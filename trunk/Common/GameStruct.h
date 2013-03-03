
#ifndef __GAMESTRUCT_H__
#define __GAMESTRUCT_H__

#include "Type.h"
#include "GameDefine.h"


//#pragma pack(push, 1)

//��������������ĸ���λ��
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

//�������������������λ��
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




//Ч��״̬
struct _EFFECT
{
	BOOL				m_bActive ;
	INT					m_Value ;	//Ч��ֵ
	INT					m_Time ;	//Ч��ʱ��

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

//������������ʼ������
struct _MONSTERCREATER_INIT
{
	CHAR				m_FileName[_MAX_PATH] ;
	WORLD_POS			m_Position ;
};


#define			DEFAULT_ITEMBOX_RECYCLE_TIME	300000		//300�룬5����



//װ������
struct EQUIP_LIST
{
	GUID_t	m_GUID;			//װ������ID
	UINT	m_uParam1;		//װ������1
	UINT	m_uParam2;		//װ������2
};
#define EQUIP_PLAYER_FIXNUM		(8)		//����������������װ����

//��Ʒ����
struct EMBELLISH_LIST
{
	GUID_t	m_GUID;			//��Ʒ����ID
	UINT	m_uParam1;		//��Ʒ����1
	UINT	m_uParam2;		//��Ʒ����2
};
#define EMBELLISH_PLAYER_FIXNUM		(6)		//������������������Ʒ��



//��һ������л���Ϣ
struct PLAYER_OWN
{
	GUID_t    m_nGUID;						//�����ȫΨһID
	CHAR	  m_szName[MAX_CHARACTER_NAME];	//�������
	
	Coord_t   m_nX;						//���λ��X
	Coord_t   m_nZ;						//���λ��Z

	FLOAT     m_fDir;						//����泯�ķ��򣨷�Χ��0~1.0��
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
	GUID_t    m_nGUID;						//�����ȫΨһID
	CHAR	  m_szName[MAX_CHARACTER_NAME];	//�������
	
	Coord_t   m_nX;						//���λ��X
	Coord_t   m_nZ;						//���λ��Z

	FLOAT     m_fDir;						//����泯�ķ��򣨷�Χ��0~1.0��
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

//һ��ս�����Խṹ
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

//����ս�����Խṹ
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

//��ɫ��ӵ�еĳƺ�
//#��ͷ���ַ���������һ���ַ�����ԴID,����ͨ��������������������ñ���������
#define IDTOSTRING(str, strid, strsize)	char str[strsize];\
										memset(str, 0, strsize);\
										sprintf(str, "#%d", strid);\

#define STRINGTOID(str, strid)			INT strid = atoi((CHAR*)(str+1));\
										

struct _TITLE
{
	enum
	{
		NO_TITLE	 = -1,

		GUOJIA_TITLE = 1,       //���ҳƺ�
		BANGPAI_TITLE,          //���ɳƺ�
		WANFA_TITLE,            //�淨�ƺ�
		MOOD_TITLE,				//�������

		MAX_NUM_TITLE,
	};

    struct TITLE_INFO
    {
        INT     m_iTitleID;
        INT     m_iSuitID;      //��ϳƺ�ID    ��һ�ƺ���-1
        INT     m_iTitleType;   //�ƺ�����     ���ң���ᣬ�淨�ƺ�
        INT     m_iBuffID;      //�ƺŵ�BUFFid
        UINT    m_uTime;        //ʱ��title����ʱ�䣬�����ڵ���0  

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

    struct TITLE_COMBINATION                        //��ɳƺ���Ϣ
    {
        INT m_iGroupID;
        INT m_comTitleID;
        INT m_arPart[MAX_TITLE_COMBINATION];        //��ϳ�Ա����10��

        TITLE_COMBINATION ()
        {
            m_comTitleID = INVALID_ID;
            memset(m_arPart, 0, sizeof(INT)*MAX_TITLE_COMBINATION);
        }
    };


public:

    INT                 m_CurCountryTitle;                              //��ǰ���ҳƺ�ID  ��Ч��Ϊ-1
    INT                 m_CurGuildTitle;                                //��ǰ���ɳƺ�ID  ��Ч��Ϊ-1  
    INT                 m_CurNormalTitle;                               //��ǰ��ͨ�ƺ�ID  ��Ч��Ϊ-1
    TITLE_INFO          m_TitleArray[MAX_TITLE_SIZE];

    CHAR                m_szCurCountryTitle[MAX_CHARACTER_TITLE];      //��ǰ���ҳƺ�
    CHAR                m_szCurGuildTitle[MAX_CHARACTER_TITLE];        //��ǰ���ƺ�
    CHAR                m_szCurNormalTitle[MAX_CHARACTER_TITLE];       //��ǰ�淨�ƺ�

    CHAR                m_szOfficialTitleName[MAX_CHARACTER_TITLE];    //�Զ����ְ�ƺ�

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
//��Ʒʰȡ����
struct	ITEM_PICK_CTL
{
	ObjID_t				OwnerID;			//���յ�ʰȡ��ID
	uint				uBetTime;			//ϵͳ�Ĳ�ʱ��
	UCHAR				uMaxBetPoint;		//���Bet����
	PICK_RULER			ePickRuler;			//ϵͳ���Ʒ���


	ITEM_PICK_CTL()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		OwnerID			=	INVALID_ID;		//��������
		ePickRuler		=	IPR_FREE_PICK;	//����ʰȡ
		uBetTime		=	0;				//����ʰȡ
		uMaxBetPoint	=	0;
	}
};

typedef	ITEM_PICK_CTL	IPC;

#define MAX_PICKER_COUNT	6


//�����ܲ���ʰȡ����Ա�б�
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




//����˺���¼
#define		MAX_DAMAGE_REC_COUNT	10


//�˺���¼
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



//�˺�����
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
	// AbilityID_t	m_Ability_ID; ����Ҫ ID���������� ID
	WORD		m_Level; // ���ܵȼ�
	WORD		m_Exp; // ����������
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
	INT				m_nLevel;							//��ɫ�ȼ�
	INT				m_nMenPai;							//���� MENPAI_ATTRIBUTE
	INT				m_nPortrait;						// ͷ��
	GuildID_t		m_GuildID;							//���ID
	
	struct ReMember_ExtData
	{
		INT			m_nLevel;							//��ɫ�ȼ�
		INT			m_nMenPai;							//���� MENPAI_ATTRIBUTE
		INT			m_nPortrait;						//ͷ��
		GuildID_t	m_GuildID;							//���ID
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
//��ż��Ϣ
struct MarriageInfo
{
	GUID_t		m_SpouseGUID;								// ��ż�� GUID
//	UINT		m_uWeddingTime;								// ����ʱ��

	MarriageInfo()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		m_SpouseGUID = INVALID_ID;
	}
};
//ͽ����Ϣ
struct PrenticeInfo
{
//	UINT		m_uRecruitingTime;							// ��ͽʱ��
	time_t		m_BetrayingTime;							// ���һ����ʦʱ��
	UINT		m_uMoralPoint;								// ʦ�µ�
	UCHAR		m_uPrenticeCount;							// ͽ������
	GUID_t		m_PrenticeGUID[MAX_PRENTICE_COUNT];			// ͽ�ܵ� GUID

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
//ʦ����Ϣ
struct MasterInfo
{
	GUID_t		m_MasterGUID;								// ʦ���� GUID
//	UINT		m_uApprenticingTime;						// ��ʦʱ��
//	UINT		m_uBetrayingTime;							// �ϴ���ʦʱ��
//	UINT		m_uBetrayTimes;								// ��ʦ����

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
//�ʼ�
struct MAIL
{

	struct MailInfo
	{
		GUID_t		m_GUID;								// ������ GUID
		BYTE		m_SourSize ;
		INT			m_nPortrait;						// ������ͷ��
		BYTE		m_DestSize ;
		WORD		m_ContexSize ;
		UINT		m_uFlag ;							//�ʼ���־ enum MAIL_TYPE
		time_t		m_uCreateTime ;						//�ʼ�����ʱ��
		//ִ���ʼ�Ӧ�ò���
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

	GUID_t		m_GUID;								// ������ GUID
	BYTE		m_SourSize ;
	CHAR		m_szSourName[MAX_CHARACTER_NAME] ;	//������

	INT			m_nPortrait;						// ������ͷ��

	BYTE		m_DestSize ;
	CHAR		m_szDestName[MAX_CHARACTER_NAME] ;	//������

	WORD		m_ContexSize ;
	CHAR		m_szContex[MAX_MAIL_CONTEX] ;		//����

	UINT		m_uFlag ;							//�ʼ���־ enum MAIL_TYPE
	time_t		m_uCreateTime ;						//�ʼ�����ʱ��

	//ִ���ʼ�Ӧ�ò���
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
	BYTE		m_Count ;//�ʼ�����
	BYTE		m_TotalLeft ;//�û��ʺ�����ʼ�ʣ������

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

// �����ʼ���ָ���͸���ͬ�˵�ͬ�����ʼ�
#define MAX_RECEIVER 100
struct BATCH_MAIL
{
	GUID_t				m_GUID;								// GUID

	BYTE				m_SourSize;
	CHAR				m_szSourName[MAX_CHARACTER_NAME];	//������

	BYTE				m_ReceiverCount;					//����������
	struct
	{
		BYTE			m_DestSize;
		CHAR			m_szDestName[MAX_CHARACTER_NAME];	//������
	}m_Receivers[MAX_RECEIVER];

	WORD				m_ContentSize;
	CHAR				m_szContent[MAX_MAIL_CONTEX];		//����

	UCHAR				m_uFlag;							//�ʼ���־ enum MAIL_TYPE
	time_t				m_uCreateTime;						//�ʼ�����ʱ��

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
	CHAR				m_Name[MAX_CHARACTER_NAME];		            // ���û��Ľ�ɫ����
	CHAR				m_Account[MAX_ACCOUNT_LENGTH];	// �˽�ɫ�����˺�
	GUID_t				m_GUID;							            // ���û���Ψһ��
	INT                 m_nCountry;                                 // ����
	UINT				m_uMenPai;						            // ����
	INT					m_nPortrait;					            // ͷ��
	UCHAR				m_uFaceMeshID;					            // ����ģ��
	UCHAR				m_uHairMeshID;					            // ͷ��ģ��
	UINT				m_uHairColor;					            // ��ɫ
	INT					m_nLevel;						            // ����
	USHORT				m_uSex;							            // �Ա�
	CHAR				m_szTitle[MAX_CHARACTER_TITLE];	            // �ƺ�
	GuildID_t			m_GuildID;						            // ��� ID
    CHAR                m_szGuildName[MAX_GUILD_NAME_SIZE];         // �������
    CHAR                m_szFamilyName[MAX_GUILD_FAMILY_NAME_SIZE]; // ��������
	INT					m_iPostCode;								// �ʱ��

	UINT				m_uMoney;									//��ɫ���ϻ���
	UINT				m_uBankMoney;								//��ɫ���л���

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
	UCHAR	m_SqlStr[MAX_SQL_LENGTH];		//ִ�е�Sql���
	
	VOID	Clear()
	{
		memset(m_SqlStr,0,MAX_SQL_LENGTH);
	}

	VOID	Parse(const CHAR* pTemplate,...);
	
};

struct LONG_DB_QUERY
{
	UCHAR	m_SqlStr[MAX_LONG_SQL_LENGTH];		//ִ�е�Sql���

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
	UINT		m_Equip[HEQUIP_NUMBER];			//װ��
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
		
		m_Menpai	= INVALID_JOB;						//��ɫ����
		m_Country	= INVALID_COUNTRY;                  //����
		m_Sex		= INVALID_SEX;
	}

	GUID_t				m_GUID;							//��ɫȫ�ֱ��
	BYTE				m_Sex;							//�Ա�
	CHAR				m_Name[MAX_CHARACTER_NAME];		//��ɫ����
	INT					m_Level;						//��ɫ�ȼ�
	UINT				m_HairColor;					//ͷ����ɫ	
	BYTE				m_FaceColor;					//������ɫ
	BYTE				m_HairModel;					//ͷ��ģ��
	BYTE				m_FaceModel;					//����ģ��
	SceneID_t			m_StartScene;					//��ɫ���ڳ���
	INT					m_Menpai;						//��ɫ����
	INT					m_HeadID;						//ͷ�����
	DB_CHAR_EQUIP_LIST	m_EquipList;					//װ���б�
	INT                 m_Country;                      //����
};

// ��Ա�б��еĶ�Ա��Ϣ��
struct TEAM_LIST_ENTRY
{
	GUID_t				m_GUID;							// GUID
	SceneID_t			m_SceneID;						// ����ID
	ID_t				m_SceneResID;					// ������Դ ID	
	UINT				m_ExtraID;						// ��Ա�� PlayerID(WG) �� ObjID(GC)
	UCHAR				m_NameSize;						// ��������
	CHAR				m_Name[MAX_CHARACTER_NAME];		// ��Ա������
	INT					m_nPortrait;					// ͷ��
	USHORT				m_uDataID;						// ��Ա���Ա�
	UINT				m_uFamily;						// 2.����

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

//����̵��ΨһID
struct _PLAYERSHOP_GUID
{
	ID_t	m_World ;		//�����: 
	ID_t	m_Server ;		//����˳���ţ�
	ID_t	m_Scene ;		//������
	INT		m_PoolPos ;		//���ݳ�λ��

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


//�ܱ����
#define MIBAOUNIT_NAME_LENGTH			2	//ÿ����ֵ�ĳ���
#define MIBAOUNIT_VALUE_LENGTH			2	//ÿ����ֵ�ĳ���

#define MIBAOUNIT_NUMBER				3	//�ܱ���һ����Ч���ݵ��ܱ���Ԫ����

//7��7
#define MIBAO_TABLE_ROW_MAX				7	//�ܱ�ʹ�õı���������
#define MIBAO_TABLE_COLUMN_MAX			7	//�ܱ�ʹ�õı���������

//�ܱ���Ԫ
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

//����������֤��
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


//һ���ܱ�����
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
// �齱��ز�������
//////////////////////////////////////////////////////
#define MAX_PRIZE_STRING	20		//�μ��������˲��ƹ�Ա�ʺ��콱ͨ��Э�顷�еĽ�Ʒ���볤�ȶ���
#define MAX_PRIZE_NUMBER	30		//һ�������30����ͬ����Ľ�Ʒ
#define MAX_NEWUSER_CARD_SIZE	20	//���ֿ�����

enum PRIZE_TYPE_ENUM
{
	PRIZE_TYPE_INVALID	= 0,
	PRIZE_TYPE_CDKEY	= 1,	//�ƹ�Ա
	PRIZE_TYPE_YUANBAO	= 2,	//Ԫ��
	PRIZE_TYPE_NEWUSER	= 3,	//���ֿ�(�Ƹ���)
	PRIZE_TYPE_ZENGDIAN	= 4,	//����
	PRIZE_TYPE_ITEM		= 5,	//��Ʒ
	PRIZE_TYPE_SPORTS	= 6,	//�������¿�
	PRIZE_TYPE_JU		= 7,	//���ۻ��
};

//Billing���صĽ�Ʒ�ṹ
struct _PRIZE_DATA
{
	CHAR	m_PrizeString[MAX_PRIZE_STRING];	//��Ʒ����
	BYTE	m_PrizeNum;							//��Ʒ����

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

//Billing���صĹ���ṹ
struct _RETBUY_DATA
{
	UINT	m_BuyInt;						//��Ʒ����(Ԫ��)
	USHORT	m_BuyNumber;					//��Ʒ����

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

//��Ʒ�ṹ
struct _BUY_DATA
{
	CHAR	m_BuyString[MAX_PRIZE_STRING];	//��Ʒ����(CD-KEY)
	UINT	m_BuyPoint;						//��Ʒ���ĵ���
	UINT	m_BuyInt;						//��Ʒ����(Ԫ��)
	UINT	m_BuyNumber;					//��Ʒ����

	_BUY_DATA()
	{
		memset(m_BuyString,0,MAX_PRIZE_STRING);
		m_BuyPoint	= 0;
		m_BuyInt	= 0;
		m_BuyNumber	= 0;
	}

	BYTE	GetPrizeType();						//��Ʒ����
	UINT	GetPrizeSerial();					//��Ʒ���к�
	BYTE	GetPrizeNum();						//��Ʒ����
	UINT	GetCostPoint();						//���ĵ���

	VOID	GetSubString(INT nIdx,CHAR* pBuf,INT nBufLength);	//��ֽ�Ʒ�ִ�

	BYTE	GetGoodsType();						//��Ʒ����
	USHORT	GetGoodsNum();						//��Ʒ����
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

//�ʺŰ�ȫ���
enum _ACCOUNT_SAFE_FLAG
{
	ASF_BIND_NONE = 0,
	ASF_BIND_IP =2,						//IP��
	ASF_BIND_MIBAOKA = 4,				//�ܱ�����
	ASF_BIND_MOBILE_PHONE = 8,			//�ֻ���
	ASF_BIND_MAC = 16,					//MAC��
	ASF_BIND_REALNAME = 32,				//ʵ����֤�������������ݿ���֤����
	ASF_BIND_INPUTNAME = 64,			//��д��ʵ���������WEB����д��ʵ������Ϣ������δͨ�����������ݿ���֤��
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

//���Ѽ�¼�ṹ
struct _COST_LOG
{
	CHAR			m_SerialKey[MAX_PRIZE_SERIAL_LENGTH+1];		//�������к�
	INT				m_WorldId;									//World��
	INT				m_ServerId;									//Server��
	INT				m_SceneId;									//Scene��
	GUID_t			m_UserGUID;									//�û�GUID
	INT				m_UserLevel;								//�û��ȼ�
	LONG			m_CostTime;									//����ʱ��(��1970-01-01 ������)
	INT				m_YuanBao;									//���ĵ�Ԫ����

	CHAR			m_AccName[MAX_ACCOUNT+1];					//�ʺ�
	CHAR			m_CharName[MAX_CHARACTER_NAME+1];			//��ɫ
	CHAR			m_Host[IP_SIZE+1];							//IP
	CHAR			m_OtherInfo[MAX_COST_OTHER_SIZE+1];			//Log�ı�ע��Ϣ

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
//1 �� SOUXIA_DATA �й̶����ԣ��˻�Ϊͨ������ȡ
struct  SouXia_Skill
{
	SkillID_t	StudySkillId[MAX_SKILL_COUNT]; // ѧϰ����Skill ����
	BYTE		StudyCount;
	//�Ӹ���ֵ����,������������
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
	SkillID_t	StudyProductId[MAX_PRODUCT_COUNT]; // ѧϰ���������䷽ ����
	BYTE		StudyCount;

	//�Ӹ���ֵ����,������������
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
	SkillID_t	StudyZhaoHuan; // ѧϰ���������ٻ���������
	SHORT		LeftUseTime;   // ������ʹ�õĴ���

	//�Ӹ���ֵ����,������������
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
	ZhaoHuan	StudyPet[MAX_PET_ZHAOHUAN_COUNT]; // ѧϰ���������ٻ���������
	BYTE		StudyCount;   // ��ǰѧϰ������

	//�Ӹ���ֵ����,������������
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
	ZhaoHuan	StudyZuoji[MAX_ZUOJI_ZHAOHUAN_COUNT]; // ѧϰ���������ٻ���������
	BYTE		StudyCount;   // ��ǰѧϰ������

	//�Ӹ���ֵ����,������������
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
ע��:���ڿ�������͸�ֵ������ϵͳĬ���ṩλ����,�Ѿ�ʵ���˲����õ�����ʱ�ò�������ûд
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
	UINT					m_SouXiaID;	// ����¼����
	SouXia_Skill			m_Skill;	// ��������������
	SouXia_Product			m_Product;	//  ...  �����䷽
	SouXia_PetZhaoHuan		m_Pet;		//  ...  �����ٻ�
	SouXia_ZuojiZhaoHuan	m_ZuoJi;	//  ...  �����ٻ�

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

	// ȡ�õ�ǰ�����ܵ�ҳ��
	BYTE	GetCurSkillPage()			{ return (BYTE)(((GetCurSkillCount()+SKILL_PER_PAGE)-1)/SKILL_PER_PAGE); }
	BYTE	GetCurProductPage()			{ return GetCurProductCount(); }
	BYTE	GetCurPetZhaoHuanPage()		{ return (BYTE)(((GetCurPetCount()+PET_ZHAOHUAN_PER_PAGE)-1)/PET_ZHAOHUAN_PER_PAGE); }
	BYTE	GetCurZuoJiZhaoHuanPage()	{ return (BYTE)(((GetCurZuoJiCount()+ZUOJI_ZHAOHUAN_PER_PAGE)-1)/ZUOJI_ZHAOHUAN_PER_PAGE); }

	VOID	ReadSouXiaVarAttr(SocketInputStream& iStream);
	VOID	WriteSouXiaVarAttr(SocketOutputStream& oStream) const;
};

//������ļ����õ�ǰ��Ķ���
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
