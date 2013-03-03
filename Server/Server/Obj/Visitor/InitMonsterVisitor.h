/********************************************************************
	created:	2008/01/02
	created:	2:1:2008   10:11
	filename: 	MapServer\Server\Obj\Visitor\InitMonsterVisitor.h
	file path:	MapServer\Server\Obj\Visitor
	file base:	InitMonsterVisitor
	file ext:	h
	author:		Richard
	
	purpose:	怪物进行初始化或重生时计算怪物属性的访问者类

				此类是角色访问者类体系中的具体访问者之一，负责所有动态怪物属性的计算

				并将计算后的结果直接反馈给相应的怪物类
*********************************************************************/

#ifndef InitMonsterVisitor_h__
#define InitMonsterVisitor_h__

#include "CharacterVisitor.h"

class Obj_Character;
class Obj_Monster;
struct _OBJ_INIT;
struct MONSTER_EXT_ATTR;

class InitMonsterVisitor : public CharacterVisitor
{
public:
	InitMonsterVisitor();
	virtual ~InitMonsterVisitor ();
public:
	//************************************
	// Method:    GetType
	// FullName:  InitMonsterVisitor::GetType
	// Access:    virtual private 
	// Returns:   CharacterVisitor::VisitorType
	// Qualifier:
	// 访问者类型查询
	// 返回访问者类型
	//************************************
	virtual CharacterVisitor::VisitorType GetType();

	//************************************
	// Method:    VisitInitMonster
	// FullName:  InitMonsterVisitor::VisitInitMonster
	// Access:    virtual public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Character * pChar   被访问者自身的指针
	// Parameter: const _OBJ_INIT * pInit 被访问者的初始化数据对象
	// 怪物初始化访问者的访问接口
	//************************************
	virtual VOID VisitInitMonster( Obj_Character* pChar );
private:
	//************************************
	// Method:    InitMinDamagePercent
	// FullName:  InitMonsterVisitor::InitMinDamagePercent
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化怪物的最小伤血百分比
	//************************************
	VOID InitMinDamagePercent();
	//************************************
	// Method:    InitBaseExp
	// FullName:  InitMonsterVisitor::InitBaseExp
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: MONSTER_EXT_ATTR * pAttr
	// Parameter: Obj_Character * pChar
	// 初始化怪物的基础经验获得
	//************************************
	VOID InitBaseExp();
	//************************************
	// Method:    InitAttackNear
	// FullName:  InitMonsterVisitor::InitAttackNear
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化近程攻击
	//************************************
	VOID InitAttackNear();
	//************************************
	// Method:    InitAttackFar
	// FullName:  InitMonsterVisitor::InitAttackFar
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化远程攻击获得
	//************************************
	VOID InitAttackFar();
	//************************************
	// Method:    InitAttackMagicNear
	// FullName:  InitMonsterVisitor::InitAttackMagicNear
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化近程内功攻击获得
	//************************************
	VOID InitAttackMagicNear();
	VOID InitAttackMagicFar();
	//************************************
	// Method:    InitDefenceNear
	// FullName:  InitMonsterVisitor::InitDefenceNear
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化近程防御
	//************************************
	VOID InitDefenceNear();
	//************************************
	// Method:    InitDefenceFar
	// FullName:  InitMonsterVisitor::InitDefenceFar
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化远程防御
	//************************************
	VOID InitDefenceFar();
	//************************************
	// Method:    InitDefenceMagicNear
	// FullName:  InitMonsterVisitor::InitDefenceMagicNear
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化近程内功防御
	//************************************
	VOID InitDefenceMagicNear();
	VOID InitDefenceMagicFar();
	//************************************
	// Method:    InitMaxHP
	// FullName:  InitMonsterVisitor::InitMaxHP
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化HP上限
	//************************************
	VOID InitMaxHP();
	//************************************
	// Method:    InitMaxMP
	// FullName:  InitMonsterVisitor::InitMaxMP
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化MP上限
	//************************************
	VOID InitMaxMP();
	//************************************
	// Method:    InitRestoreHP
	// FullName:  InitMonsterVisitor::InitRestoreHP
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化HP回复
	//************************************
	VOID InitRestoreHP();
	//************************************
	// Method:    InitRestoreMP
	// FullName:  InitMonsterVisitor::InitRestoreMP
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化MP回复
	//************************************
	VOID InitRestoreMP();
	//************************************
	// Method:    InitHitRate
	// FullName:  InitMonsterVisitor::InitHitRate
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化命中率
	//************************************
	VOID InitHitRate();
	//************************************
	// Method:    InitMissRete
	// FullName:  InitMonsterVisitor::InitMissRete
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化闪避率
	//************************************
	VOID InitMissRete();
	//************************************
	// Method:    InitCritRate
	// FullName:  InitMonsterVisitor::InitCritRate
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化会心率
	//************************************
	VOID InitCritRate();
	//************************************
	// Method:    InitAttrAttack1
	// FullName:  InitMonsterVisitor::InitAttrAttack1
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化属性攻击1
	//************************************
	VOID InitAttrAttack1();
	//************************************
	// Method:    InitAttrAttack2
	// FullName:  InitMonsterVisitor::InitAttrAttack2
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化属性攻击2
	//************************************
	VOID InitAttrAttack2();
	//************************************
	// Method:    InitAttrAttack3
	// FullName:  InitMonsterVisitor::InitAttrAttack3
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化属性攻击3
	//************************************
	VOID InitAttrAttack3();
	//************************************
	// Method:    InitAttrAttack4
	// FullName:  InitMonsterVisitor::InitAttrAttack4
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化属性攻击4
	//************************************
	VOID InitAttrAttack4();
	VOID InitAttrAttack5();
	//************************************
	// Method:    InitAttrAttack1
	// FullName:  InitMonsterVisitor::InitAttrAttack1
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化属性防御1
	//************************************
	VOID InitAttrDefence1();
	//************************************
	// Method:    InitAttrAttack2
	// FullName:  InitMonsterVisitor::InitAttrAttack2
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化属性防御2
	//************************************
	VOID InitAttrDefence2();
	//************************************
	// Method:    InitAttrAttack3
	// FullName:  InitMonsterVisitor::InitAttrAttack3
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化属性防御3
	//************************************
	VOID InitAttrDefence3();
	//************************************
	// Method:    InitAttrAttack4
	// FullName:  InitMonsterVisitor::InitAttrAttack4
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化属性防御4
	//************************************
	VOID InitAttrDefence4();
	VOID InitAttrDefence5();
	//************************************
	// Method:    InitWeaponID
	// FullName:  InitMonsterVisitor::InitWeaponID
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化武器ID
	//************************************
	VOID InitWeaponID();
	//************************************
	// Method:    InitHorseID
	// FullName:  InitMonsterVisitor::InitHorseID
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 初始化马ID
	//************************************
	VOID InitHorseID();

	//************************************
	// Method:    MakeRand
	// FullName:  InitMonsterVisitor::MakeRand
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: INT iMax 随机的上限
	// 产生0至iMax之间的随机数
	//************************************
	INT MakeRand( INT iMax );
private:
	//待初始化的怪物
	Obj_Monster*		m_pChar;
	//怪物2级属性表
	MONSTER_EXT_ATTR*	m_pAttr;
};

#endif // InitMonsterVisitor_h__