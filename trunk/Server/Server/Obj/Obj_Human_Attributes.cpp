//***************************************************************************************************
// �ļ�����:	Obj_Human_Attributes.cpp
// ϵͳ����:	
// ģ������:	Obj_Human_Attributes
// ����˵��:	��ɫ������
//
// ����ĵ�:	
// ��    ��:	liam
// ��������:	
// ��    ��:	
// �����Ȩ:	
//
//�޸ļ�¼:
//          1.
//				�� �� ��:liam
//				�޸�ʱ��:2007/10/30
//				�޸�����:���Ե�һ�����ع�(����μ��ĵ�)
//          2.
//				�� �� ��:liam
//				�޸�ʱ��:2007/11/02
//				�޸�����:���Եڶ������ع�(����μ��ĵ�)
//***************************************************************************************************
#include "stdafx.h"
#include "Type.h"
#include "GameDefine.h"
#include "GameDefine2.h"
#include "GameDefine_Attr.h"
#include "Obj_Human.h"
#include "LogicManager.h"
#include "GameTable.h"
#include "Config.h"
using namespace Combat_Module::Skill_Module;
using namespace MenPai_Module;
////////////////////////////////////////////////////////////////////////////////
//������Դ	E:\Project\�߻��ĵ�\����ĵ�\01 �����趨\02_����\ְҵ��λ���.xls

typedef int (Obj_Human::*GET_BASE_ATTACK_CALLBACK) (VOID);
struct  HumanAttackParam
{
	MENPAI_ATTRIBUTE			menpaiID;
	GET_BASE_ATTACK_CALLBACK	get_base_attack_callback;
	ITEM_ATTRIBUTE				base_attack_id;
	ITEM_ATTRIBUTE				base_rate_attack_id;
	ITEM_ATTRIBUTE				point_attack_id;
};

const HumanAttackParam g_TableHumanAttackParamArray[MATTRIBUTE_NUMBER] = 
{
	{MATTRIBUTE_SHAOLIN_GWS,	&Obj_Human::GetBaseAttackMagicFar,	IATTRIBUTE_BASE_ATTACK_MAGIC_FAR,	IATTRIBUTE_BASE_RATE_ATTACK_MAGIC_FAR,	IATTRIBUTE_POINT_ATTACK_MAGIC_FAR	},
	{MATTRIBUTE_SHAOLIN_TLH,	&Obj_Human::GetBaseAttackMagicNear,	IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR,	IATTRIBUTE_BASE_RATE_ATTACK_MAGIC_NEAR,	IATTRIBUTE_POINT_ATTACK_MAGIC_NEAR	},
	{MATTRIBUTE_GAIBANG_GGB,	&Obj_Human::GetBaseAttackFar,		IATTRIBUTE_BASE_ATTACK_FAR,			IATTRIBUTE_BASE_RATE_ATTACK_FAR,		IATTRIBUTE_POINT_ATTACK_FAR			},
	{MATTRIBUTE_GAIBANG_ZGB,	&Obj_Human::GetBaseAttackNear,		IATTRIBUTE_BASE_ATTACK_NEAR,		IATTRIBUTE_BASE_RATE_ATTACK_NEAR,		IATTRIBUTE_POINT_ATTACK_NEAR		},

	{MATTRIBUTE_WUDANG_JWD,		&Obj_Human::GetBaseAttackFar,		IATTRIBUTE_BASE_ATTACK_FAR,			IATTRIBUTE_BASE_RATE_ATTACK_FAR,		IATTRIBUTE_POINT_ATTACK_FAR			},
	{MATTRIBUTE_WUDANG_QWD,		&Obj_Human::GetBaseAttackNear,		IATTRIBUTE_BASE_ATTACK_NEAR,		IATTRIBUTE_BASE_RATE_ATTACK_NEAR,		IATTRIBUTE_POINT_ATTACK_NEAR		},
	{MATTRIBUTE_TANGMEN_DT,		&Obj_Human::GetBaseAttackMagicFar,	IATTRIBUTE_BASE_ATTACK_MAGIC_FAR,	IATTRIBUTE_BASE_RATE_ATTACK_MAGIC_FAR,	IATTRIBUTE_POINT_ATTACK_MAGIC_FAR	},
	{MATTRIBUTE_TANGMEN_XJT,	&Obj_Human::GetBaseAttackNear,		IATTRIBUTE_BASE_ATTACK_NEAR,		IATTRIBUTE_BASE_RATE_ATTACK_NEAR,		IATTRIBUTE_POINT_ATTACK_NEAR		},

	{MATTRIBUTE_XIAOYAO_XXY,	&Obj_Human::GetBaseAttackFar,		IATTRIBUTE_BASE_ATTACK_FAR,			IATTRIBUTE_BASE_RATE_ATTACK_FAR,		IATTRIBUTE_POINT_ATTACK_FAR			},
	{MATTRIBUTE_XIAOYAO_ZXY,	&Obj_Human::GetBaseAttackMagicNear,	IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR,	IATTRIBUTE_BASE_RATE_ATTACK_MAGIC_NEAR,	IATTRIBUTE_POINT_ATTACK_MAGIC_NEAR	},
	{MATTRIBUTE_YAOWANGGU_KYW,	&Obj_Human::GetBaseAttackFar,		IATTRIBUTE_BASE_ATTACK_FAR,			IATTRIBUTE_BASE_RATE_ATTACK_FAR,		IATTRIBUTE_POINT_ATTACK_FAR			},
	{MATTRIBUTE_YAOWANGGU_YYW,	&Obj_Human::GetBaseAttackMagicNear,	IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR,	IATTRIBUTE_BASE_RATE_ATTACK_MAGIC_NEAR,	IATTRIBUTE_POINT_ATTACK_MAGIC_NEAR	},

	{MATTRIBUTE_TAOHUAGU_DTH,	&Obj_Human::GetBaseAttackFar,		IATTRIBUTE_BASE_ATTACK_FAR,			IATTRIBUTE_BASE_RATE_ATTACK_FAR,		IATTRIBUTE_POINT_ATTACK_FAR			},
	{MATTRIBUTE_TAOHUAGU_YTH,	&Obj_Human::GetBaseAttackMagicNear,	IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR,	IATTRIBUTE_BASE_RATE_ATTACK_MAGIC_NEAR,	IATTRIBUTE_POINT_ATTACK_MAGIC_NEAR	},
	{MATTRIBUTE_EMEI_CEM,		&Obj_Human::GetBaseAttackMagicFar,	IATTRIBUTE_BASE_ATTACK_MAGIC_FAR,	IATTRIBUTE_BASE_RATE_ATTACK_MAGIC_FAR,	IATTRIBUTE_POINT_ATTACK_MAGIC_FAR	},
	{MATTRIBUTE_EMEI_FEM,		&Obj_Human::GetBaseAttackNear,		IATTRIBUTE_BASE_ATTACK_NEAR,		IATTRIBUTE_BASE_RATE_ATTACK_NEAR,		IATTRIBUTE_POINT_ATTACK_NEAR		},

	{MATTRIBUTE_WUMENPAI,		&Obj_Human::GetBaseAttackFar,		IATTRIBUTE_BASE_ATTACK_FAR,			IATTRIBUTE_BASE_RATE_ATTACK_FAR,		IATTRIBUTE_POINT_ATTACK_FAR			},
};


//////////////////////////////////////////////////////////////////////////

//��ɫ���Բ���
INT		Obj_Human::GetAttack()
{
	_ITEM_EFFECT* pIE=NULL;
	INT nBaseAttr			=0;
	INT nEquipBaseAttr		=0;
	INT nItemBaseRateRefix	=0;
	INT nItemBasePointRefix =0;
	INT	nItemPointRefix     =0;

	INT menpaiID = GetMenPai();

	AssertEx(menpaiID < MATTRIBUTE_NUMBER && menpaiID >= MATTRIBUTE_SHAOLIN_GWS ,"�Ƿ�����ID");

	if ( menpaiID >= MATTRIBUTE_NUMBER && menpaiID < MATTRIBUTE_SHAOLIN_GWS)
	{
		return 0;
	}

	const HumanAttackParam & cref_HumanAttackParam = g_TableHumanAttackParamArray[menpaiID];

	nBaseAttr= (this->*cref_HumanAttackParam.get_base_attack_callback)();

	nItemBaseRateRefix=0;

	//װ���Ļ�������Ӱ��
	pIE = ItemEffect(cref_HumanAttackParam.base_attack_id);
	Assert(pIE);
	if(pIE->IsActive())
	{
		nItemBasePointRefix= pIE->m_Attr.m_Value;
	}

	//װ���Ļ����ٷ���Ӱ��
	pIE = ItemEffect(cref_HumanAttackParam.base_rate_attack_id);
	Assert(pIE);
	if(pIE->IsActive())
	{
		nItemBaseRateRefix= pIE->m_Attr.m_Value;
	}

	//////////////////////////////////////////////////////////////////////
	//װ�������Եĵ���Ӱ��
	pIE = ItemEffect(cref_HumanAttackParam.point_attack_id);
	Assert(pIE);
	if(pIE->IsActive())
	{
		nItemPointRefix= pIE->m_Attr.m_Value;
	}

	nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) 
		+ nItemBasePointRefix + nItemPointRefix;

	return nEquipBaseAttr;
}

//Strike point
INT	Obj_Human::GetMaxStrikePoint(VOID)
{
	if(TRUE==GetMaxStrikePointDirtyFlag())
	{
		INT nValue=GetBaseMaxStrikePoint()+GetMaxStrikePointRefix();
		nValue>MAX_STRIKE_POINT?nValue=MAX_STRIKE_POINT:NULL;
		SetIntAttr(CharIntAttrs_T::ATTR_MAX_STRIKE_POINT, nValue);
		ClearMaxStrikePointDirtyFlag();
		if(GetStrikePoint()>nValue) SetStrikePoint(nValue);
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MAX_STRIKE_POINT);	
}

//Attr1: Str
INT	Obj_Human::GetStr(VOID)
{
	//����=�ȼ���������Ӱ��*�ȼ�+�������ɷ���ĵ���������+����ֶ������ϵĵ���
	//������=��һ��������������+װ���������ӵ���+�����������ӵ�����*��װ����������Ӱ��ٷֱ�+���ܶ�������Ӱ��ٷֱȣ�
	if(TRUE==GetStrDirtyFlag())
	{
		INT nItemValue = 0;
		INT nBaseAttr =	GetBaseStr();
		INT nImpactAndSkillRefix=0;
		INT nItemRateRefix=0;
		//////////////////////////////////////////////////////////////////////////
		INT nMenPaiID = GetMenPai() ;
		MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
		if(NULL == pMenPaiLogic)
		{
			AssertEx(FALSE,"[Obj_Human::GetStr]: Can't not find MenPai Logic!");
			return 0;
		}
		//�ȼ��Ը����Ե�Ӱ��ϵ��
		INT nLevelRefix = pMenPaiLogic->GetBaseStrLevelupRefix();
		//////////////////////////////////////////////////////////////////////
		//�õ����ɵĵ���
		INT	nMenPaiValue = pMenPaiLogic->GetStrLevelupRefix(GetLevel());
		//////////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		_ITEM_EFFECT* pIE = ItemEffect(IATTRIBUTE_POINT_STR) ;
		Assert( pIE ) ;
		if( pIE->IsActive() )
		{
			nItemValue= pIE->m_Attr.m_Value ;
		}
		
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix = GetStrRefix();

		nBaseAttr = Float2Int(nLevelRefix * GetLevel() / 100.0f) + nMenPaiValue + nBaseAttr + nItemValue + nImpactAndSkillRefix ;

		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_STR);
		Assert( pIE ) ;
		if( pIE->IsActive() )
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}

		INT nValue=nBaseAttr+nItemRateRefix;

		nValue = MINLV0(nValue);

		SetIntAttr(CharIntAttrs_T::ATTR_STR, nValue);
		ClearStrDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_STR);
}

INT	Obj_Human::GetBaseStr(VOID)
{
	return m_DB.GetDBAttrLvl1(CATTR_LEVEL1_STR);
}

INT	Obj_Human::GetBaseStr2(VOID)
{
	__ENTER_FUNCTION
	INT nItemValue = 0;
	INT nBaseAttr =	GetBaseStr();
	//////////////////////////////////////////////////////////////////////////
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetStr]: Can't not find MenPai Logic!");
		return 0;
	}
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetBaseStrLevelupRefix();
	//////////////////////////////////////////////////////////////////////
	//�õ����ɵĵ���
	INT	nMenPaiValue = pMenPaiLogic->GetStrLevelupRefix(GetLevel());
	//////////////////////////////////////////////////////////////////////////
	//װ�������Եĵ���Ӱ��
	_ITEM_EFFECT* pIE = ItemEffect(IATTRIBUTE_POINT_STR) ;
	Assert( pIE ) ;
	if( pIE->IsActive() )
	{
		nItemValue= pIE->m_Attr.m_Value ;
	}
	
	nBaseAttr = Float2Int(nLevelRefix * GetLevel() / 100.0f) + nMenPaiValue + nBaseAttr + nItemValue;

	return nBaseAttr;
	__LEAVE_FUNCTION

	return 0;
}

VOID 	Obj_Human::SetBaseStr(INT const nValue)
{
	m_DB.SetDBAttrLvl1(CATTR_LEVEL1_STR, nValue);
	MarkStrDirtyFlag();
	MarkAttackNearDirtyFlag();
}

//Attr1: Con
INT	Obj_Human::GetCon(VOID)
{
	if(TRUE==GetConDirtyFlag())
	{
		//����=�ȼ������ʵ�Ӱ��*�ȼ�+�������ɷ���ĵ���������+����ֶ������ϵĵ���
		INT nItemValue = 0;
		INT	nBaseAttr = GetBaseCon();
		INT nImpactAndSkillRefix=0;
		INT nItemRateRefix=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		_ITEM_EFFECT* pIE = ItemEffect(IATTRIBUTE_POINT_CON) ;
		Assert( pIE ) ;
		if( pIE->IsActive() )
		{
			nItemValue= pIE->m_Attr.m_Value ;
		}
		//////////////////////////////////////////////////////////////////////////
		INT nMenPaiID = GetMenPai() ;
		MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
		if(NULL == pMenPaiLogic)
		{
			AssertEx(FALSE,"[Obj_Human::GetCon]: Can't not find MenPai Logic!");
			return 0;
		}
		//�ȼ��Ը����Ե�Ӱ��ϵ��
		INT nLevelRefix = pMenPaiLogic->GetBaseConLevelupRefix();
		//////////////////////////////////////////////////////////////////////
		//�õ����ɵĵ���
		INT	nMenPaiValue = pMenPaiLogic->GetConLevelupRefix(GetLevel());
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix = GetConRefix();

		nBaseAttr = Float2Int(nLevelRefix * GetLevel() / 100.0f) + nMenPaiValue + nBaseAttr + nItemValue + nImpactAndSkillRefix;

		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_CON);
		Assert( pIE ) ;
		if( pIE->IsActive() )
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}

		INT nValue=nBaseAttr+nItemRateRefix;

		nValue = MINLV0(nValue);

		SetIntAttr(CharIntAttrs_T::ATTR_CON, nValue);
		ClearConDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_CON);
}

INT	Obj_Human::GetBaseCon(VOID)
{
	return m_DB.GetDBAttrLvl1(CATTR_LEVEL1_CON);
}

INT	Obj_Human::GetBaseCon2(VOID)
{
	__ENTER_FUNCTION
	INT nItemValue = 0;
	INT	nBaseAttr = GetBaseCon();
	//////////////////////////////////////////////////////////////////////
	//װ�������Եĵ���Ӱ��
	_ITEM_EFFECT* pIE = ItemEffect(IATTRIBUTE_POINT_CON) ;
	Assert( pIE ) ;
	if( pIE->IsActive() )
	{
		nItemValue= pIE->m_Attr.m_Value ;
	}
	//////////////////////////////////////////////////////////////////////////
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetCon]: Can't not find MenPai Logic!");
		return 0;
	}
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetBaseConLevelupRefix();
	//////////////////////////////////////////////////////////////////////
	//�õ����ɵĵ���
	INT	nMenPaiValue = pMenPaiLogic->GetConLevelupRefix(GetLevel());
	//////////////////////////////////////////////////////////////////////
	nBaseAttr = Float2Int(nLevelRefix * GetLevel() / 100.0f) + nMenPaiValue + nBaseAttr + nItemValue;

	return nBaseAttr;
	__LEAVE_FUNCTION
	return 0;		

}

VOID	Obj_Human::SetBaseCon(INT const nValue)
{
	m_DB.SetDBAttrLvl1(CATTR_LEVEL1_CON, nValue);
	MarkConDirtyFlag();
	MarkMaxHPDirtyFlag();
}

//Attr1: Int


INT	Obj_Human::GetInt(VOID)
{
	if(TRUE==GetIntDirtyFlag())
	{
		//����=�ȼ���������Ӱ��*�ȼ�+�������ɷ���ĵ���������+����ֶ������ϵĵ���
		INT nItemValue = 0;
		INT	nBaseAttr = GetBaseInt();
		INT nImpactAndSkillRefix=0;
		INT nItemRateRefix=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		_ITEM_EFFECT* pIE = ItemEffect(IATTRIBUTE_POINT_INT) ;
		Assert( pIE ) ;
		if( pIE->IsActive() )
		{
			nItemValue= pIE->m_Attr.m_Value ;
		}
		//////////////////////////////////////////////////////////////////////////
		INT nMenPaiID = GetMenPai() ;
		MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
		if(NULL == pMenPaiLogic)
		{
			AssertEx(FALSE,"[Obj_Human::GetInt]: Can't not find MenPai Logic!");
			return 0;
		}
		//�ȼ��Ը����Ե�Ӱ��ϵ��
		INT nLevelRefix = pMenPaiLogic->GetBaseIntLevelupRefix();
		//////////////////////////////////////////////////////////////////////
		//�õ����ɵĵ���
		INT	nMenPaiValue = pMenPaiLogic->GetIntLevelupRefix(GetLevel());
		
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix = GetIntRefix();

		nBaseAttr = Float2Int(nLevelRefix * GetLevel() / 100.0f) + nMenPaiValue + nBaseAttr + nItemValue + nImpactAndSkillRefix;

		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_INT);
		Assert( pIE ) ;
		if( pIE->IsActive() )
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}

		INT nValue=nBaseAttr + nItemRateRefix;

		nValue = MINLV0(nValue);
				
		SetIntAttr(CharIntAttrs_T::ATTR_INT, nValue);
		ClearIntDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_INT);
}

INT	Obj_Human::GetBaseInt(VOID)
{
	return m_DB.GetDBAttrLvl1(CATTR_LEVEL1_INT);
}

INT Obj_Human::GetBaseInt2(VOID)
{
	__ENTER_FUNCTION
	INT nItemValue = 0;
	INT	nBaseAttr = GetBaseInt();
	//////////////////////////////////////////////////////////////////////
	//װ�������Եĵ���Ӱ��
	_ITEM_EFFECT* pIE = ItemEffect(IATTRIBUTE_POINT_INT) ;
	Assert( pIE ) ;
	if( pIE->IsActive() )
	{
		nItemValue= pIE->m_Attr.m_Value ;
	}
	//////////////////////////////////////////////////////////////////////////
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetInt]: Can't not find MenPai Logic!");
		return 0;
	}
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetBaseIntLevelupRefix();
	//////////////////////////////////////////////////////////////////////
	//�õ����ɵĵ���
	INT	nMenPaiValue = pMenPaiLogic->GetIntLevelupRefix(GetLevel());

	nBaseAttr = Float2Int(nLevelRefix * GetLevel() / 100.0f) + nMenPaiValue + nBaseAttr + nItemValue;

	return nBaseAttr;
	
	__LEAVE_FUNCTION
	return 0;
}

VOID	Obj_Human::SetBaseInt(INT const nValue)
{
	m_DB.SetDBAttrLvl1(CATTR_LEVEL1_INT, nValue);
	MarkIntDirtyFlag();
	MarkMaxMPDirtyFlag();
	MarkCriticalDirtyFlag();
	MarkToughnessDirtyFlag();
}

//Attr1: Dex
INT	Obj_Human::GetDex(VOID)
{
	if(TRUE==GetDexDirtyFlag())
	{
		//��=�ȼ�������Ӱ��*�ȼ�+�������ɷ���ĵ���������+����ֶ������ϵĵ���
		INT nItemValue = 0;
		INT	nBaseAttr = GetBaseDex();
		INT nImpactAndSkillRefix=0;
		INT nItemRateRefix=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		_ITEM_EFFECT* pIE = ItemEffect(IATTRIBUTE_POINT_DEX) ;
		Assert( pIE ) ;
		if( pIE->IsActive() )
		{
			nItemValue= pIE->m_Attr.m_Value ;
		}
		//////////////////////////////////////////////////////////////////////////
		INT nMenPaiID = GetMenPai() ;
		MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
		if(NULL == pMenPaiLogic)
		{
			AssertEx(FALSE,"[Obj_Human::GetDex]: Can't not find MenPai Logic!");
			return 0;
		}
		//�ȼ��Ը����Ե�Ӱ��ϵ��
		INT nLevelRefix = pMenPaiLogic->GetBaseDexLevelupRefix();

		//////////////////////////////////////////////////////////////////////
		//�õ����ɵĵ���
		INT	nMenPaiValue = pMenPaiLogic->GetIntLevelupRefix(GetLevel());
		
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix = GetDexRefix();

		nBaseAttr = Float2Int(nLevelRefix * GetLevel() / 100.0f) + nMenPaiValue + nBaseAttr + nItemValue + nImpactAndSkillRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_DEX);
		Assert( pIE ) ;
		if( pIE->IsActive() )
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}

		INT nValue = nBaseAttr + nItemRateRefix;

		nValue = MINLV0(nValue);

		SetIntAttr(CharIntAttrs_T::ATTR_DEX, nValue);
		ClearDexDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_DEX);
}

INT	Obj_Human::GetBaseDex(VOID)
{
	return m_DB.GetDBAttrLvl1(CATTR_LEVEL1_DEX);
}

INT	Obj_Human::GetBaseDex2(VOID)
{
	__ENTER_FUNCTION
	//��=�ȼ�������Ӱ��*�ȼ�+�������ɷ���ĵ���������+����ֶ������ϵĵ���
	INT nItemValue = 0;
	INT	nBaseAttr = GetBaseDex();
	//////////////////////////////////////////////////////////////////////
	//װ�������Եĵ���Ӱ��
	_ITEM_EFFECT* pIE = ItemEffect(IATTRIBUTE_POINT_DEX) ;
	Assert( pIE ) ;
	if( pIE->IsActive() )
	{
		nItemValue= pIE->m_Attr.m_Value ;
	}
	//////////////////////////////////////////////////////////////////////////
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetDex]: Can't not find MenPai Logic!");
		return 0;
	}
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetBaseDexLevelupRefix();

	//////////////////////////////////////////////////////////////////////
	//�õ����ɵĵ���
	INT	nMenPaiValue = pMenPaiLogic->GetIntLevelupRefix(GetLevel());
	
	nBaseAttr = Float2Int(nLevelRefix * GetLevel() / 100.0f) + nMenPaiValue + nBaseAttr + nItemValue;

	return nBaseAttr;
		
	__LEAVE_FUNCTION
	return 0;
}

VOID	Obj_Human::SetBaseDex(INT const nValue)
{
	m_DB.SetDBAttrLvl1(CATTR_LEVEL1_DEX, nValue);
	MarkDexDirtyFlag();
	MarkAttackMagicNearDirtyFlag();
	MarkAttackFarDirtyFlag();
}



//HP
INT	Obj_Human::GetMaxHP(VOID) 
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetMaxHPDirtyFlag())
	{
		INT nBaseAttr=GetBaseMaxHP();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_POINT_MAXHP);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetMaxHPRefix();

		nBaseAttr = nBaseAttr + nItemPointRefix + nImpactAndSkillRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_MAXHP);
		Assert( pIE ) ;
		if( pIE->IsActive() )
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}
		//////////////////////////////////////////////////////////////////////

		//��ǰ��������=((��ʼ��������+���ʶ��������޵�Ӱ��ϵ��*����+�ȼ����������޵�Ӱ��ϵ��*�ȼ�)*
		//(1+װ�������������޵İٷֱ����ܺ�)+װ��������������ֵ���ܺ�)*����������������ֵ�ٷֱ����ܺ�+����������������ֵ�ܺ�
		nValue = nBaseAttr+nItemRateRefix;

		nValue = MINLV0(nValue);

		SetIntAttr(CharIntAttrs_T::ATTR_MAX_HP, nValue);
		ClearMaxHPDirtyFlag();
		if(GetHP()>nValue)	SetHP(nValue);
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MAX_HP);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseMaxHP(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseMaxHP]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitMaxHP();
	//һ�����ԶԸ����Ե�Ӱ��ϵ��
	INT nAttrLevel1Refix = pMenPaiLogic->GetMaxHPConRefix();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetMaxHPLevelRefix();

	//���������������
	INT nBase = nInitAttr+GetCon()*nAttrLevel1Refix+GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID 	Obj_Human::SetBaseMaxHP(INT const nHp)
{ // forbiden modify this attribute
}

INT	Obj_Human::GetHPRegenerate(VOID) 
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetHPRegenerateDirtyFlag())
	{
		INT nBaseAttr=GetBaseHPRegenerate();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_HP_RESTORE);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetHPRegenerateRefix();
		nValue = nBaseAttr+nImpactAndSkillRefix+nItemPointRefix;

		nValue = MINLV0(nValue);
		
		SetIntAttr(CharIntAttrs_T::ATTR_HP_REGENERATE, nValue);
		ClearHPRegenerateDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_HP_REGENERATE);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseHPRegenerate(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseHPRegenerate]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitHPRegenerate();
	////һ�����ԶԸ����Ե�Ӱ��ϵ��
	//INT nAttrLevel1Refix = pMenPaiLogic->GetHPRegenerateConRefix();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetHPRegenerateLevelRefix();

	//���������������
	INT nBase = nInitAttr+GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID 	Obj_Human::SetBaseHPRegenerate(INT const nValue)
{// forbiden modify this attribute
}
//MP
INT Obj_Human::GetMaxMP(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetMaxMPDirtyFlag())
	{
		INT nBaseAttr=GetBaseMaxMP();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_POINT_MAXMP);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetMaxMPRefix();

		nBaseAttr = nBaseAttr+nImpactAndSkillRefix+nItemPointRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_MAXMP);
		Assert( pIE ) ;
		if( pIE->IsActive() )
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}
		//////////////////////////////////////////////////////////////////////
		nValue = nBaseAttr+nItemRateRefix;

		nValue = MINLV0(nValue);
		
		SetIntAttr(CharIntAttrs_T::ATTR_MAX_MP, nValue);
		ClearMaxMPDirtyFlag();
		if(GetMP()>nValue) SetMP(nValue);
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MAX_MP);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseMaxMP(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseMaxMP]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitMaxMP();
	//һ�����ԶԸ����Ե�Ӱ��ϵ��
	INT nAttrLevel1Refix =pMenPaiLogic->GetMaxMPIntRefix();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetMaxMPLevelRefix();

	//���������������
	INT nBase = nInitAttr+GetInt()*nAttrLevel1Refix+GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID 	Obj_Human::SetBaseMaxMp(INT const nMp)
{// forbiden modify this attribute
}

INT	Obj_Human::GetMPRegenerate(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetMPRegenerateDirtyFlag())
	{
		INT nBaseAttr=GetBaseMPRegenerate();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_MP_RESTORE);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetMPRegenerateRefix();
		nValue = nBaseAttr+nImpactAndSkillRefix+nItemPointRefix+nItemRateRefix;

		nValue = MINLV0(nValue);
		
		SetIntAttr(CharIntAttrs_T::ATTR_MP_REGENERATE, nValue);
		ClearMPRegenerateDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MP_REGENERATE);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseMPRegenerate(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseMPRegenerate]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitMPRegenerate();
	////һ�����ԶԸ����Ե�Ӱ��ϵ��
	//INT nAttrLevel1Refix = pMenPaiLogic->GetMPRegenerateIntRefix();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetMPRegenerateLevelRefix();

	//���������������
	INT nBase = nInitAttr+GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID 	Obj_Human::SetBaseMPRegenerate(INT const nValue)
{// forbiden modify this attribute
}

//Pneuma
INT Obj_Human::GetMaxPneuma(VOID)
{
	__ENTER_FUNCTION
	_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetMaxPneumaDirtyFlag())
	{
		INT nBaseAttr=GetBaseMaxPneuma();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_POINT_MAX_PNEUMA);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetMaxPneumaRefix();

		nBaseAttr = nBaseAttr+nImpactAndSkillRefix+nItemPointRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_MAX_PNEUMA);
		Assert( pIE ) ;
		if( pIE->IsActive() )
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}
		//////////////////////////////////////////////////////////////////////
		nValue = nBaseAttr+nItemRateRefix;

		nValue = MINLV0(nValue);
		
		SetIntAttr(CharIntAttrs_T::ATTR_MAX_PNEUMA, nValue);
		ClearMaxPneumaDirtyFlag();
		if(GetPneuma()>nValue) SetPneuma(nValue);
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MAX_PNEUMA);
	__LEAVE_FUNCTION
	return 0;
}

INT Obj_Human::GetBaseMaxPneuma(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseMaxPneuma]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitMaxPneuma();
	//һ�����ԶԸ����Ե�Ӱ��ϵ��
	//INT nAttrLevel1Refix =0; //= pMenPaiLogic->GetMaxMPIntRefix();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetMaxPneumaLevelRefix();

	//���������������
	INT nBase = nInitAttr+GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID 	Obj_Human::SetBaseMaxPneuma(INT const nPneuma)
{// forbiden modify this attribute
}

INT	Obj_Human::GetPneumaRegenerate(VOID)
{
	__ENTER_FUNCTION
	_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetPneumaRegenerateDirtyFlag())
	{
		INT nBaseAttr=GetBasePneumaRegenerate();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_PNEUMA_RESTORE);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetPneumaRegenerateRefix();
		nValue = nBaseAttr+nImpactAndSkillRefix+nItemPointRefix+nItemRateRefix;

		nValue = MINLV0(nValue);
		
		SetIntAttr(CharIntAttrs_T::ATTR_PNEUMA_REGENERATE, nValue);
		ClearPneumaRegenerateDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_PNEUMA_REGENERATE);
	__LEAVE_FUNCTION
	return 0;
}

INT Obj_Human::GetBasePneumaRegenerate(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBasePneumaRegenerate]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitPneumaRegenerate();
	////һ�����ԶԸ����Ե�Ӱ��ϵ��
	//INT nAttrLevel1Refix = pMenPaiLogic->GetMPRegenerateIntRefix();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetPneumaRegenerateLevelRefix();

	//���������������
	INT nBase = nInitAttr+GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID 	Obj_Human::SetBasePneumaRegenerate(INT const nValue)
{// forbiden modify this attribute
}

//Rage
INT Obj_Human::GetMaxRage(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetMaxRageDirtyFlag())
	{
		INT nBaseAttr=GetBaseMaxRage();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemValue=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		_ITEM_EFFECT* pIE = ItemEffect(IATTRIBUTE_MAXRAGE) ;
		Assert( pIE ) ;
		if( pIE->IsActive() )
		{
			nItemValue= pIE->m_Attr.m_Value ;
		}

		pIE = ItemEffect(IATTRIBUTE_RATE_MAXRAGE) ;
		Assert( pIE ) ;
		if( pIE->IsActive() )
		{
			nItemRateRefix= pIE->m_Attr.m_Value ;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetMaxRageRefix();

		nBaseAttr = nBaseAttr+nImpactAndSkillRefix+nItemPointRefix;
		//////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////
		nValue = nBaseAttr+Float2Int((nBaseAttr*nItemRateRefix)/100.0f);

		nValue = MINLV0(nValue);
		
		SetIntAttr(CharIntAttrs_T::ATTR_MAX_RAGE, nValue);
		ClearMaxRageDirtyFlag();
		if(GetRage()>nValue) SetRage(nValue);
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MAX_RAGE);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseMaxRage(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseMaxRage]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitMaxRage();

	//һ�����ԶԸ����Ե�Ӱ��ϵ��
	/*INT nAttrLevel1Refix = pMenPaiLogic->GetMaxRageIntRefix();*/

	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetMaxRageLevelRefix();

	//���������������
	INT nBase = nInitAttr + /*GetInt()*nAttrLevel1Refix +*/ GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID 	Obj_Human::SetBaseMaxRage(INT const nRage)
{// forbiden modify this attribute
}

INT	Obj_Human::GetRageRegenerate(VOID)
{
	//__ENTER_FUNCTION
	//	_ITEM_EFFECT* pIE=NULL;
	//if(TRUE==GetRageRegenerateDirtyFlag())
	//{
	//	INT nBaseAttr=GetBaseRageRegenerate();
	//	INT nImpactAndSkillRefix=0;

	//	INT nItemPointRefix=0;
	//	INT nItemRateRefix=0;
	//	INT nValue=0;

	//	//////////////////////////////////////////////////////////////////////
	//	//���ܺ�Ч�������Ե�Ӱ��
	//	nImpactAndSkillRefix=GetRageRegenerateRefix();
	//	//////////////////////////////////////////////////////////////////////
	//	_ITEM_EFFECT* pIE = ItemEffect(IATTRIBUTE_POINT_RAGE_RESTORE) ;
	//	Assert( pIE ) ;

	//	if( pIE->IsActive() )
	//	{
	//		nItemPointRefix= pIE->m_Attr.m_Value ;
	//	}

	//	pIE = ItemEffect(IATTRIBUTE_RATE_RAGE_RESTORE) ;
	//	Assert( pIE ) ;

	//	if( pIE->IsActive() )
	//	{
	//		nItemRateRefix= pIE->m_Attr.m_Value ;
	//	}

	//	nValue = nBaseAttr+nImpactAndSkillRefix+nItemPointRefix+Float2Int((GetMaxRage()*nItemRateRefix)/100.0f);
	//	
	//	nValue = MINLV0(nValue);
	//	SetIntAttr(CharIntAttrs_T::ATTR_RAGE_REGENERATE, nValue);
	//	ClearRageRegenerateDirtyFlag();
	//}
	//return GetIntAttr(CharIntAttrs_T::ATTR_RAGE_REGENERATE);
	//__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseRageRegenerate(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseRageRegenerate]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitRageRegenerate();
	////һ�����ԶԸ����Ե�Ӱ��ϵ��
	//INT nAttrLevel1Refix = pMenPaiLogic->GetRageRegenerateIntRefix();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetRageRegenerateLevelRefix();

	//���������������
	INT nBase = nInitAttr+GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	//0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID 	Obj_Human::SetBaseRageRegenerate(INT const nValue)
{// forbiden modify this attribute
}

const _CAMP_DATA *Obj_Human::GetBaseCampData(VOID)const
{
	return m_DB.GetDBCampData();
}

VOID Obj_Human::SetBaseCampData(const _CAMP_DATA *pCampData)
{
	m_DB.SetDBCampData(pCampData);
}

//CampID
INT Obj_Human::GetCampID(VOID)
{
	if(TRUE==GetCampIDDirtyFlag())
	{
		INT nValue=GetCampIDRefix();
		if(INVALID_ID==nValue)
		{
			nValue=GetBaseCampID();
		}
		SetIntAttr(CharIntAttrs_T::ATTR_CAMP, nValue);
		ClearCampIDDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_CAMP);
}

INT Obj_Human::GetBaseCampID(VOID) const
{
	HumanDB * pDB=(HumanDB *)&m_DB;
	return pDB->GetDBCampData()->m_nCampID;
}

VOID	Obj_Human::SetBaseCampID(INT const nID)
{
	_CAMP_DATA stCampData = *(m_DB.GetDBCampData());
	stCampData.m_nCampID = nID;
	m_DB.SetDBCampData(&stCampData);
	MarkCampIDDirtyFlag();
}

INT Obj_Human::GetModelID(VOID)
{
	if(TRUE==GetModelIDDirtyFlag())
	{
		INT nValue=GetModelIDRefix();
		if(INVALID_ID==nValue)
		{
			nValue=GetBaseModelID();
		}
		SetIntAttr(CharIntAttrs_T::ATTR_MODEL_ID, nValue);
		ClearModelIDDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MODEL_ID);
}

//modelID
INT Obj_Human::GetBaseModelID(VOID) const
{
	return INVALID_ID;
}

VOID	Obj_Human::SetBaseModelID(INT const nID)
{ // forbiden modify this attribute
}

// Alive flag, wraped in ObjCharacter
// In Combat flag, wraped in ObjCharacter
// Can move flag, wraped in ObjCharacter
// Can Action flag, wraped in ObjCharacter
// Unbreakable flag, wraped in ObjCharacter
BOOL	Obj_Human::IsUnbreakable(VOID)
{
	return (this->Obj_Character::IsUnbreakable())||IsGod()||IsHaveChangeSceneFlag();
}

// Attr2: Move Speed, wraped in ObjCharacter
FLOAT	Obj_Human::GetMoveSpeed(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;

	if( __GetTeamFollowFlag() )
	{
		if ( GetTeamInfo()->IsLeader()==FALSE )
		{
			return __GetTeamFollowSpeed();
		}
	}

	if(TRUE==GetMoveSpeedDirtyFlag())
	{
		INT nBaseAttr=GetBaseMoveSpeed();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_SPEED_MOVE);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetMoveSpeedRefix();
		nValue = nBaseAttr+nImpactAndSkillRefix+nItemPointRefix+nItemRateRefix;
		SetIntAttr(CharIntAttrs_T::ATTR_MOVE_SPEED, nValue);
		ClearMoveSpeedDirtyFlag();
	}

	FLOAT fMoveSpeed = GetIntAttr(CharIntAttrs_T::ATTR_MOVE_SPEED) / 1000.f;
	ENUM_MOVE_MODE eMoveMode = GetMoveMode();
	if(eMoveMode == MOVE_MODE_HOBBLE)
	{//��ɻ����ٶȵ�50%
		fMoveSpeed = fMoveSpeed*0.5f;
	}
	else if(eMoveMode == MOVE_MODE_RUN)
	{//��ɻ����ٶȵ�150%
		fMoveSpeed = fMoveSpeed*1.5f;
	}
	else if (eMoveMode == MOVE_MODE_SPRINT)
	{//��ɻ����ٶȵ�500%
		fMoveSpeed = fMoveSpeed*5.0f;
	}

	return fMoveSpeed;
	__LEAVE_FUNCTION
		return 0;

}

INT	Obj_Human::GetBaseMoveSpeed(VOID)
{
	return g_Config.m_ConfigInfo.m_DefaultMoveSpeed;	
}

VOID	Obj_Human::SetBaseMoveSpeed(INT const nValue)
{// forbiden modify this attribute
}

// Attr2: Attack Speed
INT Obj_Human::GetAttackSpeed(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetAttackSpeedDirtyFlag())
	{
		INT nBaseAttr=GetBaseAttackSpeed();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_ATTACK_SPEED);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetAttackSpeedRefix();
		nValue = nBaseAttr-nImpactAndSkillRefix-nItemPointRefix;
		nValue<0?nValue=0:NULL;
		if(400<nValue)
		{
			nValue = 400;
		}
		if(25>=nValue)
		{
			nValue = 25;
		}
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_SPEED, nValue);
		ClearAttackSpeedDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_SPEED);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseAttackSpeed(VOID)
{
	//return g_Config.m_ConfigInfo.m_DefaultAttackSpeed;
	return BASE_ATTACK_SPEED; //use 100 as base cooldown and other actiontime refix;
}

VOID	Obj_Human::SetBaseAttackSpeed(INT const nValue)
{// forbiden modify this attribute
}

// Attr2: Miss
INT Obj_Human::GetMiss(VOID)
{
	__ENTER_FUNCTION
	if(TRUE==GetMissDirtyFlag())
	{
		INT nBaseAttr=GetBaseMiss();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		nItemPointRefix = GetIattributeMiss();

		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetMissRefix();
		nValue = nBaseAttr+nImpactAndSkillRefix+nItemPointRefix+nItemRateRefix;
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_MISS, nValue);
		ClearMissDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MISS);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetIattributeMiss()
{
	__ENTER_FUNCTION

	_ITEM_EFFECT* pIE=NULL;
	pIE = ItemEffect(IATTRIBUTE_MISS);
	Assert( pIE );
	if( pIE->IsActive() )
	{
		return pIE->m_Attr.m_Value;
	}	
	return 0;
	__LEAVE_FUNCTION
	return 	0;
}

INT Obj_Human::GetBaseMiss(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseMiss]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitMiss();
	//һ�����ԶԸ����Ե�Ӱ��ϵ��
	INT nAttrLevel1Refix = pMenPaiLogic->GetMissDexRefix();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetMissLevelRefix();

	//���������������
	INT nBase = nInitAttr+GetDex()*nAttrLevel1Refix+GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID	Obj_Human::SetBaseMiss(INT const nValue)
{// forbiden modify this attribute
}

// Attr2 Hit
INT Obj_Human::GetHit(VOID)
{
	__ENTER_FUNCTION
	if(TRUE==GetHitDirtyFlag())
	{
		INT nBaseAttr=GetBaseHit();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		nItemPointRefix = GetIattributeHit();

		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetHitRefix();
		nValue = nBaseAttr+nImpactAndSkillRefix+nItemPointRefix+nItemRateRefix;
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_HIT, nValue);
		ClearHitDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_HIT);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetIattributeHit()
{
	__ENTER_FUNCTION

	//װ�������Եĵ���Ӱ��
	_ITEM_EFFECT* pIE=NULL;
	pIE = ItemEffect(IATTRIBUTE_HIT);
	Assert( pIE );
	if( pIE->IsActive() )
	{
		return pIE->m_Attr.m_Value;
	}	
	return 0;
	__LEAVE_FUNCTION
	return 0;
}

INT Obj_Human::GetBaseHit(VOID)
{
	__ENTER_FUNCTION

	//����
	INT nMenPaiID = GetMenPai();
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseHit]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitHit();
	//һ�����ԶԸ����Ե�Ӱ��ϵ��
	INT nAttrLevel1Refix = pMenPaiLogic->GetHitStrRefix();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetHitLevelRefix();

	//���������������
	INT nBase = nInitAttr+GetStr()*nAttrLevel1Refix+GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;	
	return nBase;
	__LEAVE_FUNCTION
	return 0;
}

VOID	Obj_Human::SetBaseHit(INT const nValue)
{// forbiden modify this attribute
}

// Attr2 Critical
INT Obj_Human::GetCritical(VOID)
{
	__ENTER_FUNCTION
	if(TRUE==GetCriticalDirtyFlag())
	{
		INT nBaseAttr=GetBaseCritical();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		nItemPointRefix = GetIattributeCritiacalRefix();
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetCriticalRefix();
		nValue = nBaseAttr+nImpactAndSkillRefix+nItemPointRefix+nItemRateRefix;
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_CRITICAL, nValue);
		ClearCriticalDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_CRITICAL);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetIattributeCritiacalRefix()
{
	__ENTER_FUNCTION

	_ITEM_EFFECT* pIE=NULL;
	pIE = ItemEffect(IATTRIBUTE_CRIT);
	Assert( pIE );
	if( pIE->IsActive() )
	{
		return pIE->m_Attr.m_Value;
	}
	 return 0;
	__LEAVE_FUNCTION
	return 0;
}

//װ�����ӱ����˺�
INT Obj_Human::GetEquipCriticalHurt()
{
	__ENTER_FUNCTION

		_ITEM_EFFECT* pIE=NULL;
	pIE = ItemEffect(IATTRIBUTE_CRIT_HURT);
	Assert( pIE );
	if( pIE->IsActive() )
	{
		return pIE->m_Attr.m_Value;
	}
	return 0;
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetCriticalHurt()
{
	__ENTER_FUNCTION
	INT nValue = g_Config.m_ConfigInfo.m_nCriticalDamageRate + GetEquipCriticalHurt();

	return nValue;
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseCritical(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseCritical]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitCritical();
	//�����Ը����Ե�Ӱ��ϵ��
	INT nAttrIntRefix = pMenPaiLogic->GetCriticalIntRefix();
	//���ݶԸ����Ե�Ӱ��ϵ��
	INT nAttrDexRefix = pMenPaiLogic->GetCriticalDexRefix();
	////�ȼ��Ը����Ե�Ӱ��ϵ��
	//INT nLevelRefix = pMenPaiLogic->GetCriticalLevelRefix();

	//���������������
	INT nBase = nInitAttr + GetInt()*nAttrIntRefix + GetDex()*nAttrDexRefix /*+ GetLevel()*nLevelRefix*/;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID	Obj_Human::SetBaseCritical(INT const nValue)
{// forbiden modify this attribute
}

//add by liam------------------------------------
INT Obj_Human::GetDefenceNear(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetDefenceNearDirtyFlag())
	{
		INT nBaseAttr=GetBaseDefenceNear();
		INT nEquipBaseAttr=0;
		INT nImpackAndSkillBaseAtt = 0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_BASE_DEFENCE_NEAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemBasePointRefix= pIE->m_Attr.m_Value;
		}
		// passive Skill refix start
		//	...
		// passive skill refix end
		//////////////////////////////////////////////////////////////////////
		//װ���Ļ����ٷ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_BASE_RATE_DEFENCE_ENEAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemBaseRateRefix= pIE->m_Attr.m_Value;
		}

		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_POINT_DEFENCE_NEAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}

		nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_DEFENCE_NEAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemRateRefix = pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetDefenceNearRefix();

		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_DEFENCE_NEAR, nValue);
		ClearDefenceNearDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_DEFENCE_NEAR);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseDefenceNear (VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseDefenceNear]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitNearDefencePhysics();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetNearDefencePhysicsLevelRefix();

	//���������������
	INT nBase = nInitAttr+GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;
	return nBase;
}

VOID Obj_Human::SetBaseDefenceNear (INT const nValue)
{
	// forbiden modify this attribute
}

INT Obj_Human::GetDefenceFar(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetDefenceFarDirtyFlag())
	{
		INT nBaseAttr=GetBaseDefenceFar();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_BASE_DEFENCE_FAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemBasePointRefix= pIE->m_Attr.m_Value;
		}
		// passive Skill refix start
		//	...
		// passive skill refix end
		//////////////////////////////////////////////////////////////////////
		//װ���Ļ����ٷ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_BASE_RATE_DEFENCE_FAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemBaseRateRefix= pIE->m_Attr.m_Value;
		}

		nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_POINT_DEFENCE_FAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_DEFENCE_FAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemRateRefix = pIE->m_Attr.m_Value;
		}

		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetDefenceFarRefix();


		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);

		nValue = MINLV0(nValue);

		SetIntAttr(CharIntAttrs_T::ATTR_DEFENCE_FAR, nValue);
		ClearDefenceFarDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_DEFENCE_FAR);
	__LEAVE_FUNCTION
		return 0;
}
INT Obj_Human::GetBaseDefenceFar (VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseDefenceFar]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitFarDefencePhysics();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetFarDefencePhysicsLevelRefix();

	//���������������
	INT nBase = nInitAttr+GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;
	return nBase;
}

VOID Obj_Human::SetBaseDefenceFar (INT const nValue)
{
	// forbiden modify this attribute
}


INT Obj_Human::GetAttackNear(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetAttackNearDirtyFlag())
	{ 
		//if((GetMenPai() != MATTRIBUTE_GAIBANG_ZGB) 
		// &&(GetMenPai() != MATTRIBUTE_WUDANG_QWD)
		// &&(GetMenPai() != MATTRIBUTE_TANGMEN_XJT)
		// &&(GetMenPai() != MATTRIBUTE_EMEI_FEM))
		//{
		//	SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_NEAR, 0);
		//	ClearAttackNearDirtyFlag();

		//	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_NEAR);
		//}
		INT nBaseAttr=GetBaseAttackNear();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_BASE_ATTACK_NEAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemBasePointRefix= pIE->m_Attr.m_Value;
		}
		// passive Skill refix start
		//	...
		// passive skill refix end
		//////////////////////////////////////////////////////////////////////
		//װ���Ļ����ٷ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_BASE_RATE_ATTACK_NEAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemBaseRateRefix= pIE->m_Attr.m_Value;
		}

		nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_POINT_ATTACK_NEAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_ATTACK_NEAR);
		Assert(pIE) ;
		if(pIE->IsActive())
		{
			nItemRateRefix = pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetAttackNearRefix();

		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_NEAR, nValue);
		ClearAttackNearDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_NEAR);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseAttackNear(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseAttackNear]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitNearAttackPhysics();
	//һ�����ԶԸ����Ե�Ӱ��ϵ��
	INT nAttrLevel1Refix = pMenPaiLogic->GetNearAttackPhysicsIntRefix();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetNearAttackPhysicsLevelRefix();

	//���������������
	INT nBase = nInitAttr+GetInt()*nAttrLevel1Refix+GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ	
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID Obj_Human::SetBaseAttackNear(INT const nValue)
{
	// forbiden modify this attribute
}


INT Obj_Human::GetAttackFar(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetAttackFarDirtyFlag())
	{
		//if((GetMenPai() != MATTRIBUTE_GAIBANG_GGB)
		// &&(GetMenPai() != MATTRIBUTE_WUDANG_JWD)
		// &&(GetMenPai() != MATTRIBUTE_XIAOYAO_XXY)
		// &&(GetMenPai() != MATTRIBUTE_TAOHUAGU_DTH))
		//{
		//	SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_FAR, 0);
		//	ClearAttackNearDirtyFlag();

		//	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_FAR);
		//}
		INT nBaseAttr=GetBaseAttackFar();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_BASE_ATTACK_FAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemBasePointRefix= pIE->m_Attr.m_Value;
		}
		// passive Skill refix start
		//	...
		// passive skill refix end
		//////////////////////////////////////////////////////////////////////
		//װ���Ļ����ٷ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_BASE_RATE_ATTACK_FAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemBaseRateRefix= pIE->m_Attr.m_Value;
		}

		nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_POINT_ATTACK_FAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_ATTACK_FAR);
		Assert(pIE) ;
		if(pIE->IsActive())
		{
			nItemRateRefix = pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetAttackFarRefix();

		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_FAR, nValue);
		ClearAttackFarDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_FAR);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseAttackFar(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseAttackFar]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitFarAttackPhysics();
	//һ�����ԶԸ����Ե�Ӱ��ϵ��
	INT nAttrLevel1Refix = pMenPaiLogic->GetFarAttackPhysicsStrRefix();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetFarAttackPhysicsLevelRefix();

	//���������������
	INT nBase = nInitAttr+GetStr()*nAttrLevel1Refix+GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ	
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID Obj_Human::SetBaseAttackFar(INT const nValue)
{
	// forbiden modify this attribute
}

INT  Obj_Human::GetToughness(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetToughnessDirtyFlag())
	{
		INT nBaseAttr=GetBaseToughness();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		////////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_TOUGHNESS);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//// passive Skill refix start
		////	...
		//// passive skill refix end
		////////////////////////////////////////////////////////////////////////
		////װ���Ļ����ٷ���Ӱ��
		//pIE = ItemEffect(IATTRIBUTE_BASE_RATE_ATTACK_FAR);
		//Assert(pIE);
		//if(pIE->IsActive())
		//{
		//	nItemBaseRateRefix= pIE->m_Attr.m_Value;
		//}

		//nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetToughnessRefix();

		nBaseAttr = nBaseAttr + nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_TOUGHNESS, nValue);
		ClearToughnessDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_TOUGHNESS);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseToughness(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseAttackNear]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitToughness();

	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetToughnessLevelRefix();

	//���ʶ����Ե�Ӱ��
	INT nIntToughness = pMenPaiLogic->GetIntToughness();

	//���������������
	INT nBase = nInitAttr+GetLevel()*nLevelRefix + GetInt() * nIntToughness;
	nBase=Float2Int((nBase)/100); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ	
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID  Obj_Human::SetBaseToughness(INT const nValue)
{
	// forbiden modify this attribute
}


INT Obj_Human::GetNearAttReduce(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetNearAttReduceDirtyFlag())
	{
		INT nBaseAttr=GetBaseNearAttReduce();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		////////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_NEARATT_REDUCE);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		// passive Skill refix start
		//	...
		// passive skill refix end
		////////////////////////////////////////////////////////////////////////
		////װ���Ļ����ٷ���Ӱ��
		//pIE = ItemEffect(IATTRIBUTE_BASE_RATE_ATTACK_FAR);
		//Assert(pIE);
		//if(pIE->IsActive())
		//{
		//	nItemBaseRateRefix= pIE->m_Attr.m_Value;
		//}

		//nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetNearAttReduceRefix();

		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);

		SetIntAttr(CharIntAttrs_T::ATTR_NEARATT_REDUCE, nValue);
		ClearNearAttReduceDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_NEARATT_REDUCE);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseNearAttReduce(VOID)
{
	//to do:
	return 0;
}

VOID Obj_Human::SetBaseNearAttReduce(INT const nValue)
{
	// forbiden modify this attribute
}

INT 				Obj_Human::GetFarAttReduce(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetFarAttReduceDirtyFlag())
	{
		INT nBaseAttr=GetBaseFarAttReduce();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		////////////////////////////////////////////////////////////////////////
		////װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_FARATT_REDUCE);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//// passive Skill refix start
		////	...
		//// passive skill refix end
		////////////////////////////////////////////////////////////////////////
		////װ���Ļ����ٷ���Ӱ��
		//pIE = ItemEffect(IATTRIBUTE_BASE_RATE_ATTACK_FAR);
		//Assert(pIE);
		//if(pIE->IsActive())
		//{
		//	nItemBaseRateRefix= pIE->m_Attr.m_Value;
		//}

		//nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetFarAttReduceRefix();

		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);

		SetIntAttr(CharIntAttrs_T::ATTR_FARATT_REDUCE, nValue);
		ClearFarAttReduceDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_FARATT_REDUCE);
	__LEAVE_FUNCTION
		return 0;
}
INT 				Obj_Human::GetBaseFarAttReduce(VOID)
{
	return 0;
}
VOID				Obj_Human::SetBaseFarAttReduce(INT const nValue)
{

}

INT 				Obj_Human::GetMagicNearAttReduce(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetMagicNearAttReduceDirtyFlag())
	{
		INT nBaseAttr=GetBaseMagicNearAttReduce();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		////////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_MAGIC_NEAR_ATT_REDUCE);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//// passive Skill refix start
		////	...
		//// passive skill refix end
		////////////////////////////////////////////////////////////////////////
		////װ���Ļ����ٷ���Ӱ��
		//pIE = ItemEffect(IATTRIBUTE_BASE_RATE_ATTACK_FAR);
		//Assert(pIE);
		//if(pIE->IsActive())
		//{
		//	nItemBaseRateRefix= pIE->m_Attr.m_Value;
		//}

		//nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetMagicNearAttReduceRefix();

		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);

		SetIntAttr(CharIntAttrs_T::ATTR_MAGIC_NEAR_ATT_REDUCE, nValue);
		ClearMagicNearAttReduceDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MAGIC_NEAR_ATT_REDUCE);
	__LEAVE_FUNCTION
		return 0;
}
INT 				Obj_Human::GetMagicFarAttReduce(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetMagicFarAttReduceDirtyFlag())
	{
		INT nBaseAttr=GetBaseMagicFarAttReduce();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		////////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_MAGIC_FAR_ATT_REDUCE);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//// passive Skill refix start
		////	...
		//// passive skill refix end
		////////////////////////////////////////////////////////////////////////
		////װ���Ļ����ٷ���Ӱ��
		//pIE = ItemEffect(IATTRIBUTE_BASE_RATE_ATTACK_FAR);
		//Assert(pIE);
		//if(pIE->IsActive())
		//{
		//	nItemBaseRateRefix= pIE->m_Attr.m_Value;
		//}

		//nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetMagicFarAttReduceRefix();

		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);

		SetIntAttr(CharIntAttrs_T::ATTR_MAGIC_FAR_ATT_REDUCE, nValue);
		ClearMagicFarAttReduceDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MAGIC_FAR_ATT_REDUCE);
	__LEAVE_FUNCTION
		return 0;
}

INT 				Obj_Human::GetBaseMagicNearAttReduce(VOID)
{
	return 0;
}
INT 				Obj_Human::GetBaseMagicFarAttReduce(VOID)
{
	return 0;
}

VOID				Obj_Human::SetBaseMagicNearAttReduce(INT const nValue)
{

}
VOID				Obj_Human::SetBaseMagicFarAttReduce(INT const nValue)
{

}

INT 				Obj_Human::GetDreadAttResist(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetDreadAttResistDirtyFlag())
	{
		INT nBaseAttr=GetBaseDreadAttResist();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		////////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_DREAD_RESIST);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//// passive Skill refix start
		////	...
		//// passive skill refix end
		////////////////////////////////////////////////////////////////////////
		////װ���Ļ����ٷ���Ӱ��
		//pIE = ItemEffect(IATTRIBUTE_BASE_RATE_ATTACK_FAR);
		//Assert(pIE);
		//if(pIE->IsActive())
		//{
		//	nItemBaseRateRefix= pIE->m_Attr.m_Value;
		//}

		//nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetDreadAttResistRefix();

		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);

		SetIntAttr(CharIntAttrs_T::ATTR_DREAD_RESIST, nValue);
		ClearDreadAttResistDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_DREAD_RESIST);
	__LEAVE_FUNCTION
		return 0;
}
INT 				Obj_Human::GetBaseDreadAttResist(VOID)
{
	return 0;
}
VOID				Obj_Human::SetBaseDreadAttResist(INT const nValue)
{

}

INT 				Obj_Human::GetComaAttResist(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetComaAttResistDirtyFlag())
	{
		INT nBaseAttr=GetBaseComaAttResist();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		////////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_COMA_RESIST);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//// passive Skill refix start
		////	...
		//// passive skill refix end
		////////////////////////////////////////////////////////////////////////
		////װ���Ļ����ٷ���Ӱ��
		//pIE = ItemEffect(IATTRIBUTE_BASE_RATE_ATTACK_FAR);
		//Assert(pIE);
		//if(pIE->IsActive())
		//{
		//	nItemBaseRateRefix= pIE->m_Attr.m_Value;
		//}

		//nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetComaAttResistRefix();

		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);

		SetIntAttr(CharIntAttrs_T::ATTR_COMA_RESIST, nValue);
		ClearFarAttReduceDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_COMA_RESIST);
	__LEAVE_FUNCTION
		return 0;
}
INT 				Obj_Human::GetBaseComaAttResist(VOID)
{
	return 0;
}
VOID				Obj_Human::SetBaseComaAttResist(INT const nValue)
{

}

//��Ĭ����
INT 				Obj_Human::GetHushAttResist(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetHushAttResistDirtyFlag())
	{
		INT nBaseAttr=GetBaseHushAttResist();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		////////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_HUSH_RESIST);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//// passive Skill refix start
		////	...
		//// passive skill refix end
		////////////////////////////////////////////////////////////////////////
		////װ���Ļ����ٷ���Ӱ��
		//pIE = ItemEffect(IATTRIBUTE_BASE_RATE_ATTACK_FAR);
		//Assert(pIE);
		//if(pIE->IsActive())
		//{
		//	nItemBaseRateRefix= pIE->m_Attr.m_Value;
		//}

		//nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetHushAttResistRefix();

		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);

		SetIntAttr(CharIntAttrs_T::ATTR_HUSH_RESIST, nValue);
		ClearHushAttResistDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_HUSH_RESIST);
	__LEAVE_FUNCTION
		return 0;
}
INT 				Obj_Human::GetBaseHushAttResist(VOID)
{
	return 0;
}
VOID				Obj_Human::SetBaseHushAttResist(INT const nValue)
{

}

//��е����
INT 				Obj_Human::GetUnArmyAttResist(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetUnArmyAttResistDirtyFlag())
	{
		INT nBaseAttr=GetBaseUnArmyAttResist();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		////////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_UNARM_RESIST);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//// passive Skill refix start
		////	...
		//// passive skill refix end
		////////////////////////////////////////////////////////////////////////
		////װ���Ļ����ٷ���Ӱ��
		//pIE = ItemEffect(IATTRIBUTE_BASE_RATE_ATTACK_FAR);
		//Assert(pIE);
		//if(pIE->IsActive())
		//{
		//	nItemBaseRateRefix= pIE->m_Attr.m_Value;
		//}

		//nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetUnArmyAttResistRefix();

		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);

		SetIntAttr(CharIntAttrs_T::ATTR_UNARMY_RESIST, nValue);
		ClearUnArmyAttResistDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_UNARMY_RESIST);
	__LEAVE_FUNCTION
		return 0;
}
INT 				Obj_Human::GetBaseUnArmyAttResist(VOID)
{
	return 0;
}
VOID				Obj_Human::SetBaseUnArmyAttResist(INT const nValue)
{

}

//�����ٿ���
INT 				Obj_Human::GetAttSpeedAttResist(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetAttSpeedAttResistDirtyFlag())
	{
		INT nBaseAttr=GetBaseAttSpeedAttResist();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		////////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_ATTACKSPEED_RESIST);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//// passive Skill refix start
		////	...
		//// passive skill refix end
		////////////////////////////////////////////////////////////////////////
		////װ���Ļ����ٷ���Ӱ��
		//pIE = ItemEffect(IATTRIBUTE_BASE_RATE_ATTACK_FAR);
		//Assert(pIE);
		//if(pIE->IsActive())
		//{
		//	nItemBaseRateRefix= pIE->m_Attr.m_Value;
		//}

		//nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetAttSpeedAttResistRefix();

		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);

		SetIntAttr(CharIntAttrs_T::ATTR_ATTACKSPEED_RESIST, nValue);
		ClearAttSpeedAttResistDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACKSPEED_RESIST);
	__LEAVE_FUNCTION
		return 0;
}
INT 				Obj_Human::GetBaseAttSpeedAttResist(VOID)
{
	return 0;
}
VOID				Obj_Human::SetBaseAttSpeedAttResist(INT const nValue)
{

}

//�����ٿ���
INT 				Obj_Human::GetSkillSpeedAttResist(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetSkillSpeedAttResistDirtyFlag())
	{
		INT nBaseAttr=GetBaseSkillSpeedAttResist();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		////////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_SKILLSPEED_RESIST);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//// passive Skill refix start
		////	...
		//// passive skill refix end
		////////////////////////////////////////////////////////////////////////
		////װ���Ļ����ٷ���Ӱ��
		//pIE = ItemEffect(IATTRIBUTE_BASE_RATE_ATTACK_FAR);
		//Assert(pIE);
		//if(pIE->IsActive())
		//{
		//	nItemBaseRateRefix= pIE->m_Attr.m_Value;
		//}

		//nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetSkillSpeedAttResistRefix();

		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);

		SetIntAttr(CharIntAttrs_T::ATTR_SKILLSPEED_RESIST, nValue);
		ClearSkillSpeedAttResistDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_SKILLSPEED_RESIST);
	__LEAVE_FUNCTION
		return 0;
}
INT 				Obj_Human::GetBaseSkillSpeedAttResist(VOID)
{
	return 0;
}
VOID				Obj_Human::SetBaseSkillSpeedAttResist(INT const nValue)
{

}

//�����ٿ���
INT 				Obj_Human::GetMoveSpeedAttResist(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetMoveSpeedAttResistDirtyFlag())
	{
		INT nBaseAttr=GetBaseMoveSpeedAttResist();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		////////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_MOVEREDUCE_RESIST);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//// passive Skill refix start
		////	...
		//// passive skill refix end
		////////////////////////////////////////////////////////////////////////
		////װ���Ļ����ٷ���Ӱ��
		//pIE = ItemEffect(IATTRIBUTE_BASE_RATE_ATTACK_FAR);
		//Assert(pIE);
		//if(pIE->IsActive())
		//{
		//	nItemBaseRateRefix= pIE->m_Attr.m_Value;
		//}

		//nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetMoveSpeedAttResistRefix();

		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);

		SetIntAttr(CharIntAttrs_T::ATTR_MOVEREDUCE_RESIST, nValue);
		ClearMoveSpeedAttResistDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MOVEREDUCE_RESIST);
	__LEAVE_FUNCTION
		return 0;
}
INT 				Obj_Human::GetBaseMoveSpeedAttResist(VOID)
{
	return 0;
}
VOID				Obj_Human::SetBaseMoveSpeedAttResist(INT const nValue)
{

}
//-----------------------------------------------------------
// Attr2: 
INT Obj_Human::GetAttackMagicNear(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetAttackMagicNearDirtyFlag())
	{
		//if((GetMenPai() != MATTRIBUTE_SHAOLIN_TLH)
		// &&(GetMenPai() != MATTRIBUTE_XIAOYAO_ZXY)
		// &&(GetMenPai() != MATTRIBUTE_YAOWANGGU_YYW)
		// &&(GetMenPai() != MATTRIBUTE_TAOHUAGU_YTH))
		//{
		//	SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_MAGIC_NEAR, 0);
		//	ClearAttackNearDirtyFlag();

		//	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_MAGIC_NEAR);
		//}
		INT nBaseAttr=GetBaseAttackMagicNear();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemBasePointRefix= pIE->m_Attr.m_Value;
		}
		// passive Skill refix start
		//	...
		// passive skill refix end
		//////////////////////////////////////////////////////////////////////
		//װ���Ļ����ٷ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_BASE_RATE_ATTACK_MAGIC_NEAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemBaseRateRefix= pIE->m_Attr.m_Value;
		}

		nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_POINT_ATTACK_MAGIC_NEAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_ATTACK_MAGIC_NEAR);
		Assert(pIE) ;
		if(pIE->IsActive())
		{
			nItemRateRefix = pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetAttackMagicNearRefix();

		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_MAGIC_NEAR, nValue);
		ClearAttackMagicNearDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_MAGIC_NEAR);
	__LEAVE_FUNCTION
		return 0;
}
INT Obj_Human::GetAttackMagicFar(VOID)
{
	__ENTER_FUNCTION

	_ITEM_EFFECT* pIE=NULL;

	if(TRUE==GetAttackMagicFarDirtyFlag())
	{
		//if((GetMenPai() != MATTRIBUTE_SHAOLIN_GWS)
		// &&(GetMenPai() != MATTRIBUTE_TANGMEN_DT)
		// &&(GetMenPai() != MATTRIBUTE_YAOWANGGU_KYW)
		// &&(GetMenPai() != MATTRIBUTE_EMEI_CEM)
		// &&(GetMenPai() != MATTRIBUTE_WUMENPAI))
		//{
		//	SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_MAGIC_FAR, 0);
		//	ClearAttackFarDirtyFlag();

		//	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_MAGIC_FAR);
		//}
		INT nBaseAttr=GetBaseAttackMagicFar();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_BASE_ATTACK_MAGIC_FAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemBasePointRefix= pIE->m_Attr.m_Value;
		}
		// passive Skill refix start
		//	...
		// passive skill refix end
		//////////////////////////////////////////////////////////////////////
		//װ���Ļ����ٷ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_BASE_RATE_ATTACK_MAGIC_FAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemBaseRateRefix= pIE->m_Attr.m_Value;
		}

		nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_POINT_ATTACK_MAGIC_FAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_ATTACK_MAGIC_FAR);
		Assert(pIE) ;
		if(pIE->IsActive())
		{
			nItemRateRefix = pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetAttackMagicFarRefix();

		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_MAGIC_FAR, nValue);
		ClearAttackMagicFarDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_MAGIC_FAR);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseAttackMagicNear(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseAttackMagicNear]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitAttackMagicNear();
	//һ�����ԶԸ����Ե�Ӱ��ϵ��
	INT nAttrLevel1Refix = pMenPaiLogic->GetAttackMagicNearIniRefix();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetAttackMagicNearLevelRefix();

	//���������������
	INT nBase = nInitAttr+GetDex()*nAttrLevel1Refix+GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ	
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

INT Obj_Human::GetBaseAttackMagicFar(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseAttackMagicFar]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitAttackMagicFar();
	//һ�����ԶԸ����Ե�Ӱ��ϵ��
	INT nAttrLevel1Refix = pMenPaiLogic->GetAttackMagicFarIniRefix();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetAttackMagicFarLevelRefix();

	//���������������
	INT nBase = nInitAttr+GetStr()*nAttrLevel1Refix+GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ	
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID	Obj_Human::SetBaseAttackMagicNear(INT const nValue)
{// forbiden modify this attribute
}

VOID	Obj_Human::SetBaseAttackMagicFar(INT const nValue)
{
}

INT Obj_Human::GetDefenceMagicNear(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetDefenceMagicNearDirtyFlag())
	{
		INT nBaseAttr=GetBaseDefenceMagicNear();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemBasePointRefix= pIE->m_Attr.m_Value;
		}
		// passive Skill refix start
		//	...
		// passive skill refix end
		//////////////////////////////////////////////////////////////////////
		//װ���Ļ����ٷ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_BASE_RATE_DEFENCE_MAGIC_NEAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemBaseRateRefix= pIE->m_Attr.m_Value;
		}

		nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_POINT_DEFENCE_MAGIC_NEAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_DEFENCE_MAGIC_NEAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetDefenceMagicNearRefix();

		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_DEFENCE_MAGIC_NEAR, nValue);
		ClearDefenceMagicNearDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_DEFENCE_MAGIC_NEAR);
	__LEAVE_FUNCTION
		return 0;
}
INT Obj_Human::GetDefenceMagicFar(VOID)
{
	__ENTER_FUNCTION
	_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetDefenceMagicFarDirtyFlag())
	{
		INT nBaseAttr=GetBaseDefenceMagicFar();
		INT nEquipBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nItemBasePointRefix=0;
		INT nItemBaseRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ���Ļ�������Ӱ��
		pIE = ItemEffect(IATTRIBUTE_BASE_DEFENCE_MAGIC_FAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemBasePointRefix= pIE->m_Attr.m_Value;
		}
		// passive Skill refix start
		//	...
		// passive skill refix end
		//////////////////////////////////////////////////////////////////////
		//װ���Ļ����ٷ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_BASE_RATE_DEFENCE_MAGIC_FAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemBaseRateRefix= pIE->m_Attr.m_Value;
		}

		nEquipBaseAttr = nBaseAttr + Float2Int((nItemBasePointRefix + nBaseAttr)*nItemBaseRateRefix/100.0f) + nItemBasePointRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_POINT_DEFENCE_MAGIC_FAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_DEFENCE_MAGIC_FAR);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetDefenceMagicFarRefix();

		nBaseAttr = nEquipBaseAttr + nImpactAndSkillRefix + nItemPointRefix;
		nValue = nBaseAttr +  Float2Int(nBaseAttr*nItemRateRefix/100.0f);
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_DEFENCE_MAGIC_FAR, nValue);
		ClearDefenceMagicFarDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_DEFENCE_MAGIC_FAR);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseDefenceMagicNear(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseDefenceMagicNear]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitDefenceMagicNear();
	////һ�����ԶԸ����Ե�Ӱ��ϵ��
	//INT nAttrLevel1Refix = pMenPaiLogic->GetDefenceMagicIntRefix();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetDefenceMagicNearLevelRefix();

	//���������������
	INT nBase = nInitAttr+GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;	
	return nBase;
}
INT Obj_Human::GetBaseDefenceMagicFar(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseDefenceMagicFar]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitDefenceMagicFar();
	////һ�����ԶԸ����Ե�Ӱ��ϵ��
	//INT nAttrLevel1Refix = pMenPaiLogic->GetDefenceMagicIntRefix();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetDefenceMagicFarLevelRefix();

	//���������������
	INT nBase = nInitAttr+GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID	Obj_Human::SetBaseDefenceMagicNear(INT const nValue)
{// forbiden modify this attribute
}
VOID	Obj_Human::SetBaseDefenceMagicFar(INT const nValue)
{// forbiden modify this attribute
}

// Attr2: Attack and Defence power of gold
INT Obj_Human::GetAttackGold(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetAttackGoldDirtyFlag())
	{
		INT nBaseAttr=GetBaseAttackGold();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_GOLD_ATTACK);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}

		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetAttackGoldRefix();


		nBaseAttr = nBaseAttr + nItemPointRefix + nImpactAndSkillRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_GOLD_ATTACK);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}
		
		
		nValue = nBaseAttr + nItemRateRefix;
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_GOLD, nValue);
		ClearAttackGoldDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_GOLD);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseAttackGold(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseAttackGold]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitAttackGold();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetLevelAttackGold();

	//���������������
	INT nBase = nInitAttr + GetLevel()*nLevelRefix;;
	nBase=Float2Int((nBase)/100); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ	
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID	Obj_Human::SetBaseAttackGold(INT const nValue)
{// forbiden modify this attribute
}

INT Obj_Human::GetDefenceGold(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetResistGoldDirtyFlag())
	{
		INT nBaseAttr=GetBaseDefenceGold();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_GOLD_RESIST);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetDefenceGoldRefix();

		nBaseAttr = nBaseAttr + nItemPointRefix + nImpactAndSkillRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_GOLD_RESIST);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}
		nValue = nBaseAttr+nItemRateRefix;
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_RESIST_GOLD, nValue);
		ClearResistGoldDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_RESIST_GOLD);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseDefenceGold(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseDefenceGold]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitDefenceGold();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetLevelDefenceGold();

	//���������������
	INT nBase = nInitAttr + GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;
	return nBase;
}

VOID	Obj_Human::SetBaseDefenceGold(INT const nValue)
{// forbiden modify this attribute
}

// Attr2: Attack and Defence power of Wood
INT Obj_Human::GetAttackWood(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetAttackWoodDirtyFlag())
	{
		INT nBaseAttr=GetBaseAttackWood();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_WOOD_ATTACK);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetAttackWoodRefix();

		nBaseAttr = nBaseAttr + nItemPointRefix + nImpactAndSkillRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_WOOD_ATTACK);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}

		nValue = nBaseAttr+nItemRateRefix;
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_WOOD, nValue);
		ClearAttackWoodDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_WOOD);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseAttackWood(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseAttackWood]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitAttackWood();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetLevelAttackWood();

	//���������������
	INT nBase = nInitAttr + GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ	
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID	Obj_Human::SetBaseAttackWood(INT const nValue)
{// forbiden modify this attribute
}

INT Obj_Human::GetDefenceWood(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetResistWoodDirtyFlag())
	{
		INT nBaseAttr=GetBaseDefenceWood();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_WOOD_RESIST);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetDefenceWoodRefix();

		nBaseAttr = nBaseAttr + nItemPointRefix + nImpactAndSkillRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_WOOD_RESIST);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}

		nValue = nBaseAttr+nItemRateRefix;
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_RESIST_WOOD, nValue);
		ClearResistWoodDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_RESIST_WOOD);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseDefenceWood(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseDefenceWood]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitDefenceWood();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetLevelDefenceWood();

	//���������������
	INT nBase = nInitAttr + GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;
	return nBase;
}

VOID	Obj_Human::SetBaseDefenceWood(INT const nValue)
{// forbiden modify this attribute
}

// Attr2: Attack and Defence power of Water
INT Obj_Human::GetAttackWater(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetAttackWaterDirtyFlag())
	{
		INT nBaseAttr=GetBaseAttackWater();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_WATER_ATTACK);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetAttackWaterRefix();

		nBaseAttr = nBaseAttr + nItemPointRefix + nImpactAndSkillRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_WATER_ATTACK);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}

		nValue = nBaseAttr+nItemRateRefix;
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_WATER, nValue);
		ClearAttackWaterDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_WATER);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseAttackWater(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseAttackWater]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitAttackWater();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetLevelAttackWater();

	//���������������
	INT nBase = nInitAttr + GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ	
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID	Obj_Human::SetBaseAttackWater(INT const nValue)
{// forbiden modify this attribute
}

INT Obj_Human::GetDefenceWater(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetResistWaterDirtyFlag())
	{
		INT nBaseAttr=GetBaseDefenceWater();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_WATER_RESIST);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetDefenceWaterRefix();

		nBaseAttr = nBaseAttr + nItemPointRefix + nImpactAndSkillRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_WATER_RESIST);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}

		nValue = nBaseAttr+nItemRateRefix;
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_RESIST_WATER, nValue);
		ClearResistWaterDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_RESIST_WATER);
	__LEAVE_FUNCTION
		return 0;
}
INT Obj_Human::GetBaseDefenceWater(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseDefenceWater]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitDefenceWater();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetLevelDefenceWater();

	//���������������
	INT nBase = nInitAttr + GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;
	return nBase;
}

VOID	Obj_Human::SetBaseDefenceWater(INT const nValue)
{// forbiden modify this attribute
}

// Attr2: Attack and Defence power of Fire
INT Obj_Human::GetAttackFire(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetAttackFireDirtyFlag())
	{
		INT nBaseAttr=GetBaseAttackFire();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_FIRE_ATTACK);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetAttackFireRefix();

		nBaseAttr = nBaseAttr + nItemPointRefix + nImpactAndSkillRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_FIRE_ATTACK);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}

		nValue = nBaseAttr+nItemRateRefix;
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_FIRE, nValue);
		ClearAttackFireDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_FIRE);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseAttackFire(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseAttackFire]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitAttackFire();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetLevelAttackFire();

	//���������������
	INT nBase = nInitAttr + GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ	
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID	Obj_Human::SetBaseAttackFire(INT const nValue)
{// forbiden modify this attribute
}

INT Obj_Human::GetDefenceFire(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetResistFireDirtyFlag())
	{
		INT nBaseAttr=GetBaseDefenceFire();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_FIRE_RESIST);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetDefenceFireRefix();

		nBaseAttr = nBaseAttr + nItemPointRefix + nImpactAndSkillRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_FIRE_RESIST);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}

		nValue = nBaseAttr+nItemRateRefix;
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_RESIST_FIRE, nValue);
		ClearResistFireDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_RESIST_FIRE);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseDefenceFire(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseDefenceFire]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitDefenceFire();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetLevelDefenceFire();

	//���������������
	INT nBase = nInitAttr + GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;
	return nBase;
}

VOID	Obj_Human::SetBaseDefenceFire(INT const nValue)
{// forbiden modify this attribute
}

// Attr2: Attack and Defence power of Soil
INT Obj_Human::GetAttackSoil(VOID)
{
	__ENTER_FUNCTION
	_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetAttackSoilDirtyFlag())
	{
		INT nBaseAttr=GetBaseAttackSoil();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_SOIL_ATTACK);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetAttackSoilRefix();

		nBaseAttr = nBaseAttr + nItemPointRefix + nImpactAndSkillRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_SOIL_ATTACK);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}

		nValue = nBaseAttr+nItemRateRefix;
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_SOIL, nValue);
		ClearAttackSoilDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_SOIL);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseAttackSoil(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseAttackSoil]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitAttackSoil();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetLevelAttackSoil();

	//���������������
	INT nBase = nInitAttr + GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ	
	0<=nBase?NULL:nBase=0;	
	return nBase;
}

VOID	Obj_Human::SetBaseAttackSoil(INT const nValue)
{// forbiden modify this attribute
}

INT Obj_Human::GetDefenceSoil(VOID)
{
	__ENTER_FUNCTION
	_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetResistSoilDirtyFlag())
	{
		INT nBaseAttr=GetBaseDefenceSoil();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		pIE = ItemEffect(IATTRIBUTE_SOIL_RESIST);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetDefenceSoilRefix();

		nBaseAttr = nBaseAttr + nItemPointRefix + nImpactAndSkillRefix;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եİٷֱ�Ӱ��
		pIE = ItemEffect(IATTRIBUTE_RATE_SOIL_RESIST);
		Assert(pIE);
		if(pIE->IsActive())
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}

		nValue = nBaseAttr+nItemRateRefix;
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_RESIST_SOIL, nValue);
		ClearResistSoilDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_RESIST_SOIL);
	__LEAVE_FUNCTION
	return 0;
}

INT Obj_Human::GetBaseDefenceSoil(VOID)
{
	//����
	INT nMenPaiID = GetMenPai() ;
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::GetBaseDefenceSoil]: Can't not find MenPai Logic!");
		return 0;
	}
	//��ʼ����ֵ
	INT nInitAttr = pMenPaiLogic->GetInitDefenceSoil();
	//�ȼ��Ը����Ե�Ӱ��ϵ��
	INT nLevelRefix = pMenPaiLogic->GetLevelDefenceSoil();

	//���������������
	INT nBase = nInitAttr + GetLevel()*nLevelRefix;
	nBase=Float2Int((nBase)/100.0f); //���������ֵ���ǳ���100�ģ������Ӧ��������ȥ
	0<=nBase?NULL:nBase=0;
	return nBase;
}

VOID	Obj_Human::SetBaseDefenceSoil(INT const nValue)
{// forbiden modify this attribute
}
// Attr2 Reduce Slower Duration
INT Obj_Human::GetReduceSlowerDuration(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetReduceSlowerDurationDirtyFlag())
	{
		INT nBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		/*pIE = ItemEffect(IATTRIBUTE_COLD_TIME);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}*/
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetReduceSlowerDurationRefix();
		nValue = nBaseAttr+nImpactAndSkillRefix+nItemPointRefix+nItemRateRefix;
		SetIntAttr(CharIntAttrs_T::ATTR_REDUCE_SLOWER_DURATION, nValue);
		ClearReduceSlowerDurationDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_REDUCE_SLOWER_DURATION);
	__LEAVE_FUNCTION
		return 0;
}

// Attr2 Reduce Weaken Duration
INT Obj_Human::GetReduceWeakenDuration(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetReduceWeakenDurationDirtyFlag())
	{
		INT nBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		/*pIE = ItemEffect(IATTRIBUTE_FIRE_TIME);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}*/
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetReduceWeakenDurationRefix();
		nValue = nBaseAttr+nImpactAndSkillRefix+nItemPointRefix+nItemRateRefix;
		SetIntAttr(CharIntAttrs_T::ATTR_REDUCE_WEAKEN_DURATION, nValue);
		ClearReduceWeakenDurationDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_REDUCE_WEAKEN_DURATION);
	__LEAVE_FUNCTION
		return 0;
}

// Attr2 Reduce Faint Duration
INT Obj_Human::GetReduceFaintDuration(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetReduceFaintDurationDirtyFlag())
	{
		INT nBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		/*pIE = ItemEffect(IATTRIBUTE_WOOD_TIME);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}*/
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetReduceFaintDurationRefix();
		nValue = nBaseAttr+nImpactAndSkillRefix+nItemPointRefix+nItemRateRefix;
		SetIntAttr(CharIntAttrs_T::ATTR_REDUCE_FAINT_DURATION, nValue);
		ClearReduceFaintDurationDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_REDUCE_FAINT_DURATION);
	__LEAVE_FUNCTION
		return 0;
}

// Attr2 Reduce Poisoned Duration
INT Obj_Human::GetReducePoisonedDuration(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetReducePoisonedDurationDirtyFlag())
	{
		INT nBaseAttr=0;
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//װ�������Եĵ���Ӱ��
		/*pIE = ItemEffect(IATTRIBUTE_POISON_TIME);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}*/
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetReducePoisonedDurationRefix();
		nValue = nBaseAttr+nImpactAndSkillRefix+nItemPointRefix+nItemRateRefix;
		SetIntAttr(CharIntAttrs_T::ATTR_REDUCE_POISONED_DURATION, nValue);
		ClearReducePoisonedDurationDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_REDUCE_POISONED_DURATION);
	__LEAVE_FUNCTION
		return 0;
}

// Attr2 VisionRange
INT Obj_Human::GetVisionRange(VOID)
{
	__ENTER_FUNCTION
		_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetVisionRangeDirtyFlag())
	{
		INT nBaseAttr=GetBaseVisionRange();
		INT nImpactAndSkillRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;
		//////////////////////////////////////////////////////////////////////
		//���ܺ�Ч�������Ե�Ӱ��
		nImpactAndSkillRefix=GetVisionRangeRefix();
		nValue = nBaseAttr+nImpactAndSkillRefix+nItemPointRefix+nItemRateRefix;
		SetIntAttr(CharIntAttrs_T::ATTR_VISION_RANGE, nValue);
		ClearVisionRangeDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_VISION_RANGE);
	__LEAVE_FUNCTION
		return 0;
}

INT Obj_Human::GetBaseVisionRange(VOID)
{
	return BASE_VISION_RANGE;
}

VOID	Obj_Human::SetBaseVisionRange(INT const nValue)
{// forbiden modify this attribute
}

//mountID   [7/23/2010 �¾���]
INT Obj_Human::GetMountID(VOID)
{
	return GetDB()->GetMountID();
	//PET_GUID_t petID = GetCurrentHorseGUID();
	//
	//if( !petID.IsNull() )
	//{
	//	Item* pItem = GetHorseItem(petID);
	//	if( pItem != NULL )
	//	{
	//		INT iDataID = pItem->GetDataID();
	//		_HORSE_ATTR *pPetAttr = NULL;

	//		pPetAttr = g_HorseAttrTbl.GetAttr( iDataID );
	//		if( pPetAttr == NULL )
	//		{
	//			Assert(0);
	//		}
	//		SetMountID(pPetAttr->m_iMountID);
	//	}
	//}
	//return GetIntAttr(CharIntAttrs_T::ATTR_MOUNT_ID);
}

//mountIndex  [7/23/2010 �¾���]
INT Obj_Human::GetMountIndex(VOID)
{
	return GetDB()->GetMountIndex();
}