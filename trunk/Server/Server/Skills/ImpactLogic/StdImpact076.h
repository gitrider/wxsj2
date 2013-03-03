/********************************************************************
	created:	2008/05/16
	created:	16:5:2008   20:14
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact076.h
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact076
	file ext:	h
	author:		Richard
	
	purpose:	ÕÙ»½ÊÞ×¨ÓÃ
*********************************************************************/

#ifndef StdImpact076_h__
#define StdImpact076_h__

#include "ImpactLogic.h"

struct _MONSTER_PET_ATTR;

namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact076_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_076,
				};

				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_MONSTER_ID=0,
							IDX_LEFE_TIME,
							IDX_IMPACT_SELF,
							IDX_IMPACT_ONWER,
							IDX_SKILL1_ID,
							IDX_SKILL2_ID,
							IDX_SKILL3_ID,
							IDX_SKILL4_ID,
							IDX_SKILL5_ID,
							IDX_SKILL6_ID,
							IDX_SKILL7_ID,
							IDX_SKILL8_ID
						};
				};
				StdImpact076_T() {};
				~StdImpact076_T() {};
				BOOL IsOverTimed(VOID) const {return FALSE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;
				
				INT GetMonsterID( OWN_IMPACT const& rImp ) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_MONSTER_ID,
						rImp.GetSkillLevel()
						);
				}

				INT GetLifeTime( OWN_IMPACT const& rImp ) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_LEFE_TIME,
						rImp.GetSkillLevel()
						);
				}

				INT GetImpactSelf( OWN_IMPACT const& rImp ) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_IMPACT_SELF,
						rImp.GetSkillLevel()
						);
				}

				INT GetImpactOnwer( OWN_IMPACT const& rImp ) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_IMPACT_ONWER,
						rImp.GetSkillLevel()
						);
				}

				INT GetSkillIDByIndex( OWN_IMPACT const& rImp, INT iIndex ) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						iIndex,
						rImp.GetSkillLevel()
						);
				}

			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				VOID CriticalRefix(OWN_IMPACT& rImp) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
				VOID InitAttr( _MONSTER_PET_ATTR* pInit, OWN_IMPACT& rImp ) const;
			private:
		} ;
	};
};
#endif // StdImpact076_h__
