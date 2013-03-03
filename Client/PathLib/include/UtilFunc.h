#ifndef __UtilFunc_H__
#define __UtilFunc_H__

#include "PathLibType.h"

#include "Grid.h"
#include "Triangle.h"

namespace PathLib
{
    /** 直线相交
    */
    bool intersectWith(
        const Vector2&		A,		// 直线1的开始点。
        const Vector2&		B,			// 直线1的结束点。
        const Vector2&		C,		// 直线2的开始点。
        const Vector2&		D,			// 直线2的结束点。
        float&			fcrossx,		// 找到的交点x。
        float&			fcrossy			// 找到的交点y
        );

    /// 判断点是否在grid中
    bool pointInGrid(const Vector2& p, const Grid& grid);
    /// 判断三角形是否属于grid
    bool intersectGridTriangle(const Grid& grid, const Triangle& triangle);
    /// 判断edge是否属于grid
    bool edgeInGrid(const Vector2& p1, const Vector2& p2, const Grid& grid);
}

#endif