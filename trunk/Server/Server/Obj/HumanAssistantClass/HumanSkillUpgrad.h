/********************************************************************
	created:	2008/01/11
	created:	11:1:2008   15:44
	filename: 	MapServer\Server\Obj\HumanAssistantClass\HumanSkillUpgrad.h
	file path:	MapServer\Server\Obj\HumanAssistantClass
	file base:	HumanSkillUpgrad
	file ext:	h
	author:		Richard
	
	purpose:	处理玩家技能学习、升级的条件判断和消耗，通过自身逻辑，

	判断学习和升级的合法性，并通过Obj_Human的相关属性访问接口直接设置

	结果

	封装所有与技能学习和升级有关的逻辑，此类会与Obj_Human产生单向关联

	HumanSkillUpgrade依赖Obj_Human所有与技能学习、升级有关属性的访问接口

	属于弱耦合关系，对技能学习和升级有关的逻辑改变只需要改变此类的逻辑，

	Obj_Human对此一无所知，不会产生连锁式的修改

	要注意避免和技能系统的强耦合关系，比如技能表格的改造不应需要修改此类，

	对技能表格及属性的访问可通过一个中介者进行，外部变化被封装在中介者内
*********************************************************************/

/********************************************************************

	HumanSkillLevelUp为模板方法，定义了判断玩家技能升级的处理步骤不可重载

	其它方法为过程的每个步骤地实现，可以重载实现其它具体实现，以改变此接口

	的行为.

*********************************************************************/


#ifndef HumanSkillUpgrad_h__
#define HumanSkillUpgrad_h__

#include "Type.h"

class Obj_Human;

class HumanSkillUpgrade
{
public:
	HumanSkillUpgrade();
	virtual ~HumanSkillUpgrade();

	//************************************
	// Method:    Cleanup
	// FullName:  HumanSkillUpgrade::Cleanup
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: VOID
	// 清除所有属性值
	//************************************
	VOID Cleanup(VOID);

public:

	//************************************
	// Method:    HumanSkillLevelUp
	// FullName:  HumanSkillUpgrade::HumanSkillLevelUp
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: const Obj_Human * pHuman 请求升级的玩家
	// Parameter: SkillID_t iSkillID 请求升级的技能ID
	// Parameter: INT iLevel 要升到多少级
	// template method 成员，不要重载
	//************************************
	BOOL HumanSkillLevelUp( const Obj_Human* pHuman, SkillID_t iSkillID, INT iLevel );

	//************************************
	// Method:    StartPassiveSkill
	// FullName:  HumanSkillUpgrade::StartPassiveSkill
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: SkillID_t iSkillID
	// Parameter: const Obj_Human * pHuman
	// 加载被动技能
	//************************************
	VOID StartPassiveSkill( SkillID_t iSkillID, const Obj_Human* pHuman );
protected:
	//************************************
	// Method:    CheckCondition
	// FullName:  HumanSkillUpgrade::CheckCondition
	// Access:    protected 
	// Returns:   BOOL
	// Qualifier:
	// 检查是否符合升级条件
	// template method 成员，不要重载
	//************************************
	BOOL CheckCondition();

	//************************************
	// Method:    DepleteHumanAttr
	// FullName:  HumanSkillUpgrade::DepleteHumanAttr
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 消耗玩家的技能升级相关属性
	// template method 成员，不要重载
	//************************************
	VOID DepleteHumanAttr();

	//************************************
	// Method:    UpgradeLevel
	// FullName:  HumanSkillUpgrade::UpgradeLevel
	// Access:    protected 
	// Returns:   VOID
	// Qualifier:
	//************************************
	VOID UpgradeLevel();

	//比较玩家等级是否符合升级需要
	//************************************
	// Method:    IsEnoughHumanLevel
	// FullName:  HumanSkillUpgrade::IsEnoughHumanLevel
	// Access:    private 
	// Returns:   BOOL
	// Qualifier:
	//************************************
	virtual BOOL IsEnoughHumanLevel();

	//比较玩家剩余技能点数是否符合升级需要
	//************************************
	// Method:    IsEnoughSkillPoints
	// FullName:  HumanSkillUpgrade::IsEnoughSkillPoints
	// Access:    private 
	// Returns:   BOOL
	// Qualifier:
	//************************************
	virtual BOOL IsEnoughSkillPoints();

	//************************************
	// Method:    IsSkillClassSpendEnoughPosints
	// FullName:  HumanSkillUpgrade::IsSkillClassSpendEnoughPosints
	// Access:    virtual protected 
	// Returns:   BOOL
	// Qualifier:
	// 技能所属系消耗点数总合需求
	//************************************
	virtual BOOL IsSkillClassSpendEnoughPosints();
	//比较玩家职业否符合升级需要
	//************************************
	// Method:    IsOccupationMatch
	// FullName:  HumanSkillUpgrade::IsOccupationMatch
	// Access:    private 
	// Returns:   BOOL
	// Qualifier:
	//************************************
	virtual BOOL IsOccupationMatch();

	// 是否满足父技能条件 以及 比较此技能的父技能等级否符合升级需要
	//************************************
	// Method:    IsEnoughParentSkillPoints
	// FullName:  HumanSkillUpgrade::IsEnoughParentSkillPoints
	// Access:    private 
	// Returns:   BOOL
	// Qualifier:
	//************************************
	virtual BOOL IsEnoughParentSkillLevel();

	//比较剩余金钱否符合升级需要
	//************************************
	// Method:    IsEnoughMoney
	// FullName:  HumanSkillUpgrade::IsEnoughMoney
	// Access:    private 
	// Returns:   BOOL
	// Qualifier:
	//************************************
	virtual BOOL IsEnoughMoney();

	//比较剩余经验值否符合升级需要
	//************************************
	// Method:    IsEnoughExp
	// FullName:  HumanSkillUpgrade::IsEnoughExp
	// Access:    private 
	// Returns:   BOOL
	// Qualifier:
	//************************************
	virtual BOOL IsEnoughExp();

	//************************************
	// Method:    IsHadEnoughItem1
	// FullName:  HumanSkillUpgrade::IsHadEnoughItem1
	// Access:    virtual protected 
	// Returns:   BOOL
	// Qualifier:
	// 是否具有足够的第一类物品
	//************************************
	virtual BOOL IsHadEnoughItem1();

	//************************************
	// Method:    IsHadEnoughItem2
	// FullName:  HumanSkillUpgrade::IsHadEnoughItem2
	// Access:    virtual protected 
	// Returns:   BOOL
	// Qualifier:
	// 是否具有足够的第二类物品
	//************************************
	virtual BOOL IsHadEnoughItem2();

	//************************************
	// Method:    IsHadEnoughItem3
	// FullName:  HumanSkillUpgrade::IsHadEnoughItem3
	// Access:    virtual protected 
	// Returns:   BOOL
	// Qualifier:
	// 是否具有足够的第三类物品
	//************************************
	virtual BOOL IsHadEnoughItem3();

	//************************************
	// Method:    SendSuccessSkillLevelupMsg
	// FullName:  HumanSkillUpgrade::SendSuccessSkillLevelupMsg
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 发送升级成功之后的结果消息给client
	//************************************
	virtual VOID SendSuccessSkillLevelupMsg();

	//************************************
	// Method:    SendFailedSkillLevelupMsg
	// FullName:  HumanSkillUpgrade::SendFailedSkillLevelupMsg
	// Access:    virtual protected 
	// Returns:   VOID
	// Qualifier:
	// 否决升级请求
	//************************************
	virtual VOID SendFailedSkillLevelupMsg();


	//************************************
	// Method:    DepleteSkillPoints
	// FullName:  HumanSkillUpgrade::DepleteSkillPoints
	// Access:    virtual protected 
	// Returns:   VOID
	// Qualifier:
	// 消耗技能点
	//************************************
	virtual VOID DepleteSkillPoints();

	//************************************
	// Method:    DepleteMoney
	// FullName:  HumanSkillUpgrade::DepleteMoney
	// Access:    virtual protected 
	// Returns:   VOID
	// Qualifier:
	// 消耗金钱
	//************************************
	virtual VOID DepleteMoney();

	//************************************
	// Method:    DepleteExp
	// FullName:  HumanSkillUpgrade::DepleteExp
	// Access:    virtual protected 
	// Returns:   VOID
	// Qualifier:
	// 消耗经验
	//************************************
	virtual VOID DepleteExp();

	//************************************
	// Method:    DepleteItem1
	// FullName:  HumanSkillUpgrade::DepleteItem1
	// Access:    virtual protected 
	// Returns:   VOID
	// Qualifier:
	// 消耗物品1
	//************************************
	virtual VOID DepleteItem1();

	//************************************
	// Method:    DepleteItem2
	// FullName:  HumanSkillUpgrade::DepleteItem2
	// Access:    virtual protected 
	// Returns:   VOID
	// Qualifier:
	// 消耗物品2
	//************************************
	virtual VOID DepleteItem2();

	//************************************
	// Method:    DepleteItem3
	// FullName:  HumanSkillUpgrade::DepleteItem3
	// Access:    virtual protected 
	// Returns:   VOID
	// Qualifier:
	// 消耗物品3
	//************************************
	virtual VOID DepleteItem3();


private:
	//************************************
	// Method:    InitAttr
	// FullName:  HumanSkillUpgrade::InitAttr
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: const Obj_Human * pHuman
	// Parameter: SkillID_t iSkillID
	// Parameter: INT iLevel
	// 初始化类成员
	//************************************
	VOID InitAttr( const Obj_Human* pHuman, SkillID_t iSkillID, INT iLevel );

	//************************************
	// Method:    GetSkillStudyCondition
	// FullName:  HumanSkillUpgrade::GetSkillStudyCondition
	// Access:    private 
	// Returns:   BOOL
	// Qualifier:
	// 检查技能ID的合法性
	//************************************
	BOOL CheckSkillStudyCondition();

	//************************************
	// Method:    FindSkillIndex
	// FullName:  HumanSkillUpgrade::FindSkillIndex
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 查找指定技能在技能升级表结构中的索引
	//************************************
	VOID FindSkillIndex();

protected:
	//请求升级的玩家
	Obj_Human*	m_pHuman;
	//请求升级的技能ID
	SkillID_t	m_iSkillID;
	//要升到多少级
	INT			m_iLevel;
	//索引用的下标
	INT			m_iIndex;
};

#endif // HumanSkillUpgrad_h__