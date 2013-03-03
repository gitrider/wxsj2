#ifndef SINGLE_TYPE_H_IN__
#define SINGLE_TYPE_H_IN__

namespace math_Basic
{
//	using namespace UTIL_Computer;
	struct data_Pointer
	{
		int m_nX;
		int m_nY;
	};

	struct data_Rect
	{
		int m_nMinX;
		int m_nMinY;
		int m_nMaxX;
		int m_nMaxY;

		int minX ( int x ) { m_nMinX = x; }
	};


}

#if _MSC_VER > 1200
#include <atltypes.h>
#endif

#endif
