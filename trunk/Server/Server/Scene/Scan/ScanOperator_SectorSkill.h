/********************************************************************
	created:	2007/10/31
	created:	31:10:2007   13:05
	filename: 	d:\project\MapServer\Server\ScanOperator_SectorSkill.h
	file path:	d:\project\MapServer\Server
	file base:	ScanOperator_SectorSkill
	file ext:	h
	author:		Richard
	
	purpose:	扇形范围扫描目标,圆形可以作为一个中心角为360度的扇形
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
	//保存扫描结果，由外部进行初始化，不要在此释放
	OBJLIST*			m_pTargets;
	//作用半径
	FLOAT				m_fRadius;
	//最大有效合法被搜索对象数量
	INT					m_nCount;
	//搜索主体当前的技能信息
	SkillInfo_T const*	m_pSkillInfo;
	//搜索主体
	Obj_Character*		m_pMe;
	//搜索主体所在的位置
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

	//初始化扫描控制器
	virtual BOOL				Init(SCANOPERATOR_SECTORSKILL_INIT* pInit) ;
	//执行扫描
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
	//取得面积攻击技能的圆心角
	FLOAT	GetAngle();
	//判断obj是否在给定圆心角的扇形范围内
	// fDistance 为输出参数，当前怪物到我的距离的平方
	BOOL	IsInSector( FLOAT fAngle, Obj* pObj, FLOAT& fDistance );
	//根据给定坐标，计算0至2*PI范围内的弧度 
	FLOAT	GetFineAngle( FLOAT fX, FLOAT fY );
	INT		InsterObjToTagList( FLOAT fDistance, Obj* pObj );
	VOID	Clear();
};


#endif // ScanOperator_SectorSkill_h__