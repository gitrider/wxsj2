/********************************************************************
	created:	2007/10/31
	created:	31:10:2007   13:05
	filename: 	d:\project\MapServer\Server\ScanOperator_SectorSkill.h
	file path:	d:\project\MapServer\Server
	file base:	ScanOperator_SectorSkill
	file ext:	h
	author:		Richard
	
	purpose:	���η�Χɨ��Ŀ��,Բ�ο�����Ϊһ�����Ľ�Ϊ360�ȵ�����
*********************************************************************/

#ifndef ScanOperator_SectorSkill_h__31_10_2007_13_09
#define ScanOperator_SectorSkill_h__31_10_2007_13_09

#include "Type.h"
#include "ScanOperator.h"
#include "AI_Monster_Def.h"
#include "SkillLogic.h"
#include "Obj_Character.h"
#include "Scene.h"
#include "SkillInfo.h"

using namespace Combat_Module::Skill_Module;

struct SCANOPERATOR_SECTORSKILL_INIT : public SCANOPERATOR_INIT
{
	//����ɨ���������ⲿ���г�ʼ������Ҫ�ڴ��ͷ�
	OBJLIST*			m_pTargets;
	//���ð뾶
	FLOAT				m_fRadius;
	//�����Ч�Ϸ���������������
	INT					m_nCount;
	//�������嵱ǰ�ļ�����Ϣ
	SkillInfo_T const*	m_pSkillInfo;
	//��������
	Obj_Character*		m_pMe;
	//�����������ڵ�λ��
	WORLD_POS			m_CentrePoint;
	SCANOPERATOR_SECTORSKILL_INIT()
	{
		m_pTargets	= NULL;
		m_fRadius	= 0.0f;
		m_nCount	= 0;
		m_pSkillInfo= NULL;
		m_pMe		= NULL;
		
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



class ScanOperator_SectorSkill : public ScanOperator
{
	FLOAT m_aDistance[_OBJLIST::MAX_OBJ_LIST_SIZE];
public :
	ScanOperator_SectorSkill();
	virtual ~ScanOperator_SectorSkill();

	//��ʼ��ɨ�������
	virtual BOOL				Init(SCANOPERATOR_SECTORSKILL_INIT* pInit) ;
	//ִ��ɨ��
	virtual UINT				On_FindObj(Obj* pObj) ;

public :
	OBJLIST*			m_pTargets;
	FLOAT				m_fRadius;
	INT					m_nCount;
	SkillInfo_T const*	m_pSkillInfo;
	Obj_Character*		m_pMe;
	WORLD_POS			m_CentrePoint;
	BOOL				m_bReady;

private:
	//ȡ������������ܵ�Բ�Ľ�
	FLOAT	GetAngle();
	//�ж�obj�Ƿ��ڸ���Բ�Ľǵ����η�Χ��
	// fDistance Ϊ�����������ǰ���ﵽ�ҵľ����ƽ��
	BOOL	IsInSector( FLOAT fAngle, Obj* pObj, FLOAT& fDistance );
	//���ݸ������꣬����0��2*PI��Χ�ڵĻ��� 
	FLOAT	GetFineAngle( FLOAT fX, FLOAT fY );
	INT		InsterObjToTagList( FLOAT fDistance, Obj* pObj );
	VOID	Clear();
};


#endif // ScanOperator_SectorSkill_h__