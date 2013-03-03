/*************************************************************************
 �ļ���	: 	AIScriptDef.h
 �汾�� :	0.0.1
 ������	:	FrankWu
 ��  ��	:	AIScript�õ������ݽṹ����
 �޸ļ�¼:	
 *************************************************************************/


#ifndef _AISCRIPTDEF_H_
#define _AISCRIPTDEF_H_

#include "AIScriptFunction.h"
#include <vector>
#include "State.h"

using namespace std;

#define		SET_FLAG_DOWN(flag,index)	flag = ( flag | 1<<(31-index)) 
#define		RESET_FLAG_DOWN(flag,index)	flag = ( flag & ~(1<<(31-index))) 
#define		GET_FLAG_DOWN(flag,index)	( ((flag)>>(31-index)) & 0x00000001 )

#define		MAX_STACK_NUM  (1024)
#define		NORMAL_AI_PRI  (20)
#define		MAX_TIMES_NUM  (7)
#define		MAX_ID_NUM	   (10)

enum
{
	ANDOROP = 0, //&,|,(,)
	RELATIONOP,	 //=,>,<,>=,<=	
	TODOOP,		 //{,},;,','
	PARAMOP,	 //[,]
	OTHEROP,	 //�����ַ�
};

enum 
{
	OPERATOR = 0,
	CDTEXPRESSION,
	TODOEXPRESSION,

};

// ��ϵ���������
enum REL_OPERATOR
{
	OPTEQUAL = 0 ,
	OPTUNEQUAL ,
	OPTLESSTHAN ,
	OPTLARGETHAN ,
	OPTLESSEQUAL ,
	OPTLARGEEQUAL ,
	OPTAND ,
	OPTOR ,
	OPTROOT ,

};

enum ENUM_AISTATE
{
	// { Ϊ�˼���ԭ���ĺ궨��--���Ѿ����úõ�scriptxxx.ai�ļ����
	SIDLE			= ESTATE_IDLE,			//����״̬
	SAPPROACH		= ESTATE_APPROACH,		//����״̬
	SATTACK			= ESTATE_COMBAT,		//����״̬
	SFOLLOW			= ESTATE_TEAMFOLLOW,	//����״̬
	SRETURN			= ESTATE_GOHOME,		//����׷��
	SPATROL			= ESTATE_PATROL,		//Ѳ��	
	SFLEE			= ESTATE_FLEE,			//����״̬
	// }

	SKILLSECTION	= ESTATE_NUMBER+1,		//���ܶ�
	ONBESKILLSECTION,	//��������
	ONDAMAGESECTION,	//�ܵ��˺���
	ONDEADSECTION,		//������

	// { section of disuse...
	//SCHANGEENEMY,		//�ı����״̬
	//SWAIT,			//�ȴ�״̬
	//SFASTFLEE,		//��������״̬
	//SARRIVE,			//�ѵ���Ŀ�ĵ�״̬
	//SUSESKILL,		//ʹ�ü���״̬
	//����״̬
	//YUN,				//������
	//ZHONGDU,			//�����ж�
	//DEAD,				//��������
	//SLEEP,			//����˯��
	//����˵������
	//HAPPY,			//˵������--ϲ
	//ANGRY,			//˵������--ŭ
	//SORROW,			//˵������--��
	//ENJOY,			//˵������--��
	//���﹥��������
	//PHYSICSKILL,		//������
	//MAGICSKILL,		//ħ������
	//ASSISTSKILL,		//��������
	// }

	ERRORCMD,			//��������
};

struct BracketArray
{
	INT			array[10];
	INT			count;
	BracketArray()
	{
		memset(array,0,sizeof(array));
		count = 0;
	}
};

struct Atom
{
	CHAR		szCHAR[32];
	INT			param[3];
	INT			pos;
	INT			PRI;
	Atom()
	{
        memset(szCHAR,0,sizeof(szCHAR));
		memset( param, -1, sizeof(param) );
		pos = -1;
		PRI = -1;
	}
};

struct TreeNode
{
	PLAISFunc 	FuncPtr;
	INT			op;
	INT			param[3];
	INT			value; 
	INT			pos;
	INT			PRI;
	TreeNode()
		:FuncPtr(0),
		op(OPTROOT),
		value(-1),
		PRI(-1),
		pos(-1)
	{
		memset( param, -1, sizeof(param) );
	}
};

struct TreeNodeList 
{
	TreeNode			Node;
	TreeNodeList*		pNext;
	TreeNodeList()
		:pNext(0)
	{}
};

struct ConditionTree
{
	TreeNode			Node;
	ConditionTree* 		pParent;
	ConditionTree* 		pLeft;
	ConditionTree* 		pRight;	
	ConditionTree()
		:pLeft(0),
		pRight(0)
	{};
};

struct ConditionNode
{
	DWORD				flag;
	//flag��ÿλ�ĺ������£�
	//	00000000,00000000,00000000,00000000
	//	||
	//	||_�������ڵ�ִ�й��� 
	//	|__ �������ڵ�ִ������
	//   

	INT 				id;
	INT 				PRI;
	INT					times;
	INT					times2;
	ConditionTree* 		pRootCondition;
	TreeNodeList*		pToDo;
	ConditionNode()
		:flag(0),
		id(-1),
		PRI(-1),
		times(0),
		times2(0),
		pRootCondition(0),
		pToDo(0)
	{}
	VOID ResetTimes(VOID)
	{
		times = times2;
	}
	VOID SetDownFlag(INT index)
	{
		SET_FLAG_DOWN(flag,index);
	}
	INT GetDownFlag(INT index)
	{
		return GET_FLAG_DOWN(flag,index);
	}
	VOID ReSetDownFlag(INT index)
	{
		RESET_FLAG_DOWN(flag,index);
	}
	VOID ReSetAllDownFlags(VOID)
	{
		for(INT i=0;32>i;++i)
			ReSetDownFlag(i);
	}
};

struct ConditionList
{
	ConditionNode*		pNode;
	ConditionList*		pNext;
	ConditionList()
		:pNode(0),
		pNext(0)
	{}
};

struct TODOTIMES
{
	INT current_times;
	INT total_times;
	TODOTIMES()
	{
		current_times = 0;
		total_times = 1;
	}
};

typedef ConditionTree* Elem;



typedef vector<Atom> OperatorQueue;
typedef vector<Atom> CdtExpressionQueue;
typedef vector<Atom> ToDoExpressionQueue;

class Stack
{
public:
	Stack()
	{
		memset(elem, 0, MAX_STACK_NUM);
		top = -1;
	}
	~Stack()
	{
	}
	VOID push(Elem* p)
	{
		if (++top < MAX_STACK_NUM)
		{
			elem[top] = p;
		}
		else
		{
			Assert(NULL && "++top<MAX_STACK_NUM");
		}
	}
	Elem* pop()
	{
		if (top != -1 && top >= 0)
		{
			return elem[top--];
		}

		return NULL;
	}
	bool isEmpty()
	{
		return top==-1 ? true : false;
	}
	void Reset()
	{
		memset(elem, 0, MAX_STACK_NUM);
		top = -1;
	}

private:
	Elem*	elem[MAX_STACK_NUM];
	INT		top;

};

#endif