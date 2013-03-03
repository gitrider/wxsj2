
/** ���ݺ���
 */

#include "StdAfx.h"
#include "GIKernel.h"
#include "GIException.h"
#include "GIUtil.h"
#include "GIPlugin.h"
#include "GIObjectManager.h"



tKernel* tKernel::s_pMe = NULL;
void*	 tKernel::s_pUserdata = NULL;

tKernel::tKernel(VOID)
	: m_theRoot(_T("root")),
	  m_pFirstClass(NULL)
{
	s_pMe = this;
}

tKernel::~tKernel(VOID)
{
}


// ��ĳ����ע�ᵽȫ�����������
VOID tKernel::ReisgerClass(tClass* pNewClass)
{
	KLAssert(pNewClass);

	if(m_ClassMap[STRING(pNewClass->szClassName)] != NULL)
	{
		KLThrow(_T("Multi register class:%s"), pNewClass->szClassName);
	}

	pNewClass->pNextClass = m_pFirstClass;
	m_pFirstClass = pNewClass;
	//���µ�����뵽�������
	(m_ClassMap)[pNewClass->szClassName] = pNewClass;
}

//����������Ƿ���м̳й�ϵ
BOOL tKernel::IsKindOf(const tClass* pThisClass, const tClass* pBaseClass) const
{
	if(!pBaseClass || !pThisClass) 
		return FALSE;

	register const tClass *pClass = pThisClass;

	while(pClass != NULL)
	{
		if(pClass == pBaseClass)
		{
			return TRUE;
		}

		pClass = pClass->pBaseClass;
	}

	return FALSE;
}
//�����ַ�����ʽ����ȡ��ĳ������
tClass* tKernel::OpenClass(LPCTSTR szClassName)
{
	std::map< STRING, tClass* >::iterator it = m_ClassMap.find(szClassName);
	if(it == m_ClassMap.end()) 
		return 0;
	else 
		return it->second;
}

//�����ַ�����ʽ��·���������½ڵ�
tNode*	tKernel::NewNode(LPCTSTR szClassName, LPCTSTR szPosition, LPCTSTR szNodeName)
{
	KLAssert(szClassName && szPosition && szNodeName);

	//����
	tClass *pClass = OpenClass(szClassName);
	if(!pClass)
	{
		KLThrow(_T("(tKernel::NewNode)Can't create new Node[%s]"), szClassName);
	}
	
	std::vector< STRING > vPathSplitBuf;
	KLU_ConvertStringToVector(szPosition, vPathSplitBuf, _T("\\/"));

	tNode *pCurrentNode = &m_theRoot;
	tNode *pFindNode = NULL;

	//�������ڵ�
	for(register INT i=0; i<(INT)vPathSplitBuf.size(); i++)
	{
		tNode *pFindNode = pCurrentNode->LookUpChild((LPCTSTR)vPathSplitBuf[i].c_str());
		if(pFindNode == NULL)
		{
			tNode *pNewNode = new tNode(vPathSplitBuf[i].c_str());
			pCurrentNode->AddChild(pNewNode);
			pFindNode = pNewNode;
		}

		pCurrentNode = pFindNode;
	}

	//�����Ƿ�ýڵ��Ѿ�����
	if(pCurrentNode->LookUpChild(szNodeName))
	{
		KLThrow(_T("(tKernel::NewNode)The Node[%s] has exist!"), szNodeName);
	}

	tNode *pNewNode = (tNode *)pClass->CreateObject(szNodeName);
	pCurrentNode->AddChild(pNewNode);
	return pNewNode;
}

//���ݸ��ڵ�����µĽڵ�
tNode*	tKernel::NewNode(LPCTSTR szClassName, tNode* pParentNode, LPCTSTR szNodeName)
{
	KLAssert(szClassName && pParentNode && szNodeName);

	//����
	tClass *pClass = OpenClass(szClassName);
	if(!pClass)
	{
		KLThrow(_T("(tKernel::NewNode)Can't create new Node[%s]"), szClassName);
	}

	//�����Ƿ�ýڵ��Ѿ�����
	if(pParentNode->LookUpChild(szNodeName))
	{
		KLThrow(_T("(tKernel::NewNode)The Node[%s] has exist!"), szNodeName);
	}

	tNode *pNewNode = (tNode *)pClass->CreateObject(szNodeName);
	pParentNode->AddChild(pNewNode);

	return pNewNode;
}


tNode*	tKernel::GetNode(LPCTSTR szPathName)
{
	KLAssert(szPathName);

	std::vector< STRING > vPathSplitBuf;
	KLU_ConvertStringToVector(szPathName, vPathSplitBuf, _T("\\/"));

	if(vPathSplitBuf.empty()) return NULL;

	tNode *pCurrentNode = &m_theRoot;
	for(register INT i=0; i<(INT)vPathSplitBuf.size(); i++)
	{
		tNode *pFindNode = pCurrentNode->LookUpChild((LPCTSTR)vPathSplitBuf[i].c_str());
		if(pFindNode == NULL)
		{
			return NULL;
		}
		pCurrentNode = pFindNode;
	}

	return pCurrentNode;
}

BOOL tKernel::LoadPlugin(LPCTSTR szPluginName, LPVOID pParam)
{
	if(!szPluginName) return FALSE;

	HMODULE hModule = NULL;
	FUNC_DLLINIT pfnDllInit = NULL;
	FUNC_DLLRELEASE pfnDllRelease = NULL;

	try
	{
		//����Load��dll
		hModule = ::LoadLibrary(szPluginName);
		if(!hModule)
		{
			KLThrow(_T("Load plugin dll(%s) failed!"), szPluginName);
			return FALSE;
		}

		//�쿴�Ƿ��Ѿ�����
		for(INT i=0; i<(INT)m_vAllPlugin.size(); i++)
		{
			if(m_vAllPlugin[i].hModule == hModule) return TRUE;
		}

		//�鿴�Ƿ��кϷ���DllMagicCode����
		UINT* pMagicCode = (UINT*)::GetProcAddress(hModule, "DllMagicCode");
		if(!pMagicCode || ((DLL_MAGIC_CODE|KL_ENGINE_VERSION) != *pMagicCode))
		{ 
			::FreeLibrary(hModule); 
			hModule = NULL; 
			KLThrow(_T("Wrong plugin dll(%s) version!"), szPluginName);
			return FALSE; 
		}

		//�鿴�Ƿ��кϷ��ĵ�������
		pfnDllInit = (FUNC_DLLINIT)::GetProcAddress(hModule, "DllInit");
		pfnDllRelease = (FUNC_DLLRELEASE)::GetProcAddress(hModule, "DllRelease");
		if(!pfnDllInit || !pfnDllRelease)
		{ 
			::FreeLibrary(hModule); hModule = NULL; 
			KLThrow(_T("Wrong plugin dll(%s) export function!"), szPluginName);
			return FALSE; 
		}
	}
	catch(...) 
	{
		if(hModule) ::FreeLibrary(hModule);
		throw;
		return FALSE;
	}

	//�ǺϷ��Ĳ����ִ�м��غ���
	(*pfnDllInit)(pParam);

	PLUGIN_INFO newPlugin;
	newPlugin.strPluginFile = szPluginName;
	newPlugin.hModule = hModule;
	newPlugin.pfnDllInit = pfnDllInit;
	newPlugin.pfnDllRelease = pfnDllRelease;

	m_vAllPlugin.push_back(newPlugin);

	return TRUE;
}


VOID tKernel::FreeAllPlugin(VOID)
{
	//�ͷ����в��
	for(INT i=0; i<(INT)m_vAllPlugin.size(); i++)
	{
		//ִ���ͷź���
		(*(m_vAllPlugin[i].pfnDllRelease))();

		::FreeLibrary(m_vAllPlugin[i].hModule);
		m_vAllPlugin[i].hModule = NULL;
	}

	m_vAllPlugin.clear();
}

//�رպ���
VOID tKernel::ShutDown(VOID)
{
	//�ͷŽڵ���
	m_theRoot.Release();

	//�ͷ����в��
	FreeAllPlugin();
}
