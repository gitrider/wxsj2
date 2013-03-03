#include "CDT.h"

namespace PathLib
{

    CDT::CDT(const Point2DList& pointList)
    {
        for (size_t i=0; i<pointList.size(); i=i+2)
        {
            mCGALCDT.insert_constraint( Point(pointList[i].x,pointList[i].y), Point(pointList[i+1].x,pointList[i+1].y));

        }
    }

    void CDT::insert(const Vector2& p1, const Vector2& p2)
    {
        mCGALCDT.insert_constraint( Point(p1.x,p1.y), Point(p2.x,p2.y));
    }
}