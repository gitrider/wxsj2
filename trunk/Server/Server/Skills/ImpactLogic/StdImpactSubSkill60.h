/********************************************************************
	created:	2008/02/28
	created:	28:2:2008   11:05
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpactSubSkill60.h
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpactSubSkill60
	file ext:	h
	author:		Richard
	
	purpose:	ʵ���Ӽ��ܻ��ƣ������ܴ�������
				
				ͨ�� OnUseSkillSuccessfully msg handler, �ҵ�Ŀǰ����ʹ�ü��ܵ�������

				���ܣ���ӵ�������ϴ����ͼ����б������Ҷ�����������Ϣ�ﴦ��
*********************************************************************/

#ifndef StdImpactSubSkill60_h__
#define StdImpactSubSkill60_h__

#include "ImpactLogic.h"

class Skill_Module::SkillTemplateData_T;

namespace Combat_Module
{
	namespace Impact_Module
	{

		class StdImpact060_T : public ImpactLogic_T
		{
		public:
			enum
			{
				ID = STD_IMPACT_060,
			};
			
			virtual BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;
			//flags
			//�Ƿ�Ϊ������Ч��
			virtual BOOL IsOverTimed(VOID) const {return TRUE;};
			//�Ƿ�ʱ��������
			virtual BOOL IsIntervaled(VOID) const {return FALSE;};
			virtual VOID OnUseSkillSuccessfully(OWN_IMPACT& rImp, Obj_Character& rMe, SkillInfo_T& rSkill) const;

		private: 
			INT StdImpact060_T::IsSubSkillCanSuccess( INT iRate, Obj_Character& rMe ) const;
		};
	}
}

#endif // StdImpactSubSkill60_h__