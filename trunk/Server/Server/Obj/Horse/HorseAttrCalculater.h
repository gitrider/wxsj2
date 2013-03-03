/********************************************************************
	created:	2008/03/12
	created:	12:3:2008   14:47
	filename: 	MapServer\Server\Obj\HumanAssistantClass\HorseAttrCalculater.h
	file path:	MapServer\Server\Obj\HumanAssistantClass
	file base:	HorseAttrCalculater
	file ext:	h
	author:		Richard
	
	purpose:	ʵ������������Լ�����ص�����

	���ԡ����ǡ����ʵ�����������Ʒʹ��
	1��ս�����ܣ�2��ս����������ȼ�����ʱ���д���
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
	// ����˸������սʱ�ĳ�ʼ��
	//************************************
	VOID ResetHorse();

	//************************************
	// Method:    OnExpChange
	// FullName:  HorseAttrCalculater::OnExpChange
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// Parameter: INT iExp ������ľ���ֵ
	// �����þ���ֵ�Ժ󣬴���ȼ���������ȼ��仯�����Եĸı�
	//************************************
	VOID OnExpChange( INT iIncExp );

	//************************************
	// Method:    SetGenGu
	// FullName:  HorseAttrCalculater::SetGenGu
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: INT iGenGu
	// ������ĸ���,���Ǳ仯���������ʵı仯
	//************************************
	VOID SetGenGu( INT iGenGu );

	//************************************
	// Method:    SetSavvy
	// FullName:  HorseAttrCalculater::SetSavvy
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: INT iSavvy
	// �����������, ���Եı仯���������ʵı仯
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
	// ����һ�õ�����ʱ�򣬸����ڳ�ս��������1��ս�����Ե���
	//************************************
	VOID UpdateAttackLevel1();

	//************************************
	// Method:    UpdateRemainPoint
	// FullName:  HorseAttrCalculater::UpdateRemainPoint
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// ����ʣ�����
	//************************************
	VOID UpdateRemainPoint();

	//************************************
	// Method:    UpdateAttackLevel2
	// FullName:  HorseAttrCalculater::UpdateAttackLevel2
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// �����ڳ�ս�������2��ս������
	//************************************
	VOID UpdateAttackLevel2();

	//************************************
	// Method:    UpdatePerception
	// FullName:  HorseAttrCalculater::UpdatePerception
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// ���ʼ���
	//************************************
	VOID UpdatePerception();

	//************************************
	// Method:    IsActive
	// FullName:  HorseAttrCalculater::IsActive
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human
	// �Ƿ��г�ս����
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
	// Returns:   INT ������ʽ�����Ľ��
	// Qualifier:
	// Parameter: INT iPer ��ǰ����
	// ���ʼ��㹫ʽ
	//************************************
	INT CalcPerception( INT iPer );

	//************************************
	// Method:    RefreshAttr
	// FullName:  HorseAttrCalculater::RefreshAttr
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// �����Է����ı�ʱ��client���͸������Ե���Ϣ
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
	//������ֶ���ʱ���½�
	VOID HappinessHeartBeat( UINT uTime );
	//��������
	//ÿ10������һ�Σ���60��Ϊֹ
	VOID SkillRealization();
	
private:
	INT					m_nIndex;
	_PET_DB*			m_pPetDB;
	Obj_Human*			m_pOnwer;
	_HORSE_ATTR_BACKUP	m_AttrBackUp;
	UINT				m_iHappinessTimer;
};



#endif // HorseAttrCalculater_h__