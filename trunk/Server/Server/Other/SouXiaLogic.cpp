#include "stdafx.h"
#include "SouXiaLogic.h"

// �������ҳ
SHORT SouXiaLogic::DismantleSouXiaSkill(BYTE rule, SOUXIA_DATA& dismantleSouXiaData, SOUXIA_DATA* souXiaData, BYTE& newPages, const BYTE curSouXiaCount)
{
	BYTE nPages[MAX_SKILL_COUNT/SKILL_PER_PAGE] = {0};
	BYTE index = 0;//��ǰ�ļ�������

	BYTE curPage = dismantleSouXiaData.GetCurSkillPage();// ȡ�õ�ǰ�ܵ�ҳ��
	Assert(curPage <= MAX_SKILL_COUNT/SKILL_PER_PAGE);

	SouXia::MyRandNumbers(nPages ,curPage);

	switch(rule)
	{
	case 0://������
		newPages = 0;
		break;

	case 1://���1ҳ
		newPages = 1;
		//�Ƿ���Բ���ɹ����ж� ��ҵ�ǰ����¼����+ �����ҳ(����) > 32 ����
		if(curSouXiaCount + newPages-1 > MAX_SOUXIA_CONTAINER)
		{
			Assert(FALSE);
			return BAG_OUT_OF_SPACE;
		}
		Assert(nPages[0] >= 0);
		for (int i=0; i<SKILL_PER_PAGE; ++i)//ÿҳ���3��
		{
			index = nPages[0]*SKILL_PER_PAGE+i;
			if (dismantleSouXiaData.m_Skill.StudySkillId[index] > 0)
			{
				souXiaData[0].m_Skill.StudySkillId[i] = dismantleSouXiaData.m_Skill.StudySkillId[index];
				souXiaData[0].m_Skill.StudyCount++;
			}
		}
		break;
	case 2://���2ҳ
		newPages = 2;
		if (curPage  < newPages)
		{
			newPages = curPage;
		}
		if(curSouXiaCount + newPages-1 > MAX_SOUXIA_CONTAINER)
		{
			Assert(FALSE);
			return BAG_OUT_OF_SPACE;
		}
		Assert(nPages[0] >= 0);
		Assert(nPages[1] >= 0);

		for (int i=0; i<newPages; ++i)// ѭ����ҳ
		{
			for (int j=0; j<SKILL_PER_PAGE; ++j)
			{
				index = nPages[i]*SKILL_PER_PAGE+j;
				if (dismantleSouXiaData.m_Skill.StudySkillId[index] > 0)
				{
					souXiaData[i].m_Skill.StudySkillId[j] = dismantleSouXiaData.m_Skill.StudySkillId[index];
					souXiaData[i].m_Skill.StudyCount++;
				}
			}
		}
		break;
	case 3://���3ҳ
		newPages = 3;
		if (curPage  < newPages)
		{
			newPages = curPage;
		}
		if(curSouXiaCount + newPages-1 > MAX_SOUXIA_CONTAINER)
		{
			Assert(FALSE);
			return BAG_OUT_OF_SPACE;
		}
		Assert(nPages[0] >= 0);
		Assert(nPages[1] >= 0);
		Assert(nPages[2] >= 0);

		for (int i=0; i<newPages; ++i) // ѭ����ҳ
		{
			for (int j=0; j<SKILL_PER_PAGE; ++j)//ÿҳ���3��
			{
				index = nPages[i]*SKILL_PER_PAGE+j;
				if (dismantleSouXiaData.m_Skill.StudySkillId[index] > 0)
				{
					souXiaData[i].m_Skill.StudySkillId[j] = dismantleSouXiaData.m_Skill.StudySkillId[index];
					souXiaData[i].m_Skill.StudyCount++;
				}
			}
		}
		break;
	case 4://ȫ������//ȫ��ĳһ�����͵ĵ���,�ɿͻ���ָ��,Ȼ�󱣴�relicType
		newPages = (BYTE)( ((dismantleSouXiaData.m_Skill.StudyCount + SKILL_PER_PAGE)-1)/SKILL_PER_PAGE);
		if(curSouXiaCount + newPages-1 > MAX_SOUXIA_CONTAINER)
		{
			Assert(FALSE);
			return BAG_OUT_OF_SPACE;
		}
		for (int i=0; i<newPages; ++i) // ѭ����ҳ
		{
			for (int j=0; j<SKILL_PER_PAGE; ++j)//ÿҳ���3��
			{
				index = i*SKILL_PER_PAGE+j;
				if (dismantleSouXiaData.m_Skill.StudySkillId[index] > 0)
				{
					souXiaData[i].m_Skill.StudySkillId[j] = dismantleSouXiaData.m_Skill.StudySkillId[index];
					souXiaData[i].m_Skill.StudyCount++;
				}
			}
		}
		break;
	default:
		Assert(FALSE);
		break;
	}
	return SOUXIA_DISMANTLE_SUCCESS;
}
// ֻ�ܲ���䷽ҳ
SHORT SouXiaLogic::DismantleSouXiaProduct(BYTE rule,  SOUXIA_DATA& dismantleSouXiaData, SOUXIA_DATA* souXiaData, BYTE& newPages, const BYTE curSouXiaCount)
{
	BYTE nPages[MAX_PRODUCT_COUNT/PRODUCT_PER_PAGE] = {0};
	BYTE index = 0;//��ǰ�ļ�������

	BYTE curPage = dismantleSouXiaData.GetCurProductPage();// ȡ�õ�ǰ�ܵ�ҳ��
	Assert(curPage <= MAX_PRODUCT_COUNT/PRODUCT_PER_PAGE);

	SouXia::MyRandNumbers(nPages ,curPage);

	switch(rule)
	{
	case 0://������
		break;

	case 1://���1ҳ
		newPages = 1;
		//�Ƿ���Բ���ɹ����ж� ��ҵ�ǰ����¼����+ �����ҳ(����) > 32 ����
		if(curSouXiaCount + newPages-1 > MAX_SOUXIA_CONTAINER)
		{
			Assert(FALSE);
			return BAG_OUT_OF_SPACE;
		}
		Assert(nPages[0] >= 0);
		for (int i=0; i<PRODUCT_PER_PAGE; ++i)//ÿҳ���1��
		{
			index = nPages[0]*PRODUCT_PER_PAGE+i;
			if (dismantleSouXiaData.m_Product.StudyProductId[index] > 0)
			{
				souXiaData[0].m_Product.StudyProductId[i] = dismantleSouXiaData.m_Product.StudyProductId[index];
				souXiaData[0].m_Product.StudyCount++;
			}
		}
		break;
	case 2://���2ҳ
		newPages = 2;
		if (curPage  < newPages)
		{
			newPages = curPage;
		}
		//�Ƿ���Բ���ɹ����ж� ��ҵ�ǰ����¼����+ �����ҳ(����) > 32 ����
		if(curSouXiaCount + newPages-1 > MAX_SOUXIA_CONTAINER)
		{
			Assert(FALSE);
			return BAG_OUT_OF_SPACE;
		}
		Assert(nPages[0] >= 0);
		Assert(nPages[1] >= 0);

		for (int i=0; i<newPages; ++i)// ѭ����ҳ
		{
			for (int j=0; j<PRODUCT_PER_PAGE; ++j)
			{
				index = nPages[i]*PRODUCT_PER_PAGE+j;//���ڱ����(�ɵ�)����¼�е�����
				if (dismantleSouXiaData.m_Product.StudyProductId[index] > 0)//�����ɲ�ҳ������(�µ�)
				{
					souXiaData[i].m_Product.StudyProductId[j] = dismantleSouXiaData.m_Product.StudyProductId[index];
					souXiaData[i].m_Product.StudyCount++;
				}
			}
		}
		break;
	case 3://���3ҳ
		newPages = 3;
		if (curPage  < newPages)
		{
			newPages = curPage;
		}
		//�Ƿ���Բ���ɹ����ж� ��ҵ�ǰ����¼����+ �����ҳ(����) > 32 ����
		if(curSouXiaCount + newPages-1 > MAX_SOUXIA_CONTAINER)
		{
			Assert(FALSE);
			return BAG_OUT_OF_SPACE;
		}
		Assert(nPages[0] >= 0);
		Assert(nPages[1] >= 0);
		Assert(nPages[2] >= 0);

		for (int i=0; i<newPages; ++i) // ѭ����ҳ
		{
			for (int j=0; j<PRODUCT_PER_PAGE; ++j)//ÿҳ���1��
			{
				index = nPages[i]*PRODUCT_PER_PAGE+j;
				if ( dismantleSouXiaData.m_Product.StudyProductId[index] > 0)
				{
					souXiaData[i].m_Product.StudyProductId[j] = dismantleSouXiaData.m_Product.StudyProductId[index];
					souXiaData[i].m_Product.StudyCount++;
				}
			}
		}
		break;
	case 4://ȫ������//ȫ��ĳһ�����͵ĵ���,�ɿͻ���ָ��,Ȼ�󱣴�relicType
		newPages = (BYTE)( ((dismantleSouXiaData.m_Product.StudyCount + PRODUCT_PER_PAGE)-1)/PRODUCT_PER_PAGE);
		//�Ƿ���Բ���ɹ����ж� ��ҵ�ǰ����¼����+ �����ҳ(����) > 32 ����
		if(curSouXiaCount + newPages-1 > MAX_SOUXIA_CONTAINER)
		{
			Assert(FALSE);
			return BAG_OUT_OF_SPACE;
		}

		for (int i=0; i<newPages; ++i) // ѭ����ҳ
		{
			for (int j=0; j<PRODUCT_PER_PAGE; ++j)//ÿҳ���1��
			{
				index = i*PRODUCT_PER_PAGE+j;
				if ( dismantleSouXiaData.m_Product.StudyProductId[index] > 0)
				{
					souXiaData[i].m_Product.StudyProductId[j] = dismantleSouXiaData.m_Product.StudyProductId[index];
					souXiaData[i].m_Product.StudyCount++;
				}
			}
		}
		break;
	default:
		Assert(FALSE);
		break;
	}
	return SOUXIA_DISMANTLE_SUCCESS;
}

SHORT SouXiaLogic::DismantleSouXiaPet(BYTE rule,  SOUXIA_DATA& dismantleSouXiaData, SOUXIA_DATA* souXiaData, BYTE& newPages, const BYTE curSouXiaCount)
{
	BYTE nPages[MAX_PET_ZHAOHUAN_COUNT/PET_ZHAOHUAN_PER_PAGE] = {0};
	BYTE index = 0;//��ǰ�ļ�������

	BYTE curPage = dismantleSouXiaData.GetCurPetZhaoHuanPage();// ȡ�õ�ǰ�ܵ�ҳ��
	Assert(curPage <= MAX_PET_ZHAOHUAN_COUNT/PET_ZHAOHUAN_PER_PAGE);

	SouXia::MyRandNumbers(nPages ,curPage);

	switch(rule)
	{
	case 0://������
		break;

	case 1://���1ҳ
		newPages = 1;
		//�Ƿ���Բ���ɹ����ж� ��ҵ�ǰ����¼����+ �����ҳ(����) > 32 ����
		if(curSouXiaCount + newPages-1 > MAX_SOUXIA_CONTAINER)
		{
			Assert(FALSE);
			return BAG_OUT_OF_SPACE;
		}

		Assert(nPages[0] >= 0);
		for (int i=0; i<PET_ZHAOHUAN_PER_PAGE; ++i)//ÿҳ���8��
		{
			index = nPages[0]*PET_ZHAOHUAN_PER_PAGE+i;
			if (dismantleSouXiaData.m_Pet.StudyPet[i].StudyZhaoHuan > 0)
			{
				souXiaData[0].m_Pet.StudyPet[i].StudyZhaoHuan = dismantleSouXiaData.m_Pet.StudyPet[i].StudyZhaoHuan;
				souXiaData[0].m_Pet.StudyPet[i].LeftUseTime = dismantleSouXiaData.m_Pet.StudyPet[i].LeftUseTime;
				souXiaData[0].m_Pet.StudyCount++;
			}
		}
		break;
	case 2://���2ҳ
		newPages = 2;
		if (curPage  < newPages)
		{
			newPages = curPage;
		}
		//�Ƿ���Բ���ɹ����ж� ��ҵ�ǰ����¼����+ �����ҳ(����) > 32 ����
		if(curSouXiaCount + newPages-1 > MAX_SOUXIA_CONTAINER)
		{
			Assert(FALSE);
			return BAG_OUT_OF_SPACE;
		}

		Assert(nPages[0] >= 0);
		Assert(nPages[1] >= 0);

		for (int i=0; i<newPages; ++i)// ѭ����ҳ
		{
			for (int j=0; j<PET_ZHAOHUAN_PER_PAGE; ++j)
			{
				index = nPages[i]*PET_ZHAOHUAN_PER_PAGE+j;
				if (dismantleSouXiaData.m_Pet.StudyPet[index].StudyZhaoHuan > 0)
				{
					souXiaData[i].m_Pet.StudyPet[j].StudyZhaoHuan = dismantleSouXiaData.m_Pet.StudyPet[index].StudyZhaoHuan;
					souXiaData[i].m_Pet.StudyPet[j].LeftUseTime = dismantleSouXiaData.m_Pet.StudyPet[index].LeftUseTime;
					souXiaData[i].m_Pet.StudyCount++;
				}
			}
		}
		break;
	case 3://���3ҳ
		newPages = 3;
		if (curPage  < newPages)
		{
			newPages = curPage;
		}

		//�Ƿ���Բ���ɹ����ж� ��ҵ�ǰ����¼����+ �����ҳ(����) > 32 ����
		if(curSouXiaCount + newPages-1 > MAX_SOUXIA_CONTAINER)
		{
			Assert(FALSE);
			return BAG_OUT_OF_SPACE;
		}

		Assert(nPages[0] >= 0);
		Assert(nPages[1] >= 0);
		Assert(nPages[2] >= 0);

		for (int i=0; i<newPages; ++i) // ѭ����ҳ
		{
			for (int j=0; j<PET_ZHAOHUAN_PER_PAGE; ++j)//ÿҳ���8��
			{
				index = nPages[i]*PET_ZHAOHUAN_PER_PAGE+j;
				if (dismantleSouXiaData.m_Pet.StudyPet[index].StudyZhaoHuan> 0)
				{
					souXiaData[i].m_Pet.StudyPet[j].StudyZhaoHuan = dismantleSouXiaData.m_Pet.StudyPet[index].StudyZhaoHuan;
					souXiaData[i].m_Pet.StudyPet[j].LeftUseTime = dismantleSouXiaData.m_Pet.StudyPet[index].LeftUseTime;
					souXiaData[i].m_Pet.StudyCount++;
				}
			}
		}
		break;
	case 4://ȫ������//ȫ��ĳһ�����͵ĵ���,�ɿͻ���ָ��,Ȼ�󱣴�relicType
		newPages = (BYTE)( ((dismantleSouXiaData.m_Pet.StudyCount + PET_ZHAOHUAN_PER_PAGE)-1)/PET_ZHAOHUAN_PER_PAGE);
		//�Ƿ���Բ���ɹ����ж� ��ҵ�ǰ����¼����+ �����ҳ(����) > 32 ����
		if(curSouXiaCount + newPages-1 > MAX_SOUXIA_CONTAINER)
		{
			Assert(FALSE);
			return BAG_OUT_OF_SPACE;
		}
		for (int i=0; i<newPages; ++i) // ѭ����ҳ
		{
			for (int j=0; j<PET_ZHAOHUAN_PER_PAGE; ++j)//ÿҳ���8��
			{
				index = i*PET_ZHAOHUAN_PER_PAGE+j;
				if (dismantleSouXiaData.m_Pet.StudyPet[index].StudyZhaoHuan > 0)
				{
					souXiaData[i].m_Pet.StudyPet[j].StudyZhaoHuan = dismantleSouXiaData.m_Pet.StudyPet[index].StudyZhaoHuan;
					souXiaData[i].m_Pet.StudyPet[j].LeftUseTime = dismantleSouXiaData.m_Pet.StudyPet[index].LeftUseTime;
					souXiaData[i].m_Pet.StudyCount++;
				}
			}
		}
		break;
	default:
		Assert(FALSE);
		break;
	}
	return SOUXIA_DISMANTLE_SUCCESS;
}

SHORT SouXiaLogic::DismantleSouXiaZuoJi(BYTE rule, SOUXIA_DATA& dismantleSouXiaData, SOUXIA_DATA* souXiaData, BYTE& newPages, const BYTE curSouXiaCount)
{
	BYTE nPages[MAX_ZUOJI_ZHAOHUAN_COUNT/ZUOJI_ZHAOHUAN_PER_PAGE] = {0};
	BYTE index = 0;//��ǰ�ļ�������

	BYTE curPage = dismantleSouXiaData.GetCurZuoJiZhaoHuanPage();// ȡ�õ�ǰ�ܵ�ҳ��
	Assert(curPage <= MAX_ZUOJI_ZHAOHUAN_COUNT/ZUOJI_ZHAOHUAN_PER_PAGE);

	SouXia::MyRandNumbers(nPages ,curPage);

	switch(rule)
	{
	case 0://������
		break;

	case 1://���1ҳ
		newPages = 1;
		//�Ƿ���Բ���ɹ����ж� ��ҵ�ǰ����¼����+ �����ҳ(����) > 32 ����
		if(curSouXiaCount + newPages-1 > MAX_SOUXIA_CONTAINER)
		{
			Assert(FALSE);
			return BAG_OUT_OF_SPACE;
		}

		Assert(nPages[0] >= 0);
		for (int i=0; i<ZUOJI_ZHAOHUAN_PER_PAGE; ++i)//ÿҳ���8��
		{
			index = nPages[0]*ZUOJI_ZHAOHUAN_PER_PAGE+i;
			if (dismantleSouXiaData.m_ZuoJi.StudyZuoji[i].StudyZhaoHuan > 0)
			{
				souXiaData[0].m_ZuoJi.StudyZuoji[i].StudyZhaoHuan = dismantleSouXiaData.m_ZuoJi.StudyZuoji[i].StudyZhaoHuan;
				souXiaData[0].m_ZuoJi.StudyZuoji[i].LeftUseTime = dismantleSouXiaData.m_ZuoJi.StudyZuoji[i].LeftUseTime;
				souXiaData[0].m_ZuoJi.StudyCount++;
			}
		}
		break;
	case 2://���2ҳ
		newPages = 2;
		if (curPage  < newPages)
		{
			newPages = curPage;
		}
		//�Ƿ���Բ���ɹ����ж� ��ҵ�ǰ����¼����+ �����ҳ(����) > 32 ����
		if(curSouXiaCount + newPages-1 > MAX_SOUXIA_CONTAINER)
		{
			Assert(FALSE);
			return BAG_OUT_OF_SPACE;
		}

		Assert(nPages[0] >= 0);
		Assert(nPages[1] >= 0);

		for (int i=0; i<newPages; ++i)// ѭ����ҳ
		{
			for (int j=0; j<ZUOJI_ZHAOHUAN_PER_PAGE; ++j)
			{
				index = nPages[i]*ZUOJI_ZHAOHUAN_PER_PAGE+j;
				if (dismantleSouXiaData.m_ZuoJi.StudyZuoji[index].StudyZhaoHuan > 0)
				{
					souXiaData[i].m_ZuoJi.StudyZuoji[j].StudyZhaoHuan = dismantleSouXiaData.m_ZuoJi.StudyZuoji[index].StudyZhaoHuan;
					souXiaData[i].m_ZuoJi.StudyZuoji[j].LeftUseTime = dismantleSouXiaData.m_ZuoJi.StudyZuoji[index].LeftUseTime;
					souXiaData[i].m_ZuoJi.StudyCount++;
				}
			}
		}
		break;
	case 3://���3ҳ
		newPages = 3;
		if (curPage  < newPages)
		{
			newPages = curPage;
		}
		//�Ƿ���Բ���ɹ����ж� ��ҵ�ǰ����¼����+ �����ҳ(����) > 32 ����
		if(curSouXiaCount + newPages-1 > MAX_SOUXIA_CONTAINER)
		{
			Assert(FALSE);
			return BAG_OUT_OF_SPACE;
		}

		Assert(nPages[0] >= 0);
		Assert(nPages[1] >= 0);
		Assert(nPages[2] >= 0);

		for (int i=0; i<newPages; ++i) // ѭ����ҳ
		{
			for (int j=0; j<ZUOJI_ZHAOHUAN_PER_PAGE; ++j)//ÿҳ���8��
			{
				index = nPages[i]*ZUOJI_ZHAOHUAN_PER_PAGE+j;
				if (dismantleSouXiaData.m_ZuoJi.StudyZuoji[index].StudyZhaoHuan> 0)
				{
					souXiaData[i].m_ZuoJi.StudyZuoji[j].StudyZhaoHuan = dismantleSouXiaData.m_ZuoJi.StudyZuoji[index].StudyZhaoHuan;
					souXiaData[i].m_ZuoJi.StudyZuoji[j].LeftUseTime = dismantleSouXiaData.m_ZuoJi.StudyZuoji[index].LeftUseTime;
					souXiaData[i].m_ZuoJi.StudyCount++;
				}
			}
		}
		break;
	case 4://ȫ������//ȫ��ĳһ�����͵ĵ���,�ɿͻ���ָ��,Ȼ�󱣴�relicType
		newPages = (BYTE)( ((dismantleSouXiaData.m_ZuoJi.StudyCount + PET_ZHAOHUAN_PER_PAGE)-1)/PET_ZHAOHUAN_PER_PAGE);
		//�Ƿ���Բ���ɹ����ж� ��ҵ�ǰ����¼����+ �����ҳ(����) > 32 ����
		if(curSouXiaCount + newPages-1 > MAX_SOUXIA_CONTAINER)
		{
			Assert(FALSE);
			return BAG_OUT_OF_SPACE;
		}
		for (int i=0; i<newPages; ++i) // ѭ����ҳ
		{
			for (int j=0; j<ZUOJI_ZHAOHUAN_PER_PAGE; ++j)//ÿҳ���8��
			{
				index = i*ZUOJI_ZHAOHUAN_PER_PAGE+j;
				if (dismantleSouXiaData.m_ZuoJi.StudyZuoji[index].StudyZhaoHuan > 0)
				{
					souXiaData[i].m_ZuoJi.StudyZuoji[j].StudyZhaoHuan = dismantleSouXiaData.m_ZuoJi.StudyZuoji[index].StudyZhaoHuan;
					souXiaData[i].m_ZuoJi.StudyZuoji[j].LeftUseTime = dismantleSouXiaData.m_ZuoJi.StudyZuoji[index].LeftUseTime;
					souXiaData[i].m_ZuoJi.StudyCount++;
				}
			}
		}
		break;
	default:
		Assert(FALSE);
		break;
	}
	return SOUXIA_DISMANTLE_SUCCESS;
}

//����
BOOL SouXiaLogic::RefiningSouXia(Obj_Human* pHuman, Item* pSouXiaItem1, Item* pSouXiaItem2, BYTE refiningRule, SOUXIA_DATA& randSouXiaData, const SOUXIA_PAGE_TYPE& type)
{
	__ENTER_FUNCTION
	
	Assert(pHuman);
	Assert(pSouXiaItem1);
	Assert(pSouXiaItem2);
	Assert(refiningRule >= 0);

	//��ȡ��һ������¼������
	ItemParamValue ipv = (ItemParamValue)IPV_INT;
	INT	iParamSouXiaID = pSouXiaItem1->GetItemParam(0,ipv);
	INT	iParamSouXiaPos = pSouXiaItem1->GetItemParam(4,ipv);

	SOUXIA_DATA souxiadata,souxiadata2;
	if (iParamSouXiaID > 0 && iParamSouXiaPos >= 0)
	{
		souxiadata = pHuman->GetSouXia(iParamSouXiaPos).GetSouXiaData();
	}

	//��ȡ�ڶ�������¼������
	INT	iParamSouXiaID2 = pSouXiaItem2->GetItemParam(0,ipv);
	INT	iParamSouXiaPos2 = pSouXiaItem2->GetItemParam(4,ipv);

	if (iParamSouXiaID2 > 0 && iParamSouXiaPos2 >= 0)
	{
		souxiadata2 = pHuman->GetSouXia(iParamSouXiaPos2).GetSouXiaData();	
	}

	// ��һ����ҳ��
	BYTE maxPage = 0, maxPage2 = 0;
	SOUXIA_INFO_TB* pGet1 = g_ItemTable.GetSouXiaTB(pSouXiaItem1->GetItemTableIndex());
	Assert(pGet1);
	if (!pGet1)
	{
		return FALSE;
	}
	// �ڶ���ҳ��
	SOUXIA_INFO_TB* pGet2 = g_ItemTable.GetSouXiaTB(pSouXiaItem2->GetItemTableIndex());
	Assert(pGet2);
	if (!pGet2)
	{
		return FALSE;
	}

	BYTE nPages = 0;
	//����������¼��ѧϰ�������ݺϳ�Ϊһ�������--�ڼ���
	switch (type)
	{
	case ITEM_SOUXIASKILL:
		maxPage = pGet1->m_nMaxPages[1];
		maxPage2 = pGet2->m_nMaxPages[1];
		if (maxPage >= maxPage2)//����������ҳ
		{
			nPages = maxPage;
		} 
		else
		{
			nPages = maxPage2;
		}
		RefiningSouXiaSkill(refiningRule, randSouXiaData, souxiadata, souxiadata2, nPages) ;//randSouXiaData ���������¼����ҳ
		break;
	case ITEM_PRODUCT:
		maxPage = pGet1->m_nMaxPages[2];
		maxPage2 = pGet2->m_nMaxPages[2];
		if (maxPage >= maxPage2)
		{
			nPages = maxPage;
		} 
		else
		{
			nPages = maxPage2;
		}
		RefiningSouXiaProduct(refiningRule, randSouXiaData, souxiadata, souxiadata2, nPages);
		break;
	case ITEM_PET_ZHAOHUAN:
		maxPage = pGet1->m_nMaxPages[3];
		maxPage2 = pGet2->m_nMaxPages[3];
		if (maxPage >= maxPage2)
		{
			nPages = maxPage;
		} 
		else
		{
			nPages = maxPage2;
		}
		RefiningSouXiaPet(refiningRule, randSouXiaData, souxiadata, souxiadata2, nPages);
		break;
	case ITEM_ZUOJI_ZHAOHUAN:
		maxPage = pGet1->m_nMaxPages[4];
		maxPage2 = pGet2->m_nMaxPages[4];
		if (maxPage >= maxPage2)
		{
			nPages = maxPage;
		} 
		else
		{
			nPages = maxPage2;
		}
		RefiningSouXiaZuoJi(refiningRule, randSouXiaData, souxiadata, souxiadata2, nPages);
		break;
	default:
		break;
	}
	return TRUE;
	
	__LEAVE_FUNCTION
	
	return FALSE;
}
 // ��������¼����
SHORT SouXiaLogic::RefiningSouXiaSkill(BYTE rule, SOUXIA_DATA& randSouXiaData, SOUXIA_DATA& souXiaData, SOUXIA_DATA& souXiaData2, BYTE maxPages)
{
	UINT skillArray[MAX_SKILL_COUNT*2] = {0};//��������¼���ܰ������ܵ����ҳ
	BYTE skillCount = 0;
	for (int i=0; i<souXiaData.GetCurSkillCount(); ++i)
	{
		if (souXiaData.m_Skill.StudySkillId[i] > 0)
		{
			skillArray[skillCount] = souXiaData.m_Skill.StudySkillId[i];
			skillCount++;
		}
	}

	BYTE curPage = (BYTE)(((skillCount+SKILL_PER_PAGE)-1)/SKILL_PER_PAGE);// ȡ�õ�ǰ�ܵ�ҳ������������¼�Ŀո�
	skillCount = curPage*SKILL_PER_PAGE;

	for (int i=0; i<souXiaData2.GetCurSkillCount(); ++i)
	{
		if (souXiaData2.m_Skill.StudySkillId[i] > 0)
		{
			skillArray[skillCount] = souXiaData2.m_Skill.StudySkillId[i];
			skillCount++;
		}
	}

	curPage = (BYTE)(((skillCount+SKILL_PER_PAGE)-1)/SKILL_PER_PAGE);// ȡ�õ�ǰ�µ���ҳ��
	
	BYTE nPages[MAX_SKILL_COUNT*2/SKILL_PER_PAGE] = {0};
	BYTE index = 0;//��ǰ�ļ�������
	BYTE newPages =0;
	Assert(curPage <= MAX_SKILL_COUNT*2/SKILL_PER_PAGE);
	SouXia::MyRandNumbers(nPages ,curPage);

	switch(rule)
	{
	case 0://������
		newPages = 0;
		break;

	case 1://���1ҳ
		{
			int nCount = 0;
			newPages = 1;
			Assert(nPages[0] >= 0);
			for (int i=0; i<SKILL_PER_PAGE; ++i)//ÿҳ���3��
			{
				index = nPages[0]*SKILL_PER_PAGE+i;
				if (skillArray[index] > 0)
				{
					randSouXiaData.m_Skill.StudySkillId[nCount] = skillArray[index];
					randSouXiaData.m_Skill.StudyCount++;
					nCount++;
				}
			}
		}
		break;
	case 2://���2ҳ
		{
			int nCount = 0;
			newPages = 2;
			if (curPage  < newPages)
			{
				newPages = curPage;
			}
			Assert(nPages[0] >= 0);
			Assert(nPages[1] >= 0);

			for (int i=0; i<newPages; ++i)// ѭ����ҳ
			{
				for (int j=0; j<SKILL_PER_PAGE; ++j)
				{
					index = nPages[i]*SKILL_PER_PAGE+j;
					if (skillArray[index] > 0)
					{
						randSouXiaData.m_Skill.StudySkillId[nCount] = skillArray[index];
						randSouXiaData.m_Skill.StudyCount++;
						nCount++;
					}
				}
			}
		}
		break;
	case 3://���3ҳ
		{
			int nCount = 0;
			newPages = 3;
			if (curPage  < newPages)
			{
				newPages = curPage;
			}
			Assert(nPages[0] >= 0);
			Assert(nPages[1] >= 0);
			Assert(nPages[2] >= 0);

			for (int i=0; i<newPages; ++i) // ѭ����ҳ
			{
				for (int j=0; j<SKILL_PER_PAGE; ++j)//ÿҳ���3��
				{
					index = nPages[i]*SKILL_PER_PAGE+j;
					if (skillArray[index] > 0)
					{
						randSouXiaData.m_Skill.StudySkillId[nCount] = skillArray[index];
						randSouXiaData.m_Skill.StudyCount++;
						nCount++;
					}
				}
			}
		}
		break;
	case 4://ȫ������
		{
			int nCount = 0;
			newPages = (BYTE)( ((skillCount + SKILL_PER_PAGE)-1)/SKILL_PER_PAGE);
			if (newPages > maxPages)
			{
				newPages = maxPages;
			}
			for (int i=0; i<newPages; ++i) // ѭ����ҳ
			{
				for (int j=0; j<SKILL_PER_PAGE; ++j)//ÿҳ���3��
				{
					index = i*SKILL_PER_PAGE+j;
					if (skillArray[index] > 0)
					{
						randSouXiaData.m_Skill.StudySkillId[nCount] = skillArray[index];
						randSouXiaData.m_Skill.StudyCount++;
						nCount++;
					}
				}
			}
		}
		break;
	default:
		Assert(FALSE);
		break;
	}
	return SOUXIA_REFINING_SUCCESS;
}
// �����䷽
SHORT SouXiaLogic::RefiningSouXiaProduct(BYTE rule, SOUXIA_DATA& randSouXiaData, SOUXIA_DATA& souXiaData, SOUXIA_DATA& souXiaData2, BYTE maxPages)
{
	UINT productArray[MAX_PRODUCT_COUNT*2] = {0};
	BYTE productCount = 0;

	for (int i=0; i<souXiaData.GetCurProductCount(); ++i)
	{
		if (souXiaData.m_Product.StudyProductId[i] > 0)
		{
			productArray[productCount] = souXiaData.m_Product.StudyProductId[i];
			productCount++;
		}
	}
	BYTE curPage = (BYTE)(((productCount+PRODUCT_PER_PAGE)-1)/PRODUCT_PER_PAGE);// ȡ�õ�ǰ�ܵ�ҳ������������¼�Ŀո�
	productCount = curPage*PRODUCT_PER_PAGE;

	for (int i=0; i<souXiaData2.GetCurProductCount(); ++i)
	{
		if (souXiaData2.m_Product.StudyProductId[i] > 0)
		{
			productArray[productCount] = souXiaData2.m_Product.StudyProductId[i];
			productCount++;
		}
	}

	BYTE nPages[MAX_PRODUCT_COUNT*2/PRODUCT_PER_PAGE] = {0};
	BYTE index = 0;//��ǰ���䷽����
	BYTE newPages =0;

	curPage = (BYTE)(((productCount+PRODUCT_PER_PAGE)-1)/PRODUCT_PER_PAGE);// ȡ�õ�ǰ�ܵ�ҳ��
	Assert(curPage <= MAX_PRODUCT_COUNT*2/PRODUCT_PER_PAGE);
	SouXia::MyRandNumbers(nPages, curPage);

	switch(rule)
	{
	case 0://������
		newPages = 0;
		break;

	case 1://���1ҳ
		{
			int nCount = 0;
			newPages = 1;
			Assert(nPages[0] >= 0);
			for (int i=0; i<PRODUCT_PER_PAGE; ++i)//ÿҳ���
			{
				index = nPages[0]*PRODUCT_PER_PAGE+i;
				if (productArray[index] > 0)
				{
					randSouXiaData.m_Product.StudyProductId[nCount] = productArray[index];
					randSouXiaData.m_Product.StudyCount++;
					nCount++;
				}
			}
		}
		break;
	case 2://���2ҳ
		{
			int nCount = 0;
			newPages = 2;
			if (curPage  < newPages)
			{
				newPages = curPage;
			}
			Assert(nPages[0] >= 0);
			Assert(nPages[1] >= 0);

			for (int i=0; i<newPages; ++i)// ѭ����ҳ
			{
				for (int j=0; j<PRODUCT_PER_PAGE; ++j)
				{
					index = nPages[i]*PRODUCT_PER_PAGE+j;
					if (productArray[index] > 0)
					{
						souXiaData.m_Product.StudyProductId[nCount] = productArray[index];
						souXiaData.m_Product.StudyCount++;
						nCount++;
					}
				}
			}
		}
		break;
	case 3://���3ҳ
		{
			int nCount = 0;
			newPages = 3;
			if (curPage  < newPages)
			{
				newPages = curPage;
			}
			Assert(nPages[0] >= 0);
			Assert(nPages[1] >= 0);
			Assert(nPages[2] >= 0);

			for (int i=0; i<newPages; ++i) // ѭ����ҳ
			{
				for (int j=0; j<PRODUCT_PER_PAGE; ++j)//ÿҳ���
				{
					index = nPages[i]*PRODUCT_PER_PAGE+j;
					if (productArray[index] > 0)
					{
						randSouXiaData.m_Product.StudyProductId[nCount] = productArray[index];
						randSouXiaData.m_Product.StudyCount++;
						nCount++;
					}
				}
			}
		}
		break;
	case 4://ȫ������
		{
			int nCount = 0;
			newPages = (BYTE)( ((productCount + PRODUCT_PER_PAGE)-1)/PRODUCT_PER_PAGE);
			if (newPages > maxPages)
			{
				newPages = maxPages;
			}
			for (int i=0; i<newPages; ++i) // ѭ����ҳ
			{
				for (int j=0; j<PRODUCT_PER_PAGE; ++j)//ÿҳ���
				{
					index = i*PRODUCT_PER_PAGE+j;
					if (productArray[index]> 0)
					{
						souXiaData.m_Product.StudyProductId[nCount] = productArray[index];
						souXiaData.m_Product.StudyCount++;
						nCount++;
					}
				}
			}
		}
		break;
	default:
		Assert(FALSE);
		break;
	}
	return SOUXIA_REFINING_SUCCESS;
}
// ���������ٻ�
SHORT SouXiaLogic::RefiningSouXiaPet(BYTE rule, SOUXIA_DATA& randSouXiaData, SOUXIA_DATA& souXiaData, SOUXIA_DATA& souXiaData2, BYTE maxPages)
{
	UINT petArray[MAX_PET_ZHAOHUAN_COUNT*2][2] = {0};
	BYTE petCount = 0;
	for (int i=0; i<souXiaData.GetCurPetCount(); ++i)
	{
		if (souXiaData.m_Pet.StudyPet[i].StudyZhaoHuan > 0)
		{
			petArray[petCount][0] = souXiaData.m_Pet.StudyPet[i].StudyZhaoHuan;
			petArray[petCount][1] = souXiaData.m_Pet.StudyPet[i].LeftUseTime;
			petCount++;
		}
	}
	BYTE curPage = (BYTE)(((petCount+PET_ZHAOHUAN_PER_PAGE)-1)/PET_ZHAOHUAN_PER_PAGE);// ȡ�õ�ǰ�ܵ�ҳ������������¼�Ŀո�
	petCount = curPage*PET_ZHAOHUAN_PER_PAGE;

	for (int i=0; i<souXiaData2.GetCurPetCount(); ++i)
	{
		if (souXiaData2.m_Pet.StudyPet[i].StudyZhaoHuan > 0)
		{
			petArray[petCount][0] = souXiaData2.m_Pet.StudyPet[i].StudyZhaoHuan;
			petArray[petCount][1] = souXiaData2.m_Pet.StudyPet[i].LeftUseTime;
			petCount++;
		}
	}

	BYTE nPages[MAX_PET_ZHAOHUAN_COUNT*2/PET_ZHAOHUAN_PER_PAGE] = {0};
	BYTE index = 0;//��ǰ�ļ�������
	BYTE newPages =0;

	curPage = (BYTE)(((petCount+PET_ZHAOHUAN_PER_PAGE)-1)/PET_ZHAOHUAN_PER_PAGE);// ȡ�õ�ǰ�ܵ�ҳ��
	Assert(curPage <= MAX_PET_ZHAOHUAN_COUNT*2/PET_ZHAOHUAN_PER_PAGE);
	SouXia::MyRandNumbers(nPages ,curPage);

	switch(rule)
	{
	case 0://������
		newPages = 0;
		break;

	case 1://���1ҳ
		{
			int nCount = 0;
			newPages = 1;
			Assert(nPages[0] >= 0);
			for (int i=0; i<PET_ZHAOHUAN_PER_PAGE; ++i)//ÿҳ���8��
			{
				index = nPages[0]*PET_ZHAOHUAN_PER_PAGE+i;
				if (petArray[index][0] > 0)
				{
					randSouXiaData.m_Pet.StudyPet[nCount].StudyZhaoHuan = petArray[index][0];
					randSouXiaData.m_Pet.StudyPet[nCount].LeftUseTime = petArray[index][1];
					randSouXiaData.m_Pet.StudyCount++;
					nCount++;
				}
			}
		}
		
		break;
	case 2://���2ҳ
		{
			int nCount = 0;
			newPages = 2;
			if (curPage  < newPages)
			{
				newPages = curPage;
			}
			Assert(nPages[0] >= 0);
			Assert(nPages[1] >= 0);

			for (int i=0; i<newPages; ++i)// ѭ����ҳ
			{
				for (int j=0; j<PET_ZHAOHUAN_PER_PAGE; ++j)
				{
					index = nPages[i]*PET_ZHAOHUAN_PER_PAGE+j;
					if (petArray[index][0] > 0)
					{
						randSouXiaData.m_Pet.StudyPet[nCount].StudyZhaoHuan =petArray[index][0];
						randSouXiaData.m_Pet.StudyPet[nCount].LeftUseTime =  petArray[index][1];
						randSouXiaData.m_Pet.StudyCount++;
						nCount++;
					}
				}
			}
		}
		break;
	case 3://���3ҳ
		{
			int nCount = 0;
			newPages = 3;
			if (curPage  < newPages)
			{
				newPages = curPage;
			}
			Assert(nPages[0] >= 0);
			Assert(nPages[1] >= 0);
			Assert(nPages[2] >= 0);

			for (int i=0; i<newPages; ++i) // ѭ����ҳ
			{
				for (int j=0; j<PRODUCT_PER_PAGE; ++j)//ÿҳ���8��
				{
					index = nPages[i]*PET_ZHAOHUAN_PER_PAGE+j;
					if (petArray[index][0] > 0)
					{
						randSouXiaData.m_Pet.StudyPet[nCount].StudyZhaoHuan =petArray[index][0];
						randSouXiaData.m_Pet.StudyPet[nCount].LeftUseTime =  petArray[index][1];
						randSouXiaData.m_Pet.StudyCount++;
						nCount++;
					}
				}
			}
		}
		break;
	case 4://ȫ������
		{
			int nCount = 0;
			newPages = (BYTE)( ((petCount + PET_ZHAOHUAN_PER_PAGE)-1)/PET_ZHAOHUAN_PER_PAGE);
			if (newPages > maxPages)
			{
				newPages = maxPages;
			}
			for (int i=0; i<newPages; ++i) // ѭ����ҳ
			{
				for (int j=0; j<PET_ZHAOHUAN_PER_PAGE; ++j)//ÿҳ���8��
				{
					index = i*PET_ZHAOHUAN_PER_PAGE+j;
					if (petArray[index][0] > 0)
					{
						randSouXiaData.m_Pet.StudyPet[nCount].StudyZhaoHuan =petArray[index][0];
						randSouXiaData.m_Pet.StudyPet[nCount].LeftUseTime =  petArray[index][1];
						randSouXiaData.m_Product.StudyCount++;
						nCount++;
					}
				}
			}
		}
		break;
	default:
		Assert(FALSE);
		break;
	}
	return SOUXIA_REFINING_SUCCESS;
}

// ���������ٻ�
SHORT SouXiaLogic::RefiningSouXiaZuoJi(BYTE rule, SOUXIA_DATA& randSouXiaData, SOUXIA_DATA& souXiaData, SOUXIA_DATA& souXiaData2, BYTE maxPages)
{
	UINT zuoJiArray[MAX_ZUOJI_ZHAOHUAN_COUNT*2][2] = {0};
	BYTE zuoJiCount = 0;
	for (int i=0; i<souXiaData.GetCurZuoJiCount(); ++i)
	{
		if (souXiaData.m_ZuoJi.StudyZuoji[i].StudyZhaoHuan > 0)
		{
			zuoJiArray[zuoJiCount][0] = souXiaData.m_ZuoJi.StudyZuoji[i].StudyZhaoHuan;
			zuoJiArray[zuoJiCount][1] = souXiaData.m_ZuoJi.StudyZuoji[i].LeftUseTime;
			zuoJiCount++;
		}
	}

	BYTE curPage = (BYTE)(((zuoJiCount+ZUOJI_ZHAOHUAN_PER_PAGE)-1)/ZUOJI_ZHAOHUAN_PER_PAGE);// ȡ�õ�ǰ�ܵ�ҳ������������¼�Ŀո�
	zuoJiCount = curPage*ZUOJI_ZHAOHUAN_PER_PAGE;

	for (int i=0; i<souXiaData2.GetCurZuoJiCount(); ++i)
	{
		if (souXiaData2.m_ZuoJi.StudyZuoji[i].StudyZhaoHuan > 0)
		{
			zuoJiArray[zuoJiCount][0] = souXiaData2.m_ZuoJi.StudyZuoji[i].StudyZhaoHuan;
			zuoJiArray[zuoJiCount][1] = souXiaData2.m_ZuoJi.StudyZuoji[i].LeftUseTime;
			zuoJiCount++;
		}
	}

	BYTE nPages[MAX_ZUOJI_ZHAOHUAN_COUNT*2/ZUOJI_ZHAOHUAN_PER_PAGE] = {0};
	BYTE index = 0;//��ǰ�ļ�������
	BYTE newPages =0;

	curPage = (BYTE)(((zuoJiCount + ZUOJI_ZHAOHUAN_PER_PAGE)-1)/ZUOJI_ZHAOHUAN_PER_PAGE);// ȡ�õ�ǰ�ܵ�ҳ��
	Assert(curPage <= MAX_ZUOJI_ZHAOHUAN_COUNT*2/ZUOJI_ZHAOHUAN_PER_PAGE);
	SouXia::MyRandNumbers(nPages ,curPage);

	switch(rule)
	{
	case 0://������
		newPages = 0;
		break;

	case 1://���1ҳ
		{
			int nCount = 0;
			newPages = 1;
			Assert(nPages[0] >= 0);
			for (int i=0; i<ZUOJI_ZHAOHUAN_PER_PAGE; ++i)//ÿҳ���8��
			{
				index = nPages[0]*ZUOJI_ZHAOHUAN_PER_PAGE+i;
				if (zuoJiArray[index][0] > 0)
				{
					randSouXiaData.m_ZuoJi.StudyZuoji[nCount].StudyZhaoHuan = zuoJiArray[index][0];
					randSouXiaData.m_ZuoJi.StudyZuoji[nCount].LeftUseTime = zuoJiArray[index][1];
					randSouXiaData.m_ZuoJi.StudyCount++;
					nCount++;
				}
			}
		}
	
		break;
	case 2://���2ҳ
		{
			int nCount = 0;
			newPages = 2;
			if (curPage  < newPages)
			{
				newPages = curPage;
			}
			Assert(nPages[0] >= 0);
			Assert(nPages[1] >= 0);

			for (int i=0; i<newPages; ++i)// ѭ����ҳ
			{
				for (int j=0; j<ZUOJI_ZHAOHUAN_PER_PAGE; ++j)
				{
					index = nPages[i]*ZUOJI_ZHAOHUAN_PER_PAGE+j;
					if (zuoJiArray[index][0] > 0)
					{
						randSouXiaData.m_ZuoJi.StudyZuoji[nCount].StudyZhaoHuan =zuoJiArray[index][0];
						randSouXiaData.m_ZuoJi.StudyZuoji[nCount].LeftUseTime =  zuoJiArray[index][1];
						randSouXiaData.m_ZuoJi.StudyCount++;
						nCount++;
					}
				}
			}
		}
		
		break;
	case 3://���3ҳ
		{
			int nCount = 0;
			newPages = 3;
			if (curPage  < newPages)
			{
				newPages = curPage;
			}
			Assert(nPages[0] >= 0);
			Assert(nPages[1] >= 0);
			Assert(nPages[2] >= 0);

			for (int i=0; i<newPages; ++i) // ѭ����ҳ
			{
				for (int j=0; j<PRODUCT_PER_PAGE; ++j)//ÿҳ���8��
				{
					index = nPages[i]*ZUOJI_ZHAOHUAN_PER_PAGE+j;
					if (zuoJiArray[index][0] > 0)
					{
						randSouXiaData.m_ZuoJi.StudyZuoji[nCount].StudyZhaoHuan =zuoJiArray[index][0];
						randSouXiaData.m_ZuoJi.StudyZuoji[nCount].LeftUseTime =  zuoJiArray[index][1];
						randSouXiaData.m_ZuoJi.StudyCount++;
						nCount++;
					}
				}
			}
		}
		break;
	case 4://ȫ������
		{
			int nCount = 0;
			newPages = (BYTE)( ((zuoJiCount + ZUOJI_ZHAOHUAN_PER_PAGE)-1)/ZUOJI_ZHAOHUAN_PER_PAGE);
			if (newPages > maxPages)
			{
				newPages = maxPages;
			}
			for (int i=0; i<newPages; ++i) // ѭ����ҳ
			{
				for (int j=0; j<ZUOJI_ZHAOHUAN_PER_PAGE; ++j)//ÿҳ���8��
				{
					index = i*ZUOJI_ZHAOHUAN_PER_PAGE+j;
					if (zuoJiArray[index][0] > 0)
					{
						randSouXiaData.m_ZuoJi.StudyZuoji[nCount].StudyZhaoHuan =zuoJiArray[index][0];
						randSouXiaData.m_ZuoJi.StudyZuoji[nCount].LeftUseTime =  zuoJiArray[index][1];
						randSouXiaData.m_ZuoJi.StudyCount++;
						nCount++;
					}
				}
			}
		}
		break;
	default:
		Assert(FALSE);
		break;
	}
	return SOUXIA_REFINING_SUCCESS;
}

