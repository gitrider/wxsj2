#ifndef __CPolygon_H__
#define __CPolygon_H__

#include "MathLib.h"

#include "PathLibType.h"

#include <CGAL/Cartesian.h>
#include <CGAL/Polygon_2.h>

#include <list>


namespace PathLib
{
    typedef CGAL::Cartesian<double> K1;
typedef CGAL::Point_2<K1> Point1;
typedef CGAL::Polygon_2<K1, std::list<Point1> > CGALPolygon;
typedef CGAL::Polygon_2<K1, std::list<Point1> >::Vertex_iterator VertexIterator;
typedef CGAL::Polygon_2<K1, std::list<Point1> >::Edge_const_iterator EdgeIterator;

    class CPolygon
    {
    public:

        CPolygon()
            : mRegionFlag(0)
        {
        }
        
        void pushPoint(const Vector2& point);

        bool ptInPolygon(const Vector2& point) const;

        void clear(void);

        bool intersert(const CPolygon& polygon, Point2DList& pointList) const;
        bool intersert(const Vector2& lineStart, const Vector2& lineEnd, Point2DList& pointList) const;

        void counterClockwise(void);

    public:

        CGALPolygon mCGALPolygon;
        int mRegionFlag;
    };
}

#endif