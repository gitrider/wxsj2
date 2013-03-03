

#ifndef __HUMANDB_H__
#define __HUMANDB_H__

#include "Type.h"
#include "DB_Struct.h"
#include "TimeManager.h"
#include "GameTable.h"

#define			GetDBStr(db)			(db)->GetDBAttrLvl1(CATTR_LEVEL1_STR)
#define			GetDBCon(db)			(db)->GetDBAttrLvl1(CATTR_LEVEL1_CON)
#define			GetDBInt(db)			(db)->GetDBAttrLvl1(CATTR_LEVEL1_INT)
#define			GetDBDex(db)			(db)->GetDBAttrLvl1(CATTR_LEVEL1_DEX)

#define			SetDBStr(db,v)			(db)->SetDBAttrLvl1(CATTR_LEVEL1_STR,v)
#define			SetDBCon(db,v)			(db)->SetDBAttrLvl1(CATTR_LEVEL1_CON,v)
#define			SetDBInt(db,v)			(db)->SetDBAttrLvl1(CATTR_LEVEL1_INT,v)
#define			SetDBDex(db,v)			(db)->SetDBAttrLvl1(CATTR_LEVEL1_DEX,v)



class HumanDB
{

		
#define			N_SM_LOCK(type)						if(m_HumanSM){SM_LOCK(m_HumanSM->m_SMUHead.flag,type)};
#define			N_SM_UNLOCK(type)					if(m_HumanSM){SM_UNLOCK(m_HumanSM->m_SMUHead.flag,type)};

public :
	HumanDB( ) ;
	~HumanDB( ) ;

	VOID					CleanUp( ) ;
	VOID					ValidateShareMem(BOOL bForceAll,BOOL& bUpdateAttr);
	BOOL					IsNoDirectAttr(CHAR_ATTR_DB attrType);
	VOID					ValidatePlayerID(PlayerID_t	PID);
	inline VOID				Lock();
	inline VOID				UnLock();

	/*=========================================
	 *	�������Է��ʷ���
	 ===========================================*/
	inline	GUID_t				GetGUID()	const;
	inline	BOOL				IsFirstEnterScene();
	inline	VOID				ClearFirstEnterScene();
	inline	VOID				SetGUID( GUID_t guid );
	inline	INT					GetPortraitID() const;
	inline	VOID				SetPortraitID( INT nID );
	inline	const	CHAR*		GetName()const;
	inline	BYTE				GetSex();
	inline	INT					GetHP();
	inline	VOID				SetHP( INT hp );
	inline	INT					GetMP();
	inline	VOID				SetMP( INT mp );
	inline	INT					GetPneuma();
	inline	VOID				SetPneuma( INT pneuma );
	inline	INT					GetStrikePoint(VOID);
	inline	VOID				SetStrikePoint(INT nStrikePoint);
	inline	INT					GetVigor(VOID);
	inline	VOID				SetVigor(INT nVigor);
	inline	INT					GetMaxVigor(VOID);
	inline	VOID				SetMaxVigor(INT nMaxVigor);
	inline	INT					GetVigorRegeneRate(VOID);
	inline	VOID				SetVigorRegeneRate(INT nRate);
	inline	INT					GetEnergy(VOID);
	inline	VOID				SetEnergy(INT nEnergy);
	inline	INT					GetMaxEnergy(VOID);
	inline	VOID				SetMaxEnergy(INT nMaxEnergy);
	inline	INT					GetEnergyRegeneRate(VOID);
	inline	VOID				SetEnergyRegeneRate(INT nRate);
	inline	INT 				GetRage( VOID );
	inline	VOID				SetRage( INT nRage );
	inline	BYTE				GetLevel();
	inline	VOID				SetLevel( BYTE level );
	inline	INT					GetPKValue( VOID )const;
	inline	VOID				SetPKValue( INT nValue );
	inline	PET_GUID_t			GetCurrentPetGUID( VOID )const;
	inline	VOID				SetCurrentPetGUID( PET_GUID_t guid );
	inline	UINT				GetExp();
	inline	VOID				SetExp( UINT exp );

	inline	INT					GetCountry( ); //����
	inline	VOID				SetCountry( INT nCountry );
	inline	INT					GetMenPai( );
	inline	VOID				SetMenPai( INT MenPai );
	inline	UINT				GetMoney();
	inline	VOID				SetMoney(UINT money);
    inline	UINT				GetGold();
    inline	VOID				SetGold(UINT gold);
    inline	UINT				GetTicket();
    inline	VOID				SetTicket(UINT ticket);

	inline	INT					GetGoodBadValue();
	inline	INT					SetGoodBadValue(INT nValue);
	inline	_PLAYERSHOP_GUID	GetShopGuid(INT nIndex);									
	inline	VOID				SetShopGuid( INT nIndex, _PLAYERSHOP_GUID pGuid );
	inline	_PLAYERSHOP_GUID	GetFavoriteShopGuid(INT nIndex);
	inline	VOID				SetFavoriteShopGuid( INT nIndex, _PLAYERSHOP_GUID pGuid );
	inline	const CHAR*			GetPasswd();
	inline	VOID				SetPasswd( const CHAR* szPasswd );
	inline	VOID				DelPasswd();
	inline	UINT				GetPasswdDeleteTime();
	inline	VOID				SetPasswdDeleteTime( UINT uTime );
	inline	BYTE				GetXinfaLevel(INT nXinfaID);
	inline	VOID				SetXinfaUplevel(INT nXinfaID, BYTE UplevelNum);
	inline	UINT				GetHairColor(VOID)const;
	inline	VOID				SetHairColor(UINT uColor);
	inline	BYTE				GetFaceColor(VOID)const;
	inline	VOID				SetFaceColor(BYTE byColor);
	inline	BYTE				GetHairModel(VOID)const;
	inline	VOID				SetHairModel(BYTE byModel);
	inline	BYTE				GetFaceModel(VOID)const;
	inline	VOID				SetFaceModel(BYTE byModel);

	inline	UINT				GetEquipID(HUMAN_EQUIP	equipPoint);
	inline	UINT				GetEquipLevel(HUMAN_EQUIP	equipPoint);
	inline	INT					GetDBAttrLvl1(CHAR_ATTR_LEVEL1 enAttrType);					//һ������GET
	inline	VOID				SetDBAttrLvl1(CHAR_ATTR_LEVEL1 enAttrType,INT iValue);		//һ������SET

	inline	INT					GetNeiShang();												//����

	inline 	const _CAMP_DATA	*GetDBCampData(VOID)const;	//ȡ��Ӫ
	inline	VOID				SetDBCampData(const _CAMP_DATA *pCampData);//����Ӫ
	inline	VOID				SetDBStartScene(SceneID_t	sceneID);
	inline	SceneID_t			GetDBBakScene();
	inline	VOID				SetDBBakScene(SceneID_t	sceneID);
	inline	VOID				SetDBPosition(const WORLD_POS& pos);						//position ��ʵʱд���ݿ�
	inline	VOID				SetDBBakPosition(const WORLD_POS& pos);
	inline  const WORLD_POS*	GetDBBakPosition();
	inline	VOID				WriteDBPosition(WORLD_POS& pos);							//���logout ʱ���õĺ���
	
	inline	VOID				SetOnlineTime(UINT uTime);	    //����������ʱ��
	inline	UINT				GetOnlineTime();			    //ȡ��������ʱ��	
	inline  VOID				SetLoginTime(UINT uTime);	    //��������¼ʱ��
	inline  UINT				GetLoginTime();				    //ȡ������¼ʱ��
	inline  VOID				SetLogoutTime(UINT uTime);	    //�����������ʱ��
	inline  UINT				GetLogoutTime();			    //ȡ���������ʱ��
    inline  VOID				SetLeaveGuildTime(time_t uTime);//�����뿪����ʱ��
    inline  time_t				GetLeaveGuildTime();		    //ȡ���뿪����ʱ��
	inline  VOID				SetDBVersion(UINT Ver);		    //�������ݿ�汾
	inline  UINT				GetDBVersion();				    //ȡ�����ݿ�汾



	inline	VOID				SetDBGuid(GUID_t	guid);
	inline	GuildID_t			GetDBGuildID( ) ;
	inline	VOID				SetDBGuildID( GuildID_t guildID ) ;
    inline	INT                 GetDBFamilyID( ) ;
    inline	VOID				SetDBFamilyID( INT nFamilyID ) ;
    inline	INT                 GetDBGuildOfficial( ) ;
    inline	VOID				SetDBGuildOfficial( INT nOfficial ) ;
    inline	INT                 GetDBGuildPoint( ) ;
    inline	VOID				SetDBGuildPoint( INT nGP ) ;
    inline	INT                 GetDBHonor( ) ;
    inline	VOID				SetDBHonor( INT nHonor ) ;


	inline	TeamID_t			GetDBTeamID( ) ;
	inline	VOID				SetDBTeamID( TeamID_t teamID ) ;
	
	inline	INT                 GetMountID( ) ;                // [7/8/2010 �¾���]
	inline	VOID				SetMountID( INT nID ) ;        // [7/8/2010 �¾���]
	inline	INT                 GetMountIndex( ) ;             // [7/8/2010 �¾���]
	inline	VOID				SetMountIndex( INT nIndex ) ;  // [7/8/2010 �¾���]

	//////////////////////////////////////////////////////////////////////////
	//���гƺ����ݷ��ʷ���		
	//////////////////////////////////////////////////////////////////////////
    inline    const _TITLE*     GetTitleInfo() const;
    inline    VOID				SetCurCountryTitle( INT iTitleID );                              //���õ�ǰ���ҳƺţ������ǰ�ƺŲ����ڣ����Զ����
    inline    VOID              SetCurGuildTitle(INT iTitleID );                                 //���õ�ǰ���ƺţ������ǰ�ƺŲ����ڣ����Զ����
    inline    VOID              SetCurNormalTitle(INT iTitleID );                                //���õ�ǰ�淨�ƺţ������ǰ�ƺŲ����ڣ����Զ����
    inline    VOID				SetCurCountryTitle( INT iTitleID, INT &BuffID1, INT &BuffID2  ); //���õ�ǰ���ҳƺţ������ǰ�ƺŲ����ڣ����Զ����
    inline    VOID              SetCurGuildTitle(INT iTitleID, INT &BuffID1, INT &BuffID2 );     //���õ�ǰ���ƺţ������ǰ�ƺŲ����ڣ����Զ����
    inline    VOID              SetCurNormalTitle(INT iTitleID, INT &BuffID1, INT &BuffID2 );    //���õ�ǰ�淨�ƺţ������ǰ�ƺŲ����ڣ����Զ����
    inline    INT				GetCurCountryTitle();
    inline    INT               GetCurGuildTitle();
    inline    INT               GetCurNormalTitle();

    inline    VOID              AddTitle(INT iTitleID, INT &BuffID1, INT &BuffID2 );
    inline    INT               RemoveTitle(INT iTitleID);
    inline    VOID              SetGuildTitleName(const CHAR *pszTitle, INT iSize);
    inline    const CHAR*       GetGuildTitleName() const;
    inline    VOID              SetCountryTitleName(const CHAR *pszTitle, INT iSize);
    inline    const CHAR*       GetCountryTitleName() const;
    inline    VOID              SetNormalTitleName(const CHAR *pszTitle, INT iSize);          //�����淨�ƺ�
    inline    const CHAR*       GetNormalTitleName() const;
    inline    VOID              SetOfficalTitleName(const CHAR *pszTitle, INT iSize);         //��������Զ���ƺ�
    inline    const CHAR*       GetOfficalTitleName() const;


    inline    const CHAR*       GetTitleNameByID( INT iTitleID, INT iSex  ) ;
    inline    BOOL              UpdateTitleTime();
    
	//////////////////////////////////////////////////////////////////////////
	//�������ݷ��ʷ���		
	//////////////////////////////////////////////////////////////////////////
	inline	const INT			GetBankItemCount( ) const ;
	inline	VOID				SetBankItemCount( const INT Bankitemcount ) ;
	inline	const _ITEM*		GetBankItem( const INT bankindex ) const ;
	inline	VOID				SetBankItem( const INT bankindex, const _ITEM* item ) ;
	inline	const INT			GetBankEndIndex( ) const ;
	inline	VOID				SetBankEndIndex( const INT bankindex ) ;
	inline	const INT			GetBankMoney( ) const ;
	inline	VOID				SetBankMoney( const INT money ) ;
	inline  VOID				EraseBankItem(UINT uBankIndex);
    inline	const INT			GetBankSize( ) const ;
	inline  VOID				ExpandBankSize(UINT uBanksize);

	inline	VOID				SwapBankWithBank(UINT iBankIndexFr, UINT iBankIndexTo );
	inline	VOID				SwapBankWithBag(UINT iBagIndex, UINT iBankIndex );
	inline	VOID				SwapBankWithEquip( HUMAN_EQUIP	equipPoint, UINT iBankIndex ) ;
	inline	VOID				OverWriteBag2Bank(UINT iBagIndex, UINT iBankIndex);
	inline	VOID				OverWriteEquip2Bank(HUMAN_EQUIP	equipPoint, UINT iBankIndex);
	inline	VOID				OverWriteBank2Bag(UINT iBagIndex, UINT iBankIndex);
	inline	VOID				OverWriteBank2Equip(HUMAN_EQUIP	equipPoint, UINT iBankIndex);

	VOID						SetFreeOwnFlag(INT Flag);
	INT							GetFreeOwnFlag();

	
	//////////////////////////////////////////////////////////////////////////
	//�������ݷ��ʷ���		
	//////////////////////////////////////////////////////////////////////////
	inline	VOID				EraseBagItem(uint BagIndex);
	inline	VOID				SwapEquipBag(HUMAN_EQUIP	equipPoint,uint iBagIndex);
	inline	VOID				OverWriteBag2Equip(uint iBagIndex,HUMAN_EQUIP	equipPoint);
	inline	VOID				OverWriteEquip2Bag(HUMAN_EQUIP	equipPoint,uint iBagIndex);
	inline	VOID				PutBagDB(const _ITEM* pItem,uint iBagIndex);
	inline	VOID				SwapBagDB(uint iAIndex,uint iBIndex);
	inline	BOOL				IncBagItemCount(uint iBagIndex);
	inline	BOOL				DecBagItemCount(uint iBagIndex);
	inline	VOID				SetItemBind(uint iBagIndex,BOOL	bBind);
	inline	const _ITEM*		GetBagItem(uint	iBagIndex)	const ;
	inline	const _ITEM*		GetEquipItem(HUMAN_EQUIP equipPoint);
	inline	VOID				EraseEquipItem(HUMAN_EQUIP equipPoint);
	inline	VOID				SetEquipMark(HUMAN_EQUIP eEquipPoint, BYTE bMark);
	inline	VOID				SetEquipDur(HUMAN_EQUIP eEquipPoint, INT Dur);
	inline	VOID				SetItemDur(uint BagIndex, INT Dur);
	inline	VOID				RandSet( ) ;										//�����ã���������û������Ϣ
	inline	UINT				GetBaseBagSize();
	inline	UINT				GetExtraContainerSize();
	inline	UINT				GetExtraBagSize(UINT nIndex);
	
	inline	UINT				GetTaskBagSize();
	inline	UINT				GetMatBagSize();

	//////////////////////////////////////////////////////////////////////////
	//�������ݷ��ʷ���		   [7/8/2010 �¾���]
	//////////////////////////////////////////////////////////////////////////
	inline const	_ITEM*		GetMountItem(UINT iMountIndex);
	inline	VOID				EraseMountItem(UINT iMountIndex);


	//////////////////////////////////////////////////////////////////////////
	//��ȴʱ�����
	//////////////////////////////////////////////////////////////////////////
	
	inline  VOID				SetCooldown(CooldownID_t nID,Time_t nTime);
	inline	BOOL				IsCooldowned(CooldownID_t nID) const;
	inline	VOID				HeartBeat_Cooldown(Time_t nDeltaTime);
	inline	Time_t				GetCooldownRemain(CooldownID_t nID) const;
	//////////////////////////////////////////////////////////////////////////
	//�������ݷ���
	//////////////////////////////////////////////////////////////////////////
	inline BYTE					GetSkillCount();
	inline VOID					SetSkillCount(BYTE Count);
	inline _OWN_SKILL&			GetSkillID(BYTE Index);
	inline VOID					SetSkillID(const _OWN_SKILL& oSkill,BYTE Index);
	inline VOID					SetSkillLevel(const INT nID, BYTE Index, const INT nLevel);
	inline	INT					GetRemainPoints();	//ʣ��һ�����Ե�
	inline  INT					GetRemainSkillPoints();  //ʣ�༼�ܵ�
	inline	INT					Get_S_SkillPoints(INT nS);	//���ϵ���ܵ�
	inline	VOID				SetDBRemainPoints(INT Point);
	inline  VOID                SetDBRemainSkillPoints(INT Point);
	inline  VOID                SetDB_S_SkillPoints(INT nS, INT Point);
	inline  VOID				SetMonsterPetAttr( INT iIndex, _Monster_Pet_DB_Struct* pMonsterPet );
	inline  VOID				SetHorseMattingFlag( BOOL bFlag );
	//////////////////////////////////////////////////////////////////////////
	//�ķ����ݷ���
	//////////////////////////////////////////////////////////////////////////
	inline BYTE					GetXinFaCount();
	inline VOID					SetXinFaCount(BYTE Count);
	inline SkillID_t			GetXinfaID(BYTE Index);
	inline VOID					SetXinfaID(SkillID_t nID,BYTE Index);
	inline BYTE					GetXinfaLevel(BYTE Index);
	inline VOID					SetXinfaLevel(BYTE Level,BYTE Index);
	//////////////////////////////////////////////////////////////////////////
	//Impact���ݷ���
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	//�������ݷ���
	//////////////////////////////////////////////////////////////////////////
	inline VOID					SetMissionDone(MissionID_t idMission,MissionID_t idIndex);
	inline VOID					SetMissionParam( UINT uIndexMission, UINT uIndexParam, INT nValue );
	inline VOID					SetMissionData( INT nIndex, INT nData );//���õ�ǰ������������
	inline VOID					SetMission(UINT uIndex,MissionID_t idMission,ScriptID_t idScript);
	inline VOID					SetMissionCount(UINT uCount);
	inline VOID					SetFlags_KillObject(UINT uIndex, BOOL bSet );
	inline VOID					SetFlags_EnterArea(UINT uIndex, BOOL bSet );
	inline VOID					SetFlags_ItemChanged(UINT uIndex, BOOL bSet );
	inline VOID					SetFlags_PetChanged(UINT uIndex, BOOL bSet );
	inline VOID					ClearMission(UINT uIndex);
	//////////////////////////////////////////////////////////////////////////
	//��ϵ���ݷ��ʷ���
	//////////////////////////////////////////////////////////////////////////

	inline  VOID				SetRelation(INT Index, const _OWN_RELATION* pRelation);
	inline	VOID				ModifyFriendType( INT nIndex, RELATION_TYPE RelationType );
	inline	VOID				UpdateRelationInfo( INT Index, const RELATION_MEMBER* pMemberInfo );
	inline	VOID				SetMood(const CHAR* szMood);
	inline	VOID				SetName(const INT index,const CHAR* szName);
	inline	VOID				ModifyFriendPoint(INT Index, INT nFriendPoint);
	inline	VOID				AdjustRelationType(INT Index, UCHAR uType);
	inline	VOID				AdjustRelationGroup(INT Index, UCHAR uGroup);
	inline	VOID				DeleteRelation(INT Index);
	inline	VOID				IncRelationCount(INT Index);
	inline	VOID				DecRelationCount(INT Index);
	inline	VOID				SetSpouse(const MarriageInfo* pMarriageInfo);
	inline	VOID				AddPrentice(GUID_t guid);
	inline	VOID				DelPrentice(GUID_t guid);
	inline	VOID				SetMoralPoint(UINT uPoint);
	inline	VOID				SetBetrayTime(time_t BetrayTime);
	inline	VOID				SetMaster(const MasterInfo* pMasterInfo);


	inline	VOID				SetAbilityLevel(AbilityID_t abilityid,INT lvl);
	inline	VOID				SetAbilityExp(AbilityID_t abilityid,INT exp);
	inline  VOID				SetPrescrLearnedFlag( const PrescriptionID_t prescrid, BOOL flag );

	//////////////////////////////////////////////////////////////////////////
	//�������ݷ��ʷ���
	//////////////////////////////////////////////////////////////////////////
	inline const	_PET_DB_LOAD*	GetPet(uint	iIndex)	const ;


	//////////////////////////////////////////////////////////////////////////
	//�û��������ݷ��ʷ���
	//////////////////////////////////////////////////////////////////////////
	inline	VOID				SetSettingData( INT nType, _OWN_SETTING* Value ) ;
	inline const	_OWN_SETTING*	GetSetting( INT nType ) const ;
	//////////////////////////////////////////////////////////////////////////
	//���˽����Ϣ���ʷ���
	//////////////////////////////////////////////////////////////////////////
	inline  VOID				SetPrivateInfo(_PRIVATE_INFO_DB_LOAD* pPrivateInfo);
	inline  VOID				SetSouXiaDBInfo(_SOUXIA_DB_LOAD* pSouXiaInfo, SHORT offSet);
	inline	INT					GetReserve(INT index);
	inline	VOID				SetReserve(INT index,INT value);
	inline  SOUXIA_DATA*		GetSouXiaInfo(SHORT index);

public :
	/*========================================
	 *	��ʼ����ɫ�������ݿ���Ϣ
	 ========================================*/
	BOOL						InitHumanDB( ) ;
	BOOL						InitEquipDB( ) ;
	BOOL						InitBagDB( ) ;
	BOOL						InitBankDB( ) ;
	BOOL						InitEffectDB( ) ;
	BOOL						InitSkillDB( ) ;
	BOOL						InitCooldownDB();
	



private:
	inline _ITEM*				_GetBagItem(UINT uBagIndex);
	inline _ITEM*				_GetBankItem(UINT uBankIndex);
	inline VOID					_RegDBAttr(CHAR_ATTR_DB eOffSet,VOID* pArg,INT iSize);//ע�ᵥ������
	inline VOID					_RegSMAttr(CHAR_ATTR_DB eOffSet,VOID* pArg,INT iSize);//ע��ShareMem����
	VOID						_RegisterDBAttributes();  //ע����������
	VOID						_RegisterSMAttributes();  //ע������ShareMem����
	
public:
	
	_PET_DB_LIST_LOAD*				m_dbPetList;	//����

	//�������Ƿ��ʷ���
	VOID						SetDBEnumAttrDirty(CHAR_ATTR_DB eOffSet,BOOL bFlag = TRUE);
	BOOL						GetDBEnumAttrDirty(CHAR_ATTR_DB eOffSet);
	//��GLServerˢ�����ݵ����Ƿ��ʷ���
	VOID						SetToWorldEnumAttrDirty(CHAR_ATTR_DB eOffSet,BOOL bFlag = TRUE);
	BOOL						GetToWorldEnumAttrDirty(CHAR_ATTR_DB eOffSet);

private:
	
	
	_HUMAN_DB_LOAD*					m_dbHuman ;		//��ɫ������Ϣ
	_BAG_DB_LOAD*					m_dbBag ;		//��ɫ������Ʒ��Ϣ
	_EQUIP_DB_LOAD*					m_dbEquip ;		//��ɫװ����Ϣ
	_BANK_DB_LOAD*					m_dbBank ;		//��ɫ������Ʒ��Ϣ
	_SKILL_DB_LOAD*					m_dbSkill ;		//��ɫ����ӵ�еļ�����Ϣ
	_COOLDOWN_DB_LOAD_FOR_HUMAN*	m_dbCooldown ;	//��ɫ���ϵ���ȴ��Ϣ
	_RELATION_DB_LOAD*				m_dbRelation;	//��ɫ��ϵ�ˣ����ѡ���������
	_ABILITY_DB_LOAD*				m_dbAbility;	//��ɫѧ����������Ϣ�Լ��䷽��
	_XINFA_DB_LOAD*					m_dbXinFa ;		//��ɫѧ����ķ���Ϣ
	_IMPACT_DB_LOAD*				m_dbImpact ;	//��ɫ������ʩ�ӵĸ���Ч����Ϣ
	_MISSION_DB_LOAD*				m_dbMission;	//�����б�
	_SETTING_DB_LOAD*				m_dbSetting ;	//��������
	_PRIVATE_INFO_DB_LOAD*			m_dbPrivateInfo;//˽����Ϣ
	_MONSTER_PET_DB*				m_dbMonsterPet;	//�ٻ��޿��ר�ã���Ҫд���ݿ�
	_SOUXIA_DB_LOAD*				m_dbSouXia;		//����¼��Ϣ add by gh 2010/05/15
    _MOUNT_DB_LOAD*                 m_dbMount;      //��ɫ��������Ϣ  [7/1/2010 �¾���]

private:
	//�����ڴ��������
	HumanSMU*						m_HumanSM;		//�����ڴ�����
	HUMAN_DB_ATTR_FLAG*				m_AttrFlag;		//��ɫ����ˢ�¿�������
	HUMAN_DB_ATTR_FLAG*				m_AttrFlag_ToWorld;		//��ɫ������GLServerˢ�µĿ�������
	HUMAN_DB_ATTR_REG*				m_AttrReg;		//��ɫ����ע������
	HUMAN_DB_ATTR_REG*				m_AttrRegSM;	//��ɫ������ShareMem��ע������

public:
	inline const	_BAG_DB_LOAD*					GetBag();
	inline const	_HUMAN_DB_LOAD*					GetHumanDB()	const;
	inline const	_EQUIP_DB_LOAD*					GetEquipDB();
	inline const	_BANK_DB_LOAD*					GetBankDB();
	inline const	_SKILL_DB_LOAD*					GetSkillDB();
	inline const	_COOLDOWN_DB_LOAD_FOR_HUMAN* 	GetCooldownDB() const;
	inline const	_RELATION_DB_LOAD*				GetRelationDB()	const;
	inline const	_ABILITY_DB_LOAD*				GetAbilityDB()	const;
	inline const	_XINFA_DB_LOAD*					GetXinFaDB()	const;
	inline const	_IMPACT_DB_LOAD*				GetImpactDB()	const;
	inline const	_MISSION_DB_LOAD*				GetMissionDB()  const;
	inline const	_SETTING_DB_LOAD*				GetSettingDB()  const;
	inline const	_PRIVATE_INFO_DB_LOAD*			GetPrivateInfoDB() const;
	inline const	_MONSTER_PET_DB*				GetMonsterPetDB() const;
	inline const	_SOUXIA_DB_LOAD*				GetSouXiaDB()	const;
	inline const	_MOUNT_DB_LOAD*			        GetMountDB()    const;    // [7/8/2010 �¾���]

	//�벻Ҫ��human���߼������е������к���
	inline VOID								WriteHumanDB(const	_HUMAN_DB_LOAD* pHumanDB);
	inline VOID								WriteBagDB(const	_BAG_DB_LOAD* pBagDB);
	inline VOID								WriteEquipDB(const	_EQUIP_DB_LOAD*	pEquipDB);
	inline VOID								WriteBankDB(const	_BANK_DB_LOAD*	pBankDB);
	inline VOID								WriteSkillDB(const _SKILL_DB_LOAD* pSkillDB);
	inline VOID								WriteCooldownDB(const _COOLDOWN_DB_LOAD_FOR_HUMAN*	pCooldownDB);
	inline VOID								WriteRelationDB(const _RELATION_DB_LOAD* pRelationDB);
	inline VOID								WriteAbilityDB(const _ABILITY_DB_LOAD* pAbilityDB);
	inline VOID								WriteXinFaDB(const _XINFA_DB_LOAD* pXinFaDB);
	inline VOID								WriteImpactDB(const _IMPACT_DB_LOAD* pImpactDB);
	inline VOID								WriteMissionDB(const _MISSION_DB_LOAD* pMissionDB);
	inline VOID								WriteSettingDB(const _SETTING_DB_LOAD* pSettingDB);
	inline VOID								WritePrivateInfo(const _PRIVATE_INFO_DB_LOAD* pPrivateDB);
	inline VOID								WriteSouXiaDB(const _SOUXIA_DB_LOAD* pSouXiaDB);
	inline VOID								WriteMountDB(const _MOUNT_DB_LOAD* pMountDB);                        // [7/8/2010 �¾���]

private:
	//////////////////////////////////////////////////////////////////////////
	// �ӿںͷ���  2005.11.25 ��
	//////////////////////////////////////////////////////////////////////////
	//�����ڴ桢DB����ע������
	VOID*									GetDBEnumAttr(CHAR_ATTR_DB eOffSet);
	VOID*									GetSMEnumAttr(CHAR_ATTR_DB eOffSet);
	INT										GetDBEnumAttrSize(CHAR_ATTR_DB eOffSet);
	
};

#include "HumanDB.inl"

#endif
