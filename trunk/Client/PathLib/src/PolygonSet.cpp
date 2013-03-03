#include "PolygonSet.h"

namespace PathLib
{
    void JoinPolygon::pushPoint(const Vector2& point)
    {
        mCGALPolygon.push_back(CPS_Point_2(point.x, point.y));

    }  

    void JoinPolygon::counterClockwise(void)
    {
        if (mCGALPolygon.orientation() == CGAL::CLOCKWISE)
        {
            mCGALPolygon.reverse_orientation();
        }   
    }
}
