
/**	ObjectEntity.cpp
 */

#include "StdAfx.h"
#include "ObjectEntity.h"
#include "../../Engine/EngineInterface.h"
#include "Core/FairySystem.h"
#include "Core/FairyObjectFactoryManager.h"
#include "GIException.h"
#include <OgreSceneManager.h>
#include <OgreVector3.h>



CObjectEntity::CObjectEntity()
{
	m_nRayQueryLevel = RL_UNKNOWN;
	m_nData = 0;
}

CObjectEntity::~CObjectEntity()
{
}


VOID CObjectEntity::Destroy(VOID)
{
	if(ETYPE_DUMMY != GetType())	// 调用dummy自身的
		// 销毁FairyObj
		m_pFairyObj.reset();

	// 删除自身
	CEngineInterface::GetMe()->Destroy_EntityObject(this);
	
	return;
	// 之后不能调用任何代码
	//...
}


VOID CObjectEntity::SetFairyProperty(const char* szPropertyName, const char* szPropertyValue)
{
	if(ETYPE_DUMMY == GetType())
		return ;

	KLAssert(m_pFairyObj);

	m_pFairyObj->setPropertyAsString(szPropertyName, szPropertyValue);
}

