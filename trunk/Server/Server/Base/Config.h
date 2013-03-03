

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "Type.h"


//Config
struct _CONFIG_INFO
{
	ID_t			m_ServerID ;
	INT				m_SystemMode;				//ϵͳģʽ0��ʾ������ģʽ,1��ʾ����ģʽ
	INT				m_ZoneSize ;
	FLOAT			m_DropParam;				//ȫ�ֵ�����������
	FLOAT			m_ExpParam;					//ȫ�־�����������
	INT				m_EquipmentDamagePoint;		//��Ʒ�;öȵ�΢��ֵ
	INT				m_DropSearchRange;			//��������������
	FLOAT			m_DropPosDistance;			//������Ʒ���
	INT				m_LevelupAllocPoints;		//���������������������ҿ��ֶ����㣩
	INT				m_RecoverTime ;
	INT				m_MaxMonster ;
	INT				m_MaxPet ;
	INT				m_PetBodyTime ;
	INT				m_PetHappinessInterval;		// ������ֶ��½�1��ʱ����(����)
	INT				m_PetLifeInterval;			// ���������½�1��ʱ����(����)
	INT				m_PetCallUpHappiness;		// ������ֶȴ��ڲſ����ٳ�
	UINT			m_PetPlacardTime;			// ����һ�����﹫��ĳ���ʱ��
	INT				m_PetPlacardNeedLevel;		// �������﹫��ĳ������輶��
	INT				m_PetPlacardNeedHappiness;	// �������﹫��ĳ���������ֶ�
	INT				m_PetPlacardNeedLife;		// �������﹫��ĳ�����������
	INT				m_nHorseCheckUpSpend;		// ��������ѵ���Ϸ��
	INT				m_nHorseMattingSpend;		// ����Ҫ���ѵ���Ϸ��
	INT				m_DefaultRespawnTime ;
	INT				m_ValidAttackTime;
	INT				m_DropBoxRecycleTime;
	INT				m_DropProtectTime;
	INT				m_TimeChangeInterval;		//ʱ��������
	INT				m_PositionRange ;
	INT				m_AIType ;
	INT				m_DisconnectTime ;
	CHAR			m_UserPath[_MAX_PATH] ;
	INT				m_MaxPortal ;
	INT				m_MaxSkillObj ;
	INT				m_DefaultBodyTime ;			//ʬ�����ʱ��
	INT				m_OutGhostTime ;			//�ͷ����ʱ��
	FLOAT			m_CanGetExpRange ;			//ɱ����ɫ����Եõ�EXP�ķ�Χ
	INT				m_DefaultMoveSpeed ;
	INT				m_DefaultAttackSpeed ;
	INT				m_HumanVERecoverInterval;	// ����������Ļָ�ʱ����(����)
	INT				m_HumanHGRecoverInterval;	// Ѫ�Ļָ�ʱ���������룩
	INT				m_HumanPneumaRecoverInterval;	// Ԫ���Ļָ�ʱ���������룩
	INT				m_RAGERecoverInterval;		//ŭ���Ļָ�ʱ���������룩

	INT				m_DefaultRefreshRate ;		//������ˢ��ʱ��
	INT				m_DefaultCheckZoneRate ;	//ˢ��������Ϣ��ʱ����
	INT				m_DefaultRefuseScanTime;	//ȱʡ���������󲻻�ɨ����˵�ʱ��
	INT				m_MaxPlatform;
	INT				m_MaxSpecial;				// ����OBJ������
	INT				m_MaxBus;					//���Bus����
	INT				m_MaxPlayerShopNum;			// ����̵������
	INT             m_MaxTimerCount;			//������ʱ����Ŀ
	INT				m_nMaxHumanUseTimer;		//Obj_Human��ʹ�ü�ʱ����Ŀ
	INT				m_nAvailableFollowDist;		// ��Ӹ������Ч����
	INT				m_nTimeForLoseFollow;		// ��Ӹ��泬����Ч����೤ʱ��������˳���Ӹ���(��)
	INT				m_nFoundDurationHour;		// ��������
	INT				m_nDefaultMaxMemberCount;	// Ĭ�ϰ�����ɰ�����
	INT				m_nResponseUserCount;		// ���������Ҫ����Ӧ���������������
	INT				m_nPasswdPoint;				// ���ٵ��Ѻö���Ҫ�����������
	INT				m_nPromptPoint;				// ���ٵ��Ѻö���Ҫ��ʾ������
	INT				m_nDeleteDelayTime;			// ����Сʱ�Ժ��ٴ�ǿ�ƽ����ɾ����������
	INT				m_nExpPoint;				// ��ö��پ�����������Ѻö�
	INT				m_nHashOnlineUserCount ;	//������ҵ�hash���С
	INT				m_nHashMailUserCount ;		//ӵ���ʼ������hash���С
	INT				m_nMaxOfflineUserCount ;	//��½����Ϸ������������ֵ
	FLOAT			m_fRespawnParam ;			//����ˢ������ϵ��
	INT				m_KickUserTime ;			//����������Ϸ��ҵ��߳�����ʱ��
	INT				m_SMU_KickUserTime;			//������ҵ������߳�ʣ��ʱ��
	INT				m_nDefaultDamageFluctuation; // ȱʡ���˺�����
	INT				m_nCriticalDamageRate;		//���������˺�����
	INT				m_nToughEffCriticalDamage;	//���ԶԱ����˺���Ӱ�����

	INT				m_nMinGoodBadValue;			// ��С�ƶ�ֵ
	INT				m_nMaxGoodBadValue;			// ����ƶ�ֵ
	INT				m_nLevelNeeded;				// ����ƶ�ֵ����͵ȼ�
	INT				m_nMemberLevel;				// �ܸ��ӳ������ƶ�ֵ�Ķ�����ߵȼ�
	FLOAT			m_fGoodBadRadius;			// ����ƶ�ֵ�İ뾶
	INT				m_nBonusPerMember;			// ÿ����Ч���������ӵ��ƶ�ֵ����
	INT				m_nMaxBonus;				// ɱ��һֻ���ܸ��ӳ�����������ƶ�ֵ����
	INT				m_nPenaltyWhenMemberDie;	// ��Ч���ѵ��������ٶӳ����ƶ�ֵ����

	INT				m_nMinPaiScore;				//��С�����������
	INT				m_nMinZiPaiScore;			//��С�����������
	INT				m_nComMonsterDrop;			//��ͨ��Ӱ�����Եȼ��ӳ�
	INT				m_nSenMonsterDrop;			//�����Ӱ�����Եȼ��ӳ�
	INT				m_nBossDrop;				//BossӰ�����Եȼ��ӳ�
	INT				m_nRefreshAttrToWorldInterval;	//��Worldˢ�����ݵ�ʱ����

	INT				m_nSubTrainExp;				//��������Ч�ʳ���
	INT				m_nSubTrainMoney;			//������ǮЧ�ʳ���
    INT				m_nDelateNum;	    		//��������Ʊ�����߻�Ĭ��3
    INT				m_nVoteNum;	    		    //ͶƱ����Ч�����ޣ��߻�Ĭ��100
    INT				m_nVoteTime;                //ͶƱ����ʱ�䣬�߻�Ĭ��4��Сʱ,��λ����

	_CONFIG_INFO( )
	{
		m_ServerID				= INVALID_ID ;
		m_SystemMode			= 0;
		m_ZoneSize				= 10 ;
		m_EquipmentDamagePoint	= 1000;
		m_DropSearchRange		= 200;
		m_DropPosDistance		= 2.0;
		m_RecoverTime			= 1000 ;
		m_MaxMonster			= 1000 ;
		m_MaxPet				= 1000 ;
		m_PetBodyTime			= 2000 ;
		m_PetHappinessInterval	= 600000;
		m_PetLifeInterval		= 30 * 60 * 1000;
		m_PetCallUpHappiness	= 60;
		m_PetPlacardTime		= 1000*60*30;
		m_DefaultRespawnTime	= 300000 ;
		m_DropBoxRecycleTime	= 300000 ;
		m_ValidAttackTime		= 6000;
		m_DropProtectTime		= 100000 ;
		m_TimeChangeInterval	= 300000 ;
		m_PositionRange			= 10 ;
		m_AIType				= 1 ;
		m_DisconnectTime		= 0 ;
		memset( m_UserPath, 0, _MAX_PATH ) ;
		m_MaxPortal				= 0 ;
		m_MaxPlatform			= 0;
		m_MaxSpecial			= 1024;
		m_MaxBus	=64;
		m_MaxPlayerShopNum		= 512;
		m_MaxSkillObj			= 1024;
		m_DefaultBodyTime		= 0 ;
		m_OutGhostTime			= 0 ;
		m_CanGetExpRange		= 30.f;
		m_DefaultMoveSpeed      = 2800 ;
		m_DefaultAttackSpeed    = 2000 ;
		m_HumanVERecoverInterval= 300000;
		m_HumanHGRecoverInterval= 5000;
		m_RAGERecoverInterval= 5000;		

		m_DefaultRefreshRate    = 60000 ;
		m_DefaultCheckZoneRate  = 1000 ;
		m_nAvailableFollowDist	= 15;
		m_nTimeForLoseFollow	= 30;
		m_nFoundDurationHour	= 24;
		m_nDefaultMaxMemberCount= 160;
		m_nResponseUserCount	= 10;
		m_nPasswdPoint			= 0;
		m_nPromptPoint			= 0;
		m_nDeleteDelayTime		= 0;
		m_nExpPoint				= 0;
		m_nHashOnlineUserCount	= 1000 ;
		m_nHashMailUserCount	= 1000 ;
		m_nMaxOfflineUserCount	= 0 ;
		m_fRespawnParam			= 1.0f ;
		m_KickUserTime			= 300000 ;
		m_nDefaultDamageFluctuation = 10 ;
		m_nCriticalDamageRate	= 100;		//���������˺�����
		m_nToughEffCriticalDamage = 100;	//���ԶԱ����˺���Ӱ�����

		m_nMinGoodBadValue		= 0;
		m_nMaxGoodBadValue		= 9999;
		m_nLevelNeeded			= 31;
		m_nMemberLevel			= 19;
		m_fGoodBadRadius		= 15.;
		m_nBonusPerMember		= 1;
		m_nMaxBonus				= 2;
		m_nPenaltyWhenMemberDie	= 10;

		m_nMinPaiScore			= 100;				
		m_nMinZiPaiScore		= 300;			
		m_nComMonsterDrop		= 0;			
		m_nSenMonsterDrop		= 0;			
		m_nBossDrop				= 0;
		m_nHorseCheckUpSpend	= 100;
		m_nHorseMattingSpend	= 20000;
		m_nRefreshAttrToWorldInterval = 300000;//5 minutes
		
        m_nDelateNum            = 3;
        m_nVoteNum              = 100;	 
        m_nVoteTime             = 240; 
	};
	~_CONFIG_INFO( )
	{
	};
};

//Login
struct _LOGIN_INFO
{
	ID_t			m_LoginID ;
	CHAR			m_DBIP[IP_SIZE];
	UINT			m_DBPort;
	CHAR			m_DBName[DATABASE_STR_LEN];
	CHAR			m_DBUser[DB_USE_STR_LEN];
	CHAR			m_DBPassword[DB_PASSWORD_STR_LEN];
	INT				CryptPwd;
	INT				ProxyConnect;			//�ͻ������Դ��������,0����������ӣ�1������������ӣ�2���������������
	INT				EnableLicense;			//>0�ͻ��˱���ͬ���µ����Э�飬<=0����ͬ��
	INT				ReLoginLimit;			//�Ƿ���ReLogin���ƹ���
	INT				NotifySafeSign;			//�Ƿ���а�ȫ����

	_LOGIN_INFO()
	{
		m_LoginID = INVALID_ID;
		CryptPwd  = 0;
		ProxyConnect = PERMIT_PROXY_CONNECT;
		EnableLicense = 0;
		ReLoginLimit = 1;
		NotifySafeSign = 0;
	}
	~_LOGIN_INFO()
	{

	}
};

struct _BILLING_DATA
{
	CHAR			m_IP[IP_SIZE] ;		//Billing��IP��ַ
	uint			m_Port ;			//Billing�ļ����˿�
	INT				m_ContainerPos;		//�����ݳ��е�λ��

	_BILLING_DATA()
	{
		memset( m_IP, 0, IP_SIZE ) ;
		m_Port = 0 ;
		m_ContainerPos = -1;
	}

	~_BILLING_DATA(){}
};

//Billing
class _BILLING_INFO
{
public:
	_BILLING_INFO():
	  m_pInfo(NULL),
		  m_nNumber(0),
		  m_nCur(0),
		  m_bUse(FALSE)
	  {
	  }

	~_BILLING_INFO()
	{
		  CleanUp();
	  }

	  VOID				CleanUp()
	  {
		  if(m_pInfo && m_nNumber > 0)
		  {
			  for(INT i = 0; i < m_nNumber; ++i)
			  {
				  if(m_pInfo[i])
				  {
					  SAFE_DELETE(m_pInfo[i]);
	}
			  }
			  SAFE_DELETE_ARRAY(m_pInfo);
			  m_pInfo = NULL;
		  }
		  memset( m_IP, 0, IP_SIZE ) ;
		  m_nNumber = 0;
		  m_nCur = 0;
		  m_bUse = FALSE;
		  m_Port = 0;
	  }

	  BOOL				Init(INT nNumber)
	  {
		  Assert(nNumber > 0);
		  m_nNumber = nNumber;

		  m_pInfo	= new _BILLING_DATA*[m_nNumber];
		  Assert(m_pInfo);

		  for(INT i = 0; i < m_nNumber; ++i)
		  {
			  m_pInfo[i] = new _BILLING_DATA();
			  Assert(m_pInfo[i]);
			  m_pInfo[i]->m_ContainerPos = i;
		  }

		  return TRUE;
	  }

	  INT					GetNumber()			{return m_nNumber;}

	  _BILLING_DATA*		Next()
	  {
		  _BILLING_DATA* cur = m_pInfo[m_nCur];
		  m_nCur = (m_nCur+1 >= m_nNumber)?0:m_nCur+1;
		  return cur;
	  }

	  VOID				BeginUse()
	  {
			m_nCur = 0;
			m_bUse = TRUE;

			if (m_pInfo)
			{
				_BILLING_DATA* cur = m_pInfo[m_nCur];
				m_Port = cur->m_Port;
				sprintf(m_IP, "%s", cur->m_IP);
			}
	  }

	  BOOL				IsUse()			{return m_bUse;}

public:
	uint				m_Port ;			//Billing�ļ����˿�
	CHAR			m_IP[IP_SIZE] ;		//Billing��IP��ַ
private:
	_BILLING_DATA**	m_pInfo;		//���ݳ�
	INT				m_nNumber;		//Billing������
	INT				m_nCur;			//��ǰʹ�õ�Billing��

	BOOL			m_bUse;			//�Ƿ��������ʹ��
};
//World
struct _WORLD_INFO
{
	ID_t			m_WorldID ;
	INT			m_ZoneID;		//����һ��World�ı�ʾ,��ֹ��ͬWorld֮������ݿ�����
	SM_KEY			m_GuildKey;
	SM_KEY			m_MailKey;
	BOOL			m_EnableShareMem;
	_WORLD_INFO( )
	{
		m_WorldID	= INVALID_ID ;
		m_GuildKey	=	0;
		m_MailKey	=	0;
		m_EnableShareMem = FALSE;
	}
	~_WORLD_INFO( )
	{
	}
};

//ShareMem

struct _SHAREMEM_DATA
{
	SM_KEY				m_Key;
	SHAREMEM_TYPE		m_Type;

	_SHAREMEM_DATA()
	{
		m_Key	=	0;
		m_Type	=	ST_INVAILD;
	}

};

struct _SHAREMEM_INFO 
{
	uint				m_SMUObjCount;

	_SHAREMEM_DATA*	    m_pShareData;

	CHAR			m_DBIP[IP_SIZE];
	UINT			m_DBPort;
	CHAR			m_DBName[DATABASE_STR_LEN];
	CHAR			m_DBUser[DB_USE_STR_LEN];
	CHAR			m_DBPassword[DB_PASSWORD_STR_LEN];
	INT				SMUInterval;
	INT				DATAInterval;
	UINT			CryptPwd;
	_SHAREMEM_INFO()
	{
		m_pShareData	=	NULL;
		m_SMUObjCount	=	0;
		memset(m_DBIP,0,IP_SIZE);
		m_DBPort		= -1;
		memset(m_DBName,0,DATABASE_STR_LEN);
		memset(m_DBUser,0,DB_USE_STR_LEN);
		memset(m_DBPassword,0,DB_PASSWORD_STR_LEN);
		SMUInterval		= 60;
		DATAInterval	= 300000;
		CryptPwd = 0;
	}
	~_SHAREMEM_INFO()
	{
		SAFE_DELETE_ARRAY(m_pShareData);
	};
};

//Machine
struct _MACHINE_DATA
{
	ID_t			m_MachineID ;

	_MACHINE_DATA( )
	{
		Init( ) ;
	};
	VOID Init( )
	{
		m_MachineID = INVALID_ID ;
	}
};

struct _MACHINE_INFO
{
	_MACHINE_DATA*	m_pMachine ;
	uint			m_MachineCount ;

	_MACHINE_INFO( )
	{
		m_pMachine = NULL ;
		m_MachineCount = 0 ;
	};
	~_MACHINE_INFO( )
	{
		SAFE_DELETE_ARRAY( m_pMachine ) ;
		m_MachineCount = 0 ;
	};
};
//proxy
typedef enum _ENUM_ISP
{
	INVALID_ISP = -1,
	ISP_CHINA_NET_COM = 0,
	ISP_CHINA_TELE_COM,
	ISP_CHINA_EDU,
	NUM_OF_ISP
} ENUM_ISP;
// IspID��˳������_ENUM_ISP��˳��һ��
extern CHAR g_szIspID[NUM_OF_ISP][16];

typedef struct _PROXY_DATA
{
	ID_t 	m_nIsp;
	CHAR	m_szIP[IP_SIZE];
	UINT	m_nPort;
	BOOL	m_bEnabled;
	_PROXY_DATA(VOID): m_nIsp(INVALID_ISP), m_nPort(0xFFFFFFFF), m_bEnabled(FALSE)
	{
		m_szIP[0]='\0';
	}
	VOID CleanUp(VOID)
	{
		m_nIsp = INVALID_ISP;
		m_szIP[0] ='\0';
		m_nPort = 0xFFFFFFFF;
		m_bEnabled = FALSE;
	}
} PROXY_DATA;

//Server
struct _SERVER_DATA
{
	ID_t			m_ServerID ;
	ID_t			m_MachineID ;
	CHAR			m_IP0[IP_SIZE] ;
	uint			m_Port0 ;
	CHAR			m_IP1[IP_SIZE] ;
	uint			m_Port1 ;
	INT				m_Type ;
	PROXY_DATA		m_aProxy[NUM_OF_ISP];

	SM_KEY			m_HumanSMKey;
	SM_KEY			m_PlayShopSMKey;
	SM_KEY			m_ItemSerialKey;
	SM_KEY			m_CommionShopSMKey;
	BOOL			m_EnableShareMem;


	_SERVER_DATA( )
	{
		Init( ) ;
	};
	VOID Init( )
	{
		m_ServerID = INVALID_ID ;
		m_MachineID = INVALID_ID ;
		memset( m_IP0, 0, IP_SIZE ) ;
		m_Port0 = 0 ;
		memset( m_IP1, 0, IP_SIZE ) ;
		m_Port1 = 0 ;
		m_Type = -1 ;
		for(INT i = ISP_CHINA_NET_COM; NUM_OF_ISP>i; ++i)
		{
			m_aProxy[i].CleanUp();
		}
		m_EnableShareMem = FALSE;
	}
};

struct _SERVER_WORLD
{
	CHAR			m_IP[IP_SIZE] ;
	uint			m_Port ;

	_SERVER_WORLD( )
	{
		Init( ) ;
	}
	VOID Init( )
	{
		memset( m_IP, 0, IP_SIZE ) ;
		m_Port = 0 ;
	}
};



#define OVER_MAX_SERVER 256
struct _SERVER_INFO
{
	_SERVER_DATA*	m_pServer ;	
	uint			m_ServerCount ;
	INT				m_HashServer[OVER_MAX_SERVER] ;
	_SERVER_WORLD	m_World ;

	_SERVER_INFO( )
	{
		m_pServer = NULL ;
		m_ServerCount = 0 ;
		for( INT i=0; i<OVER_MAX_SERVER; i++ )
		{
			m_HashServer[i] = -1 ;
		}
	}
	~_SERVER_INFO( )
	{
		SAFE_DELETE_ARRAY( m_pServer ) ;
		m_ServerCount = 0 ; 
	}
};


//Scene
struct _SCENE_DATA
{
	SceneID_t		m_SceneID ;					//����ID
	INT				m_IsActive ;				//�ǲ��ǿ���ʹ�õĳ���
	CHAR			m_szName[_MAX_PATH] ;		//��������
	CHAR			m_szFileName[_MAX_PATH] ;	//������Դ�ļ����ļ���
	ID_t			m_ServerID ;				//���д˳�����Server�˳���ID
	ID_t			m_Type ;					//��ǰ��������ĳ������
												//���ֵΪINVALID_ID�����ڸ���
	ID_t			m_ThreadIndex ;				//�����̵߳�����

	_SCENE_DATA()
	{
		Init( ) ;
	};
	VOID Init( )
	{
		m_SceneID = INVALID_ID ;
		m_IsActive = 1 ;
		memset( m_szName, 0, _MAX_PATH ) ;
		memset( m_szFileName, 0, _MAX_PATH ) ;
		m_ServerID = INVALID_ID ;
		m_Type = INVALID_ID ;
		m_ThreadIndex = INVALID_INDEX ;
		m_ClientRes = INVALID_INDEX ;
	}

	VOID _SetClientRes( ID_t ClientRes )
	{
		m_ClientRes = ClientRes ;
	}
	ID_t _GetClientRes()
	{
		return m_ClientRes ;
	}

private:
	ID_t			m_ClientRes ;				//�ͻ�����Դ����

};

struct _SCENE_INFO
{
	_SCENE_DATA*	m_pScene ;
	uint			m_SceneCount ;
	INT				m_HashScene[MAX_SCENE] ;

	_SCENE_INFO(){
		m_pScene = NULL ;
		m_SceneCount = 0 ;
		for( INT i=0; i<MAX_SCENE; i++ )
		{
			m_HashScene[i] = -1 ;
		}
	};
	~_SCENE_INFO(){
		SAFE_DELETE_ARRAY( m_pScene ) ;
		m_SceneCount = 0 ;
	};
};



class Config
{
public :
	Config( ) ;
	~Config( ) ;

	BOOL					Init( ) ;
	VOID					ReLoad( ) ;


	VOID					LoadConfigInfo( ) ;
	VOID					LoadConfigInfo_Only( ) ;
	VOID					LoadConfigInfo_ReLoad( ) ;

	VOID					LoadLoginInfo( );
	VOID					LoadLoginInfo_Only( );
	VOID					LoadLoginInfo_Reload( );

	VOID					LoadWorldInfo( ) ;
	VOID					LoadWorldInfo_Only( ) ;
	VOID					LoadWorldInfo_ReLoad( ) ;

	VOID					LoadBillingInfo( ) ;
	VOID					LoadBillingInfo_Only( ) ;
	VOID					LoadBillingInfo_ReLoad( ) ;

	VOID					LoadShareMemInfo();
	VOID					LoadShareMemInfo_Only();
	VOID					LoadShareMemInfo_ReLoad();
	
	VOID					LoadMachineInfo( ) ;
	VOID					LoadMachineInfo_Only( ) ;
	VOID					LoadMachineInfo_ReLoad( ) ;
	
	VOID					LoadServerInfo( ) ;
	VOID					LoadServerInfo_Only( ) ;
	VOID					LoadServerInfo_ReLoad( ) ;
	
	VOID					LoadSceneInfo( ) ;
	VOID					LoadSceneInfo_Only( ) ;
	VOID					LoadSceneInfo_ReLoad( );

	ID_t					SceneID2ServerID(SceneID_t sID) const;

	ID_t					Key2ServerID(SM_KEY key)	const;

	VOID					LoadCopySceneInfo( ) ;
	VOID					LoadCopySceneInfo_Only( ) ;
	VOID					LoadCopySceneInfo_ReLoad( ) ;



public :
	_CONFIG_INFO			m_ConfigInfo ;
	_LOGIN_INFO				m_LoginInfo;
	_WORLD_INFO				m_WorldInfo ;
	_MACHINE_INFO			m_MachineInfo ;
	_SERVER_INFO			m_ServerInfo ;
	_SCENE_INFO				m_SceneInfo ;
	_SHAREMEM_INFO			m_ShareMemInfo;
	_BILLING_INFO			m_BillingInfo;

};

//���������������IP�� ������Ϸ������ȷ���û���������ͨ�����Ż��ǽ�������
struct _INTERNAL_IP_OF_PROXY
{
	enum
	{
		MAX_NUM_OF_PROXY_FOR_ONE_NETWORK = 2
	};
	//ProxyForEduUser[2][IP_SIZE]�е�2����Ϊ��������ͨ�͵�����������Ϸ��������
	//��˽�����������ͨ��һ̨������������ڵ���Ҳ����һ��Ϊ�������ṩ����Ĵ��������
	//��Ϸ�����������ҵ�IP��ַ, �����ҵ�IP��ַ�������������������֮һ��
	//���ɶ϶������������Խ�������������֪ͨ���ת�Ƴ���ʱ���Ϳ���֪ͨ�����ӵ��µĴ����ַ�Ͷ˿ڡ�
	//��������ڽ�������Ҳ�ܷ������Ļ������2��Ӧ�����ӵ�3
	CHAR m_szProxyForCncUser[MAX_NUM_OF_PROXY_FOR_ONE_NETWORK][IP_SIZE];
	CHAR m_szProxyForCtcUser[MAX_NUM_OF_PROXY_FOR_ONE_NETWORK][IP_SIZE];
	CHAR m_szProxyForEduUser[MAX_NUM_OF_PROXY_FOR_ONE_NETWORK][IP_SIZE];

	_INTERNAL_IP_OF_PROXY(VOID)
	{
		CleanUp();
	}
	VOID CleanUp(VOID)
	{
		memset(m_szProxyForCncUser, '\0', sizeof(m_szProxyForCncUser));
		memset(m_szProxyForCtcUser, '\0', sizeof(m_szProxyForCtcUser));
		memset(m_szProxyForEduUser, '\0', sizeof(m_szProxyForEduUser));
	}
	ENUM_ISP WhereThisIpFrom(CHAR const* szIP)
	{
		for(INT nIdx =0; MAX_NUM_OF_PROXY_FOR_ONE_NETWORK>nIdx; ++nIdx)
		{
			if(0==strncmp(szIP, m_szProxyForCncUser[nIdx], IP_SIZE))
			{
				return ISP_CHINA_NET_COM;
			}
			else if(0==strncmp(szIP, m_szProxyForCtcUser[nIdx], IP_SIZE))
			{
				return ISP_CHINA_TELE_COM;
			}
			else if(0==strncmp(szIP, m_szProxyForEduUser[nIdx], IP_SIZE))
			{
				return ISP_CHINA_EDU;
			}
		}
		return INVALID_ISP;
	}
};

extern Config g_Config ;

#endif
