#include "StdAfx.h"
#include ".\hairmodelmng.h"
#include "..\dbc\GMDataBase.h"
#include "GameProcedure.h"
#include "GIException.h"

CHairModelMng::CHairModelMng(void)
{
}

CHairModelMng::~CHairModelMng(void)
{
}

// �õ����Ƿ��͸���
//
// iRace = 0 - Ů
//         1 - ��
// iIndex = ������Ϣ����
int CHairModelMng::GetHairModelCount(int iRace)
{
	switch(iRace)
	{
	case 0:
		{
			
			return GetWomanHairModelCount();
			break;
		}
	case 1:
		{
			return GetManHairModelCount();
			break;
		}
	}
	return 0;
}

	
// �õ������Ƿ��͸���
int CHairModelMng::GetManHairModelCount()
{

	// �����Ƿ��θ���
	return  m_iManHairModelCount;
	
}


// �õ�Ů���Ƿ��͸���
int CHairModelMng::GetWomanHairModelCount()
{
	// Ů���Ƿ��θ���
	return m_iWomanHairModelCount;
}


// �õ����Ƿ�����Ϣ
//
// iRace = 0 - Ů
//         1 - ��
// iIndex = ������Ϣ����
LPCSTR  CHairModelMng::GetHairModel(int iRace, int iIndex)
{
	switch(iRace)
	{
	case 0:
		{
			return GetWomanHairModel(iIndex);
			break;
		}
	case 1:
		{
			return GetManHairModel(iIndex);
			break;
		}
	}
	return NULL;
}


// �õ������Ƿ�����Ϣ
//
// iIndex = ������Ϣ����
LPCSTR  CHairModelMng::GetManHairModel(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_ManHairModelVector.size()))
	{
		return m_ManHairModelVector[iIndex].pMeshFile;
	}
	return NULL;
}


// �õ�Ů���Ƿ�����Ϣ
//
// iIndex = ������Ϣ����
LPCSTR	CHairModelMng::GetWomanHairModel(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_WomanHairModelVector.size()))
	{
		return m_WomanHairModelVector[iIndex].pMeshFile;
	}
	return NULL;
}

// �õ���ɫ������Ϣ
// 0 -- �õ���ɫ���ͳɹ�
// ���� -- ʧ��.
int CHairModelMng::GetCharacterModel()
{
	// �������
	ClearData();

	DBC_DEFINEHANDLE(s_CharacterHairModel, DBC_CHARACTER_HAIR_GEO);		// char_hair.tab
	if(NULL == s_CharacterHairModel)
	{
		return 1;
	}

	int iItemCount = s_CharacterHairModel->GetRecordsNum();

	for(int i = 0; i < iItemCount; i++)
	{
		//������¼
		const _DBC_CHAR_HAIR_GEO* pHaireInfo = (const _DBC_CHAR_HAIR_GEO*)s_CharacterHairModel->Search_LineNum_EQU((UINT)i);
		if(pHaireInfo)
		{
			switch(pHaireInfo->nRace)			
			{
			case 0:
				{
					// Ů����ͷ�����
					m_iWomanHairModelCount++;
					m_WomanHairModelVector.push_back(*pHaireInfo);
					break;
				}
			case 1:
				{
					// ������ͷ�����
					m_iManHairModelCount++;
					m_ManHairModelVector.push_back(*pHaireInfo);
					break;
				}
			default:
				{
					break;
				}
			}
		}

	}//for(int i = 0; i < iItemCount; i++)

	return 0;
}

// �������
void CHairModelMng::ClearData()
{
	// �����Ƿ��θ���
	m_iManHairModelCount   = 0;
	// Ů���Ƿ��θ���
	m_iWomanHairModelCount = 0;

	// �����Ƿ�����Ϣ����
	m_ManHairModelVector.clear();
	// Ů���Ƿ�����Ϣ����
	m_WomanHairModelVector.clear();

}


// �õ�Ů���Ƿ���Id
int CHairModelMng::GetWomanHairModelId(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_WomanHairModelVector.size()))
	{
		return m_WomanHairModelVector[iIndex].nID;
	}
	return 0;
}


// �õ������Ƿ���Id
int CHairModelMng::GetManHairModelId(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_ManHairModelVector.size()))
	{
		return m_ManHairModelVector[iIndex].nID;
	}
	return 0;
}


// �õ�����model Id
int CHairModelMng::GetHairModelId(int iRace, int iIndex)
{
	switch(iRace)
	{
	case 0:
		{
			return GetWomanHairModelId(iIndex);
			break;
		}
	case 1:
		{
			return GetManHairModelId(iIndex);
			break;
		}
	}
	return 0;
}

// �õ����Ƿ�����Ϣ����
//
// iRace = 0 - Ů
//         1 - ��
// iIndex = ������Ϣ����
LPCSTR  CHairModelMng::GetHairModelShowName(int iRace, int iIndex)
{
	switch(iRace)
	{
	case 0:
		{
			return GetWomanHairModelShowName(iIndex);
			break;
		}
	case 1:
		{
			return GetManHairModelShowName(iIndex);
			break;
		}
	}
	return NULL;
}

// �õ������Ƿ�����Ϣ����
//
// iIndex = ������Ϣ����
LPCSTR  CHairModelMng::GetManHairModelShowName(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_ManHairModelVector.size()))
	{
		return m_ManHairModelVector[iIndex].pShowName;
	}

	return NULL;
}

// �õ�Ů���Ƿ�����Ϣ����
//
// iIndex = ������Ϣ����
LPCSTR	CHairModelMng::GetWomanHairModelShowName(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_WomanHairModelVector.size()))
	{
		return m_WomanHairModelVector[iIndex].pShowName;
	}

	return NULL;
}
