
/** 
 *	��ɫ�߼�����
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
	 *	Character ��������
	 */


	// ����
	INT					Get_Country(VOID) const				{ return m_pData->m_nCountry; }			
	VOID				Set_Country(INT country);

	// ����(�Ա�)
	INT					Get_RaceID(VOID) const				{ return m_pData->m_nRaceID; }			
	VOID				Set_RaceID(INT nRaceID);

	// ͷ��
	INT					Get_PortraitID(VOID) const			{ return m_pData->m_nPortraitID; }			
	VOID				Set_PortraitID(INT nPortraitID);

	// ����
	LPCTSTR				Get_Name(VOID) const				{ return m_pData->m_strName.c_str(); }	
	LPCTSTR				Get_Utf8Name(VOID) const			{ return m_pData->m_strUtf8Name.c_str(); }	
	VOID				Set_Name(LPCTSTR szName);

	// �ƺ�(����\���\�淨)
	LPCTSTR				Get_CurTitle(INT nType) const;
	VOID				Set_CurTitle(LPCTSTR szTitle, INT nType);

	// �ƺ�����
	BYTE				Get_CurTitleType(VOID) const		{ return m_pData->m_TitleType; }
	VOID				Set_CurTitleType(BYTE bTitleType);

	// ����ֵ�ٷֱ�
	FLOAT				Get_HPPercent(VOID) const			{ return m_pData->m_fHPPercent; }		
	VOID				Set_HPPercent(FLOAT fPercent);
	// ͨ������ֵ�Ƿ�����
	BOOL				IsDie(VOID) const					{ return (Get_HPPercent() <= 0.f); }

	//Ԫ���ٷֱ�
	FLOAT				Get_PenumaPercent(VOID) const			{ return m_pData->m_fPenumaPercent; }		
	VOID				Set_PenumaPercent(FLOAT fPercent);

	// ħ���ٷֱ�
	FLOAT				Get_MPPercent(VOID) const			{ return m_pData->m_fMPPercent; }		
	VOID				Set_MPPercent(FLOAT fPercent);

	// �ƶ��ٶ�
	FLOAT				Get_MoveSpeed(VOID) const			{ return m_pData->m_fMoveSpeed; }		
	VOID				Set_MoveSpeed(FLOAT fSpeed);

	// ��Ӫ
	const _CAMP_DATA	*Get_CampData(VOID) const			{ return &(m_pData->m_CampData); }			
	VOID				Set_CampData(const _CAMP_DATA *pCampData);

	// ������
	ObjID_t				Get_OwnerID(VOID) const				{ return m_pData->m_nOwnerID; }
	VOID				Set_OwnerID(ObjID_t nOwnerID);

	// 
	INT					Get_AIType(VOID) const				{ return m_pData->m_nAIType; }
	VOID				Set_AIType(INT nAIType);

	// ռ����
	GUID_t				Get_OccupantGUID(VOID) const		{ return m_pData->m_OccupantGUID; }
	VOID				Set_OccupantGUID(GUID_t guid);

	// 
	INT					Get_Relative(VOID) const			{ return m_pData->m_nRelative; }											
	VOID				Set_Relative(INT nRelative);

	// �����ڲ������ģ��id�� ������
	INT					Get_ModelID(VOID) const				{ return m_pData->m_nModelID; }
	VOID				Set_ModelID(INT nModelID);

	//	����ID
	INT					Get_MountID(VOID) const				{ return m_pData->m_nMountID; }
	VOID				Set_MountID(INT nMountID);

	//	��������  [7/23/2010 �¾���]
	INT					Get_MountIndex(VOID) const			{ return m_pData->m_nMountIndex; }
	VOID				Set_MountIndex(INT nMountIndex);

	// �ȼ�
	INT					Get_Level(VOID) const				{ return m_pData->m_nLevel; }				
	VOID				Set_Level(INT nLevel);

	// ������
	INT					Get_StealthLevel(VOID) const		{ return m_pData->m_nStealthLevel; }				
	VOID				Set_StealthLevel(INT nStealthLevel);

	// �Ƿ�����
	BOOL				IsSit(VOID) const					{ return m_pData->m_bSit; }	
	VOID				Set_Sit(BOOL bSit);


	//---------------------------------
	/**
	 *	Character_PlayerOther �����������
	 */


	// ����
	INT					Get_MenPai(VOID) const;											
	VOID				Set_MenPai(INT nMenPai);

	//GUID
	GUID_t					Get_GUID(VOID) const;											
	VOID				Set_GUID(GUID_t guid);

	// ���
	INT					Get_Guild(VOID) const;											
	VOID				Set_Guild(INT nGuild);

	// ͷ��ģ��
	INT					Get_HairMesh(VOID) const;										
	VOID				Set_HairMesh(INT nHairMesh);
	// ͷ����ɫ
	UINT				Get_HairColor(VOID) const;										
	VOID				Set_HairColor(UINT uHairColor);

	// ��ģ��
	INT					Get_FaceMesh(VOID) const;										
	VOID				Set_FaceMesh(INT nFaceMesh);

	// װ��״̬�汾�ţ����ںͷ�����ͬ������
	INT					Get_EquipVer(VOID) const;										
	VOID				Set_EquipVer(INT nEquipVer);

	// װ��
	INT					Get_Equip(HUMAN_EQUIP point) const;
	VOID				Set_Equip(HUMAN_EQUIP point, INT nID);
	// װ����ʯ
	INT					Get_EquipGem(HUMAN_EQUIP point) const;								
	VOID				Set_EquipGem(HUMAN_EQUIP point, INT nID); 
	// ��װ
	_SUIT_SETTING&		Get_EquipSuit(int suitIdx) const;				
	VOID				Set_EquipSuit(int suitIdx, _SUIT_SETTING& pSuit);
	// ��װid
	INT					Get_EquipSuitId() const { return m_iCurSuitId; }
	VOID				Set_EquipSuitId(int suitIdx) { m_iCurSuitId = suitIdx; }

	// �ֵ�����
	INT					Get_MonstWeapon( VOID ) const;
	VOID				Set_MonstWeapon( INT nID );

	// ����Ļ�װ
	INT					Get_MountEquip( HORSE_EQUIP point ) const;
	VOID				Set_MountEquip( HORSE_EQUIP point, INT nID );

	// ӵ�е�����  [7/14/2010 �¾���]
	INT					Get_Mount( INT nIndex ) const;
	VOID				Set_Mount( INT nIndex, INT nID );

	// �Ƿ���ӱ��
	BOOL				Get_HaveTeamFlag(VOID) const;
	VOID				Set_HaveTeamFlag(BOOL flag);

	// �Ƿ���ӱ��
	BOOL				Get_TeamLeaderFlag(VOID) const;
	VOID				Set_TeamLeaderFlag(BOOL flag);

	// ���ӱ��
	BOOL				Get_TeamFullFlag(VOID) const;
	VOID				Set_TeamFullFlag(BOOL flag);

	// ��Ӹ���
	BOOL				Get_TeamFollowFlag(VOID) const;
	VOID				Set_TeamFollowFlag(BOOL flag);

	// �����б��������
	INT					Get_TeamFollowListCount(VOID) const;
	VOID				Set_TeamFollowListCount(INT count);

	// �����б���ĳ�Ա
	GUID_t				Get_TeamFollowedMember(INT index) const;
	VOID				Set_TeamFollowedMember(INT index, GUID_t guid);

	// �ƺű�
	_TITLE_				Get_Title(INT index) const;
	VOID				Set_Title(INT index, _TITLE_ title);
	// �ƺ�����
	INT					Get_TitleNum() const;
	VOID				Set_TitleNum(INT num );

	// ��ǰ�ƺ�����
	INT					Get_CurTitleIndex() const;
	VOID				Set_CurTitleIndex(INT index );

	// ��ճƺ�
	VOID				Clear_AllTitles();				

	VOID				Set_BusObjID(ObjID_t nBusObjID);
	ObjID_t				Get_BusObjID(VOID)const;

	
	//---------------------------------
	/**
	 *	Character_PlayerMySelf ��������Լ�
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
	//VOID				Set_MaxExp(INT nMaxExp); //ͨ�������

	INT					Get_Money(VOID) const;											
	VOID				Set_Money(INT nMoney);

	const PET_GUID_t&	Get_CurrentPetGUID(VOID) const;
	VOID				Set_CurrentPetGUID(PET_GUID_t guid);

	INT					Get_Rage(VOID)const;
	VOID				Set_Rage(INT nRage);

	INT					Get_StrikePoint(VOID)const;
	VOID				Set_StrikePoint(INT nStrikePoint);

	// ����ֵ
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


	// ���ö�����������
	BOOL				Get_IsMinorPwdSetup(  BOOL bOpenDlg = TRUE ) const;
	VOID				Set_SetMinorPwdSetup( BOOL bSet );

	// �Ƿ���֤��������������
	BOOL				Get_IsMinorPwdUnlocked( BOOL bOpenDlg = TRUE ) const;
	VOID				Set_SetMinorPwdUnlocked( BOOL bSet );

	//---------------------------------
//$$const SXINFA_MAP&			Get_XinFa(VOID) const;
//	const SCLIENT_XINFA*		Get_XinFa(INT nID) const;
//	VOID						Set_XinFa(INT nID, INT nLevel);

	// �õ��������� Nick 2007.11.23
	int							Get_SkillNum(VOID) const;
	const SSKILL_MAP&			Get_Skill(VOID) const;
	const SCLIENT_SKILL*		Get_Skill(INT nID) const;
	VOID						Set_Skill(INT nID, BOOL bLean, BOOL bDefault = FALSE);
	INT							Get_DefaultSkillID(VOID) { return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nDefaultSkillID; }
	VOID						Skill_CleanAll(void);
//	VOID						Update_SkillByXinFaID( INT nXinFaID );

	INT							Get_SeriesPoint( INT nSeriesID );						// ��ȡ ĳϵ����ܹ����ѵĵ���
	VOID						Set_SeriesPoint( INT nSeriesID, INT nSeriesPoint );		// ����	......................

	INT							Get_CurrentSkillPoint();								// ��� ӵ�еļ��ܵ���
	VOID						Set_CurrentSkillPoint( INT nPoint );					// ���� ..............

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

	//��õ�ǰ̯λ����״̬		20100706 BLL
	BOOL						Get_StallOpenPublic(VOID) const;
	
	//����̯λ����״̬
	VOID						Set_StallOpenPublic( BOOL bOpenPublic );
	

	const FLOAT					Get_SpeedRate(VOID) const { return m_fSpeedRate; }


public:

	CCharacterData(CObject_Character* pCharObj);
	virtual ~CCharacterData();

protected:

	/// ��ʼ������

	VOID				_Init_AsCharacter(VOID);
	VOID				_Init_AsNPC(VOID);
	VOID				_Init_AsPlayerOther(VOID);
	VOID				_Init_AsPlayerMySelf(VOID);


	/// �ı�ְҵ����Ӧ�ĵ���ְҵ��Ӧ������
	VOID				_On_MenPaiChanged(VOID);


protected:

	/// ��ɫ�߼�ָ��
	CObject_Character*	m_pObjRef;
	/// ��ɫ���ݽṹ
	SDATA_CHARACTER*	m_pData;
	/// �ı��ɫƵ�����
	FLOAT				m_fSpeedRate;

	/// �Ƿ�����
	BOOL				m_Die;

	INT					m_iCurSuitId;	// ��ǰ��װid

};	// class


#endif	// __CHARACTERDATA_H__
