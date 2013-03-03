
/**	GINode.cpp
 */

#include "StdAfx.h"
#include "GINode.h"



KL_IMPLEMENT_DYNAMIC(tNode, NULL);


tNode::tNode(VOID)
	: m_pParentNode(NULL)
{
}

tNode::tNode(LPCTSTR szName)
{ 
	m_strName = szName;	
}


// �����ӽڵ�,���û���ҵ�,����NULL;
tNode*	tNode::LookUpChild(LPCTSTR szNodeName)
{
	register std::list< tNode * >::iterator it;
	for(it = m_ChildBuf.begin(); it!=m_ChildBuf.end(); it++)
	{
		if((*it)->GetNodeName() == szNodeName) return (tNode*)*it;
	}
	return NULL;
}

//����ӽڵ㵽�ýڵ���
VOID tNode::AddChild(tNode *pNewNode)
{
	if(!pNewNode) return;
	(pNewNode->m_pParentNode) = this;
	m_ChildBuf.push_back(pNewNode);
}

//ɾ��ĳ���ӽڵ�,���ýڵ����ʱ����TRUE,���򷵻�FALSE
BOOL tNode::EraseChild(tNode* pNode)
{
	if(GetChildBuf().empty()) return FALSE;
	register std::list< tNode * >::iterator it;
	for(it = m_ChildBuf.begin(); it!=m_ChildBuf.end(); it++)
	{
		if(pNode == *it)
		{
			m_ChildBuf.erase(it);
			return TRUE;
		}
	}
	return FALSE;
}

//ɾ�������ӽڵ�
VOID tNode::EraseAllChild( VOID )
{
	if(GetChildBuf().empty()) return ;
	m_ChildBuf.clear( );
}

//�ͷ��Լ�����ӵ�е���Դ
VOID tNode::Release(VOID)
{
	if(GetChildBuf().empty()) return;
	//�ͷ������ӽڵ�
	register std::list< tNode * >::iterator it;
	for(it = m_ChildBuf.begin(); it!=m_ChildBuf.end(); it++)
	{
		(*it)->Release();
		delete (*it);
		*it = NULL;
	}
	m_ChildBuf.clear();
}

VOID tNode::Tick(VOID)
{
	if(GetChildBuf().empty()) return;

	register std::list< tNode* >::iterator it=GetChildBuf().end(), itNext;
	it--;
	
	do
	{
		itNext = it;
		if(itNext == GetChildBuf().begin()) itNext = GetChildBuf().end();
		else itNext--;

		((tNode*)(*it))->Tick();

		if(itNext == GetChildBuf().end()) break;

		it = itNext;

	} while(true);
}