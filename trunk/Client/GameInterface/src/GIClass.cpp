
#include "StdAfx.h"
#include "GIClass.h"
#include "GINode.h"



/// ��������һ��ʵ���ĺ���
tNode* tClass::CreateObject(LPCTSTR szName)	
{
	if(pfnCreateObject == NULL) 
		return NULL;

	tNode *pObject = (*pfnCreateObject)();
	pObject->m_strName = szName;
	
	return pObject;
}