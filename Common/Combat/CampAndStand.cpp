///////////////////////////////////////////////////////////////////////////////
// 文件名：CampAndStand.cpp
// 程序员：高骐
// 功能说明：阵营数据表结构和功能实现
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
/** \file CampAndStand.cpp
*		\brief the implement of camp and stand structure
*/
#include "stdafx.h"
#include "CampAndStand.h"
#include "GameUtil.h"
#include "DB_Struct.h"
#include "WXSJ_DBC.h"

namespace Combat_Module
{
	namespace Camp_Stand
	{
		using namespace DBC;

		//class CampStandRecord_T
		CampStandRecord_T::CampStandRecord_T() : m_bInited(FALSE)
		{
			memset((VOID*)m_aStands, (INT)FALSE, sizeof(m_aStands));
		}
		BOOL CampStandRecord_T::InitFromDB(const DBCFile * pDB, INT const nRow)
		{
			__ENTER_FUNCTION
			Assert(pDB);
			Assert(0<=nRow);
			INT column = 1;
			for(INT nIdx=0;MAX_ID>=nIdx;++nIdx)
			{
				m_aStands[nIdx]= pDB->Search_Posistion(nRow, column++)->iValue != 0;
			}
			m_bInited = TRUE;
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL CampStandRecord_T::StandByCampID(CampID_t const nCampID)const
		{
			__ENTER_FUNCTION
			if(0<=nCampID&&MAX_ID>=nCampID)
			{
				return m_aStands[nCampID];
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	}
}

ENUM_RELATION CalcRelationType( const _CAMP_DATA *pCamp_A, BOOL bHuman_A, const _CAMP_DATA *pCamp_B, BOOL bHuman_B, const CampAndStandDataMgr_T *pCampMgr )
{
	__ENTER_FUNCTION

	ENUM_RELATION eRelation_A, eRelation_B;

	if ( pCamp_A == NULL || pCamp_B == NULL )
		return RELATION_ENEMY;

	// 对A来说
	if ( pCamp_A->IsActiveFlag_Dirty( pCamp_B->m_nCampID ) )
	{
		if ( pCamp_A->IsRelationFlag_Dirty( pCamp_B->m_nCampID ) )
		{
			eRelation_A = RELATION_FRIEND;
		}
		else
		{
			eRelation_A = RELATION_ENEMY;
		}
	}
	else
	{
		const CampStandRecord_T *pCampAndStand;
		pCampAndStand = pCampMgr->GetInstanceByID( pCamp_A->m_nCampID );
		if ( pCampAndStand != NULL )
		{
			if ( pCampAndStand->StandByCampID( pCamp_B->m_nCampID ) )
			{
				eRelation_A = RELATION_ENEMY;
			}
			else
			{
				eRelation_A = RELATION_FRIEND;
			}
		}
		else
		{
			eRelation_A = RELATION_INVALID;
		}
	}

	// 对B来说
	if ( pCamp_B->IsActiveFlag_Dirty( pCamp_A->m_nCampID ) )
	{
		if ( pCamp_B->IsRelationFlag_Dirty( pCamp_A->m_nCampID ) )
		{
			eRelation_B = RELATION_FRIEND;
		}
		else
		{
			eRelation_B = RELATION_ENEMY;
		}
	}
	else
	{
		const CampStandRecord_T *pCampAndStand;
		pCampAndStand = pCampMgr->GetInstanceByID( pCamp_B->m_nCampID );
		if ( pCampAndStand != NULL )
		{
			if ( pCampAndStand->StandByCampID( pCamp_A->m_nCampID ) )
			{
				eRelation_B = RELATION_ENEMY;
			}
			else
			{
				eRelation_B = RELATION_FRIEND;
			}
		}
		else
		{
			eRelation_B = RELATION_INVALID;
		}
	}

	if ( eRelation_A == eRelation_B )
	{
		return eRelation_A;
	}
	else
	{
		if ( bHuman_A && bHuman_B )
		{
			return RELATION_ENEMY;
		}
		else if ( bHuman_A )
		{
			return eRelation_A;
		}
		else if ( bHuman_B )
		{
			return eRelation_B;
		}
		else
		{
			return RELATION_ENEMY;
		}
	}
	__LEAVE_FUNCTION
		return RELATION_ENEMY;
}
