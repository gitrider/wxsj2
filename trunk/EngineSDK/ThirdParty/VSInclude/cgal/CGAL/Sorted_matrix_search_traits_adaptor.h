// Copyright (c) 1998-2003  ETH Zurich (Switzerland).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Matrix_search/include/CGAL/Sorted_matrix_search_traits_adaptor.h $
// $Id: Sorted_matrix_search_traits_adaptor.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Michael Hoffmann <hoffmann@inf.ethz.ch>

#if ! (CGAL_SORTED_MATRIX_SEARCH_TRAITS_ADAPTOR_H)
#define CGAL_SORTED_MATRIX_SEARCH_TRAITS_ADAPTOR_H 1

CGAL_BEGIN_NAMESPACE

template < class FeasibilityTest_, class Matrix_ >
class Sorted_matrix_search_traits_adaptor {
public:
  typedef FeasibilityTest_         FeasibilityTest;
  typedef Matrix_                  Matrix;
  typedef typename Matrix::Value   Value;
  typedef std::less< Value >       Compare_strictly;
  typedef std::less_equal< Value > Compare_non_strictly;

  Sorted_matrix_search_traits_adaptor(FeasibilityTest ft)
  : ft_( ft)
  {}

  Compare_strictly
  compare_strictly() const
  { return Compare_strictly(); }

  Compare_non_strictly
  compare_non_strictly() const
  { return Compare_non_strictly(); }

  bool
  is_feasible(Value a)
  { return ft_( a); }

protected:
  FeasibilityTest ft_;
};

//!!! with iterator traits we replace const Matrix&
// by an iterator with value type Matrix
template < class FeasibilityTest, class Matrix >
Sorted_matrix_search_traits_adaptor<
  FeasibilityTest, Matrix >
sorted_matrix_search_traits_adaptor(FeasibilityTest f, const Matrix&)
{
  typedef Sorted_matrix_search_traits_adaptor<
    FeasibilityTest, Matrix > Traits;
  return Traits(f);
} // sorted_matrix_search_traits_adaptor( ... )

CGAL_END_NAMESPACE

#endif // ! (CGAL_SORTED_MATRIX_SEARCH_TRAITS_ADAPTOR_H)
// ----------------------------------------------------------------------------
// ** EOF
// ----------------------------------------------------------------------------

