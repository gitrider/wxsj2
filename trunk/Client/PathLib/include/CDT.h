#ifndef __TestCDT__H__
#define __TestCDT__H__
#include "PathLibType.h"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>



namespace PathLib
{
    struct K : CGAL::Exact_predicates_inexact_constructions_kernel {};

typedef CGAL::Triangulation_vertex_base_2<K>                     Vb;
typedef CGAL::Constrained_triangulation_face_base_2<K>           Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>              TDS;
typedef CGAL::Exact_predicates_tag                               Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, TDS, Itag> CGALCDT;
typedef CGALCDT::Point          Point;

    class CDT
    {
    public:

        CDT()
        {

        }

        CDT(const Point2DList& pointList);

        void insert(const Vector2& p1, const Vector2& p2);

    public:

        CGALCDT mCGALCDT;
    };
}

#endif