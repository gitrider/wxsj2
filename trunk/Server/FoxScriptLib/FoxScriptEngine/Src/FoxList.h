//---------------------------------------------------------------------------
// Script Engine by FireFox
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef	FoxList_H
#define	FoxList_H
//---------------------------------------------------------------------------
#include "FoxDebug.h"
#include "FoxNode.h"
//---------------------------------------------------------------------------

class FoxList
{
public:
	FoxNode m_ListHead; // ͷ�ڵ㣨���ǵ�һ���ڵ㣩
	FoxNode m_ListTail; // β�ڵ㣨�������Ľڵ㣩
public:
	FoxList(void);
	FoxNode* GetHead(void); // ȡ�õ�һ���ڵ�
	FoxNode* GetTail(void); // ȡ�����һ���ڵ�
	void AddHead(FoxNode *pNode); // ����ǰ������һ���ڵ�
	void AddTail(FoxNode *pNode); // �����������һ���ڵ�
	FoxNode* RemoveHead(void); // ɾ����һ���ڵ�
	FoxNode* RemoveTail(void); // ɾ�����һ���ڵ�
	BOOL IsEmpty(void); // �Ƿ��Ǹ��յ�����
	LONG GetNodeCount(void);
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����:	FoxList
// ����:	����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
inline FoxList::FoxList(void)
{
	m_ListHead.m_pNext = &m_ListTail;
	m_ListTail.m_pPrev = &m_ListHead;
}
//-------------------------------------------------------------------------
// ����:	IsEmpty
// ����:	�Ƿ�Ϊ��
// ����:	void
// ����:	BOOL
//---------------------------------------------------------------------------
inline BOOL FoxList::IsEmpty(void)
{
	return (m_ListHead.GetNext() == NULL);
}
//-------------------------------------------------------------------------
// ����:	GetHead
// ����:	ȡ�������ͷ
// ����:	void
// ����:	FoxNode*
//---------------------------------------------------------------------------
inline FoxNode* FoxList::GetHead(void)
{
	return m_ListHead.GetNext();
}
//-------------------------------------------------------------------------
// ����:	GetTail
// ����:	ȡ�������β
// ����:	void
// ����:	FoxNode*
//---------------------------------------------------------------------------
inline FoxNode* FoxList::GetTail(void)
{
	return m_ListTail.GetPrev();
}
//-------------------------------------------------------------------------
// ����:	AddHead
// ����:	��ͷ������һ���ڵ�
// ����:	FoxNode*
// ����:	BOOL
//---------------------------------------------------------------------------
inline void FoxList::AddHead(FoxNode *pNode)
{
	m_ListHead.InsertAfter(pNode);
}
//-------------------------------------------------------------------------
// ����:	AddTail
// ����:	��ĩβ����һ���ڵ�
// ����:	FoxNode*
// ����:	void
//---------------------------------------------------------------------------
inline void FoxList::AddTail(FoxNode *pNode)
{
	m_ListTail.InsertBefore(pNode);
}
//-------------------------------------------------------------------------
// ����:	RemoveHead
// ����:	ɾ����һ���ڵ�
// ����:	void
// ����:	FoxNode*
//---------------------------------------------------------------------------
inline FoxNode* FoxList::RemoveHead(void)
{
	FoxNode* pNode = m_ListHead.GetNext();
	if (pNode)
		pNode->Remove();
	return pNode;
}
//-------------------------------------------------------------------------
// ����:	RemoveTail
// ����:	ɾ�����һ���ڵ�
// ����:	void
// ����:	FoxNode*
//---------------------------------------------------------------------------
inline FoxNode* FoxList::RemoveTail(void)
{
	FoxNode* pNode = m_ListTail.GetPrev();
	if (pNode)
		pNode->Remove();
	return pNode;
}
//-------------------------------------------------------------------------
// ����:	GetNodeCount
// ����:	ȡ�ýڵ����
// ����:	void
// ����:	LONG
//---------------------------------------------------------------------------
inline LONG FoxList::GetNodeCount(void)
{
	long nNode = 0;
	FoxNode* pNode = GetHead();
	while (pNode)
	{
		pNode = pNode->GetNext();
		nNode++;
	}
	return nNode;
}
//-------------------------------------------------------------------------

#endif
