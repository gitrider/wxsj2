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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kernel_23/src/CGAL/kernel.cpp $
// $Id: kernel.cpp,v 1.1 2007/07/09 09:52:13 chaojin Exp $
// 
//
// Author(s)     : Andreas Fabri, Stefan Schirra
 
#include <CGAL/Origin.h>
#include <CGAL/aff_transformation_tags.h>

CGAL_BEGIN_NAMESPACE

Translation             TRANSLATION;
Rotation                ROTATION;
Scaling                 SCALING;
Reflection              REFLECTION;
Identity_transformation IDENTITY;

const Origin      ORIGIN = Origin();
const Null_vector NULL_VECTOR = Null_vector();

CGAL_END_NAMESPACE
