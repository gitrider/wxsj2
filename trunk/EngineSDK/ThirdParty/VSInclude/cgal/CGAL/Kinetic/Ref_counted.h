// Copyright (c) 2005  Stanford University (USA).
// All rights reserved.
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/Ref_counted.h $
// $Id: Ref_counted.h 29334 2006-03-10 00:00:09Z drussel $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_REF_COUNTED_H
#define CGAL_REF_COUNTED_H
#include <CGAL/Kinetic/basic.h>
#include <boost/intrusive_ptr.hpp>

//#define NEW_REF_COUNTED

CGAL_KINETIC_BEGIN_INTERNAL_NAMESPACE;

class Ref_counted_base;

void intrusive_ptr_add_ref(const Ref_counted_base *t);

void intrusive_ptr_release(const Ref_counted_base *t);

class Ref_counted_base
{
  typedef Ref_counted_base This;
  Ref_counted_base(const Ref_counted_base&) : reference_count_(0) {
    std::cerr << "Copy constructor called. Why?" << std::endl;
  }
  Ref_counted_base operator=(const Ref_counted_base&) const
  {
    std::cerr << "Assignment called. Why?" << std::endl;
    return *this;
  }
public:
  Ref_counted_base() : reference_count_(0) {}

  void write(std::ostream &out) const
  {
    out << "(" << reference_count_ << ")";
  }
  bool is_referenced() const
  {
    return reference_count_ != 0;
  }

  virtual ~Ref_counted_base() {
    CGAL_assertion(reference_count_==0);
  }

  friend void intrusive_ptr_release(const This *t);
  friend void intrusive_ptr_add_ref(const This *t);

  unsigned int reference_count() const {return reference_count_;}

  void  new_ref() const { ++reference_count_; }
  void  delete_ref() const
  {
    CGAL_precondition(reference_count_!=0);
    --reference_count_;
  }

  mutable unsigned int reference_count_;
};

inline void intrusive_ptr_add_ref(const Ref_counted_base *t)
{
  t->new_ref();
}


inline void intrusive_ptr_release(const Ref_counted_base *t)
{
  t->delete_ref();
  if (t->reference_count() == 0) {
    delete t;
  }
}


CGAL_KINETIC_END_INTERNAL_NAMESPACE;

CGAL_KINETIC_BEGIN_NAMESPACE;

template <class T>
class Ref_counted: public internal::Ref_counted_base

{
  typedef internal::Ref_counted_base P;
  typedef Ref_counted<T> This;

  This operator=(const This &) {
    this_should_not_compile(T());
    assert(0);
    return *this;
  }
  Ref_counted(const Ref_counted&o) {
    this_should_not_compile(T());
    assert(0);
  }
public:


  Ref_counted() {                          
  }

  typedef typename boost::intrusive_ptr<T> Handle;

  typedef typename boost::intrusive_ptr<const T> Const_handle;
};

CGAL_KINETIC_END_NAMESPACE;
#endif
