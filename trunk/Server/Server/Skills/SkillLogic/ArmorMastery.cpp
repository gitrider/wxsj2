///////////////////////////////////////////////////////////////////////////////
// �ļ�����ArmorMastery.cpp
// ����Ա��frankwu
// ����˵����
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Type.h"
#include "Obj_Character.h"
#include "ArmorMastery.h"
#include "Scene.h"
#include "Obj.h"
#include "Obj_Human.h"
#include "LogicManager.h"


namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Impact_Module;
		//class ArmorMastery_T : public SkillLogic_T
		BOOL ArmorMastery_T::Refix_ItemEffect(SkillInfo_T & rThisPassiveSkill, INT nSlotID, INT nItemType, INT nAttrID, SHORT & nAttrValue) const
		{
			__ENTER_FUNCTION
			INT nRefixDefencePhysics = rThisPassiveSkill.GetDescriptorByIndex(BASE_DEFENCE_NEAR)->GetValueBySkillLevel(rThisPassiveSkill.GetCharacterSkillLevel());
			INT nRefixDefenceMagic = rThisPassiveSkill.GetDescriptorByIndex(BASE_DEFENCE_MAGIC_NEAR)->GetValueBySkillLevel(rThisPassiveSkill.GetCharacterSkillLevel());
			//switch (nSlotID)
			//{
			//	case	HEQUIP_WEAPON: 	//����	WEAPON
			//		break;
			//	case	HEQUIP_CAP:		//ñ��	DEFENCE
			//	case	HEQUIP_ARMOR:	//����	DEFENCE
			//	case	HEQUIP_CUFF:	//����	DEFENCE
			//	case	HEQUIP_BOOT:	//Ь	DEFENCE
			//	case	HEQUIP_SASH:	//����	ADORN
			//		{
			//			switch (nAttrID)
			//			{
			//				case IATTRIBUTE_BASE_DEFENCE_NEAR:			//�����������
			//					{
			//						nAttrValue = Float2Int((nAttrValue * nRefixDefencePhysics)/100.0f);
			//					}
			//					break;
			//				case IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR:			//���������ڹ�����
			//					{
			//						nAttrValue = Float2Int((nAttrValue * nRefixDefenceMagic)/100.0f);
			//					}
			//					break;
			//				default:
			//					break;
			//			}
			//		}
			//		break;
			//	case	HEQUIP_RING:	//����	ADORN
			//	case	HEQUIP_NECKLACE://����	ADORN
			//	case	HEQUIP_RIDER:   //���  ADORN
			//		break;
			//	default:
			//		break;
			//}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
