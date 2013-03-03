///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact016.cpp
// ����Ա������
// ����˵����Ч��--State: �ٷֱ��������������������ԡ�����������
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact016.h"
#include "Obj_Human.h"
#include "Obj_Pet.h"
namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact016_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetRefixRate(rImp, 0);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact016_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			if(Obj::OBJ_TYPE_HUMAN==rMe.GetObjType()||Obj::OBJ_TYPE_PET==rMe.GetObjType())
			{
				if(0!=GetStrRefix(rImp))
				{
					rMe.MarkStrRefixDirtyFlag();
				}
				if(0!=GetConRefix(rImp))
				{
					rMe.MarkConRefixDirtyFlag();
				}
				/*if(0!=GetSprRefix(rImp))
				{
					rMe.MarkSprRefixDirtyFlag();
				}*/
				if(0!=GetIntRefix(rImp))
				{
					rMe.MarkIntRefixDirtyFlag();
				}
				if(0!=GetDexRefix(rImp))
				{
					rMe.MarkDexRefixDirtyFlag();
				}
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact016_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			//ֻ��ʹ����������	
			if(Obj::OBJ_TYPE_HUMAN==rMe.GetObjType())
			{
				Obj_Human & rMan = (Obj_Human &)rMe;
				INT nRefixRate = GetRefixRate(rImp);
				nRefixRate += 100;
				INT nValue = 0;
				switch (nIdx)
				{
					case CharIntAttrRefixs_T::REFIX_STR:
						{
							if(0!=GetStrRefix(rImp))
							{
								nValue = Float2Int((GetStrRefix(rImp)*nRefixRate)/100.0f);
								nValue = Float2Int((rMan.GetBaseStr2()*nValue)/100.0f);
								rIntAttrRefix += nValue;
								return TRUE;
							}
						}
						break;
					case CharIntAttrRefixs_T::REFIX_CON:
						{
							if(0!=GetConRefix(rImp))
							{
								nValue = Float2Int((GetConRefix(rImp)*nRefixRate)/100.0f);
								nValue = Float2Int((rMan.GetBaseCon2()*nValue)/100.0f);
								rIntAttrRefix += nValue;
								return TRUE;
							}
						}
						break;
					case CharIntAttrRefixs_T::REFIX_INT:
						{
							if(0!=GetIntRefix(rImp))
							{
								nValue = Float2Int((GetIntRefix(rImp)*nRefixRate)/100.0f);
								nValue = Float2Int((rMan.GetBaseInt2()*nValue)/100.0f);
								rIntAttrRefix += nValue;
								return TRUE;
							}
						}
						break;
					case CharIntAttrRefixs_T::REFIX_DEX:
						{
							if(0!=GetDexRefix(rImp))
							{
								nValue = Float2Int((GetDexRefix(rImp)*nRefixRate)/100.0f);
								nValue = Float2Int((rMan.GetBaseDex2()*nValue)/100.0f);
								rIntAttrRefix += nValue;
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
		BOOL StdImpact016_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += GetRefixRate(rImp);
			SetRefixRate(rImp, nRate);
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};

