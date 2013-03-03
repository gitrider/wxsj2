#include "stdafx.h"
#include "StdImpact083.h"
#include "Obj_Human.h"
#include "HumanItemLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact083_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetParam1(rImp, const_cast<ImpactData_T*>(&rData)->GetReserveParam1());
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact083_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			if(Obj::OBJ_TYPE_HUMAN == rMe.GetObjType())
			{
				Obj_Human & rMan = (Obj_Human &)rMe;
				
				InitAttrData(rMan, rImp);//add by gh

				if(0!=GetStr(rImp))
				{
					rMe.MarkStrRefixDirtyFlag();
				}
				if(0!=GetCon(rImp))
				{
					rMe.MarkConRefixDirtyFlag();
				}
				if(0!=GetInt(rImp))
				{
					rMe.MarkIntRefixDirtyFlag();
				}
				if(0!=GetDex(rImp))
				{
					rMe.MarkDexRefixDirtyFlag();
				}//
				if(0!=GetAttackNear(rImp))
				{
					rMe.MarkAttackNearRefixDirtyFlag();
				}
				if(0!=GetAttackFar(rImp))
				{
					rMe.MarkAttackFarRefixDirtyFlag();
				}
				if(0!=GetAttackMagicNear(rImp))
				{
					rMe.MarkAttackMagicFarRefixDirtyFlag();
				}
				if(0!=GetAttackMagicFar(rImp))
				{
					rMe.MarkAttackMagicNearRefixDirtyFlag();
				}
				//
				if(0!=GetDefenceNear(rImp))
				{
					rMe.MarkDefenceNearRefixDirtyFlag();
				}
				if(0!=GetDefenceFar(rImp))
				{
					rMe.MarkDefenceFarRefixDirtyFlag();
				}
				if(0!=GetDefenceMagicNear(rImp))
				{
					rMe.MarkDefenceMagicFarRefixDirtyFlag();
				}
				if(0!=GetDefenceMagicFar(rImp))
				{
					rMe.MarkDefenceMagicNearRefixDirtyFlag();
				}
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact083_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			//只能使用在人身上	
			if(Obj::OBJ_TYPE_HUMAN==rMe.GetObjType())
			{
				Obj_Human & rMan = (Obj_Human &)rMe;
				switch (nIdx)
				{
					case CharIntAttrRefixs_T::REFIX_STR:
						{
							if(0!=GetStr(rImp))
							{
								rIntAttrRefix += GetStr(rImp);
								return TRUE;
							}
						}
						break;
					case CharIntAttrRefixs_T::REFIX_CON:
						{
							if(0!=GetCon(rImp))
							{
								rIntAttrRefix += GetCon(rImp);
								return TRUE;
							}
						}
						break;
					case CharIntAttrRefixs_T::REFIX_INT:
						{
							if(0!=GetInt(rImp))
							{
								rIntAttrRefix += GetInt(rImp);
								return TRUE;
							}
						}
						break;
					case CharIntAttrRefixs_T::REFIX_DEX:
						{
							if(0!=GetDex(rImp))
							{
								rIntAttrRefix += GetDex(rImp);
								return TRUE;
							}
						}
						break;
					//攻击力
					case CharIntAttrRefixs_T::REFIX_ATTACK_NEAR:
						{
							if(0!=GetAttackNear(rImp))
							{
								rIntAttrRefix += GetAttackNear(rImp);
								return TRUE;
							}
						}
						break;
					case CharIntAttrRefixs_T::REFIX_ATTACK_FAR:
						{
							if(0!=GetAttackFar(rImp))
							{
								rIntAttrRefix += GetAttackFar(rImp);
								return TRUE;
							}
						}
						break;
					case CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_NEAR:
						{
							if(0!=GetAttackMagicNear(rImp))
							{
								rIntAttrRefix += GetAttackMagicNear(rImp);
								return TRUE;
							}
						}
						break;
					case CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_FAR:
						{
							if(0!=GetAttackMagicFar(rImp))
							{
								rIntAttrRefix += GetAttackMagicFar(rImp);
								return TRUE;
							}
						}
						break;
					//防御
					case CharIntAttrRefixs_T::REFIX_DEFENCE_NEAR:
						{
							if(0!=GetDefenceNear(rImp))
							{
								rIntAttrRefix += GetDefenceNear(rImp);
								return TRUE;
							}
						}
						break;
					case CharIntAttrRefixs_T::REFIX_DEFENCE_FAR:
						{
							if(0!=GetDefenceFar(rImp))
							{
								rIntAttrRefix += GetDefenceFar(rImp);
								return TRUE;
							}
						}
						break;
					case CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_NEAR:
						{
							if(0!=GetDefenceMagicNear(rImp))
							{
								rIntAttrRefix += GetDefenceMagicNear(rImp);
								return TRUE;
							}
						}
						break;
					case CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_FAR:
						{
							if(0!=GetDefenceMagicFar(rImp))
							{
								rIntAttrRefix += GetDefenceMagicFar(rImp);
								return TRUE;
							}
						}
						break;
					default:
						break;
				}
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
		
		VOID StdImpact083_T::InitAttrData(Obj_Human& Human, OWN_IMPACT& rImp ) const
		{

			Item* pItem = HumanItemLogic::GetBagItem(&Human, GetParam1(rImp));
			Assert(pItem);
			if (ICLASS_SOUL_BEAD != pItem->GetItemClass())
			{
				return;
			}
			if(SOUL_BEAD_WU == pItem->GetItemType())
			{	
				for(BYTE i=0; i<pItem->GetSoulBeadAttrCount(); ++i)
				{
					_ITEM_ATTR itemAttr = pItem->GetWuSoulBeadAttr(i);
					switch(itemAttr.m_AttrType)
					{
					case IATTRIBUTE_POINT_STR:
						SetStr(rImp, pItem->GetWuSoulBeadAttr(i).m_Value.m_Value);
						break;
					case IATTRIBUTE_POINT_CON:
						SetCon(rImp, pItem->GetWuSoulBeadAttr(i).m_Value.m_Value);
						break;
					case IATTRIBUTE_POINT_INT:
						SetInt(rImp, pItem->GetWuSoulBeadAttr(i).m_Value.m_Value);
						break;
					case IATTRIBUTE_POINT_DEX:
						SetDex(rImp, pItem->GetWuSoulBeadAttr(i).m_Value.m_Value);
						break;
				
					case IATTRIBUTE_BASE_ATTACK_NEAR://基础远程内攻击
						SetAttackNear(rImp, pItem->GetWuSoulBeadAttr(i).m_Value.m_Value);
						break;
					case IATTRIBUTE_BASE_ATTACK_FAR://基础远程外攻击
						SetAttackFar(rImp, pItem->GetWuSoulBeadAttr(i).m_Value.m_Value);
						break;
					case IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR://基础近程内功攻击
						SetAttackMagicNear(rImp, pItem->GetWuSoulBeadAttr(i).m_Value.m_Value);
						break;
					case IATTRIBUTE_BASE_ATTACK_MAGIC_FAR://基础近程外功攻击
						SetAttackMagicFar(rImp, pItem->GetWuSoulBeadAttr(i).m_Value.m_Value);
						break;
					default:
						break;
					}
				}
			}
			else if(SOUL_BEAD_QI == pItem->GetItemType())
			{
				for(BYTE i=0; i<pItem->GetSoulBeadAttrCount(); ++i)
				{
					_ITEM_ATTR itemAttr = pItem->GetQiSoulBeadAttr(i);
					switch(itemAttr.m_AttrType)
					{
					case IATTRIBUTE_POINT_STR:
						SetStr(rImp, pItem->GetQiSoulBeadAttr(i).m_Value.m_Value);
						break;
					case IATTRIBUTE_POINT_CON:
						SetCon(rImp, pItem->GetQiSoulBeadAttr(i).m_Value.m_Value);
						break;
					case IATTRIBUTE_POINT_INT:
						SetInt(rImp, pItem->GetQiSoulBeadAttr(i).m_Value.m_Value);
						break;
					case IATTRIBUTE_POINT_DEX:
						SetDex(rImp, pItem->GetQiSoulBeadAttr(i).m_Value.m_Value);
						break;
					case IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR://基础近程内功防御
						SetDefenceMagicNear(rImp, pItem->GetQiSoulBeadAttr(i).m_Value.m_Value);
						break;
					case IATTRIBUTE_BASE_DEFENCE_MAGIC_FAR://基础近程外功防御
						SetDefenceMagicFar(rImp, pItem->GetQiSoulBeadAttr(i).m_Value.m_Value);
						break;
					case IATTRIBUTE_BASE_DEFENCE_NEAR://基础远程内功防御
						SetDefenceNear(rImp, pItem->GetQiSoulBeadAttr(i).m_Value.m_Value);
						break;
					case IATTRIBUTE_BASE_DEFENCE_FAR://基础远程外功防御
						SetDefenceFar(rImp, pItem->GetQiSoulBeadAttr(i).m_Value.m_Value);
						break;
					default:
						break;
					}
				}
			}
		}


	};
};

