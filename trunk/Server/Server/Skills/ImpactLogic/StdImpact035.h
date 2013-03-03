///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact035.h
// ����Ա��frankwu
// ����˵����Ч��--�־�״̬,����ʩ��
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT035_H
#define STDIMPACT035_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact035_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_035,
				};
				StdImpact035_T() {};
				~StdImpact035_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
			public:
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				VOID OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const;
			public: //message handler
				VOID MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const;
				BOOL GetBoolAttrRefix(OWN_IMPACT& rImp, Obj_Character& rMe, CharBoolAttrs_T::Index_T nIdx, BOOL& rBoolAttrRefix) const;
			protected:
			private:
		} SOT_Terror_T;
	};
};
#endif //StdImpact035_H


