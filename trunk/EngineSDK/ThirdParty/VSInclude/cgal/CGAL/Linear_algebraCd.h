// Copyright (c) 1999  Utrecht University (The Netherlands),
// ETH Zurich (Switzerland), Freie Universitaet Berlin (Germany),
// INRIA Sophia-Antipolis (France), Martin-Luther-University Halle-Wittenberg
// (Germany), Max-Planck-Institute Saarbruecken (Germany), RISC Linz (Austria),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kernel_d/include/CGAL/Linear_algebraCd.h $
// $Id: Linear_algebraCd.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Herve.Bronnimann@sophia.inria.fr

#ifndef CGAL_LINEAR_ALGEBRACD_H
#define CGAL_LINEAR_ALGEBRACD_H

#include <CGAL/Kernel_d/Vector__.h>
#include <CGAL/Kernel_d/Matrix__.h>
#include <memory>
#include <vector>

#undef _DEBUG
#define _DEBUG 13
#include <CGAL/Kernel_d/debug.h>


//#define CGAL_LA_SELFTEST

CGAL_BEGIN_NAMESPACE

template < class _FT, class _AL = CGAL_ALLOCATOR(_FT) >
class Linear_algebraCd
{
public:
  typedef _FT                     FT;
  typedef _FT                     RT;
  typedef _AL                     AL;
  typedef Linear_algebraCd<FT,AL> Self;
  typedef CGALLA::Vector_<FT,AL>  Vector;
  typedef CGALLA::Matrix_<FT,AL>  Matrix;
  typedef const FT*               const_iterator;
  typedef FT*                     iterator;
  
  Linear_algebraCd() {}

protected:
  // Major routines for Linear_algebra_d
  static 
  void   Gaussian_elimination(const Matrix &M,
            Matrix &L, Matrix &U,
            std::vector<int> &row_permutation,
            std::vector<int> &column_permutation,
            FT &det, int &rank, Vector &c);
  static 
  bool Triangular_system_solver(const Matrix &U, const Matrix &L, 
            const Vector &b, int rank, Vector &x, FT &det);
  static 
  void   Triangular_left_inverse(const Matrix &U, Matrix &Uinv);

public:
  static
  std::pair<int,int> transpose(std::pair<int,int> dim)
  { std::swap(dim.first,dim.second); return dim; }
  static 
  Matrix transpose(const Matrix &M);

  static
  bool   inverse(const Matrix &M, Matrix &I, FT &D, Vector &c);

  static
  Matrix inverse(const Matrix &M, RT &D);

  static
  FT     determinant(const Matrix &M, Matrix &L, Matrix &U,
             std::vector<int> &q, Vector &c);
  static
  FT     determinant(const Matrix &M);

  static
  Sign   sign_of_determinant(const Matrix &M);

  static
  bool   verify_determinant(const Matrix &M, const RT &D,
             const Matrix &L, const Matrix &U, 
             const std::vector<int> &q, const Vector &c);
  static
  bool   linear_solver(const Matrix &M, const Vector &b,
             Vector &x, RT &D, Matrix &spanning_vectors, Vector &c);
  static
  bool   linear_solver(const Matrix &M, const Vector &b,
             Vector &x, RT &D, Vector &c);
  static
  bool   linear_solver(const Matrix &M, const Vector &b,
             Vector &x, RT &D);
  static
  bool   is_solvable(const Matrix &M, const Vector &b);

  static
  bool   homogeneous_linear_solver(const Matrix &M, Vector &x);
  static
  int    homogeneous_linear_solver(const Matrix &M,
             Matrix &spanning_vectors);
  static
  int rank(const Matrix &M);

  static 
  int independent_columns(const Matrix& M, std::vector<int>& columns); 

};

CGAL_END_NAMESPACE

#include <CGAL/Kernel_d/Linear_algebraCd.C>

#endif // CGAL_LINEAR_ALGEBRACD_H


