#include "stdafx.h"
#include "GaiBangGGB.h"

namespace MenPai_Module
{
	VOID GaiBangGGB_T::OnDepleteStrikePoints(Obj_Human& rMe, INT nStrikePoint) const
	{
		if(0>=nStrikePoint)
		{
			return;
		}
		
		//INT nRage = TransferValueToRage(nStrikePoint);
		//rMe.RefixRageRegeneration(nRage);
		//rMe.RageIncrement(nRage,rMe.GetLogicCount(),NULL);
	}

	INT GaiBangGGB_T::TransferValueToRage(INT nValue) const
	{
		if(0>=nValue)
		{
			return 0;
		}
		return nValue*10;
	}
		
};


