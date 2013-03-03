// WGDetailOtherOnlineAttrib_Human.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "WGDetailOtherOnlineAttrib_Human.h"

namespace Packets
{


BOOL WGDetailOtherOnlineAttrib_Human::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read( (CHAR*)(&m_Flags), sizeof(m_Flags));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_LEVEL))
		iStream.Read( (CHAR*)(&m_uLevel), sizeof(m_uLevel));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_HP))
		iStream.Read( (CHAR*)(&m_nHP), sizeof(m_nHP));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MP))
		iStream.Read( (CHAR*)(&m_nMP), sizeof(m_nMP));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_PNEUMA))
		iStream.Read( (CHAR*)(&m_nPneuma), sizeof(m_nPneuma));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAXHP))
		iStream.Read( (CHAR*)(&m_nMAXHP), sizeof(m_nMAXHP));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAXMP))
		iStream.Read( (CHAR*)(&m_nMAXMP), sizeof(m_nMAXMP));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAXPNEUMA))
		iStream.Read( (CHAR*)(&m_nMaxPneuma), sizeof(m_nMaxPneuma));


	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MOVESPEED))
		iStream.Read( (CHAR*)(&m_fMoveSpeed), sizeof(m_fMoveSpeed));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_EXP))
		iStream.Read( (CHAR*)(&m_nExp), sizeof(m_nExp));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MONEY))
		iStream.Read( (CHAR*)(&m_uMoney), sizeof(m_uMoney));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_STR))
		iStream.Read( (CHAR*)(&m_Str), sizeof(INT));
	/*if ( m_Flags.isSetBit(DETAIL_ATTRIB_SPR))
	iStream.Read( (CHAR*)(&m_Spr), sizeof(INT));*/
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CON))
		iStream.Read( (CHAR*)(&m_Con), sizeof(INT));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_INT))
		iStream.Read( (CHAR*)(&m_Int), sizeof(INT));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEX))
		iStream.Read( (CHAR*)(&m_Dex), sizeof(INT));


	if ( m_Flags.isSetBit(DETAIL_ATTRIB_POINT_REMAIN))
		iStream.Read( (CHAR*)(&m_nPoint_Remain), sizeof(m_nPoint_Remain));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_SKILLPOINT_REMAIN))
		iStream.Read( (CHAR*)(&m_nSkillPoint_Remain), sizeof(m_nSkillPoint_Remain));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_S_SKILLPOINT))
		iStream.Read( (CHAR*)(&m_nS_SkillPoints), sizeof(INT)*COMMON_S_NUMBER);

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_HP_RESPEED))
		iStream.Read( (CHAR*)(&m_nHP_ReSpeed), sizeof(m_nHP_ReSpeed));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MP_RESPEED))
		iStream.Read( (CHAR*)(&m_nMP_ReSpeed), sizeof(m_nMP_ReSpeed));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_VIGOR_RESPEED))
		iStream.Read( (CHAR*)(&m_nVigor_ReSpeed), sizeof(m_nVigor_ReSpeed));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATT_PHYSICS))
	//	iStream.Read( (CHAR*)(&m_nAtt_Physics), sizeof(m_nAtt_Physics));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATT_NEAR))
		iStream.Read( (CHAR*)(&m_nAtt_Near), sizeof(m_nAtt_Near));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATT_FAR))
		iStream.Read( (CHAR*)(&m_nAtt_Far), sizeof(m_nAtt_Far));


	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATT_MAGIC_NEAR))
		iStream.Read( (CHAR*)(&m_nAtt_MagicNear), sizeof(m_nAtt_MagicNear));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATT_MAGIC_FAR))
		iStream.Read( (CHAR*)(&m_nAtt_MagicFar), sizeof(m_nAtt_MagicFar));

	/*if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEF_PHYSICS))
	iStream.Read( (CHAR*)(&m_nDef_Physics), sizeof(m_nDef_Physics));
	*/
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEF_NEAR))
		iStream.Read( (CHAR*)(&m_nDef_Near), sizeof(m_nDef_Near));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEF_FAR))
		iStream.Read( (CHAR*)(&m_nDef_Far), sizeof(m_nDef_Far));


	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEF_MAGIC_NEAR))
		iStream.Read( (CHAR*)(&m_nDef_MagicNear), sizeof(m_nDef_MagicNear));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEF_MAGIC_FAR))
		iStream.Read( (CHAR*)(&m_nDef_MagicFar), sizeof(m_nDef_MagicFar));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_TOUGHNESS))
		iStream.Read( (CHAR*)(&m_nToughness), sizeof(m_nToughness));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_HIT))
		iStream.Read( (CHAR*)(&m_nHit), sizeof(m_nHit));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISS))
		iStream.Read( (CHAR*)(&m_nMiss), sizeof(m_nMiss));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CRITRATE))
		iStream.Read( (CHAR*)(&m_nCritic), sizeof(m_nCritic));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CRITICALHURT))
		iStream.Read( (CHAR*)(&m_nCriticHurt), sizeof(m_nCriticHurt));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAXRAGE))
		iStream.Read( (CHAR*)(&m_nMaxRage), sizeof(m_nMaxRage));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_RAGE))
		iStream.Read( (CHAR*)(&m_nRage), sizeof(m_nRage));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_RAGE_RESPEED))
		iStream.Read( (CHAR*)(&m_nRage_ReSpeed), sizeof(m_nRage_ReSpeed));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_STRIKE_POINT))
		iStream.Read( (CHAR*)(&m_nStrikePoint), sizeof(m_nStrikePoint));


	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKSPEED))
		iStream.Read( (CHAR*)(&m_nAttackSpeed), sizeof(m_nAttackSpeed));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACK_GOLD))
		iStream.Read( (CHAR*)(&m_nAttGold), sizeof(m_nAttGold));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCE_GOLD))
		iStream.Read( (CHAR*)(&m_nDefGold), sizeof(m_nDefGold));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACK_WOOD))
		iStream.Read( (CHAR*)(&m_nAttWood), sizeof(m_nAttWood));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCE_WOOD))
		iStream.Read( (CHAR*)(&m_nDefWood), sizeof(m_nDefWood));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACK_WATER))
		iStream.Read( (CHAR*)(&m_nAttWater), sizeof(m_nAttWater));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCE_WATER))
		iStream.Read( (CHAR*)(&m_nDefWater), sizeof(m_nDefWater));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACK_FIRE))
		iStream.Read( (CHAR*)(&m_nAttFire), sizeof(m_nAttFire));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCE_FIRE))
		iStream.Read( (CHAR*)(&m_nDefFire), sizeof(m_nDefFire));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACK_SOIL))
		iStream.Read( (CHAR*)(&m_nAttSoil), sizeof(m_nAttSoil));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCE_SOIL))
		iStream.Read( (CHAR*)(&m_nDefSoil), sizeof(m_nDefSoil));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_NEARATT_REDUCE))
		iStream.Read( (CHAR*)(&m_nNearReduce), sizeof(m_nNearReduce));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_FARATT_REDUCE))
		iStream.Read( (CHAR*)(&m_nFarReduce), sizeof(m_nFarReduce));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAGIC_NEAR_ATT_REDUCE))
		iStream.Read( (CHAR*)(&m_nMagicNearReduce), sizeof(m_nMagicNearReduce));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAGIC_FAR_ATT_REDUCE))
		iStream.Read( (CHAR*)(&m_nMagicFarReduce), sizeof(m_nMagicFarReduce));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DREAD_RESIST))
		iStream.Read( (CHAR*)(&m_nDreadReduce), sizeof(m_nDreadReduce));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_COMA_RESIST))
		iStream.Read( (CHAR*)(&m_nComaReduce), sizeof(m_nComaReduce));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_HUSH_RESIST))
		iStream.Read( (CHAR*)(&m_nHushReduce), sizeof(m_nHushReduce));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_UNARM_RESIST))
		iStream.Read( (CHAR*)(&m_nUnarmReduce), sizeof(m_nUnarmReduce));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKSPEED_RESIST))
		iStream.Read( (CHAR*)(&m_nAttSpeedResist), sizeof(m_nAttSpeedResist));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_SKILLSPEED_RESIST))
		iStream.Read( (CHAR*)(&m_nSkillSpeedResist), sizeof(m_nSkillSpeedResist));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MOVEREDUCE_RESIST))
		iStream.Read( (CHAR*)(&m_nMoveSpeedResist), sizeof(m_nMoveSpeedResist));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_COUNTRY))
		iStream.Read( (CHAR*)(&m_nCountry), sizeof(m_nCountry));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MEMPAI))
		iStream.Read( (CHAR*)(&m_nMenPai), sizeof(m_nMenPai));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_GUILD))
		iStream.Read( (CHAR*)(&m_nGuild), sizeof(m_nGuild));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_GUILD_POINT))
		iStream.Read( (CHAR*)(&m_nGP), sizeof(m_nGP));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_PK_VALUE))
		iStream.Read( (CHAR*)(&m_nPK), sizeof(m_nPK));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_HONOR))
		iStream.Read( (CHAR*)(&m_nHonor), sizeof(m_nHonor));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_FAMILY))
		iStream.Read( (CHAR*)(&m_cFamilyID), sizeof(m_cFamilyID));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_GUILD_OFFICIAL))
		iStream.Read( (CHAR*)(&m_cGuildOfficial), sizeof(m_cGuildOfficial));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CAMP))
		iStream.Read( (CHAR*)(&m_CampData), sizeof(m_CampData));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DATAID))
		iStream.Read( (CHAR*)(&m_nDataID), sizeof(m_nDataID));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_PORTRAITID))
		iStream.Read( (CHAR*)(&m_nPortraitID), sizeof(m_nPortraitID));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MODELID))
		iStream.Read( (CHAR*)(&m_nModelID), sizeof(m_nModelID));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_FACEMODEL))
		iStream.Read( (CHAR*)(&m_nFaceModel), sizeof(m_nFaceModel));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MOUNTID))
		iStream.Read( (CHAR*)(&m_nMountID), sizeof(m_nMountID));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CURRENT_PET_GUID))
		iStream.Read( (CHAR*)(&m_guidCurrentPet), sizeof(m_guidCurrentPet));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_LIMIT_MOVE))
		iStream.Read( (CHAR*)(&m_bLimitMove), sizeof(m_bLimitMove));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CAN_ACTION1))
		iStream.Read( (CHAR*)(&m_bCanActionFlag1), sizeof(m_bCanActionFlag1));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CAN_ACTION2))
		iStream.Read( (CHAR*)(&m_bCanActionFlag2), sizeof(m_bCanActionFlag2));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_LIMIT_HANDLE))
	//	iStream.Read( (CHAR*)(&m_bLimitHandle), sizeof(m_bLimitHandle));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_VIGOR))
		iStream.Read( (CHAR*)(&m_Vigor), sizeof(m_Vigor));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAX_VIGOR))
		iStream.Read( (CHAR*)(&m_MaxVigor), sizeof(m_MaxVigor));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ENERGY))
		iStream.Read( (CHAR*)(&m_Energy), sizeof(m_Energy));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAX_ENERGY))
		iStream.Read( (CHAR*)(&m_MaxEnergy), sizeof(m_MaxEnergy));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_GOODBADVALUE))
		iStream.Read( (CHAR*)(&m_nGoodBadValue), sizeof(m_nGoodBadValue));

	iStream.Read( (CHAR*)(&m_TargetID), sizeof(ObjID_t) );


	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_HAVEDONE_FLAGS))
	//	iStream.Read( (CHAR*)(m_aMissionHaveDoneFlags), sizeof(m_aMissionHaveDoneFlags));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_KILLOBJECT_FLAGS))
	//	iStream.Read( (CHAR*)(m_aMissionKillObjectFlags),sizeof(m_aMissionKillObjectFlags));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_ENTERAREA_FLAGS))
	//	iStream.Read( (CHAR*)(m_aMissionEnterAreaFlags),sizeof(m_aMissionEnterAreaFlags));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_ITEMCHANGED_FLAGS))
	//	iStream.Read( (CHAR*)(m_aMissionItemChangedFlags),sizeof(m_aMissionItemChangedFlags));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL WGDetailOtherOnlineAttrib_Human::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION


		oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write( (CHAR*)(&m_Flags), sizeof(m_Flags));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_LEVEL))
		oStream.Write( (CHAR*)(&m_uLevel), sizeof(m_uLevel));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_HP))
		oStream.Write( (CHAR*)(&m_nHP), sizeof(m_nHP));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MP))
		oStream.Write( (CHAR*)(&m_nMP), sizeof(m_nMP));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_PNEUMA))
		oStream.Write( (CHAR*)(&m_nPneuma), sizeof(m_nPneuma));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAXHP))
		oStream.Write( (CHAR*)(&m_nMAXHP), sizeof(m_nMAXHP));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAXMP))
		oStream.Write( (CHAR*)(&m_nMAXMP), sizeof(m_nMAXMP));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAXPNEUMA))
		oStream.Write( (CHAR*)(&m_nMaxPneuma), sizeof(m_nMaxPneuma));


	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MOVESPEED))
		oStream.Write( (CHAR*)(&m_fMoveSpeed), sizeof(m_fMoveSpeed));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_EXP))
		oStream.Write( (CHAR*)(&m_nExp), sizeof(m_nExp));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MONEY))
		oStream.Write( (CHAR*)(&m_uMoney), sizeof(m_uMoney));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_STR))
		oStream.Write( (CHAR*)(&m_Str), sizeof(INT));
	/*if ( m_Flags.isSetBit(DETAIL_ATTRIB_SPR))
	oStream.Write( (CHAR*)(&m_Spr), sizeof(INT));*/
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CON))
		oStream.Write( (CHAR*)(&m_Con), sizeof(INT));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_INT))
		oStream.Write( (CHAR*)(&m_Int), sizeof(INT));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEX))
		oStream.Write( (CHAR*)(&m_Dex), sizeof(INT));


	if ( m_Flags.isSetBit(DETAIL_ATTRIB_POINT_REMAIN))
		oStream.Write( (CHAR*)(&m_nPoint_Remain), sizeof(m_nPoint_Remain));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_SKILLPOINT_REMAIN))
		oStream.Write( (CHAR*)(&m_nSkillPoint_Remain), sizeof(m_nSkillPoint_Remain));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_S_SKILLPOINT))
		oStream.Write( (CHAR*)(&m_nS_SkillPoints), sizeof(INT)*COMMON_S_NUMBER);

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_HP_RESPEED))
		oStream.Write( (CHAR*)(&m_nHP_ReSpeed), sizeof(m_nHP_ReSpeed));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MP_RESPEED))
		oStream.Write( (CHAR*)(&m_nMP_ReSpeed), sizeof(m_nMP_ReSpeed));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_VIGOR_RESPEED))
		oStream.Write( (CHAR*)(&m_nVigor_ReSpeed), sizeof(m_nVigor_ReSpeed));

	/*if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATT_PHYSICS))
	oStream.Write( (CHAR*)(&m_nAtt_Physics), sizeof(m_nAtt_Physics));*/
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATT_NEAR))
		oStream.Write( (CHAR*)(&m_nAtt_Near), sizeof(m_nAtt_Near));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATT_FAR))
		oStream.Write( (CHAR*)(&m_nAtt_Far), sizeof(m_nAtt_Far));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATT_MAGIC_NEAR))
		oStream.Write( (CHAR*)(&m_nAtt_MagicNear), sizeof(m_nAtt_MagicNear));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATT_MAGIC_FAR))
		oStream.Write( (CHAR*)(&m_nAtt_MagicFar), sizeof(m_nAtt_MagicFar));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEF_NEAR))
		oStream.Write( (CHAR*)(&m_nDef_Near), sizeof(m_nDef_Near));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEF_FAR))
		oStream.Write( (CHAR*)(&m_nDef_Far), sizeof(m_nDef_Far));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEF_MAGIC_NEAR))
		oStream.Write( (CHAR*)(&m_nDef_MagicNear), sizeof(m_nDef_MagicNear));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEF_MAGIC_FAR))
		oStream.Write( (CHAR*)(&m_nDef_MagicFar), sizeof(m_nDef_MagicFar));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_TOUGHNESS))
		oStream.Write( (CHAR*)(&m_nToughness), sizeof(m_nToughness));


	if ( m_Flags.isSetBit(DETAIL_ATTRIB_HIT))
		oStream.Write( (CHAR*)(&m_nHit), sizeof(m_nHit));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISS))
		oStream.Write( (CHAR*)(&m_nMiss), sizeof(m_nMiss));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CRITRATE))
		oStream.Write( (CHAR*)(&m_nCritic), sizeof(m_nCritic));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CRITICALHURT))
		oStream.Write( (CHAR*)(&m_nCriticHurt), sizeof(m_nCriticHurt));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAXRAGE))
		oStream.Write( (CHAR*)(&m_nMaxRage), sizeof(m_nMaxRage));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_RAGE))
		oStream.Write( (CHAR*)(&m_nRage), sizeof(m_nRage));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_RAGE_RESPEED))
		oStream.Write( (CHAR*)(&m_nRage_ReSpeed), sizeof(m_nRage_ReSpeed));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_STRIKE_POINT))
		oStream.Write( (CHAR*)(&m_nStrikePoint), sizeof(m_nStrikePoint));


	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKSPEED))
		oStream.Write( (CHAR*)(&m_nAttackSpeed), sizeof(m_nAttackSpeed));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACK_GOLD))
		oStream.Write( (CHAR*)(&m_nAttGold), sizeof(m_nAttGold));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCE_GOLD))
		oStream.Write( (CHAR*)(&m_nDefGold), sizeof(m_nDefGold));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACK_WOOD))
		oStream.Write ((CHAR*)(&m_nAttWood), sizeof(m_nAttWood));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCE_WOOD))
		oStream.Write( (CHAR*)(&m_nDefWood), sizeof(m_nDefWood));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACK_WATER))
		oStream.Write( (CHAR*)(&m_nAttWater), sizeof(m_nAttWater));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCE_WATER))
		oStream.Write( (CHAR*)(&m_nDefWater), sizeof(m_nDefWater));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACK_FIRE))
		oStream.Write( (CHAR*)(&m_nAttFire), sizeof(m_nAttFire));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCE_FIRE))
		oStream.Write( (CHAR*)(&m_nDefFire), sizeof(m_nDefFire));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACK_SOIL))
		oStream.Write ((CHAR*)(&m_nAttSoil), sizeof(m_nAttSoil));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCE_SOIL))
		oStream.Write( (CHAR*)(&m_nDefSoil), sizeof(m_nDefSoil));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_NEARATT_REDUCE))
		oStream.Write( (CHAR*)(&m_nNearReduce), sizeof(m_nNearReduce));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_FARATT_REDUCE))
		oStream.Write( (CHAR*)(&m_nFarReduce), sizeof(m_nFarReduce));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAGIC_NEAR_ATT_REDUCE))
		oStream.Write( (CHAR*)(&m_nMagicNearReduce), sizeof(m_nMagicNearReduce));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAGIC_FAR_ATT_REDUCE))
		oStream.Write( (CHAR*)(&m_nMagicFarReduce), sizeof(m_nMagicFarReduce));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DREAD_RESIST))
		oStream.Write( (CHAR*)(&m_nDreadReduce), sizeof(m_nDreadReduce));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_COMA_RESIST))
		oStream.Write( (CHAR*)(&m_nComaReduce), sizeof(m_nComaReduce));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_HUSH_RESIST))
		oStream.Write( (CHAR*)(&m_nHushReduce), sizeof(m_nHushReduce));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_UNARM_RESIST))
		oStream.Write( (CHAR*)(&m_nUnarmReduce), sizeof(m_nUnarmReduce));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKSPEED_RESIST))
		oStream.Write( (CHAR*)(&m_nAttSpeedResist), sizeof(m_nAttSpeedResist));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_SKILLSPEED_RESIST))
		oStream.Write( (CHAR*)(&m_nSkillSpeedResist), sizeof(m_nSkillSpeedResist));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MOVEREDUCE_RESIST))
		oStream.Write( (CHAR*)(&m_nMoveSpeedResist), sizeof(m_nMoveSpeedResist));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_COUNTRY))
		oStream.Write( (CHAR*)(&m_nCountry), sizeof(m_nCountry));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MEMPAI))
		oStream.Write( (CHAR*)(&m_nMenPai), sizeof(m_nMenPai));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_GUILD))
		oStream.Write( (CHAR*)(&m_nGuild), sizeof(m_nGuild));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_GUILD_POINT))
		oStream.Write( (CHAR*)(&m_nGP), sizeof(m_nGP));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_PK_VALUE))
		oStream.Write( (CHAR*)(&m_nPK), sizeof(m_nPK));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_HONOR))
		oStream.Write( (CHAR*)(&m_nHonor), sizeof(m_nHonor));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_FAMILY))
		oStream.Write( (CHAR*)(&m_cFamilyID), sizeof(m_cFamilyID));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_GUILD_OFFICIAL))
		oStream.Write( (CHAR*)(&m_cGuildOfficial), sizeof(m_cGuildOfficial));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CAMP))
		oStream.Write( (CHAR*)(&m_CampData), sizeof(m_CampData));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DATAID))
		oStream.Write( (CHAR*)(&m_nDataID), sizeof(m_nDataID));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_PORTRAITID))
		oStream.Write( (CHAR*)(&m_nPortraitID), sizeof(m_nPortraitID));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MODELID))
		oStream.Write( (CHAR*)(&m_nModelID), sizeof(m_nModelID));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_FACEMODEL))
		oStream.Write( (CHAR*)(&m_nFaceModel), sizeof(m_nFaceModel));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MOUNTID))
		oStream.Write( (CHAR*)(&m_nMountID), sizeof(m_nMountID));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CURRENT_PET_GUID))
		oStream.Write( (CHAR*)(&m_guidCurrentPet), sizeof(m_guidCurrentPet));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_LIMIT_MOVE))
		oStream.Write( (CHAR*)(&m_bLimitMove), sizeof(m_bLimitMove));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CAN_ACTION1))
		oStream.Write( (CHAR*)(&m_bCanActionFlag1), sizeof(m_bCanActionFlag1));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CAN_ACTION2))
		oStream.Write( (CHAR*)(&m_bCanActionFlag2), sizeof(m_bCanActionFlag2));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_LIMIT_HANDLE))
	//	oStream.Write( (CHAR*)(&m_bLimitHandle), sizeof(m_bLimitHandle));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_VIGOR))
		oStream.Write( (CHAR*)(&m_Vigor), sizeof(m_Vigor));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAX_VIGOR))
		oStream.Write( (CHAR*)(&m_MaxVigor), sizeof(m_MaxVigor));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ENERGY))
		oStream.Write( (CHAR*)(&m_Energy), sizeof(m_Energy));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAX_ENERGY))
		oStream.Write( (CHAR*)(&m_MaxEnergy), sizeof(m_MaxEnergy));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_GOODBADVALUE))
		oStream.Write( (CHAR*)(&m_nGoodBadValue), sizeof(m_nGoodBadValue));

	oStream.Write( (CHAR*)(&m_TargetID), sizeof(ObjID_t) );

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_HAVEDONE_FLAGS))
	//	oStream.Write( (CHAR*)(m_aMissionHaveDoneFlags), sizeof(m_aMissionHaveDoneFlags));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_KILLOBJECT_FLAGS))
	//	oStream.Write( (CHAR*)(m_aMissionKillObjectFlags),sizeof(m_aMissionKillObjectFlags));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_ENTERAREA_FLAGS))
	//	oStream.Write( (CHAR*)(m_aMissionEnterAreaFlags),sizeof(m_aMissionEnterAreaFlags));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_ITEMCHANGED_FLAGS))
	//	oStream.Write( (CHAR*)(m_aMissionItemChangedFlags),sizeof(m_aMissionItemChangedFlags));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}
/*
VOID WGDetailOtherOnlineAttrib_Human::FillParamBuf( VOID *pBuf )
{
UINT *aBuf	= (UINT*)(pBuf);
UINT **apdwBuf	= (UINT**)(pBuf);
BOOL *abBuf		= (BOOL*)(pBuf);
INT *anBuf		= (INT*)(pBuf);
FLOAT *afBuf	= (FLOAT*)(pBuf);
CHAR *apszBuf	= (CHAR*)(pBuf);

UINT uParamIndex;
uParamIndex = 0;
if ( m_Flags.isSetBit(DETAIL_ATTRIB_LEVEL) )		//0
aBuf[uParamIndex++] = m_uLevel;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_HP) )			//1
anBuf[uParamIndex++] = m_nHP;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_MP) )			//2
anBuf[uParamIndex++] = m_nMP;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_PNEUMA) )			//2
anBuf[uParamIndex++] = m_nPneuma;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_EXP) )			//3
anBuf[uParamIndex++] = m_nExp;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_MONEY) )		//4
aBuf[uParamIndex++] = m_uMoney;

//------------------------------------------------
if ( m_Flags.isSetBit( DETAIL_ATTRIB_STR) )			//5
anBuf[uParamIndex++] = m_Str;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_SPR) )			//6
anBuf[uParamIndex++] = m_Spr;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_CON) )			//7
anBuf[uParamIndex++] = m_Con;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_INT) )			//8
anBuf[uParamIndex++] = m_Int;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_DEX) )			//9
anBuf[uParamIndex++] = m_Dex;


if ( m_Flags.isSetBit( DETAIL_ATTRIB_POINT_REMAIN) )//10
anBuf[uParamIndex++] = m_nPoint_Remain;

//-------------------------------------------------
if ( m_Flags.isSetBit( DETAIL_ATTRIB_ATT_PHYSICS) )	//11
anBuf[uParamIndex++] = m_nAtt_Physics;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_DEF_PHYSICS) )	//12
anBuf[uParamIndex++] = m_nDef_Physics;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_ATT_MAGIC_NEAR) )	//13
anBuf[uParamIndex++] = m_nAtt_MagicNear;
if ( m_Flags.isSetBit( DETAIL_ATTRIB_ATT_MAGIC_FAR) )	//13
anBuf[uParamIndex++] = m_nAtt_MagicFar;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_DEF_MAGIC_NEAR) )	//14
anBuf[uParamIndex++] = m_nDef_MagicNear;
if ( m_Flags.isSetBit( DETAIL_ATTRIB_DEF_MAGIC_FAR) )	//14
anBuf[uParamIndex++] = m_nDef_MagicFar;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_MAXHP) )		//15
anBuf[uParamIndex++] = m_nMAXHP;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_MAXMP) )		//16
anBuf[uParamIndex++] = m_nMAXMP;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_HP_RESPEED) )	//17
anBuf[uParamIndex++] = m_nHP_ReSpeed;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_MP_RESPEED) )	//18
anBuf[uParamIndex++] = m_nMP_ReSpeed;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_HIT) )			//19
anBuf[uParamIndex++] = m_nHit;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_MISS) )		//20
anBuf[uParamIndex++] = m_nMiss;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_CRITRATE) )	//21
anBuf[uParamIndex++] = m_nCritic;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_RAGE) )	//22
anBuf[uParamIndex++] = m_nRage;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_STRIKE_POINT) )	//23
anBuf[uParamIndex++] = m_nStrikePoint;

if ( m_Flags.isSetBit( DETAIL_ATTRIB_MOVESPEED) )	//24
afBuf[uParamIndex++] = m_fMoveSpeed;

if(m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKSPEED))	//25
anBuf[uParamIndex++]= m_nAttackSpeed;

if(m_Flags.isSetBit(DETAIL_ATTRIB_ATTACK_GOLD))	//26
anBuf[uParamIndex++]= m_nAttGold;

if(m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCE_GOLD))	//27
anBuf[uParamIndex++]= m_nDefGold;

if(m_Flags.isSetBit(DETAIL_ATTRIB_ATTACK_WOOD))	//30
anBuf[uParamIndex++]= m_nAttWood;

if(m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCE_WOOD))	//31
anBuf[uParamIndex++]= m_nDefWood;

if(m_Flags.isSetBit(DETAIL_ATTRIB_ATTACK_WATER))	//32
anBuf[uParamIndex++]= m_nAttWater;

if(m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCE_WATER))	//33
anBuf[uParamIndex++]= m_nDefWater;

if(m_Flags.isSetBit(DETAIL_ATTRIB_ATTACK_FIRE))	//28
anBuf[uParamIndex++]= m_nAttFire;

if(m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCE_FIRE))	//29
anBuf[uParamIndex++]= m_nDefFire;

if(m_Flags.isSetBit(DETAIL_ATTRIB_ATTACK_SOIL))	//30
anBuf[uParamIndex++]= m_nAttSoil;

if(m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCE_SOIL))	//31
anBuf[uParamIndex++]= m_nDefSoil;

if(m_Flags.isSetBit(DETAIL_ATTRIB_MEMPAI)	)		//34
anBuf[uParamIndex++]= m_nMenPai;

if(m_Flags.isSetBit(DETAIL_ATTRIB_CAMP)	)			//35
anBuf[uParamIndex++]= m_nCamp;

if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_HAVEDONE_FLAGS) )
apdwBuf[uParamIndex++]	= &(m_aMissionHaveDoneFlags[0]);

if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_KILLOBJECT_FLAGS))
apdwBuf[uParamIndex++]	= &(m_aMissionKillObjectFlags[0]);

if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_ENTERAREA_FLAGS))
apdwBuf[uParamIndex++] = &(m_aMissionEnterAreaFlags[0]);

if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_ITEMCHANGED_FLAGS))
apdwBuf[uParamIndex++] = &(m_aMissionItemChangedFlags[0]);

}
*/
UINT WGDetailOtherOnlineAttrib_Human::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return WGDetailOtherOnlineAttrib_HumanHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


VOID WGDetailOtherOnlineAttrib_Human::SetTargetID( ObjID_t objID )
{
	m_TargetID = objID;
}

}

