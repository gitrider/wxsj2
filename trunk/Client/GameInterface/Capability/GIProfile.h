
/** Ч�ʼ����
 */

#pragma once


#include "GIBasicType.h"
#include <hash_set>
#include <hash_map>



/////////////////////////////////////////////////////////////
//ʹ�ú�
// 
//		PROFILE_PUSH(name1);
//			PROFILE_PUSH(name2)
//			PROFILE_POP();
//		PROFILE_POP();


//�� �ڶ���Push�ڵ�
#define PROFILE_PUSH(name) { \
	KLProfile_PushNode(#name, NULL); }

#define PROFILE_PUSHEX(name, nameEx) {\
	KLProfile_PushNode(#name, nameEx); }

//�� �Ӷ���Pop�ڵ�
#define PROFILE_POP(name) { \
KLProfile_PopNode(#name); }

/////////////////////////////////////////////////////////////


struct RPOFILE_TREE_NODE;
//��ջ�еĽڵ�(����Push/Pop)
struct KYLIN_API PROFILE_STACK_NODE
{
	//����
	STRING				strName;
	//��ʼʱ��
	LARGE_INTEGER		nStartTime;
	//�����νڵ��ϵ�ָ��
	RPOFILE_TREE_NODE*	pTreeNode;

	PROFILE_STACK_NODE(LPCTSTR szName=NULL, LPCTSTR szAttName=NULL);
};

//��ջ
struct PROFILE_STACK
{
	enum 
	{ 
		MAX_STATCK_NUM = 256,
	};

	//�ڵ��(MAX_STATCK_NUM)
	PROFILE_STACK_NODE 	vNodeStack[MAX_STATCK_NUM];
	//ջ��ָ��,��ʼ0
	INT					nTopIndex;
	//��ǰջ�й����Ľڵ����Ƶ�Hash����

#ifdef __SGI_STL_PORT
	typedef std::hash_set< STRING > HASH_NAMESET;
#else
	typedef stdext::hash_set< STRING > HASH_NAMESET;
#endif

	HASH_NAMESET  vHashName;
};

//------------------------------

// ���νڵ�(����ͳ��)
struct RPOFILE_TREE_NODE
{
#ifdef __SGI_STL_PORT
	typedef std::hash_map< STRING, RPOFILE_TREE_NODE* > HASH_TREENODE;
#else
	typedef stdext::hash_map< STRING, RPOFILE_TREE_NODE* > HASH_TREENODE;
#endif
	// ����
	STRING				strName;
	// ���д���
	UINT				nInvokeCount;
	// �ӽڵ�
	HASH_TREENODE		mapChild;
	// ���ڵ�
	RPOFILE_TREE_NODE*	pParentNode;
	// ռ�õ�ʱ��
	LARGE_INTEGER		nTotalTime;

	~RPOFILE_TREE_NODE();
};

// �ڵ���
struct PROFILE_TREE
{
	// ���ڵ�
	RPOFILE_TREE_NODE	theRoot;
	// ��ǰ�ڵ�
	RPOFILE_TREE_NODE*	pCurrentNode;
};

// ��ѯ�ڵ�������ָ��
typedef VOID (__stdcall* FUNC_PROFILE_DUMP)(const PROFILE_TREE* pProfileNodeTree);

extern "C"
{
	// �ڶ���Push�ڵ�
	VOID	KYLIN_API KLProfile_PushNode(LPCTSTR name, LPCTSTR nameEx = NULL);
	// �Ӷ���Pop�ڵ�
	VOID	KYLIN_API KLProfile_PopNode(LPCTSTR name=NULL);
	// �����ѯProfile�ڵ���Ϣ(����ջ��ʱ����ô���ĺ���ָ��)
	VOID	KYLIN_API KLProfile_DumpSruct(FUNC_PROFILE_DUMP func);
}

// ��ʱ�����������!!!
VOID	KLProfile_Tick(VOID);

