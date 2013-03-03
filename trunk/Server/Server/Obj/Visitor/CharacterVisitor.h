/********************************************************************
	created:	2008/01/02
	created:	2:1:2008   9:58
	filename: 	MapServer\Server\Obj\Visitor\CharacterVisitor.h
	file path:	MapServer\Server\Obj\Visitor
	file base:	CharacterVisitor
	file ext:	h
	author:		Richard
	
	purpose:	ʵ�ֽ�ɫ�ิ�Ӳ����ķ����߻��࣬���о�������ߴӴ�������
*********************************************************************/

#ifndef CharacterVisitor_h__
#define CharacterVisitor_h__

#include "Type.h"

class Obj_Character;
struct _OBJ_INIT;

class CharacterVisitor
{
public:
	enum VisitorType
	{
		//��Ч������
		CharacterVisitor_None,
		//�����ʼ��ʱ���Լ���ķ�����
		CharacterVisitor_MonsterAttr
	};
//����
public:
	CharacterVisitor();
	virtual ~CharacterVisitor();

public:
	//************************************
	// Method:    GetType
	// FullName:  CharacterVisitor::GetType
	// Access:    virtual public 
	// Returns:   CharacterVisitor::VisitorType
	// Qualifier: /*= 0
	// ���������Ͳ�ѯ
	// ���ط���������
	//************************************
	virtual CharacterVisitor::VisitorType GetType() = 0;

	//************************************
	// Method:    VisitInitMonster
	// FullName:  CharacterVisitor::VisitInitMonster
	// Access:    virtual public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Character * pChar   �������������ָ��
	// Parameter: const _OBJ_INIT * pInit �������ߵĳ�ʼ�����ݶ���
	// �����ʼ�������ߵķ��ʽӿ�
	//************************************
	virtual VOID VisitInitMonster( Obj_Character* pChar ){};
};



#endif // CharacterVisitor_h__