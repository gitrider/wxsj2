#include "StdAfx.h"
#include ".\characterfacemng.h"
#include "..\dbc\GMDataBase.h"
#include "GameProcedure.h"
#include "GIException.h"
CCharacterFaceMng::CCharacterFaceMng(void)
{
	// ������ͷ�����
	m_iManFaceCount   = 0;
	// Ů����ͷ�����
	m_iWomanFaceCount = 0;
}

CCharacterFaceMng::~CCharacterFaceMng(void)
{
}

// �õ�����ͷ�����
//
// iRace = 0 - Ů
//         1 - ��
// iIndex = ͷ����Ϣ����
int CCharacterFaceMng::GetManFaceCount(int iRace)
{
	switch(iRace)
	{
	case 0://Ů
		{
			int iCount = GetWomanFaceCount();
			return iCount;
			break;
		}
	case 1://��
		{
			int iCount = GetManFaceCount();
			return iCount;
			break;
		}
	default:
		{
			return 0;
		}
	}
}

	
// �õ�������ͷ�����
int CCharacterFaceMng::GetManFaceCount()
{
	return m_iManFaceCount;
}

	// �õ�Ů����ͷ�����
int CCharacterFaceMng::GetWomanFaceCount()
{
	return m_iWomanFaceCount;
}

// �õ�����ͷ����Ϣ
//
// iRace = 0 - Ů
//         1 - ��
// iIndex = ͷ����Ϣ����
LPCSTR  CCharacterFaceMng::GetFaceImageInfo(int iRace, int iIndex)
{
	switch(iRace)
	{
	case 0://Ů
		{
			return GetWomanFaceImageInfo(iIndex);
			break;
		}
	case 1://��
		{
			return GetManFaceImageInfo(iIndex);
			break;
		}
	default:
		{
			return NULL;
		}
	}

	return NULL;
}

// �õ�������ͷ����Ϣ
//
// iIndex = ͷ����Ϣ����
LPCSTR  CCharacterFaceMng::GetManFaceImageInfo(int iIndex)
{
	if((iIndex < m_iManFaceCount)&&(iIndex >= 0))
	{
		return m_ManFaceInfoVector[iIndex].pImageSetName;
	}
	return NULL;
}

// �õ�Ů����ͷ����Ϣ
//
// iIndex = ͷ����Ϣ����
LPCSTR	CCharacterFaceMng::GetWomanFaceImageInfo(int iIndex)
{
	
	if((iIndex < m_iWomanFaceCount)&&(iIndex >= 0))
	{
		LPCSTR pImageName = NULL;
		const _DBC_CHAR_FACE& FaceInfo = m_WomanFaceInfoVector[iIndex];
		pImageName = FaceInfo.pImageSetName;

		return pImageName;
	}
	return NULL;
}

// �õ���ɫͷ����Ϣ
int CCharacterFaceMng::GetCharacterFaceIfo()
{
	// �������
	ClearData();

	DBC_DEFINEHANDLE(s_CharacterFaceInfo, DBC_CHARACTER_FACE);	// char_face.tab
	if(NULL == s_CharacterFaceInfo)
	{
		return 1;
	}

	int iItemCount = s_CharacterFaceInfo->GetRecordsNum();

	for(int i = 0; i < iItemCount; i++)
	{
		//������¼
		const _DBC_CHAR_FACE* pFaceInfo = (const _DBC_CHAR_FACE*)s_CharacterFaceInfo->Search_LineNum_EQU((UINT)i);
		if(pFaceInfo)
		{
			switch(pFaceInfo->nRace)			
			{
			case 0:
				{
					// Ů����ͷ�����
					m_iWomanFaceCount++;
					m_WomanFaceInfoVector.push_back(*pFaceInfo);
					break;
				}
			case 1:
				{
					// ������ͷ�����
					m_iManFaceCount++;
					m_ManFaceInfoVector.push_back(*pFaceInfo);
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
void CCharacterFaceMng::ClearData()
{
	// ������ͷ�����
	m_iManFaceCount   = 0;
	// Ů����ͷ�����
	m_iWomanFaceCount = 0;

	// ������ͷ����Ϣ����
	m_ManFaceInfoVector.clear();
	// Ů����ͷ����Ϣ����
	m_WomanFaceInfoVector.clear();
}


// �õ�Ů����ͷ��Id
int CCharacterFaceMng::GetWomanFaceId(int iIndex)
{
	if(iIndex >= (int)m_WomanFaceInfoVector.size())
	{
		return  0;
	}

	return m_WomanFaceInfoVector[iIndex].nID;
}


// �õ�������ͷ��Id
int CCharacterFaceMng::GetManFaceId(int iIndex)
{
	if(iIndex >= (int)m_ManFaceInfoVector.size())
	{
		return 0;
	}

	return m_ManFaceInfoVector[iIndex].nID;
}

