// Obj_Pet.inl

#ifndef __OBJ_PET_INL__
#define __OBJ_PET_INL__
#include "ItemOperator.h"
#include "GCOperateResult.h"
#include "Obj_Pet.h"

_IMPACT_LIST& Obj_Pet::Impact_GetImpactList(VOID)
{
	return m_ImpactList;
}

INT Obj_Pet::GetCaptureCount(VOID)const
{
	return m_nCapturerCount;
}

VOID Obj_Pet::CleanAllCapturer(VOID)
{
	m_nCapturerCount = 0;
}

AI_Pet* Obj_Pet::GetPetAI( VOID )const
{
	return ((AI_Pet*)m_pAI_Character);
}

VOID Obj_Pet::SetTargetID( ObjID_t TargetID )
{
	m_TargetID = TargetID;
}

ObjID_t Obj_Pet::GetTargetID( VOID )const
{
	return m_TargetID;
}

const WORLD_POS* Obj_Pet::GetRespawnPos( VOID )const
{
	return &m_RespawnPos;
}

Obj_Human *Obj_Pet::GetCreator( VOID )const
{
	return m_pCreator;
}

const Obj_Human *Obj_Pet::GetConstCreator( VOID )const
{
	return m_pCreator;
}

GUID_t Obj_Pet::GetGUID( VOID )const
{
	return m_GUID;
}

VOID Obj_Pet::SetPetGUID(PET_GUID_t guid)
{
	m_guidPet = guid;
}

PET_GUID_t Obj_Pet::GetPetGUID(VOID)const
{
	return m_PetDB.m_GUID;
}

ObjID_t Obj_Pet::GetOwnerID(VOID)const
{
	return m_OwnerID;
}

//////////////////////////////////////////////////////////////
// 属性访问
//////////////////////////////////////////////////////////////
VOID Obj_Pet::SetDataID(const UINT dataID)
{
	m_iDataID = dataID;
}

UINT Obj_Pet::GetDataID( VOID )const
{
	return m_iDataID;
}

VOID Obj_Pet::SetName( const CHAR *pszName )
{
}

const CHAR *Obj_Pet::GetName( VOID )const
{
	return m_PetDB.m_szName;
}

VOID Obj_Pet::SetTitle( const CHAR *pszTitle )
{
}

const CHAR *Obj_Pet::GetTitle( VOID )const
{
	return m_PetDB.m_szNick;
}

VOID Obj_Pet::SetHP( INT nHP )
{
	//ItemContainer* pPetContainer = GetCreator()->GetPetContain();
	//Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	//Assert(pPetItem);	

	//g_ItemOperator.SetHP(pPetContainer, m_nIndex, nHP);
	m_iHP = nHP;
}

INT Obj_Pet::GetHP( VOID )
{
	return m_iHP;//100;//
}
//
//VOID Obj_Pet::SetMP( INT nMP )
//{// 宠物没有MP
//	return;
//	/*
//	if (m_pdbPet)
//		m_pdbPet->m_nMP = nMP;
//	*/
//}
//
//INT Obj_Pet::GetMP( VOID )
//{
//	// 宠物没有MP
//	//if (m_pdbPet)
//	//{
//	//	if(m_pdbPet->m_nMP>GetMaxMP())
//	//	{
//	//		m_pdbPet->m_nMP = GetMaxMP();
//	//	}
//	//	return m_pdbPet->m_nMP;
//	//}
//	return 0;
//}
//
//VOID	Obj_Pet::SetRage( INT nRage )
//{
//}

INT Obj_Pet::GetAIType( VOID )const
{
	//if (!GetConstCreator())	
	//{
	//	return m_PetDB.m_AIType;
	//}

	//ItemContainer* pPetContainer = GetCreator()->GetPetContain();
	//Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	//Assert(pPetItem);

	//return pPetItem->GetAIType();
	return m_AIType;
}

VOID Obj_Pet::SetAIType( INT nAIType )
{
	//if (!GetConstCreator())	
	//{
	//	m_PetDB.m_AIType = nAIType;
	//	return ;
	//}

	//ItemContainer* pPetContainer = GetCreator()->GetPetContain();
	//Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	//Assert(pPetItem);

	//g_ItemOperator.SetAIType(pPetContainer, m_nIndex, nAIType);
	m_AIType = nAIType;
}

INT Obj_Pet::GetLevel( VOID )const
{
	//if (!GetConstCreator())	
	//{
	//	return m_PetDB.m_nLevel;
	//}

	//ItemContainer* pPetContainer = GetCreator()->GetPetContain();
	//Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	//Assert(pPetItem);

	//return pPetItem->GetLevel();
	return m_iLevel;
}

VOID Obj_Pet::SetLevel( INT nLevel )
{
	//if (!GetConstCreator())	
	//{
	//	if(m_PetDB.m_nLevel != nLevel)
	//	{
	//		m_PetDB.m_nLevel = nLevel;
	//		OnLevelChanged();
	//	}
	//	return ;
	//}

	//ItemContainer* pPetContainer = GetCreator()->GetPetContain();
	//Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	//Assert(pPetItem);

	//if(nLevel != pPetItem->GetLevel())
	//{
	//	g_ItemOperator.SetLevel(pPetContainer, m_nIndex, nLevel);
	//	OnLevelChanged();
	//}
	if( nLevel != m_iLevel )
	{
		OnLevelChanged();
	}
}

//INT Obj_Pet::GetLife( VOID )const
//{
//	if (!GetConstCreator())	
//	{
//		return m_PetDB.m_nLife;
//	}
//
//	ItemContainer* pPetContainer = GetCreator()->GetPetContain();
//	Item* pPetItem = pPetContainer->GetItem(m_nIndex);
//	Assert(pPetItem);
//
//	return pPetItem->GetLife();
//}

//VOID Obj_Pet::SetLife( INT nLife )
//{
//	if (!GetConstCreator())	
//	{
//		m_PetDB.m_nLife = nLife;
//		return ;
//	}
//
//	ItemContainer* pPetContainer = GetCreator()->GetPetContain();
//	Item* pPetItem = pPetContainer->GetItem(m_nIndex);
//	Assert(pPetItem);
//
//	g_ItemOperator.SetLife(pPetContainer, m_nIndex, nLife);
//}

BYTE Obj_Pet::GetGeneration( VOID )const
{
	//if (!GetConstCreator())	
	//{
	//	return m_PetDB.m_byGeneration;
	//}

	//ItemContainer* pPetContainer = GetCreator()->GetPetContain();
	//Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	//Assert(pPetItem);

	//return pPetItem->GetGeneration();
	return 0;
}

VOID Obj_Pet::SetGeneration( BYTE byGeneration )
{
	//if (!GetConstCreator())	
	//{
	//	m_PetDB.m_byGeneration = byGeneration;
	//	return ;
	//}

	//ItemContainer* pPetContainer = GetCreator()->GetPetContain();
	//Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	//Assert(pPetItem);

	//g_ItemOperator.SetGeneration(pPetContainer, m_nIndex, byGeneration);
}

VOID Obj_Pet::IncrementHappyness(INT nIncrement)
{
	//INT nHappyness = (INT)(GetHappiness()) + nIncrement;
	//if ( nHappyness < 0 )
	//	nHappyness = 0;
	//else if ( nHappyness > 100 )
	//	nHappyness = 100;

	//SetHappiness((BYTE)nHappyness);
}

BYTE Obj_Pet::GetHappiness( VOID )const
{
	//if (!GetConstCreator())	
	//{
	//	return m_PetDB.m_byHappiness;
	//}

	//ItemContainer* pPetContainer = GetCreator()->GetPetContain();
	//Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	//Assert(pPetItem);

	//return pPetItem->GetHappiness();
	return 100;
}

VOID Obj_Pet::SetHappiness( BYTE byHappiness )
{
	//
	//if (!GetConstCreator())	
	//{
	//	m_PetDB.m_byHappiness = byHappiness;
	//	return ;
	//}

	//ItemContainer* pPetContainer = GetCreator()->GetPetContain();
	//Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	//Assert(pPetItem);

	//g_ItemOperator.SetHappiness(pPetContainer, m_nIndex, byHappiness);

}

INT Obj_Pet::GetGenGu( VOID )const
{
	//if (!GetConstCreator())	
	//{
	//	return m_PetDB.m_nGenGu;
	//}

	//ItemContainer* pPetContainer = GetCreator()->GetPetContain();
	//Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	//Assert(pPetItem);

	//return pPetItem->GetGenGu();
	return 1;
}

VOID Obj_Pet::SetGenGu( INT nGenGu )
{
	//if (!GetConstCreator())	
	//{
	//	m_PetDB.m_nGenGu = nGenGu;
	//	return ;
	//}

	//ItemContainer* pPetContainer = GetCreator()->GetPetContain();
	//Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	//Assert(pPetItem);

	//g_ItemOperator.SetGenGu(pPetContainer, m_nIndex, nGenGu);

}

//INT Obj_Pet::GetExp( VOID )const
//{
//	if (!GetConstCreator())	
//	{
//		return m_PetDB.m_nExp;
//	}
//
//	ItemContainer* pPetContainer = GetCreator()->GetPetContain();
//	Item* pPetItem = pPetContainer->GetItem(m_nIndex);
//	Assert(pPetItem);
//
//	return pPetItem->GetExp();
//}

//VOID Obj_Pet::SetExp( INT nExp )
//{
//	if (!GetConstCreator())	
//	{
//		m_PetDB.m_nExp = nExp;
//		return ;
//	}
//
//	if (GetLevel() > (GetCreator()->GetLevel() + LEVELDELTA_GETEXP) )
//	{// 当宠物等级高于玩家等级5级时将不能获得经验
//
//		Obj_Human* pHuman = static_cast<Obj_Human*>(GetCreator());
//		if(NULL!=pHuman)
//		{
//			pHuman->SendOperateResultMsg(OR_CANNOT_GETEXP);
//		}
//        return ;
//	}
//	ItemContainer* pPetContainer = GetCreator()->GetPetContain();
//	Item* pPetItem = pPetContainer->GetItem(m_nIndex);
//	Assert(pPetItem);
//
//	g_ItemOperator.SetExp(pPetContainer, m_nIndex, nExp);
//}

INT Obj_Pet::GetStrPerception( VOID )const
{
	//if (!GetConstCreator())	
	//{
	//	return m_PetDB.m_iStrengthPerception;
	//}

	//ItemContainer* pPetContainer = GetCreator()->GetPetContain();
	//Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	//Assert(pPetItem);

	//return pPetItem->GetStrengthPer();
	return 0;
}

INT Obj_Pet::GetSmartnessPerception( VOID )const
{
	//if (!GetConstCreator())	
	//{
	//	return m_PetDB.m_iSmartnessPerception;
	//}


	//ItemContainer* pPetContainer = GetCreator()->GetPetContain();
	//Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	//Assert(pPetItem);

	//return pPetItem->GetSmartnessPer();
	return 0;
}

INT Obj_Pet::GetMindPerception( VOID )const
{
	//if (!GetConstCreator())	
	//{
	//	return m_PetDB.m_iMindPerception;
	//}

	//ItemContainer* pPetContainer = GetCreator()->GetPetContain();
	//Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	//Assert(pPetItem);

	//return pPetItem->GetMindPer();
	return 0;
}

INT Obj_Pet::GetConstitutionPerception( VOID )const
{
	//if (!GetConstCreator())	
	//{
	//	return m_PetDB.m_iConstitutionPerception;
	//}

	//ItemContainer* pPetContainer = GetCreator()->GetPetContain();
	//Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	//Assert(pPetItem);

	//return pPetItem->GetConstitutionPer();
	return 0;
}

INT Obj_Pet::GetRemainPoint( VOID )const
{
	//if (!GetConstCreator())	
	//{
	//	return m_PetDB.m_nRemainPoint;
	//}

	//ItemContainer* pPetContainer = GetCreator()->GetPetContain();
	//Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	//Assert(pPetItem);

	//return pPetItem->GetRemainPoint();
	return 0;
}

VOID Obj_Pet::SetRemainPoint( INT nRemainPoint )
{
	//if (!GetConstCreator())	
	//{
	//	m_PetDB.m_nRemainPoint = nRemainPoint;
	//	return ;
	//}

 // 	ItemContainer* pPetContainer = GetCreator()->GetPetContain();
	//Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	//Assert(pPetItem);

	//g_ItemOperator.SetRemainPoint(pPetContainer, m_nIndex, nRemainPoint);

}


#endif // __OBJ_PET_INL__
