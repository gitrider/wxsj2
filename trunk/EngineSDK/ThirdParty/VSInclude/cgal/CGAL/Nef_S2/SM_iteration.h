// Copyright (c) 1997-2002  Max-Planck-Institute Saarbruecken (Germany).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Nef_S2/include/CGAL/Nef_S2/SM_iteration.h $
// $Id: SM_iteration.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Michael Seel  <seel@mpi-sb.mpg.de>
//               : Peter Hachenberger <hachenberger@mpi-sb.mpg.de>

#ifndef CGAL_SM_ITERATION_H
#define CGAL_SM_ITERATION_H

#undef CGAL_forall_iterators
#define CGAL_forall_iterators(x,S)\
for(x = (S).begin(); x != (S).end(); ++x)

#undef CGAL_forall_svertices
#define CGAL_forall_svertices(x,SM)\
for(x = (SM).svertices_begin(); x != (SM).svertices_end(); ++x) 

#undef CGAL_forall_shalfedges
#define CGAL_forall_shalfedges(x,SM)\
for(x = (SM).shalfedges_begin(); x != (SM).shalfedges_end(); ++x) 

#undef CGAL_forall_sedges
#define CGAL_forall_sedges(x,SM)\
for(x = (SM).shalfedges_begin(); x != (SM).shalfedges_end(); ++(++x))

#undef CGAL_forall_shalfloops
#define CGAL_forall_shalfloops(x,SM)\
for(x = (SM).shalfloops_begin(); x != (SM).shalfloops_end(); ++x) 

#undef CGAL_forall_sfaces
#define CGAL_forall_sfaces(x,SM)\
for(x = (SM).sfaces_begin(); x != (SM).sfaces_end(); ++x) 

#undef CGAL_forall_sface_cycles_of
#define CGAL_forall_sface_cycles_of(x,F)\
for(x = (F)->sface_cycles_begin(); x != (F)->sface_cycles_end(); ++x) 

#endif //CGAL_SM_ITERATION_H

