
/** 工具函数集
 */

#pragma once


#include "GIBasicType.h"
#include "GIMath.h"




/** 运行Log */
void KYLIN_API KLU_Log(LPCSTR pszFmt, ...);


/** 字符串相关
 *	将字符串按照关键字分割
 */
INT KYLIN_API KLU_ConvertStringToVector(LPCTSTR strStrintgSource, std::vector< STRING >& vRet, LPCTSTR szKey=_T("\\/"), BOOL bOneOfKey = TRUE, BOOL bIgnoreEmpty = TRUE);


/** 数学相关
 */
#define KLU_PI			(3.1415926535)
#define KLU_MINFLOAT	(1.0E-9)


/** 取得两点间的距离 */
FLOAT KYLIN_API KLU_GetDist(const fVector2& fvPos1, const fVector2& fvPos2);
FLOAT KYLIN_API KLU_GetDist(const fVector3& fvPos1, const fVector3& fvPos2);

/** 取得两点间的距离平方 */
FLOAT KYLIN_API KLU_GetDistSq(const fVector2& fvPos1, const fVector2& fvPos2);
FLOAT KYLIN_API KLU_GetDistSq(const fVector3& fvPos1, const fVector3& fvPos2);

template<class T, class U, class V>	inline void KLU_Clamp(T &v, const U &min, const V &max)
{
	v = (v < min) ? min : v;
	v = (v > max) ? max : v;
}


/*
|
|	求两个向量的点积
|
*/
inline float KLU_DotProduct(const fVector2& vec1, const fVector2& vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

/*
|
|	取得从Pos1到Pos2的矢量相对于Y轴的旋转角度, 以z轴为0度
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
|	取得从Pos1与Pos2之间的中心点
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
|	取得v1相对于从Pos1与Pos2之间直线的镜像点
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
|	从内存中读取一行文本(相当于fgets)
|
*/
KYLIN_API const char* KLU_GetLineFromMemory(char* pStringBuf, int nBufSize, const char* pMemory, const char* pDeadEnd);


/** 检查字符串的非法字符
 *	返回值：
 *		TRUE	不含有非法字符
 *		FALSE	有非法字符
 */
BOOL	KYLIN_API KLU_CheckStringValid(const char* pStr);


/*
|	KLU_RotateAngle 使目标点根据方向轴旋转，返回旋转后的坐标。
|
|	pos		要旋转的目标点坐标
|	axis	以原点为起点的矢量方向轴
|	angle	旋转的角度
|	
|	注：axis会被单位化，变以原点为起始点的矢量方向。如果想根据任意轴做旋转，
|		需要先将目标点做相应的平移，调用该函数旋转后，再平移回去即可。
|
*/
fVector3 KYLIN_API KLU_RotateAngle(const fVector3& pos, const fVector3& axis, FLOAT angle);


/*
|	hermite曲线差值算法。曲线被划分为150段，返回所需段数的2D坐标位置
|
|	x1,y1,x2,y2			曲线端点，最好数值限制在1000以内，否侧运算时会超出运算范围
|	xr1,yr1,xr2,yr2		曲线两参考向量, 最好限制在1000以内	
|	currIter			当前段数，值限定在150以内。
*/ 
POINT HermiteCurve (int x1, int y1, int x2, int y2, int xr1, int yr1, int xr2, int yr2, int currIter);


/*
|	同 HermiteCurve
*/
POINT BezierCurve(int x1, int y1, int x2, int y2, int xr1, int yr1, int xr2, int yr2, int currIter);












