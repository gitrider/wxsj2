#include "stdafx.h"
#include "GaiBangZGB.h"

namespace MenPai_Module
{
	VOID GaiBangZGB_T::OnDepleteStrikePoints(Obj_Human& rMe, INT nStrikePoint) const
	{
		if(0>=nStrikePoint)
		{
			return;
		}
		
		//INT nRage = TransferValueToRage(nStrikePoint);
		//rMe.RefixRageRegeneration(nRage);
		//rMe.RageIncrement(nRage,rMe.GetLogicCount(),NULL);
	}

	INT GaiBangZGB_T::TransferValueToRage(INT nValue) const
	{
		if(0>=nValue)
		{
			return 0;
		}
		return nValue*10;
	}
		
};


