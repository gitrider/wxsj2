
/** Region.h
 *	�������߷�ΧRegion
 */

#ifndef __REGION_H__
#define __REGION_H__


#include <vector>
#include "GIMath.h"

class tEntityNode;

class CRegion
{
public:

	struct VisibilityPoints //  ���ӵ�ṹ
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

	// ��������:����δʵ��
	//BOOL JudgeInside( const fVector2 *pfvPoint );

	//�õ�һ����������,���������㲻�ڶ������,����-1
	INT GetIndex( const fVector2 *pfvPoint ) const;

	/** �õ����ڵ�͹�� */
	INT GetNeighborBulgeIndex( INT nIndex, BOOL bCountSelf, BOOL bClockwise ) const;
	INT GetNeighborBulgeIndex( const fVector2 *pfvPoint, BOOL bCountSelf, BOOL bClockwise ) const;
	const fVector2* GetNeighborBulge( INT nIndex, BOOL bCountSelf, BOOL bClockwise ) const;
	const fVector2* GetNeighborBulge( const fVector2 *pfvPoint, BOOL bCountSelf, BOOL bClockwise ) const;

	/** �����nStart�㵽nEnd���Ʊ߾����ľ��� */
	FLOAT GetDistanceSq( INT nStart, INT nEnd, BOOL bClockwise, BOOL bOnlyBugle ) const;

	const fVector2* GetPoint( INT nIndex, BOOL bLoop ) const;
	const fVector2* GetPoint( INT nIndex ) const;

	// ��������:�ò���
	//INT GetLastBugle(INT startIndex,fVector2 endPoint,BOOL clockwise);

public:

	INT						m_ID;			// IDӦ��û��Ҫ���ļ��ж���
	INT						m_nCount;
	bool					m_bDynamic;		/// �Ƿ�Ϊ��̬��
											//  ����Щ������һ���������»���ʧ����

	std::vector<VisibilityPoints>	m_vfPoints;
	std::vector<BOOL>				m_vbBulge;

};	// class


#endif // __REGION_H__
