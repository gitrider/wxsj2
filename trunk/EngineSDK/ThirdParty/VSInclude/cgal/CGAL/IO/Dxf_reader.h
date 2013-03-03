// Copyright (c) 2003-2006  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Curved_kernel/include/CGAL/IO/Dxf_reader.h $
// $Id: Dxf_reader.h 30667 2006-04-19 16:56:12Z glisse $
//
// Author(s)     : Monique Teillaud, Sylvain Pion, Andreas Fabri

// Descriptions of the file format can be found at
// http://www.autodesk.com/techpubs/autocad/acad2000/dxf/
// http://www.tnt.uni-hannover.de/soft/compgraph/fileformats/docs/DXF.ascii

#ifndef CGAL_IO_DXF_READER_H
#define CGAL_IO_DXF_READER_H

#include <CGAL/basic.h>
#include <iostream>
#include <string>
#include <list>

CGAL_BEGIN_NAMESPACE

template <typename K>
class Dxf_reader {

public:
  typedef typename K::FT       FT;
  typedef typename K::Point_2  Point_2;
  typedef typename K::Circle_2 Circle_2;
  
  
  typedef std::list<std::pair<Point_2, double> > Polygon;
  typedef std::list<Polygon> Polygons;
  typedef std::list<Circle_2> Circles;
  typedef std::list<std::pair<Point_2, FT> > Centers_and_radii;
  
private:

  void
  header(std::istream& is)
  {
    int n;
    double xmin, ymin;
    double xmax, ymax;
    is >> n;
    assert(n == 9);
    char c;
    is >> c;
    assert(c == '$');
    std::string str;
    is >> str;
    if(str == std::string("EXTMIN")){
      is >> n;
      assert(n == 10);
    is >> xmin;
    is >> n;
    assert(n == 20);
    is >> ymin;
    }
    is >> n;
    assert(n == 9);
    is >> c;
    assert(c == '$');
    is >> str;
    if(str == "EXTMAX"){
      is >> n;
      assert(n == 10);
      is >> xmax;
      is >> n;
      assert(n == 20);
      is >> ymax;
    }
  }
  
  
  void
  skip_header(std::istream& is)
  {
    int n;
    is >> n;
    assert(n == 0);
    std::string str;
    is >> str;
    assert(str == "SECTION");
    is >> n;
    assert(n == 2);
    is >> str;
    if(str == "HEADER"){
      header(is);
    }
    is >> n;
    assert(n == 0);
    is >> str;
    assert(str == "ENDSEC");
  }
  
  
  
  void 
  read_circle(std::istream& is, Circle_2& circ)
  {
    int n;
    double cx, cy, r;
    std::string str;
    is >> n;
    assert(n == 8);
    is >> n;
    assert(n == 0);
  
  is >> n;
  assert(n == 10);
  is >> cx;
  is >> n;
  assert(n == 20);
  is >> cy;
  is >> n;
  assert(n == 40);
  is >> r;
  FT rft(r);
  circ = typename K::Construct_circle_2()(Point_2(cx,cy), rft);
}

  void 
  read_center_and_radius(std::istream& is, Point_2& center, FT& rft)
  {
    int n;
    double cx, cy, r;
    std::string str;
    is >> n;
    assert(n == 8);
    is >> n;
    assert(n == 0);
  
  is >> n;
  assert(n == 10);
  is >> cx;
  is >> n;
  assert(n == 20);
  is >> cy;
  is >> n;
  assert(n == 40);
  is >> r;

  center = typename K::Construct_point_2()(cx,cy);
  rft = FT(r);
}


void
read_polygon(std::istream& is, Polygon& poly)
{
  int n;
  double x, y, len;
  std::string str;

  do {
    is >> n;
    if(n != 0){
      int m;
      is >> m; 
    }
  } while(n != 0);

  do {
    is >> str;
    if(str == "VERTEX"){
      is >> n;
      assert(n == 8);
      is >> n;
      assert(n == 0);
      is >> n;
      assert(n == 10);
      is >> x;
      is >> n;
      assert(n == 20);
      is >> y;
      is >> n;
      len = 0;
      if(n == 42){
	is >> len;
      } else {
	assert(n == 0);
      }
      poly.push_back(std::make_pair(typename K::Construct_point_2()(x,y), len));
    }
    
  } while (str != "SEQEND");
  is >> n;
  assert(n == 8);
  is >> n;
  assert(n == 0);


}


void
read_entities(std::istream& is, Polygons& polys, Circles& circles)
{
  int n;
  //double x, y;
  std::string str;
  is >> n;
  assert(n == 0);
  is >> str;
  assert(str == "SECTION");
  is >> n;
  is >> str;
  assert(str == "ENTITIES");
  do {
    is >> n;
    assert(n == 0);
    is >> str;
    if(str == "POLYLINE"){
      Polygon p;
      polys.push_back(p);
      read_polygon(is, polys.back());
    } else if(str == "CIRCLE"){
      Circle_2 c;
      read_circle(is,c);      
      circles.push_back(c);
    } else if(str == "ENDSEC"){
      
    } else {
      std::cerr << "unknown entity" << std::endl;
      std::exit(0);
    }
  } while(str != "ENDSEC");
  is >> n;
  assert(n == 0);
  is >> str;
  assert(str == "EOF");
}

void
read_entities(std::istream& is, Polygons& polys, Centers_and_radii& car)
{
  int n;
  //double x, y;
  std::string str;
  is >> n;
  assert(n == 0);
  is >> str;
  assert(str == "SECTION");
  is >> n;
  is >> str;
  assert(str == "ENTITIES");
  do {
    is >> n;
    assert(n == 0);
    is >> str;
    if(str == "POLYLINE"){
      Polygon p;
      polys.push_back(p);
      read_polygon(is, polys.back());
    } else if(str == "CIRCLE"){
      Point_2 center;
      FT radius;
      read_center_and_radius(is,center, radius);      
      car.push_back(std::make_pair(center, radius));
    } else if(str == "ENDSEC"){
      
    } else {
      std::cerr << "unknown entity" << std::endl;
      std::exit(0);
    }
  } while(str != "ENDSEC");
  is >> n;
  assert(n == 0);
  is >> str;
  assert(str == "EOF");
}

public:

void operator()(std::istream& is, Polygons& polygons, Circles& circles)
{
  skip_header(is);
  read_entities(is, polygons, circles);
}

void operator()(std::istream& is, Polygons& polygons, Centers_and_radii& car)
{
  skip_header(is);
  read_entities(is, polygons, car);
}
};

CGAL_END_NAMESPACE

#endif // CGAL_IO_DXF_READER_H
