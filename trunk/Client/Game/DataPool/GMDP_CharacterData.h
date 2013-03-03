
/** 
 *	角色逻辑数据
 */

#ifndef	__CHARACTERDATA_H__
#define __CHARACTERDATA_H__


#include "GMDP_Struct_CharacterData.h"



struct _CAMP_DATA;
class CObject_Character;

class CCharacterData
{
public:

	//---------------------------------
	/**
	 *	Character 基本属性
	 */


	// 国家
	INT					Get_Country(VOID) const				{ return m_pData->m_nCountry; }			
	VOID				Set_Country(INT country);

	// 种族(性别)
	INT					Get_RaceID(VOID) const				{ return m_pData->m_nRaceID; }			
	VOID				Set_RaceID(INT nRaceID);

	// 头像
	INT					Get_PortraitID(VOID) const			{ return m_pData->m_nPortraitID; }			
	VOID				Set_PortraitID(INT nPortraitID);

	// 名称
	LPCTSTR				Get_Name(VOID) const				{ return m_pData->m_strName.c_str(); }	
	LPCTSTR				Get_Utf8Name(VOID) const			{ return m_pData->m_strUtf8Name.c_str(); }	
	VOID				Set_Name(LPCTSTR szName);

	// 称号(国家\帮会\玩法)
	LPCTSTR				Get_CurTitle(INT nType) const;
	VOID				Set_CurTitle(LPCTSTR szTitle, INT nType);

	// 称号类型
	BYTE				Get_CurTitleType(VOID) const		{ return m_pData->m_TitleType; }
	VOID				Set_CurTitleType(BYTE bTitleType);

	// 生命值百分比
	FLOAT				Get_HPPercent(VOID) const			{ return m_pData->m_fHPPercent; }		
	VOID				Set_HPPercent(FLOAT fPercent);
	// 通过生命值是否死亡
	BOOL				IsDie(VOID) const					{ return (Get_HPPercent() <= 0.f); }

	//元气百分比
	FLOAT				Get_PenumaPercent(VOID) const			{ return m_pData->m_fPenumaPercent; }		
	VOID				Set_PenumaPercent(FLOAT fPercent);

	// 魔法百分比
	FLOAT				Get_MPPercent(VOID) const			{ return m_pData->m_fMPPercent; }		
	VOID				Set_MPPercent(FLOAT fPercent);

	// 移动速度
	FLOAT				Get_MoveSpeed(VOID) const			{ return m_pData->m_fMoveSpeed; }		
	VOID				Set_MoveSpeed(FLOAT fSpeed);

	// 阵营
	const _CAMP_DATA	*Get_CampData(VOID) const			{ return &(m_pData->m_CampData); }			
	VOID				Set_CampData(const _CAMP_DATA *pCampData);

	// 所有者
	ObjID_t				Get_OwnerID(VOID) const				{ return m_pData->m_nOwnerID; }
	VOID				Set_OwnerID(ObjID_t nOwnerID);

	// 
	INT					Get_AIType(VOID) const				{ return m_pData->m_nAIType; }
	VOID				Set_AIType(INT nAIType);

	// 占有者
	GUID_t				Get_OccupantGUID(VOID) const		{ return m_pData->m_OccupantGUID; }
	VOID				Set_OccupantGUID(GUID_t guid);

	// 
	INT					Get_Relative(VOID) const			{ return m_pData->m_nRelative; }											
	VOID				Set_Relative(INT nRelative);

	// 数据内部保存的模型id， 变身用
	INT					Get_ModelID(VOID) const				{ return m_pData->m_nModelID; }
	VOID				Set_ModelID(INT nModelID);

	//	坐骑ID
	INT					Get_MountID(VOID) const				{ return m_pData->m_nMountID; }
	VOID				Set_MountID(INT nMountID);

	//	坐骑索引  [7/23/2010 陈军龙]
	INT					Get_MountIndex(VOID) const			{ return m_pData->m_nMountIndex; }
	VOID				Set_MountIndex(INT nMountIndex);

	// 等级
	INT					Get_Level(VOID) const				{ return m_pData->m_nLevel; }				
	VOID				Set_Level(INT nLevel);

	// 隐身级别
	INT					Get_StealthLevel(VOID) const		{ return m_pData->m_nStealthLevel; }				
	VOID				Set_StealthLevel(INT nStealthLevel);

	// 是否坐下
	BOOL				IsSit(VOID) const					{ return m_pData->m_bSit; }	
	VOID				Set_Sit(BOOL bSit);


	//---------------------------------
	/**
	 *	Character_PlayerOther 对于其他玩家
	 */


	// 门派
	INT					Get_MenPai(VOID) const;											
	VOID				Set_MenPai(INT nMenPai);

	//GUID
	GUID_t					Get_GUID(VOID) const;											
	VOID				Set_GUID(GUID_t guid);

	// 帮会
	INT					Get_Guild(VOID) const;											
	VOID				Set_Guild(INT nGuild);

	// 头发模型
	INT					Get_HairMesh(VOID) const;										
	VOID				Set_HairMesh(INT nHairMesh);
	// 头发颜色
	UINT				Get_HairColor(VOID) const;										
	VOID				Set_HairColor(UINT uHairColor);

	// 脸模型
	INT					Get_FaceMesh(VOID) const;										
	VOID				Set_FaceMesh(INT nFaceMesh);

	// 装备状态版本号，用于和服务器同步数据
	INT					Get_EquipVer(VOID) const;										
	VOID				Set_EquipVer(INT nEquipVer);

	// 装备
	INT					Get_Equip(HUMAN_EQUIP point) const;
	VOID				Set_Equip(HUMAN_EQUIP point, INT nID);
	// 装备宝石
	INT					Get_EquipGem(HUMAN_EQUIP point) const;								
	VOID				Set_EquipGem(HUMAN_EQUIP point, INT nID); 
	// 套装
	_SUIT_SETTING&		Get_EquipSuit(int suitIdx) const;				
	VOID				Set_EquipSuit(int suitIdx, _SUIT_SETTING& pSuit);
	// 套装id
	INT					Get_EquipSuitId() const { return m_iCurSuitId; }
	VOID				Set_EquipSuitId(int suitIdx) { m_iCurSuitId = suitIdx; }

	// 怪的武器
	INT					Get_MonstWeapon( VOID ) const;
	VOID				Set_MonstWeapon( INT nID );

	// 坐骑的换装
	INT					Get_MountEquip( HORSE_EQUIP point ) const;
	VOID				Set_MountEquip( HORSE_EQUIP point, INT nID );

	// 拥有的坐骑  [7/14/2010 陈军龙]
	INT					Get_Mount( INT nIndex ) const;
	VOID				Set_Mount( INT nIndex, INT nID );

	// 是否组队标记
	BOOL				Get_HaveTeamFlag(VOID) const;
	VOID				Set_HaveTeamFlag(BOOL flag);

	// 是否组队标记
	BOOL				Get_TeamLeaderFlag(VOID) const;
	VOID				Set_TeamLeaderFlag(BOOL flag);

	// 满队标记
	BOOL				Get_TeamFullFlag(VOID) const;
	VOID				Set_TeamFullFlag(BOOL flag);

	// 组队跟随
	BOOL				Get_TeamFollowFlag(VOID) const;
	VOID				Set_TeamFollowFlag(BOOL flag);

	// 跟随列表里的人数
	INT					Get_TeamFollowListCount(VOID) const;
	VOID				Set_TeamFollowListCount(INT count);

	// 跟随列表里的成员
	GUID_t				Get_TeamFollowedMember(INT index) const;
	VOID				Set_TeamFollowedMember(INT index, GUID_t guid);

	// 称号表
	_TITLE_				Get_Title(INT index) const;
	VOID				Set_Title(INT index, _TITLE_ title);
	// 称号数量
	INT					Get_TitleNum() const;
	VOID				Set_TitleNum(INT num );

	// 当前称号索引
	INT					Get_CurTitleIndex() const;
	VOID				Set_CurTitleIndex(INT index );

	// 清空称号
	VOID				Clear_AllTitles();				

	VOID				Set_BusObjID(ObjID_t nBusObjID);
	ObjID_t				Get_BusObjID(VOID)const;

	
	//---------------------------------
	/**
	 *	Character_PlayerMySelf 对于玩家自己
	 */


	INT					Get_HP(VOID) const;
	VOID				Set_HP(INT nHP);

	INT					Get_MP(VOID) const;
	VOID				Set_MP(INT nMP);

	VOID				Set_Penuma(INT nPneuma);
	INT					Get_Penuma(VOID) const;

	INT					Get_Exp(VOID) const;
	VOID				Set_Exp(INT nExp);

	INT					Get_MaxExp(VOID) const;											
	//VOID				Set_MaxExp(INT nMaxExp); //通过查表获得

	INT					Get_Money(VOID) const;											
	VOID				Set_Money(INT nMoney);

	const PET_GUID_t&	Get_CurrentPetGUID(VOID) const;
	VOID				Set_CurrentPetGUID(PET_GUID_t guid);

	INT					Get_Rage(VOID)const;
	VOID				Set_Rage(INT nRage);

	INT					Get_StrikePoint(VOID)const;
	VOID				Set_StrikePoint(INT nStrikePoint);

	// 活力值
	INT					Get_Vigor(VOID)const;
	VOID				Set_Vigor(INT nVigor);

	INT					Get_MaxVigor(VOID)const;
	VOID				Set_MaxVigor(INT nVigor);

	INT					Get_Energy(VOID)const;
	VOID				Set_Energy(INT nEnergy);

	INT					Get_MaxEnergy(VOID)const;
	VOID				Set_MaxEnergy(INT nEnergy);

	INT					Get_GoodBadValue(VOID)const;
	VOID				Set_GoodBadValue(INT nValue);

	//----
	INT					Get_STR(VOID) const;											
	VOID				Set_STR(INT nSTR);

	INT					Get_CON(VOID) const;											
	VOID				Set_CON(INT nCON);

	INT					Get_INT(VOID) const;											
	VOID				Set_INT(INT nINT);

	INT					Get_DEX(VOID) const;											
	VOID				Set_DEX(INT nDEX);

	INT					Get_PointRemain(VOID) const;									
	VOID				Set_PointRemain(INT nPoint);

	INT					Get_AttMagic_Near(VOID) const;										
	VOID				Set_AttMagic_Near(INT nAttMagic);

	INT					Get_AttMagic_Far(VOID) const;										
	VOID				Set_AttMagic_Far(INT nAttMagic);

	//======
	INT					Get_AttNear(VOID) const;
	VOID				Set_AttNear(INT nAtt);
	INT					Get_DefNear(VOID) const;
	VOID				Set_DefNear(INT nDef);

	INT					Get_AttFar(VOID) const;
	VOID				Set_AttFar(INT nAtt);
	INT					Get_DefFar(VOID) const;
	VOID				Set_DefFar(INT nDef);


	INT					Get_DefMagic_Near(VOID) const;										
	VOID				Set_DefMagic_Near(INT nDefMagic);

	INT					Get_DefMagic_Far(VOID) const;										
	VOID				Set_DefMagic_Far(INT nDefMagic);

	//----
	INT					Get_MaxHP(VOID) const;											
	VOID				Set_MaxHP(INT nMaxHP);

	INT					Get_MaxMP(VOID) const;											
	VOID				Set_MaxMP(INT nMaxMP);

	INT					Get_MaxPneuma(VOID) const;											
	VOID				Set_MaxPneuma(INT nMaxPneuma);
	
	INT					Get_MaxRage(VOID) const;
	VOID				Set_MaxRage(INT nMaxRage);

	INT					Get_HPRespeed(VOID) const;										
	VOID				Set_HPRespeed(INT nHPRespeed);

	INT					Get_MPRespeed(VOID) const;										
	VOID				Set_MPRespeed(INT nMPRespeed);

	INT					Get_Toughness(VOID) const;
	VOID				Set_Toughness(INT nToughness);

	INT					Get_Hit(VOID) const;											
	VOID				Set_Hit(INT nHit);

	INT					Get_Miss(VOID) const;											
	VOID				Set_Miss(INT nMiss);

	INT					Get_CritRate(VOID) const;										
	VOID				Set_CritRate(INT nCritRate);

	INT					Get_AttackSpeed(VOID) const;									
	VOID				Set_AttackSpeed(INT fSpeed);

	INT					Get_AttGold(VOID) const;										
	VOID				Set_AttGold(INT nAttGold);

	INT					Get_DefGold(VOID) const;										
	VOID				Set_DefGold(INT nDefGold);

	INT					Get_AttFire(VOID) const;										
	VOID				Set_AttFire(INT nAttFire);

	INT					Get_DefFire(VOID) const;										
	VOID				Set_DefFire(INT nDefFire);

	INT					Get_AttWood(VOID) const;										
	VOID				Set_AttWood(INT nAttWood);

	INT					Get_DefWood(VOID) const;										
	VOID				Set_DefWood(INT nDefWood);

	INT					Get_AttWater(VOID) const;										
	VOID				Set_AttWater(INT nAttWater);

	INT					Get_DefWater(VOID) const;										
	VOID				Set_DefWater(INT nDefWater);

	INT					Get_AttSoil(VOID) const;										
	VOID				Set_AttSoil(INT nAttWater);

	INT					Get_DefSoil(VOID) const;										
	VOID				Set_DefSoil(INT nDefWater);

	BOOL				IsLimitMove(VOID) const;
	VOID				SetLimitMove(BOOL bSet);

	//BOOL				IsLimitUseSkill(VOID) const;
	//VOID				SetLimitUseSkill(BOOL bSet);

	//BOOL				IsLimitHandle(VOID) const;
	//VOID				SetLimitHandle(BOOL bSet);

	BOOL				IsHaveCanActionFlag1( VOID )const;
	VOID				SetCanActionFlag1( BOOL bSet );

	BOOL				IsHaveCanActionFlag2( VOID )const;
	VOID				SetCanActionFlag2( BOOL bSet );


	// 设置二级保护密码
	BOOL				Get_IsMinorPwdSetup(  BOOL bOpenDlg = TRUE ) const;
	VOID				Set_SetMinorPwdSetup( BOOL bSet );

	// 是否验证过二级保护密码
	BOOL				Get_IsMinorPwdUnlocked( BOOL bOpenDlg = TRUE ) const;
	VOID				Set_SetMinorPwdUnlocked( BOOL bSet );

	//---------------------------------
//$$const SXINFA_MAP&			Get_XinFa(VOID) const;
//	const SCLIENT_XINFA*		Get_XinFa(INT nID) const;
//	VOID						Set_XinFa(INT nID, INT nLevel);

	// 得到技能总数 Nick 2007.11.23
	int							Get_SkillNum(VOID) const;
	const SSKILL_MAP&			Get_Skill(VOID) const;
	const SCLIENT_SKILL*		Get_Skill(INT nID) const;
	VOID						Set_Skill(INT nID, BOOL bLean, BOOL bDefault = FALSE);
	INT							Get_DefaultSkillID(VOID) { return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nDefaultSkillID; }
	VOID						Skill_CleanAll(void);
//	VOID						Update_SkillByXinFaID( INT nXinFaID );

	INT							Get_SeriesPoint( INT nSeriesID );						// 获取 某系别的总共花费的点数
	VOID						Set_SeriesPoint( INT nSeriesID, INT nSeriesPoint );		// 设置	......................

	INT							Get_CurrentSkillPoint();								// 获得 拥有的技能点数
	VOID						Set_CurrentSkillPoint( INT nPoint );					// 设置 ..............

	VOID						Set_SkillLevel(INT nSkillID, INT nSkillLevel);

	const SLIFEABILITY_MAP&		Get_LifeAbility(VOID) const;
	const SCLIENT_LIFEABILITY*	Get_LifeAbility(INT nID) const;
	VOID						Set_LifeAbility(INT nID, INT nLevel, INT nExp);

	const SPRESCR_MAP&			Get_Prescr(VOID) const;
	const SCLIENT_PRESCR*		Get_Prescr(INT nID) const;
	VOID						Set_Prescr(INT nID, BOOL bLean);

	BOOL						Get_IsInStall(VOID) const;										
	VOID						Set_IsInStall(BOOL bIsInStall);

	LPCTSTR						Get_StallName(VOID) const;		
	VOID						Set_StallName(LPCTSTR szName);

	//获得当前摊位开市状态		20100706 BLL
	BOOL						Get_StallOpenPublic(VOID) const;
	
	//设置摊位开市状态
	VOID						Set_StallOpenPublic( BOOL bOpenPublic );
	

	const FLOAT					Get_SpeedRate(VOID) const { return m_fSpeedRate; }


public:

	CCharacterData(CObject_Character* pCharObj);
	virtual ~CCharacterData();

protected:

	/// 初始化操作

	VOID				_Init_AsCharacter(VOID);
	VOID				_Init_AsNPC(VOID);
	VOID				_Init_AsPlayerOther(VOID);
	VOID				_Init_AsPlayerMySelf(VOID);


	/// 改变职业后相应的调整职业对应的属性
	VOID				_On_MenPaiChanged(VOID);


protected:

	/// 角色逻辑指针
	CObject_Character*	m_pObjRef;
	/// 角色数据结构
	SDATA_CHARACTER*	m_pData;
	/// 改变角色频率相关
	FLOAT				m_fSpeedRate;

	/// 是否死亡
	BOOL				m_Die;

	INT					m_iCurSuitId;	// 当前套装id

};	// class


#endif	// __CHARACTERDATA_H__
