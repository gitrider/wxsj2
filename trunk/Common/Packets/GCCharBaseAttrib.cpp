
#include "stdafx.h"
#include "GCCharBaseAttrib.h"

BOOL GCCharBaseAttrib::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_uFlags), sizeof(UINT));

	if ( m_uFlags & (1 << UPDATE_CHAR_ATT_JOB) )
		iStream.Read( (CHAR*)(&m_Job), sizeof(m_Job));

	if ( m_uFlags & (1 << UPDATE_CHAR_ATT_COUNTRY) )
		iStream.Read( (CHAR*)(&m_Country), sizeof(m_Country));

	if ( m_uFlags & (1 << UPDATE_CHAR_ATT_DATA_ID) )
		iStream.Read( (CHAR*)(&m_wDataID), sizeof(m_wDataID));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_NAME) )
	{
		iStream.Read( (CHAR*)(&m_byNameSize), sizeof(m_byNameSize));
		iStream.Read( (CHAR*)(m_szName), sizeof(CHAR) * m_byNameSize);
		m_szName[m_byNameSize] = '\0';
	}

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_COUNTRYTITLE) )
	{
		iStream.Read( (CHAR*)(&m_CountryTitleType), sizeof(m_CountryTitleType));
		iStream.Read( (CHAR*)(&m_byCountryTitleSize), sizeof(m_byCountryTitleSize));
		iStream.Read( (CHAR*)(m_szCountryTitle), sizeof(CHAR) * m_byCountryTitleSize);
		m_szCountryTitle[m_byCountryTitleSize] = '\0';
	}

    if ( m_uFlags & (1<<UPDATE_CHAR_ATT_GUILDTITLE) )
    {
        iStream.Read( (CHAR*)(&m_GuildTitleType), sizeof(m_GuildTitleType));
        iStream.Read( (CHAR*)(&m_byGuildTitleSize), sizeof(m_byGuildTitleSize));
        iStream.Read( (CHAR*)(m_szGuildTitle), sizeof(CHAR) * m_byGuildTitleSize);
        m_szGuildTitle[m_byGuildTitleSize] = '\0';
    }

    if ( m_uFlags & (1<<UPDATE_CHAR_ATT_NORMALTITLE) )
    {
        iStream.Read( (CHAR*)(&m_NormalTitleType), sizeof(m_NormalTitleType));
        iStream.Read( (CHAR*)(&m_byNormalTitleSize), sizeof(m_byNormalTitleSize));
        iStream.Read( (CHAR*)(m_szNormalTitle), sizeof(CHAR) * m_byNormalTitleSize);
        m_szNormalTitle[m_byNormalTitleSize] = '\0';
    }

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_LEVEL) )
		iStream.Read( (CHAR*)(&m_Level), sizeof(m_Level));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_HP_PERCENT) )
		iStream.Read( (CHAR*)(&m_HPPercent), sizeof(m_HPPercent));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MP_PERCENT) )
		iStream.Read( (CHAR*)(&m_MPPercent), sizeof(m_MPPercent));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_PNEUMA_PERCENT) )
		iStream.Read( (CHAR*)(&m_PneumaPercent), sizeof(m_PneumaPercent));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_RAGE) )
		iStream.Read( (CHAR*)(&m_nRage), sizeof(m_nRage));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_STEALTH_LEVEL) )
		iStream.Read( (CHAR*)(&m_nStealthLevel), sizeof(m_nStealthLevel));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_SIT) )
		iStream.Read( (CHAR*)(&m_cMoodState), sizeof(m_cMoodState));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MOVE_SPEED) )
		iStream.Read( (CHAR*)(&m_fMoveSpeed), sizeof(m_fMoveSpeed));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_ATTACK_SPEED) )
		iStream.Read( (CHAR*)(&m_fAttackSpeed), sizeof(m_fAttackSpeed));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_CAMP_ID) )
		iStream.Read( (CHAR*)(&m_CampData), sizeof(m_CampData));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_PORTRAIT_ID) )
		iStream.Read( (CHAR*)(&m_nPortraitID), sizeof(m_nPortraitID));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MODEL_ID) )
		iStream.Read( (CHAR*)(&m_nModelID), sizeof(m_nModelID));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MOUNT_ID) )
		iStream.Read( (CHAR*)(&m_nMountID), sizeof(m_nMountID));

    //  [7/23/2010 ³Â¾üÁú]
	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MOUNT_INDEX) )
		iStream.Read( (CHAR*)(&m_nMountID), sizeof(m_nMountIndex));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_AITYPE) )
		iStream.Read( (CHAR*)(&m_nAIType), sizeof(m_nAIType));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_PLAYER_DATA) )
	{
		iStream.Read( (CHAR*)(&m_uPlayerData), sizeof(m_uPlayerData));
		iStream.Read( (CHAR*)(&m_HairColor), sizeof(m_HairColor));
	}

	if ( m_uFlags & (1<<UPDATE_CHAR_IS_IN_STALL) )
		iStream.Read( (CHAR*)(&m_bStallIsOpen), sizeof(m_bStallIsOpen));

	if ( m_uFlags & (1<<UPDATE_CHAR_STALL_PUBLIC) )
		iStream.Read( (CHAR*)(&m_bOpenPublic), sizeof(m_bOpenPublic));

	if ( m_uFlags & (1<<UPDATE_CHAR_STALL_NAME) )
	{
		iStream.Read( (CHAR*)(&m_nStallNameSize), sizeof(m_nStallNameSize));
		Assert(m_nStallNameSize<MAX_STALL_NAME ) ;
		iStream.Read( (CHAR*)(m_szStallName), m_nStallNameSize);
		m_szStallName[m_nStallNameSize] = '\0';
	}

	if ( m_uFlags & (1<<UPDATE_CHAR_OCCUPANT) )
		iStream.Read( (CHAR*)(&m_OccupantGUID), sizeof(m_OccupantGUID));

	if ( m_uFlags & (1<<UPDATE_CHAR_OWNER) )
		iStream.Read( (CHAR*)(&m_OwnerID), sizeof(m_OwnerID));

	if ( m_uFlags & (1<<UPDATE_CHAR_BUSOBJID) )
		iStream.Read( (CHAR*)(&m_BusID), sizeof(m_BusID));

	

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharBaseAttrib::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_uFlags), sizeof(UINT));

	if ( m_uFlags & (1 << UPDATE_CHAR_ATT_JOB) )
		oStream.Write( (CHAR*)(&m_Job), sizeof(m_Job));

	if ( m_uFlags & (1 << UPDATE_CHAR_ATT_COUNTRY) )
		oStream.Write( (CHAR*)(&m_Country), sizeof(m_Country));

	if ( m_uFlags & (1 << UPDATE_CHAR_ATT_DATA_ID) )
		oStream.Write( (CHAR*)(&m_wDataID), sizeof(m_wDataID));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_NAME) )
	{
		oStream.Write( (CHAR*)(&m_byNameSize), sizeof(m_byNameSize));
		oStream.Write( (CHAR*)(m_szName), sizeof(CHAR) * m_byNameSize);
	}

    if ( m_uFlags & (1<<UPDATE_CHAR_ATT_COUNTRYTITLE) )
    {
        oStream.Write( (CHAR*)(&m_CountryTitleType), sizeof(m_CountryTitleType));
        oStream.Write( (CHAR*)(&m_byCountryTitleSize), sizeof(m_byCountryTitleSize));
        oStream.Write( (CHAR*)(m_szCountryTitle), sizeof(CHAR) * m_byCountryTitleSize);
    }

    if ( m_uFlags & (1<<UPDATE_CHAR_ATT_GUILDTITLE) )
    {
        oStream.Write( (CHAR*)(&m_GuildTitleType), sizeof(m_GuildTitleType));
        oStream.Write( (CHAR*)(&m_byGuildTitleSize), sizeof(m_byGuildTitleSize));
        oStream.Write( (CHAR*)(m_szGuildTitle), sizeof(CHAR) * m_byGuildTitleSize);
    }

    if ( m_uFlags & (1<<UPDATE_CHAR_ATT_NORMALTITLE) )
    {
        oStream.Write( (CHAR*)(&m_NormalTitleType), sizeof(m_NormalTitleType));
        oStream.Write( (CHAR*)(&m_byNormalTitleSize), sizeof(m_byNormalTitleSize));
        oStream.Write( (CHAR*)(m_szNormalTitle), sizeof(CHAR) * m_byNormalTitleSize);
    }

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_LEVEL) )
		oStream.Write( (CHAR*)(&m_Level), sizeof(m_Level));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_HP_PERCENT) )
		oStream.Write( (CHAR*)(&m_HPPercent), sizeof(m_HPPercent));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MP_PERCENT) )
		oStream.Write( (CHAR*)(&m_MPPercent), sizeof(m_MPPercent));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_PNEUMA_PERCENT) )
		oStream.Write( (CHAR*)(&m_PneumaPercent), sizeof(m_PneumaPercent));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_RAGE) )
		oStream.Write( (CHAR*)(&m_nRage), sizeof(m_nRage));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_STEALTH_LEVEL) )
		oStream.Write( (CHAR*)(&m_nStealthLevel), sizeof(m_nStealthLevel));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_SIT) )
		oStream.Write( (CHAR*)(&m_cMoodState), sizeof(m_cMoodState));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MOVE_SPEED) )
		oStream.Write( (CHAR*)(&m_fMoveSpeed), sizeof(m_fMoveSpeed));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_ATTACK_SPEED) )
		oStream.Write( (CHAR*)(&m_fAttackSpeed), sizeof(m_fAttackSpeed));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_CAMP_ID) )
		oStream.Write( (CHAR*)(&m_CampData), sizeof(m_CampData));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_PORTRAIT_ID) )
		oStream.Write( (CHAR*)(&m_nPortraitID), sizeof(m_nPortraitID));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MODEL_ID) )
		oStream.Write( (CHAR*)(&m_nModelID), sizeof(m_nModelID));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MOUNT_ID) )
		oStream.Write( (CHAR*)(&m_nMountID), sizeof(m_nMountID));

    //  [7/23/2010 ³Â¾üÁú]
	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MOUNT_INDEX) )
		oStream.Write( (CHAR*)(&m_nMountID), sizeof(m_nMountIndex));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_AITYPE) )
		oStream.Write( (CHAR*)(&m_nAIType), sizeof(m_nAIType));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_PLAYER_DATA) )
	{
		oStream.Write( (CHAR*)(&m_uPlayerData), sizeof(m_uPlayerData));
		oStream.Write( (CHAR*)(&m_HairColor), sizeof(m_HairColor));
	}

	if ( m_uFlags & (1<<UPDATE_CHAR_IS_IN_STALL) )
		oStream.Write( (CHAR*)(&m_bStallIsOpen), sizeof(m_bStallIsOpen));

	if ( m_uFlags & (1<<UPDATE_CHAR_STALL_PUBLIC) )
		oStream.Write( (CHAR*)(&m_bOpenPublic), sizeof(m_bOpenPublic));

	if ( m_uFlags & (1<<UPDATE_CHAR_STALL_NAME) )
	{
		oStream.Write( (CHAR*)(&m_nStallNameSize), sizeof(m_nStallNameSize));
		oStream.Write( (CHAR*)(m_szStallName), m_nStallNameSize);
		Assert(m_nStallNameSize<MAX_STALL_NAME) ;
	}

	if ( m_uFlags & (1<<UPDATE_CHAR_OCCUPANT) )
		oStream.Write( (CHAR*)(&m_OccupantGUID), sizeof(m_OccupantGUID));

	if ( m_uFlags & (1<<UPDATE_CHAR_OWNER) )
		oStream.Write( (CHAR*)(&m_OwnerID), sizeof(m_OwnerID));

	if ( m_uFlags & (1<<UPDATE_CHAR_BUSOBJID) )
		oStream.Write( (CHAR*)(&m_BusID), sizeof(m_BusID));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCharBaseAttrib::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCharBaseAttribHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

