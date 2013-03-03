#include "Polygon.h"
#include "UtilFunc.h"

namespace PathLib
{
    void CPolygon::pushPoint(const Vector2& point)
    {
        mCGALPolygon.push_back(Point1(point.x, point.y));
    }

    bool CPolygon::ptInPolygon(const Vector2& point) const
    {
        return (mCGALPolygon.bounded_side(Point1(point.x, point.y)) == CGAL::ON_BOUNDED_SIDE);
    }

    bool CPolygon::intersert(const CPolygon& polygon, Point2DList& pointList) const
    {
        bool intersected = false;
        for (EdgeIterator p1ei = mCGALPolygon.edges_begin(); p1ei != mCGALPolygon.edges_end(); ++p1ei)
            for (EdgeIterator p2ei = polygon.mCGALPolygon.edges_begin(); p2ei != polygon.mCGALPolygon.edges_end(); ++p2ei)
            {
                Vector2 start1((*p1ei).source().x(), (*p1ei).source().y());
                Vector2 end1((*p1ei).target().x(), (*p1ei).target().y());
                Vector2 start2((*p2ei).source().x(), (*p2ei).source().y());
                Vector2 end2((*p2ei).target().x(), (*p2ei).target().y());

                float crossx, crossy;

                if ( intersectWith(start1, end1, start2, end2, crossx, crossy) )
                {
                    pointList.push_back(Vector2(crossx,crossy));

                    intersected = true;
                }
            }

        return intersected;
    }

    bool CPolygon::intersert(const Vector2& lineStart, const Vector2& lineEnd, Point2DList& pointList) const
    {
        bool intersected = false;
        for (EdgeIterator p1ei = mCGALPolygon.edges_begin(); p1ei != mCGALPolygon.edges_end(); ++p1ei)
        {
            Vector2 start1((*p1ei).source().x(), (*p1ei).source().y());
            Vector2 end1((*p1ei).target().x(), (*p1ei).target().y());

            float crossx, crossy;

            if ( intersectWith(start1, end1, lineStart, lineEnd, crossx, crossy) )
            {
                pointList.push_back(Vector2(crossx,crossy));

                intersected = true;
            }
        }

        return intersected;
    }

    void CPolygon::clear(void)
    {
        mCGALPolygon.clear();
    }

    void CPolygon::counterClockwise(void)
    {
        if (mCGALPolygon.orientation() == CGAL::CLOCKWISE)
        {
            mCGALPolygon.reverse_orientation();
        }   
    }
}