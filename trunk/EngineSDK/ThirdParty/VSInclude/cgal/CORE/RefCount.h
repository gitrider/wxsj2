/****************************************************************************
 * Core Library Version 1.7, August 2004
 * Copyright (c) 1995-2004 Exact Computation Project
 * All rights reserved.
 *
 * This file is part of CORE (http://cs.nyu.edu/exact/core/); you may
 * redistribute it under the terms of the Q Public License version 1.0.
 * See the file LICENSE.QPL distributed with CORE.
 *
 * Licensees holding a valid commercial license may use this file in
 * accordance with the commercial license agreement provided with the
 * software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * File: RefCount.h
 * Synopsis: 
 *     1. This file defines two templated classes:
 *               RCRepImpl<class N>
 *     to create Reps of the class N.  The basic functions provided by
 *     this class is reference counting.   The other class is
 *               RCImpl<class T>
 *     for implementing the envelop-letter paradigm for a class whose Rep
 *     is the class T.  So, T is the "letter", and RCImpl<T> the "envelop".
 *
 *     2. All Rep classes (BigIntRep, BigFloatRep, BigRatRep, ExprRep, etc)
 *     are derived from RCRepImpl<N>.  E.g.,
 *
 *         class BigRatRep : public RCRepImp<BigRatRep> {
 *         ...
 *         }
 *     (Note the recursive use of "BigRatRep").
 *
 *     3. All Number classes (BigInt, BigFloat, BigRat, Expr, etc)
 *     are derived from RCImpl<T>.  E.g.
 *
 *         typedef RCImpl<BigRatRep> RCBigRat;
 *         class BigRat : public RCBigRat {
 *         ...
 *         }
 * 
 * Written by 
 *       Zilin Du <zilin@cs.nyu.edu>
 *       Chee Yap <yap@cs.nyu.edu>
 *
 * WWW URL: http://cs.nyu.edu/exact/
 * Email: exact@cs.nyu.edu
 *
 * $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Core/include/CORE/RefCount.h $
 * $Id: RefCount.h 28567 2006-02-16 14:30:13Z lsaboret $
 ***************************************************************************/


#ifndef _CORE_REFCOUNT_H_
#define _CORE_REFCOUNT_H_

CORE_BEGIN_NAMESPACE

template<class Deriving>
class RCRepImpl {
public:
  RCRepImpl() : refCount(1) {}
  void incRef() {
    ++refCount;
  }
  // Without static_cast this to Deriving*,
  // the destructor of Deriving class will never been called.
  // this is an example of simulating dynamic binding from ATL.
  void decRef() {
    if (--refCount == 0)
      delete static_cast<Deriving*>(this);
  }
  int getRefCount() const {
    return refCount;
  }
private:
  int refCount;
};

template<class T>
class RCImpl {
protected:
  RCImpl(T* p) : rep(p) {}
  RCImpl(const RCImpl& x) : rep(x.rep) {}
  T* rep; ///<= rep is the actual representation
public:
  /// get rep (const)
  const T& getRep() const {
    return *rep;
  }
  /// get rep (non-const)
  T& getRep() {
    return *rep;
  }
  /// clone data
  void makeCopy() {
    if (rep->getRefCount() > 1) {
      T* oldValue = rep;
      rep->decRef(); // safe since rep has been referred at least once.
      rep = oldValue ? new T(*oldValue) : 0;
    }
  }
#ifdef CORE_RC_DEBUG
  /// get counter
  int getRefCount() const {
    return rep->getRefCount();
  }
#endif
};

CORE_END_NAMESPACE
#endif // _CORE_REFCOUNT_H_
