
/**	UIModel.cpp
*/

#include "StdAfx.h"
#include "UIModel.h"
#include "..\character\Obj_PlayerOther.h"
#include "..\Character\Obj_Character.h"
#include "FakeObject\GMFakeObjSystem.h"
#include "DataPool\GMDP_CharacterData.h"



//--------------------------------------------------------------------------------------------------------------------------
//
// �ڽ�������ʾ��ģ��.
//

// ģ�ͼ���.
UINT CModelShowInUI::m_TeamNumCount = 0;



CModelShowInUI::CModelShowInUI()
{
	m_pAvatar = NULL;								// ����UI��ʾ���߼�����.
	memset(m_szBufModel, 0, sizeof(m_szBufModel));	// ģ������.

	// �����µ�ģ��
	//CreateNewUIModel();
}

CModelShowInUI::~CModelShowInUI()
{
	DestroyUIModel();
}


// ɾ��uiģ��
BOOL CModelShowInUI::DestroyUIModel()
{
	if(m_pAvatar)
	{
		CFakeObjSystem::GetMe()->DestroyObject(m_szBufModel);
		m_pAvatar = NULL;

//		m_TeamNumCount--;
	}

	memset(m_szBufModel, 0, sizeof(m_szBufModel));	// ģ������.

	return FALSE;
}

// �����µ�ģ��
BOOL CModelShowInUI::CreateNewUIModel(const CHAR* szModelName)
{

//	static int iUiModelCount = 0;
//	iUiModelCount++;
//	m_TeamNumCount = ::timeGetTime();
//	m_TeamNumCount += iUiModelCount;

	if( m_pAvatar )
	{
		DestroyUIModel();
	}

	if (!szModelName)
	{
		m_TeamNumCount++;
		memset(m_szBufModel, 0, sizeof(m_szBufModel));
		_stprintf(m_szBufModel, "uiModel%d", m_TeamNumCount);
	}
	else
	{
		strncpy(m_szBufModel, szModelName,127);
	}
	// ����Avatar
	m_pAvatar  = (CObject_PlayerOther*)(CFakeObjSystem::GetMe()->NewFakeObject(_T("CObject_PlayerOther"), m_szBufModel,"Camera_Main"));

	m_pAvatar->SetFaceDir(0);
//	m_pAvatar->GetCharacterData()->Set_RaceID(0);
//	// ͷ����ɫ
//	m_pAvatar->GetCharacterData()->Set_HairColor(0);

	return FALSE;
}



//***************
//***************
// ����ģ����Ϣ

// ����װ��
VOID CModelShowInUI::SetUIModelInfo(HUMAN_EQUIP point, INT nID)
{
	if( m_pAvatar )
	{
		// ������
		if( HEQUIP_MAINHAND == point )
		{
			char buf[512];
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "������id �� %d", nID);
			AxTrace(0, 0, buf);
		}
		// ������
		if( HEQUIP_ASSIHAND == point )
		{
			char buf[512];
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "������id �� %d", nID);
			AxTrace(0, 0, buf);
		}

		m_pAvatar->GetCharacterData()->Set_Equip( point, nID );
	}
}

// ��������ģ��
VOID CModelShowInUI::SetFaceMeshId(INT nID)
{
	m_pAvatar->GetCharacterData()->Set_FaceMesh(nID);
}

// ���÷��͵�ģ��.
VOID CModelShowInUI::SetFaceHairId(INT nID)
{
	m_pAvatar->GetCharacterData()->Set_HairMesh(nID);
}

// ����ͷ����ɫ
VOID CModelShowInUI::SetHairColor(INT nID)
{
	m_pAvatar->GetCharacterData()->Set_HairColor(nID);
}


// �õ�uiģ������
const TCHAR* CModelShowInUI::GetUIModelName()
{
	return m_szBufModel;
}
