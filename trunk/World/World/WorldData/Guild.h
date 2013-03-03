/////////////////////////////////////////////////////////////////////////////////
//�ļ�����Guild.h
//����������������ݺ��߼�����
//�޸ļ�¼��2005-11-28 ���� 
//			2005-12-06 ������ṹ�͹��ܣ�
//          2008-03-04 ����������޸İ��ṹ modify by - amos
/////////////////////////////////////////////////////////////////////////////////


#ifndef __GUILD_H__
#define __GUILD_H__

#include "Type.h"
#include "GameDefine_Guild.h"
#include "DB_Struct.h"

#define MAX_USER_IN_GUILD			160
#define USER_ARRAY_SIZE				MAX_USER_IN_GUILD + MAX_PROPOSER_IN_GUILD

//����г�Ա����Ϣ
struct GUILDUSER
{
	GUID_t				m_UserGUID;							//���GUID
	CHAR				m_szUserName[MAX_CHARACTER_NAME];	//�������
	GUILD_POSITION		m_Position;							//ְ��
	UINT				m_uAccess;							//Ȩ��
	BOOL				m_bOnlineFlag;						//�Ƿ����ߣ���������Ҫ�洢
	UINT				m_uLastLoginTime;					//�������ʱ��
	UINT				m_uLevel;							//���Ｖ��
	BYTE				m_bMenPaiID;						//����
	INT					m_iCurContribute;					//����
	INT					m_iMaxContribute;					//�����
	INT					m_iJoinTime;						//����ʱ��
	INT     			m_iFamilyID;						//����������
	BYTE     			m_cSex;     						//����Ա�
	CHAR				m_szTitleName[MAX_CHARACTER_TITLE];	//���ƺ�����
	INT 				m_nGP;	                            //��ṱ�׶�

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
    //�����Ա ֻ��¼m_aGuildUser�±�
    INT                 m_aFamilyUser[GUILD_FAMILY_SIZE][USER_FAMILY_SIZE]; 
    //��������
    CHAR                m_aFamilyName[GUILD_FAMILY_SIZE][MAX_GUILD_FAMILY_NAME_SIZE];
    //������ּ
    CHAR                m_aFamilyDesc[GUILD_FAMILY_SIZE][MAX_GUILD_FAMILY_DESC_SIZE];
    //���峤������
    CHAR                m_szShaikhName[GUILD_FAMILY_SIZE][MAX_CHARACTER_NAME];

    enum ORESULT
    {
        RET_SUCCESS = 0,
        RET_POS_FULL,	//�˶�����
        RET_NO_EXIST,	//������

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

    INT Add (INT iFamilyId, INT iIndex, INT nMax = USER_FAMILY_SIZE)   //����ID�������m_aGuildUser���±�
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
	GuildID_t			m_GuildID;							                //���ID
	CHAR				m_GuildName[MAX_GUILD_NAME_SIZE];	                //�������
	GUILD_STATUS		m_Status;							                //����״̬
	GUID_t				m_ChieftainGUID;					                //���� GUID
	INT					m_nProposerCount;					                //��������
	INT					m_UserCount;						                //����û�����
	INT					m_MaxUserSize;						                //����˿�����
	INT					m_GuildPoint;						                //��ṱ�׵�
	INT					m_GuildMoney;						                //����ʽ�
	INT					m_nTime;							                //����ʱ��򴴽�ʣ��ʱ��
	GUILDUSER			m_aGuildUser[USER_ARRAY_SIZE];		                //����Ա
    FAMILYUSER          m_FamilyUser;                                       //������Ϣ

	_GUILD_INIT( )
	{
		m_GuildID = INVALID_ID;
		memset( m_GuildName, 0, sizeof(CHAR)*MAX_GUILD_NAME_SIZE );
		m_Status = GUILD_STATUS_INVALID;
		m_nProposerCount = 0; // ��������
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
		PACKET_RETURN	= 1, // ���ز���
	};

	//��������
	enum OPT_TYPE
	{
		OPT_NONE	= 0,	            // ���ز���
		OPT_ASK_MEMBER_LIST,			//����ǰ���а��ɳ�Ա�б�
		OPT_ASK_DETAIL_INFO,			//������ɵ���ϸ��Ϣ
		OPT_ASK_APPOINT_POS_INFO,		//������ɵĿ�������Ϣ
		OPT_ASK_SELF_INFO,				//�����˰�����Ϣ
	};

	//-------------------------------------------------------------------------------------------------
	//��ʼ��
	//-------------------------------------------------------------------------------------------------
	BOOL				Init( _GUILD_INIT* pInit );
	BOOL				InitFromShareMem();
	VOID				ValidateShareMem();
	VOID				CleanUp( );

	//-------------------------------------------------------------------------------------------------
	//��������
	//-------------------------------------------------------------------------------------------------
	BOOL				HeartBeat( UINT uTime=0 );		//�߼������ӿ�
	BOOL				IsMemberFull( ) { return (m_UserCount >= m_MaxUserSize); }// �Ƿ����Ѿ���Ա
	BOOL				IsProposerFull( ) { return (m_nProposerCount >= MAX_PROPOSER_IN_GUILD); }// �Ƿ��Ѿ���̫���������
	BOOL				IsAuthorized( GUID_t userGUID, GUILD_AUTHORITY authority );	// �ж��Ƿ���ĳ��Ȩ��

	//-------------------------------------------------------------------------------------------------
	// �����¼��ӿ�
	//-------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------
	// ������ᣬ��������ʽ��ᣬFALSE ��ʾ���������������������
	BOOL				OnGuildCreate( const USER* pCreater, const CHAR* szName );
	VOID				OnGuildCreateFailed();
	VOID				OnGuildFound();
	BOOL				OnGuildDestroy( GUID_t createrGUID );

	//-------------------------------------------------------------------------------------------------
	// ����ְ��Ȩ�ޱ����Ȩ���ж�
	GUILD_ADMIN_t::ORESULT	OnUserPositionChange( GUID_t userGUID, GUILD_POSITION position );//����ְ���ԭ��,һ��������ְΪ��ͨ����,����ְΪĳ����Ա
	
	VOID				OnUserAuthorityChange( GUID_t userGUID, GUILD_AUTHORITY authority );
	VOID				OnPositionAuthorityChange( GUILD_POSITION position, GUILD_AUTHORITY authority );
	VOID				OnUserAccessChange( GUID_t userGUID, INT newAccess ); //���Ȩ�ޱ仯
	VOID				OnGuildPointChange( GUID_t userGUID, INT nMoney ); //��ṱ�׵㷢���仯
	BOOL				CanDoIt(OPT_TYPE Opt, GUILD_POSITION position, GUILD_AUTHORITY authority);//����ְλ��Ȩ�޲����ж�����
	BOOL				CanAppointedBy(GUILD_POSITION position, const GUILDUSER* pUser);//�Ƿ��ܱ���������
	CHAR*				GetPosName(INT iPosition);//��ô˰���е�ǰְλ������

	//-------------------------------------------------------------------------------------------------
	// ������������Ӧһ�����
	GUILD_ERROR_TYPE	OnUserEnter( const USER* pUser );
	GUILD_ERROR_TYPE	OnRecruitUser( GUID_t userGUID );// ���հ���
	GUILD_ERROR_TYPE	OnUserLeave( GUID_t userGUID );// ���������뿪���
	GUILD_ERROR_TYPE	OnUserLeaveByIndex( INT iIndex );// ���������뿪���
	GUILD_ERROR_TYPE	OnDemise();// ���ã�������ְ��ͬ���ǣ�������Ҫ������ְ����˼���ȥ
	VOID				OnGuildMoneyChange( GUID_t userGUID, INT nMoney ); //����ʽ����仯// ֧ȡ��������

	//-------------------------------------------------------------------------------------------------
    // ���������
	GUILD_ERROR_TYPE	OnUserInvite( const USER* pUser );

    // ��������
    INT                 OnCreateFamily(const GUID_t CreatorGuid,const INT iShaikhIndex,
                            const GUID_t ShaikhGuid, const CHAR* szFamilyName, const INT iFamilyNameSize,
                            const CHAR* szFamilyDesc,  const INT iFamilyDescSize, CHAR &cFamilyID );

    // ��Ӽ����Ա
    INT                 OnAddFamilyUser(const GUID_t InvitorGuid, CHAR cFamilyIndex, INT iGuildIndex, GUID_t guid);

    // ɾ�������Ա
    INT                 OnRemoveFamilyUser(const GUID_t BossGuid, CHAR cFamilyIndex, INT iGuildIndex, GUID_t guid);

    // ��ɢ����
    INT                 OnFamilyDismiss(CHAR cFamilyIndex );

    // �޸ļ�������

    // �޸ļ�����ּ

    // ��ȡ�����Ա��Ϣ�б�,���ؼ����Ա����
    INT                 GetFamilyUser(CHAR cFamilyIndex, INT* pFamilyUser);

    // ��ȡ��������
    const CHAR *        GetFamilyName(CHAR cFamilyIndex);

    // ��ȡ������ּ
    const CHAR *        GetFamilyDesc(CHAR cFamilyIndex);

    // ��ȡ���峤����
    const CHAR *        GetShaikhName(CHAR cFamilyIndex);

    // ȡ�ü�������
    UINT                GetFamilyCount();

    // ���ü�����
    VOID                SetFamilyID(INT iGuildIndex, INT iID);

    // ȡ�ü�����
    INT                 GetFamilyID(GUID_t userGUID);

    // ���ð��ڹ��׶�   
    VOID                SetGuildPoint(GUID_t userGUID, INT nGP);

    // ȡ�ð��ڹ��׶�   
    INT                 GetGuildPoint(GUID_t userGUID);

    // ���ð���ܹ��׶�   
    VOID                SetTotalGuildPoint(INT nGP);

    // ȡ�ð���ܹ��׶�   
    INT                 GetTotalGuildPoint();

	//-------------------------------------------------------------------------------------------------
	// ������������
	VOID				OnUserLogin( GUID_t userGUID );
	VOID				OnUserLogoff( GUID_t userGUID );

	//-------------------------------------------------------------------------------------------------
	// �㲥���� position ����ְ��İ��ڹ㲥����������Ϣ
	VOID				BroadCast_Return( Packet* pPacket, GUILD_POSITION positon = GUILD_POSITION_MEMBER );

	//-------------------------------------------------------------------------------------------------
	//������
	BOOL				IsLock(){return m_IsLock;}
	VOID				SetLock(BOOL bIsLock){m_IsLock	=	bIsLock;}

	//-------------------------------------------------------------------------------------------------
	//���ݽӿ�
	//-------------------------------------------------------------------------------------------------

	//�˰���ID
	GuildID_t			GetGuildID( ){ return m_GuildID; }

	// �������
	const CHAR*			GetGuildName( ) { return m_GuildName; }

	// �����ּ
	const CHAR*			GetGuildDesc( ) { return m_GuildDesc; }

	// �����ּ
	VOID				SetGuildDesc(CHAR*	pszGuildDesc)
	{
		memset(m_GuildDesc, 0, MAX_GUILD_DESC_SIZE);
		memcpy(m_GuildDesc, pszGuildDesc, MAX_GUILD_DESC_SIZE); 
	}

	BYTE				GetGuildLevel(){ return m_GuildLevel;}
	VOID				SetGuildLevel(BYTE GuildLevel){m_GuildLevel = GuildLevel;}

	//������
	CHAR*				GetGuildChiefName(){return m_GuildChiefName;}

	//��������
	CHAR*				GetCreatorName( ) { return m_GuildCreatorName; }

	//���ɵ����
	City*				GetCity(){ return m_pCity;}

	//����ʱ��
	INT					GetFoundedTime(){return m_nTime;}

	// ȡ�ð���
	const GUILDUSER*	GetGuildUser( GUID_t userGUID );

    const GUILDUSER*	GetGuildUser( GUID_t userGUID, INT &iIndex);

	// ͨ��������ð���
	GUILDUSER*	GetGuildUserByIndex( INT nIndex);

	// ȡ�ð�������
	INT					GetGuildUserCount( ) { return m_UserCount; }

	// ȡ��Ԥ����������
	INT					GetGuildProposerCount( ) { return m_nProposerCount; }

	// ȡ�ð�������
	INT					GetGuildUserMax( ) { return m_MaxUserSize; }

	// ���״̬
	GUILD_STATUS		GetGuildStatus( ) { return m_Status; }

	// ���� GUID
	GUID_t				GetChieftainGUID( ) { return m_ChieftainGUID; }

	//-------------------------------------------------------------------------------------------------
	//�������Խӿ�
	
	//����
	INT					GetLongevity(){ return m_Longevity;}
	VOID				SetLongevity(INT Longevity){ m_Longevity = Longevity;}

	//���׶�
	INT					GetContribute(){ return m_Contribute;}
	VOID				SetContribute(INT Contribute){ m_Contribute = Contribute;}

	//����
	INT					GetHonor(){ return m_Honor;}
	VOID				SetHonor(INT Honor){ m_Honor = Honor;}

	//����ʽ�
	INT					GetMoney(){ return m_GuildMoney;}
	VOID				SetMoney(INT GuildMoney){ m_GuildMoney = GuildMoney;}

    VOID            	SetGuildUserTitleName( GUID_t userGUID, const CHAR *pszTitleName);

    //���úͰ��ȼ��ҹ����������ֵ
    VOID                SetGuildMaxByLevel(INT nMaxUser, INT nMaxFamily, INT nMaxGoldFamilyUser);
    INT                 GetMaxUser() { return m_MaxUser; }
    INT                 GetMaxFamily() { return m_MaxFamily; }
    INT                 GetMaxGoldFamilyUser() { return m_MaxGoldFamilyUser; }

protected :
	//-------------------------------------------------------------------------------------------------
	//����ڲ��߼������ܽӿ�
	VOID				AddGuildUser( GUILDUSER& GuildUser );

	
protected :
	inline GUILDUSER*	ILGetGuildUser( GUID_t userGUID );
	inline INT			ILGetGuildUserIndex( GUID_t userGUID );

	GuildID_t			m_GuildID; //���ID
	CHAR				m_GuildName[MAX_GUILD_NAME_SIZE];		    //�������
	CHAR				m_GuildDesc[MAX_GUILD_DESC_SIZE];		    //�������
    CHAR                m_FamilyDesc[MAX_GUILD_FAMILY_DESC_SIZE];   //������ּ
	CHAR				m_GuildChiefName[MAX_CHARACTER_NAME];	    //������
	CHAR				m_GuildCreatorName[MAX_CHARACTER_NAME];	    //��������
	BYTE				m_GuildLevel;
	GUILD_STATUS		m_Status;								    //����״̬
	GUID_t				m_ChieftainGUID;						    //������ GUID
	INT					m_nProposerCount;						    //��������
	INT					m_UserCount;							    //����û�����
	INT					m_MaxUserSize;							    //����˿�����
	INT					m_GuildPoint;							    //��ṱ�׵�
    UINT    			m_FamilyCount;							    //��������
	GUILDUSER			m_aGuildUser[USER_ARRAY_SIZE];			    //����Ա
    FAMILYUSER          m_FamilyUser;                               //������Ϣ
	BOOL				m_IsLock;								    //��ǰ�����Ƿ���ס��ĳЩ������ֻ��ͬʱ����һ�����紴������
	CityID_t			m_CityID;								    //�󶨵ĳ���ID
	City*				m_pCity;								    //�󶨵ĳ���ָ��
    INT                 m_TotalGuildPoint;                          //����ܹ��׶�

    INT                 m_MaxUser;                                  //�����������          �ͼ���ҹ�
    INT                 m_MaxFamily;                                //��������              �ͼ���ҹ�
    INT                 m_MaxGoldFamilyUser;                        //�ƽ������������      �ͼ���ҹ�

	// GUILD_STATUS_UNCOMMITTED ʱ����ʾʣ�������
	// GUILD_STATUS_NORMAL ʱ����ʾ������ʱ��
	INT					m_nTime;
	GuildSMU*			m_pGuildSmu;
protected:
	//-------------------------------------------------------------------------------------------------
	//��������
	INT					m_Longevity;				//����
	INT					m_Contribute;				//���׶�
	INT					m_Honor;					//����
	INT					m_GuildMoney;				//����ʽ�

	//-------------------------------------------------------------------------------------------------
	//����ṹ
	GUILD_ADMIN_t		m_AdminLayOut;							


private :

	// ���ڴ洢����������ʱ���
	UINT				m_uTimeStamp;

	// �������
	INT					m_nUserMallocIndex;
};


#endif
