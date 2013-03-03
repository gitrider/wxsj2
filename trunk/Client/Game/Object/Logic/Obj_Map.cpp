
#include "StdAfx.h"
#include "Obj_Map.h"
#include "World/WorldManager.h"
#include "Global.h"


KL_IMPLEMENT_DYNAMIC(CObject_Map, GETCLASS(CObject_Phy));


CObject_Map::CObject_Map()
	: m_ivGridPos(-1, -1)
	, m_pZone(NULL)
{
}

CObject_Map::~CObject_Map()
{
}


VOID CObject_Map::SetPosition(const fVector3& fvPosition) 
{ 
	//�ҵ�����������������
	CScene* pActiveScene = (CScene*)CWorldManager::GetMe()->GetActiveScene();
	if(pActiveScene)
	{
		//��ǰ�м����
		m_ivGridPos.x = (INT)pActiveScene->GetZoneX(fvPosition.x);
		m_ivGridPos.y = (INT)pActiveScene->GetZoneZ(fvPosition.z);

		//�Ƿ���UIģ��
		if(!GetFakeObjectFlag()) 
		{
			//����Ѿ���ע������������ȷ�ע��
			if(m_pZone)
				m_pZone->UnRegisterObject(this);

			//ע�ᵽ��������
			CZone* pZone = pActiveScene->GetZone(m_ivGridPos.x, m_ivGridPos.y);
			if(!pZone || pZone == m_pZone) return;
			pZone->RegisterObject(this);
		}
	}

	CObject::SetPosition( fvPosition );
}

