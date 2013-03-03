/////////////////////////////////////////////////////////////////////////////
// Name:        sdi_example.h
// Purpose:     declares stuff required for our mini SDI sample
// Author:      Mark McCormack
// Modified by: Francesco Montorsi
// Created:     2004-05-03
// RCS-ID:      $Id: sdi_example.h,v 1.8 2005/05/10 20:12:12 frm Exp $
// Copyright:   (c) 2004 Mark McCormack
// Licence:     wxWidgets license
/////////////////////////////////////////////////////////////////////////////


#ifndef SDI_EXAMPLE_H
#define SDI_EXAMPLE_H

// includes
#include "wx/frame.h"
#include "wx/dockwindow.h"
#include "wx/slidebar.h"

// IDs
#define ID_FRAME		10000
#define ID_FOREIGN		10001

// choose the load/save layout system to test...
//#define USE_CONFIG_LOADSAVE
//#define USE_BINARY_LOADSAVE
#define USE_XML_LOADSAVE

// declares the wxBarHash hash map which will be used
WX_DECLARE_HASH_MAP( int, wxWindow*, wxIntegerHash, wxIntegerEqual, wxBarHash );


// This will be the user-dockable window used in this test program.
// It derives from wxDockWindow which is obviously a wxDockIt class.
// It provides all the required 
class CDockWindow : public wxDockWindow
{
public:
    CDockWindow( wxWindow * parent, wxWindowID id, const wxString& title, 
				const wxString& host, const wxPoint& pos, const wxSize& size, 
				const wxString& name, unsigned int flags = wxDWC_DEFAULT)
        : wxDockWindow( parent, id, title, pos, size, name, flags), m_pHost(host) {}

    wxString GetHost() const
		{ return m_pHost; } 

private:
	wxString m_pHost;
};


// A simple wxFrame: it creates some wxDockWindow-derived windows and 
// various TODO
class CMyFrame : public wxFrame
{    
    DECLARE_CLASS(CMyFrame)
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    CMyFrame();
    ~CMyFrame();
    CMyFrame( wxWindow* parent, wxWindowID id = -1, const wxString& caption = wxT("wxDockIt SDI sample"), 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, 
		long style = wxDEFAULT_FRAME_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = -1, 
		const wxString& caption = wxT("wxDockIt SDI sample"), 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxDEFAULT_FRAME_STYLE );

	// Event handlers
    void OnMode( wxCommandEvent& event );
    void OnLoadLayout( wxCommandEvent& event );
    void OnSaveLayout( wxCommandEvent& event );
    void OnExit( wxCommandEvent &event );

    /// Should we show tooltips?
    static bool ShowToolTips();


protected:
	
	// The hash map used to keep trace of all our dockable windows
    //wxBarHash m_pDockWindows;

	wxLayoutManager *m_pLayoutManager;
	wxSlideBar *m_pSlideBar;
	wxMenu *m_pFileMenu;
	wxMenu *m_pWindowMenu;
};

#endif
    // SDI_EXAMPLE_H

