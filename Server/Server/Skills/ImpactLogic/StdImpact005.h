///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact005.h
// 程序员：高骐
// 功能说明：效果--瞬发-生命、魔法、怒气、连击点的百分率直接修改
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT005_H
#define STDIMPACT005_H

#include "Type.h"
#include "ImpactLogic.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact005_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_005,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_HP=0,
							IDX_MP,
							IDX_RAGE,
							IDX_RAGE1,
							IDX_RAGE2,
							IDX_RAGE3,
							IDX_RAGE4,
							IDX_RAGE5,
							IDX_REFIX_RATE,				
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_HP=0,
							IDX_MP,
							IDX_RAGE,
							IDX_RAGE1,
							IDX_RAGE2,
							IDX_RAGE3,
							IDX_RAGE4,
							IDX_RAGE5,
							IDX_REFIX_RATE,
						};
				};
				StdImpact005_T() {};
				~StdImpact005_T() {};
				BOOL IsOverTimed(VOID) const {return FALSE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetHpModification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_HP);}
				VOID SetHpModification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_HP, nValue);}

				INT GetMpModification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_MP);}
				VOID SetMpModification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_MP, nValue);}

				INT GetRageModification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_RAGE);}
				VOID SetRageModification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_RAGE, nValue);}

				INT GetRage1Modification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_RAGE1);}
				VOID SetRage1Modification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_RAGE1, nValue);}

				INT GetRage2Modification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_RAGE2);}
				VOID SetRage2Modification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_RAGE2, nValue);}

				INT GetRage3Modification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_RAGE3);}
				VOID SetRage3Modification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_RAGE3, nValue);}

				INT GetRage4Modification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_RAGE4);}
				VOID SetRage4Modification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_RAGE4, nValue);}

				INT GetRage5Modification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_RAGE5);}
				VOID SetRage5Modification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_RAGE5, nValue);}

				INT GetRefixRate(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE);}
				VOID SetRefixRate(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE, nValue);}
			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
				INT GetDamageRefix( Obj_Character * pImpactSender, OWN_IMPACT& rImp ) const;
			private:
		} ModifyHpMpRageStrikePointByRate_T;
	} ;
};
#endif //STDIMPACT005_H
