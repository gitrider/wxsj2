///////////////////////////////////////////////////////////////////////////////
// 文件名：WeaponMastery.cpp
// 程序员：frankwu
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Type.h"
#include "WeaponMastery.h"
#include "LogicManager.h"


namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Impact_Module;
		BOOL WeaponMastery_T::Refix_ItemEffect(SkillInfo_T & rThisPassiveSkill, INT nSlotID, INT nItemType, INT nAttrID, SHORT & rAttrValue) const
		{
			__ENTER_FUNCTION
			INT nRefixAttackPhysics = rThisPassiveSkill.GetDescriptorByIndex(BASE_ATTACK_NEAR_REFIX)->GetValueBySkillLevel(rThisPassiveSkill.GetCharacterSkillLevel());
			INT nRefixAttackMagic = rThisPassiveSkill.GetDescriptorByIndex(BASE_ATTACK_MAGIC_NEAR_REFIX)->GetValueBySkillLevel(rThisPassiveSkill.GetCharacterSkillLevel());
			INT const nMasteryType1 = rThisPassiveSkill.GetDescriptorByIndex(MASTERY_OF_WEAPON_TYPE1)->GetValueBySkillLevel(rThisPassiveSkill.GetCharacterSkillLevel());
			INT const nMasteryType2 = rThisPassiveSkill.GetDescriptorByIndex(MASTERY_OF_WEAPON_TYPE2)->GetValueBySkillLevel(rThisPassiveSkill.GetCharacterSkillLevel());
			//switch (nSlotID)
			//{
			//	case	HEQUIP_WEAPON: 	//武器	WEAPON
			//		{
			//			if(nMasteryType1 == nItemType || nMasteryType2 == nItemType)
			//			{
			//				switch (nAttrID)
			//				{
			//					case IATTRIBUTE_BASE_ATTACK_NEAR:			//基础物理攻击
			//						{
			//							rAttrValue = Float2Int((rAttrValue * nRefixAttackPhysics)/100.0f);
			//						}
			//						break;
			//					case IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR:			//基础近程内功攻击
			//						{
			//							rAttrValue = Float2Int((rAttrValue * nRefixAttackMagic)/100.0f);
			//						}
			//						break;
			//					default:
			//						break;
			//				}
			//			}
			//		}
			//		break;
			//		break;
			//	case	HEQUIP_CAP:		//帽子	DEFENCE
			//	case	HEQUIP_ARMOR:	//盔甲	DEFENCE
			//	case	HEQUIP_CUFF:	//护腕	DEFENCE
			//	case	HEQUIP_BOOT:	//鞋	DEFENCE
			//	case	HEQUIP_SASH:	//腰带	ADORN
			//	case	HEQUIP_RING:	//戒子	ADORN
			//	case	HEQUIP_NECKLACE://项链	ADORN
			//	case	HEQUIP_RIDER:	//骑乘  ADORN
			//	default:
			//		break;
			//}
			//return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
