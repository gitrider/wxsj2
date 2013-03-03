//---------------------------------------------------------------------------
// Script Engine by FireFox
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef	FoxNode_H
#define	FoxNode_H
//---------------------------------------------------------------------------

#include "FoxDebug.h"

#pragma warning(disable: 4275)
#pragma warning(disable: 4251)

class FoxNode
{
public:
	FoxNode* m_pNext;
	FoxNode* m_pPrev;

public:
	FoxNode(void);
	virtual ~FoxNode(){};
	FoxNode* GetNext(void);
	FoxNode* GetPrev(void);
	void InsertBefore(FoxNode* pNode);
	void InsertAfter(FoxNode* pNode);
	void Remove(void);
	BOOL IsLinked(void);
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����:	Foxnode
// ����:	����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
inline FoxNode::FoxNode(void)
{
	m_pNext = NULL;
	m_pPrev = NULL;
}
//---------------------------------------------------------------------------
// ����:	GetNext
// ����:	��һ�����
// ����:	void
// ����:	FoxNode*
//---------------------------------------------------------------------------
inline FoxNode* FoxNode::GetNext(void)
{
	if (m_pNext->m_pNext)
		return m_pNext;
	return NULL;
}
//---------------------------------------------------------------------------
// ����:	GetPrev
// ����:	ǰһ�����
// ����:	void
// ����:	FoxNode*
//---------------------------------------------------------------------------
inline FoxNode* FoxNode::GetPrev(void)
{
	if (m_pPrev->m_pPrev)
		return m_pPrev;
	return NULL;
}
//----------------------------------------------------------------------------
// ����:	InsertBefore
// ����:	��ǰ�����һ�����
// ����:	FoxNode*
// ����:	void
//---------------------------------------------------------------------------
inline void FoxNode::InsertBefore(FoxNode *pNode)
{
	KASSERT(m_pPrev);
	if (!pNode || !m_pPrev)
		return;

	pNode->m_pPrev = m_pPrev;
	pNode->m_pNext = this;
	m_pPrev->m_pNext = pNode;
	m_pPrev = pNode;
}
//---------------------------------------------------------------------------
// ����:	InsertAfter
// ����:	�ں������һ�����
// ����:	FoxNode*
// ����:	void
//---------------------------------------------------------------------------
inline void FoxNode::InsertAfter(FoxNode *pNode)
{
	KASSERT(m_pNext);
	if (!pNode || !m_pNext)
		return;
	pNode->m_pPrev = this;
	pNode->m_pNext = m_pNext;
	m_pNext->m_pPrev = pNode;
	m_pNext = pNode;
}
//---------------------------------------------------------------------------
// ����:	Remove the node
// ����:	ɾ��������
// ����:	void
// ����:	FoxNode*
//---------------------------------------------------------------------------
inline void FoxNode::Remove(void)
{
	KASSERT(m_pPrev);
	KASSERT(m_pNext);
	if (m_pPrev)
		m_pPrev->m_pNext = m_pNext;
	if (m_pNext)
		m_pNext->m_pPrev = m_pPrev;
	m_pPrev = NULL;
	m_pNext = NULL;
}
//---------------------------------------------------------------------------
// ����:	IsLinked
// ����:	����ڵ��Ƿ�����
// ����:	void
// ����:	bool
//---------------------------------------------------------------------------
inline BOOL FoxNode::IsLinked(void)
{
	return (m_pPrev && m_pNext);
}
//--------------------------------------------------------------------------------

#endif
