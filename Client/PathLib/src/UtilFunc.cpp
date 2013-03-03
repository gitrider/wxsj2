#include "UtilFunc.h"

namespace PathLib
{
    /**
    Subject 1.03: How do I find intersections of 2 2D line segments?

    This problem can be extremely easy or extremely difficult; it
    depends on your application. If all you want is the intersection
    point, the following should work:

    Let A,B,C,D be 2-space position vectors.  Then the directed line
    segments AB & CD are given by:

    AB=A+r(B-A), r in [0,1]
    CD=C+s(D-C), s in [0,1]

    If AB & CD intersect, then

    A+r(B-A)=C+s(D-C), or

    Ax+r(Bx-Ax)=Cx+s(Dx-Cx)
    Ay+r(By-Ay)=Cy+s(Dy-Cy)  for some r,s in [0,1]

    Solving the above for r and s yields

    (Ay-Cy)(Dx-Cx)-(Ax-Cx)(Dy-Cy)
    r = -----------------------------  (eqn 1)
    (Bx-Ax)(Dy-Cy)-(By-Ay)(Dx-Cx)

    (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
    s = -----------------------------  (eqn 2)
    (Bx-Ax)(Dy-Cy)-(By-Ay)(Dx-Cx)

    Let P be the position vector of the intersection point, then

    P=A+r(B-A) or

    Px=Ax+r(Bx-Ax)
    Py=Ay+r(By-Ay)

    By examining the values of r & s, you can also determine some
    other limiting conditions:

    If 0<=r<=1 & 0<=s<=1, intersection exists
    r<0 or r>1 or s<0 or s>1 line segments do not intersect

    If the denominator in eqn 1 is zero, AB & CD are parallel
    If the numerator in eqn 1 is also zero, AB & CD are collinear.

    If they are collinear, then the segments may be projected to the x- 
    or y-axis, and overlap of the projected intervals checked.

    If the intersection point of the 2 lines are needed (lines in this
    context mean infinite lines) regardless whether the two line
    segments intersect, then

    If r>1, P is located on extension of AB
    If r<0, P is located on extension of BA
    If s>1, P is located on extension of CD
    If s<0, P is located on extension of DC

    Also note that the denominators of eqn 1 & 2 are identical.

    References:

    [O'Rourke (C)] pp. 249-51
    [Gems III] pp. 199-202 "Faster Line Segment Intersection,"

    */
    bool intersectWith(
        const Vector2&		A,		// 直线1的开始点。
        const Vector2&		B,			// 直线1的结束点。
        const Vector2&		C,		// 直线2的开始点。
        const Vector2&		D,			// 直线2的结束点。
        float&			fcrossx,		// 找到的交点x。
        float&			fcrossy			// 找到的交点y
        )
    {
        float denominator = (B.x-A.x)*(D.y-C.y) - (B.y-A.y)*(D.x-C.x);

        // 平行，两直线相同，都为false
        if (Math::RealEqual(denominator, 0.0f))
            return false;

        float numeratorR = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
        float numeratorS = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);

        float r = numeratorR / denominator;
        float s = numeratorS / denominator;

        fcrossx = A.x+r*(B.x-A.x);
        fcrossy = A.y+r*(B.y-A.y);

        if ( r >= 0 && r <= 1 && s >= 0 && s <= 1 )
            return true;
        else
            return false;
    }
    //-----------------------------------------------------------------------
    bool pointInGrid(const Vector2& p, const Grid& grid)
    {
        if (p.x >= grid.mMinPos.x && p.x <= grid.mMaxPos.x && p.y >= grid.mMinPos.y && p.y <= grid.mMaxPos.y)
            return true;

        return false;
    }
    //-----------------------------------------------------------------------
    bool intersectGridTriangle(const Grid& grid, const Triangle& triangle)
    {
        // 先检测包围盒
        if (grid.mMaxPos.x < triangle.minPos.x)
            return false;
        if (grid.mMaxPos.y < triangle.minPos.y)
            return false;

        if (grid.mMinPos.x > triangle.maxPos.x)
            return false;
        if (grid.mMinPos.y > triangle.maxPos.y)
            return false;

        Vector2 gridPoint1, gridPoint2, gridPoint3, gridPoint4;
        gridPoint1 = grid.mMinPos;
        gridPoint2 = grid.mMaxPos;
        gridPoint3.x = grid.mMinPos.x;
        gridPoint3.y = grid.mMaxPos.y;
        gridPoint4.x = grid.mMaxPos.x;
        gridPoint4.y = grid.mMinPos.y;

        float x,y;

        // 只要有一个相交，返回true
        if (intersectWith(triangle.pos1, triangle.pos2,gridPoint1,gridPoint3,x,y) ||
            intersectWith(triangle.pos1, triangle.pos2,gridPoint1,gridPoint4,x,y) ||
            intersectWith(triangle.pos1, triangle.pos2,gridPoint2,gridPoint4,x,y) ||
            intersectWith(triangle.pos1, triangle.pos2,gridPoint2,gridPoint3,x,y) ||

            intersectWith(triangle.pos3, triangle.pos2,gridPoint1,gridPoint3,x,y) ||
            intersectWith(triangle.pos3, triangle.pos2,gridPoint1,gridPoint4,x,y) ||
            intersectWith(triangle.pos3, triangle.pos2,gridPoint2,gridPoint4,x,y) ||
            intersectWith(triangle.pos3, triangle.pos2,gridPoint2,gridPoint3,x,y) ||
            
            intersectWith(triangle.pos1, triangle.pos3,gridPoint1,gridPoint3,x,y) ||
            intersectWith(triangle.pos1, triangle.pos3,gridPoint1,gridPoint4,x,y) ||
            intersectWith(triangle.pos1, triangle.pos3,gridPoint2,gridPoint4,x,y) ||
            intersectWith(triangle.pos1, triangle.pos3,gridPoint2,gridPoint3,x,y)
            )
            return true;

        // 都不相交，判断是否互相包含
        if ( ( Math::pointInTri2D(gridPoint1, triangle.pos1, triangle.pos2, triangle.pos3) &&
               Math::pointInTri2D(gridPoint2, triangle.pos1, triangle.pos2, triangle.pos3) &&
               Math::pointInTri2D(gridPoint3, triangle.pos1, triangle.pos2, triangle.pos3) &&
               Math::pointInTri2D(gridPoint4, triangle.pos1, triangle.pos2, triangle.pos3) ) ||

             ( pointInGrid(triangle.pos1, grid) &&
               pointInGrid(triangle.pos2, grid) &&
               pointInGrid(triangle.pos3, grid) )
            )
            return true;
        else
            return false;
    }
    //-----------------------------------------------------------------------
    bool edgeInGrid(const Vector2& p1, const Vector2& p2, const Grid& grid)
    {
        Vector2 gridPoint1, gridPoint2, gridPoint3, gridPoint4;
        gridPoint1 = grid.mMinPos;
        gridPoint2 = grid.mMaxPos;
        gridPoint3.x = grid.mMinPos.x;
        gridPoint3.y = grid.mMaxPos.y;
        gridPoint4.x = grid.mMaxPos.x;
        gridPoint4.y = grid.mMinPos.y;

        float x,y;

        if (pointInGrid(p1, grid) || pointInGrid(p2, grid) ||
            intersectWith(p1, p2, gridPoint1, gridPoint3,x,y) ||
            intersectWith(p1, p2, gridPoint1, gridPoint4,x,y) ||
            intersectWith(p1, p2, gridPoint2, gridPoint4,x,y) ||
            intersectWith(p1, p2, gridPoint2, gridPoint3,x,y))
            return true;

        return false;
    }

}