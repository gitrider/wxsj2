
/** ���ߺ�����
 */

#pragma once


#include "GIBasicType.h"
#include "GIMath.h"




/** ����Log */
void KYLIN_API KLU_Log(LPCSTR pszFmt, ...);


/** �ַ������
 *	���ַ������չؼ��ַָ�
 */
INT KYLIN_API KLU_ConvertStringToVector(LPCTSTR strStrintgSource, std::vector< STRING >& vRet, LPCTSTR szKey=_T("\\/"), BOOL bOneOfKey = TRUE, BOOL bIgnoreEmpty = TRUE);


/** ��ѧ���
 */
#define KLU_PI			(3.1415926535)
#define KLU_MINFLOAT	(1.0E-9)


/** ȡ�������ľ��� */
FLOAT KYLIN_API KLU_GetDist(const fVector2& fvPos1, const fVector2& fvPos2);
FLOAT KYLIN_API KLU_GetDist(const fVector3& fvPos1, const fVector3& fvPos2);

/** ȡ�������ľ���ƽ�� */
FLOAT KYLIN_API KLU_GetDistSq(const fVector2& fvPos1, const fVector2& fvPos2);
FLOAT KYLIN_API KLU_GetDistSq(const fVector3& fvPos1, const fVector3& fvPos2);

template<class T, class U, class V>	inline void KLU_Clamp(T &v, const U &min, const V &max)
{
	v = (v < min) ? min : v;
	v = (v > max) ? max : v;
}


/*
|
|	�����������ĵ��
|
*/
inline float KLU_DotProduct(const fVector2& vec1, const fVector2& vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

/*
|
|	ȡ�ô�Pos1��Pos2��ʸ�������Y�����ת�Ƕ�, ��z��Ϊ0��
|   
|              ->x
|                  o pos1
|          |       |\
|       z  v       |  \
|                  |    \
|                  |      \
|                  |        o pos2
|
|  Return [0 ~ 2pi)
*/

FLOAT KYLIN_API KLU_GetYAngle(const fVector2& fvPos1, const fVector2& fvPos2);

/*
|
|	ȡ�ô�Pos1��Pos2֮������ĵ�
|        
|             ->x
|                    o pos2
|          |       /    
|       z  v      o <- This point!    
|               /    
|              o 
|             pos1        
|
*/
fVector3 KYLIN_API KLU_GetCenter(const fVector3& fvPos1, const fVector3& fvPos2);


/*
|
|	ȡ��v1����ڴ�Pos1��Pos2֮��ֱ�ߵľ����
|        
|             ->x
|                       o pos2
|        |  v1        /    
|     z  v    \     / 
|                \/     
|               /  \
|              o      \ 
|             pos1      v2  <- This point!    
|
*/
fVector2 KYLIN_API KLU_GetReflect(const fVector2& fvPos1, const fVector2& fvPos2, const fVector2& v1);

/*
|
|	���ڴ��ж�ȡһ���ı�(�൱��fgets)
|
*/
KYLIN_API const char* KLU_GetLineFromMemory(char* pStringBuf, int nBufSize, const char* pMemory, const char* pDeadEnd);


/** ����ַ����ķǷ��ַ�
 *	����ֵ��
 *		TRUE	�����зǷ��ַ�
 *		FALSE	�зǷ��ַ�
 */
BOOL	KYLIN_API KLU_CheckStringValid(const char* pStr);


/*
|	KLU_RotateAngle ʹĿ�����ݷ�������ת��������ת������ꡣ
|
|	pos		Ҫ��ת��Ŀ�������
|	axis	��ԭ��Ϊ����ʸ��������
|	angle	��ת�ĽǶ�
|	
|	ע��axis�ᱻ��λ��������ԭ��Ϊ��ʼ���ʸ������������������������ת��
|		��Ҫ�Ƚ�Ŀ�������Ӧ��ƽ�ƣ����øú�����ת����ƽ�ƻ�ȥ���ɡ�
|
*/
fVector3 KYLIN_API KLU_RotateAngle(const fVector3& pos, const fVector3& axis, FLOAT angle);


/*
|	hermite���߲�ֵ�㷨�����߱�����Ϊ150�Σ��������������2D����λ��
|
|	x1,y1,x2,y2			���߶˵㣬�����ֵ������1000���ڣ��������ʱ�ᳬ�����㷶Χ
|	xr1,yr1,xr2,yr2		�������ο�����, ���������1000����	
|	currIter			��ǰ������ֵ�޶���150���ڡ�
*/ 
POINT HermiteCurve (int x1, int y1, int x2, int y2, int xr1, int yr1, int xr2, int yr2, int currIter);


/*
|	ͬ HermiteCurve
*/
POINT BezierCurve(int x1, int y1, int x2, int y2, int xr1, int yr1, int xr2, int yr2, int currIter);












