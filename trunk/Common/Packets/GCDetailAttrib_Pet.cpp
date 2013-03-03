// GCDetailAttrib_Pet.cpp

#include "stdafx.h"
#include "GCDetailAttrib_Pet.h"

using namespace Packets;

BOOL GCDetailAttrib_Pet::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID));
	iStream.Read( (CHAR*)(&m_nTradeIndex), sizeof(m_nTradeIndex));
	iStream.Read( (CHAR*)(&m_iHorseState), sizeof(m_iHorseState));
	iStream.Read( (CHAR*)(&m_Flags), sizeof(m_Flags));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_OBJ_ID ) )
		iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_DATA_ID ) )
		iStream.Read( (CHAR*)(&m_nDataID), sizeof(m_nDataID));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_NAME ) )
	{
		iStream.Read( (CHAR*)(&m_byNameSize), sizeof(m_byNameSize));
		iStream.Read( (CHAR*)(m_szName), sizeof(CHAR) * m_byNameSize);
		m_szName[m_byNameSize] = '\0';
	}
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_NICK_NAME ) )
	{
		iStream.Read( (CHAR*)(&m_byNickNameSize), sizeof(m_byNickNameSize));
		iStream.Read( (CHAR*)(m_szNickName), sizeof(CHAR) * m_byNickNameSize);
		m_szNickName[m_byNickNameSize] = '\0';
	}
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_AI_TYPE ) )
		iStream.Read( (CHAR*)(&m_nAIType), sizeof(m_nAIType));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_SPOUSE_GUID ) )
		iStream.Read( (CHAR*)(&m_SpouseGUID), sizeof(m_SpouseGUID));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_LEVEL ) )
		iStream.Read( (CHAR*)(&m_nLevel), sizeof(m_nLevel));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_EXP ) )
		iStream.Read( (CHAR*)(&m_nExp), sizeof(m_nExp));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_HP ) )
		iStream.Read( (CHAR*)(&m_nHP), sizeof(m_nHP));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_HPMAX ) )
		iStream.Read( (CHAR*)(&m_nHPMax), sizeof(m_nHPMax));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_LIFE ) )
		iStream.Read( (CHAR*)(&m_nLife), sizeof(m_nLife));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_GENERATION ) )
		iStream.Read( (CHAR*)(&m_byGeneration), sizeof(m_byGeneration));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_HAPPINESS ) )
		iStream.Read( (CHAR*)(&m_byHappiness), sizeof(m_byHappiness));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_ATT_NEAR ) )
		iStream.Read( (CHAR*)(&m_nAtt_Near), sizeof(m_nAtt_Near));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_ATT_FAR ) )
		iStream.Read( (CHAR*)(&m_nAtt_Far), sizeof(m_nAtt_Far));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_ATT_MAGIC_NEAR ) )
		iStream.Read( (CHAR*)(&m_nAtt_MagicNear), sizeof(m_nAtt_MagicNear));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_ATT_MAGIC_FAR ) )
		iStream.Read( (CHAR*)(&m_nAtt_MagicFar), sizeof(m_nAtt_MagicFar));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_DEF_NEAR) )
		iStream.Read( (CHAR*)(&m_nDef_Near), sizeof(m_nDef_Near));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_DEF_FAR) )
		iStream.Read( (CHAR*)(&m_nDef_Far), sizeof(m_nDef_Far));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_DEF_MAGIC_NEAR) )
		iStream.Read( (CHAR*)(&m_nDef_MagicNear), sizeof(m_nDef_MagicNear));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_DEF_MAGIC_FAR) )
		iStream.Read( (CHAR*)(&m_nDef_MagicFar), sizeof(m_nDef_MagicFar));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_HIT ) )
		iStream.Read( (CHAR*)(&m_nHit), sizeof(m_nHit));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_MISS ) )
		iStream.Read( (CHAR*)(&m_nMiss), sizeof(m_nMiss));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_CRITICAL ) )
		iStream.Read( (CHAR*)(&m_nCritical), sizeof(m_nCritical));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_MODELID ) )
		iStream.Read( (CHAR*)(&m_nModelID), sizeof(m_nModelID));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_MOUNTID ) )
		iStream.Read( (CHAR*)(&m_nMountID), sizeof(m_nMountID));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_STRPERCEPTION ) )
		iStream.Read( (CHAR*)(&m_StrPerception), sizeof(m_StrPerception));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_CONPERCEPTION ) )
		iStream.Read( (CHAR*)(&m_ConPerception), sizeof(m_ConPerception));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_DEXPERCEPTION ) )
		iStream.Read( (CHAR*)(&m_DexPerception), sizeof(m_DexPerception));
	/*if ( m_Flags.isSetBit(PET_DETAIL_ATTR_SPRPERCEPTION ) )
		iStream.Read( (CHAR*)(&m_SprPerception), sizeof(m_SprPerception));*/
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_INTPERCEPTION ) )
		iStream.Read( (CHAR*)(&m_IntPerception), sizeof(m_IntPerception));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_STR ) )
		iStream.Read( (CHAR*)(&m_Str), sizeof(m_Str));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_CON ) )
		iStream.Read( (CHAR*)(&m_Con), sizeof(m_Con));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_DEX ) )
		iStream.Read( (CHAR*)(&m_Dex), sizeof(m_Dex));
	/*if ( m_Flags.isSetBit(PET_DETAIL_ATTR_SPR ) )
		iStream.Read( (CHAR*)(&m_Spr), sizeof(m_Spr));*/
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_INT ) )
		iStream.Read( (CHAR*)(&m_Int), sizeof(m_Int));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_GENGU ) )
		iStream.Read( (CHAR*)(&m_GenGu), sizeof(m_GenGu));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_POINT_REMAIN ) )
		iStream.Read( (CHAR*)(&m_nRemainPoint), sizeof(m_nRemainPoint));

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_0 ) )
		iStream.Read( (CHAR*)(&m_aSkill[0]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_1 ) )
		iStream.Read( (CHAR*)(&m_aSkill[1]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_2 ) )
		iStream.Read( (CHAR*)(&m_aSkill[2]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_3 ) )
		iStream.Read( (CHAR*)(&m_aSkill[3]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_4 ) )
		iStream.Read( (CHAR*)(&m_aSkill[4]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_5 ) )
		iStream.Read( (CHAR*)(&m_aSkill[5]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_6 ) )
		iStream.Read( (CHAR*)(&m_aSkill[6]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_7 ) )
		iStream.Read( (CHAR*)(&m_aSkill[7]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_8 ) )
		iStream.Read( (CHAR*)(&m_aSkill[8]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_9 ) )
		iStream.Read( (CHAR*)(&m_aSkill[9]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_10 ) )
		iStream.Read( (CHAR*)(&m_aSkill[10]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_11 ) )
		iStream.Read( (CHAR*)(&m_aSkill[11]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SAVVY ) )
		iStream.Read( (CHAR*)(&m_iSavvy), sizeof( m_iSavvy ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_GROWRATE ) )
		iStream.Read( (CHAR*)(&m_iGrowRate), sizeof( m_iGrowRate ) );
	
	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SEX ) )
		iStream.Read( (CHAR*)(&m_iSex), sizeof( m_iSex ) );

	iStream.Read( (CHAR*)(&m_ExtraInfoLength), sizeof(BYTE));
	iStream.Read( (CHAR*)(m_ExtraInfoData), m_ExtraInfoLength);

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCDetailAttrib_Pet::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID));
	oStream.Write( (CHAR*)(&m_nTradeIndex), sizeof(m_nTradeIndex));
	oStream.Write( (CHAR*)(&m_iHorseState), sizeof(m_iHorseState));
	oStream.Write( (CHAR*)(&m_Flags), sizeof(m_Flags));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_OBJ_ID ) )
		oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_DATA_ID ) )
		oStream.Write( (CHAR*)(&m_nDataID), sizeof(m_nDataID));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_NAME ) )
	{
		oStream.Write( (CHAR*)(&m_byNameSize), sizeof(m_byNameSize));
		oStream.Write( (CHAR*)(m_szName), sizeof(CHAR) * m_byNameSize);
	}

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_NICK_NAME ) )
	{
		oStream.Write( (CHAR*)(&m_byNickNameSize), sizeof(m_byNickNameSize));
		oStream.Write( (CHAR*)(m_szNickName), sizeof(CHAR) * m_byNickNameSize);
	}

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_AI_TYPE ) )
		oStream.Write( (CHAR*)(&m_nAIType), sizeof(m_nAIType));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_SPOUSE_GUID ) )
		oStream.Write( (CHAR*)(&m_SpouseGUID), sizeof(m_SpouseGUID));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_LEVEL ) )
		oStream.Write( (CHAR*)(&m_nLevel), sizeof(m_nLevel));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_EXP ) )
		oStream.Write( (CHAR*)(&m_nExp), sizeof(m_nExp));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_HP ) )
		oStream.Write( (CHAR*)(&m_nHP), sizeof(m_nHP));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_HPMAX ) )
		oStream.Write( (CHAR*)(&m_nHPMax), sizeof(m_nHPMax));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_LIFE ) )
		oStream.Write( (CHAR*)(&m_nLife), sizeof(m_nLife));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_GENERATION ) )
		oStream.Write( (CHAR*)(&m_byGeneration), sizeof(m_byGeneration));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_HAPPINESS ) )
		oStream.Write( (CHAR*)(&m_byHappiness), sizeof(m_byHappiness));

	/*if ( m_Flags.isSetBit(PET_DETAIL_ATTR_ATT_PHYSICS ) )
		oStream.Write( (CHAR*)(&m_nAtt_Physics), sizeof(m_nAtt_Physics));*/
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_ATT_NEAR ) )
		oStream.Write( (CHAR*)(&m_nAtt_Near), sizeof(m_nAtt_Near));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_ATT_FAR ) )
		oStream.Write( (CHAR*)(&m_nAtt_Far), sizeof(m_nAtt_Far));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_ATT_MAGIC_NEAR ) )
		oStream.Write( (CHAR*)(&m_nAtt_MagicNear), sizeof(m_nAtt_MagicNear));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_ATT_MAGIC_FAR ) )
		oStream.Write( (CHAR*)(&m_nAtt_MagicFar), sizeof(m_nAtt_MagicFar));

	/*if ( m_Flags.isSetBit(PET_DETAIL_ATTR_DEF_PHYSICS ) )
		oStream.Write( (CHAR*)(&m_nDef_Physics), sizeof(m_nDef_Physics));*/
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_DEF_NEAR ) )
		oStream.Write( (CHAR*)(&m_nDef_Near), sizeof(m_nDef_Near));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_DEF_FAR ) )
		oStream.Write( (CHAR*)(&m_nDef_Far), sizeof(m_nDef_Far));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_DEF_MAGIC_NEAR) )
		oStream.Write( (CHAR*)(&m_nDef_MagicNear), sizeof(m_nDef_MagicNear));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_DEF_MAGIC_FAR) )
		oStream.Write( (CHAR*)(&m_nDef_MagicFar), sizeof(m_nDef_MagicFar));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_HIT ) )
		oStream.Write( (CHAR*)(&m_nHit), sizeof(m_nHit));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_MISS ) )
		oStream.Write( (CHAR*)(&m_nMiss), sizeof(m_nMiss));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_CRITICAL ) )
		oStream.Write( (CHAR*)(&m_nCritical), sizeof(m_nCritical));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_MODELID ) )
		oStream.Write( (CHAR*)(&m_nModelID), sizeof(m_nModelID));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_MOUNTID ) )
		oStream.Write( (CHAR*)(&m_nMountID), sizeof(m_nMountID));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_STRPERCEPTION ) )
		oStream.Write( (CHAR*)(&m_StrPerception), sizeof(m_StrPerception));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_CONPERCEPTION ) )
		oStream.Write( (CHAR*)(&m_ConPerception), sizeof(m_ConPerception));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_DEXPERCEPTION ) )
		oStream.Write( (CHAR*)(&m_DexPerception), sizeof(m_DexPerception));
	/*if ( m_Flags.isSetBit(PET_DETAIL_ATTR_SPRPERCEPTION ) )
		oStream.Write( (CHAR*)(&m_SprPerception), sizeof(m_SprPerception));*/
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_INTPERCEPTION ) )
		oStream.Write( (CHAR*)(&m_IntPerception), sizeof(m_IntPerception));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_STR ) )
		oStream.Write( (CHAR*)(&m_Str), sizeof(m_Str));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_CON ) )
		oStream.Write( (CHAR*)(&m_Con), sizeof(m_Con));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_DEX ) )
		oStream.Write( (CHAR*)(&m_Dex), sizeof(m_Dex));
	/*if ( m_Flags.isSetBit(PET_DETAIL_ATTR_SPR ) )
		oStream.Write( (CHAR*)(&m_Spr), sizeof(m_Spr));*/
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_INT ) )
		oStream.Write( (CHAR*)(&m_Int), sizeof(m_Int));
	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_GENGU ) )
		oStream.Write( (CHAR*)(&m_GenGu), sizeof(m_GenGu));

	if ( m_Flags.isSetBit(PET_DETAIL_ATTR_POINT_REMAIN ) )
		oStream.Write( (CHAR*)(&m_nRemainPoint), sizeof(m_nRemainPoint));

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_0 ) )
		oStream.Write( (CHAR*)(&m_aSkill[0]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_1 ) )
		oStream.Write( (CHAR*)(&m_aSkill[1]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_2 ) )
		oStream.Write( (CHAR*)(&m_aSkill[2]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_3 ) )
		oStream.Write( (CHAR*)(&m_aSkill[3]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_4 ) )
		oStream.Write( (CHAR*)(&m_aSkill[4]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_5 ) )
		oStream.Write( (CHAR*)(&m_aSkill[5]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_6 ) )
		oStream.Write( (CHAR*)(&m_aSkill[6]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_7 ) )
		oStream.Write( (CHAR*)(&m_aSkill[7]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_8 ) )
		oStream.Write( (CHAR*)(&m_aSkill[8]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_9 ) )
		oStream.Write( (CHAR*)(&m_aSkill[9]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_10 ) )
		oStream.Write( (CHAR*)(&m_aSkill[10]), sizeof( _PET_SKILL ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SKILL_11 ) )
		oStream.Write( (CHAR*)(&m_aSkill[11]), sizeof( _PET_SKILL ) );



	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SAVVY ) )
		oStream.Write( (CHAR*)(&m_iSavvy), sizeof( m_iSavvy ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_GROWRATE ) )
		oStream.Write( (CHAR*)(&m_iGrowRate), sizeof( m_iGrowRate ) );

	if ( m_Flags.isSetBit( PET_DETAIL_ATTR_SEX ) )
		oStream.Write( (CHAR*)(&m_iSex), sizeof( m_iSex ) );

	oStream.Write( (CHAR*)(&m_ExtraInfoLength), sizeof(BYTE));
	oStream.Write( (CHAR*)(m_ExtraInfoData), m_ExtraInfoLength);

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCDetailAttrib_Pet::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCDetailAttrib_PetHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


const CHAR* Packets::GCDetailAttrib_Pet::GetName( VOID ) const
{
	return m_szName;
}

const CHAR* Packets::GCDetailAttrib_Pet::GetNickName() const
{
	return m_szNickName;
}