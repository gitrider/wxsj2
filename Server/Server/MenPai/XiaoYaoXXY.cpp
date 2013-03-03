#include "stdafx.h"
#include "XiaoYaoXXY.h"

namespace MenPai_Module
{
	VOID XiaoYaoXXY_T::OnMyTrapActivated(Obj_Human& rMe, Obj_Special& rTrap) const
	{
		//INT nRage = TransferValueToRage(1);
		//rMe.RefixRageRegeneration(nRage);
		//rMe.RageIncrement(nRage,rMe.GetLogicCount(),NULL);
	}

	INT XiaoYaoXXY_T::TransferValueToRage(INT nValue) const
	{
		if(0>=nValue)
		{
			return 0;
		}
		return nValue*10;
	}
		
};


