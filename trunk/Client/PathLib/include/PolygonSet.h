#ifndef __CPolygonSet_H__
#define __CPolygonSet_H__

#include "MathLib.h"

#include "bso_rational_nt.h"
#include <CGAL/Cartesian.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_set_2.h>

#include <list>


namespace PathLib
{
    typedef CGAL::Cartesian<Number_type>               CPS_Kernel;
    typedef CPS_Kernel::Point_2                        CPS_Point_2;
    typedef CGAL::Polygon_2<CPS_Kernel>                CPS_Polygon_2;
    typedef CGAL::Polygon_with_holes_2<CPS_Kernel>     CPS_Polygon_with_holes_2;
    typedef CGAL::Polygon_set_2<CPS_Kernel>            CPS_Polygon_set_2;
class JoinPolygon
    {
    public:

        void pushPoint(const Vector2& point);

        void counterClockwise(void);

    public:
        CPS_Polygon_2 mCGALPolygon;
    };


    class PolygonSet
    {
    public:

        PolygonSet()
        {
        }

    public:

       CPS_Polygon_set_2 mCGALPolygonSet; 
    };
}


#endif