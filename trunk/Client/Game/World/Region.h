
/** Region.h
 *	不可行走范围Region
 */

#ifndef __REGION_H__
#define __REGION_H__


#include <vector>
#include "GIMath.h"

class tEntityNode;

class CRegion
{
public:

	struct VisibilityPoints //  可视点结构
	{
		fVector2					m_position;
		int							m_belongRegionID;
		std::vector<const VisibilityPoints*>	m_visibilityPointsList;
	};

	CRegion( VOID );
	~CRegion( VOID );

	BOOL AddPoint( const VisibilityPoints *pfvPoint );
	BOOL IsBulge( INT nIndex )const;
	BOOL JudgeBulge( INT nIndex );
	VOID JudgeAllBulge( VOID );

	// 函数废弃:功能未实现
	//BOOL JudgeInside( const fVector2 *pfvPoint );

	//得到一个顶点的序号,如果这个顶点不在多边形上,返回-1
	INT GetIndex( const fVector2 *pfvPoint ) const;

	/** 得到相邻的凸点 */
	INT GetNeighborBulgeIndex( INT nIndex, BOOL bCountSelf, BOOL bClockwise ) const;
	INT GetNeighborBulgeIndex( const fVector2 *pfvPoint, BOOL bCountSelf, BOOL bClockwise ) const;
	const fVector2* GetNeighborBulge( INT nIndex, BOOL bCountSelf, BOOL bClockwise ) const;
	const fVector2* GetNeighborBulge( const fVector2 *pfvPoint, BOOL bCountSelf, BOOL bClockwise ) const;

	/** 计算从nStart点到nEnd点绕边经过的距离 */
	FLOAT GetDistanceSq( INT nStart, INT nEnd, BOOL bClockwise, BOOL bOnlyBugle ) const;

	const fVector2* GetPoint( INT nIndex, BOOL bLoop ) const;
	const fVector2* GetPoint( INT nIndex ) const;

	// 函数废弃:用不上
	//INT GetLastBugle(INT startIndex,fVector2 endPoint,BOOL clockwise);

public:

	INT						m_ID;			// ID应该没必要从文件中读入
	INT						m_nCount;
	bool					m_bDynamic;		/// 是否为动态的
											//  （有些区域在一定的条件下会消失掉）

	std::vector<VisibilityPoints>	m_vfPoints;
	std::vector<BOOL>				m_vbBulge;

};	// class


#endif // __REGION_H__
