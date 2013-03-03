
#include "stdafx.h"
#include "GCCharEquipment.h"

BOOL GCCharEquipment::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_wFlags), sizeof(UINT));

	if ( m_wFlags & (1 << HEQUIP_MAINHAND) )
		iStream.Read( (CHAR*)(&m_MainWeapon), sizeof(UINT));
	if ( m_wFlags & (1 << HEQUIP_ASSIHAND) )
		iStream.Read( (CHAR*)(&m_AssiWeapon), sizeof(UINT));
	if ( m_wFlags & (1 << HEQUIP_HEAD) )
		iStream.Read( (CHAR*)(&m_Cap), sizeof(UINT));
	if ( m_wFlags & (1 << HEQUIP_SHOULDER) )
		iStream.Read( (CHAR*)(&m_Shoulder), sizeof(UINT));
	if ( m_wFlags & (1 << HEQUIP_BACK) )
		iStream.Read( (CHAR*)(&m_Armour), sizeof(UINT));
	if ( m_wFlags & (1 << HEQUIP_HAND) )
		iStream.Read( (CHAR*)(&m_Hand), sizeof(UINT));
	if ( m_wFlags & (1 << HEQUIP_FEET) )
		iStream.Read( (CHAR*)(&m_Foot), sizeof(UINT));
	if ( m_wFlags & (1 << HEQUIP_SUIT) )
		iStream.Read( (CHAR*)(&m_Suit), sizeof(UINT));
	if ( m_wFlags & (1 << HEQUIP_RIDER) )
		iStream.Read( (CHAR*)(&m_Rider), sizeof(UINT));
	if ( m_wFlags & (1 << HEQUIP_SOUXIA) )
		iStream.Read( (CHAR*)(&m_Souxia), sizeof(UINT));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharEquipment::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_wFlags), sizeof(UINT));

	if ( m_wFlags & (1 << HEQUIP_MAINHAND) )
		oStream.Write( (CHAR*)(&m_MainWeapon), sizeof(UINT));
	if ( m_wFlags & (1 << HEQUIP_ASSIHAND) )
		oStream.Write( (CHAR*)(&m_AssiWeapon), sizeof(UINT));
	if ( m_wFlags & (1 << HEQUIP_HEAD) )
		oStream.Write( (CHAR*)(&m_Cap), sizeof(UINT));
	if ( m_wFlags & (1 << HEQUIP_SHOULDER) )
		oStream.Write( (CHAR*)(&m_Shoulder), sizeof(UINT));
	if ( m_wFlags & (1 << HEQUIP_BACK) )
		oStream.Write( (CHAR*)(&m_Armour), sizeof(UINT));
	if ( m_wFlags & (1 << HEQUIP_HAND) )
		oStream.Write( (CHAR*)(&m_Hand), sizeof(UINT));
	if ( m_wFlags & (1 << HEQUIP_FEET) )
		oStream.Write( (CHAR*)(&m_Foot), sizeof(UINT));
	if ( m_wFlags & (1 << HEQUIP_SUIT) )
		oStream.Write( (CHAR*)(&m_Suit), sizeof(UINT));
	if ( m_wFlags & (1 << HEQUIP_RIDER) )
		oStream.Write( (CHAR*)(&m_Rider), sizeof(UINT));
	if ( m_wFlags & (1 << HEQUIP_SOUXIA) )
		oStream.Write( (CHAR*)(&m_Souxia), sizeof(UINT));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID GCCharEquipment::FillParamBuf( VOID *pBuf )
{
	UINT *aBuf	= (UINT*)(pBuf);

	UINT uParamIndex;
	uParamIndex = 0;

	if ( m_wFlags & (1 << HEQUIP_MAINHAND) )
	{
		aBuf[uParamIndex++] = m_MainWeapon;
	}

	if ( m_wFlags & (1 << HEQUIP_ASSIHAND) )
	{
		aBuf[uParamIndex++] = m_AssiWeapon;
	}

	if ( m_wFlags & (1<<HEQUIP_HEAD) )
	{
		aBuf[uParamIndex++] = m_Cap;
	}

	if ( m_wFlags & (1<<HEQUIP_SHOULDER) )
	{
		aBuf[uParamIndex++] = m_Shoulder;
	}

	if ( m_wFlags & (1<<HEQUIP_BACK) )
	{
		aBuf[uParamIndex++] = m_Armour;
	}

	if ( m_wFlags & (1<<HEQUIP_HAND) )
	{
		aBuf[uParamIndex++] = m_Hand;
	}

	if ( m_wFlags & (1<<HEQUIP_FEET) )
	{
		aBuf[uParamIndex++] = m_Foot;
	}

	if ( m_wFlags & (1<<HEQUIP_SUIT) )
	{
		aBuf[uParamIndex++] = m_Suit;
	}

	if ( m_wFlags & (1<<HEQUIP_RIDER) )
	{
		aBuf[uParamIndex++] = m_Rider;
	}
	if ( m_wFlags & (1<<HEQUIP_SOUXIA) )
	{
		aBuf[uParamIndex++] = m_Souxia;
	}
}

UINT GCCharEquipment::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCharEquipmentHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

