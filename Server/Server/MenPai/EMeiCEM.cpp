#include "stdafx.h"
#include "EMeiCEM.h"

namespace MenPai_Module
{
	VOID EMeiCEM_T::OnHealTarget(Obj_Human& rMe, INT nHealHp) const
	{
		if(0>=nHealHp)
		{
			return;
		}
		//INT nRage = TransferValueToRage(nHealHp);
		//rMe.RefixRageRegeneration(nRage);
		//rMe.RageIncrement(nRage,rMe.GetLogicCount(),NULL);
	}

	INT EMeiCEM_T::TransferValueToRage(INT nValue) const
	{
		if(0>=nValue)
		{
			return 0;
		}
		return nValue/20;
	}
		
};

