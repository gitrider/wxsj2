
#include "StdAfx.h"
#include "Zone.h"
#include "Global.h"
#include "Object\Logic\Obj_Map.h"
#include "GIException.h"



CZone::CZone()
{
}

CZone::~CZone()
{
}


VOID CZone::Initial(VOID)
{
	// �������
	m_setRegisterObject.clear();
	m_listRegion.clear();

	return;
}

VOID CZone::RegisterObject(CObject_Map* pObject)
{
	KLAssert(pObject);

	// �ж��Ƿ�ȷʵ��CObject_Map�̳���
	if(!g_theKernel.IsKindOf(pObject->GetClass(), GETCLASS(CObject_Map))) 
		return;

	// ע�ᵽSet�У�����Ѿ��и�id������ע�ᣬ��ֱ���滻
	INT id = pObject->GetID();
	m_setRegisterObject.insert( id );

	pObject->m_pZone = this;
}

VOID CZone::UnRegisterObject(CObject_Map* pObject)
{
	KLAssert(pObject);

	// ��Set�����������壬����ҵ���ɾ��
	INT id = pObject->GetID();
	std::set< INT >::iterator it = m_setRegisterObject.find( id );
	if(it != m_setRegisterObject.end())
	{
		m_setRegisterObject.erase(it);
	}

	pObject->m_pZone = NULL;
}

VOID CZone::AddRegion(INT id)
{
	std::list<INT>::iterator it;
	it = find(m_listRegion.begin(),m_listRegion.end(), id);
	
	if(it == m_listRegion.end())
	{
		m_listRegion.push_back(id);		
	}

}

bool CZone::RemoveRegion(INT id)
{
	std::list<INT>::iterator it;
	it = find(m_listRegion.begin(), m_listRegion.end(), id);

	if(it != m_listRegion.end())
	{
		m_listRegion.erase(it);

		return true;
	}

	return false;
}

