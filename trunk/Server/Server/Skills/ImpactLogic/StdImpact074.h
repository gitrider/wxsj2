/********************************************************************
	created:	2008/05/10
	created:	10:5:2008   18:45
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact074.h
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact074
	file ext:	h
	author:		Richard
	
	purpose:	马专用给主人按点数或百分比修正HP和怒气
*********************************************************************/

#ifndef StdImpact074_h__
#define StdImpact074_h__

#include "Type.h"
#include "ImpactLogic.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact074_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_074,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_HP,
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
							IDX_REFIX_TYPE = 0,	//按点数或百分比修正
							IDX_PARA_ID,		//对应4个公式的ID,顺序为
												//力量参数=等级*成长率/1000+力量*力量资质/1000
												//敏捷参数=等级*成长率/1000+敏捷*敏捷资质/1000
												//智力参数=等级*成长率/1000+智力*智力资质/1000
												//体质参数=等级*成长率/1000+体质*体质资质/1000
							IDX_REFIX_RATE_HORSE,
							IDX_HP,
							IDX_MP,
							IDX_RAGE,
							IDX_RAGE1,
							IDX_RAGE2,
							IDX_RAGE3,
							IDX_RAGE4,
							IDX_RAGE5,
							IDX_REFIX_RATE
						};
				};
				StdImpact074_T() {};
				~StdImpact074_T() {};
				BOOL IsOverTimed(VOID) const {return FALSE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetRefixType(OWN_IMPACT const& rImp) const 
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_REFIX_TYPE,
						rImp.GetSkillLevel()
						);
				};
				INT GetParaID(OWN_IMPACT const& rImp) const 
				{
					return Impact_GetImpactDataDescriptorValueByIndex
							(rImp.GetDataIndex(), 
							ImpactDataDescriptorIndex_T::IDX_PARA_ID,
							rImp.GetSkillLevel()
							);
				};
				INT GetRefixRateHorse(OWN_IMPACT const& rImp) const 
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_REFIX_RATE_HORSE,
						rImp.GetSkillLevel()
						);
				};
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
				INT GetRefixPara( OWN_IMPACT & rImp, Obj_Character& rMe ) const;
				INT GetResult( INT iPara1, FLOAT fPara2, INT iPara3, INT iPara4, INT iRate ) const;
			private:
		} ;
	} ;
};
#endif // StdImpact074_h__
