///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact000.h
// ����Ա������
// ����˵����Ч��--����֪ͨ�ͻ�����ʾ��Ч�õ�
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT000_H
#define STDIMPACT000_H

#include "ImpactLogic.h"
namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact000_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_000,
				};
				
				StdImpact000_T() {};
				~StdImpact000_T() {};				
			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
			protected:
			private:
		} DI_JustSpecialEffect_T;
	};
};
#endif //STDIMPACT000_H
