/////////////////////////////////////////////////////////////////////////////
// Name:        template.h
// Purpose:     
// Author:      
// Modified by: 
// Created:     
// RCS-ID:      $Id: mdi_example.h,v 1.3 2005/05/12 14:03:43 frm Exp $
// Copyright:   (c) 2004 Mark McCormack
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _TEMPLATE_H_
#define _TEMPLATE_H_

/*!
 * Includes
 */

////@begin includes
////@end includes

#include "wx/mdi.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_FRAME 10000
#define ID_FOREIGN 10001
////@end control identifiers

/*!
 * Child class declaration
 */

class Child: public wxMDIChildFrame
{    
    DECLARE_CLASS( Child )

public:
    /// Constructors
    Child(){};
    ~Child(){};
    Child( wxMDIParentFrame* parent, wxWindowID id = -1, const wxString& caption = wxT("wxChild"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE );

    /// Creation
    bool Create( wxMDIParentFrame* parent, wxWindowID id = -1, const wxString& caption = wxT("wxChild"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE );
};

/*!
 * Template class declaration
 */

class Template: public wxMDIParentFrame
{    
    DECLARE_CLASS( Template )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    Template( );
    ~Template();
    Template( wxWindow* parent, wxWindowID id = -1, const wxString& caption = wxT("wxTemplate"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE|wxVSCROLL|wxHSCROLL );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = -1, const wxString& caption = wxT("wxTemplate"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE|wxVSCROLL|wxHSCROLL );

    /// Creates the controls and sizers
    void CreateControls();

    void OnMode( wxCommandEvent& event );
    void OnLoadLayout( wxCommandEvent& event );
    void OnSaveLayout( wxCommandEvent& event );
    void OnExit( wxCommandEvent &event );
    void OnNewChild( wxCommandEvent &event );
    void OnSize( wxSizeEvent& event );
////@begin Template event handler declarations

////@end Template event handler declarations

////@begin Template member function declarations

////@end Template member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin Template member variables
////@end Template member variables
};

#endif
    // _TEMPLATE_H_
