/********************************************************************
	created:	2007/11/24
	created:	24:11:2007   12:08
	filename: 	d:\project\projectNew\MapServer\Server\Skills\ImpactLogic\StdImpact058.h
	file path:	d:\project\projectNew\MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact058
	file ext:	h
	author:		Richard
	
	purpose:	��֪����ʹ�ú󣬱��������ϵĳ���Ч��, Ϊ�������˺�

				1 �˴�Ч��ʵ�ֳ����Եİ����ڵ��˺�

				2 �ر�֮������m_uUniquidID �м�¼��ɴ�Ч����С���ȫ��ID

				3 m_uUniquidID��Ч����ɢʱҪ���͸�client

				4 �� Obj_Character::Impact_OnImpactFadeOut�У���client����
				  Ч����ɢ��Ϣ�ĵط������ȡ
*********************************************************************/
#ifndef StdImpact058_h__24_11_2007_14_42
#define StdImpact058_h__24_11_2007_14_42

#include "Type.h"
#include "ImpactLogic.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"

 
namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact058_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_058,
				};
				class OwnImpactParamIndex_T
				{
				public:
					enum
					{
						IDX_HP=0,
						IDX_MP,
						IDX_RAGE,
						IDX_STRIKE_POINT,				
					};
				};
				StdImpact058_T();
				~StdImpact058_T();
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				//�Ƿ�Ϊ������Ч��
				virtual BOOL IsOverTimed(VOID)  const {return TRUE;}
				//�Ƿ�Ϊ�����Է���Ч��
				virtual BOOL IsIntervaled(VOID) const {return TRUE;}
				//������ʱ�䵽�˻���ô���Ϣ����, �ڴ˷�����ʵ�־���Ķ�ӵ���ߵ��˺�
				virtual VOID OnIntervalOver(OWN_IMPACT& rImp, Obj_Character& rMe) const;

			public: //message handler
				//Ч������ʱ����Ϣ����
				virtual VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				//Ч����ɢʱ����Ϣ����,�ڴ˴�Ϊ��client������Ϣ��Я��m_uUniquidID��Ϣ
				//��С��ķ�����(����С���Ч������)֪ͨ,��ʹ��ά��С�����ȷ����
				virtual VOID OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const;
			protected:
				INT GetHpModification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_HP);}
				VOID SetHpModification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_HP, nValue);}

				INT GetMpModification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_MP);}
				VOID SetMpModification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_MP, nValue);}

				INT GetRageModification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_RAGE);}
				VOID SetRageModification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_RAGE, nValue);}

			private:

		} DI_IntervalOverHurt_T;
	};
};

#endif //StdImpact058_h__24_11_2007_14_43
