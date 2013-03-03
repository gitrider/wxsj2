/********************************************************************
	created:	2008/01/02
	created:	2:1:2008   10:11
	filename: 	MapServer\Server\Obj\Visitor\InitMonsterVisitor.h
	file path:	MapServer\Server\Obj\Visitor
	file base:	InitMonsterVisitor
	file ext:	h
	author:		Richard
	
	purpose:	������г�ʼ��������ʱ����������Եķ�������

				�����ǽ�ɫ����������ϵ�еľ��������֮һ���������ж�̬�������Եļ���

				���������Ľ��ֱ�ӷ�������Ӧ�Ĺ�����
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
	// ���������Ͳ�ѯ
	// ���ط���������
	//************************************
	virtual CharacterVisitor::VisitorType GetType();

	//************************************
	// Method:    VisitInitMonster
	// FullName:  InitMonsterVisitor::VisitInitMonster
	// Access:    virtual public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Character * pChar   �������������ָ��
	// Parameter: const _OBJ_INIT * pInit �������ߵĳ�ʼ�����ݶ���
	// �����ʼ�������ߵķ��ʽӿ�
	//************************************
	virtual VOID VisitInitMonster( Obj_Character* pChar );
private:
	//************************************
	// Method:    InitMinDamagePercent
	// FullName:  InitMonsterVisitor::InitMinDamagePercent
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ���������С��Ѫ�ٷֱ�
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
	// ��ʼ������Ļ���������
	//************************************
	VOID InitBaseExp();
	//************************************
	// Method:    InitAttackNear
	// FullName:  InitMonsterVisitor::InitAttackNear
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ�����̹���
	//************************************
	VOID InitAttackNear();
	//************************************
	// Method:    InitAttackFar
	// FullName:  InitMonsterVisitor::InitAttackFar
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ��Զ�̹������
	//************************************
	VOID InitAttackFar();
	//************************************
	// Method:    InitAttackMagicNear
	// FullName:  InitMonsterVisitor::InitAttackMagicNear
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ�������ڹ��������
	//************************************
	VOID InitAttackMagicNear();
	VOID InitAttackMagicFar();
	//************************************
	// Method:    InitDefenceNear
	// FullName:  InitMonsterVisitor::InitDefenceNear
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ�����̷���
	//************************************
	VOID InitDefenceNear();
	//************************************
	// Method:    InitDefenceFar
	// FullName:  InitMonsterVisitor::InitDefenceFar
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ��Զ�̷���
	//************************************
	VOID InitDefenceFar();
	//************************************
	// Method:    InitDefenceMagicNear
	// FullName:  InitMonsterVisitor::InitDefenceMagicNear
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ�������ڹ�����
	//************************************
	VOID InitDefenceMagicNear();
	VOID InitDefenceMagicFar();
	//************************************
	// Method:    InitMaxHP
	// FullName:  InitMonsterVisitor::InitMaxHP
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ��HP����
	//************************************
	VOID InitMaxHP();
	//************************************
	// Method:    InitMaxMP
	// FullName:  InitMonsterVisitor::InitMaxMP
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ��MP����
	//************************************
	VOID InitMaxMP();
	//************************************
	// Method:    InitRestoreHP
	// FullName:  InitMonsterVisitor::InitRestoreHP
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ��HP�ظ�
	//************************************
	VOID InitRestoreHP();
	//************************************
	// Method:    InitRestoreMP
	// FullName:  InitMonsterVisitor::InitRestoreMP
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ��MP�ظ�
	//************************************
	VOID InitRestoreMP();
	//************************************
	// Method:    InitHitRate
	// FullName:  InitMonsterVisitor::InitHitRate
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ��������
	//************************************
	VOID InitHitRate();
	//************************************
	// Method:    InitMissRete
	// FullName:  InitMonsterVisitor::InitMissRete
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ��������
	//************************************
	VOID InitMissRete();
	//************************************
	// Method:    InitCritRate
	// FullName:  InitMonsterVisitor::InitCritRate
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ��������
	//************************************
	VOID InitCritRate();
	//************************************
	// Method:    InitAttrAttack1
	// FullName:  InitMonsterVisitor::InitAttrAttack1
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ�����Թ���1
	//************************************
	VOID InitAttrAttack1();
	//************************************
	// Method:    InitAttrAttack2
	// FullName:  InitMonsterVisitor::InitAttrAttack2
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ�����Թ���2
	//************************************
	VOID InitAttrAttack2();
	//************************************
	// Method:    InitAttrAttack3
	// FullName:  InitMonsterVisitor::InitAttrAttack3
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ�����Թ���3
	//************************************
	VOID InitAttrAttack3();
	//************************************
	// Method:    InitAttrAttack4
	// FullName:  InitMonsterVisitor::InitAttrAttack4
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ�����Թ���4
	//************************************
	VOID InitAttrAttack4();
	VOID InitAttrAttack5();
	//************************************
	// Method:    InitAttrAttack1
	// FullName:  InitMonsterVisitor::InitAttrAttack1
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ�����Է���1
	//************************************
	VOID InitAttrDefence1();
	//************************************
	// Method:    InitAttrAttack2
	// FullName:  InitMonsterVisitor::InitAttrAttack2
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ�����Է���2
	//************************************
	VOID InitAttrDefence2();
	//************************************
	// Method:    InitAttrAttack3
	// FullName:  InitMonsterVisitor::InitAttrAttack3
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ�����Է���3
	//************************************
	VOID InitAttrDefence3();
	//************************************
	// Method:    InitAttrAttack4
	// FullName:  InitMonsterVisitor::InitAttrAttack4
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ�����Է���4
	//************************************
	VOID InitAttrDefence4();
	VOID InitAttrDefence5();
	//************************************
	// Method:    InitWeaponID
	// FullName:  InitMonsterVisitor::InitWeaponID
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ������ID
	//************************************
	VOID InitWeaponID();
	//************************************
	// Method:    InitHorseID
	// FullName:  InitMonsterVisitor::InitHorseID
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ��ʼ����ID
	//************************************
	VOID InitHorseID();

	//************************************
	// Method:    MakeRand
	// FullName:  InitMonsterVisitor::MakeRand
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: INT iMax ���������
	// ����0��iMax֮��������
	//************************************
	INT MakeRand( INT iMax );
private:
	//����ʼ���Ĺ���
	Obj_Monster*		m_pChar;
	//����2�����Ա�
	MONSTER_EXT_ATTR*	m_pAttr;
};

#endif // InitMonsterVisitor_h__