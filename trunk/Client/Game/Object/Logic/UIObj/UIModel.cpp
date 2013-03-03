
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
// 在界面上显示的模型.
//

// 模型计数.
UINT CModelShowInUI::m_TeamNumCount = 0;



CModelShowInUI::CModelShowInUI()
{
	m_pAvatar = NULL;								// 用于UI显示的逻辑对象.
	memset(m_szBufModel, 0, sizeof(m_szBufModel));	// 模型名字.

	// 创建新的模型
	//CreateNewUIModel();
}

CModelShowInUI::~CModelShowInUI()
{
	DestroyUIModel();
}


// 删除ui模型
BOOL CModelShowInUI::DestroyUIModel()
{
	if(m_pAvatar)
	{
		CFakeObjSystem::GetMe()->DestroyObject(m_szBufModel);
		m_pAvatar = NULL;

//		m_TeamNumCount--;
	}

	memset(m_szBufModel, 0, sizeof(m_szBufModel));	// 模型名字.

	return FALSE;
}

// 创建新的模型
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
	// 创建Avatar
	m_pAvatar  = (CObject_PlayerOther*)(CFakeObjSystem::GetMe()->NewFakeObject(_T("CObject_PlayerOther"), m_szBufModel,"Camera_Main"));

	m_pAvatar->SetFaceDir(0);
//	m_pAvatar->GetCharacterData()->Set_RaceID(0);
//	// 头发颜色
//	m_pAvatar->GetCharacterData()->Set_HairColor(0);

	return FALSE;
}



//***************
//***************
// 设置模型信息

// 设置装备
VOID CModelShowInUI::SetUIModelInfo(HUMAN_EQUIP point, INT nID)
{
	if( m_pAvatar )
	{
		// 主武器
		if( HEQUIP_MAINHAND == point )
		{
			char buf[512];
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "主武器id ＝ %d", nID);
			AxTrace(0, 0, buf);
		}
		// 副武器
		if( HEQUIP_ASSIHAND == point )
		{
			char buf[512];
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "副武器id ＝ %d", nID);
			AxTrace(0, 0, buf);
		}

		m_pAvatar->GetCharacterData()->Set_Equip( point, nID );
	}
}

// 设置脸的模型
VOID CModelShowInUI::SetFaceMeshId(INT nID)
{
	m_pAvatar->GetCharacterData()->Set_FaceMesh(nID);
}

// 设置发型的模型.
VOID CModelShowInUI::SetFaceHairId(INT nID)
{
	m_pAvatar->GetCharacterData()->Set_HairMesh(nID);
}

// 设置头发颜色
VOID CModelShowInUI::SetHairColor(INT nID)
{
	m_pAvatar->GetCharacterData()->Set_HairColor(nID);
}


// 得到ui模型名字
const TCHAR* CModelShowInUI::GetUIModelName()
{
	return m_szBufModel;
}
