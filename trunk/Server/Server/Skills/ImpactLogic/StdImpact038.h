///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact038.h
// ����Ա������
// ����˵����Ч��--ֻ��ʾ��Ч��
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT038_H
#define STDIMPACT038_H


#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact038_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_038,
				};
				StdImpact038_T() {};
				~StdImpact038_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
			public: //message handler
			protected:
			private:
		};
	};
};
#endif //STDIMPACT038_H
