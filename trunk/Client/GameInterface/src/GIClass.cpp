
#include "StdAfx.h"
#include "GIClass.h"
#include "GINode.h"



/// 用于生成一个实例的函数
tNode* tClass::CreateObject(LPCTSTR szName)	
{
	if(pfnCreateObject == NULL) 
		return NULL;

	tNode *pObject = (*pfnCreateObject)();
	pObject->m_strName = szName;
	
	return pObject;
}