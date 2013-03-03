// AbilityFoundry.cpp

#include "stdafx.h"
#include "AbilityFoundry.h"

AbilityFoundry::AbilityFoundry(const Ability& a) : AbilityCompound(a)
{
}

BOOL AbilityFoundry::ValidateSpecStuff( UINT uItemIndex )
{
__ENTER_FUNCTION

	/*if ( GetSerialClass( uItemIndex ) == ICLASS_MATERIAL
		&& uItemIndex >= ITEM_JINGLIAN_ITEM_START
		&& uItemIndex <= ITEM_JINGLIAN_ITEM_END
	  )
	{
		COMMITEM_INFO_TB* pTB = g_ItemTable.GetCommItemInfoTB( uItemIndex );
		if ( pTB )
		{
			if ( pTB->m_nItemGrade > 1 )
			{
				return TRUE;
			}
		}
		else
		{
			Assert( pTB );
		}

	}*/

__LEAVE_FUNCTION

	return FALSE;
}
