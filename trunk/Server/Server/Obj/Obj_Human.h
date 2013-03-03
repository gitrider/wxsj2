//***************************************************************************************************
// �ļ�����:	Obj_Human.h
// ϵͳ����:	
// ģ������:	Obj_Human
// ����˵��:	�����SERVER�ϵ�ʵ����
//
// ����ĵ�:	
// ��    ��:	liam
// ��������:	
// ��    ��:	
// �����Ȩ:	
//
//�޸ļ�¼:
//          1.
//				�� �� ��:liam
//				�޸�ʱ��:2007/10/30
//				�޸�����:���Ե�һ�����ع�(����μ��ĵ�)
//          2.
//				�� �� ��:liam
//				�޸�ʱ��:2007/11/02
//				�޸�����:���Եڶ������ع�(����μ��ĵ�)
//***************************************************************************************************

#ifndef __HUMAN_H__
#define __HUMAN_H__

#include "Obj_Character.h"
#include "HumanDB.h"
#include "HumanItemContainer.h"
#include "HumanItemExtraContainer.h"
#include "HumanItemExtraBag.h"
#include "HumanPetContainer.h"
#include "HumanMountContainer.h"    // [7/8/2010 �¾���]
#include "Team.h"
#include "HumanRelation.h"
#include "StallBox.h"
#include "ExchangBox.h"
#include "GameStruct_Team.h"

#include "GCShopSoldList.h"

#include "HorseAttrCalculater.h"
#include "TimeManager.h"
#include "GameStruct_Skill.h"

#include "HumanOpenBox.h"

#include "SouXia.h"
namespace Packets {
	class GCDetailAttrib_Pet;
};

struct PET_LOG_PARAM;
struct SKILL_LOG_PARAM;
struct XINFA_LOG_PARAM;

//////////////////////////////////////////////////////
// ��ʼ��Obj_Human�Ľṹ
//////////////////////////////////////////////////////
struct FULLUSERDATA;
struct _OBJ_HUMAN_INIT :
	public _OBJ_CHARACTER_INIT
{
	const FULLUSERDATA	*m_pUseData;

	_OBJ_HUMAN_INIT( VOID )
	{
		m_pUseData		= NULL;
	}

	virtual VOID CleanUp( VOID )
	{
		m_pUseData		= NULL;
		_OBJ_CHARACTER_INIT::CleanUp();
	}
};

//////////////////////////////////////////////////////
// ��ͻ���ͬ��������Ϣ����
//////////////////////////////////////////////////////
struct _TEAM_MEMBER_INFO
{
	_TEAM_MEMBER_INFO()
	{
		memset(this, 0, sizeof(_TEAM_MEMBER_INFO));
	}

	UINT					m_uFamily;						// 1.����
	UINT					m_uLevel;						// 2.�ȼ�
	WORLD_POS				m_WorldPos;						// 3.λ�ã����꣩
	INT						m_nHP;							// 4.HP
	UINT					m_uMaxHP;						// 5.HP����
	INT						m_nMP;							// 6.MP
	UINT					m_uMaxMP;						// 7.MP����
	INT						m_nAnger;						// 8.ŭ��
	UINT					m_WeaponID;						// 9.����
	//UINT					m_AssihandID;					// ������
	UINT					m_CapID;						// 10.ñ��
	//UINT					m_ScapularID;					// ���
	UINT					m_ArmourID;						// 11.�·�
	UINT					m_CuffID;						// 12.����
	UINT					m_FootID;						// 13.ѥ��
	// 14.buff����ʱ������
	BOOL					m_bDeadFlag;					// 15.�Ƿ�����
	UINT					m_uFaceMeshID;					// 16.�沿ģ��
	UINT					m_uHairMeshID;					// 17.ͷ��ģ��
	UINT					m_uHairColor;					// 18.ͷ����ɫ
	SimpleImpactList		m_SimpleImpactList;				// Buff �б�
};

//////////////////////////////////////////////////////
// ��ͻ���ͬ����������
//////////////////////////////////////////////////////
struct _HUMAN_ATTR_BACKUP 
{
public:
	_HUMAN_ATTR_BACKUP(VOID);
	VOID CleanUp(VOID);

public:
	BOOL				m_bNameModified;
	BOOL				m_bTitleModified;
	INT					m_Level;
	UINT				m_Exp;
	INT					m_HP;
	INT					m_MP;
	INT					m_nPneuma;			//Ԫ��
	INT					m_MaxHp;
	INT					m_MaxMp;
	INT					m_MaxPneuma;
	FLOAT       		m_MoveSpeed;
	INT					m_nDataID;
	INT					m_nPortraitID;
	INT					m_nModelID;
	INT					m_nMountID;			// ����ı��
	INT					m_nMountIndex;		// ���������   [7/23/2010 �¾���]
	UINT				m_uFaceMeshID;		// �沿ģ��
	UINT				m_uHairMeshID;		// ͷ��ģ��
	UINT				m_uHairColor;		// ͷ����ɫ
	INT					m_nStealthLevel;	// ������
	INT					m_nMoodState;
	UINT				m_Money;
	INT					m_nGoodBadValue;
	INT					m_nStr;
	INT					m_nCon;
	INT					m_nInt;
	INT					m_nDex;
	INT					m_nPointRemain;
	INT					m_nSkillPointRemain;
	INT					m_nS_SkillPoints[COMMON_S_NUMBER];
	INT					m_nReHpSpeed;
	INT					m_nReMpSpeed;
	INT					m_nReVigorSpeed;		//�����ָ�ֵ
	INT                 m_nAttNear;
	INT                 m_nAttFar;

	INT					m_nAttMagicNear;
	INT					m_nAttMagicFar;

	INT                 m_nDefNear;
	INT                 m_nDefFar;

	INT					m_nDefMagicNear;
	INT					m_nDefMagicFar;

	INT                 m_nToughness;
	INT					m_nHit;
	INT					m_nMiss;
	INT					m_nCritic;
	INT					m_nCriticHurt;
	INT					m_MaxRage;
	INT					m_nRage;
	INT					m_nReRageSpeed;
	INT					m_nStrikePoint;
	INT					m_nAttSpeed;

	INT					m_nAttGold;
	INT					m_nDefGold;

	INT					m_nAttWood;
	INT					m_nDefWood;

	INT					m_nAttWater;
	INT					m_nDefWater;

	INT					m_nAttFire;
	INT					m_nDefFire;

	INT					m_nAttSoil;
	INT					m_nDefSoil;

	INT					m_nNearReduce;		//���̼���
	INT					m_nFarReduce;		//Զ�̼���
	
	INT					m_nMagicNearReduce;	//�����ڹ�����
	INT					m_nMagicFarReduce;	//�����⹦����

	INT					m_nDreadReduce;		//�ֿ��־�
	INT					m_nComaReduce;		//�ֿ�����
	INT					m_nHushReduce;		//�ֿ���Ĭ
	INT					m_nUnarmReduce;		//�ֿ���е
	INT					m_nAttSpeedResist;	//����������
	INT					m_nSkillSpeedResist;//��ʩ��
	INT					m_nMoveSpeedResist;	//������

	INT                 m_nCountry;
	INT					m_nMenpai;
	_CAMP_DATA			m_CampData;
	ObjID_t				m_nOwnerID;
	PET_GUID_t			m_guidCurrentHorse;
	INT					m_Vigor ;
	INT					m_MaxVigor ;
	INT					m_Energy;
	INT					m_MaxEnergy;
	BOOL				m_bLimitMove;		//�Ƿ����Ʋ����ƶ�
	BOOL				m_bCanActionFlag1;	//�������Ʊ��1
	BOOL				m_bCanActionFlag2;	//�������Ʊ��2
	BOOL				m_bMissionHaveDoneFlagsChanged;
	BOOL				m_bMainWeaponModified;		//����
	BOOL				m_bAssiWeaponModified;		//����
	BOOL				m_bCapModified;				//ñ��
	BOOL				m_bShoulderModified;		//�ļ�
	BOOL				m_bArmorModified;			//�·�
	BOOL				m_bCuffModified;			//����
	BOOL				m_bBootModified;
	BOOL				m_bSuitModified;			// ��װ - ��Դ��ID
	BOOL				m_bRiderModified;			// ���� - ��Դ��ID
	BOOL				m_bStallIsOpen;
	BOOL				m_bStallNameChanged;

	BOOL				m_bStallIsPublic;			// �Ƿ���(������ʾ̯λ�Ƹ��������)  20100706 BLL
	GuildID_t			m_GuildID;		            //	������ڰ��
	CHAR			    m_cFamilyID;	            //	����
	CHAR	    		m_cGuildOfficial;		    //	����ְ
    INT                 m_nGP;                      //  ��ṱ�׶�
    INT                 m_nHonor;                   //  ����ֵ
    INT                 m_nPK;                      //  PKֵ
};

//////////////////////////////////////////////////////
// ����ܲ�������
//////////////////////////////////////////////////////
struct AbilityOpera
{
	AbilityID_t			m_AbilityID;
	BYTE				m_SubType;	// �ӹ�������ͨ����ʯ�ϳɡ���Ƕ
	PrescriptionID_t	m_PresID;
	ObjID_t				m_Obj;
	INT					m_nMaxTime;
	INT					m_nCurTime;
	INT					m_nRobotTime;
	BYTE				m_BagPos[MAX_ABILITY_ITEM];
	UINT				m_uItemIndex; // ����ϳɹ�������Ҫ����Ʒ��Ϣ�����籦ʯ�ϳɣ�

	VOID	CleanUp()
	{
		memset(this,0,sizeof(AbilityOpera));
	}
};


//////////////////////////////////////////////////////
// HUMANLIST
//////////////////////////////////////////////////////
#define MAX_HUMAN_LIST_SIZE 128
#define LEVELDELTA_CALLUP (10)

class Obj_Human;
struct HUMANLIST
{
	Obj_Human*		m_aHuman[MAX_HUMAN_LIST_SIZE];
	BYTE			m_Count;

	HUMANLIST( )
	{
		m_Count = 0;
	}
};


//////////////////////////////////////////////////////
// Obj_Human
//////////////////////////////////////////////////////
class Player;
class AI_Human;
class Obj_Pet;
class AvoidOverlap;
class ItemContainer;
struct _SHOP;
struct _OBJ_LIST_NODE;
class SouXia;

class Obj_Human : public Obj_Character
{
//===========================================
// Obj���нӿڼ̳�
//===========================================
public:
	Obj_Human( );
	virtual ~Obj_Human( );

	virtual ObjType			GetObjType( VOID )const{ return OBJ_TYPE_HUMAN; }

	virtual VOID OnEnterScene(VOID);
	virtual VOID OnRegisterToZone(VOID);
	virtual VOID OnUnregisterFromZone(VOID);

	virtual VOID CleanUp( VOID );
	virtual BOOL Init( const _OBJ_INIT *pInit );
	virtual BOOL HeartBeat( UINT uTime=0 );

	virtual BOOL IsEnemy(Obj_Character* pCharacter );
	virtual BOOL IsFriend(Obj_Character* pCharacter);
	virtual BOOL IsPartner(Obj_Character* pCharacter);
	BOOL IsMySpouse(Obj_Human& rHuman);
	virtual VOID Teleport( const WORLD_POS* pos );

	//��ȡ����ɵ���ĵ�(dropType��������)
	virtual BOOL GetNearCanDropPos(WORLD_POS& DropPos);
//===========================================
// Character���нӿڼ̳�
//===========================================
public:
	virtual VOID InitAIObj( );

	virtual VOID OnKillObject( ObjID_t idObj );
	virtual VOID OnItemChanged( UINT uItemDataID );
	virtual VOID OnEnterArea( UINT uAreaID );
	virtual VOID OnPetChanged( UINT uPetDataID ) ;
	virtual VOID OnDie( ObjID_t idKiller );

//===========================================
// ��Ϣ/����ˢ��
//===========================================
public:
	virtual Packet		*CreateNewObjPacket( VOID );
	virtual VOID		DestroyNewObjPacket( Packet *pPacket );
	virtual VOID		ReInitExtraBag();
	virtual VOID 		InitBackupAttr( );
	virtual VOID 		SendMsg_RefeshAttrib( VOID );
	virtual VOID 		SyncTeamMemberInfo( );
	virtual VOID 		AskMsg_BaseAttrib( Obj_Human *pTargetHuman );
	VOID 				AskMsg_DetailAttrib(Obj_Human *pAsker);
	static	VOID		CalculatePetDetailAttrib(Packets::GCDetailAttrib_Pet& rMsg, Item* pPetItem);
	VOID				RefeshHorseDetailAttrib(PET_GUID_t guidPet);

	WORD				GetEquipVer( );
	VOID				SetEquipVer( WORD Ver );
	virtual VOID		UpdateViewCharacter(VOID);
protected:
	//Ϊ����ͬ����ʵʱ����CharBase��Ϣ�������HP���ǣ���֯�Զ�ˢ����ʱ�ظ�����
	virtual VOID SendCharBaseDirectForHP();

protected:
	_HUMAN_ATTR_BACKUP	m_AttrBackUp;

//===========================================
//  �����
//===========================================
public:
	// �õ�������Ϣ
	// bSkillRelive		:	�Ƿ�Ϊ���ܸ���
	virtual const _RELIVE_INFO		*GetReliveInfo( BOOL bSkillRelive )const;

	// ���ø�������
	// bSkillRelive		:	�Ƿ�Ϊ���ܸ���
	// pReliveInfo		:	������Ϣ
	virtual VOID			SetReliveInfo( BOOL bSkillRelive, const _RELIVE_INFO *pReliveInfo );

protected:
	// ������Ϣ�ı���¼�(�磺��Ҷ���ȥ�����ʹ�ø����)
	VOID					OnReliveInfoChanged( VOID );

protected:
	_RELIVE_INFO			m_DefaultReliveInfo;		// ȱʡ�������Ϣ
	

//===========================================
//  ����Ĵ���
//===========================================
public:
	// �õ�������Ϣ
	const _CALLOF_INFO		*GetCallOfInfo(VOID)const;

	VOID					ResetCallOfInfo(VOID);

	// ������������
	// pCallOfInfo		:	������Ϣ
	VOID					SetCallOfInfo(const _CALLOF_INFO *pCallOfInfo);

	// ����
	VOID					CallOfHuman(GUID_t guidTarget, UINT uDuration);

protected:
	// ������Ϣ�ı���¼�
	VOID					OnCallOfInfoChanged( VOID );

protected:
	_CALLOF_INFO			m_CallOfInfo;			// �������Ϣ
	UINT					m_uCallOf_StartTime;	// ������Ϣ����ʼʱ��

//===========================================
// AI
//===========================================
public:
	AI_Human*				GetHumanAI( VOID );
	AvoidOverlap*           GetAvoidOverLap()const;
	ObjID_t					GetAssistantID( VOID ) const;
	VOID					SetAssistantID( ObjID_t id );
	VOID					SetCurTarget( ObjID_t idTarget );
	ObjID_t					GetCurTarget( VOID ) const;

protected:
	AvoidOverlap			*m_pAvoidOverlap;	// Add by FrankWu[9/19/2005]
	ObjID_t					m_AssistantID;		//����������

//===========================================
// ����
//===========================================
public:
	BOOL					IsFirstEnterScene();
	VOID					ClearFirstEnterScene();

	virtual UINT			GetUniqueID(VOID) const;
	//DB�ӿ�
	HumanDB*				GetDB();
	GUID_t					GetGUID( VOID )const;

	virtual UINT			GetDataID( VOID )const;

	virtual VOID			SetPortraitID( INT nID );
	virtual INT				GetPortraitID( VOID ) const;

	virtual	VOID			SetName( const CHAR *pszName );
	virtual const CHAR		*GetName( VOID )const;

	virtual	VOID			SetTitle( const CHAR *pszTitle );
	virtual const CHAR		*GetTitle( VOID )const;

	virtual	VOID			SetTitleType( BYTE nType );
	virtual const BYTE		GetTitleType( VOID )const;

	virtual VOID			SetHP( INT nHP );
	virtual INT				GetHP( VOID );

	virtual VOID			SetMP( INT nMP );
	virtual INT				GetMP( VOID );

	virtual VOID			SetPneuma( INT nPneuma);
	virtual INT				GetPneuma( VOID );

	virtual VOID			SetLevel( INT nLevel );
	virtual INT				GetLevel( VOID )const;

	virtual VOID			SetPKValue( INT nValue );
	virtual INT				GetPKValue( VOID )const;

	virtual VOID			SetCurrentHorseGUID( PET_GUID_t guid );
	virtual PET_GUID_t		GetCurrentHorseGUID( VOID )const;

	VOID					SetAlterExp( INT AlterExp );	//�ı�ľ���
	virtual UINT			GetExp( );
	virtual VOID			SetExp(UINT uExp);
	virtual INT				GetStrikePoint(VOID);
	virtual VOID			SetStrikePoint(INT nStrikePoint);

	virtual INT				GetVigor(VOID);
	virtual VOID			SetVigor(INT nVigor);

	virtual INT				GetMaxVigor(VOID);
	virtual VOID			SetMaxVigor(INT nMaxVigor);

	virtual	INT				GetVigorRegeneRate(VOID);
	virtual	VOID			SetVigorRegeneRate(INT nRate);

	virtual INT				GetEnergy(VOID);
	virtual VOID			SetEnergy(INT nEnergy);

	virtual INT				GetMaxEnergy(VOID);
	virtual VOID			SetMaxEnergy(INT nMaxEnergy);

	virtual	INT				GetEnergyRegeneRate(VOID);
	virtual	VOID			SetEnergyRegeneRate(INT nRate);

	virtual INT 			GetRage( VOID );
	virtual VOID			SetRage( INT nRage );
	VOID					RestoreRageFromEXP(INT nExp);

	UINT					GetMoney();
	VOID					SetMoney(UINT money);

	INT						GetGoodBadValue();
	INT						SetGoodBadValue(INT nValue);
	INT						IncGoodBadValue(INT nValue);
	INT						DecGoodBadValue(INT nValue);

	_PLAYERSHOP_GUID		GetShopGuid(INT nIndex);
	VOID					SetShopGuid(INT nIndex, _PLAYERSHOP_GUID pGuid);

	_PLAYERSHOP_GUID		GetFavoriteShopGuid(INT nIndex);
	VOID					SetFavoriteShopGuid(INT nIndex, _PLAYERSHOP_GUID pGuid);
	BOOL					IsInFavorite(_PLAYERSHOP_GUID pGuid);
	BOOL					AddFavorite(_PLAYERSHOP_GUID pGuid);
	BOOL					DelFavorite(_PLAYERSHOP_GUID pGuid);
	VOID					UpdateFavorite(_PLAYERSHOP_GUID* pGuidList);

	VOID					SetGuildID(GuildID_t guildID);
	GuildID_t				GetGuildID( );
    VOID					SetFamilyID(INT nFamilyID);
    INT     				GetFamilyID( );
    VOID                    SetGuildOfficial(CHAR cOfficial);
    CHAR                    GetGuildOffocial( );
    VOID                    SetGuildPoint(INT nGP);
    INT                     GetGuildPoint( );
    INT                     GetHonor();
    VOID                    SetHonor(INT nHonor);

    VOID 		            SendMsg_RefeshGP( INT nGP, INT nType );
    VOID 		            SendMsg_RefeshAP( INT nAP, INT nType );

	BOOL					GetSex();
	INT						GetCountry( );
	VOID					SetCountry( INT nCountry );
	MENPAI_ATTRIBUTE		GetMenPai( );
	VOID					SetMenPai( INT Menpai );
	INT						Get_RemainPoints();
	VOID					Set_RemainPoints(INT Point);
	VOID					Set_CommonS_SkillPoints(INT nS, INT Point);
	
	INT						Get_RemainSkillPoints();	//ʣ�༼�ܵ�
	VOID					Set_RemainSkillPoints(INT Point);	//ʣ�༼�ܵ�
	
	INT						Get_S_SkillPoints(INT nS);	//���ܵ�
	VOID					Set_S_SkillPoints(INT nS, INT Point);	//�༼�ܵ�

	VOID					ValidatePlayerID();

	UINT					GetEquipID(HUMAN_EQUIP equipPoint);
	UINT					GetEquipLevel(HUMAN_EQUIP equipPoint);
	//�ƺ�		
	//const CHAR*				GetCurCountryTitle();
	VOID					SetCurCountryTitle( INT iTitleID);
    VOID                    SetCurGuildTitle(INT iTitleID);
    VOID                    SetCurNormalTitle(INT iTitleID);
    INT  					GetCurCountryTitle();
    INT                     GetCurGuildTitle();
    INT                     GetCurNormalTitle();
    const CHAR*             GetCurCountryTitleName();
    const CHAR*             GetCurGuildTitleName();
    const CHAR*             GetCurNormalTitleName();
    const CHAR*             GetTitleNameByID(INT iTitleID, INT iSex );


    VOID                    AddTitleBuff(INT buffID1, INT buffID2);
    VOID                    DelTitleBuff(INT buffID1, INT buffID2);
    VOID                    RemoveTitle(INT iTitleID);

    VOID                    SetGuildTitleName(const CHAR *pszTitle, INT iSize);
    const CHAR*             GetGuildTitleName() const;

    VOID                    SetCountryTitleName(const CHAR *pszTitle, INT iSize);
    const CHAR*             GetCountryTitleName() const;

    VOID                    SetNormalTitleName(const CHAR *pszTitle, INT iSize);
    const CHAR*             GetNormalTitleName() const;

	VOID					UpdateTitlesToClient(BYTE cType = 0);

    VOID                    SetTitleLoginTime (time_t time);
    time_t                  GetTitleLoginTime ();

    VOID                    UpdateTitleTime();

    //���ޱ���ʱ��
    VOID                    SetLoginProtectTime(time_t time);
    BOOL                    IsInLoginProtectTime();

    private:
    time_t                  m_TitleLoginTime;                    //��¼������ߵ�ʱ��㣬��ʱ�ƺ���
    time_t                  m_LoginProtectTime;

    public:
	//һ����װ
	const _SUIT_SETTING& 	GetEquipSuitSetting( UINT nEquipSuitNum ) const;
	virtual VOID			SetEquipSuitSetting( _SUIT_SETTING& nSuitSetting, UINT nEquipSuitNum );

	const _SUBTRAIN_SETTING& GetSubTrainSetting( ) const;
	virtual VOID			SetSubTrainSetting( _SUBTRAIN_SETTING& nSuitSetting);
	// GMר��
	VOID					SetGod( BYTE bGod );
	BOOL					IsGod(VOID)const;

	VOID					SetChangeSceneFlag( BYTE bChangeSceneFlag );
	BOOL					IsHaveChangeSceneFlag(VOID) const;

	VOID					UpdateSubTrainToClient();
protected:
	//������й���Obj_Human�ġ������ݿ����ȡ����Ϣ
	HumanDB					m_DB;
	BYTE					m_bChangeSceneFlag;
	BYTE					m_bGod;

//===========================================
// ����
//===========================================
public:
	virtual INT				SkillCharacter( Obj_Character* pCharacter, SkillID_t SkillID, BOOL bNow=FALSE );

	BOOL					Skill_RefixItemAttr(INT nSlotID, INT nItemType, INT nATTRID, SHORT& nAttrValue);
	BOOL					Skill_DepleteItem(VOID);
	virtual BOOL 			Skill_HaveSkill( SkillID_t const nID ) const;
	virtual BOOL 			Skill_ModifySkill(SkillID_t	const nID, INT const nModifyFlag);
	VOID					Skill_AllXinFaLevel(INT const nLevel);
	virtual BOOL 			Skill_HaveXinFa( SkillID_t const nID ) const;
	virtual INT 			Skill_GetXinFaLevel(SkillID_t const nID) const;
	virtual BOOL 			Skill_SetXinFaLevel(SkillID_t const nID, INT const nLevel);
	virtual VOID 			Skill_OnEquipItem(VOID);


	
	// ��ӻ�����һ������
	VOID					Skill_SetSkillLevel( SkillID_t const nID, INT const nLevel);
	// 
	const _OWN_SKILL*		Skill_GetSkill(INT const nID);

	const	_SKILL_LIST&	Skill_GetSkillList(VOID);
	const   _XINFA_LIST& 	Skill_GetXinFaList(VOID);

	virtual Time_t			GetCooldownRemained(CooldownID_t const nID) const;
	virtual BOOL			IsCooldowned(CooldownID_t const nID) const;
	virtual VOID			SetCooldown(CooldownID_t const nID, Time_t nCooldown);
	virtual BOOL 			HeartBeat_Cooldown(UINT uTime = 0);
	virtual BOOL			HeartBeat_Recover(UINT uTime = 0);

	VOID					ValidateShareMem(BOOL bForceAll); //HeartBeat ������ShareMemory
	VOID SaveMoneyLogByHuman( MONEY_OP_TYPE eReason, INT iModify );


private:
	VOID	Skill_InitSkillList();
	VOID	Skill_InitXinFaList();
	BOOL 	Skill_RegisterSkill(SkillID_t const nID);
	BOOL 	Skill_UnregisterSkill(SkillID_t const nID);
	BOOL 	Skill_RegisterXinFa(XinFaID_t const nID);
	BOOL 	Skill_UnregisterXinFa(SkillID_t const nID);
	VOID	Skill_CleanMenPaiSkillAndXinFa(SKILL_LOG_PARAM* pSkillLog,XINFA_LOG_PARAM* pXinFaLog,INT Menpai);
	VOID	Skill_ADDAllMenPaiSkillAndXinFa(SKILL_LOG_PARAM* pSkillLog,XINFA_LOG_PARAM* pXinFaLog,INT Menpai);

private:
	BOOL					m_bCanIncreaseRage;					//��������ר��
	Time_t					m_nCommonCooldown;					//������ȴʱ��

//===========================================
// Ч��
//===========================================
public:

protected:
	virtual _IMPACT_LIST&	Impact_GetImpactList(VOID);

private:
	VOID 					Impact_InitList(VOID);

//===========================================
// ����
//===========================================
public:
	//��Ʒ�����ⲿ�߼��ӿ�
	BOOL					IsVisualPart(HUMAN_EQUIP EquipPoint);						//�Ƿ�������ҿɼ�����
	BOOL					ActivateItem(VOID);											//ʹ����Ʒ
	INT						UseStoreMap(uint BagIndex);									//ʹ�òر�ͼ
	INT						UseIdentScroll(UINT BagIndex,UINT TargetBagIndex);			//ʹ�ü�������
	INT						UseGem(Item* pEquipItem,uint GemBagIndex, UINT GemHolePos);//ʹ��Gem
	INT						RemoveGem(uint BagIndex,uint GemIndex);						//�Ƴ�Gem
	INT						AddHole(Item* pEquipItem);
	INT						EquipLevelup(Item* pEquipItem, UINT nGemID, UINT nGemCount);
	INT						EquipRemould(Item* pEquipItem, UINT nMaterialScore);
	INT						EquipBind(Item* pEquipItem,uint GemBagIndex);
	INT						EquipChangePai(Item* pEquipItem, uint GemBagIndex);
	INT						EquipChangePai(Item* pEquip1Item, Item* pEquip2Item, uint GemBagIndex);
	INT						EquipAdsorbMagic(Item* pEquipItem, Item* pBeadItem);		//��ħ
	INT						EquipCancelMagic(Item* pEquipItem);							//��ħ
	BOOL					UseItem(ID_t nBagIndex, ObjID_t nTargetObj, const WORLD_POS& posTarget, PET_GUID_t const& guidTargetPet, ID_t nTargetItem);//ʹ����Ʒ�Ĺ���
	INT						UseEquip(INT BagIndex);
	BOOL 					VerifyItem(VOID); //У��ʹ���е���Ʒ�Ƿ����
	virtual VOID			UpdataEquipDamagePoint(INT nType);	//����װ�����˵�
	virtual	BOOL			IsCarrayItem(UINT ItemType,UINT ItemCount);
	/*
	*	��Ʒ��������
	*/
	//������Ʒ��
	ItemContainer*	GetBaseContain();
	
	ItemContainer*	 GetExtraContain();

	ItemContainer*	 GetExtraBagContain(INT nIndex);
	////������Ʒ��
	//ItemContainer*	GetTaskContain();
	////������Ʒ��
	//ItemContainer*	GetMatContain();
	//װ��
	ItemContainer*	GetEquipContain();
	//����
	ItemContainer*	GetBankContain();
	//����
	ItemContainer*	GetPetContain();
    //���� [7/8/2010 �¾���]
	ItemContainer*  GetMountContain();

	//�������ϵ�װ��������Ӱ������
	//������ϵ�װ�������仯������Ҫ����һ�δ˽ӿڣ����¼�������Ч��
	_ITEM_EFFECT_SKILL*     GetEquipEffectSkill(INT nSkillID);
	_ITEM_IMPACT*			GetEquipImpact(INT nImpactID);

	VOID					CleanEquipEffectSkill();
	VOID					CleanEquipImpact();
	VOID					UnRegEquipImpact();
	VOID					RegEquipImpact();
	VOID					ItemEffectFlush( );
	VOID					CaculateEffect(_ITEM_ATTR& Attr,INT EquipOffset, INT nItemType=INVALID_ID);
	VOID					CaculateEquipLevelEffect(UINT nRate);
	VOID					CaculateEquipSetEffect();
	VOID					CaculatePaiEffect();
	VOID					CaculateEquipSoulAndChainEffect();
	void					AddEffect(_ITEM_ATTR& Attr);

	//װ���ĸ���Ӱ������
	_ITEM_EFFECT*		ItemEffect( BYTE AttrType ){//AttrType=enum ITEM_ATTRIBUTE 
		Assert( AttrType<IATTRIBUTE_NUMBER );
		return &m_pItemEffect[AttrType];
	}

	virtual _ITEM_VALUE	ItemValue( BYTE AttrType ){//AttrType=enum ITEM_ATTRIBUTE 
		Assert( AttrType<IATTRIBUTE_NUMBER );
		return m_pItemEffect[AttrType].m_Attr;
	}

	VOID					CheckAllAbility(); //����ܼ��
private:
	//����Ʒ����ĸ���Ч��
	_ITEM_EFFECT		m_pItemEffect[IATTRIBUTE_NUMBER];

	//װ��Ӱ��ļ���
	INT						m_EffectSkillNum;
	_ITEM_EFFECT_SKILL		m_EquipEffectSkill[HEQUIP_NUMBER*MAX_EQUIP_EFFECT_SKILL];

	//װ�����ӵ�Ч��
	INT						m_EquipImpactNum;
	_ITEM_IMPACT			m_EquipImpactEffect[HEQUIP_NUMBER+MAX_ITEM_SET_ATTR];

private:
	//����������Ʒ��
	HumanItemContainer		m_BaseItem;	
	
	//��չ��
	HumanItemExtraContainer m_ExtraContainerItem;
	
	//��չ��Ʒ��
	HumanItemExtraBag		m_ExtraBagItem[MAX_EXTRA_BAG_NUM];	

	
	////������Ʒ��
	//HumanItemContainer		m_TaskItem;
	////������Ʒ
	//HumanItemContainer		m_MatItem;
	//��ɫװ��				
	HumanItemContainer		m_EquipItem;
	//��ɫ����
	HumanItemContainer		m_BankItem;
	//���ﱳ��
	HumanPetContainer		m_PetItem;
	//���� [7/8/2010 �¾���]
	HumanMountContainer     m_MountItem;

//===========================================
// �����
//===========================================
public:
	const _ABILITY_LIST*	GetAbilityList( ) const;
	AbilityOpera*			GetAbilityOpera( );
	VOID					reset_AbilityOpera( ); 
	VOID					InterruptCurrentAbilityOpera();

	// ��ȡ��ҽ����¼������ʱ��
	UINT GetAreaTimeStamp()
	{
		return m_uTimeStamp;
	}

	// ����������ҽ����¼������ʱ�䣨��Ϊ��һ�δ����¼���ʱ�����
	VOID ResetAreaTimeStamp(UINT newTimeStamp)
	{
		m_uTimeStamp = newTimeStamp;
	}

protected:
	virtual	ORESULT			Do_UseAbility( VOID );				//����UseAbility�߼�
	virtual	BOOL			Obj_Logic_UseAbility( UINT uTime );			//ִ��UseAbility�߼�
	virtual VOID			OnCharacterLogicStopped(BOOL bAbortive);

private:
	AbilityOpera			m_LastOpera;
	//������ֵ����ʱ����Ӧ�;���Ĺ���ID;
	ObjID_t					m_iTargetForExp;

//===========================================
// ����
//===========================================
public:
	const _MISSION_DB_LOAD *GetMissionList( VOID );
	const _MISSION_DB_LOAD *GetConstMissionList( VOID )const;
	//const UINT *GetMissionHaveDoneFlags( VOID )const;

	// �����Ƿ��Ѿ�������ĳ������
	VOID SetMissionHaveDone( MissionID_t idMission, BOOL bHaveDone );

	// �Ƿ��Ѿ�������ĳ������
	BOOL IsMissionHaveDone( MissionID_t idMission )const;

	// ��һ�������������б�
	ORESULT AddMission( MissionID_t idMission, ScriptID_t idScript, BOOL bKillObjectEvent = TRUE, BOOL bEnterAreaEvent = TRUE, BOOL bItemChangedEvent = TRUE );
	ORESULT AddMissionEx( MissionID_t idMission, ScriptID_t idScript ) ;
	ORESULT SetMissionEvent( MissionID_t idMission, INT EventID/*TASK_EVENT*/ ) ;
	//ORESULT AddMissionKillObjectNotify( MissionID_t idMission, ScriptID_t idScript,INT killObject0,INT killObject1,INT killObject2,INT killObject3);
	//ORESULT AddMissionEnterAreaNotify( MissionID_t idMission, ScriptID_t idScript,INT areaId0,INT areaId1,INT areaId2,INT areaId3);
	//ORESULT AddMissionItemChangedNotify( MissionID_t idMission, ScriptID_t idScript,INT itemTypeSn0,INT itemTypeSn1,INT itemTypeSn2,INT itemTypeSn3);

	// �������б���ɾ��һ������
	VOID DelMission( MissionID_t idMission );
	VOID DelMissionByIndex( UINT uIndex );

	// ͨ������IDȡ�������������������ֵ�� �޸�����ʱ����UINT_MAX
	UINT GetMissionIndexByID( MissionID_t idMission )const;

	// ����Ƿ��Ѿ������������
	BOOL IsHaveMission( MissionID_t idMission )const;

	// �����������
	VOID SetMissionParam( UINT uIndexMission, UINT uIndexParam, INT nValue );

	// ȡ�������
	INT GetMissionParam( UINT uIndexMission, UINT uIndexParam )const;
	const _OWN_MISSION *GetMission( UINT uIndex )const;

	//UINT GetCanPickMissionItemCount( VOID )const;
	//const UINT *GetCanPickMissionItemList( VOID )const;
	//VOID AddCanPickMissionItem( UINT uItemDataID );
	//VOID RemoveCanPickMissionItem( UINT uItemDataID );

	//��ȡ������������
	INT GetMissionData( INT nIndex );

	//���õ�ǰ������������
	VOID SetMissionData( INT nIndex, INT nData );

protected:
	virtual VOID OnMissionHaveDoneFlagsChanged( MissionID_t idMission );
	virtual VOID OnMissionParamChanged( UINT uIndexMission, UINT uIndexParam ,INT index =0);
	virtual VOID OnAddMission( UINT uIndex );
	virtual VOID OnRemoveMission( MissionID_t idMission );
	virtual VOID OnLeaveScene(VOID);

	//virtual VOID OnAddCanPickMissionItem( UINT uItemDataID );
	//virtual VOID OnRemoveCanPickMissionItem( UINT uItemDataID );

protected:
	// ����һ��Ҫ�ص��Ż���ɱ��OBJECT���¼����õ���ɾ����Ʒ���¼����Լ������л����¼���
	//UINT				m_uMissionIndex_KillObjectEvent_Num;
	//UINT				m_uMissionIndex_EnterAreaEvent_Num;
	//UINT				m_uMissionIndex_ItemChangedEvent_Num;
	//UINT				m_aMissionIndex_KillObjectEvent[MAX_CHAR_MISSION_NUM];
	//UINT				m_aMissionIndex_EnterAreaEvent[MAX_CHAR_MISSION_NUM];
	//UINT				m_aMissionIndex_ItemChangedEvent[MAX_CHAR_MISSION_NUM];

	//UINT				m_aMissionIndex_KillObject[MAX_CHAR_MISSION_NUM * MAX_MISSION_NOTIFY_NUM];
	//UINT				m_aMissionIndex_EnterArea[MAX_CHAR_MISSION_NUM * MAX_MISSION_NOTIFY_NUM];
	//UINT				m_aMissionIndex_ItemChanged[MAX_CHAR_MISSION_NUM * MAX_MISSION_NOTIFY_NUM];

	//UINT				m_uCanPickMissionItemIDNum;
	//UINT				m_aCanPickMissionItemList[MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM];

//===========================================
// ����/��ϵ��
//===========================================
public:
	TeamInfo*		GetTeamInfo();
	HumanRelation*	GetHumanRelation();

private :
	FLOAT					m_fTeamFollowSpeed;
	BOOL					m_bTeamFollowSpeedUp;
	time_t					m_OutofTeamFollowRangeTime;
	_TEAM_MEMBER_INFO		m_SyncTeamMemberInfo;
	TeamInfo				m_TeamInfo;			//�����������
	HumanRelation			m_HumanRelation;	//��ϵ���������

//===========================================
// Timer
//===========================================
public:

	VOID					AddTimer(INT sceneTimerIndex);		
	VOID					RemoveTimer(INT sceneTimerIndex);	

	INT						GetUseTimerCount();	
	INT						GetMaxTimerCount();	

	BOOL					HaveTimer(INT sceneTimerIndex);

private:
	INT						m_nUseTimer;
	INT*					m_pUseTimer;

//===========================================
// ��̯/����/����/�̵�
//===========================================
public:
	//��̯
	VOID				StallNameChanged()	{m_AttrBackUp.m_bStallNameChanged	=	TRUE;}
	BOOL				GetStallIsOpen() { return m_StallBox.GetStallIsOpen(); }
	ServerStallBox		m_StallBox;

	//����
	BOOL				IsBankOpen(){return BankIsOpen;}
	VOID				SetBankOpen(BOOL bopen){BankIsOpen = bopen;}

	//�̵�
	VOID				InitialSoldList();
	VOID				AddToSoldList(INT index, UINT uPrice, UINT uCount);
    VOID                GetSlodListInfo(GCShopSoldList::_MERCHANDISE_ITEM *pItem);
	BOOL				GetCurFromSoldList(_ITEM& itemref, UINT& uPrice );
	BOOL				GetFromSoldListByIndex(INT iIndex, _ITEM& itemref, UINT& uPrice);
	BOOL				RecieveOneFromSoldList(INT iIndex);
	VOID				DeleteFromSoldList();
	VOID				ClearSoldList();

	// 20100413 AddCodeBegin
	VOID				ReorderSoldList();	// ��������ع���Ʒ�б�ȥ���м�Ŀ���Ʒ��
	// 20100413 AddCodeEnd

	//���������õ�
	VOID				Shop( _SHOP* pShop){m_pShop = pShop;};
	_SHOP*				Shop( ){return m_pShop;};

public:
	//����
	ServerExchangeBox	m_ExchangBox;//��Ϊ���У���Ϊ��Ҫ���׶Է���Ƶ�����ʡ�

	//����������У��ݴ���Ʒ������
private:
	HumanOpenBox	m_HumanOpenBox;

public:
	//by zheng 
	//������ʱ��ʹ������
	HumanOpenBox* GetHumanOpenBox();


private:
	//����
	BOOL				BankIsOpen;

	//�̵�
	ItemContainer		m_SoldContainer;							//������Ʒ��container;
	UINT				m_SoldPrice[MAX_BOOTH_SOLD_NUMBER];			//���ۼ۸�;
	_ITEM				m_ItemHasBeenSold[MAX_BOOTH_SOLD_NUMBER]; 
	INT					m_nCurSold;
	ObjID_t				m_CurTargetID;

	//�������˷��������ĵ�ǰNPCָ��
	_SHOP*				m_pShop;
    MINORPASSWD_INFO    m_PWProtect;                                //���뱣��

//===========================================
// Other
//===========================================
public:
	VOID ChangeScene( SceneID_t sSceneID, SceneID_t dSceneID, const WORLD_POS& pos, BYTE uDir ) ;
	VOID CreateCity( SceneID_t PortSceneID) ;
	VOID DeleteCity( SceneID_t PortSceneID, INT iSceneIndex) ;
	VOID ChangeCityBuilding( SceneID_t SceneID, INT BuildingID, INT BuildingLevel) ;
	// �Ƚ϶�������
	BOOL ComparePasswd( const CHAR* szPasswd );

	PlayerID_t				GetPlayerID( );

	Player*					GetPlayer(){ return m_pPlayer; }
	VOID					SetPlayer( Player* pPlayer ){ m_pPlayer = pPlayer; }
	VOID 					Levelup(VOID); //������������Բ���
	VOID					ChangeMenpaiPoints(VOID); //ת������ʱ�������¼���
	VOID 					WashPoints(VOID);//ϴ�����ز���
	BOOL 					ManualAttrPoint(CHAR_ATTR_LEVEL1 attrType);//�ֶ��ӵ�
	BOOL 					ManualAttrPoint(INT nStr, INT nCon, INT nInt, INT nDex);//�ֶ��ӵ�
	VOID					ChangeMenpai(SKILL_LOG_PARAM* pSkillLog,XINFA_LOG_PARAM* pXinFaLog,INT Menpai);
	const _SETTING_DB_LOAD *GetSettingList( VOID );
	const _SETTING_DB_LOAD *GetConstSettingList( VOID )const;

	VOID 					SendOperateResultMsg(INT nCode);
	VOID					RefreshAttrToWorld(UINT uTime);	//��GLServerˢ������

protected:

	BOOL						m_bIsPasswdUnlock;	// ����������
	BOOL						m_bUpdateAttr;		//���·Ǽ�ʱ��Ϣ
	CMyTimer					m_UpdateTimer;

	CMyTimer					m_VERecoverTimer;	// �����뾫���Ļָ�����

	CMyTimer					m_RefreshAttrToWorldTimer;	//��GLServerˢ�����ݵĶ�ʱ��

public:
	//////////////////////////////////////////////////////////////////////////
	//�����ڵ�ͼ�ϵ�ע��ڵ�
	_OBJ_LIST_NODE*			HumanNode(){ return m_pHumanNode; }
	BYTE					getMoveIndex( ){ return m_MoveIndex; }
	VOID					setMoveIndex( BYTE index ){ m_MoveIndex = index; }

	//���Լ������¿�����pObj����Ϣ
	//VOID					SendPacket_NewViewObj( Obj* pObj );

	SceneID_t			GetTransSceneID( );
	VOID				SetTransSceneID( SceneID_t id );

private :
	PlayerID_t			m_PlayerID;
	Player*				m_pPlayer;

	//��ҽڵ���Ϣ������ע����Zone��HumanList���
	_OBJ_LIST_NODE*		m_pHumanNode;

	BYTE				m_MoveIndex;	//�ͻ��˵ķ����ƶ�������

	WORD				m_EquipVer;
	SceneID_t			m_TransSceneID;

protected:
	CMyTimer			m_CheckZoneTimer; // �� Zone ���м�飨Area etc...���ļ�ʱ��
	UINT				m_uAreaID;
	ScriptID_t			m_uAreaScript;
	UINT				m_uTimeStamp;
	UINT				m_uLastTurnPickTime; //���һ��ʰȡʱ��

protected:
	VOID				CheckArea();
	VOID				RefeshAttrMsg() ;		//ˢ�½�ɫ���Զ���������

protected:
	CMyTimer			m_RefeshTimer;

public:
	BOOL				GetPosition(WORLD_POS& Pos, const WORLD_POS* pRefPos, FLOAT fDir);

//===========================================
// ����
//===========================================
public:
	//���Գ����Ƿ���Ա��г�
	ORESULT				TestCallUpPet(const PET_GUID_t guidPet);
	ORESULT				CallUpPet(VOID);

	ORESULT				ReCallPet( INT iIndex );

	//�ջس���
	ORESULT				ReCallPet(VOID);
	//����������������
	ORESULT				FreePetToNature(PET_LOG_PARAM* pPetLogParam,PET_GUID_t guidPet);
	ORESULT				AddPetByDataID(PET_LOG_PARAM* pPetLogParam,const UINT uDataID);

	VOID				BabyToAttack(VOID);
	//�������ܵ�����ʱ�ص�
	//ObjID_t iObjID ����ID
	VOID				BabyToProtectMe( ObjID_t iObjID );
	BOOL				CaptureHorse(PET_LOG_PARAM* pPetLogParam, ObjID_t PetID );
	Obj_Pet*			GetPet( INT iIndex ) const;
	BOOL				CanHorseReturnToChild(PET_GUID_t guid); // �Ƿ���Ի�ͯ
	BOOL				HorseReturnToChild(PET_GUID_t guid); // ��ͯ�ӿ�

	VOID				SetGUIDOfCallUpPet(PET_GUID_t guid) { m_GUIDOfCallUpPet = guid; }
	PET_GUID_t			GetGUIDOfCallUpPet(VOID) const { return m_GUIDOfCallUpPet; }

	ORESULT				ReCallHorse(VOID);
	INT					GetHorseCount(VOID)const;
	_PET_DB *			GetHorseDB( INT nIndex )const;
	_PET_DB *			GetEmptyHorseDB();
	const _PET_DB *		GetConstHorseDB( INT nIndex )const;

	Item *				GetHorseItem(PET_GUID_t guid);
	INT					GetHorseIndexByGUID(PET_GUID_t guid);

	// ��ʱ����
	PET_GUID_t			GetPetGUIDByIndex(INT nIndex);
	PET_GUID_t			GetPetGUIDByValidIndex(INT nIndex);

	PET_GUID_t			GetGUIDOfCallUpHorse() const { return m_GUIDOfCallUpHorse; }
	VOID				SetGUIDOfCallUpHorse(PET_GUID_t val);
	VOID				UpdateHouseExp( INT iExp );

	BOOL				IsHaveHorseFindingMate() const { return !m_guidFindingMateHorse.IsNull(); }
	VOID				SetFindingMateHorseGUID(PET_GUID_t guid) { m_guidFindingMateHorse = guid; }
	PET_GUID_t			GetFindingMateHorseGUID() const {return m_guidFindingMateHorse;}

public:
	BYTE				GetHorseHappiness( PET_GUID_t guid );
	VOID				SetPetHappiness( PET_GUID_t guid, BYTE byHappiness );

	INT					GetPetHP( PET_GUID_t guid );
	VOID				SetPetHP( PET_GUID_t guid, INT nHP );

	INT					GetPetMaxHP( PET_GUID_t guid );

	INT					GetPetLife( PET_GUID_t guid );
	VOID				SetPetLife( PET_GUID_t guid, INT nLife );
	PET_GUID_t			GetMatingHorseGuid() const { return m_guidMatingHorse; }
	VOID				SetMatingHorseGuid(PET_GUID_t val) { m_guidMatingHorse = val; }
	BOOL				IsMatingHorseConfirm() const { return m_bMatingHorseConfirm; }
	VOID				SetMatingHorseConfirm(BOOL val) { m_bMatingHorseConfirm = val; }
	//�ٻ��޵ı��ID
	INT GetMonsterPetDataID() const { return m_iDataID; }
	VOID SetMonsterPetDataID(INT val) { m_iDataID = val; }

	BOOL IsCanLogic( );

	SceneID_t 			m_CreateCopySceneID;

	//===========================================
	// BUS
	//===========================================
public:
	BOOL IsInBus(VOID)const
	{
		return m_nBusObjID != INVALID_ID;
	}
	BOOL IsPassenger(VOID);
	BOOL IsChauffeur(VOID);

	ObjID_t GetBusObjID(VOID)const
	{
		return m_nBusObjID;
	}

	VOID SetBusObjID(ObjID_t nBusObjID);

protected:
		ObjID_t				m_nBusObjID;

protected:
	BOOL				AddHorse(const _PET_DB *pPetDB);
	//�ٻ�����ط���
	BOOL				RemovePet(PET_LOG_PARAM* pPetLogParam,PET_GUID_t guid);

	ORESULT				CreatePetOnRegisterZone(VOID);
	BOOL				ReleasePet(VOID);

	ORESULT				CreatePet( INT iIndex );
	BOOL				ReleasePet( INT iIndex );
	INT					GetEmptyPetIndex();
	VOID				SaveMonsterPetToDB();
	//��������ӵ�е���ĸ���
	VOID				HorseHeartBeat( UINT uTime );

	VOID				MatingHorseHeartBeat( INT iIndex, UINT uDurnTime );
	VOID				RemoveMatingTimeOutHorse( INT iIndex );
	VOID				SendHorseMatingDeleteMSG();

	ORESULT		CreateHorse( INT iIndex );
	BOOL				ReleaseHorse(VOID);
	BOOL				ReleaseHorse( INT iIndex );
	INT					GetEmptyHorseIndex();
	ORESULT		CallUpHorse(VOID);

	//������ڽ�����������
	INT					GetMatingEndHorseIndex( UINT uTime, UINT& uDurnTime );
	VOID				SendHorseMatingSuccessMSG();
protected:
	Obj_Pet*			m_pPet[MAX_MONSTER_PET_TAKE];
	//Obj_Horse*		m_pHorse[MAX_MONSTER_HORSE_TAKE];
	INT					m_iDataID;

	PET_GUID_t			m_GUIDOfCallUpPet;
	//��ǰ��ս�����GUID
	PET_GUID_t			m_GUIDOfCallUpHorse;
	PET_GUID_t			m_guidMatingHorse;
	BOOL				m_bMatingHorseConfirm;
	HorseAttrCalculater m_ActiveHorse;
	PET_GUID_t			m_guidFindingMateHorse;

// ����¼add by gh 2010/05/12
protected:
	BYTE	m_nCurSouXiaCount; // ��ǰ�����Ѿ�ѧϰ��������¼����
	SouXia	m_SouXia[MAX_SOUXIA_CONTAINER];
	
private:
	BOOL	CanAddSouXia();
	INT		FindEmptyPos();

public:
	BYTE	GetCurSouXiaCount()			const	{ return m_nCurSouXiaCount; }
	SouXia  GetSouXia(SHORT posIndex)	const	{ return m_SouXia[posIndex]; }	
	VOID    SetSouXia(SOUXIA_DATA& souxiadata, SHORT index)	{ Assert(index>=0); m_SouXia[index].SetSouXiaData(souxiadata);}
	
	INT		AddSouXia(const INT souXiaIndex);		//���Ӓ���¼
	BOOL	EraseSouXia(const INT humanSouXiaPos);//ɾ������¼
	SouXia  GetCurEquipSouXia();//��ȡ��ǰװ���Ē���¼

	BOOL	HasLearnedProduct(UINT index);//�Ƿ��Ѿ�ѧ�����䷽

	SHORT	GetPetZhaoHuanSkillTimes( SkillID_t skillId );	//��ȡ�����ٻ����ܴ���
	SHORT	GetZuoJiZhaoHuanSkillTimes( SkillID_t skillId );//��ȡ�����ٻ����ܴ���

	VOID	SetPetZhaoHuanSkillTimes( SkillID_t skillId, SHORT iDeplete );//���������ٻ����ܴ���
	VOID	SetZuoJiZhaoHuanSkillTimes( SkillID_t skillId, SHORT iDeplete );//���������ٻ����ܴ���

public:
	virtual BOOL		Pet_Skill_Have( PET_GUID_t guidPet, SkillID_t const nID );

	// nIndex	:	ENUM_PET_SKILL_INDEX
	const _PET_SKILL	*Pet_Skill_Get( PET_GUID_t guidPet, INT nIndex ) ;

	INT 				Pet_Skill_GetCount_CortrolByPlayer(PET_GUID_t guidPet);
	INT 				Pet_Skill_GetMaxCount_CortrolByPlayer(PET_GUID_t guidPet);
	INT 				Pet_Skill_GetCount_CortrolByAI(PET_GUID_t guidPet);
	INT 				Pet_Skill_GetMaxCount_CortrolByAI(PET_GUID_t guidPet);

	BOOL 				Pet_Skill_Register(PET_GUID_t guidPet, SkillID_t const nID, INT nIndex);
	BOOL 				Pet_Skill_Unregister(PET_GUID_t guidPet, SkillID_t const nID, INT nIndex);
	BOOL 				Pet_Skill_Modify_Study(PET_GUID_t guidPet, SkillID_t nID, INT *pnOutSkillIndex);
	BOOL 				Pet_Skill_Modify_Realize(PET_GUID_t guidPet, SkillID_t nID);
	BOOL 				Pet_Skill_Modify_MenPai(PET_GUID_t guidPet, SkillID_t nID);
	BOOL 				Pet_Skill_Apperceive(PET_GUID_t guidPet);

//===========================================
// ����
//===========================================
public:
	INT							GetAttack();
	//StrikePoint
	INT 						GetMaxStrikePoint(VOID);
	
	//Attr1: Str
	INT							GetStr(VOID);
	//�õ��ֹ����������
	INT							GetBaseStr(VOID);
	INT							GetBaseStr2(VOID);	
	VOID 						SetBaseStr(INT const nValue);
	//Attr1: Con
	INT							GetCon(VOID);
	//�õ��ֹ����������
	INT							GetBaseCon(VOID);
	INT							GetBaseCon2(VOID);
	VOID						SetBaseCon(INT const nValue);
	//Attr1: Int
	INT							GetInt(VOID);
	//�õ��ֹ����������
	INT							GetBaseInt(VOID);
	//�õ�������(�ֹ�����+����Ӱ��)
	INT							GetBaseInt2(VOID);	
	VOID						SetBaseInt(INT const nValue);
	//Attr1: Dex
	INT							GetDex(VOID);
	//�õ��ֹ��������
	INT							GetBaseDex(VOID);
	INT							GetBaseDex2(VOID);	
	VOID						SetBaseDex(INT const nValue);
	//HP
	INT							GetMaxHP(VOID) ; 
	INT 						GetBaseMaxHP(VOID);
	VOID 						SetBaseMaxHP(INT const nHp);
	INT							GetHPRegenerate(VOID) ; 
	INT							GetBaseHPRegenerate(VOID);
	VOID 						SetBaseHPRegenerate(INT const nValue);

	//MP
	INT 						GetMaxMP(VOID);
	INT 						GetBaseMaxMP(VOID);
	VOID 						SetBaseMaxMp(INT const nMp);
	INT							GetMPRegenerate(VOID); 
	INT 						GetBaseMPRegenerate(VOID);
	VOID 						SetBaseMPRegenerate(INT const nValue);

	//Ԫ��
	INT 						GetMaxPneuma(VOID);
	INT 						GetBaseMaxPneuma(VOID);
	VOID 						SetBaseMaxPneuma(INT const nPneuma);
	INT							GetPneumaRegenerate(VOID); 
	INT 						GetBasePneumaRegenerate(VOID);
	VOID 						SetBasePneumaRegenerate(INT const nValue);

	//Rage
	INT 						GetMaxRage(VOID);
	INT 						GetBaseMaxRage(VOID);
	VOID 						SetBaseMaxRage(INT const nRage);
	INT							GetRageRegenerate(VOID); 
	INT 						GetBaseRageRegenerate(VOID);
	VOID 						SetBaseRageRegenerate(INT const nValue);
	//CampID
	virtual const _CAMP_DATA	*GetBaseCampData(VOID)const;
	virtual VOID				SetBaseCampData(const _CAMP_DATA *pCampData);
	INT 						GetCampID(VOID);
	INT 						GetBaseCampID(VOID) const;
	VOID						SetBaseCampID(INT const nID);
	//modelID
	INT							GetModelID(VOID) ;
	INT 						GetBaseModelID(VOID) const;
	VOID						SetBaseModelID(INT const nID);
	//mountID
	//Alive flag
	//In Combat flag
	// Can move flag
	// Can Action flag
	// Unbreakable flag
	virtual BOOL				IsUnbreakable(VOID);
	// Attr2: Move Speed
	FLOAT						GetMoveSpeed(VOID);
	INT							GetBaseMoveSpeed(VOID);
	VOID						SetBaseMoveSpeed(INT const nValue);
	// Attr2: Attack Speed
	INT 						GetAttackSpeed(VOID);
	INT 						GetBaseAttackSpeed(VOID);
	VOID						SetBaseAttackSpeed(INT const nValue);
	// Attr2: Miss
	INT 						GetMiss(VOID);

	INT							GetIattributeMiss();
	INT 						GetBaseMiss(VOID);
	VOID						SetBaseMiss(INT const nValue);
	// Attr2 Hit
	INT 						GetHit(VOID);

	INT							GetIattributeHit();
	INT 						GetBaseHit(VOID);
	VOID						SetBaseHit(INT const nValue);
	// Attr2 Critical
	INT 						GetCritical(VOID);

	INT							GetIattributeCritiacalRefix();
	INT							GetEquipCriticalHurt();
	INT							GetCriticalHurt();
	INT 						GetBaseCritical(VOID);
	VOID						SetBaseCritical(INT const nValue);
	// Attr2: Attack and Defence power physics
	//INT 						GetAttackPhysics(VOID);
	//INT 						GetBaseAttackPhysics(VOID);
	//VOID						SetBaseAttackPhysics(INT const nValue);
	//INT 						GetDefencePhysics(VOID);
	//INT 						GetBaseDefencePhysics(VOID);
	//VOID						SetBaseDefencePhysics(INT const nValue);

	//add by liam------------------------------------
	virtual INT 				GetDefenceNear(VOID);
	virtual INT 				GetBaseDefenceNear (VOID);
	virtual VOID				SetBaseDefenceNear (INT const nValue);

	virtual INT 				GetDefenceFar(VOID);
	virtual INT 				GetBaseDefenceFar (VOID);
	virtual VOID				SetBaseDefenceFar (INT const nValue);


	virtual INT 				GetAttackNear(VOID);
	virtual INT 				GetBaseAttackNear(VOID);
	virtual VOID				SetBaseAttackNear(INT const nValue);


	virtual INT 				GetAttackFar(VOID);
	virtual INT 				GetBaseAttackFar(VOID);
	virtual VOID				SetBaseAttackFar(INT const nValue);

	INT                         GetToughness(VOID);
	INT 						GetBaseToughness(VOID);
	VOID						SetBaseToughness(INT const nValue);


	virtual INT 				GetNearAttReduce(VOID);
	virtual INT 				GetBaseNearAttReduce(VOID);
	virtual VOID				SetBaseNearAttReduce(INT const nValue);

	virtual INT 				GetFarAttReduce(VOID);
	virtual INT 				GetBaseFarAttReduce(VOID);
	virtual VOID				SetBaseFarAttReduce(INT const nValue);

	virtual INT 				GetMagicNearAttReduce(VOID);
	virtual INT 				GetMagicFarAttReduce(VOID);

	virtual INT 				GetBaseMagicNearAttReduce(VOID);
	virtual INT 				GetBaseMagicFarAttReduce(VOID);

	virtual VOID				SetBaseMagicNearAttReduce(INT const nValue);
	virtual VOID				SetBaseMagicFarAttReduce(INT const nValue);

	virtual INT 				GetDreadAttResist(VOID);
	virtual INT 				GetBaseDreadAttResist(VOID);
	virtual VOID				SetBaseDreadAttResist(INT const nValue);

	virtual INT 				GetComaAttResist(VOID);
	virtual INT 				GetBaseComaAttResist(VOID);
	virtual VOID				SetBaseComaAttResist(INT const nValue);

	//��Ĭ����
	virtual INT 				GetHushAttResist(VOID);
	virtual INT 				GetBaseHushAttResist(VOID);
	virtual VOID				SetBaseHushAttResist(INT const nValue);

	//��е����
	virtual INT 				GetUnArmyAttResist(VOID);
	virtual INT 				GetBaseUnArmyAttResist(VOID);
	virtual VOID				SetBaseUnArmyAttResist(INT const nValue);

	//�����ٿ���
	virtual INT 				GetAttSpeedAttResist(VOID);
	virtual INT 				GetBaseAttSpeedAttResist(VOID);
	virtual VOID				SetBaseAttSpeedAttResist(INT const nValue);

	//�����ٿ���
	virtual INT 				GetSkillSpeedAttResist(VOID);
	virtual INT 				GetBaseSkillSpeedAttResist(VOID);
	virtual VOID				SetBaseSkillSpeedAttResist(INT const nValue);

	//�����ٿ���
	virtual INT 				GetMoveSpeedAttResist(VOID);
	virtual INT 				GetBaseMoveSpeedAttResist(VOID);
	virtual VOID				SetBaseMoveSpeedAttResist(INT const nValue);
	//-----------------------------------------------------------

	// Attr2: 
	INT 						GetAttackMagicNear(VOID);
	INT 						GetAttackMagicFar(VOID);

	INT 						GetBaseAttackMagicNear(VOID);
	INT 						GetBaseAttackMagicFar(VOID);

	VOID						SetBaseAttackMagicNear(INT const nValue);
	VOID						SetBaseAttackMagicFar(INT const nValue);

	INT 						GetDefenceMagicNear(VOID);
	INT 						GetDefenceMagicFar(VOID);

	INT 						GetBaseDefenceMagicNear(VOID);
	INT 						GetBaseDefenceMagicFar(VOID);

	VOID						SetBaseDefenceMagicNear(INT const nValue);
	VOID						SetBaseDefenceMagicFar(INT const nValue);
	
	// Attr2: Attack and Defence power of Gold
	INT 						GetAttackGold(VOID);
	INT 						GetBaseAttackGold(VOID);
	VOID						SetBaseAttackGold(INT const nValue);
	INT 						GetDefenceGold(VOID);
	INT 						GetBaseDefenceGold(VOID);
	VOID						SetBaseDefenceGold(INT const nValue);
	
	// Attr2: Attack and Defence power of Wood
	INT 						GetAttackWood(VOID);
	INT 						GetBaseAttackWood(VOID);
	VOID						SetBaseAttackWood(INT const nValue);
	INT 						GetDefenceWood(VOID);
	INT 						GetBaseDefenceWood(VOID);
	VOID						SetBaseDefenceWood(INT const nValue);

	// Attr2: Attack and Defence power of Water
	INT 						GetAttackWater(VOID);
	INT 						GetBaseAttackWater(VOID);
	VOID						SetBaseAttackWater(INT const nValue);
	INT 						GetDefenceWater(VOID);
	INT 						GetBaseDefenceWater(VOID);
	VOID						SetBaseDefenceWater(INT const nValue);

	// Attr2: Attack and Defence power of Fire
	INT 						GetAttackFire(VOID);
	INT 						GetBaseAttackFire(VOID);
	VOID						SetBaseAttackFire(INT const nValue);
	INT 						GetDefenceFire(VOID);
	INT 						GetBaseDefenceFire(VOID);
	VOID						SetBaseDefenceFire(INT const nValue);

	// Attr2: Attack and Defence power of Soil
	INT 						GetAttackSoil(VOID);
	INT 						GetBaseAttackSoil(VOID);
	VOID						SetBaseAttackSoil(INT const nValue);
	INT 						GetDefenceSoil(VOID);
	INT 						GetBaseDefenceSoil(VOID);
	VOID						SetBaseDefenceSoil(INT const nValue);

	// Attr2 Reduce Slower Duration
	INT 						GetReduceSlowerDuration(VOID);
	// Attr2 Reduce Weaken Duration
	INT 						GetReduceWeakenDuration(VOID);
	// Attr2 Reduce Faint Duration
	INT 						GetReduceFaintDuration(VOID);
	// Attr2 Reduce Poisoned Duration
	INT 						GetReducePoisonedDuration(VOID);
	// Attr2 VisionRange
	INT 						GetVisionRange(VOID);
	INT 						GetBaseVisionRange(VOID);
	VOID						SetBaseVisionRange(INT const nValue);

	INT							GetMountID(VOID);
	INT                         GetMountIndex(VOID);	 //  [7/23/2010 �¾���]                               //[7/23/2010 �¾���]

//===========================================
// _���Խӿ�
//===========================================
public:
	virtual const INT			__GetSex( ) const ;//�Ա� human
	virtual VOID				__SetSex( const INT sex ) ;
	virtual const UINT			__GetCreateDate( ) const ;//����ʱ�� human
	virtual VOID				__SetCreateDate( const UINT createdate ) ;
	virtual const CHAR*			__GetNick( ) const ;//�ǳ� human
	virtual VOID				__SetNick( const CHAR* nick ) ;
	virtual VOID				__SetPasswordProtect(INT nType);
	virtual VOID				__SetProtectFlag(BOOL bFlag);
    virtual BOOL                __GetProtectFlag();
	virtual BOOL				__SetProtectTime(CHAR cTime);
	virtual CHAR				__GetProtectTime();
    virtual BOOL				__IsPasswordProtect(MINORPASSWD_INFO::PROTECT_TYPE nType);
	virtual BOOL				__IsPasswordSetup();
	virtual BOOL				__IsPasswordUnlock();
	virtual VOID				__LockPassword(); // �������������������
	virtual VOID				__UnlockPassword(); // �������������ȷ
	virtual const CHAR*			__GetPassword( );//�������� human
	virtual BOOL				__SetPassword( const CHAR* password );
	virtual VOID				__DelPassword( ); // ɾ����������
	virtual UINT				__GetPasswordDeleteTime( ); // ǿ�ƽ�����������ʱ��
	virtual VOID				__SetPasswordDeleteTime( UINT uTime ); // ����ǿ�ƽ�����������ʱ��
	virtual UINT				__GetPasswordDeleteRemainTime( ); // �õ�ǿ�ƽ�������ʣ��ʱ��
	virtual BOOL				__IsValidToDeletePassword( ); // �Ƿ�����ǿ�ƽ��������������
	virtual UINT				__GetHairColor( ) const ;//ͷ����ɫ human
	virtual VOID				__SetHairColor( UINT haircolor ) ;
	virtual BYTE				__GetFaceColor( ) const ;//����ɫ human
	virtual VOID				__SetFaceColor( BYTE facecolor ) ;
	virtual BYTE				__GetHairModel( ) const ;//ͷ��ģ�� human
	virtual VOID				__SetHairModel( BYTE hairmodel ) ;
	virtual BYTE				__GetFaceModel( ) const ;//��ģ�� human
	virtual VOID				__SetFaceModel( BYTE facemodel ) ;
	virtual const UINT			__GetOnlineTime( ) const ;//����ʱ�� human
	virtual VOID				__SetOnlineTime( const UINT onlinetime ) ;
	virtual const UINT			__GetLastLevelUpTime( ) const ;//�ϴ�����ʱ�� human
	virtual VOID				__SetLastLevelUpTime( const UINT lastleveluptime ) ;
	virtual const UINT			__GetLastLoginTime( ) const ;//�ϴε�¼ʱ�� human
	virtual VOID				__SetLastLoginTime( const UINT lastlogintime ) ;
	virtual const UINT			__GetLastLogoutTime( ) const ;//�ϴεǳ�ʱ�� human
	virtual VOID				__SetLastLogoutTime( const UINT lastlogouttime ) ;
	virtual const INT			__GetBagItemCount( ) const ;//��������Ʒ���� human
	virtual VOID				__SetBagItemCount( const INT bagitemcount ) ;
	virtual const _ITEM*		__GetBagItem( const INT bagindex ) const ;//������ĳ��λ�õ���Ʒ human
	virtual VOID				__SetBagItem( const INT bagindex, const _ITEM* item ) ;
	virtual const INT			__GetBankItemCount( ) const ;//��������Ʒ���� human
	virtual VOID				__SetBankItemCount( const INT Bankitemcount ) ;
	virtual const _ITEM*		__GetBankItem( const INT bankindex ) const ;//������ĳ��λ�õ���Ʒ human
	virtual VOID				__SetBankItem( const INT bankindex, const _ITEM* item ) ;
	virtual const INT			__GetBankEndIndex( ) const ;//��ǰ���еĴ�С
	virtual VOID				__SetBankEndIndex( const INT bankindex ) ;
	virtual const INT			__GetBankMoney( ) const ;//��ǰ�����ڵ�Ǯ��
	virtual VOID				__SetBankMoney( const INT money ) ;
    virtual const INT			__GetBankSize( ) const ;//��ǰ���и�����
    virtual VOID				__ExpandBankSize( const INT size ) ;//��չ���и���

	virtual const INT			__GetEquipItemCount( ) const ;//װ������ human
	virtual VOID				__SetEquipItemCount( const INT equipitemcount ) ;
	virtual const _ITEM*		__GetEquipItem( const INT equipindex ) const ;//����ĳ��λ�õ�װ�� human
	virtual VOID				__SetEquipItem( const INT equipindex, const _ITEM* item ) ;
	virtual const INT			__GetMissionCount( ) const ;//���ܵ��������� human
	virtual VOID				__SetMissionCount( const INT missioncount ) ;
	virtual const _OWN_MISSION* __GetMission( const INT missionindex ) const ;//ĳ���������� human
	virtual VOID				__SetMission( const INT missionindex, _OWN_MISSION* mission ) ;
	virtual const BOOL			__IsMissionHaveDone( MissionID_t missionid ) const ;//�ж�ĳ�������Ƿ���� human
	virtual VOID				__SetMissionDoneFalg( MissionID_t missionid, BOOL flag ) ;
	virtual const BOOL			__IsAbilityHaveLearned(AbilityID_t abilityid) const; // ĳ��������Ƿ�ѧ��
	virtual const INT			__GetAbilityLevel( const AbilityID_t abilityid ) const ;//ĳ������ܼ��� human
	virtual VOID				__SetAbilityLevel( const AbilityID_t abilityid, INT lvl) ;
	virtual const INT			__GetAbilityExp( const AbilityID_t abilityid ) const ;//ĳ������������� human
	virtual VOID				__SetAbilityExp( const AbilityID_t abilityid, INT exp) ;
	virtual const BOOL			__IsPrescrHaveLearned( const PrescriptionID_t prescrid ) const;//ĳ���䷽�Ƿ�ѧ�� human
	virtual VOID				__SetPrescrLearnedFlag( const PrescriptionID_t prescrid, BOOL flag ) ;
	virtual BOOL				__IsCanLearnAbility( const AbilityID_t abilityid) ;//add by gh 2010/07/19 �Ƿ���ѧϰ�����
	virtual const _ITEM_EFFECT* __GetItemEffect( const INT index ) const ;//װ��Ч�� human
	virtual VOID				__SetItemEffect( const INT index, _ITEM_EFFECT* itemeffect ) ;
	virtual const _ITEM_VALUE	__GetItemValue( const INT index ) const ;//װ��Ч��ֵ human
	virtual VOID				__SetItemValue( const INT index, _ITEM_VALUE value) ;
	virtual const UINT			__GetLastTurnPickTime()	const;
	virtual	VOID				__SetLastTurnPickTime(UINT uTime)	;
	virtual const TeamID_t		__GetTeamID( ) const ;//����� human
	virtual VOID				__SetTeamID( const TeamID_t teamid ) ;
	virtual BOOL				__IsTeamLeader( ) const; // �Ƿ�ӳ�
	virtual const INT			__GetTeamMemberCount( ) const ;//�����г�Ա���� human
	virtual const TEAMMEMBER*	__GetTeamMember( const INT memberindex ) const ;//ȡ�ö�����ĳ����Ա
	virtual VOID				__AddTeamMember( const TEAMMEMBER* member ) ;
	virtual const INT			__GetTeamSceneMemberCount( ) const ;//������ͬ������Ա���� human
	virtual const ObjID_t		__GetTeamSceneMember( const INT memberindex ) const ;//ȡ�ö�����ĳͬ������Ա�� OBJID
	virtual VOID				__SetTeamFollowFlag( BOOL flag ); // ���ö������״̬
	virtual const BOOL			__GetTeamFollowFlag( ) const; // ��ö������״̬
	virtual VOID				__AddFollowedMember( const _FOLLOWEDMEMBER& FollowedMember ); // ����һ������Ķ�Ա
	virtual VOID				__DelFollowedMember( GUID_t guid ); // �Ƴ�һ�������Ա
	virtual const INT			__GetFollowedMembersCount() const; // �õ�����״̬�Ķ�������
	virtual const _FOLLOWEDMEMBER*	__GetFollowedMember( INT i ) const; // �õ��� i ������Ķ���
	virtual VOID				__ClearFollowedMembers(); // ������еĸ�������б�
	virtual const FLOAT			__GetTeamFollowSpeed( ) const; // �õ���Ӹ�����ٶ�
	virtual VOID				__SetTeamFollowSpeed( FLOAT fSpeed ); // ������Ӹ�����ٶ�
	virtual VOID				__OutOfTeamFollowRange(); // ������Ӹ��淶Χ
	virtual VOID				__InTeamFollowRange(); // ����Ӹ��淶Χ��
	virtual const BOOL			__GetTeamFollowSpeedUp( ) const; // �����Ӹ������״̬
	virtual VOID				__SetTeamFollowSpeedUp( BOOL bSpeedUp); // ������Ӹ������״̬
	virtual VOID				__StopTeamFollow( BOOL bWorldFlag=TRUE ); // ֹͣ����,flag��ʾ�Ƿ�֪ͨ world
};


#endif
