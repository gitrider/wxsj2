
/**	GMObjectDestoryQueue.cpp
 */

#include "StdAfx.h"
#include "GMObjectDestoryQueue.h"
#include "Manager/ObjectManager.h"
#include "Logic/Character/Obj_Character.h"


CObjectDestoryQueue::CObjectDestoryQueue()
{
}

CObjectDestoryQueue::~CObjectDestoryQueue()
{
}


BOOL CObjectDestoryQueue::TryAddDestoryTask(INT idObj)
{
	m_DestoryTask.push_back(idObj);

	return TRUE;
}

void CObjectDestoryQueue::Tick(void)
{
	if(!m_DestoryTask.empty())
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		if(!pObjectManager)
			return ;

		CObject_Character* pObj = NULL;

		std::vector<INT>::iterator iter = m_DestoryTask.begin();
		std::vector<INT>::iterator iterE = m_DestoryTask.end();
		std::vector<INT>::iterator itPrev;
		for(; iter != iterE; )
		{
			itPrev = iter;
			iter++;

			pObj = (CObject_Character*)(pObjectManager->FindServerObject(*itPrev));
			if(!pObj)
				continue ;

			// �ж��Ƿ����ɾ��
//			if(pObj->IsDestory())
			{
				pObj->Release();
				CObjectManager::GetMe()->DestroyObject(pObj);

				m_DestoryTask.erase(itPrev);
			}
		}

	}
}

void CObjectDestoryQueue::ClearAllTask(void)
{
	// ���ͷţ� ��ȫ�����
	m_DestoryTask;
}
