#include "StdAfx.h"
#include "WorldManager.h"
#include "Path.h"
#include "../Procedure/GameProcedure.h"
#include "GIUtil.h"
#include "GIException.h"

#undef min
#undef max
#include "PathLibInterface.h"

CPath::CPath()
{
}

CPath::~CPath()
{
}

void CPath::Clean(void)
{	
	m_vPosStack.clear();	
}

CPath::PathUnit& CPath::GetPathUnit(INT nIndex)
{
	return m_vPosStack[nIndex];
}

const CPath::PathUnit& CPath::GetConstPathUnit(INT nIndex)const
{
	return const_cast<CPath*>(this)->GetPathUnit( nIndex );
}

// 计算从开始点到结束点的路径 2006-4-17
BOOL CPath::CreateMovePath(const fVector2& fvCurrent, const fVector2& fvTarget)
{      
	KLU_Log("Begin CreateMovePath : %f,%f,%f,%f", fvCurrent.x, fvCurrent.y,
		fvTarget.x, fvTarget.y);

	m_vPosStack.clear();

	CScene* pActiveScene = (CScene*)CWorldManager::GetMe()->GetActiveScene();

	if( pActiveScene == NULL ) 
		return FALSE;

	if( FALSE == pActiveScene->IsValidPosition(fvCurrent) )
		KLThrow("CPath::CreateMovePath() current position is invalid : %f,%f", fvCurrent.x, fvCurrent.y);

	if( FALSE == pActiveScene->IsValidPosition(fvTarget) )
		KLThrow("CPath::CreateMovePath() target position is invalid : %f,%f", fvTarget.x, fvTarget.y);

	bool find = pActiveScene->mPathFinder->findPath(fvCurrent.x, fvCurrent.y, fvTarget.x, fvTarget.y);

	int pathCount = 0;

	if (find)
	{   
		size_t count = pActiveScene->mPathFinder->getNumPathPoints();

		pathCount = static_cast<int>(count);

		KLU_Log("CreateMovePath : %f,%f,%f,%f,%d,%s", fvCurrent.x, fvCurrent.y,
			fvTarget.x, fvTarget.y, pathCount, find ? "find" : "not find");

		if (count > 16)
		{
			KLU_Log("CreateMovePath : %f,%f,%f,%f,%d,path unit > 16", fvCurrent.x, fvCurrent.y,
				fvTarget.x, fvTarget.y, pathCount);
		}

		for (size_t i=0; i<count-1; ++i)
		{
			const PathLib::ResultPoint& point1 = pActiveScene->mPathFinder->getResultPoint(i);
			const PathLib::ResultPoint& point2 = pActiveScene->mPathFinder->getResultPoint(i+1);

			PathUnit pathUnit;

			pathUnit.fvStart.x = point1.x;
			pathUnit.fvStart.y = point1.y;

			pathUnit.fvTarget.x = point2.x;
			pathUnit.fvTarget.y = point2.y;
			m_vPosStack.push_back(pathUnit);
		}

	}

	return find;
}

//
// 但前点是否在不可行走区域之内
//
BOOL CPath::IsPointInUnreachRegion(const fVector2& fvTarget)
{
	CScene* pActiveScene = (CScene*)CWorldManager::GetMe()->GetActiveScene();

	if( pActiveScene == NULL ) 
		return FALSE;

	//检测坐标是否非法
	if( FALSE == pActiveScene->IsValidPosition(fvTarget) )
		//     KLThrow("CPath::IsPointInUnreachRegion() target position is invalid : %f,%f", fvTarget.x, fvTarget.y);
		// 不抛异常，如果目标点非法，直接返回true
		return TRUE;

	unsigned int id;
	return pActiveScene->mPathFinder->isPointInRegion(fvTarget.x, fvTarget.y, id, 1);
}

BOOL CPath::GetPathDirect(const fVector2& fvCurrent, const fVector2& fvTarget, const BOOL bUseFly)
{
	KLU_Log("GetPathDirect Begin : %f,%f,%f,%f,%s", fvCurrent.x, fvCurrent.y, fvTarget.x, fvTarget.y,
		bUseFly ? "use fly" : "not use fly");

	m_vPosStack.clear();

	CScene* pActiveScene = (CScene*)CWorldManager::GetMe()->GetActiveScene();

	if( pActiveScene == NULL ) 
		return FALSE;

	if( FALSE == pActiveScene->IsValidPosition(fvCurrent) )
		KLThrow("CPath::GetPathDirect() current position is invalid : %f,%f", fvCurrent.x, fvCurrent.y);

	if( FALSE == pActiveScene->IsValidPosition(fvTarget) )
		KLThrow("CPath::GetPathDirect() target position is invalid : %f,%f", fvTarget.x, fvTarget.y);

	bool find = pActiveScene->mPathFinder->getDirectPath(fvCurrent.x, fvCurrent.y, fvTarget.x, fvTarget.y);

	if (!find)
		return FALSE;    

	const PathLib::IntersectPoints& intersectPoints = pActiveScene->mPathFinder->getIntersectPoints();

	unsigned int startID, endID;

	if (pActiveScene->mPathFinder->isPointInRegion(fvCurrent.x, fvCurrent.y, startID, 1) &&
		pActiveScene->mPathFinder->isPointInRegion(fvTarget.x, fvTarget.y, endID, 0))
	{
		int intersectSize = static_cast<int>(intersectPoints.size());

		KLU_Log("GetPathDirect : %f,%f,%f,%f,%d, move from obstacle to nonobstacle",
			fvCurrent.x, fvCurrent.y, fvTarget.x, fvTarget.y, intersectSize);

		if (intersectSize == 1 || intersectSize == 0)
		{
			PathUnit newPathUnit;
			newPathUnit.fvStart	= fvCurrent;
			newPathUnit.fvTarget = fvTarget;
			m_vPosStack.push_back(newPathUnit);

			return TRUE;

		}
		else
		{
			return FALSE;
		}
	}   

	fVector2 vResult;
	vResult = fvTarget;

	bool directToGoal = true;

	if( intersectPoints.size() > 0 )
	{
		if( bUseFly )
		{
			//轻功寻路,查找第一个不能用轻功通过的点pos0
			fVector2 posFirstUnFlyable;
			bool findFirstUnFlyable = false;

			for( PathLib::IntersectPoints::const_iterator pos0 = intersectPoints.begin(); pos0 != intersectPoints.end(); ++ pos0 )
			{
				if( (pos0->second.flag & 0x2) != 0 == false )
				{
					findFirstUnFlyable = true;
					posFirstUnFlyable.x = pos0->second.intersectPos.x;
					posFirstUnFlyable.y = pos0->second.intersectPos.y;
					break;
				}
			}

			if( !findFirstUnFlyable ) //所有的点都可以飞过
			{
				if( IsPointInUnreachRegion( fvTarget ) == FALSE ) //如果终点在可走区域,则一下子飞到终点
				{
					vResult = fvTarget;
				}
				else 
				{
					directToGoal = false;

					vResult.x = intersectPoints.begin()->second.intersectPos.x;
					vResult.y = intersectPoints.begin()->second.intersectPos.y;
				}
			}
			else
			{
				directToGoal = false;

				vResult.x = intersectPoints.begin()->second.intersectPos.x;
				vResult.y = intersectPoints.begin()->second.intersectPos.y;           
			}
		}
		else
		{
			directToGoal = false;

			vResult.x = intersectPoints.begin()->second.intersectPos.x;
			vResult.y = intersectPoints.begin()->second.intersectPos.y;
		}
	}

	PathUnit newPathUnit;
	newPathUnit.fvStart	= fvCurrent;
	newPathUnit.fvTarget = vResult;    

	if (!directToGoal)
	{
		fVector2 dir = newPathUnit.fvStart - newPathUnit.fvTarget;
		dir.normalise();

		fVector2 offset;
		offset.x = newPathUnit.fvTarget.x + dir.x * 0.1f;
		offset.y = newPathUnit.fvTarget.y + dir.y * 0.1f;

		if ( pActiveScene->mPathFinder->isPointInRegion(offset.x, offset.y, startID) )
		{
			KLU_Log("GetPathDirect : %f,%f,in obstacle", offset.x, offset.y);
		}
		else
		{
			newPathUnit.fvTarget = offset;
		}
	}

	//不知道这句有什么作用,反正导致距离平方小于2无法移动.
	//if(KLU_GetDistSq(newPathUnit.fvStart, newPathUnit.fvTarget) < 2.0f)
	//	return FALSE;

	m_vPosStack.push_back(newPathUnit);

	KLU_Log("GetPathDirect : %f,%f,%f,%f,%s", fvCurrent.x, fvCurrent.y, fvTarget.x, fvTarget.y,
		bUseFly ? "use fly" : "not use fly");

	return TRUE;
}

INT CPath::GetUnitNum(VOID)const 
{ 
	return static_cast<INT>(m_vPosStack.size());
}

FLOAT CPath::fvDistancePow(fVector2 fv1, fVector2 fv2)
{
	return (fv1.x-fv2.x) * (fv1.x-fv2.x) + (fv1.y-fv2.y) * (fv1.y-fv2.y) ;
}