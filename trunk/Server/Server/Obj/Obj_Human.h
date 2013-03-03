//***************************************************************************************************
// 文件名称:	Obj_Human.h
// 系统名称:	
// 模块名称:	Obj_Human
// 功能说明:	玩家在SERVER上的实现类
//
// 相关文档:	
// 作    者:	liam
// 建立日期:	
// 审    核:	
// 软件版权:	
//
//修改记录:
//          1.
//				修 改 人:liam
//				修改时间:2007/10/30
//				修改内容:属性第一部分重构(具体参见文档)
//          2.
//				修 改 人:liam
//				修改时间:2007/11/02
//				修改内容:属性第二部分重构(具体参见文档)
//***************************************************************************************************

#ifndef __HUMAN_H__
#define __HUMAN_H__

#include "Obj_Character.h"
#include "HumanDB.h"
#include "HumanItemContainer.h"
#include "HumanItemExtraContainer.h"
#include "HumanItemExtraBag.h"
#include "HumanPetContainer.h"
#include "HumanMountContainer.h"    // [7/8/2010 陈军龙]
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
// 初始化Obj_Human的结构
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
// 与客户端同步队友信息数据
//////////////////////////////////////////////////////
struct _TEAM_MEMBER_INFO
{
	_TEAM_MEMBER_INFO()
	{
		memset(this, 0, sizeof(_TEAM_MEMBER_INFO));
	}

	UINT					m_uFamily;						// 1.门派
	UINT					m_uLevel;						// 2.等级
	WORLD_POS				m_WorldPos;						// 3.位置（坐标）
	INT						m_nHP;							// 4.HP
	UINT					m_uMaxHP;						// 5.HP上限
	INT						m_nMP;							// 6.MP
	UINT					m_uMaxMP;						// 7.MP上限
	INT						m_nAnger;						// 8.怒气
	UINT					m_WeaponID;						// 9.武器
	//UINT					m_AssihandID;					// 副武器
	UINT					m_CapID;						// 10.帽子
	//UINT					m_ScapularID;					// 肩甲
	UINT					m_ArmourID;						// 11.衣服
	UINT					m_CuffID;						// 12.护腕
	UINT					m_FootID;						// 13.靴子
	// 14.buff，暂时不考虑
	BOOL					m_bDeadFlag;					// 15.是否死亡
	UINT					m_uFaceMeshID;					// 16.面部模型
	UINT					m_uHairMeshID;					// 17.头发模型
	UINT					m_uHairColor;					// 18.头发颜色
	SimpleImpactList		m_SimpleImpactList;				// Buff 列表
};

//////////////////////////////////////////////////////
// 与客户端同步备份数据
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
	INT					m_nPneuma;			//元气
	INT					m_MaxHp;
	INT					m_MaxMp;
	INT					m_MaxPneuma;
	FLOAT       		m_MoveSpeed;
	INT					m_nDataID;
	INT					m_nPortraitID;
	INT					m_nModelID;
	INT					m_nMountID;			// 座骑的编号
	INT					m_nMountIndex;		// 座骑的索引   [7/23/2010 陈军龙]
	UINT				m_uFaceMeshID;		// 面部模型
	UINT				m_uHairMeshID;		// 头发模型
	UINT				m_uHairColor;		// 头发颜色
	INT					m_nStealthLevel;	// 隐身级别
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
	INT					m_nReVigorSpeed;		//活力恢复值
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

	INT					m_nNearReduce;		//近程减免
	INT					m_nFarReduce;		//远程减免
	
	INT					m_nMagicNearReduce;	//近程内功减免
	INT					m_nMagicFarReduce;	//近程外功减免

	INT					m_nDreadReduce;		//抵抗恐惧
	INT					m_nComaReduce;		//抵抗昏迷
	INT					m_nHushReduce;		//抵抗沉默
	INT					m_nUnarmReduce;		//抵抗缴械
	INT					m_nAttSpeedResist;	//抗攻击减速
	INT					m_nSkillSpeedResist;//抗施法
	INT					m_nMoveSpeedResist;	//抗减速

	INT                 m_nCountry;
	INT					m_nMenpai;
	_CAMP_DATA			m_CampData;
	ObjID_t				m_nOwnerID;
	PET_GUID_t			m_guidCurrentHorse;
	INT					m_Vigor ;
	INT					m_MaxVigor ;
	INT					m_Energy;
	INT					m_MaxEnergy;
	BOOL				m_bLimitMove;		//是否限制不能移动
	BOOL				m_bCanActionFlag1;	//技能限制标记1
	BOOL				m_bCanActionFlag2;	//技能限制标记2
	BOOL				m_bMissionHaveDoneFlagsChanged;
	BOOL				m_bMainWeaponModified;		//主手
	BOOL				m_bAssiWeaponModified;		//副手
	BOOL				m_bCapModified;				//帽子
	BOOL				m_bShoulderModified;		//衬肩
	BOOL				m_bArmorModified;			//衣服
	BOOL				m_bCuffModified;			//护腕
	BOOL				m_bBootModified;
	BOOL				m_bSuitModified;			// 外装 - 资源表ID
	BOOL				m_bRiderModified;			// 座骑 - 资源表ID
	BOOL				m_bStallIsOpen;
	BOOL				m_bStallNameChanged;

	BOOL				m_bStallIsPublic;			// 是否开市(公开显示摊位牌给其他玩家)  20100706 BLL
	GuildID_t			m_GuildID;		            //	玩家所在帮会
	CHAR			    m_cFamilyID;	            //	家族
	CHAR	    		m_cGuildOfficial;		    //	帮会官职
    INT                 m_nGP;                      //  帮会贡献度
    INT                 m_nHonor;                   //  荣誉值
    INT                 m_nPK;                      //  PK值
};

//////////////////////////////////////////////////////
// 生活技能操作数据
//////////////////////////////////////////////////////
struct AbilityOpera
{
	AbilityID_t			m_AbilityID;
	BYTE				m_SubType;	// 加工包括普通、宝石合成、镶嵌
	PrescriptionID_t	m_PresID;
	ObjID_t				m_Obj;
	INT					m_nMaxTime;
	INT					m_nCurTime;
	INT					m_nRobotTime;
	BYTE				m_BagPos[MAX_ABILITY_ITEM];
	UINT				m_uItemIndex; // 保存合成过程中需要的物品信息（例如宝石合成）

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
// Obj公有接口继承
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

	//获取最近可掉落的点(dropType代表类型)
	virtual BOOL GetNearCanDropPos(WORLD_POS& DropPos);
//===========================================
// Character公有接口继承
//===========================================
public:
	virtual VOID InitAIObj( );

	virtual VOID OnKillObject( ObjID_t idObj );
	virtual VOID OnItemChanged( UINT uItemDataID );
	virtual VOID OnEnterArea( UINT uAreaID );
	virtual VOID OnPetChanged( UINT uPetDataID ) ;
	virtual VOID OnDie( ObjID_t idKiller );

//===========================================
// 消息/属性刷新
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
	//为技能同步，实时发送CharBase消息，并清除HP脏标记，组织自动刷属性时重复发送
	virtual VOID SendCharBaseDirectForHP();

protected:
	_HUMAN_ATTR_BACKUP	m_AttrBackUp;

//===========================================
//  复活点
//===========================================
public:
	// 得到复活信息
	// bSkillRelive		:	是否为技能复活
	virtual const _RELIVE_INFO		*GetReliveInfo( BOOL bSkillRelive )const;

	// 设置复活属性
	// bSkillRelive		:	是否为技能复活
	// pReliveInfo		:	复活信息
	virtual VOID			SetReliveInfo( BOOL bSkillRelive, const _RELIVE_INFO *pReliveInfo );

protected:
	// 复活信息改变的事件(如：玩家对死去的玩家使用复活技能)
	VOID					OnReliveInfoChanged( VOID );

protected:
	_RELIVE_INFO			m_DefaultReliveInfo;		// 缺省复活的信息
	

//===========================================
//  邀请的处理
//===========================================
public:
	// 得到邀请信息
	const _CALLOF_INFO		*GetCallOfInfo(VOID)const;

	VOID					ResetCallOfInfo(VOID);

	// 设置邀请属性
	// pCallOfInfo		:	邀请信息
	VOID					SetCallOfInfo(const _CALLOF_INFO *pCallOfInfo);

	// 拉人
	VOID					CallOfHuman(GUID_t guidTarget, UINT uDuration);

protected:
	// 邀请信息改变的事件
	VOID					OnCallOfInfoChanged( VOID );

protected:
	_CALLOF_INFO			m_CallOfInfo;			// 邀请的信息
	UINT					m_uCallOf_StartTime;	// 邀请信息的起始时间

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
	ObjID_t					m_AssistantID;		//辅助攻击者

//===========================================
// 属性
//===========================================
public:
	BOOL					IsFirstEnterScene();
	VOID					ClearFirstEnterScene();

	virtual UINT			GetUniqueID(VOID) const;
	//DB接口
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

	VOID					SetAlterExp( INT AlterExp );	//改变的经验
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
	
	INT						Get_RemainSkillPoints();	//剩余技能点
	VOID					Set_RemainSkillPoints(INT Point);	//剩余技能点
	
	INT						Get_S_SkillPoints(INT nS);	//技能点
	VOID					Set_S_SkillPoints(INT nS, INT Point);	//余技能点

	VOID					ValidatePlayerID();

	UINT					GetEquipID(HUMAN_EQUIP equipPoint);
	UINT					GetEquipLevel(HUMAN_EQUIP equipPoint);
	//称号		
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

    //上限保护时间
    VOID                    SetLoginProtectTime(time_t time);
    BOOL                    IsInLoginProtectTime();

    private:
    time_t                  m_TitleLoginTime;                    //记录玩家上线的时间点，计时称号用
    time_t                  m_LoginProtectTime;

    public:
	//一键换装
	const _SUIT_SETTING& 	GetEquipSuitSetting( UINT nEquipSuitNum ) const;
	virtual VOID			SetEquipSuitSetting( _SUIT_SETTING& nSuitSetting, UINT nEquipSuitNum );

	const _SUBTRAIN_SETTING& GetSubTrainSetting( ) const;
	virtual VOID			SetSubTrainSetting( _SUBTRAIN_SETTING& nSuitSetting);
	// GM专用
	VOID					SetGod( BYTE bGod );
	BOOL					IsGod(VOID)const;

	VOID					SetChangeSceneFlag( BYTE bChangeSceneFlag );
	BOOL					IsHaveChangeSceneFlag(VOID) const;

	VOID					UpdateSubTrainToClient();
protected:
	//存放所有关于Obj_Human的、从数据库里读取的信息
	HumanDB					m_DB;
	BYTE					m_bChangeSceneFlag;
	BYTE					m_bGod;

//===========================================
// 技能
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


	
	// 添加或设置一个技能
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

	VOID					ValidateShareMem(BOOL bForceAll); //HeartBeat 最后更新ShareMemory
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
	BOOL					m_bCanIncreaseRage;					//门派特性专用
	Time_t					m_nCommonCooldown;					//公共冷却时间

//===========================================
// 效果
//===========================================
public:

protected:
	virtual _IMPACT_LIST&	Impact_GetImpactList(VOID);

private:
	VOID 					Impact_InitList(VOID);

//===========================================
// 道具
//===========================================
public:
	//物品部分外部逻辑接口
	BOOL					IsVisualPart(HUMAN_EQUIP EquipPoint);						//是否其他玩家可见部分
	BOOL					ActivateItem(VOID);											//使用物品
	INT						UseStoreMap(uint BagIndex);									//使用藏宝图
	INT						UseIdentScroll(UINT BagIndex,UINT TargetBagIndex);			//使用鉴定卷轴
	INT						UseGem(Item* pEquipItem,uint GemBagIndex, UINT GemHolePos);//使用Gem
	INT						RemoveGem(uint BagIndex,uint GemIndex);						//移出Gem
	INT						AddHole(Item* pEquipItem);
	INT						EquipLevelup(Item* pEquipItem, UINT nGemID, UINT nGemCount);
	INT						EquipRemould(Item* pEquipItem, UINT nMaterialScore);
	INT						EquipBind(Item* pEquipItem,uint GemBagIndex);
	INT						EquipChangePai(Item* pEquipItem, uint GemBagIndex);
	INT						EquipChangePai(Item* pEquip1Item, Item* pEquip2Item, uint GemBagIndex);
	INT						EquipAdsorbMagic(Item* pEquipItem, Item* pBeadItem);		//附魔
	INT						EquipCancelMagic(Item* pEquipItem);							//退魔
	BOOL					UseItem(ID_t nBagIndex, ObjID_t nTargetObj, const WORLD_POS& posTarget, PET_GUID_t const& guidTargetPet, ID_t nTargetItem);//使用物品的功能
	INT						UseEquip(INT BagIndex);
	BOOL 					VerifyItem(VOID); //校验使用中的物品是否存在
	virtual VOID			UpdataEquipDamagePoint(INT nType);	//更新装备损伤点
	virtual	BOOL			IsCarrayItem(UINT ItemType,UINT ItemCount);
	/*
	*	物品背包容器
	*/
	//杂项物品包
	ItemContainer*	GetBaseContain();
	
	ItemContainer*	 GetExtraContain();

	ItemContainer*	 GetExtraBagContain(INT nIndex);
	////任务物品包
	//ItemContainer*	GetTaskContain();
	////材料物品包
	//ItemContainer*	GetMatContain();
	//装备
	ItemContainer*	GetEquipContain();
	//银行
	ItemContainer*	GetBankContain();
	//宠物
	ItemContainer*	GetPetContain();
    //坐骑 [7/8/2010 陈军龙]
	ItemContainer*  GetMountContain();

	//根据身上的装备，计算影响因素
	//如果身上的装备发生变化，则需要调用一次此接口，重新计算特殊效果
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

	//装备的各个影响因素
	_ITEM_EFFECT*		ItemEffect( BYTE AttrType ){//AttrType=enum ITEM_ATTRIBUTE 
		Assert( AttrType<IATTRIBUTE_NUMBER );
		return &m_pItemEffect[AttrType];
	}

	virtual _ITEM_VALUE	ItemValue( BYTE AttrType ){//AttrType=enum ITEM_ATTRIBUTE 
		Assert( AttrType<IATTRIBUTE_NUMBER );
		return m_pItemEffect[AttrType].m_Attr;
	}

	VOID					CheckAllAbility(); //生活技能检查
private:
	//由物品引起的各种效果
	_ITEM_EFFECT		m_pItemEffect[IATTRIBUTE_NUMBER];

	//装备影响的技能
	INT						m_EffectSkillNum;
	_ITEM_EFFECT_SKILL		m_EquipEffectSkill[HEQUIP_NUMBER*MAX_EQUIP_EFFECT_SKILL];

	//装备附加的效果
	INT						m_EquipImpactNum;
	_ITEM_IMPACT			m_EquipImpactEffect[HEQUIP_NUMBER+MAX_ITEM_SET_ATTR];

private:
	//基本杂项物品包
	HumanItemContainer		m_BaseItem;	
	
	//扩展栏
	HumanItemExtraContainer m_ExtraContainerItem;
	
	//扩展物品包
	HumanItemExtraBag		m_ExtraBagItem[MAX_EXTRA_BAG_NUM];	

	
	////任务物品包
	//HumanItemContainer		m_TaskItem;
	////材料物品
	//HumanItemContainer		m_MatItem;
	//角色装备				
	HumanItemContainer		m_EquipItem;
	//角色银行
	HumanItemContainer		m_BankItem;
	//宠物背包
	HumanPetContainer		m_PetItem;
	//坐骑 [7/8/2010 陈军龙]
	HumanMountContainer     m_MountItem;

//===========================================
// 生活技能
//===========================================
public:
	const _ABILITY_LIST*	GetAbilityList( ) const;
	AbilityOpera*			GetAbilityOpera( );
	VOID					reset_AbilityOpera( ); 
	VOID					InterruptCurrentAbilityOpera();

	// 获取玩家进入事件区域的时间
	UINT GetAreaTimeStamp()
	{
		return m_uTimeStamp;
	}

	// 重新设置玩家进入事件区域的时间（作为下一次触发事件的时间戳）
	VOID ResetAreaTimeStamp(UINT newTimeStamp)
	{
		m_uTimeStamp = newTimeStamp;
	}

protected:
	virtual	ORESULT			Do_UseAbility( VOID );				//进入UseAbility逻辑
	virtual	BOOL			Obj_Logic_UseAbility( UINT uTime );			//执行UseAbility逻辑
	virtual VOID			OnCharacterLogicStopped(BOOL bAbortive);

private:
	AbilityOpera			m_LastOpera;
	//当经验值增加时，对应送经验的怪物ID;
	ObjID_t					m_iTargetForExp;

//===========================================
// 任务
//===========================================
public:
	const _MISSION_DB_LOAD *GetMissionList( VOID );
	const _MISSION_DB_LOAD *GetConstMissionList( VOID )const;
	//const UINT *GetMissionHaveDoneFlags( VOID )const;

	// 设置是否已经做过了某个任务
	VOID SetMissionHaveDone( MissionID_t idMission, BOOL bHaveDone );

	// 是否已经做过了某个任务
	BOOL IsMissionHaveDone( MissionID_t idMission )const;

	// 加一个任务入任务列表
	ORESULT AddMission( MissionID_t idMission, ScriptID_t idScript, BOOL bKillObjectEvent = TRUE, BOOL bEnterAreaEvent = TRUE, BOOL bItemChangedEvent = TRUE );
	ORESULT AddMissionEx( MissionID_t idMission, ScriptID_t idScript ) ;
	ORESULT SetMissionEvent( MissionID_t idMission, INT EventID/*TASK_EVENT*/ ) ;
	//ORESULT AddMissionKillObjectNotify( MissionID_t idMission, ScriptID_t idScript,INT killObject0,INT killObject1,INT killObject2,INT killObject3);
	//ORESULT AddMissionEnterAreaNotify( MissionID_t idMission, ScriptID_t idScript,INT areaId0,INT areaId1,INT areaId2,INT areaId3);
	//ORESULT AddMissionItemChangedNotify( MissionID_t idMission, ScriptID_t idScript,INT itemTypeSn0,INT itemTypeSn1,INT itemTypeSn2,INT itemTypeSn3);

	// 从任务列表中删除一个任务
	VOID DelMission( MissionID_t idMission );
	VOID DelMissionByIndex( UINT uIndex );

	// 通过任务ID取该任务在任务表表的索引值， 无该任务时返回UINT_MAX
	UINT GetMissionIndexByID( MissionID_t idMission )const;

	// 检测是否已经有了这个任务
	BOOL IsHaveMission( MissionID_t idMission )const;

	// 设置任务参数
	VOID SetMissionParam( UINT uIndexMission, UINT uIndexParam, INT nValue );

	// 取任务参数
	INT GetMissionParam( UINT uIndexMission, UINT uIndexParam )const;
	const _OWN_MISSION *GetMission( UINT uIndex )const;

	//UINT GetCanPickMissionItemCount( VOID )const;
	//const UINT *GetCanPickMissionItemList( VOID )const;
	//VOID AddCanPickMissionItem( UINT uItemDataID );
	//VOID RemoveCanPickMissionItem( UINT uItemDataID );

	//读取附加任务数据
	INT GetMissionData( INT nIndex );

	//设置当前附加任务数据
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
	// 这里一定要重点优化（杀死OBJECT的事件，得到和删除物品的事件，以及区域切换的事件）
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
// 队伍/联系人
//===========================================
public:
	TeamInfo*		GetTeamInfo();
	HumanRelation*	GetHumanRelation();

private :
	FLOAT					m_fTeamFollowSpeed;
	BOOL					m_bTeamFollowSpeedUp;
	time_t					m_OutofTeamFollowRangeTime;
	_TEAM_MEMBER_INFO		m_SyncTeamMemberInfo;
	TeamInfo				m_TeamInfo;			//队伍相关数据
	HumanRelation			m_HumanRelation;	//联系人相关数据

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
// 摆摊/交易/银行/商店
//===========================================
public:
	//摆摊
	VOID				StallNameChanged()	{m_AttrBackUp.m_bStallNameChanged	=	TRUE;}
	BOOL				GetStallIsOpen() { return m_StallBox.GetStallIsOpen(); }
	ServerStallBox		m_StallBox;

	//银行
	BOOL				IsBankOpen(){return BankIsOpen;}
	VOID				SetBankOpen(BOOL bopen){BankIsOpen = bopen;}

	//商店
	VOID				InitialSoldList();
	VOID				AddToSoldList(INT index, UINT uPrice, UINT uCount);
    VOID                GetSlodListInfo(GCShopSoldList::_MERCHANDISE_ITEM *pItem);
	BOOL				GetCurFromSoldList(_ITEM& itemref, UINT& uPrice );
	BOOL				GetFromSoldListByIndex(INT iIndex, _ITEM& itemref, UINT& uPrice);
	BOOL				RecieveOneFromSoldList(INT iIndex);
	VOID				DeleteFromSoldList();
	VOID				ClearSoldList();

	// 20100413 AddCodeBegin
	VOID				ReorderSoldList();	// 重新整理回购物品列表，去除中间的空物品格
	// 20100413 AddCodeEnd

	//商人买卖用的
	VOID				Shop( _SHOP* pShop){m_pShop = pShop;};
	_SHOP*				Shop( ){return m_pShop;};

public:
	//交易
	ServerExchangeBox	m_ExchangBox;//设为公有，因为需要交易对方的频繁访问。

	//开宝箱过程中，暂存物品的容器
private:
	HumanOpenBox	m_HumanOpenBox;

public:
	//by zheng 
	//开宝箱时所使用容器
	HumanOpenBox* GetHumanOpenBox();


private:
	//银行
	BOOL				BankIsOpen;

	//商店
	ItemContainer		m_SoldContainer;							//卖出物品的container;
	UINT				m_SoldPrice[MAX_BOOTH_SOLD_NUMBER];			//出售价格;
	_ITEM				m_ItemHasBeenSold[MAX_BOOTH_SOLD_NUMBER]; 
	INT					m_nCurSold;
	ObjID_t				m_CurTargetID;

	//存放与此人发生买卖的当前NPC指针
	_SHOP*				m_pShop;
    MINORPASSWD_INFO    m_PWProtect;                                //密码保护

//===========================================
// Other
//===========================================
public:
	VOID ChangeScene( SceneID_t sSceneID, SceneID_t dSceneID, const WORLD_POS& pos, BYTE uDir ) ;
	VOID CreateCity( SceneID_t PortSceneID) ;
	VOID DeleteCity( SceneID_t PortSceneID, INT iSceneIndex) ;
	VOID ChangeCityBuilding( SceneID_t SceneID, INT BuildingID, INT BuildingLevel) ;
	// 比较二级密码
	BOOL ComparePasswd( const CHAR* szPasswd );

	PlayerID_t				GetPlayerID( );

	Player*					GetPlayer(){ return m_pPlayer; }
	VOID					SetPlayer( Player* pPlayer ){ m_pPlayer = pPlayer; }
	VOID 					Levelup(VOID); //升级的相关属性操作
	VOID					ChangeMenpaiPoints(VOID); //转换门派时点数从新计算
	VOID 					WashPoints(VOID);//洗点的相关操作
	BOOL 					ManualAttrPoint(CHAR_ATTR_LEVEL1 attrType);//手动加点
	BOOL 					ManualAttrPoint(INT nStr, INT nCon, INT nInt, INT nDex);//手动加点
	VOID					ChangeMenpai(SKILL_LOG_PARAM* pSkillLog,XINFA_LOG_PARAM* pXinFaLog,INT Menpai);
	const _SETTING_DB_LOAD *GetSettingList( VOID );
	const _SETTING_DB_LOAD *GetConstSettingList( VOID )const;

	VOID 					SendOperateResultMsg(INT nCode);
	VOID					RefreshAttrToWorld(UINT uTime);	//向GLServer刷新数据

protected:

	BOOL						m_bIsPasswdUnlock;	// 密码解锁标记
	BOOL						m_bUpdateAttr;		//更新非即时信息
	CMyTimer					m_UpdateTimer;

	CMyTimer					m_VERecoverTimer;	// 活力与精力的恢复心跳

	CMyTimer					m_RefreshAttrToWorldTimer;	//向GLServer刷新数据的定时器

public:
	//////////////////////////////////////////////////////////////////////////
	//人物在地图上的注册节点
	_OBJ_LIST_NODE*			HumanNode(){ return m_pHumanNode; }
	BYTE					getMoveIndex( ){ return m_MoveIndex; }
	VOID					setMoveIndex( BYTE index ){ m_MoveIndex = index; }

	//向自己发送新看到的pObj的信息
	//VOID					SendPacket_NewViewObj( Obj* pObj );

	SceneID_t			GetTransSceneID( );
	VOID				SetTransSceneID( SceneID_t id );

private :
	PlayerID_t			m_PlayerID;
	Player*				m_pPlayer;

	//玩家节点信息，用于注册在Zone的HumanList里的
	_OBJ_LIST_NODE*		m_pHumanNode;

	BYTE				m_MoveIndex;	//客户端的发送移动索引号

	WORD				m_EquipVer;
	SceneID_t			m_TransSceneID;

protected:
	CMyTimer			m_CheckZoneTimer; // 对 Zone 进行检查（Area etc...）的计时器
	UINT				m_uAreaID;
	ScriptID_t			m_uAreaScript;
	UINT				m_uTimeStamp;
	UINT				m_uLastTurnPickTime; //最后一次拾取时间

protected:
	VOID				CheckArea();
	VOID				RefeshAttrMsg() ;		//刷新角色的自动增长数据

protected:
	CMyTimer			m_RefeshTimer;

public:
	BOOL				GetPosition(WORLD_POS& Pos, const WORLD_POS* pRefPos, FLOAT fDir);

//===========================================
// 宠物
//===========================================
public:
	//测试宠物是否可以被招出
	ORESULT				TestCallUpPet(const PET_GUID_t guidPet);
	ORESULT				CallUpPet(VOID);

	ORESULT				ReCallPet( INT iIndex );

	//收回宠物
	ORESULT				ReCallPet(VOID);
	//从玩家身上清除宠物
	ORESULT				FreePetToNature(PET_LOG_PARAM* pPetLogParam,PET_GUID_t guidPet);
	ORESULT				AddPetByDataID(PET_LOG_PARAM* pPetLogParam,const UINT uDataID);

	VOID				BabyToAttack(VOID);
	//当主人受到攻击时回调
	//ObjID_t iObjID 敌人ID
	VOID				BabyToProtectMe( ObjID_t iObjID );
	BOOL				CaptureHorse(PET_LOG_PARAM* pPetLogParam, ObjID_t PetID );
	Obj_Pet*			GetPet( INT iIndex ) const;
	BOOL				CanHorseReturnToChild(PET_GUID_t guid); // 是否可以还童
	BOOL				HorseReturnToChild(PET_GUID_t guid); // 还童接口

	VOID				SetGUIDOfCallUpPet(PET_GUID_t guid) { m_GUIDOfCallUpPet = guid; }
	PET_GUID_t			GetGUIDOfCallUpPet(VOID) const { return m_GUIDOfCallUpPet; }

	ORESULT				ReCallHorse(VOID);
	INT					GetHorseCount(VOID)const;
	_PET_DB *			GetHorseDB( INT nIndex )const;
	_PET_DB *			GetEmptyHorseDB();
	const _PET_DB *		GetConstHorseDB( INT nIndex )const;

	Item *				GetHorseItem(PET_GUID_t guid);
	INT					GetHorseIndexByGUID(PET_GUID_t guid);

	// 临时函数
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
	//召唤兽的表格ID
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
	//召唤兽相关方法
	BOOL				RemovePet(PET_LOG_PARAM* pPetLogParam,PET_GUID_t guid);

	ORESULT				CreatePetOnRegisterZone(VOID);
	BOOL				ReleasePet(VOID);

	ORESULT				CreatePet( INT iIndex );
	BOOL				ReleasePet( INT iIndex );
	INT					GetEmptyPetIndex();
	VOID				SaveMonsterPetToDB();
	//处理自身拥有的马的更新
	VOID				HorseHeartBeat( UINT uTime );

	VOID				MatingHorseHeartBeat( INT iIndex, UINT uDurnTime );
	VOID				RemoveMatingTimeOutHorse( INT iIndex );
	VOID				SendHorseMatingDeleteMSG();

	ORESULT		CreateHorse( INT iIndex );
	BOOL				ReleaseHorse(VOID);
	BOOL				ReleaseHorse( INT iIndex );
	INT					GetEmptyHorseIndex();
	ORESULT		CallUpHorse(VOID);

	//获得正在交配的马的索引
	INT					GetMatingEndHorseIndex( UINT uTime, UINT& uDurnTime );
	VOID				SendHorseMatingSuccessMSG();
protected:
	Obj_Pet*			m_pPet[MAX_MONSTER_PET_TAKE];
	//Obj_Horse*		m_pHorse[MAX_MONSTER_HORSE_TAKE];
	INT					m_iDataID;

	PET_GUID_t			m_GUIDOfCallUpPet;
	//当前出战的马的GUID
	PET_GUID_t			m_GUIDOfCallUpHorse;
	PET_GUID_t			m_guidMatingHorse;
	BOOL				m_bMatingHorseConfirm;
	HorseAttrCalculater m_ActiveHorse;
	PET_GUID_t			m_guidFindingMateHorse;

// 捜侠录add by gh 2010/05/12
protected:
	BYTE	m_nCurSouXiaCount; // 当前身上已经学习过的搜侠录数量
	SouXia	m_SouXia[MAX_SOUXIA_CONTAINER];
	
private:
	BOOL	CanAddSouXia();
	INT		FindEmptyPos();

public:
	BYTE	GetCurSouXiaCount()			const	{ return m_nCurSouXiaCount; }
	SouXia  GetSouXia(SHORT posIndex)	const	{ return m_SouXia[posIndex]; }	
	VOID    SetSouXia(SOUXIA_DATA& souxiadata, SHORT index)	{ Assert(index>=0); m_SouXia[index].SetSouXiaData(souxiadata);}
	
	INT		AddSouXia(const INT souXiaIndex);		//增加捜侠录
	BOOL	EraseSouXia(const INT humanSouXiaPos);//删除捜侠录
	SouXia  GetCurEquipSouXia();//获取当前装备的捜侠录

	BOOL	HasLearnedProduct(UINT index);//是否已经学过该配方

	SHORT	GetPetZhaoHuanSkillTimes( SkillID_t skillId );	//获取神兽召唤技能次数
	SHORT	GetZuoJiZhaoHuanSkillTimes( SkillID_t skillId );//获取坐骑召唤技能次数

	VOID	SetPetZhaoHuanSkillTimes( SkillID_t skillId, SHORT iDeplete );//设置神兽召唤技能次数
	VOID	SetZuoJiZhaoHuanSkillTimes( SkillID_t skillId, SHORT iDeplete );//设置坐骑召唤技能次数

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
// 属性
//===========================================
public:
	INT							GetAttack();
	//StrikePoint
	INT 						GetMaxStrikePoint(VOID);
	
	//Attr1: Str
	INT							GetStr(VOID);
	//得到手工分配的力道
	INT							GetBaseStr(VOID);
	INT							GetBaseStr2(VOID);	
	VOID 						SetBaseStr(INT const nValue);
	//Attr1: Con
	INT							GetCon(VOID);
	//得到手工分配的体质
	INT							GetBaseCon(VOID);
	INT							GetBaseCon2(VOID);
	VOID						SetBaseCon(INT const nValue);
	//Attr1: Int
	INT							GetInt(VOID);
	//得到手工分配的内力
	INT							GetBaseInt(VOID);
	//得到内力，(手工分配+门派影响)
	INT							GetBaseInt2(VOID);	
	VOID						SetBaseInt(INT const nValue);
	//Attr1: Dex
	INT							GetDex(VOID);
	//得到手工分配的身法
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

	//元气
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

	//沉默抗性
	virtual INT 				GetHushAttResist(VOID);
	virtual INT 				GetBaseHushAttResist(VOID);
	virtual VOID				SetBaseHushAttResist(INT const nValue);

	//缴械抗性
	virtual INT 				GetUnArmyAttResist(VOID);
	virtual INT 				GetBaseUnArmyAttResist(VOID);
	virtual VOID				SetBaseUnArmyAttResist(INT const nValue);

	//减攻速抗性
	virtual INT 				GetAttSpeedAttResist(VOID);
	virtual INT 				GetBaseAttSpeedAttResist(VOID);
	virtual VOID				SetBaseAttSpeedAttResist(INT const nValue);

	//减攻速抗性
	virtual INT 				GetSkillSpeedAttResist(VOID);
	virtual INT 				GetBaseSkillSpeedAttResist(VOID);
	virtual VOID				SetBaseSkillSpeedAttResist(INT const nValue);

	//减移速抗性
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
	INT                         GetMountIndex(VOID);	 //  [7/23/2010 陈军龙]                               //[7/23/2010 陈军龙]

//===========================================
// _属性接口
//===========================================
public:
	virtual const INT			__GetSex( ) const ;//性别 human
	virtual VOID				__SetSex( const INT sex ) ;
	virtual const UINT			__GetCreateDate( ) const ;//创建时间 human
	virtual VOID				__SetCreateDate( const UINT createdate ) ;
	virtual const CHAR*			__GetNick( ) const ;//昵称 human
	virtual VOID				__SetNick( const CHAR* nick ) ;
	virtual VOID				__SetPasswordProtect(INT nType);
	virtual VOID				__SetProtectFlag(BOOL bFlag);
    virtual BOOL                __GetProtectFlag();
	virtual BOOL				__SetProtectTime(CHAR cTime);
	virtual CHAR				__GetProtectTime();
    virtual BOOL				__IsPasswordProtect(MINORPASSWD_INFO::PROTECT_TYPE nType);
	virtual BOOL				__IsPasswordSetup();
	virtual BOOL				__IsPasswordUnlock();
	virtual VOID				__LockPassword(); // 输入二级密码错误，则调用
	virtual VOID				__UnlockPassword(); // 输入二级密码正确
	virtual const CHAR*			__GetPassword( );//二级密码 human
	virtual BOOL				__SetPassword( const CHAR* password );
	virtual VOID				__DelPassword( ); // 删除二级密码
	virtual UINT				__GetPasswordDeleteTime( ); // 强制解除二级密码的时间
	virtual VOID				__SetPasswordDeleteTime( UINT uTime ); // 设置强制解除二级密码的时间
	virtual UINT				__GetPasswordDeleteRemainTime( ); // 得到强制解除密码的剩余时间
	virtual BOOL				__IsValidToDeletePassword( ); // 是否满足强制解除二级密码条件
	virtual UINT				__GetHairColor( ) const ;//头发颜色 human
	virtual VOID				__SetHairColor( UINT haircolor ) ;
	virtual BYTE				__GetFaceColor( ) const ;//脸颜色 human
	virtual VOID				__SetFaceColor( BYTE facecolor ) ;
	virtual BYTE				__GetHairModel( ) const ;//头发模型 human
	virtual VOID				__SetHairModel( BYTE hairmodel ) ;
	virtual BYTE				__GetFaceModel( ) const ;//脸模型 human
	virtual VOID				__SetFaceModel( BYTE facemodel ) ;
	virtual const UINT			__GetOnlineTime( ) const ;//在线时间 human
	virtual VOID				__SetOnlineTime( const UINT onlinetime ) ;
	virtual const UINT			__GetLastLevelUpTime( ) const ;//上次升级时间 human
	virtual VOID				__SetLastLevelUpTime( const UINT lastleveluptime ) ;
	virtual const UINT			__GetLastLoginTime( ) const ;//上次登录时间 human
	virtual VOID				__SetLastLoginTime( const UINT lastlogintime ) ;
	virtual const UINT			__GetLastLogoutTime( ) const ;//上次登出时间 human
	virtual VOID				__SetLastLogoutTime( const UINT lastlogouttime ) ;
	virtual const INT			__GetBagItemCount( ) const ;//背包中物品数量 human
	virtual VOID				__SetBagItemCount( const INT bagitemcount ) ;
	virtual const _ITEM*		__GetBagItem( const INT bagindex ) const ;//背包中某个位置的物品 human
	virtual VOID				__SetBagItem( const INT bagindex, const _ITEM* item ) ;
	virtual const INT			__GetBankItemCount( ) const ;//银行中物品数量 human
	virtual VOID				__SetBankItemCount( const INT Bankitemcount ) ;
	virtual const _ITEM*		__GetBankItem( const INT bankindex ) const ;//银行中某个位置的物品 human
	virtual VOID				__SetBankItem( const INT bankindex, const _ITEM* item ) ;
	virtual const INT			__GetBankEndIndex( ) const ;//当前银行的大小
	virtual VOID				__SetBankEndIndex( const INT bankindex ) ;
	virtual const INT			__GetBankMoney( ) const ;//当前银行内的钱数
	virtual VOID				__SetBankMoney( const INT money ) ;
    virtual const INT			__GetBankSize( ) const ;//当前银行格子数
    virtual VOID				__ExpandBankSize( const INT size ) ;//扩展银行各自

	virtual const INT			__GetEquipItemCount( ) const ;//装备数量 human
	virtual VOID				__SetEquipItemCount( const INT equipitemcount ) ;
	virtual const _ITEM*		__GetEquipItem( const INT equipindex ) const ;//身上某个位置的装备 human
	virtual VOID				__SetEquipItem( const INT equipindex, const _ITEM* item ) ;
	virtual const INT			__GetMissionCount( ) const ;//接受的任务数量 human
	virtual VOID				__SetMissionCount( const INT missioncount ) ;
	virtual const _OWN_MISSION* __GetMission( const INT missionindex ) const ;//某个任务数据 human
	virtual VOID				__SetMission( const INT missionindex, _OWN_MISSION* mission ) ;
	virtual const BOOL			__IsMissionHaveDone( MissionID_t missionid ) const ;//判断某个任务是否完成 human
	virtual VOID				__SetMissionDoneFalg( MissionID_t missionid, BOOL flag ) ;
	virtual const BOOL			__IsAbilityHaveLearned(AbilityID_t abilityid) const; // 某种生活技能是否学会
	virtual const INT			__GetAbilityLevel( const AbilityID_t abilityid ) const ;//某种生活技能级别 human
	virtual VOID				__SetAbilityLevel( const AbilityID_t abilityid, INT lvl) ;
	virtual const INT			__GetAbilityExp( const AbilityID_t abilityid ) const ;//某种生活技能熟练度 human
	virtual VOID				__SetAbilityExp( const AbilityID_t abilityid, INT exp) ;
	virtual const BOOL			__IsPrescrHaveLearned( const PrescriptionID_t prescrid ) const;//某个配方是否学会 human
	virtual VOID				__SetPrescrLearnedFlag( const PrescriptionID_t prescrid, BOOL flag ) ;
	virtual BOOL				__IsCanLearnAbility( const AbilityID_t abilityid) ;//add by gh 2010/07/19 是否能学习生活技能
	virtual const _ITEM_EFFECT* __GetItemEffect( const INT index ) const ;//装备效果 human
	virtual VOID				__SetItemEffect( const INT index, _ITEM_EFFECT* itemeffect ) ;
	virtual const _ITEM_VALUE	__GetItemValue( const INT index ) const ;//装备效果值 human
	virtual VOID				__SetItemValue( const INT index, _ITEM_VALUE value) ;
	virtual const UINT			__GetLastTurnPickTime()	const;
	virtual	VOID				__SetLastTurnPickTime(UINT uTime)	;
	virtual const TeamID_t		__GetTeamID( ) const ;//队伍号 human
	virtual VOID				__SetTeamID( const TeamID_t teamid ) ;
	virtual BOOL				__IsTeamLeader( ) const; // 是否队长
	virtual const INT			__GetTeamMemberCount( ) const ;//队伍中成员数量 human
	virtual const TEAMMEMBER*	__GetTeamMember( const INT memberindex ) const ;//取得队伍中某个成员
	virtual VOID				__AddTeamMember( const TEAMMEMBER* member ) ;
	virtual const INT			__GetTeamSceneMemberCount( ) const ;//队伍中同场景成员数量 human
	virtual const ObjID_t		__GetTeamSceneMember( const INT memberindex ) const ;//取得队伍中某同场景成员的 OBJID
	virtual VOID				__SetTeamFollowFlag( BOOL flag ); // 设置队伍跟随状态
	virtual const BOOL			__GetTeamFollowFlag( ) const; // 获得队伍跟随状态
	virtual VOID				__AddFollowedMember( const _FOLLOWEDMEMBER& FollowedMember ); // 增加一个跟随的队员
	virtual VOID				__DelFollowedMember( GUID_t guid ); // 移出一个跟随队员
	virtual const INT			__GetFollowedMembersCount() const; // 得到跟随状态的队友数量
	virtual const _FOLLOWEDMEMBER*	__GetFollowedMember( INT i ) const; // 得到第 i 个跟随的队友
	virtual VOID				__ClearFollowedMembers(); // 清除所有的跟随队友列表
	virtual const FLOAT			__GetTeamFollowSpeed( ) const; // 得到组队跟随的速度
	virtual VOID				__SetTeamFollowSpeed( FLOAT fSpeed ); // 设置组队跟随的速度
	virtual VOID				__OutOfTeamFollowRange(); // 超出组队跟随范围
	virtual VOID				__InTeamFollowRange(); // 在组队跟随范围内
	virtual const BOOL			__GetTeamFollowSpeedUp( ) const; // 获得组队跟随加速状态
	virtual VOID				__SetTeamFollowSpeedUp( BOOL bSpeedUp); // 设置组队跟随加速状态
	virtual VOID				__StopTeamFollow( BOOL bWorldFlag=TRUE ); // 停止跟随,flag表示是否通知 world
};


#endif
