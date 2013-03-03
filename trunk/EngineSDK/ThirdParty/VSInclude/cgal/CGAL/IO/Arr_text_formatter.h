// Copyright (c) 2005  Tel-Aviv University (Israel).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Arrangement_2/include/CGAL/IO/Arr_text_formatter.h $
// $Id: Arr_text_formatter.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Michal Meyerovitch <gorgymic@post.tau.ac.il>
//                 Ron Wein           <wein@post.tau.ac.il>
//                 (based on old version by Ester Ezra)
#ifndef CGAL_ARR_TEXT_FORMATTER_H
#define CGAL_ARR_TEXT_FORMATTER_H

/*! \file
 * The header file for the text-formatter classes.
 */

#include <CGAL/basic.h>
#include <iostream>

CGAL_BEGIN_NAMESPACE

/*! \class
 * A class defining a textual (ASCII) input/output format for arrangements
 * and supports reading and writing an arrangement from or to input/output
 * streams.
 */
template <class Arrangement_>
class Arr_text_formatter
{

public:

  typedef Arrangement_                                   Arrangement_2;
  typedef typename Arrangement_2::Size                   Size;
  typedef typename Arrangement_2::Dcel                   Dcel;
  typedef typename Arrangement_2::Traits_2               Traits_2;
  typedef typename Traits_2::X_monotone_curve_2          X_monotone_curve_2;
  typedef typename Traits_2::Point_2                     Point_2;

  typedef typename Arrangement_2::Vertex_handle          Vertex_handle;
  typedef typename Arrangement_2::Halfedge_handle        Halfedge_handle;
  typedef typename Arrangement_2::Face_handle            Face_handle;

  typedef typename Arrangement_2::Vertex_const_handle    Vertex_const_handle;
  typedef typename Arrangement_2::Halfedge_const_handle  Halfedge_const_handle;
  typedef typename Arrangement_2::Face_const_handle      Face_const_handle;
 
protected:

  typedef typename Dcel::Vertex                           DVertex;
  typedef typename Dcel::Halfedge                         DHalfedge;
  typedef typename Dcel::Face                             DFace;
  
  // Data members:
  std::ostream  *m_out;
  IO::Mode       m_old_out_mode;
  std::istream  *m_in;
  IO::Mode       m_old_in_mode;

public:  

  /*! Default constructor.*/
  Arr_text_formatter ():
    m_out (NULL),
    m_in (NULL)
  {}

  /*! Construct an output formatter. */
  Arr_text_formatter (std::ostream& os) :
    m_out (&os),
    m_in(NULL)
  {}

  /*! Construct an input formatter. */
  Arr_text_formatter (std::istream& is) :
    m_out(NULL),
    m_in(&is)
  {}

  /*! Destructor. */
  virtual ~Arr_text_formatter()
  {}

  /*! Set the output stream. */
  void set_out (std::ostream& os)
  {
    m_out = &os;
    return;
  }

  /*! Set the input stream. */
  void set_in (std::istream& is)
  {
    m_in = &is;
    return;
  }

  /*! Get the output stream. */
  inline std::ostream& out ()
  {
    CGAL_assertion (m_out != NULL);
    return (*m_out);
  }

  /*! Get the input stream. */
  inline std::istream& in ()
  {
    CGAL_assertion (m_in != NULL);
    return (*m_in);
  }

  /// \name Global write functions.
  //@{

  /*! Write a begin-arrangement comment. */
  void write_arrangement_begin ()
  {
    CGAL_assertion (m_out != NULL);
    m_old_out_mode = get_mode(*m_out);
    set_ascii_mode (*m_out);
    _write_comment ("BEGIN ARRANGEMENT");

    return;
  }

  /*! Write an end-arrangement comment. */
  void write_arrangement_end()
  {
    _write_comment ("END ARRANGEMENT");
    set_mode (*m_out, m_old_out_mode);

    return;
  }

  /*! Write a labeled size value. */
  void write_size (const char *label, Size size)
  { 
    _write_comment (label);
    out() << size << '\n';
    return;
  }

  /*! Write a begin-vertices comment. */
  void write_vertices_begin ()
  {
    _write_comment ("BEGIN VERTICES");
    return;
  }

  /*! Write an end-vertices comment. */
  void write_vertices_end ()
  {
    _write_comment ("END VERTICES");
    return;
  }

  /*! Write a begin-edges comment. */
  void write_edges_begin ()
  {
    _write_comment ("BEGIN EDGES");
    return;
  }

  /*! Write an end-edges comment. */
  void write_edges_end ()
  {
    _write_comment ("END EDGES");
    return;
  }

  /*! Write a begin-faces comment. */
  void write_faces_begin ()
  {
    _write_comment ("BEGIN FACES");
    return;
  }

  /*! Write an end-faces comment. */
  void write_faces_end ()
  {
    _write_comment ("END FACES");
    return;
  }
  //@}

  /// \name Write a vertex.
  //@{
  void write_vertex_begin ()
  {}

  void write_vertex_end ()
  {}
  
  virtual void write_point (const Point_2& p)
  {
    out() << p << std::endl;
    return;
  }

  virtual void write_vertex_data (Vertex_const_handle v)
  {}
  //@}

  /// \name Write an edge.
  //@{
  void write_edge_begin ()
  {}

  void write_edge_end ()
  {}

  void write_vertex_index (std::size_t idx)
  {
    out() << idx << ' ';
    return;
  }

  virtual void write_x_monotone_curve (const X_monotone_curve_2& cv)
  {
    out() << cv << std::endl;
    return;
  }

  virtual void write_halfedge_data (Halfedge_const_handle he)
  {}
  //@}

  /// \name Write a face.
  //@{
  void write_face_begin ()
  {
    _write_comment ("BEGIN FACE");
    return;
  }

  void write_face_end ()
  {
    _write_comment ("END FACE");
  }

  void write_outer_ccb_begin ()
  {}

  void write_outer_ccb_end ()
  {}

  void write_holes_begin ()
  {}

  void write_holes_end ()
  {}

  virtual void write_face_data (Face_const_handle )
  {}

  void write_ccb_halfedges_begin()
  {}
  
  void write_ccb_halfedges_end()
  {
    out() << std::endl;    
  }

  void write_halfedge_index (std::size_t idx)
  {
    out() << idx << ' ';
    return;
  }

  void write_isolated_vertices_begin ()
  {}

  void write_isolated_vertices_end ()
  {
    out() << std::endl;
  }
  //@}

  /// \name Global read functions.
  //@{

  /*! Start reading an arrangement. */
  void read_arrangement_begin () 
  {
    CGAL_assertion (m_in != NULL);
    m_old_in_mode = get_mode(*m_in);
    set_ascii_mode(*m_in);
    _skip_comments();
  }

  /*! Read the arrangement edge. */
  void read_arrangement_end() 
  {
    _skip_comments();
    set_mode(*m_in, m_old_in_mode);
  }

  /*! Read a size value (with a label comment line before it). */
  Size read_size (const char *title = NULL)
  {
    std::size_t   val;

    _skip_comments();
    in() >> val;
    _skip_until_EOL();

    return (val);
  }

  /*! Reading the arrangement vertices. */
  void read_vertices_begin()
  {
    _skip_comments();
  }

  void read_vertices_end()
  {
    _skip_comments();
  }

  /*! Reading the arrangement edges. */
  void read_edges_begin()
  {
    _skip_comments();
  }

  void read_edges_end()
  {
    _skip_comments();
  }

  /*! Reading the arrangement faces. */
  void read_faces_begin()
  {
    _skip_comments();
  }

  void read_faces_end()
  {
    _skip_comments();
  }
  //@}

  /// \name Reading a vertex.
  //@{
  void read_vertex_begin ()
  {}
  
  void read_vertex_end ()
  {}

  virtual void read_point (Point_2& p) 
  {
    in() >> p;
    _skip_until_EOL();

    return;
  }

  virtual void read_vertex_data (Vertex_handle v)
  {}
  //@}

  /// \name Reading an edge.
  //@{
  void read_edge_begin ()
  {}
  
  void read_edge_end ()
  {}
  
  std::size_t read_vertex_index () 
  {
    std::size_t  val;

    in() >> val;
    return (val);
  }

  virtual void read_x_monotone_curve (X_monotone_curve_2& cv) 
  {
    in() >> cv;
    _skip_until_EOL();

    return;
  }

  virtual void read_halfedge_data (Halfedge_handle )
  {}
 
  /// \name Reading a face.
  //@{
  void read_face_begin ()
  {
    _skip_comments();
  }
  
  void read_face_end ()
  {
    _skip_comments();
  }

  void read_outer_ccb_begin ()
  {}
  
  void read_outer_ccb_end ()
  {}

  std::size_t read_halfedge_index ()
  { 
    std::size_t  val;

    in() >> val;
    return (val);
  }

  void read_holes_begin ()
  {}
  
  void read_holes_end ()
  {}

  void read_inner_ccb_begin ()
  {
    _skip_comments();
  }
  
  void read_inner_ccb_end ()
  {}

  void read_ccb_halfedges_begin()
  {}
  
  void read_ccb_halfedges_end() 
  {
    _skip_until_EOL ();
  }

  void read_isolated_vertices_begin ()
  {}
  
  void read_isolated_vertices_end () 
  {
    _skip_until_EOL();
  }

  virtual void read_face_data (Face_handle )
  {}
  //@}

protected:

  /*! Write a comment line. */
  void _write_comment (const char *str)
  {
    out() << "# " << str << std::endl;
    return;
  }

  /*! Skip until end of line. */
  void _skip_until_EOL () 
  {
    CGAL_assertion (m_in != NULL);

    int     c;
    while ((c = m_in->get()) != EOF && c != '\n');
    return;
  }
  
  /*! Skip comment lines. */
  void _skip_comments () 
  {
    CGAL_assertion (m_in != NULL);

    int     c;
    while ((c = m_in->get()) != EOF && c == '#')
      _skip_until_EOL();
    m_in->putback (c);

    return;
  }

};

/*! \class
 * A class defining a textual (ASCII) input/output format for arrangements
 * that store auxiliary dat with their face records, as they are templated
 * by a face-extended DCEL class.
 */
template <class Arrangement_>
class Arr_face_extended_text_formatter :
  public Arr_text_formatter<Arrangement_>
{

public:

  typedef Arrangement_                              Arrangement_2;
  typedef Arr_text_formatter<Arrangement_2>         Base;

  typedef typename Base::Size                       Size;
  typedef typename Base::Dcel                       Dcel;
  typedef typename Base::Traits_2                   Traits_2;
  typedef typename Traits_2::X_monotone_curve_2     X_monotone_curve_2;
  typedef typename Traits_2::Point_2                Point_2;

  typedef typename Base::Vertex_handle              Vertex_handle;
  typedef typename Base::Halfedge_handle            Halfedge_handle;
  typedef typename Base::Face_handle                Face_handle;

  typedef typename Base::Vertex_const_handle        Vertex_const_handle;
  typedef typename Base::Halfedge_const_handle      Halfedge_const_handle;
  typedef typename Base::Face_const_handle          Face_const_handle;

  /*! Default constructor.*/
  Arr_face_extended_text_formatter () :
    Base ()
  {}

  /*! Construct an output formatter. */
  Arr_face_extended_text_formatter (std::ostream& os) :
    Base (os)
  {}

  /*! Construct an input formatter. */
  Arr_face_extended_text_formatter (std::istream& is) :
    Base (is)
  {}

  /*! Write the auxiliary data associated with the given face. */
  virtual void write_face_data (Face_const_handle f)
  {
    this->out() << f->data() << '\n';
  }
  
  /*! Read a face-data object and attach it to the given face. */
  virtual void read_face_data (Face_handle f)
  {
    this->in() >> f->data();
    this->_skip_until_EOL();
  }
};

/*! \class
 * A class defining a textual (ASCII) input/output format for arrangements
 * that store auxiliary dat with all their DCEL records, as they are templated
 * by a extended DCEL class.
 */
template <class Arrangement_>
class Arr_extended_dcel_text_formatter :
  public Arr_text_formatter<Arrangement_>
{
public:

  typedef Arrangement_                              Arrangement_2;
  typedef Arr_text_formatter<Arrangement_2>         Base;

  typedef typename Base::Size                       Size;
  typedef typename Base::Dcel                       Dcel;
  typedef typename Base::Traits_2                   Traits_2;
  typedef typename Traits_2::X_monotone_curve_2     X_monotone_curve_2;
  typedef typename Traits_2::Point_2                Point_2;

  typedef typename Base::Vertex_handle              Vertex_handle;
  typedef typename Base::Halfedge_handle            Halfedge_handle;
  typedef typename Base::Face_handle                Face_handle;

  typedef typename Base::Vertex_const_handle        Vertex_const_handle;
  typedef typename Base::Halfedge_const_handle      Halfedge_const_handle;
  typedef typename Base::Face_const_handle          Face_const_handle;

  /*! Default constructor.*/
  Arr_extended_dcel_text_formatter () :
    Base ()
  {}

  /*! Construct an output formatter. */
  Arr_extended_dcel_text_formatter (std::ostream& os) :
    Base (os)
  {}

  /*! Construct an input formatter. */
  Arr_extended_dcel_text_formatter (std::istream& is) :
    Base (is)
  {}

  /*! Write the auxiliary data associated with the given vertex. */
  virtual void write_vertex_data (Vertex_const_handle v)
  {
    this->out() << v->data() << '\n';
  }
  
  /*! Read a vertex-data object and attach it to the given vertex. */
  virtual void read_vertex_data (Vertex_handle v)
  {
    this->in() >> v->data();
    this->_skip_until_EOL();
  }

  /*! Write the auxiliary data associated with the given halfedge. */
  virtual void write_halfedge_data (Halfedge_const_handle he)
  {
    this->out() << he->data() << '\n';
  }
  
  /*! Read a halfedge-data object and attach it to the given halfedge. */
  virtual void read_halfedge_data (Halfedge_handle he)
  {
    this->in() >> he->data();
    this->_skip_until_EOL();
  }

  /*! Write the auxiliary data associated with the given face. */
  virtual void write_face_data (Face_const_handle f)
  {
    this->out() << f->data() << '\n';
  }
  
  /*! Read a face-data object and attach it to the given face. */
  virtual void read_face_data (Face_handle f)
  {
    this->in() >> f->data();
    this->_skip_until_EOL();
  }
};

CGAL_END_NAMESPACE

#endif
