/////////////////////////////////////////////////////////////////////////////
// Name:        sdi_example.cpp
// Purpose:     implements the CMyFrame class
// Author:      Mark McCormack
// Modified by: Francesco Montorsi
// Created:     2004-05-03
// RCS-ID:      $Id: sdi_example.cpp,v 1.10 2005/07/23 22:16:13 markmccormack Exp $
// Copyright:   (c) 2004 Mark McCormack
// Licence:     wxWidgets license
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#include <wx/file.h>
#include <wx/wx.h>
#include <wx/config.h>
#include <wx/wfstream.h>
#include "sdi_example.h"

// wxdockit includes
#include "wx/layoutmanager.h"
#include "wx/dockwindow.h"
#include "wx/dockhost.h"
#include "wx/pane.h"
#include "wx/util.h"
#include "wx/slidebar.h"
#include "wx/toolbutton.h"
#include "wx/toolbar.h"

// XPM
#include "bitmaps/new.xpm"
#include "bitmaps/open.xpm"
#include "bitmaps/save.xpm"
#include "bitmaps/copy.xpm"
#include "bitmaps/cut.xpm"
#include "bitmaps/preview.xpm"  // paste XPM
#include "bitmaps/print.xpm"
#include "bitmaps/help.xpm"

// used later in the File menu
enum
{
    ID_MODE_SIMPLE = 2000,
    ID_MODE_COMPACT,
    ID_LOAD_LAYOUT,
    ID_SAVE_LAYOUT,
    ID_EXIT
};

// wx RTTI
IMPLEMENT_CLASS( CMyFrame, wxFrame )
BEGIN_EVENT_TABLE( CMyFrame, wxFrame )
    EVT_MENU( ID_EXIT, CMyFrame::OnExit )
    EVT_MENU( ID_MODE_SIMPLE, CMyFrame::OnMode )
    EVT_MENU( ID_MODE_COMPACT, CMyFrame::OnMode )
    EVT_MENU( ID_LOAD_LAYOUT, CMyFrame::OnLoadLayout )
    EVT_MENU( ID_SAVE_LAYOUT, CMyFrame::OnSaveLayout )
END_EVENT_TABLE()



// --------------------
// CMYFRAME
// --------------------

CMyFrame::CMyFrame()
{
	m_pLayoutManager = NULL;
	m_pSlideBar = NULL;
	m_pFileMenu = NULL;
	m_pWindowMenu = NULL;
}

CMyFrame::~CMyFrame() {
    if (m_pLayoutManager) delete m_pLayoutManager;
}

CMyFrame::CMyFrame( wxWindow *parent, wxWindowID id, const wxString& caption, 
				   const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

bool CMyFrame::Create( wxWindow *parent, wxWindowID id, const wxString& caption, 
					  const wxPoint& pos, const wxSize& size, long style )
{
	// base class initialization
	// -------------------------

    wxFrame::Create( parent, id, caption, pos, size, style );
    Centre();


	// create our slidebar: a slidebar is XXXX and needs XXXX to work
	// -------------------

    m_pSlideBar = new wxSlideBar( this, -1 );


    // create dockwindows
	// ------------------

	CDockWindow *pDockWindow1 = new CDockWindow( this, -1, wxT("Dock Window #1"), 
		wxT("BottomHost"), wxPoint( 64, 64 ), wxSize( 128, 128 ), wxT("d0") );
    wxTextCtrl *tc1 = new wxTextCtrl( this, -1,
		wxT("A textbox in a normal dockwindow; wxDockWindows can be dragged (by their title-bar) and ")
		wxT("docked where the user wants; you can use CTRL to disable auto-docking; you can repeat the ")
        wxT("last dock on a wxDockWindow by double-clicking the title-bar."), 
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    pDockWindow1->SetClient( tc1 );

	CDockWindow *pDockWindow2 = new CDockWindow( this, -1, wxT("Dock Window #2"), 
		wxT("BottomHost"), wxPoint( 96, 96 ), wxSize( 128, 128 ), wxT("d1") );
    wxWindow *p = pDockWindow2->GetClient();
    p->SetBackgroundColour( *wxRED );
    pDockWindow2->Show();
    
	CDockWindow *pDockWindow3 = new CDockWindow( this, -1, wxT("Dock Window #3"), 
		wxT("LeftHost"), wxPoint( 128, 128 ), wxSize( 128, 128 ), wxT("d2") );
    pDockWindow3->GetClient()->SetBackgroundColour( *wxGREEN );
    
	CDockWindow *pDockWindow4 = new CDockWindow( this, -1, wxT("Dock Window #4"), 
		wxT("RightHost"), wxPoint( 160, 160 ), wxSize( 128, 128 ), wxT("d3") );
	wxPane *pane = new wxPane( this, -1, wxT("Embedded Pane") );
    wxTextCtrl *tc4 = new wxTextCtrl( this, -1, 
		wxT("A textbox in a pane; wxPane controls can be docked just like wxDockWindows, however ")
		wxT("their positions can not be moved by the user."), 
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    pDockWindow4->SetClient( pane, true );
    pane->SetClient( tc4 );

//    pDockWindow1->Show();
//    pDockWindow3->Show();
//    pDockWindow4->Show();	


    // setup dockmanager
	// -----------------

    m_pLayoutManager = new wxLayoutManager( this );
    m_pLayoutManager->AddDefaultHosts();

    m_pLayoutManager->AddDockWindow( pDockWindow1 );
    m_pLayoutManager->AddDockWindow( pDockWindow2 );
    m_pLayoutManager->AddDockWindow( pDockWindow3 );    
    m_pLayoutManager->AddDockWindow( pDockWindow4 );

    // add main client window
    wxPane *pPane = new wxPane( this, -1, wxT("Client Pane") );
    m_pLayoutManager->SetLayout( wxDWF_SPLITTER_BORDERS, pPane );
    pPane->ShowCloseButton( false );

    // auto-dock some dockwindows
    wxHostInfo hi;
    hi = m_pLayoutManager->GetDockHost( wxDEFAULT_LEFT_HOST );
    m_pLayoutManager->DockWindow( pDockWindow1, hi );
    hi.GetHost()->SetAreaSize( 128 );
    hi = m_pLayoutManager->GetDockHost( wxDEFAULT_BOTTOM_HOST );
    m_pLayoutManager->DockWindow( pDockWindow4, hi );
    m_pLayoutManager->DockWindow( pDockWindow3, hi );
    hi.GetHost()->SetAreaSize( 128 );


	// create our menu
	// ---------------

    m_pFileMenu = new wxMenu;
    m_pFileMenu->AppendRadioItem(ID_MODE_SIMPLE, _T("Slidebar: &Simple Mode"));
    m_pFileMenu->AppendRadioItem(ID_MODE_COMPACT, _T("Slidebar: &Compact Mode"));
    m_pFileMenu->AppendSeparator();
    m_pFileMenu->Append(ID_LOAD_LAYOUT, _T("L&oad Layout"));
    m_pFileMenu->Append(ID_SAVE_LAYOUT, _T("S&ave Layout"));
    m_pFileMenu->AppendSeparator();
    m_pFileMenu->Append(ID_EXIT , _T("E&xit"));

    // Create the menu to control the window toggles
    m_pWindowMenu = new wxMenu;

	// the wxLayoutManager can be used to handle automatically a "window menu";
	// that is, it can show/hide all the windows under its control through a menu	
    m_pLayoutManager->SetWindowMenu(m_pWindowMenu);

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(m_pFileMenu, wxT("&File"));
    menuBar->Append(m_pWindowMenu, wxT("&Window"));
    SetMenuBar(menuBar);


	// create the first slidebar child: a toolbar
	// ------------------------------------------

    wxToolBar *toolBar = new wxToolBar( this, -1, wxDefaultPosition, wxDefaultSize, 
												wxTB_HORIZONTAL | wxTB_FLAT );
    wxBitmap toolBarBitmaps[8];
    toolBarBitmaps[0] = wxBitmap(new_xpm);
    toolBarBitmaps[1] = wxBitmap(open_xpm);
    toolBarBitmaps[2] = wxBitmap(save_xpm);
    toolBarBitmaps[3] = wxBitmap(copy_xpm);
    toolBarBitmaps[4] = wxBitmap(cut_xpm);
    toolBarBitmaps[5] = wxBitmap(paste_xpm);
    toolBarBitmaps[6] = wxBitmap(print_xpm);
    toolBarBitmaps[7] = wxBitmap(help_xpm);

    toolBar->AddTool(wxID_NEW, _T("New"), toolBarBitmaps[0], _T("New file"));
    toolBar->AddTool(wxID_OPEN, _T("Open"), toolBarBitmaps[1], _T("Open file"));
    toolBar->AddTool(wxID_SAVE, _T("Save"), toolBarBitmaps[2], _T("Toggle button 1"), wxITEM_CHECK);
    
    toolBar->AddTool(wxID_COPY, _T("Copy"), toolBarBitmaps[3], _T("Toggle button 2"), wxITEM_CHECK);
    toolBar->AddTool(wxID_CUT, _T("Cut"), toolBarBitmaps[4], _T("Toggle/Untoggle help button"));
    toolBar->AddTool(wxID_PASTE, _T("Paste"), toolBarBitmaps[5], _T("Paste"));
    toolBar->AddTool(wxID_PRINT, _T("Print"), toolBarBitmaps[6], _T("Delete this tool"));
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_HELP, _T("Help"), toolBarBitmaps[7], _T("Help button"), wxITEM_CHECK);

    // after adding the buttons to the toolbar, must call Realize() to reflect
    // the changes
    toolBar->Realize();
	toolBar->SetToolTip(wxT("This is a non-detachable control"));


	// create the second slidebar child: a panel with a button and a combobox
	// ----------------------------------------------------------------------

    wxPanel *panel = new wxPanel( this, -1, wxDefaultPosition, wxSize( 200, 32 ) );
    wxButton *bu = new wxButton( panel, 1, wxT("&Button"));
	wxString strarr[] = { wxT("...within a wxPanel..."), wxT("...handled by a wxSlideBar") };
    wxComboBox *cb = new wxComboBox( panel, -1, wxT("A combobox..."), 
							wxDefaultPosition, wxDefaultSize, 2, strarr);
    wxBoxSizer *sz = new wxBoxSizer( wxHORIZONTAL );
    sz->Add( bu, 1, wxTOP | wxBOTTOM | wxRIGHT | wxGROW, 3 );
    sz->Add( cb, 3, wxTOP | wxBOTTOM | wxALIGN_CENTER_VERTICAL, 3 );
    panel->SetSizer( sz );
	panel->SetToolTip(wxT("This is a non-detachable control"));

    wxTextCtrl *text = new wxTextCtrl( this, -1, 
		wxT("This text control is registered as 'Text' in the sliderbar menu; ")
		wxT("right-click on this box to see that menu."), 
		wxDefaultPosition, wxSize(64,32), wxTE_MULTILINE );
	text->SetToolTip(wxT("This is a non-detachable control"));


	// setup our slidebar object
	// -------------------------

    m_pSlideBar->AddWindow( toolBar, wxT("Toolbar") );
    m_pSlideBar->AddWindow( panel, wxT("Panel"), wxBF_EXPAND_X );
    m_pSlideBar->AddWindow( text, wxT("Text"), wxBF_EXPAND_X );
    m_pSlideBar->SetMode( wxSLIDE_MODE_SIMPLE );

    return TRUE;
}

void CMyFrame::OnExit( wxCommandEvent & WXUNUSED(event) ) {
	// just exit
    Close();
}

void CMyFrame::OnMode( wxCommandEvent & event ) {
    // change mode
	wxSlideBarMode newMode = (wxSlideBarMode)(event.GetId()-ID_MODE_SIMPLE);
    m_pSlideBar->SetMode( newMode );
}

void CMyFrame::OnLoadLayout( wxCommandEvent & WXUNUSED(event) )
{
    bool res;

#if defined(USE_XML_LOADSAVE) && defined(wxDI_USE_XMLLOADSAVE)

    // update UI
    wxFileInputStream file( wxT("layout.xml") );
	res = m_pLayoutManager->LoadFromXML( file );
    wxSlideBarMode slideMode = m_pSlideBar->GetMode();
    if( slideMode == wxSLIDE_MODE_SIMPLE ) {
        m_pFileMenu->Check( ID_MODE_SIMPLE, true );
    }
    else {
        m_pFileMenu->Check( ID_MODE_COMPACT, true );
    }

#elif defined(USE_BINARY_LOADSAVE)

    wxFileInputStream fi( wxT("layout.bin") );
    wxUtil::ReadWindowLayout( fi, this );
    res = m_pLayoutManager->LoadFromStream( fi );
    res = m_pSlideBar->LoadFromStream( fi );

#elif defined(USE_CONFIG_LOADSAVE)

	// standard wxWidgets settings load&save system
	wxConfigBase *cfg = new wxConfig(wxT("wxDockit SDI sample"));
	res = m_pLayoutManager->LoadFromConfig(cfg, wxT("layout"));

#endif

    if (!res)
		wxMessageBox(wxT("There was a problem while loading..."), 
                    wxT("Error"), wxOK | wxICON_ERROR);
    else
		wxMessageBox(wxT("Layout was correctly loaded..."), 
                    wxT("Success"), wxOK | wxICON_INFORMATION);
}

void CMyFrame::OnSaveLayout( wxCommandEvent & WXUNUSED(event) )
{
    bool res;

#if defined(USE_XML_LOADSAVE) && defined(wxDI_USE_XMLLOADSAVE)

	// XML save has the advantage of being directly user-editable	
    wxFileOutputStream file( wxT("layout.xml") );
	res = m_pLayoutManager->SaveToXML( file );
    // TODO: XML save of main window position/size
    // TODO: XML save of slidebar state

#elif defined(USE_BINARY_LOADSAVE)

    // binary load&save system
	wxFileOutputStream fo( wxT("layout.bin") );
    wxUtil::WriteWindowLayout( fo, this );
    res = m_pLayoutManager->SaveToStream( fo );
    res &= m_pSlideBar->SaveToStream( fo );

#elif defined(USE_CONFIG_LOADSAVE)

	// standard wxWidgets settings load&save system
	wxConfigBase *cfg = new wxConfig(wxT("wxDockit SDI sample"));
	res = m_pLayoutManager->SaveToConfig(cfg, wxT("layout"));

    // TODO: wxConfig save docking system
    // TODO: wxConfig save of main window position/size
    // TODO: wxConfig save of slidebar state
#endif

    if (!res)
		wxMessageBox(wxT("There was a problem while saving..."), 
                    wxT("Error"), wxOK | wxICON_ERROR);
    else
		wxMessageBox(wxT("Layout was correctly saved..."), 
                    wxT("Success"), wxOK | wxICON_INFORMATION);
}
