#include "StdAfx.h"
#include ".\facemodelmng.h"
#include "..\dbc\GMDataBase.h"
#include "GameProcedure.h"
#include "GIException.h"

CFaceModelMng::CFaceModelMng(void)
{
}

CFaceModelMng::~CFaceModelMng(void)
{
}


// �õ��������θ���
//
// iRace = 0 - Ů
//         1 - ��
// iIndex = ͷ����Ϣ����
int CFaceModelMng::GetFaceModelCount(int iRace)
{
	switch(iRace)
	{
	case 0:
		{
			return GetWomanFaceModelCount();
			break;
		}
	case 1:
		{
			return GetManFaceModelCount();
			break;
		}
	}
	return 0;
}

// �õ�������ͷ�����
int CFaceModelMng::GetManFaceModelCount()
{
	return m_iManFaceModelCount;
}

// �õ�Ů����ͷ�����
int CFaceModelMng::GetWomanFaceModelCount()
{
	return m_iWomanFaceModelCount;
}

// �õ�����ͷ����Ϣ
//
// iRace = 0 - Ů
//         1 - ��
// iIndex = ͷ����Ϣ����
LPCSTR  CFaceModelMng::GetFaceModel(int iRace, int iIndex)
{
	switch(iRace)
	{
	case 0:
		{
			return GetWomanFaceModel(iIndex);
			break;
		}
	case 1:
		{
			return GetManFaceModel(iIndex);
			break;
		}
	}
	return NULL;
}

// �õ�������ͷ����Ϣ
//
// iIndex = ͷ����Ϣ����
LPCSTR  CFaceModelMng::GetManFaceModel(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_ManFaceModelVector.size()))
	{
		return m_ManFaceModelVector[iIndex].pMeshFile;
	}
	return NULL;
}

// �õ�Ů����ͷ����Ϣ
//
// iIndex = ͷ����Ϣ����
LPCSTR	CFaceModelMng::GetWomanFaceModel(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_WomanFaceModelVector.size()))
	{
		return m_WomanFaceModelVector[iIndex].pMeshFile;
	}
	return NULL;
}

// �õ���ɫͷ����Ϣ
// 0 -- �õ���ɫͷ��ɹ�
// ���� -- ʧ��.
int CFaceModelMng::GetCharacterModel()
{
	// �������
	ClearData();

	DBC_DEFINEHANDLE(s_CharacterFaceModel, DBC_CHARACTER_HEAD_GEO);		// char_head.tab
	if( NULL == s_CharacterFaceModel )
	{
		return 1;
	}

	int iItemCount = s_CharacterFaceModel->GetRecordsNum();

	for(int i = 0; i < iItemCount; i++)
	{
		//������¼
		const _DBC_CHAR_HEAD_GEO* pFaceInfo = (const _DBC_CHAR_HEAD_GEO*)s_CharacterFaceModel->Search_LineNum_EQU((UINT)i);
		if(pFaceInfo)
		{
			switch(pFaceInfo->nRace)			
			{
			case 0:
				{
					// Ů����ͷ�����
					m_iWomanFaceModelCount++;
					m_WomanFaceModelVector.push_back(*pFaceInfo);
					break;
				}
			case 1:
				{
					// ������ͷ�����
					m_iManFaceModelCount++;
					m_ManFaceModelVector.push_back(*pFaceInfo);
					break;
				}
			default:
				{
					break;
				}
			}
		}
	}//

	return 0;
}

// �������
void CFaceModelMng::ClearData()
{
	// ������ͷ��ģ�͸���
	m_iManFaceModelCount   = 0;
	// Ů����ͷ��ģ�͸���
	m_iWomanFaceModelCount = 0;

	// ������ͷ��ģ����Ϣ����
	m_ManFaceModelVector.clear();
	// Ů����ͷ��ģ����Ϣ����
	m_WomanFaceModelVector.clear();
}

// �õ�Ů����ͷ��Id
int CFaceModelMng::GetWomanFaceModelId(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_WomanFaceModelVector.size()))
	{
		return m_WomanFaceModelVector[iIndex].nID;
	}
	return 0;
}

// �õ�������ͷ��Id
int CFaceModelMng::GetManFaceModelId(int iIndex)
{

	if((iIndex >= 0)&&(iIndex < (int)m_ManFaceModelVector.size()))
	{
		return m_ManFaceModelVector[iIndex].nID;
	}
	return 0;
}

// �õ�����model Id
int CFaceModelMng::GetFaceModelId(int iRace, int iIndex)
{
	switch(iRace)
	{
	case 0:
		{
			return GetWomanFaceModelId(iIndex);
			break;
		}
	case 1:
		{
			return GetManFaceModelId(iIndex);
			break;
		}
	}
	
	return 0;
}

// �õ�����ͷ����Ϣ
	//
	// iRace = 0 - Ů
	//         1 - ��
	// iIndex = ͷ����Ϣ����
LPCSTR  CFaceModelMng::GetFaceModelShowName(int iRace, int iIndex)
{
	switch(iRace)
	{
	case 0:
		{
			return GetWomanFaceModelShowName(iIndex);
			break;
		}
	case 1:
		{
			return GetManFaceModelShowName(iIndex);
			break;
		}
	}
	return NULL;
}

	// �õ�������ͷ����Ϣ
	//
	// iIndex = ͷ����Ϣ����
LPCSTR  CFaceModelMng::GetManFaceModelShowName(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_ManFaceModelVector.size()))
	{
		return m_ManFaceModelVector[iIndex].pShowName;
	}
	return NULL;
}

	// �õ�Ů����ͷ����Ϣ
	//
	// iIndex = ͷ����Ϣ����
LPCSTR	CFaceModelMng::GetWomanFaceModelShowName(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_WomanFaceModelVector.size()))
	{
		return m_WomanFaceModelVector[iIndex].pShowName;
	}
	return NULL;
}
