
/**	GINode.h
 */

#pragma once


#include "GIBasicType.h"
#include "GIClass.h"


class KYLIN_API tNode
{
public:

	tNode(VOID);
	tNode(LPCTSTR szName);
	virtual ~tNode()													{ Release(); }

	// �����ӽڵ���Ŀ
	virtual INT GetChildNum(VOID) const									{ return (INT)m_ChildBuf.size(); }
	// ���ؽڵ���
	virtual const STRING& GetNodeName(VOID) const						{ return m_strName; }
	// �����ӽڵ��б�
	virtual std::list< tNode* >& GetChildBuf(VOID)						{ return m_ChildBuf; }
	// ���ظ��ڵ�
	virtual tNode* GetParent(VOID)										{ return m_pParentNode; }
	// �ڵ��ʼ��
	virtual VOID Initial(VOID*)											{ }
	// �߼���ѭ����
	virtual VOID Tick(VOID);

	// �����ӽڵ�,���û���ҵ�,����NULL;
	virtual tNode*	LookUpChild(LPCTSTR szNodeName);

	// ����ӽڵ㵽�ýڵ���
	virtual VOID AddChild(tNode *pNewNode);

	// ɾ��ĳ���ӽڵ�,���ýڵ����ʱ����TRUE,���򷵻�FALSE
	virtual BOOL EraseChild(tNode* pNode);

	// ɾ�������ӽڵ�
	virtual VOID EraseAllChild( VOID );

	// �ͷ��Լ�����ӵ�е���Դ
	virtual VOID Release(VOID);

protected:

	typedef std::list<tNode*> ChildList;

	// �ڵ���
	STRING				m_strName;
	// �ӽڵ��б�
	ChildList			m_ChildBuf;
	// ���ڵ�ָ��
	tNode*				m_pParentNode;


	friend struct tClass;

	KL_DECLARE_DYNAMIC(tNode);

};	// class