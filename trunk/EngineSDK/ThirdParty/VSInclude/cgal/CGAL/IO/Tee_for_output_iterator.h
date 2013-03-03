// Copyright (c) 1999  Max-Planck-Institute Saarbruecken (Germany).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Convex_hull_2/include/CGAL/IO/Tee_for_output_iterator.h $
// $Id: Tee_for_output_iterator.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Stefan Schirra


#ifndef CGAL_IO_TEE_FOR_OUTPUT_ITERATOR_H
#define CGAL_IO_TEE_FOR_OUTPUT_ITERATOR_H

#include <CGAL/basic.h>
#include <iterator>
#include <vector>
#include <CGAL/Handle.h>

CGAL_BEGIN_NAMESPACE
template <class T> class _Tee_for_output_iterator_rep;

template <class OutputIterator, class T>
class Tee_for_output_iterator 
  : public Handle
{
  typedef std::vector<T>                             container;
  typedef typename container::iterator               iterator;
  typedef T                                          value_type;
  typedef std::output_iterator_tag                   iterator_category;
  typedef std::iterator_traits<iterator>             iter_traits;
  typedef typename iter_traits::pointer              pointer;
  typedef typename iter_traits::reference            reference;

public:
  Tee_for_output_iterator(const OutputIterator& o) : o_it(o) 
  {  PTR = (Rep*) new _Tee_for_output_iterator_rep<T>(); }

  Tee_for_output_iterator<OutputIterator,T>& 
  operator=(const T& value) 
  { 
    ptr()->output_so_far.push_back(value);
    *o_it = value;
    return *this;
  }

  Tee_for_output_iterator<OutputIterator,T>& 
  operator*() 
  { return *this; }

  Tee_for_output_iterator<OutputIterator,T>& 
  operator++() 
  { 
    ++o_it; 
    return *this; 
  } 

  Tee_for_output_iterator<OutputIterator,T> 
  operator++(int) 
  { 
    Tee_for_output_iterator<OutputIterator,T> tmp = *this;
    o_it++; 
    return tmp; 
  } 

  iterator
  output_so_far_begin()
  { return ptr()->output_so_far.begin(); }

  iterator
  output_so_far_end()
  { return ptr()->output_so_far.end(); }

  OutputIterator&
  to_output_iterator()
  { return o_it; }

  _Tee_for_output_iterator_rep<T>*
  ptr()
  { return (_Tee_for_output_iterator_rep<T>*)PTR; }

protected:
  OutputIterator o_it;
};

template <class T>
class _Tee_for_output_iterator_rep : public Rep
{
public:
  std::vector<T> output_so_far;
};

CGAL_END_NAMESPACE

#endif // CGAL_IO_TEE_FOR_OUTPUT_ITERATOR_H
