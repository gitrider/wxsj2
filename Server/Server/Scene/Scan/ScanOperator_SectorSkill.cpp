/********************************************************************
	created:	2007/10/31
	created:	31:10:2007   13:06
	filename: 	d:\project\MapServer\Server\ScanOperator_SectorSkill.cpp
	file path:	d:\project\MapServer\Server
	file base:	ScanOperator_SectorSkill
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "ScanOperator_SectorSkill.h"

using namespace Combat_Module::Skill_Module;

const FLOAT FLOAT_PRECISION = 0.000001f;
const FLOAT SCAN_PI			= 3.14159265359f;

ScanOperator_SectorSkill::ScanOperator_SectorSkill():
	m_pTargets(NULL),
	m_fRadius(0.0f),
	m_nCount(0),
	m_pSkillInfo(NULL),
	m_pMe(NULL),
	m_bReady(FALSE)
{
	Clear();
	m_CentrePoint.CleanUp();
}

ScanOperator_SectorSkill::~ScanOperator_SectorSkill()
{

}

BOOL ScanOperator_SectorSkill::Init( SCANOPERATOR_SECTORSKILL_INIT* pInit )
{
	__ENTER_FUNCTION

		if(pInit==NULL)
		{
			AssertEx(FALSE,"[ScanOperator_AESkill::Init]: NULL init struct pointer found!!") ;
			return FALSE ;
		}
		if(NULL==pInit->m_pTargets)
		{
			AssertEx(FALSE,"[ScanOperator_AESkill::Init]: NULL OBJLIST pointer found!!") ;
			return FALSE ;		
		}
		if(NULL==pInit->m_pMe)
		{
			AssertEx(FALSE,"[ScanOperator_AESkill::Init]: NULL init Me pointer found!!") ;
			return FALSE ;		
		}
		if(NULL==pInit->m_pSkillInfo)
		{
			AssertEx(FALSE,"[ScanOperator_AESkill::Init]: NULL Skill Info pointer found!!") ;
			return FALSE ;				
		}
		pInit->InitBase();
		m_pTargets = pInit->m_pTargets;
		m_fRadius = pInit->m_fRadius;
		m_nCount = pInit->m_nCount;
		m_pSkillInfo = pInit->m_pSkillInfo;
		m_pMe = pInit->m_pMe;
		m_CentrePoint.m_fX = pInit->m_CentrePoint.m_fX;
		m_CentrePoint.m_fZ = pInit->m_CentrePoint.m_fZ;
		if(TRUE==ScanOperator::Init(pInit))
		{
			m_bReady = TRUE;
			return TRUE;
		}
		m_bReady = FALSE;

	__LEAVE_FUNCTION

	return FALSE;
}

UINT ScanOperator_SectorSkill::On_FindObj( Obj* pObj )
{
	__ENTER_FUNCTION
	//此处搜索应不包括自己
	//if( m_pMe == pObj )
	//	return SCANRETURN_CONTINUE;

	if( m_nCount <= 0 )
		return SCANRETURN_RETURN;


	if(FALSE==m_bReady)
	{
		AssertEx(FALSE,"[ScanOperator_AESkill::On_FindObj]:Init first, use second! please~!!");
		return SCANRETURN_RETURN;
	}
	if(pObj==NULL)
	{
		AssertEx(FALSE,"[ScanOperator_AESkill::On_FindObj]: NULL Obj Pointer Found!!");
		return SCANRETURN_RETURN;
	}
	if(!IsCharacterObj(pObj->GetObjType()))
	{
		return SCANRETURN_CONTINUE;
	}

	FLOAT fDistance = 9999999.0f;
	//判断点是否在扇形内的算法
	if( !IsInSector(GetAngle(), pObj, fDistance) )
	{
		//不在AE范围
		return SCANRETURN_CONTINUE;
	}

	SkillLogic_T const* pSkillLogic = Skill_GetLogic(*m_pSkillInfo);
	if(FALSE == pSkillLogic->IsWantedTarget(*m_pMe, (Obj_Character&)(*pObj), *m_pSkillInfo))
	{
		return SCANRETURN_CONTINUE;
	}

	return InsterObjToTagList( fDistance, pObj );

	__LEAVE_FUNCTION
	return SCANRETURN_CONTINUE;
}

FLOAT ScanOperator_SectorSkill::GetAngle()
{
	__ENTER_FUNCTION

	return m_pSkillInfo->GetAngle();

	__LEAVE_FUNCTION

	return 0;
}

BOOL ScanOperator_SectorSkill::IsInSector( FLOAT fAngle, Obj* pObj, FLOAT& fDistance )
{
	__ENTER_FUNCTION

	TargetingAndDepletingParams_T& rParams = m_pMe->GetTargetingAndDepletingParams();

	const WORLD_POS* pTagPos	= pObj->getWorldPos();
	FLOAT fDir					= rParams.GetTargetDirection();
	FLOAT fDistSq				= MyLengthSq( &m_CentrePoint, pTagPos );
	fDistance = fDistSq;
	//首先判断是否在有效距离内
	if( fDistSq>(m_fRadius*m_fRadius) )
	{
		//不在AE范围
		return FALSE;
	}

	//非方向技能，不需要进一步判断
	if( fDir < -0.00000001 )
	{
		return TRUE;
	}

	//目标与攻击点的相对坐标
	WORLD_POS tmpPos; 
	tmpPos.m_fX = pTagPos->m_fX - m_CentrePoint.m_fX;
	tmpPos.m_fZ = pTagPos->m_fZ - m_CentrePoint.m_fZ;

	//攻击范围圆心角的半角
	FLOAT fHalfAngle	= fAngle * 0.5f;
	//计算向量的0至2*PI夹角
	FLOAT fTagAngle		= GetFineAngle( tmpPos.m_fZ, tmpPos.m_fX );
	//攻击方向与攻击角度的夹角
	fTagAngle = fabs( fTagAngle - fDir );
	
	fTagAngle = (fTagAngle>SCAN_PI) ? (2*SCAN_PI-fTagAngle):fTagAngle;

	if( fTagAngle < fHalfAngle )
	{
		//在范围内
		return TRUE;
	}

	return FALSE;

	__LEAVE_FUNCTION
	return FALSE;
}

FLOAT ScanOperator_SectorSkill::GetFineAngle( FLOAT fX, FLOAT fY )
{
	__ENTER_FUNCTION
	
	FLOAT fAngle = 0;
	//处理Y轴为0的特定情况
	if( fY<FLOAT_PRECISION && fY>-FLOAT_PRECISION )
	{
		if( fX >= 0 )
			return 0;
		else
			return SCAN_PI;
	}
	//处理X轴为0的特定情况
	else if( fX<FLOAT_PRECISION && fX>-FLOAT_PRECISION )
	{
		if( fY >= 0 )
			return SCAN_PI * 0.5f;
		else
			return SCAN_PI * 1.5f;
	}

	//处理4个象限的取值
	//第一象限不处理
	fAngle = atanf( fabs(fY / fX ) );
	//第2象限
	if( fX<0 && fY>0 )
	{
		fAngle = atanf( fabs(fX / fY ) );
		fAngle += SCAN_PI * 0.5f;
	}
	//第3象限
	else if( fX<0 && fY<0 )
		fAngle += SCAN_PI;
	//第4象限
	else if( fX>0 && fY<0 )
	{
		fAngle = atanf( fabs(fX / fY ) );
		fAngle += SCAN_PI * 1.5f;
	}

	return fAngle;

	__LEAVE_FUNCTION
	return 0;
}

VOID ScanOperator_SectorSkill::Clear()
{
	__ENTER_FUNCTION
		memset( m_aDistance, 0, sizeof(FLOAT)*_OBJLIST::MAX_OBJ_LIST_SIZE );
	__LEAVE_FUNCTION
}

INT ScanOperator_SectorSkill::InsterObjToTagList( FLOAT fDistance, Obj* pObj )
{
	__ENTER_FUNCTION

	BYTE& rCount = m_pTargets->m_Count;
	//已经到达缓冲区上限停止搜索
	if( OBJLIST::MAX_OBJ_LIST_SIZE<=rCount )
		return SCANRETURN_RETURN;

	//此处应按距离进行排序
	//if(rCount<m_nCount&&OBJLIST::MAX_OBJ_LIST_SIZE>rCount)
	//如果没有达到搜索目标数目上限则进行插入
	if(rCount<m_nCount)
	{
		//如果是第一个不需要进行排序，直接写入，并返回继续搜索
		if( rCount == 0 )
		{
			m_pTargets->m_aObj[rCount]	= pObj;
			m_aDistance[rCount]			= fDistance;
			++rCount;
			return SCANRETURN_CONTINUE;
		}
		//如果不是第一个对象，则进行按升序进行排序并插入，返回继续搜索
		for( INT i=0; i<rCount; ++i )
		{
			if( fDistance < m_aDistance[i] )
			{
				memmove( &m_pTargets->m_aObj[i+1], &m_pTargets->m_aObj[i], sizeof(Obj*)*(rCount-i) );
				memmove( &m_aDistance[i+1], &m_aDistance[i], sizeof(Obj*)*(rCount-i) );
				m_pTargets->m_aObj[i]	= pObj;
				m_aDistance[i]			= fDistance;
				++rCount;
				return SCANRETURN_CONTINUE;
			}
		}
		//没有比当前对象距离远的，则添加在最后
		m_pTargets->m_aObj[rCount]	= pObj;
		m_aDistance[rCount]			= fDistance;
		++rCount;
	}
	//否则进行replace
	else
	{
		for( INT i=0; i<rCount; ++i )
		{
			if( fDistance < m_aDistance[i] )
			{
				m_pTargets->m_aObj[i]	= pObj;
				m_aDistance[i]			= fDistance;
				break;
			}
		}
	}
	return SCANRETURN_CONTINUE;
	__LEAVE_FUNCTION
	return SCANRETURN_CONTINUE;
}





































