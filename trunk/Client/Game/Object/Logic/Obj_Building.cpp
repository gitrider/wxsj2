
#include "StdAfx.h"
#include "Obj_Building.h"
#include "DBC/GMDataBase.h"
#include "Procedure/GameProcedure.h"
#include "GIException.h"
#include "GIUtil.h"



KL_IMPLEMENT_DYNAMIC(CObject_Building, GETCLASS(CObject_Static));


CObject_Building::CObject_Building() : 
	m_pResDefine(0),
	m_nLevel(-1),
	m_resType(RT_Mesh)
{
}

CObject_Building::~CObject_Building()
{
}


VOID CObject_Building::Release(VOID)
{
	if(m_pRenderInterface)
	{
		m_pRenderInterface->Destroy();
		m_pRenderInterface = 0;
	}
}

/********************************************
	��Դ����ID�����ı�
*********************************************/
void CObject_Building::SetLogicData_Define(const _DBC_CITY_BUILDING* pDefine)
{
	//��ͬ
	if(m_pBuildingDefine == pDefine) return;

	m_pBuildingDefine = pDefine;

	//�����ﶨ���
	DBC_DEFINEHANDLE(s_pBuildingDefineDBC, DBC_BUILDING_DEINFE);
	m_pResDefine = (const _DBC_BUILDING_DEFINE*)s_pBuildingDefineDBC->Search_Index_EQU(m_pBuildingDefine->nBuildingID);
	if(!m_pResDefine)
	{
//		KLU_Log("Invalid building define[%d]", m_pBuildingDefine->nID);
		return;
	}

	//ȡ����Ⱦ��λ��
	const char* szGfxPos = pDefine->szGfxPosition;
	//�ֽ�
	std::vector< STRING > vGfxPos;
	KLU_ConvertStringToVector(szGfxPos, vGfxPos, " ", TRUE, TRUE);
	KLAssert(vGfxPos.size() == 3);

	fVector3 fvGfxPos(
		(float)atof(vGfxPos[0].c_str()), 
		(float)atof(vGfxPos[1].c_str()), 
		(float)atof(vGfxPos[2].c_str()));

	//ת������
	fVector3 fvGamePos;
	CGameProcedure::s_pGfxSystem->Axis_Trans(tGfxSystem::AX_GFX, fvGfxPos, tGfxSystem::AX_GAME, fvGamePos);

	//�����߼�λ��
	SetPosition(fvGamePos);

	//���´�����Ⱦ������
	CreateRenderInterface();
}

/********************************************
	�ȼ������ı�
*********************************************/
void CObject_Building::SetLogicData_Level(INT nLevel)
{
	if(m_nLevel == nLevel) return;

	//�Ƿ��ȼ�
	if(nLevel < 0 || nLevel >= _DBC_BUILDING_DEFINE::MAX_LEVEL)
	{
//		KLU_Log("Invalid building level[%d]", nLevel);
		return;
	}

	m_nLevel = nLevel;

	//���´�����Ⱦ������
	CreateRenderInterface();
}

/********************************************
	����Ⱦ�㴴����Ⱦָ��
*********************************************/
void CObject_Building::CreateRenderInterface(void)
{
	// �Ƿ���Ϸ�
	if(!m_pResDefine) return;
	if(m_nLevel < 0 || m_nLevel >= _DBC_BUILDING_DEFINE::MAX_LEVEL) return;

	// �ж�ģ������
	if(_stricmp(m_pResDefine->szResType, "Mesh") == 0)
	{
		m_resType = RT_Mesh;
	}
	else if(_stricmp(m_pResDefine->szResType, "Model") == 0)
	{
		m_resType = RT_Model;
	}
	else if(_stricmp(m_pResDefine->szResType, "Object") == 0)
	{
		m_resType = RT_Object;
	}
	else
	{
		// �Ƿ�����Դ����
		KLThrow("Invalid building resource type:'%s' in %d", m_pResDefine->szResType, m_pResDefine->nID);
		return;
	}


	// ���پ�����
	if(m_pRenderInterface)
	{
		m_pRenderInterface->Destroy();
		m_pRenderInterface = 0;
	}

	// ��Դ����
	const char* szRes = m_pResDefine->szLevel[m_nLevel];
	if(!szRes || szRes[0] == 0) return;

	// ��������Ⱦ����
	switch(m_resType)
	{
	case RT_Mesh:
		m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_MESH);
		m_pRenderInterface->StaObj_SetMeshFile(szRes);
		break;

	case RT_Model:
		m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_MODEL);
		m_pRenderInterface->StaObj_SetMeshFile(szRes);
		break;

	case RT_Object:
		m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_ACTOR);
		m_pRenderInterface->Actor_SetFile(szRes);
		break;

	default: return;
	}

	// ����޷�ѡ��
	m_pRenderInterface->SetRayQuery(FALSE);

	m_pRenderInterface->SetPosition( GetPosition() );
	// ������ת
	m_pRenderInterface->SetFairyProperty("orientation", m_pBuildingDefine->szGfxOrientation);


	//616====== ����͸��
	float fTransparency = 0.6f;
	m_pRenderInterface->Actor_SetTransparent(fTransparency, 1.0);

}
