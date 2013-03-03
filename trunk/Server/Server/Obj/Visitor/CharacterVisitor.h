/********************************************************************
	created:	2008/01/02
	created:	2:1:2008   9:58
	filename: 	MapServer\Server\Obj\Visitor\CharacterVisitor.h
	file path:	MapServer\Server\Obj\Visitor
	file base:	CharacterVisitor
	file ext:	h
	author:		Richard
	
	purpose:	实现角色类复杂操作的访问者基类，所有具体访问者从此类派生
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
		//无效访问者
		CharacterVisitor_None,
		//怪物初始化时属性计算的访问者
		CharacterVisitor_MonsterAttr
	};
//构造
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
	// 访问者类型查询
	// 返回访问者类型
	//************************************
	virtual CharacterVisitor::VisitorType GetType() = 0;

	//************************************
	// Method:    VisitInitMonster
	// FullName:  CharacterVisitor::VisitInitMonster
	// Access:    virtual public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Character * pChar   被访问者自身的指针
	// Parameter: const _OBJ_INIT * pInit 被访问者的初始化数据对象
	// 怪物初始化访问者的访问接口
	//************************************
	virtual VOID VisitInitMonster( Obj_Character* pChar ){};
};



#endif // CharacterVisitor_h__