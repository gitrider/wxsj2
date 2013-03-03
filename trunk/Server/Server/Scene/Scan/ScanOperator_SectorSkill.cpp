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
	//�˴�����Ӧ�������Լ�
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
	//�жϵ��Ƿ��������ڵ��㷨
	if( !IsInSector(GetAngle(), pObj, fDistance) )
	{
		//����AE��Χ
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
	//�����ж��Ƿ�����Ч������
	if( fDistSq>(m_fRadius*m_fRadius) )
	{
		//����AE��Χ
		return FALSE;
	}

	//�Ƿ����ܣ�����Ҫ��һ���ж�
	if( fDir < -0.00000001 )
	{
		return TRUE;
	}

	//Ŀ���빥������������
	WORLD_POS tmpPos; 
	tmpPos.m_fX = pTagPos->m_fX - m_CentrePoint.m_fX;
	tmpPos.m_fZ = pTagPos->m_fZ - m_CentrePoint.m_fZ;

	//������ΧԲ�Ľǵİ��
	FLOAT fHalfAngle	= fAngle * 0.5f;
	//����������0��2*PI�н�
	FLOAT fTagAngle		= GetFineAngle( tmpPos.m_fZ, tmpPos.m_fX );
	//���������빥���Ƕȵļн�
	fTagAngle = fabs( fTagAngle - fDir );
	
	fTagAngle = (fTagAngle>SCAN_PI) ? (2*SCAN_PI-fTagAngle):fTagAngle;

	if( fTagAngle < fHalfAngle )
	{
		//�ڷ�Χ��
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
	//����Y��Ϊ0���ض����
	if( fY<FLOAT_PRECISION && fY>-FLOAT_PRECISION )
	{
		if( fX >= 0 )
			return 0;
		else
			return SCAN_PI;
	}
	//����X��Ϊ0���ض����
	else if( fX<FLOAT_PRECISION && fX>-FLOAT_PRECISION )
	{
		if( fY >= 0 )
			return SCAN_PI * 0.5f;
		else
			return SCAN_PI * 1.5f;
	}

	//����4�����޵�ȡֵ
	//��һ���޲�����
	fAngle = atanf( fabs(fY / fX ) );
	//��2����
	if( fX<0 && fY>0 )
	{
		fAngle = atanf( fabs(fX / fY ) );
		fAngle += SCAN_PI * 0.5f;
	}
	//��3����
	else if( fX<0 && fY<0 )
		fAngle += SCAN_PI;
	//��4����
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
	//�Ѿ����ﻺ��������ֹͣ����
	if( OBJLIST::MAX_OBJ_LIST_SIZE<=rCount )
		return SCANRETURN_RETURN;

	//�˴�Ӧ�������������
	//if(rCount<m_nCount&&OBJLIST::MAX_OBJ_LIST_SIZE>rCount)
	//���û�дﵽ����Ŀ����Ŀ��������в���
	if(rCount<m_nCount)
	{
		//����ǵ�һ������Ҫ��������ֱ��д�룬�����ؼ�������
		if( rCount == 0 )
		{
			m_pTargets->m_aObj[rCount]	= pObj;
			m_aDistance[rCount]			= fDistance;
			++rCount;
			return SCANRETURN_CONTINUE;
		}
		//������ǵ�һ����������а�����������򲢲��룬���ؼ�������
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
		//û�бȵ�ǰ�������Զ�ģ�����������
		m_pTargets->m_aObj[rCount]	= pObj;
		m_aDistance[rCount]			= fDistance;
		++rCount;
	}
	//�������replace
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





































