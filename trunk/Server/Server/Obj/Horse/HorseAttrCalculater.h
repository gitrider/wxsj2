/********************************************************************
	created:	2008/03/12
	created:	12:3:2008   14:47
	filename: 	MapServer\Server\Obj\HumanAssistantClass\HorseAttrCalculater.h
	file path:	MapServer\Server\Obj\HumanAssistantClass
	file base:	HorseAttrCalculater
	file ext:	h
	author:		Richard
	
	purpose:	实现马的所有属性计算相关的内容

	悟性、根骨、资质的养成依靠物品使用
	1级战斗技能，2级战斗技能在马等级提升时进行处理
*********************************************************************/

#ifndef HorseAttrCalculater_h__
#define HorseAttrCalculater_h__

#include "Type.h"
#include "DB_Struct.h"
#include "HorseStruct.h"

class Obj_Human;

class HorseAttrCalculater
{
public:
	HorseAttrCalculater();
	~HorseAttrCalculater();

public:
	VOID SetOnwer( Obj_Human* pOnwer ){ m_pOnwer = pOnwer; }
	//************************************
	// Method:    HeartBeat
	// FullName:  HorseAttrCalculater::HeartBeat
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: UINT uTime
	//************************************
	VOID HeartBeat( UINT uTime = 0 );
	VOID InitCurHappinessTime();
	//************************************
	// Method:    ResetHorse
	// FullName:  HorseAttrCalculater::ResetHorse
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// 当骑乘更换或出战时的初始化
	//************************************
	VOID ResetHorse();

	//************************************
	// Method:    OnExpChange
	// FullName:  HorseAttrCalculater::OnExpChange
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// Parameter: INT iExp 马被分配的经验值
	// 当马获得经验值以后，处理等级提升及随等级变化的属性的改变
	//************************************
	VOID OnExpChange( INT iIncExp );

	//************************************
	// Method:    SetGenGu
	// FullName:  HorseAttrCalculater::SetGenGu
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: INT iGenGu
	// 设置马的根骨,根骨变化会引起资质的变化
	//************************************
	VOID SetGenGu( INT iGenGu );

	//************************************
	// Method:    SetSavvy
	// FullName:  HorseAttrCalculater::SetSavvy
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: INT iSavvy
	// 设置马的悟性, 悟性的变化会引起资质的变化
	//************************************
	VOID SetSavvy( INT iSavvy );

private:
	//************************************
	// Method:    UpdateAttackLevel2
	// FullName:  HorseAttrCalculater::UpdateAttackLevel2
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// 当玩家获得点数的时候，给正在出战的马增加1级战斗属性点数
	//************************************
	VOID UpdateAttackLevel1();

	//************************************
	// Method:    UpdateRemainPoint
	// FullName:  HorseAttrCalculater::UpdateRemainPoint
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// 处理剩余点数
	//************************************
	VOID UpdateRemainPoint();

	//************************************
	// Method:    UpdateAttackLevel2
	// FullName:  HorseAttrCalculater::UpdateAttackLevel2
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// 给正在出战的马计算2级战斗属性
	//************************************
	VOID UpdateAttackLevel2();

	//************************************
	// Method:    UpdatePerception
	// FullName:  HorseAttrCalculater::UpdatePerception
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// 资质计算
	//************************************
	VOID UpdatePerception();

	//************************************
	// Method:    IsActive
	// FullName:  HorseAttrCalculater::IsActive
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human
	// 是否有出战的马
	//************************************
	BOOL IsActive();

	//************************************
	// Method:    SetHappiness
	// FullName:  HorseAttrCalculater::SetHappiness
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: INT iHappiness
	//************************************
	VOID SetHappiness( INT iHappiness );

	//************************************
	// Method:    SetExp
	// FullName:  HorseAttrCalculater::SetExp
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: INT iExp
	//************************************
	VOID SetExp( INT iExp );

	//************************************
	// Method:    LevelUp
	// FullName:  HorseAttrCalculater::LevelUp
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	//************************************
	VOID LevelUp();

	//************************************
	// Method:    GetLevel
	// FullName:  HorseAttrCalculater::GetLevel
	// Access:    public 
	// Returns:   INT
	// Qualifier:
	// Parameter: VOID
	//************************************
	INT	GetLevel( VOID );

	//************************************
	// Method:    GetExp
	// FullName:  HorseAttrCalculater::GetExp
	// Access:    public 
	// Returns:   INT
	// Qualifier:
	//************************************
	INT GetExp();
	
	//************************************
	// Method:    CalcPerception
	// FullName:  HorseAttrCalculater::CalcPerception
	// Access:    private 
	// Returns:   INT 经过公式计算后的结果
	// Qualifier:
	// Parameter: INT iPer 当前资质
	// 资质计算公式
	//************************************
	INT CalcPerception( INT iPer );

	//************************************
	// Method:    RefreshAttr
	// FullName:  HorseAttrCalculater::RefreshAttr
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 当属性发生改变时向client发送更新属性的消息
	//************************************
	VOID RefreshAttr();

	//************************************
	// Method:    InitBackupAttr
	// FullName:  HorseAttrCalculater::InitBackupAttr
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	//************************************
	VOID InitBackupAttr();
	//处理快乐度随时间下降
	VOID HappinessHeartBeat( UINT uTime );
	//技能领悟
	//每10级领悟一次，到60级为止
	VOID SkillRealization();
	
private:
	INT					m_nIndex;
	_PET_DB*			m_pPetDB;
	Obj_Human*			m_pOnwer;
	_HORSE_ATTR_BACKUP	m_AttrBackUp;
	UINT				m_iHappinessTimer;
};



#endif // HorseAttrCalculater_h__