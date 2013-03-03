/************************************************************************* 
 �ļ���	    : AvoidOverlap.h 	
 �汾�� 	: 0.0.1
 ������		: FrankWu
 ��  ��		: ����Human��Pet���������������ܶ�ʱObj���ص�����
 �޸ļ�¼	: 
*************************************************************************/
#ifndef _AVOID_OVERLAP_
#define _AVOID_OVERLAP_

#include "Type.h"
#include "AI_Character.h"
#include "GameUtil.h"

class AvoidOverlap
{
public:
	enum 
	{
		MAX_FRACTION = 16,
		MAX_MOVETIMES = 4,
		MAX_USEDTIMES = 255,
	};
	
	AvoidOverlap( VOID )
	{
		memset( m_isUsed, 0, sizeof(BYTE)*MAX_FRACTION );
		InitCosSin( );
	}
	
	virtual ~AvoidOverlap( VOID ) {}

	/** Desc				:�õ����﹥����ҵ�Ŀ������
	 *  @param rTar			:Ŀ������
	 *  @param rMonsterPos	:��������
	 *  @param rPlayerPos	:�������
	 *  @param fMaxDist		:������󹥻�����
	 *  @param fMinDist		:������С��������
	 *  @return				:
	 */
	VOID GetDirPos( WORLD_POS& rTar, 
					const WORLD_POS& rMonsterPos, 
					const WORLD_POS& rPlayerPos,
					FLOAT fMaxDist,
					FLOAT fMinDist )
	{
	__ENTER_FUNCTION
		/** �ù�������ҵ�λ��ȷ��Ҫ��λ�ķ�Χ ��L1��L2֮��ķ��� M��L1��L2֮��ĽǶȶ���PI/4  */
		/**
		 *        L1|  /M
		 *			| /
		 *		   P|/________L2
		 */
		rTar = rPlayerPos; // ����ȷ��Ŀ�����һ����Ч�ĵ�
		INT nStartIndex = 0,nEndIndex = 0,nIndex = 0;
		FLOAT vX = rPlayerPos.m_fX - rMonsterPos.m_fX;
		FLOAT vZ = rPlayerPos.m_fZ - rMonsterPos.m_fZ;
		FLOAT fDist = MySqrt( &rMonsterPos, &rPlayerPos );
		if (fDist < 0.001)
		{
			return;
		}
		FLOAT fCos = vX / fDist;
		FLOAT fSin = vZ / fDist;
		/** ȷ����ʼ,����λ�� */
		FLOAT fRet = ::asin( fSin );
		if ( fRet > 0 && vX < 0 )
		{
			fRet += __PI/2;
		}
		else if ( fRet < 0 && vX < 0 )
		{
			fRet -= __PI/2;
		}

		if ( fRet < 0 )
		{
			fRet += 2 * __PI;
		}

		FLOAT fStart = fRet - __PI / 4;
		fStart = fStart > 0 ? fStart : ( 2 * __PI + fStart );
		FLOAT fEnd = fRet + __PI / 4;
		fEnd = fEnd > 2 * __PI ? ( fEnd - 2*__PI ) : fEnd;
		nStartIndex = GetIndex( fStart );
		nEndIndex = GetIndex( fEnd );
		INT nUsedCount = 0;
		/** ��Ȼ��nStartIndex�ڵ�������nEnd�ڵ�һ���� */
		nIndex = GetBestPos( nStartIndex, nEndIndex, nUsedCount );
		if (FALSE == HaveMonsterNearly(nIndex))
		{// �������û��monster��ԭ�ز���
			rTar = rMonsterPos;
			return;
		}
		/** �����__PI/2��Χ�ڹ��ﳬ��3�� ��ƫ��__PI/2����ɸѡ ֱ���ҵ������Ļ�����Ѱ��һȦ */
		if ( MAX_FRACTION / 4 - 2 < nUsedCount )
		{
			/** �������Թ�����һ�ߵ������� */
			INT nDir = ( rand() % 2 ) > 0 ? 4 : -4 ;
			for ( INT i = 0; MAX_MOVETIMES > i; ++i )
			{
				nUsedCount = 0;
				nStartIndex += nDir;
				nStartIndex = ( nStartIndex + MAX_FRACTION ) % MAX_FRACTION;
				nEndIndex += nDir;
				nEndIndex = ( nEndIndex + MAX_FRACTION ) % MAX_FRACTION;
				
				nIndex = GetBestPos( nStartIndex, nEndIndex, nUsedCount );
				if ( MAX_FRACTION / 4 - 2 > nUsedCount )
				{
					break;
				}
			}
		}

		/** ȷ������ҵľ���,����Ϊ����������С����֮�������ֵ */
		rTar.m_fX = rPlayerPos.m_fX + fMinDist * GetCos( nIndex );
		rTar.m_fZ = rPlayerPos.m_fZ + fMinDist * GetSin( nIndex );
		
	__LEAVE_FUNCTION
	}
	
	VOID ResetUsedDir( VOID )
	{
		memset( m_isUsed, 0, MAX_FRACTION );
	}

protected:
	INT GetMinIndex( INT nStart, INT nEnd, INT& nMinIndex, INT& nUsedCount )
	{
	__ENTER_FUNCTION
		Assert( MAX_FRACTION > nStart && 0 <= nStart && MAX_FRACTION > nEnd && 0 <= nEnd );
		/** ��ż�ֿ�����Ч�� */
		for ( INT i = nStart; i <= nEnd; )
		{
			if ( m_isUsed[nMinIndex] > m_isUsed[i] ) 
			{
				nMinIndex = i;
            }
			if( m_isUsed[i] )
			{
				++nUsedCount;
			}
			i += 2;
		}
		for (INT i = nStart+1; i <= nEnd; )
		{
			if ( m_isUsed[nMinIndex] > m_isUsed[i] ) 
			{
				nMinIndex = i;
			}
			if ( m_isUsed[i] )
			{
				++nUsedCount;
			}
			i += 2;
		}

		++m_isUsed[nMinIndex];

		if ( MAX_USEDTIMES == m_isUsed[nMinIndex] ) 
		{
			m_isUsed[nMinIndex] = 0;
		}

		return nMinIndex;
	__LEAVE_FUNCTION
		return nMinIndex;
	}

	INT GetBestPos( INT nStartIndex, INT nEndIndex, INT& nUsedCount ) 
	{
	__ENTER_FUNCTION
		INT nIndex = 0;
		/** �ҵ����ŵķ��� */
		INT nMinIndex = nStartIndex;
		INT nStepLen = ( nEndIndex - nStartIndex ) > 0 ? ( nEndIndex - nStartIndex ) / 2 : ( nEndIndex - nStartIndex + MAX_FRACTION ) / 2;
		nMinIndex += nStepLen;
		if (nMinIndex >= MAX_FRACTION)
		{// ��Ч�Լ��
			nMinIndex -= MAX_FRACTION; 
		}
		if ( nStartIndex > nEndIndex )
		{
		/** nStartIndex��Ȼ���ڵ������� */
			INT Min1 = GetMinIndex( 0, nEndIndex, nMinIndex, nUsedCount );
			INT Min2 = GetMinIndex( nStartIndex, MAX_FRACTION - 1, nMinIndex, nUsedCount );
			if (m_isUsed[Min1] > m_isUsed[Min2])
			{
				nIndex = Min2;
				--m_isUsed[Min1];
			}
			else
			{
				nIndex = Min1;
				--m_isUsed[Min2];
			}
		}
		else 
		{
			nIndex = GetMinIndex( nStartIndex, nEndIndex, nMinIndex, nUsedCount );
		}
		return nIndex;
	__LEAVE_FUNCTION
		return 0;
	}

	INT GetPosIfNoMonster( FLOAT fvalue ) 
	{
	__ENTER_FUNCTION
		for ( INT i = 0; MAX_FRACTION > i; ++i ) 
		{
			if ( 2 * i * __PI / MAX_FRACTION >= fvalue ) 
			{
				FLOAT fStep1 = fabs( 2 * i * __PI / MAX_FRACTION - fvalue );
				INT j = i-1 < 0 ? MAX_FRACTION-1 : i-1;
				FLOAT fStep2 = fabs( 2  * j * __PI / MAX_FRACTION - fvalue );
				INT m = i+1 > MAX_FRACTION-1 ? MAX_FRACTION-i-1 : i+1;
				INT n = j-1 < 0 ? MAX_FRACTION-1 : j-1;
				if( 0 == m_isUsed[i] 
					&& 0 == m_isUsed[j]
					&& 0 == m_isUsed[m]
					&& 0 == m_isUsed[n] )
				{
					return fStep1 > fStep2 ? j : i;
				}
				else
				{
					return -1;
				}
			}
		}
		return -1 ;
	__LEAVE_FUNCTION
		return -1 ;
	}
	// ��Ч�Լ��
	VOID VerifyIndex(INT& nIndex)
	{
	__ENTER_FUNCTION
		if (nIndex >= MAX_FRACTION)
		{
			nIndex -= MAX_FRACTION; 
		}
		if (nIndex < 0)
		{
			nIndex += MAX_FRACTION;
		}
	__LEAVE_FUNCTION
	}

	BOOL HaveMonsterNearly(INT nIndex)
	{
	__ENTER_FUNCTION
		// ��nIndex����������(����nIndex)���м��
		INT nTempIndex = nIndex-1;
		VerifyIndex(nTempIndex);
		if (m_isUsed[nTempIndex] > 0)
			return TRUE;

		nTempIndex = nIndex;
		VerifyIndex(nTempIndex);
		// ��Ϊ�ڴ˺�����������ǰ�Ѿ����ۼӹ���
		if (m_isUsed[nTempIndex] > 1)
			return TRUE;

		nTempIndex = nIndex+1;
		VerifyIndex(nTempIndex);
		if (m_isUsed[nTempIndex] > 0)
			return TRUE;

		return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
	}

	VOID InitCosSin( VOID ) 
	{
		for ( INT i = 0; MAX_FRACTION > i; ++i )
		{
			m_fSin[i] = sin( i * 2 * __PI / MAX_FRACTION );
			m_fCos[i] = sin( i * 2 * __PI / MAX_FRACTION + __HALF_PI);
		}
	}

	FLOAT GetCos( INT index ) const
	{
		if (index < 0 || index >= MAX_FRACTION)
		{
			Assert(NULL && "AvoidOverlap...GetCos()...index=invalid...");
			return 0.f;
		}
		return m_fCos[index];
	}

	FLOAT GetSin( INT index ) const
	{
		if (index < 0 || index >= MAX_FRACTION)
		{
			Assert(NULL && "AvoidOverlap...GetSin()...index=invalid...");
			return 0.f;
		}
		return m_fSin[index];
	}

	INT GetIndex( FLOAT fValue )
	{
		INT i;
		for(i = 0; MAX_FRACTION > i; ++i )
		{
			if ( i * 2 * __PI / MAX_FRACTION >= fValue ) 
			{
				return i;			
			}
		}
		if ( MAX_FRACTION == i )
		{
			return 0;
		}

		return -1;
	}

private:
	BYTE  m_isUsed[MAX_FRACTION];
	FLOAT m_fCos[MAX_FRACTION];
	FLOAT m_fSin[MAX_FRACTION];

};

#endif