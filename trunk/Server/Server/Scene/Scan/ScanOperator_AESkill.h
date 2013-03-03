/////////////////////////////////////////////////////////////////////////////////
//�ļ�����ScanOperator_AESkill.h
//�����ߣ�gaoqi
//���������������Ч���ܵ���������
//�޸ļ�¼��2005-12-6����
/////////////////////////////////////////////////////////////////////////////////


#ifndef SCANOPERATOR_AESKILL_H
#define SCANOPERATOR_AESKILL_H

#include "Type.h"
#include "ScanOperator.h"
#include "AI_Monster_Def.h"
#include "SkillLogic.h"
#include "Obj_Character.h"
#include "Scene.h"
#include "SkillInfo.h"

using namespace Combat_Module::Skill_Module;

struct SCANOPERATOR_AESKILL_INIT : public SCANOPERATOR_INIT
{
	OBJLIST*		m_pTargets;
	FLOAT			m_fRadius;
	INT				m_nCount;
	SkillInfo_T const*	m_pSkillInfo;
	Obj_Character*	m_pMe;
	WORLD_POS		m_CentrePoint;
	SCANOPERATOR_AESKILL_INIT()
	{
		m_pTargets = NULL;
		m_fRadius = 0.0f ;
		m_nCount = 0;
		m_pSkillInfo = NULL;
		m_pMe = NULL;
		m_CentrePoint.CleanUp();
	}
	VOID InitBase(VOID)
	{
		INT& rZoneSize = g_Config.m_ConfigInfo.m_ZoneSize;
		if(0!=rZoneSize)
		{
			m_nZoneRadius = INT(m_fRadius/rZoneSize)+1;
		}
		else
		{
			m_nZoneRadius = 0;
		}
		if(NULL==m_pScene)
		{
			AssertEx(FALSE,"[SCANOPERATOR_AESKILL_INIT::InitBase]:Scene point is NULL!!please initialize it.");
			return;
		}
		m_ZoneID = m_pScene->CalcZoneID(&m_CentrePoint);
	}
};



class ScanOperator_AESkill : public ScanOperator
{
public :
	ScanOperator_AESkill();
	virtual ~ScanOperator_AESkill();

	//��ʼ��ɨ�������
	virtual BOOL				Init(SCANOPERATOR_AESKILL_INIT* pInit) ;
	//ɨ�������ʼǰ�ص�
	//	virtual BOOL			On_BeforeScan();
	//�жϲ����е�Zone�Ƿ���Ҫɨ��
	//	virtual BOOL			On_IsNeedScan(ZoneID_t ZoneID) ;
	//������һ��Obj, ����ֵ�� SCANRETURN
	virtual UINT				On_FindObj(Obj* pObj) ;
	//ɨ�������ɺ�ص�
	//	virtual VOID			On_AfterScan() ;


public :
	OBJLIST*		m_pTargets;
	FLOAT			m_fRadius;
	INT				m_nCount;
	SkillInfo_T const*	m_pSkillInfo;
	Obj_Character*	m_pMe;
	WORLD_POS		m_CentrePoint;
	BOOL			m_bReady;
};




#endif //SCANOPERATOR_AESKILL_H
