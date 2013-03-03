///////////////////////////////////////////////////////////////////////////////
// �ļ�����ArmorMastery.h
// ����Ա��Gaoqi
// ����˵����
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef ARMORMASTERY_H
#define ARMORMASTERY_H

#include "Type.h"
#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class ArmorMastery_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = ARMOR_MASTERY,
				};
				enum
				{
					BASE_DEFENCE_NEAR = 0,
					BASE_DEFENCE_MAGIC_NEAR = 1,
				};
				// public interface
				ArmorMastery_T(){};
				~ArmorMastery_T(){};
				BOOL IsPassive(VOID) const {return TRUE;};
			protected: //interface for interval using 
				BOOL Refix_ItemEffect(SkillInfo_T & rThisPassiveSkill, INT nSlotID, INT nItemType, INT nATTRID, SHORT & nAttrValue) const;
			private:
		};
	};
};
#endif //ARMORMASTERY_H

