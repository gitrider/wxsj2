#include "stdafx.h"
#include "XiaoYaoZXY.h"

namespace MenPai_Module
{
	VOID XiaoYaoZXY_T::OnMyTrapActivated(Obj_Human& rMe, Obj_Special& rTrap) const
	{
		//INT nRage = TransferValueToRage(1);
		//rMe.RefixRageRegeneration(nRage);
		//rMe.RageIncrement(nRage,rMe.GetLogicCount(),NULL);
	}

	INT XiaoYaoZXY_T::TransferValueToRage(INT nValue) const
	{
		if(0>=nValue)
		{
			return 0;
		}
		return nValue*10;
	}
		
};


