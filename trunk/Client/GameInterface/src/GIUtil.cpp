
#include "StdAfx.h"
#include "GIUtil.h"
#include "GIException.h"
#include <stdio.h>
#include <math.h>



// 字符串相关
void KLU_Log(LPCSTR pszFmt, ...)
{
	FILE* fp = fopen(".\\GameInterface.log", "a" );
	if(!fp) 
		fp = fopen(".\\GameInterface.log", "w");
	if(!fp) 
		return;

	CHAR szTemp[1024] = {0};
	va_list ptr; va_start(ptr, pszFmt);
	_vsnprintf((char *)szTemp, 1024, pszFmt, ptr);
	va_end(ptr);

	SYSTEMTIME timeNow;
	::GetLocalTime(&timeNow);

	fprintf(fp, "[%02d_%02d:%02d:%02d]: %s\n", timeNow.wDay, timeNow.wHour, timeNow.wMinute, timeNow.wSecond, szTemp);
	fclose(fp);
}


// 字符串相关
INT	KLU_ConvertStringToVector(LPCTSTR strStrintgSource, std::vector< STRING >& vRet, LPCTSTR szKey, BOOL bOneOfKey, BOOL bIgnoreEmpty)
{
	vRet.clear();

	//------------------------------------------------------------
	//合法性
	if(!strStrintgSource || strStrintgSource[0] == '\0') return 0;
	
	STRING strSrc = strStrintgSource;

	//------------------------------------------------------------
	//查找第一个分割点
	STRING::size_type nLeft = 0;
	STRING::size_type nRight;
	if(bOneOfKey)
	{
		nRight = strSrc.find_first_of(szKey);
	}
	else
	{
		nRight = strSrc.find(szKey);
	}

	if(nRight == std::string::npos)
	{
		nRight = strSrc.length();
	}
	while(TRUE)
	{
		STRING strItem = strSrc.substr(nLeft, nRight-nLeft);
		if(strItem.length() > 0 || !bIgnoreEmpty)
		{
			vRet.push_back(strItem);
		}

		if(nRight == strSrc.length())
		{
			break;
		}

		nLeft = nRight + (bOneOfKey ? 1 : _tcslen(szKey));
		
		if(bOneOfKey)
		{
			STRING strTemp = strSrc.substr(nLeft);
			nRight = strTemp.find_first_of(szKey);
			if(nRight != STRING::npos) nRight += nLeft;
		}
		else
		{
			nRight = strSrc.find(szKey, nLeft);
		}

		if(nRight == std::string::npos)
		{
			nRight = strSrc.length();
		}
	}

	return (INT)vRet.size();
}


//====================================
// 数学相关
FLOAT KLU_GetDist(const fVector2& fvPos1, const fVector2& fvPos2)
{
	return sqrtf( (fvPos1.x-fvPos2.x)*(fvPos1.x-fvPos2.x) + (fvPos1.y-fvPos2.y)*(fvPos1.y-fvPos2.y));
}

FLOAT KLU_GetDist(const fVector3& fvPos1, const fVector3& fvPos2)
{
	return sqrtf( (fvPos1.x-fvPos2.x)*(fvPos1.x-fvPos2.x) + (fvPos1.y-fvPos2.y)*(fvPos1.y-fvPos2.y) +  (fvPos1.z-fvPos2.z)*(fvPos1.z-fvPos2.z));
}

FLOAT KLU_GetDistSq(const fVector2& fvPos1, const fVector2& fvPos2)
{
	return (fvPos1.x-fvPos2.x)*(fvPos1.x-fvPos2.x) + (fvPos1.y-fvPos2.y)*(fvPos1.y-fvPos2.y);
}

FLOAT KLU_GetDistSq(const fVector3& fvPos1, const fVector3& fvPos2)
{
	return (fvPos1.x-fvPos2.x)*(fvPos1.x-fvPos2.x) + (fvPos1.y-fvPos2.y)*(fvPos1.y-fvPos2.y) +  (fvPos1.z-fvPos2.z)*(fvPos1.z-fvPos2.z);
}

FLOAT KLU_GetYAngle(const fVector2& fvPos1, const fVector2& fvPos2)	
{
	DOUBLE dDistance = (DOUBLE)KLU_GetDist(fvPos1, fvPos2 );
	if(dDistance <= 0.0f) return 0.0f;

	DOUBLE fACos = (fvPos2.y - fvPos1.y ) / dDistance;
	if( fACos >  1.0) fACos = 0.0;
	if( fACos < -1.0) fACos = KLU_PI;

	if( fACos >-KLU_MINFLOAT && fACos <KLU_MINFLOAT)
	{
		if(fvPos2.x > fvPos1.x )	return  (FLOAT)KLU_PI/2.0f;
		else						return -(FLOAT)KLU_PI/2.0f;
	}

	fACos = ::acos(fACos);
 
	// [0~180]
	if(fvPos2.x >= fvPos1.x)
		return (FLOAT)fACos;
	//(180, 360)
	else
		return (FLOAT)(2 * KLU_PI - fACos);
}


POINT HermiteCurve (int x1, int y1, int x2, int y2, int xr1, int yr1, int xr2, int yr2, int currIter)   
{
	const int Iterative  = 150; // 曲线仿真的线数(控制在150以内， 否则算法出错) 
	const int Iterative2 = Iterative*Iterative;	
	const int Iterative3 = Iterative2*Iterative;

	if (currIter>Iterative)
	{		
		currIter = Iterative;
	}

	int		oldx	= x1,
			oldy	= y1,
			m1		= Iterative3,
			m2		= 0,
			m3		= 0,
			m4		= 0,
			k1		= 0,
			k2		= 0;
	POINT point;
	
	for (int i=0; i<150; ++i)
	{
		k1   =   (i   <<   1)   +   1;
		k2   =   (k1+i)*i   +   k1;  
		m4   +=  (k2   -=   (k1   *=   Iterative));   
		m3   +=  (k1   =   k2   -   k1)   +   Iterative2;   
		m2   -=  (k2   +=   k1);
		m1   +=   k2;

		point.x	= (int) ((x1*m1 + x2*m2 + xr1*m3 + xr2*m4) / Iterative3);
		point.y = (int) ((y1*m1 + y2*m2 + yr1*m3 + yr2*m4) / Iterative3);    

		if (i >= currIter)
			return point;
	}

	return point;   
}

POINT BezierCurve(int x1, int y1, int x2, int y2, int xr1, int yr1, int xr2, int yr2, int currIter)
{    
	return HermiteCurve(x1,y1,x2,y2,3*(xr1-x1),3*(yr1-y1),3*(x2-xr2),3*(y2-yr2), currIter);   
}


fVector3 KLU_RotateAngle(const fVector3& pos, const fVector3& axis, FLOAT angle)
{
	const fVector3			vector		= pos;	
	fVector3				unitAxis	= axis;
	unitAxis.normalise();
	const float				halfAngle	= angle/float(2);
	const float				s			= sin(halfAngle);
	const float				c			= cos(halfAngle);
	const float				x			= unitAxis.x * s;
	const float				y			= unitAxis.y * s;
	const float				z			= unitAxis.z * s;
	const float				w			= c;
	const float				xx			= x*x;
	const float				xy 			= y*x;
	const float				xz			= z*x;
	const float				yy			= y*y;
	const float				yz			= z*y;
	const float				zz			= z*z;
	const float				wx			= w*x;
	const float				wy			= w*y;
	const float				wz			= w*z;
	const float				M[3][3]		=
	{
		{float(1)-float(2)*(yy+zz),		float(2)*(xy-wz),				float(2)*(xz+wy)},
		{float(2)*(xy+wz),				float(1)-float(2)*(xx+zz),      float(2)*(yz-wx)},
		{float(2)*(xz-wy),				float(2)*(yz+wx),				float(1)-float(2)*(xx+yy)},
	};

	return fVector3( 
		vector.x*M[0][0] + vector.y*M[0][1] + vector.z*M[0][2],
		vector.x*M[1][0] + vector.y*M[1][1] + vector.z*M[1][2],
		vector.x*M[2][0] + vector.y*M[2][1] + vector.z*M[2][2] );
}

fVector3 KLU_GetCenter(const fVector3& fvPos1, const fVector3& fvPos2)
{
	fVector3 fvRet;

	fvRet.x = (fvPos1.x + fvPos2.x)/2.0f;
	fvRet.y = (fvPos1.y + fvPos2.y)/2.0f;
	fvRet.z = (fvPos1.z + fvPos2.z)/2.0f;

	return fvRet;
}

fVector2 KLU_Normalise(const fVector2& vec)
{
	fVector2 fvRet = vec;
	float fLength = sqrt(fvRet.x * fvRet.x + fvRet.y * fvRet.y);

	// Will also work for zero-sized vectors, but will change nothing
	if ( fLength > 1e-08 )
	{
		float fInvLength = 1.0f / fLength;
		fvRet.x *= fInvLength;
		fvRet.y *= fInvLength;
	}
	return fvRet;
}

fVector2 KLU_GetReflect(const fVector2& fvPos1, const fVector2& fvPos2, const fVector2& v1)
{
	// pos1 -> pos2
	fVector2 fvNormal = fvPos2 - fvPos1;
	fvNormal.normalise();
	// pos1 -> v1
	fVector2 fvLine1 = v1 - fvPos1;
	// pos1 -> v2
	fVector2 fvLine2 = 2 * KLU_DotProduct(fvLine1, fvNormal)* fvNormal - fvLine1;
	//return v2
	return fvLine2 + fvPos1;
}

//从内存中字符串读取一行文本(按照换行符)
const char* KLU_GetLineFromMemory(char* pStringBuf, int nBufSize, const char* pMemory, const char* pDeadEnd)
{
	//寻找下一个换行符
	register const char* pMem = pMemory;
	if(pMem >= pDeadEnd || *pMem==0) return 0;

	while(pMem < pDeadEnd && pMem-pMemory+1<nBufSize && 
			*pMem != 0 && *pMem != '\r' && *pMem != '\n') *(pStringBuf++) = *(pMem++);
	//add 'null' end
	*pStringBuf = 0;

	//skip all next \r and \n
	while(pMem < pDeadEnd && *pMem != 0 && (*pMem=='\r' || *pMem=='\n')) pMem++;

	return pMem;
}

BOOL KLU_CheckStringValid(const char* pStr)
{
	const char* szInvalid = " #\\/`~!@~$%^&*()-_+=|{}[];:'\"<>,.?";
	const char* szHZBlank = "　"; //中文空格

	if(NULL == pStr) return FALSE;
	std::string strName(pStr);

	if(strName.empty()) return FALSE;

	//查找是否有非法ascii字符
	STRING::size_type nInvalidPos = strName.find_first_of(szInvalid);
	if(nInvalidPos == STRING::npos)
	{
		//中文空格
		nInvalidPos = strName.find(szHZBlank);
	}

	if(nInvalidPos != STRING::npos)
	{
		return FALSE;
	}
	return TRUE;
}
