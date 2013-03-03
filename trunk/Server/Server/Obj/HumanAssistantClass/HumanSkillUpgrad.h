/********************************************************************
	created:	2008/01/11
	created:	11:1:2008   15:44
	filename: 	MapServer\Server\Obj\HumanAssistantClass\HumanSkillUpgrad.h
	file path:	MapServer\Server\Obj\HumanAssistantClass
	file base:	HumanSkillUpgrad
	file ext:	h
	author:		Richard
	
	purpose:	������Ҽ���ѧϰ�������������жϺ����ģ�ͨ�������߼���

	�ж�ѧϰ�������ĺϷ��ԣ���ͨ��Obj_Human��������Է��ʽӿ�ֱ������

	���

	��װ�����뼼��ѧϰ�������йص��߼����������Obj_Human�����������

	HumanSkillUpgrade����Obj_Human�����뼼��ѧϰ�������й����Եķ��ʽӿ�

	��������Ϲ�ϵ���Լ���ѧϰ�������йص��߼��ı�ֻ��Ҫ�ı������߼���

	Obj_Human�Դ�һ����֪�������������ʽ���޸�

	Ҫע�����ͼ���ϵͳ��ǿ��Ϲ�ϵ�����缼�ܱ��ĸ��첻Ӧ��Ҫ�޸Ĵ��࣬

	�Լ��ܱ�����Եķ��ʿ�ͨ��һ���н��߽��У��ⲿ�仯����װ���н�����
*********************************************************************/

/********************************************************************

	HumanSkillLevelUpΪģ�巽�����������ж���Ҽ��������Ĵ����費������

	��������Ϊ���̵�ÿ�������ʵ�֣���������ʵ����������ʵ�֣��Ըı�˽ӿ�

	����Ϊ.

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
	// �����������ֵ
	//************************************
	VOID Cleanup(VOID);

public:

	//************************************
	// Method:    HumanSkillLevelUp
	// FullName:  HumanSkillUpgrade::HumanSkillLevelUp
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: const Obj_Human * pHuman �������������
	// Parameter: SkillID_t iSkillID ���������ļ���ID
	// Parameter: INT iLevel Ҫ�������ټ�
	// template method ��Ա����Ҫ����
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
	// ���ر�������
	//************************************
	VOID StartPassiveSkill( SkillID_t iSkillID, const Obj_Human* pHuman );
protected:
	//************************************
	// Method:    CheckCondition
	// FullName:  HumanSkillUpgrade::CheckCondition
	// Access:    protected 
	// Returns:   BOOL
	// Qualifier:
	// ����Ƿ������������
	// template method ��Ա����Ҫ����
	//************************************
	BOOL CheckCondition();

	//************************************
	// Method:    DepleteHumanAttr
	// FullName:  HumanSkillUpgrade::DepleteHumanAttr
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ������ҵļ��������������
	// template method ��Ա����Ҫ����
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

	//�Ƚ���ҵȼ��Ƿ����������Ҫ
	//************************************
	// Method:    IsEnoughHumanLevel
	// FullName:  HumanSkillUpgrade::IsEnoughHumanLevel
	// Access:    private 
	// Returns:   BOOL
	// Qualifier:
	//************************************
	virtual BOOL IsEnoughHumanLevel();

	//�Ƚ����ʣ�༼�ܵ����Ƿ����������Ҫ
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
	// ��������ϵ���ĵ����ܺ�����
	//************************************
	virtual BOOL IsSkillClassSpendEnoughPosints();
	//�Ƚ����ְҵ�����������Ҫ
	//************************************
	// Method:    IsOccupationMatch
	// FullName:  HumanSkillUpgrade::IsOccupationMatch
	// Access:    private 
	// Returns:   BOOL
	// Qualifier:
	//************************************
	virtual BOOL IsOccupationMatch();

	// �Ƿ����㸸�������� �Լ� �Ƚϴ˼��ܵĸ����ܵȼ������������Ҫ
	//************************************
	// Method:    IsEnoughParentSkillPoints
	// FullName:  HumanSkillUpgrade::IsEnoughParentSkillPoints
	// Access:    private 
	// Returns:   BOOL
	// Qualifier:
	//************************************
	virtual BOOL IsEnoughParentSkillLevel();

	//�Ƚ�ʣ���Ǯ�����������Ҫ
	//************************************
	// Method:    IsEnoughMoney
	// FullName:  HumanSkillUpgrade::IsEnoughMoney
	// Access:    private 
	// Returns:   BOOL
	// Qualifier:
	//************************************
	virtual BOOL IsEnoughMoney();

	//�Ƚ�ʣ�ྭ��ֵ�����������Ҫ
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
	// �Ƿ�����㹻�ĵ�һ����Ʒ
	//************************************
	virtual BOOL IsHadEnoughItem1();

	//************************************
	// Method:    IsHadEnoughItem2
	// FullName:  HumanSkillUpgrade::IsHadEnoughItem2
	// Access:    virtual protected 
	// Returns:   BOOL
	// Qualifier:
	// �Ƿ�����㹻�ĵڶ�����Ʒ
	//************************************
	virtual BOOL IsHadEnoughItem2();

	//************************************
	// Method:    IsHadEnoughItem3
	// FullName:  HumanSkillUpgrade::IsHadEnoughItem3
	// Access:    virtual protected 
	// Returns:   BOOL
	// Qualifier:
	// �Ƿ�����㹻�ĵ�������Ʒ
	//************************************
	virtual BOOL IsHadEnoughItem3();

	//************************************
	// Method:    SendSuccessSkillLevelupMsg
	// FullName:  HumanSkillUpgrade::SendSuccessSkillLevelupMsg
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ���������ɹ�֮��Ľ����Ϣ��client
	//************************************
	virtual VOID SendSuccessSkillLevelupMsg();

	//************************************
	// Method:    SendFailedSkillLevelupMsg
	// FullName:  HumanSkillUpgrade::SendFailedSkillLevelupMsg
	// Access:    virtual protected 
	// Returns:   VOID
	// Qualifier:
	// �����������
	//************************************
	virtual VOID SendFailedSkillLevelupMsg();


	//************************************
	// Method:    DepleteSkillPoints
	// FullName:  HumanSkillUpgrade::DepleteSkillPoints
	// Access:    virtual protected 
	// Returns:   VOID
	// Qualifier:
	// ���ļ��ܵ�
	//************************************
	virtual VOID DepleteSkillPoints();

	//************************************
	// Method:    DepleteMoney
	// FullName:  HumanSkillUpgrade::DepleteMoney
	// Access:    virtual protected 
	// Returns:   VOID
	// Qualifier:
	// ���Ľ�Ǯ
	//************************************
	virtual VOID DepleteMoney();

	//************************************
	// Method:    DepleteExp
	// FullName:  HumanSkillUpgrade::DepleteExp
	// Access:    virtual protected 
	// Returns:   VOID
	// Qualifier:
	// ���ľ���
	//************************************
	virtual VOID DepleteExp();

	//************************************
	// Method:    DepleteItem1
	// FullName:  HumanSkillUpgrade::DepleteItem1
	// Access:    virtual protected 
	// Returns:   VOID
	// Qualifier:
	// ������Ʒ1
	//************************************
	virtual VOID DepleteItem1();

	//************************************
	// Method:    DepleteItem2
	// FullName:  HumanSkillUpgrade::DepleteItem2
	// Access:    virtual protected 
	// Returns:   VOID
	// Qualifier:
	// ������Ʒ2
	//************************************
	virtual VOID DepleteItem2();

	//************************************
	// Method:    DepleteItem3
	// FullName:  HumanSkillUpgrade::DepleteItem3
	// Access:    virtual protected 
	// Returns:   VOID
	// Qualifier:
	// ������Ʒ3
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
	// ��ʼ�����Ա
	//************************************
	VOID InitAttr( const Obj_Human* pHuman, SkillID_t iSkillID, INT iLevel );

	//************************************
	// Method:    GetSkillStudyCondition
	// FullName:  HumanSkillUpgrade::GetSkillStudyCondition
	// Access:    private 
	// Returns:   BOOL
	// Qualifier:
	// ��鼼��ID�ĺϷ���
	//************************************
	BOOL CheckSkillStudyCondition();

	//************************************
	// Method:    FindSkillIndex
	// FullName:  HumanSkillUpgrade::FindSkillIndex
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ����ָ�������ڼ���������ṹ�е�����
	//************************************
	VOID FindSkillIndex();

protected:
	//�������������
	Obj_Human*	m_pHuman;
	//���������ļ���ID
	SkillID_t	m_iSkillID;
	//Ҫ�������ټ�
	INT			m_iLevel;
	//�����õ��±�
	INT			m_iIndex;
};

#endif // HumanSkillUpgrad_h__