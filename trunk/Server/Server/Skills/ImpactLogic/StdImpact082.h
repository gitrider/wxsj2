/********************************************************************
	created:	2010/05/14
	author:		�¾�
	purpose:	�޸�HP,MP,Ԫ��,ŭ��(��62����)
*********************************************************************/

#ifndef StdImpact082_h__
#define StdImpact082_h__

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact082_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_082,
				};

				enum
				{
					REFIX_TYPE_POINT,
					REFIX_TYPE_RATE,			
					REFIX_TYPE_ME,			
					REFIX_TYPE_SENDER
				};

				enum
				{
					REFIX_PARAM_TYPE_0 = 0,			//ȡ�𹥻�����
					REFIX_PARAM_TYPE_1,				//ȡľ��������
					REFIX_PARAM_TYPE_2,				//ȡˮ��������
					REFIX_PARAM_TYPE_3,				//ȡ�𹥻�����
					REFIX_PARAM_TYPE_4,				//ȡ����������

					REFIX_PARAM_TYPE_5,				//ȡԶ���ڹ���������
					REFIX_PARAM_TYPE_6,				//ȡԶ���⹦��������
					REFIX_PARAM_TYPE_7,				//ȡ�����ڹ���������
					REFIX_PARAM_TYPE_8,				//ȡ�����⹦��������
					
					REFIX_PARAM_TYPE_MAX
				};

				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_REFIX_TYPE=0,		//0Ϊ������������1Ϊ���ٷֱ����� , 2 Ϊȡme��ĳЩ���� 3 Ϊȡsender��ĳЩ����
							IDX_REFIX_HP,			//
							IDX_REFIX_MP,
							IDX_REFIX_PNEUMA,
							IDX_REFIX_RAGE,
 							IDX_REFIX_RATE
						};
				};
				StdImpact082_T() {};
				~StdImpact082_T() {};
				BOOL IsOverTimed(VOID) const {return FALSE;};
				BOOL IsIntervaled(VOID) const {return TRUE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;
				
				INT GetHP(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_REFIX_HP);
				}
				VOID SetHP(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_REFIX_HP, nDamage);
				}

				INT GetMP(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_REFIX_MP);
				}
				VOID SetMP(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_REFIX_MP, nDamage);
				}

				INT GetPneuma(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_REFIX_PNEUMA);
				}
				VOID SetPneuma(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_REFIX_PNEUMA, nDamage);
				}
 
				INT GetRage(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_REFIX_RAGE);
				}
				VOID SetRage(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_REFIX_RAGE, nDamage);
				}
			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
				VOID InitDamages( OWN_IMPACT& rImp ) const;
				INT GetRifixResult(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rSender, INT iBase, INT iRefix, INT iRefixType ) const;
				INT	GetRefixParamValue(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rSender, INT iRefixType, INT nParamType) const;
			private:
		} ;
	};
};

#endif // StdImpact082_h__
