/********************************************************************
	created:	2008/05/10
	created:	10:5:2008   18:41
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact073.cpp
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact073
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "StdImpact073.h"
#include "Obj_Human.h"

namespace Combat_Module
{

	namespace Impact_Module
	{
		BOOL StdImpact073_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			//SetRefixRate(rImp, 0);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact073_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			if(0!=GetHPRecoverRefix(rImp))
			{
				rMe.MarkHPRegenerateRefixDirtyFlag();
			}
			if(0!=GetMPRecoverRefix(rImp))
			{
				rMe.MarkMPRegenerateRefixDirtyFlag();
			}
			if(0!=GetPneumaRecoverRefix(rImp))
			{
				rMe.MarkPneumaRegenerateRefixDirtyFlag();
			}
			/*
			if(0!=GetRageRecoverRefix(rImp))
			{
				rMe.MarkRageRegenerateRefixDirtyFlag();
			}
			*/
			__LEAVE_FUNCTION
		}
		BOOL StdImpact073_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
				
			INT iRefixType = GetRefixType(rImp);
			
			switch (nIdx)
			{
				case CharIntAttrRefixs_T::REFIX_HP_REGENERATE:
					{
						if(0!=GetHPRecoverRefix(rImp))
						{
							INT iBase = rMe.GetBaseHPRegenerate();
							INT iRefix = GetHPRecoverRefix(rImp);
							rIntAttrRefix += GetRifixResult(iBase, iRefix, iRefixType);
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_MP_REGENERATE:
					{
						if(0!=GetMPRecoverRefix(rImp))
						{
							INT iBase = rMe.GetBaseMPRegenerate();
							INT iRefix = GetMPRecoverRefix(rImp);
							rIntAttrRefix += GetRifixResult(iBase, iRefix, iRefixType);
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_PNEUMA_REGENERATE:
					{
						if(0!=GetPneumaRecoverRefix(rImp))
						{
							INT iBase = rMe.GetBasePneumaRegenerate();
							INT iRefix = GetPneumaRecoverRefix(rImp);
							rIntAttrRefix += GetRifixResult(iBase, iRefix, iRefixType);
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_RAGE_REGENERATE:
					{
						if(0!=GetRageRecoverRefix(rImp))
						{
							INT iBase = rMe.GetBaseRageRegenerate();
							INT iRefix = GetRageRecoverRefix(rImp);
							rIntAttrRefix += GetRifixResult(iBase, iRefix, iRefixType);
							return TRUE;
						}
					}
					break;
				default:
					break;
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
		//BOOL StdImpact073_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		//{
		//	__ENTER_FUNCTION
		//	//nRate += GetRefixRate(rImp);
		//	//SetRefixRate(rImp, nRate);
		//	__LEAVE_FUNCTION
		//	return TRUE;
		//}

		INT StdImpact073_T::GetRifixResult( INT iBase, INT iRefix, INT iRefixType ) const 
		{
			__ENTER_FUNCTION

			INT iRefixResult = 0;
			//按点数修正
			if( iRefixType <= 0 )
			{
				iRefixResult = iRefix;
			}
			else
			{//按百分比修正
				iRefixResult = Float2Int((iBase*iRefix)/100.0f);
			}
			return iRefixResult;
			__LEAVE_FUNCTION
			return 0;
		}

	};
};

