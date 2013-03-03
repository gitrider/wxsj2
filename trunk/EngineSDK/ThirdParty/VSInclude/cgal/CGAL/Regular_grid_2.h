// Copyright (c) 2005  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Stream_lines_2/include/CGAL/Regular_grid_2.h $
// $Id: Regular_grid_2.h 29656 2006-03-21 13:48:32Z amebarki $
// 
//
// Author(s)     : Abdelkrim Mebarki <Abdelkrim.Mebarki@sophia.inria.fr>

#ifndef CGAL_REGULAR_GRID_2_H_ 
#define CGAL_REGULAR_GRID_2_H_

#include <CGAL/basic.h>
#include <CGAL/Cartesian.h>
#include "streamlines_assertions.h"

CGAL_BEGIN_NAMESPACE

// The class  Regular_grid_2 provides a rectangular visualization
// domain, with a vector field defined on a regular grid, and also a
// bilinear interpolation to extract the vector field values

template <class StreamLinesTraits_2>
class Regular_grid_2
{
public:
  typedef Regular_grid_2<StreamLinesTraits_2> Vector_field_2;
  typedef StreamLinesTraits_2 Geom_traits;
  typedef typename StreamLinesTraits_2::FT FT;
  typedef typename StreamLinesTraits_2::Point_2 Point_2;
  typedef typename StreamLinesTraits_2::Vector_2 Vector_2;
protected:
  FT *vector_field;
  inline int get_index(const int & i,const int & j) const;
  int number_of_samples_x;
  int number_of_samples_y;
  FT domain_size_x;
  FT domain_size_y;
  Vector_2 get_vector_field(const Point_2 & p) const;
  FT get_density_field(const Point_2 & p) const;
  bool is_in_samples(const int & i,const int & j) const;
public:
  Regular_grid_2(const int & m, const int & n,const FT & x, const FT & y);
  //   Regular_grid_2();
  ~Regular_grid_2()
    {
      delete [] vector_field;
    }
  void fill(std::ifstream & f, const FT & x, const FT & y);
  inline typename Geom_traits::Iso_rectangle_2 bbox() const;

  std::pair<Vector_2,FT> 
  get_field(const Point_2 & p) const
    {
      CGAL_streamlines_precondition(is_in_domain(p));
      Vector_2 v = get_vector_field(p);
      FT density = get_density_field(p);
      return std::pair<Vector_2, FT>(v,density);
    }

  inline bool is_in_domain(const Point_2 & p) const;
  inline FT get_integration_step(const Point_2 &) const;
  inline FT get_integration_step() const;
  inline void set_field(const int & i, const int & j, const Vector_2 & v);
  inline std::pair<int, int> get_dimension()
    {
      return std::pair<int, int>(number_of_samples_x, number_of_samples_y);
    }
  inline std::pair<FT, FT> get_size()
    {
      return std::pair<FT,
	FT>(domain_size_x,
	    domain_size_y);
    }
    
  inline FT container_velue(int i) const
    {
      if (i < 2*number_of_samples_x*number_of_samples_y)
        return vector_field[i];
      else
        return 0.0;
    }
};

template <class StreamLinesTraits_2>
inline
typename Regular_grid_2<StreamLinesTraits_2>::Geom_traits::Iso_rectangle_2
Regular_grid_2<StreamLinesTraits_2>::bbox() const
{
  return typename Geom_traits::Iso_rectangle_2(0.0, 0.0,
					       domain_size_x,
					       domain_size_y);
}

template <class StreamLinesTraits_2>
inline int 
Regular_grid_2<StreamLinesTraits_2>::get_index(const int & i, const
					       int & j) const
{
  return 2*(number_of_samples_x*j + i);
}

template <class StreamLinesTraits_2>
Regular_grid_2<StreamLinesTraits_2>::Regular_grid_2(const int & m,
						    const int &
						    n,const FT & x,
						    const FT & y)
{
  number_of_samples_x = m;
  number_of_samples_y = n;
  domain_size_x = x;
  domain_size_y = y;
  vector_field = new FT[number_of_samples_x*number_of_samples_y* 2];
}

template <class StreamLinesTraits_2>
inline void
Regular_grid_2<StreamLinesTraits_2>::set_field(const int & i, const
					       int & j, const Vector_2
					       & v)
{
  CGAL_streamlines_precondition(is_in_samples(i,j));
  int index = get_index(i,j);
  vector_field[index++] = v.x();
  vector_field[index] = v.y();
}

template <class StreamLinesTraits_2>
inline bool
Regular_grid_2<StreamLinesTraits_2>::is_in_domain(const Point_2 & p)
  const
{
  return ((p.x()>=0.0) && (p.x()<=domain_size_x) && (p.y()>=0.0) &&
	  (p.y()<=domain_size_y));
}

template <class StreamLinesTraits_2>
bool 
Regular_grid_2<StreamLinesTraits_2>::is_in_samples(const int & i,
						   const int & j)
  const
{
  return ((i>=0) && (i<=number_of_samples_x-1) && (j>=0) &&
	  (j<=number_of_samples_y-1));
}

template <class StreamLinesTraits_2>
typename Regular_grid_2<StreamLinesTraits_2>::Vector_2 
Regular_grid_2<StreamLinesTraits_2>::get_vector_field(const Point_2 &
						      p) const
{
  FT fXv,fYv;
  FT x = (p.x() / domain_size_x) *  (number_of_samples_x-1);
  FT y = (p.y() / domain_size_y) *  (number_of_samples_y-1);
  int i = (int) x;
  int j = (int) y;
  FT xfract = x - (FT)  i;
  FT yfract = y - (FT)  j;
  int iIndex_1 = get_index(i,j);
  int iIndex_2 = get_index(i+1,j);
  int iIndex_3 = get_index(i,j+1);
  int iIndex_4 = get_index(i+1,j+1);
  FT x00 = container_velue(iIndex_1);
  FT x01 = container_velue(iIndex_2);
  FT x10 = container_velue(iIndex_3);
  FT x11 = container_velue(iIndex_4);
  FT x0 = x00 + xfract * (x01 - x00);
  FT x1 = x10 + xfract * (x11 - x10);
  fXv = x0 + yfract * (x1 - x0);
  iIndex_1++;
  iIndex_2++;
  iIndex_3++;
  iIndex_4++;
  FT y00 = container_velue(iIndex_1);
  FT y01 = container_velue(iIndex_2);
  FT y10 = container_velue(iIndex_3);
  FT y11 = container_velue(iIndex_4);
  FT y0 = y00 + xfract * (y01 - y00);
  FT y1 = y10 + xfract * (y11 - y10);
  fYv = y0 + yfract * (y1 - y0);
  // normalization step
  FT normal = sqrt((fXv)*(fXv) + (fYv)*(fYv));
  fXv = fXv / normal;
  fYv = fYv / normal;
  Vector_2  v = Vector_2(fXv, fYv);
  return v;
}

template <class StreamLinesTraits_2>
typename Regular_grid_2<StreamLinesTraits_2>::FT
Regular_grid_2<StreamLinesTraits_2>::get_density_field(const Point_2 &
						       p) const
{
  return 1.0;
}

template<class StreamLinesTraits_2>
inline
typename Regular_grid_2<StreamLinesTraits_2>::FT
Regular_grid_2<StreamLinesTraits_2>::get_integration_step(const
							  Point_2 &)
  const
{
  return 1.0;
}

template<class StreamLinesTraits_2>
inline
typename Regular_grid_2<StreamLinesTraits_2>::FT
Regular_grid_2<StreamLinesTraits_2>::get_integration_step() const
{
  return 1.0;
}

CGAL_END_NAMESPACE

#endif
