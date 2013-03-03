// Copyright (c) 2000  Max-Planck-Institute Saarbruecken (Germany).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Partition_2/include/CGAL/Rotation_tree_2.h $
// $Id: Rotation_tree_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Susan Hert <hert@mpi-sb.mpg.de>

/*
    A rotation tree for computing the vertex visibility graph of a set of
    non-intersecting segments in the plane (e.g. edges of a polygon).

    Let $V$ be the set of segment endpoints and
    let $p_{\infinity}$ ($p_{-\infinity}$) be a point with $y$ coordinate
    $\infinity$ ($-\infinity$) and $x$ coordinate larger than all points
    in $V$. The tree $G$ is a tree with node set
    $V \cup \{p_{\infinity}, p_{-\infinity}\}$.  Every node (except the one
    corresponding to $p_{\infinity}$) has exactly one outgoing edge to the
    point $q$ with the following property:  $q$ is the first point encountered
    when looking from $p$ in direction $d$ and rotating counterclockwise.
 */

#ifndef  CGAL_ROTATION_TREE_H
#define  CGAL_ROTATION_TREE_H

//  MSVC6 doesn't work with the CGALi::vector but it does with the std::vector
//  (from stlport?)
#if (defined( _MSC_VER) && (_MSC_VER <= 1200)) || defined(__BORLANDC__)
#include <vector>
#else
#include <CGAL/vector.h>
#endif // MSVC6
#include <CGAL/Rotation_tree_node_2.h>
#include <CGAL/functional.h>

namespace CGAL {

template <class Traits_>
#if (defined( _MSC_VER) && (_MSC_VER <= 1200)) || defined(__BORLANDC__)
class Rotation_tree_2 : public std::vector< Rotation_tree_node_2<Traits_> >
#else
class Rotation_tree_2 : public CGALi::vector< Rotation_tree_node_2<Traits_> >
#endif // MSVC 6
{
public:
   typedef Traits_                                 Traits;
   typedef Rotation_tree_node_2<Traits>            Node;
#if (defined( _MSC_VER) && (_MSC_VER <= 1200)) || defined(__BORLANDC__)
   typedef typename std::vector<Node>::iterator    Self_iterator;
#else
   typedef typename CGALi::vector<Node>::iterator  Self_iterator;
#endif // MSVC6
   typedef typename Traits::Point_2                Point_2;


   // constructor
   template<class ForwardIterator>
   Rotation_tree_2(ForwardIterator first, ForwardIterator beyond)
   {
      for (ForwardIterator it = first; it != beyond; it++)
         push_back(*it);
   
      std::sort(this->begin(), this->end(), swap_1(Traits().less_xy_2_object()));
      std::unique(this->begin(), this->end());
   
      // front() is the point with the largest x coordinate
   
      // push the point p_minus_infinity; the coordinates should never be used
      push_back(Point_2( 1, -1));

      // push the point p_infinity; the coordinates should never be used
      push_back(Point_2(1, 1));
   
      _p_inf = this->end();  // record the iterators to these extreme points
      _p_inf--;
      _p_minus_inf = _p_inf;
      _p_minus_inf--;
   
      Self_iterator child;
      // make p_minus_inf a child of p_inf
      set_rightmost_child(_p_minus_inf, _p_inf); 
      child = this->begin();               // now points to p_0
      while (child != _p_minus_inf)  // make all points children of p_minus_inf
      {
         set_rightmost_child(child, _p_minus_inf);
         child++;
      }
   }


   // the point that comes first in the right-to-left ordering is first
   // in the ordering, after the auxilliary points p_minus_inf and p_inf
   Self_iterator rightmost_point_ref() 
   {
      return this->begin();
   }

   Self_iterator right_sibling(Self_iterator p) 
   {
      if (!(*p).has_right_sibling()) return this->end();
      return (*p).right_sibling();
   }

   Self_iterator left_sibling(Self_iterator p) 
   {
      if (!(*p).has_left_sibling()) return this->end();
      return (*p).left_sibling();
   }

   Self_iterator rightmost_child(Self_iterator p) 
   {
      if (!(*p).has_children()) return this->end();
      return (*p).rightmost_child();
   }

   Self_iterator parent(Self_iterator p) 
   {
      if (!(*p).has_parent()) return this->end();
      return (*p).parent();
   }

   bool parent_is_p_infinity(Self_iterator p) 
   {
      return parent(p) == _p_inf;
   }

   bool parent_is_p_minus_infinity(Self_iterator p) 
   {
      return parent(p) == _p_minus_inf;
   }

   // makes *p the parent of *q
   void set_parent (Self_iterator p, Self_iterator q)
   {
      CGAL_assertion(q != this->end());
      if (p == this->end())
         (*q).clear_parent();
      else
         (*q).set_parent(p);
   }

   // makes *p the rightmost child of *q
   void set_rightmost_child(Self_iterator p, Self_iterator q);

   // makes *p the left sibling of *q
   void set_left_sibling(Self_iterator p, Self_iterator q);

   // makes *p the right sibling of *q
   void set_right_sibling(Self_iterator p, Self_iterator q);

   // NOTE:  this function does not actually remove the node p from the
   //        list; it only reorganizes the pointers so this node is not
   //        in the tree structure anymore
   void erase(Self_iterator p);

private:
   Self_iterator _p_inf;
   Self_iterator _p_minus_inf;
};

}

#ifdef CGAL_CFG_NO_AUTOMATIC_TEMPLATE_INCLUSION
#include <CGAL/Rotation_tree_2.C>
#endif // CGAL_CFG_NO_AUTOMATIC_TEMPLATE_INCLUSION


#endif // CGAL_ROTATION_TREE_H
