/********************************************************************
	created:	2008/05/10
	created:	10:5:2008   17:48
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact073.h
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact073
	file ext:	h
	author:		Richard
	
	purpose:	按点数修正HP和怒气回复
*********************************************************************/

#ifndef StdImpact073_h__
#define StdImpact073_h__

#include "Type.h"
#include "ImpactLogic.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact073_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_073,
				};

				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_REFIX_TYPE = 0,						//按点或百分比进行修正 0 = 点数修正
							IDX_HP_RECOVER,
							IDX_MP_RECOVER,
							IDX_PNEUMA_RECOVER,							
							IDX_RAGE_RECOVER,
						};
				};
				StdImpact073_T() {};
				~StdImpact073_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				//INT GetRefixRate(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE);};
				//VOID SetRefixRate(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE, nValue);};

				INT GetRefixType(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_REFIX_TYPE,
						rImp.GetSkillLevel()
						);
				};
				INT GetHPRecoverRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_HP_RECOVER,
						rImp.GetSkillLevel()
						);
				};
				INT GetMPRecoverRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_MP_RECOVER,
						rImp.GetSkillLevel()
						);
				};
				INT GetPneumaRecoverRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_PNEUMA_RECOVER,
						rImp.GetSkillLevel()
						);
				};
				INT GetRageRecoverRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RAGE_RECOVER,
						rImp.GetSkillLevel()
						);
				};
	
			public: //message handler
				VOID MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const;
				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;
				//BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
				INT GetRifixResult( INT iBase, INT iRefix, INT iRefixType ) const;
			private:
				
		};
	};
};

#endif // StdImpact073_h__
