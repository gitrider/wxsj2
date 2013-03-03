// Copyright (c) 1997  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Timer/include/CGAL/Real_timer.h $
// $Id: Real_timer.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Lutz Kettner  <kettner@inf.ethz.ch>  
//                 Matthias Baesken <baesken@informatik.uni-halle.de>

#ifndef CGAL_REAL_TIMER_H
#define CGAL_REAL_TIMER_H 1

#include <CGAL/basic.h>
// For the numerical limits
#include <float.h>

CGAL_BEGIN_NAMESPACE

// SECTION: A Timer Measuring Real-Time
// ========================================================================
// 
// DEFINITION
// 
// A timer `t' of type Real_timer is an object with a state. It is either
// running or it is stopped. The state is controlled with `t.start()'
// and `t.stop()'. The timer counts the time elapsed since its creation
// or last reset. It counts only the time where it is in the running
// state. The time information is given in seconds.

class Real_timer {
private:
    double      elapsed;
    double      started;
    int         interv;
    bool        running;

    static bool m_failed;

    double   get_real_time()     const; // in seconds
    double   compute_precision() const; // in seconds
public:
    Real_timer() : elapsed(0.0), started(0.0), interv(0), running(false) {}

    void     start();
    void     stop ();
    void     reset();
    bool     is_running() const { return running; }

    double   time()       const;
    int      intervals()  const { return interv; }
    double   precision()  const;
    // Returns timer precison. Computes it dynamically at first call.
    // Returns -1.0 if timer system call fails, which, for a proper coded
    // test towards precision leads to an immediate stop of an otherwise 
    // infinite loop (fixed tolerance * total time >= precision).
    double   max()        const { return DBL_MAX; }
};


// -----------------------------------------------------------------------

// Member functions for Real_timer
// ===========================

inline void Real_timer::start() {
    CGAL_precondition( ! running);
    started = get_real_time();
    running = true;
    ++ interv;
}

inline void Real_timer::stop() {
    CGAL_precondition( running);
    double t = get_real_time();
    elapsed += (t - started);
    started = 0.0;
    running = false;
}

inline void Real_timer::reset() {
    interv  = 0;
    elapsed = 0.0;
    if (running) {
	started = get_real_time();
	++ interv;
    } else {
        started = 0.0;
    }
}

inline double Real_timer::time() const {
    if (running) {
        double t = get_real_time();
	return elapsed + (t - started);
    }
    return elapsed;
}

CGAL_END_NAMESPACE

#endif // CGAL_REAL_TIMER_H //
// EOF //
