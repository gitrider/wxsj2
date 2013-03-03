
/** Region.cpp
 */

#include "StdAfx.h"
#include "Region.h"
#include "Path.h"
#include "GIUtil.h"
#include "Type.h"
#include <math.h>
#include "DynamicLines.h"
#include "OgreSceneNode.h"
#include "Engine\EngineInterface.h"
#include "Core\FairySystem.h"
#include "OgreNode.h"

CRegion::CRegion( VOID )
{
	m_nCount	= 0;
	m_ID		= INVALID_ID;
	m_bDynamic	= false;
}

CRegion::~CRegion( VOID )
{
}


BOOL CRegion::AddPoint( const VisibilityPoints *pfvPoint )
{
	m_vfPoints.push_back( *pfvPoint );
	m_nCount = (INT)m_vfPoints.size();

	return true;
}

BOOL CRegion::IsBulge( INT nIndex )const
{
	if ( ( nIndex < 0 ) || ( nIndex >= (INT)m_vbBulge.size() ) )
	{
		return FALSE;
	}
	return m_vbBulge[nIndex];
}

BOOL CRegion::JudgeBulge( INT nIndex )
{
	if ( nIndex >= m_nCount )
	{
		return FALSE;
	}
	if( m_nCount < 3 )
	{
		//多边形不合法
		return FALSE;
	}
	if ( m_nCount == 3 )
	{
		//三角形情况
		return true;
	}

	INT nIndex1, nIndex2, nIndex3;
	nIndex1 = nIndex - 1;
	nIndex2 = nIndex;
	nIndex3 = nIndex + 1;
	if ( nIndex1 < 0 )
	{
		nIndex1 = (INT)(m_vfPoints.size()) - 1;
	}
	if ( nIndex3 >= (INT)(m_vfPoints.size()) )
	{
		nIndex3 = 0;
	}

	fVector2 fvLine1, fvLine2;
	fvLine1.x = m_vfPoints[nIndex1].m_position.x - m_vfPoints[nIndex2].m_position.x;
	fvLine1.y = m_vfPoints[nIndex1].m_position.y - m_vfPoints[nIndex2].m_position.y;
	fvLine2.x = m_vfPoints[nIndex3].m_position.x - m_vfPoints[nIndex2].m_position.x;
	fvLine2.y = m_vfPoints[nIndex3].m_position.y - m_vfPoints[nIndex2].m_position.y;
	FLOAT fAngle1 = atan( (-fvLine1.y) / fvLine1.x );
	FLOAT fAngle2 = atan( (-fvLine2.y) / fvLine2.x );

	if ( fvLine1.x < 0.f )
	{
		fAngle1 += (FLOAT)KLU_PI;
	}

	if ( fvLine2.x < 0.f )
	{
		fAngle2 += (FLOAT)KLU_PI;
	}

	if ( fAngle1 < 0.f )
		fAngle1 = (FLOAT)KLU_PI * 2 + fAngle1;

	if ( fAngle2 < 0.f )
		fAngle2 = (FLOAT)KLU_PI * 2 + fAngle2;

	FLOAT fAngle = fAngle2 - fAngle1;
	if ( fAngle >= 0.f )
	{
		if ( fAngle < (FLOAT)KLU_PI )
			return true;
		else
			return FALSE;
	}
	else
	{
		if ( fAngle < (FLOAT)(-KLU_PI) )
			return true;
		else
			return FALSE;
	}

	return FALSE;
}

//BOOL CRegion::JudgeInside( const fVector2 *pfvPoint )
//{
//	FLOAT fAxisX = pfvPoint->x;
//	FLOAT fAxisY = pfvPoint->y;	
//	fVector2 sp,ep;
//	INT i, nPassBy;
//	nPassBy = 0;
//	for ( INT i = 0; i < m_nCount; i++ )
//	{
//		sp = m_vfPoints[i];
//		ep = m_vfPoints[(i+1)%m_nCount];
//	}
//	if ( nPassBy % 2 == 0)
//	{		
//		return FALSE;
//	}	
//	return true;
//}

VOID CRegion::JudgeAllBulge( VOID )
{
	m_vbBulge.clear();
	INT i;
	BOOL bBulge;

	for( i = 0; i < (INT)m_vfPoints.size(); i++ )
	{
		bBulge = JudgeBulge( i );
		m_vbBulge.push_back( bBulge );
	}
}

INT CRegion::GetIndex( const fVector2 *pfvPoint )const
{
	for ( INT i = 0; i < (INT)m_vfPoints.size(); i++ )
	{
		if ( ( abs( pfvPoint->x - m_vfPoints[i].m_position.x ) < 1e-10 )
			&& ( abs( pfvPoint->y - m_vfPoints[i].m_position.y ) < 1e-10 ) )
		{
			return i;
		}
	}
	return -1;
}
INT CRegion::GetNeighborBulgeIndex( INT nIndex, BOOL bCountSelf, BOOL bClockwise )const
{
	if( ( m_vfPoints.empty() ) || ( m_vbBulge.empty() ) )
	{
		return -1;
	}

	if( ( nIndex < 0 ) || ( nIndex >= (INT)m_vfPoints.size() ) )
	{
		return -1;
	}

	INT ret = (bCountSelf)?(nIndex):( (bClockwise)?(nIndex+1):(nIndex-1) );

	INT i, nSize;
	nSize = (INT)m_vfPoints.size();
	for( i = 0; i < nSize; i++ )
	{
		if( ret >= nSize )
		{
			ret = 0;
		}

		if( ret < 0 )
		{
			ret = nSize - 1;
		}

		if( m_vbBulge[ret] )
		{
			return ret;
		}

		(bClockwise)?(ret++):(ret--);

	}

	return -1;
}

INT CRegion::GetNeighborBulgeIndex( const fVector2 *pfvPoint, BOOL bCountSelf, BOOL bClockwise )const
{
	INT nIndex = GetIndex( pfvPoint );
	if( nIndex >= 0 )
	{
		return GetNeighborBulgeIndex( nIndex, bCountSelf, bClockwise );
	}

	return -1;
}

const fVector2* CRegion::GetNeighborBulge( INT nIndex, BOOL bCountSelf, BOOL bClockwise )const
{
	INT ret = GetNeighborBulgeIndex( nIndex, bCountSelf, bClockwise );
	if( ret >= 0 )
	{
		return &m_vfPoints[ret].m_position;
	}

	return NULL;	
}

const fVector2* CRegion::GetNeighborBulge( const fVector2 *pfvPoint, BOOL bCountSelf, BOOL bClockwise )const
{
	INT nIndex = GetIndex( pfvPoint );
	if( nIndex >= 0 )
	{
		return GetNeighborBulge( nIndex, bCountSelf, bClockwise );
	}

	return NULL;
}

FLOAT CRegion::GetDistanceSq( INT nStart, INT nEnd, BOOL bClockwise, BOOL bOnlyBugle )const
{	
	// 起点和终点是多边形的凸点(这个条件是否需要,还是任何点都可以?商榷)
	if( bOnlyBugle && ( !IsBulge( nStart ) || !IsBulge( nEnd ) ) )
		return -1.f;

	FLOAT ret = 0;
	INT m = nStart;
	INT n = nStart;

	while( n != nEnd )
	{
		n = GetNeighborBulgeIndex( m, FALSE, bClockwise );
		ret += CPath::fvDistancePow( m_vfPoints[m].m_position, m_vfPoints[n].m_position );
		m = n;
	}

	return ret;
}

const fVector2* CRegion::GetPoint( INT nIndex, BOOL bLoop )const
{
	if( m_vfPoints.empty() )
	{
		return NULL;
	}

	if( bLoop )
	{
		nIndex = nIndex % (INT)m_vfPoints.size();
	}
	else if( nIndex >= (INT)m_vfPoints.size() )
	{
		return NULL;
	}

	return &m_vfPoints[nIndex].m_position;
}

const fVector2* CRegion::GetPoint( INT nIndex )const
{
	return GetPoint( nIndex, true );
}


//INT CRegion::GetLastBugle(INT startIndex,fVector2 endPoint,BOOL clockwise)
//{
//	//得到按照时钟顺序的最后一个凸点(直达终点)
//	//是否对endPoint是这个Region的内点进行判断?	
//	INT x;
//	for (INT i = 0;i<(INT)m_vfPoints.size();i++)
//	{
//		x = clockwise?(startIndex+i):(startIndex-i);
//		if (x<0)
//		{
//			x= (INT)m_vfPoints.size()-1;
//		}
//		if (x>=(INT)m_vfPoints.size())
//		{
//			x=0;
//		}
//		if (!CPath::CheckCross(m_vfPoints[i],endPoint,this))
//		{
//			//不相交
//			return x;
//		}
//	}
//	return -1;
//}
