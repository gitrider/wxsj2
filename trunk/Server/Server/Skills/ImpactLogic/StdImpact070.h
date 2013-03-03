/********************************************************************
	created:	2008/05/09
	created:	9:5:2008   14:25
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact070.h
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact070
	file ext:	h
	author:		Richard
	
	purpose:	DOT
*********************************************************************/

#ifndef StdImpact070_h__
#define StdImpact070_h__

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact070_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_070,
				};
				class OwnImpactParamIndex_T
				{
				public:
					enum
					{
						IDX_REFIX_TYPE=0,			//0Ϊ������������1Ϊ���ٷֱ�����
						IDX_IMPACT_PARA_HORSE,		//��Ӧ4����ʽ��ID,˳��Ϊ
													//��������=�ȼ�*�ɳ���/1000+����*��������/1000
													//���ݲ���=�ȼ�*�ɳ���/1000+����*��������/1000
													//��������=�ȼ�*�ɳ���/1000+����*��������/1000
													//���ʲ���=�ȼ�*�ɳ���/1000+����*��������/1000
						IDX_IMPACT_RATE,			//Ӱ����						
						IDX_IMPACT_DAMAGE_TYPE,		//�˺����ͣ�
						IDX_IMPACT_PARA,			//�˺���ֵ
						IDX_IMPACT_ATTACK_WEIGHT,	//���������������
						IDX_IMPACT_DEFENCE_WEIGHT,	//�Է��������������
					};
				};
				class ImpactDataDescriptorIndex_T
				{
				public:
					enum
					{

						IDX_REFIX_TYPE=0,			//0Ϊ������������1Ϊ���ٷֱ�����
						IDX_REFIX_PARA_HORSE,		//��Ӧ4����ʽ��ID,˳��Ϊ
													//��������=�ȼ�*�ɳ���/1000+����*��������/1000
													//���ݲ���=�ȼ�*�ɳ���/1000+����*��������/1000
													//��������=�ȼ�*�ɳ���/1000+����*��������/1000
													//���ʲ���=�ȼ�*�ɳ���/1000+����*��������/1000
						IDX_REFIX_RATE,				//Ӱ����						
						IDX_REFIX_DAMAGE_TYPE,		//�˺����ͣ�
						IDX_REFIX_PARA,				//�˺���ֵ
						IDX_REFIX_ATTACK_WEIGHT,	//���������������
						IDX_REFIX_DEFENCE_WEIGHT	//�Է��������������
					};
				};
				StdImpact070_T() {};
				~StdImpact070_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return TRUE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetDamageType(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_DAMAGE_TYPE);
				}
				VOID SetDamageType(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_DAMAGE_TYPE, nDamage);
				}

				INT GetDamagePara(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_PARA);
				}
				VOID SetDamagePara(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_PARA, nDamage);
				}

				INT GetAttackWeight(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_ATTACK_WEIGHT);
				}
				VOID SetAttackWeight(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_ATTACK_WEIGHT, nDamage);
				}

				INT GetDefenceWeight(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_DEFENCE_WEIGHT);
				}
				VOID SetDefenceWeight(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_DEFENCE_WEIGHT, nDamage);
				}

				INT GetParaID(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_PARA_HORSE);
				}
				VOID SetParaID(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_PARA_HORSE, nDamage);
				}
				INT GetRefixRate(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_RATE);
				}
				VOID SetRefixRate(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_RATE, nDamage);
				}
		public: //message handler
			VOID OnIntervalOver(OWN_IMPACT& rImp, Obj_Character& rMe) const;
		protected:
			VOID InitDamages( OWN_IMPACT& rImp ) const;
			INT GetRifixResult( INT iBase, INT iRefix, INT iRefixType ) const;

			INT GetRefixPara( OWN_IMPACT & rImp, Obj_Character& rMe ) const;
			INT GetResult( INT iPara1, FLOAT fPara2, INT iPara3, INT iPara4, INT iRate ) const;
		private:
		};
	};
};

#endif // StdImpact070_h__