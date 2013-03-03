#include "stdafx.h"
#include "SouXia.h"

SouXia::SouXia()
{
	
}

SouXia::~SouXia(void)
{
	
}

BYTE SouXia::AddSkillToSouXia(BYTE type, SkillID_t studyId, BYTE useTime, INT* m_nMaxPages)
{
	SHORT posValue = -1;
	switch (type)
	{
		case ITEM_SOUXIASKILL:
			//Assert(m_SouXiaData.GetCurSkillCount() < MAX_SKILL_COUNT);
			if (m_SouXiaData.GetCurSkillCount() >= m_nMaxPages[1]*SKILL_PER_PAGE)
			{
				return SOUXIA_SKILL_FULL;
			}
			posValue = _AddSkillToSouXia(m_SouXiaData.m_Skill.StudySkillId, m_nMaxPages[1]*SKILL_PER_PAGE, studyId);
			if(posValue >= 0)
			{
				m_SouXiaData.IncCurSkillCount();
			}
			break;

		case ITEM_PRODUCT:
			//Assert(m_SouXiaData.GetCurProductCount() < MAX_PRODUCT_COUNT);
			if (m_SouXiaData.GetCurProductCount() >= m_nMaxPages[2]*PRODUCT_PER_PAGE)
			{
				return SOUXIA_PRODUCT_FULL;
			}
			posValue = _AddSkillToSouXia(m_SouXiaData.m_Product.StudyProductId, m_nMaxPages[2]*PRODUCT_PER_PAGE, studyId);
			if(posValue >= 0)
			{
				m_SouXiaData.IncCurProductCount();
			}
			break;

		case ITEM_PET_ZHAOHUAN:
			//Assert(m_SouXiaData.GetCurPetCount() < MAX_PET_ZHAOHUAN_COUNT);
			if (m_SouXiaData.GetCurPetCount() >= m_nMaxPages[3]*PET_ZHAOHUAN_PER_PAGE)
			{
				return SOUXIA_PET_ZHAOHUAN_FULL;
			}
			posValue = _AddSkillToSouXiaZhaoHuan(m_SouXiaData.m_Pet.StudyPet, m_nMaxPages[3]*PET_ZHAOHUAN_PER_PAGE, studyId, useTime);
			if(posValue >= 0)
			{
				m_SouXiaData.IncCurPetCount();
			}
			break;

		case ITEM_ZUOJI_ZHAOHUAN:
			//Assert(m_SouXiaData.GetCurZuoJiCount() < MAX_ZUOJI_ZHAOHUAN_COUNT);
			if (m_SouXiaData.GetCurZuoJiCount() >= m_nMaxPages[4]*ZUOJI_ZHAOHUAN_PER_PAGE)
			{
				return SOUXIA_ZUOJI_ZHAOHUAN_FULL;
			}
			posValue = _AddSkillToSouXiaZhaoHuan(m_SouXiaData.m_ZuoJi.StudyZuoji, m_nMaxPages[4]*ZUOJI_ZHAOHUAN_PER_PAGE, studyId, useTime);
			if (posValue >= 0)
			{
				m_SouXiaData.IncCurZuoJiCount();
			}
			break;

		default:
			break;
	}

	if (posValue >= 0)
	{
		return SOUXIA_SKILL_UPGRADE_SUCCESS;
	} 
	return SOUXIA_SKILL_UPGRADE_FAIL;
}

SHORT SouXia::_AddSkillToSouXia(SkillID_t* skillId, BYTE count, SkillID_t studyId)
{
	SHORT result = -1;
	for (int i=0; i<count; ++i)
	{
		if(skillId[i] <= 0)
		{
			skillId[i] = studyId;
			return i;
		}
	}
	return result;
}

SHORT SouXia::_AddSkillToSouXiaZhaoHuan(ZhaoHuan* skillId, BYTE count, SkillID_t studyId, SHORT useTime)
{
	SHORT result = -1;
	for (int i=0; i<count; ++i)
	{
		if(skillId[i].StudyZhaoHuan == 0)
		{
			skillId[i].StudyZhaoHuan = studyId;
			skillId[i].LeftUseTime = useTime;
			return i;
		}
	}
	return result;
}

VOID SouXia::MyRandNumbers(BYTE* pages, BYTE curPage)
{
	Assert(curPage >= 0);
	if (0 == curPage)
	{
		return;
	}
	//初始化数组
	for (int m=0; m<curPage; ++m)
	{
		pages[m] = m;
	}
	//打乱次序,改变数组
	BYTE i=0, tmpValue=0, randNum=0; 
	srand( (unsigned int)time(NULL)); 

	for(i=0;i<curPage;++i)   
	{   
		randNum = rand()%curPage;  

		tmpValue		= pages[randNum];
		pages[randNum]	= pages[i];
		pages[i]		= tmpValue;
	}    
}

BOOL  SouXia::HasLearnedProduct(UINT index)
{
	for(BYTE i=0; i<m_SouXiaData.GetCurProductCount(); ++i)
	{
		if (m_SouXiaData.m_Product.StudyProductId[i] == index)
		{
			return TRUE;
		}
	}
	return FALSE;
}

