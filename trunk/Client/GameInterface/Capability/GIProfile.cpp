
#include "StdAfx.h"
#include "GIProfile.h"
#include "GIException.h"



//Profile�ڵ��ջ
#define g_theStack GetTheStack()
//Profile�ڵ���
#define g_theTree GetTheTree()
//�Ƿ��в�ѯ������
BOOL				g_bAskDump = FALSE;
//��ѯ������
FUNC_PROFILE_DUMP	g_funcDump = NULL;




PROFILE_STACK& GetTheStack()
{
	static PROFILE_STACK theStack;
	return theStack;
}

PROFILE_TREE& GetTheTree()
{
	static PROFILE_TREE theTree;
	static BOOL bFirst = TRUE;
	if(bFirst)
	{
		theTree.theRoot.strName = "ROOT";
		theTree.theRoot.pParentNode = NULL;
		theTree.theRoot.nInvokeCount = 0;
		theTree.theRoot.nTotalTime.QuadPart = 0;
		theTree.pCurrentNode = &(theTree.theRoot);
		bFirst = FALSE;
	}

	return theTree;
}

PROFILE_STACK_NODE::PROFILE_STACK_NODE(LPCTSTR szName, LPCTSTR szAttName)
{
	//��¼��ʼʱ��
	QueryPerformanceCounter(&nStartTime);
	if(!szName) strName.reserve(128);
	else		
	{
		strName = szName;
		
		if(szAttName)
		{
			strName += ".";
			strName += szAttName;
		}
	}
	pTreeNode = NULL;
}

RPOFILE_TREE_NODE::~RPOFILE_TREE_NODE()
{
	//�ͷ��ӽڵ�
	HASH_TREENODE::iterator it;
	for(it=mapChild.begin(); it!=mapChild.end(); it++)
	{
		if(it->second)
		{
			delete it->second;
			it->second = NULL;
		}
	}
	mapChild.clear();
}

extern "C"
VOID KLProfile_PushNode(LPCTSTR name, LPCTSTR nameEx)
{
	PROFILE_STACK_NODE theNode(name, nameEx);

	PROFILE_STACK_NODE* pNode = &theNode;
	//--------------------------------------------------------
	//���Ŀǰ�Ķ����Ƿ�������
	PROFILE_STACK::HASH_NAMESET::iterator itHashName = g_theStack.vHashName.find(pNode->strName);
	if(itHashName != g_theStack.vHashName.end())
	{
		KLThrow("[PUSH]Profile multi name push:%s", pNode->strName.c_str());
		return;
	}

	//����ջ�Ƿ����
	if(g_theStack.nTopIndex >= PROFILE_STACK::MAX_STATCK_NUM)
	{
		KLThrow("[PUSH]Profile stack overflow!");
		return;
	}

	//--------------------------------------------------------
	//�����ϲ����½ڵ�

	//�����Ƿ��иýڵ�
	RPOFILE_TREE_NODE::HASH_TREENODE::iterator itChild = 
		g_theTree.pCurrentNode->mapChild.find(pNode->strName);
	//���û�У�����
	if(itChild == g_theTree.pCurrentNode->mapChild.end())
	{
		RPOFILE_TREE_NODE* pNewNode = new RPOFILE_TREE_NODE;
		pNewNode->strName = pNode->strName;
		pNewNode->pParentNode = g_theTree.pCurrentNode;
		pNewNode->nInvokeCount = 0;
		pNewNode->nTotalTime.QuadPart = 0;

		//���뵽����
		g_theTree.pCurrentNode->mapChild.insert(std::make_pair(pNode->strName, pNewNode));
		itChild = g_theTree.pCurrentNode->mapChild.find(pNode->strName);
	}

	//�����м�¼����
	g_theTree.pCurrentNode = itChild->second;

	//--------------------------------------------------------
	//�����ջ��
	PROFILE_STACK_NODE& topNode = g_theStack.vNodeStack[g_theStack.nTopIndex];
	topNode.strName = pNode->strName;
	topNode.nStartTime = pNode->nStartTime;
	topNode.pTreeNode = itChild->second;
	topNode.pTreeNode->nInvokeCount ++;
	g_theStack.nTopIndex++;

	int a = g_theStack.nTopIndex;

	int b=1;
}

extern "C"
VOID KLProfile_PopNode(LPCTSTR szName)
{
	//����ջ�Ƿ��Ѿ���
	if(g_theStack.nTopIndex <= 0)
	{
		KLThrow("[POP]Profile stack empty!");
		return;
	}

	//--------------------------------------------------------
	//�����ջ�нڵ�
	PROFILE_STACK_NODE& topStackNode = g_theStack.vNodeStack[g_theStack.nTopIndex-1];
	
	//�������Ƿ���ȷ
	if(szName)
	{
		if(strncmp(topStackNode.strName.c_str(), szName, strlen(szName)))
		//if(topStackNode.strName != szName)
		{
			KLThrow("[POP]Profile statck error!");
		}
	}

	//�������ʱ��
	LARGE_INTEGER tNow;
	QueryPerformanceCounter(&tNow);
	LARGE_INTEGER nProcessTime;
	nProcessTime.QuadPart = tNow.QuadPart - topStackNode.nStartTime.QuadPart;

	g_theStack.nTopIndex--;

	//--------------------------------------------------------
	//�������нڵ�
	RPOFILE_TREE_NODE&  currentTreeNode = *(g_theTree.pCurrentNode);
	currentTreeNode.nTotalTime.QuadPart += nProcessTime.QuadPart;

	g_theTree.pCurrentNode = currentTreeNode.pParentNode;

	//--------------------------------------------------------
	//��ѯ
	KLProfile_Tick();
}

VOID KLProfile_Tick(VOID)
{
	//����в�ѯ�����Ҷ�ջ��
	if(g_bAskDump && g_funcDump && g_theStack.nTopIndex==0)
	{
		//����Root��ʱ��
		g_theTree.theRoot.nTotalTime.QuadPart = 0;
		RPOFILE_TREE_NODE::HASH_TREENODE::iterator it;
		for(it=g_theTree.theRoot.mapChild.begin(); it!=g_theTree.theRoot.mapChild.end(); it++)
		{
			g_theTree.theRoot.nTotalTime.QuadPart += it->second->nTotalTime.QuadPart;
		}

		(g_funcDump)(&g_theTree);

		g_bAskDump = FALSE;
	}
}

//�����ѯProfile�ڵ���Ϣ(����ջ��ʱ����ô���ĺ���ָ��)
extern "C"
VOID KLProfile_DumpSruct(FUNC_PROFILE_DUMP func)
{
	KLAssert(func);

	g_bAskDump = TRUE;
	g_funcDump = func;
}
