/********************************************************************
	created:	2008/01/02
	created:	2:1:2008   10:15
	filename: 	MapServer\Server\Obj\Visitor\InitMonsterVisitor.cpp
	file path:	MapServer\Server\Obj\Visitor
	file base:	InitMonsterVisitor
	file ext:	cpp
	author:		Richard
	
	purpose:	怪物进行初始化或重生时计算怪物属性的访问者类

				此类是角色访问者类体系中的具体访问者之一，负责所有动态怪物属性的计算

				并将计算后的结果直接反馈给相应的怪物类
*********************************************************************/
#include "stdafx.h"
#include "InitMonsterVisitor.h"
#include "Obj_Character.h"
#include "Obj_Monster.h"
#include "GameTable.h"
#include "Scene.h"
#include "AI_Monster.h"

InitMonsterVisitor::InitMonsterVisitor()
{
	m_pChar = NULL;
}


InitMonsterVisitor::~InitMonsterVisitor()
{

}

CharacterVisitor::VisitorType InitMonsterVisitor::GetType()
{
	return CharacterVisitor_MonsterAttr;
}

VOID InitMonsterVisitor::VisitInitMonster( Obj_Character* pChar )
{
	__ENTER_FUNCTION
	m_pChar = (Obj_Monster*)pChar;

	MONSTER_EXT_ATTR *pAttr = g_MonsterAttrExTbl.GetExtAttr( m_pChar->GetDataID() );
	if ( pAttr == NULL )
		return;
	
	m_pAttr = pAttr;

	InitMinDamagePercent();
	InitBaseExp();
	InitAttackNear();
	InitAttackFar();
	InitAttackMagicNear();
	InitAttackMagicFar();
	InitDefenceNear();
	InitDefenceFar();
	InitDefenceMagicNear();
	InitDefenceMagicFar();
	InitMaxHP();
	InitMaxMP();
	InitRestoreHP();
	InitRestoreMP();
	InitHitRate();
	InitCritRate();
	InitMissRete();
	InitAttrAttack1();
	InitAttrAttack2();
	InitAttrAttack3();
	InitAttrAttack4();
	InitAttrAttack5();
	InitAttrDefence1();
	InitAttrDefence2();
	InitAttrDefence3();
	InitAttrDefence4();
	InitAttrDefence5();
	InitWeaponID();
	InitHorseID();
	__LEAVE_FUNCTION
}

//初始化怪物的最小伤血百分比
VOID InitMonsterVisitor::InitMinDamagePercent()
{
	__ENTER_FUNCTION

	//在此添加怪物每次复生时需要计算的属性相关的代码
	if( m_pAttr->m_MinDamagePercent == -1 )
	{
		//公式
	}
	else
	{
		m_pChar->SetMinDamagePercent( m_pAttr->m_MinDamagePercent );
	}
	__LEAVE_FUNCTION
}

//初始化怪物的基础经验获得
VOID InitMonsterVisitor::InitBaseExp()
{
	__ENTER_FUNCTION

	if( m_pAttr->m_BaseExp == -1 )
	{
		//公式
	}
	else
	{
		m_pChar->SetBaseExp( m_pAttr->m_BaseExp );
	}
	__LEAVE_FUNCTION

}

//初始化近程攻击
VOID InitMonsterVisitor::InitAttackNear()
{
	__ENTER_FUNCTION
	INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_ATTACK_NEAR );
	if( iTmp == -1 )
	{
		//公式
	}
	else
	{
		m_pChar->SetBaseAttackNear( iTmp );
	}
	__LEAVE_FUNCTION
}

// 远程攻击获得
VOID InitMonsterVisitor::InitAttackFar()
{
	__ENTER_FUNCTION
	INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_ATTACK_FAR );
	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseAttackFar( iTmp );
	}

	__LEAVE_FUNCTION

}

//初始化近程内功攻击获得
VOID InitMonsterVisitor::InitAttackMagicNear()
{
	__ENTER_FUNCTION
		INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_ATTACKRATE_MAGIC_NEAR );
	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseAttackMagicNear( iTmp );
	}
	__LEAVE_FUNCTION
}
VOID InitMonsterVisitor::InitAttackMagicFar()
{
	__ENTER_FUNCTION
	INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_ATTACKRATE_MAGIC_FAR);
	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseAttackMagicFar( iTmp );
	}
	__LEAVE_FUNCTION
}

//初始化近程防御
VOID InitMonsterVisitor::InitDefenceNear()
{
	__ENTER_FUNCTION
	INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_DEFENCE_NEAR );
	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseDefenceNear( iTmp );
	}
	__LEAVE_FUNCTION

}

//初始化远程防御
VOID InitMonsterVisitor::InitDefenceFar()
{
	__ENTER_FUNCTION
	INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_DEFENCE_FAR );
	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseDefenceFar( iTmp );
	}
	__LEAVE_FUNCTION
}

//初始化近程内功防御
VOID InitMonsterVisitor::InitDefenceMagicNear()
{
	__ENTER_FUNCTION
	INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_DEFENCE_MAGIC_NEAR);
	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseDefenceMagicNear( iTmp );
	}
	__LEAVE_FUNCTION
}
//初始化近程外功防御
VOID InitMonsterVisitor::InitDefenceMagicFar()
{
	__ENTER_FUNCTION
	INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_DEFENCE_MAGIC_FAR);
	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseDefenceMagicFar( iTmp );
	}
	__LEAVE_FUNCTION
}

//初始化HP上限
VOID InitMonsterVisitor::InitMaxHP()
{
	__ENTER_FUNCTION
	INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_MAXHP );
	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseMaxHP( iTmp );
	}
	__LEAVE_FUNCTION
}

//初始化MP上限
VOID InitMonsterVisitor::InitMaxMP()
{
	__ENTER_FUNCTION
	INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_MAXMP );

	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseMaxMp( iTmp );
	}
	__LEAVE_FUNCTION
}

//初始化HP回复
VOID InitMonsterVisitor::InitRestoreHP()
{
	__ENTER_FUNCTION
	INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_RESTOREHP );

	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseHPRegenerate( iTmp );
	}
	__LEAVE_FUNCTION
}

//初始化MP回复
VOID InitMonsterVisitor::InitRestoreMP()
{
	__ENTER_FUNCTION
	INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_RESTOREMP );

	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseMPRegenerate( iTmp );
	}
	__LEAVE_FUNCTION
}

//初始化命中率
VOID InitMonsterVisitor::InitHitRate()
{
	__ENTER_FUNCTION
	INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_HIT );

	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseHit( iTmp );
	}
	__LEAVE_FUNCTION
}

//初始化闪避率
VOID InitMonsterVisitor::InitMissRete()
{
	__ENTER_FUNCTION
	INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_MISS );

	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseMiss( iTmp );
	}	
	__LEAVE_FUNCTION
}

//初始化会心率
VOID InitMonsterVisitor::InitCritRate()
{
	__ENTER_FUNCTION
	INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_CRITRATE );

	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseCritical( iTmp );
	}	
	__LEAVE_FUNCTION
}

//初始化属性攻击1
VOID InitMonsterVisitor::InitAttrAttack1()
{
	__ENTER_FUNCTION
	INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_ATTACK_GOLD);

	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseAttackGold( iTmp );
	}	
	__LEAVE_FUNCTION
}

//初始化属性攻击2
VOID InitMonsterVisitor::InitAttrAttack2()
{
	__ENTER_FUNCTION
		INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_ATTACK_WOOD);

	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseAttackWood( iTmp );
	}
	__LEAVE_FUNCTION
}

//初始化属性攻击3
VOID InitMonsterVisitor::InitAttrAttack3()
{
	__ENTER_FUNCTION
		INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_ATTACK_WATER);

	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseAttackWater( iTmp );
	}
	__LEAVE_FUNCTION
}

//初始化属性攻击4
VOID InitMonsterVisitor::InitAttrAttack4()
{
	__ENTER_FUNCTION
	INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_ATTACK_FIRE);

	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseAttackFire( iTmp );
	}
	__LEAVE_FUNCTION
}

//初始化属性攻击5
VOID InitMonsterVisitor::InitAttrAttack5()
{
	__ENTER_FUNCTION
		INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_ATTACK_SOIL);

	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseAttackSoil( iTmp );
	}
	__LEAVE_FUNCTION
}

//初始化属性防御1
VOID InitMonsterVisitor::InitAttrDefence1()
{
	__ENTER_FUNCTION
	INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_DEFENCE_GOLD);

	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseDefenceGold( iTmp );
	}
	__LEAVE_FUNCTION
}

//初始化属性防御2
VOID InitMonsterVisitor::InitAttrDefence2()
{
	__ENTER_FUNCTION
		INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_DEFENCE_WOOD);

	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseDefenceWood( iTmp );
	}
	__LEAVE_FUNCTION
}

//初始化属性防御3
VOID InitMonsterVisitor::InitAttrDefence3()
{
	__ENTER_FUNCTION
		INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_DEFENCE_WATER);

	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseDefenceWater( iTmp );
	}
	__LEAVE_FUNCTION
}

//初始化属性防御4
VOID InitMonsterVisitor::InitAttrDefence4()
{
	__ENTER_FUNCTION
		INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_DEFENCE_FIRE );

	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseDefenceFire( iTmp );
	}
	__LEAVE_FUNCTION
}

//初始化属性防御5
VOID InitMonsterVisitor::InitAttrDefence5()
{
	__ENTER_FUNCTION
		INT iTmp = g_MonsterAttrExTbl.Get(m_pChar->GetDataID(), CATTR_LEVEL2_DEFENCE_SOIL);

	if( iTmp == -1 )
	{

	}
	else
	{
		m_pChar->SetBaseDefenceSoil( iTmp );
	}
	__LEAVE_FUNCTION
}

//初始化武器ID
VOID InitMonsterVisitor::InitWeaponID()
{
	__ENTER_FUNCTION
	INT iWeaponCount	= m_pAttr->m_iWeaponCount;
	//计算mon_wqskl.tab表中的索引号
	INT iRand			= MakeRand( iWeaponCount - 1 );
	//索引号超过最大值的保护
	//取得索引号
	iRand				= m_pAttr->m_WeaponID[iRand];
	if( iRand >= MonsterWeaponAndSkillTable::MWAST_MAX_NUM )
	{
		iRand = MonsterWeaponAndSkillTable::MWAST_MAX_NUM - 1;
	}
	//从mon_wqskl.tab表中取得武器ID
	INT iWeaponID	= g_MonsterWeaponAndSkill.m_aWeaponSkill[iRand].m_iWeaponID;
	//设置武器ID
	m_pChar->SetWeaponID( iWeaponID );
	Assert(g_MonsterWeaponAndSkill.m_aWeaponSkill[iRand].m_iSkillLevelIndexCount<=MonsterWeaponAndSkill::MWAS_MAX_LEVEL && "mon_wqskl.tab 技能lv 列数值超出上限");
	INT iSkillRand	= MakeRand( g_MonsterWeaponAndSkill.m_aWeaponSkill[iRand].m_iSkillLevelIndexCount - 1 );
	iSkillRand		= g_MonsterWeaponAndSkill.m_aWeaponSkill[iRand].m_aSkillLevelIndex[iSkillRand];
	//设置怪物使用的技能
	AI_Monster* pMonsterAI = m_pChar->GetMonsterAI();
	if( m_pChar->GetAIScript() != INVALID_ID )
		return;

	if( pMonsterAI != NULL )
	{
		pMonsterAI->GetScriptFile( g_MonsterWeaponAndSkill.m_aWeaponSkill[iRand].m_aSkillLevel[iSkillRand] );
		m_pChar->SetAIScript( g_MonsterWeaponAndSkill.m_aWeaponSkill[iRand].m_aSkillLevel[iSkillRand] );
	}
	__LEAVE_FUNCTION
}

//初始化马ID
VOID InitMonsterVisitor::InitHorseID()
{
	__ENTER_FUNCTION
	INT iHorseCount	= m_pAttr->m_iHorseCount;
	INT iRand		= MakeRand( iHorseCount - 1 );
	m_pChar->SetHorseID( m_pAttr->m_HorseID[iRand] );
	__LEAVE_FUNCTION
}

INT InitMonsterVisitor::MakeRand( INT iMax )
{
	__ENTER_FUNCTION

	Scene * pScene = m_pChar->getScene();

	if(NULL==pScene)
		return 0;

	FLOAT fRand = (FLOAT)(pScene->GetRand100()) / 100.0f;

	fRand *= iMax;
	
	return (INT)(fRand+0.5f);
	__LEAVE_FUNCTION
	return 0;
}
