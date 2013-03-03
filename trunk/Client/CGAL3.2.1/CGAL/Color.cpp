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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/iostream/src/CGAL/Color.cpp $
// $Id: Color.cpp,v 1.1 2007/07/09 09:52:12 chaojin Exp $
// 
//
// Author(s)     : Andreas Fabri, Herv� Br�nnimann

#include <CGAL/IO/Color.h>


CGAL_BEGIN_NAMESPACE

const Color BLACK  = Color(0, 0, 0);
const Color WHITE  = Color(255, 255, 255);
const Color GRAY   = Color(100,100,100);

const Color GREEN  = Color(0, 255, 0);

const Color DEEPBLUE   = Color(10, 0, 100);
const Color BLUE   = Color(0, 0, 255);
const Color VIOLET = Color(255, 0, 255);
const Color PURPLE = Color(100, 0, 70);

const Color RED    = Color(255, 0, 0);
const Color ORANGE = Color(235, 150, 0);
const Color YELLOW = Color(255, 255, 0);

CGAL_END_NAMESPACE

