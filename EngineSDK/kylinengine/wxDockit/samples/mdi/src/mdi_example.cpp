/////////////////////////////////////////////////////////////////////////////
// Name:        mdi_example.cpp
// Purpose:     To show the use of wxDockit within an MDI frame
// Author:      
// Modified by: 
// Created:     
// RCS-ID:      $Id: mdi_example.cpp,v 1.6 2005/07/23 22:16:13 markmccormack Exp $
// Copyright:   (c) 2004 Mark McCormack
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "mdi_example.h"

////@begin includes
#include "wx/wx.h"
////@end includes

#include "wx/layoutmanager.h"
#include "wx/dockwindow.h"
#include "wx/dockhost.h"
#include "wx/pane.h"
#include "wx/util.h"
#include "wx/slidebar.h"
#include "wx/toolbutton.h"

#include "wx/toolbar.h"

////@begin XPM images
////@end XPM images

#include "bitmaps/new.xpm"
#include "bitmaps/open.xpm"
#include "bitmaps/save.xpm"
#include "bitmaps/copy.xpm"
#include "bitmaps/cut.xpm"
#include "bitmaps/preview.xpm"  // paste XPM
#include "bitmaps/print.xpm"
#include "bitmaps/help.xpm"

IMPLEMENT_CLASS( Child, wxMDIChildFrame )

/*!
 * Child constructors
 */

Child::Child( wxMDIParentFrame* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * Child creator
 */

bool Child::Create( wxMDIParentFrame* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    bool r = wxMDIChildFrame::Create( parent, id, caption, pos, size, style );

    new wxTextCtrl( this, 0, wxT("TextBox in a mdi child"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );

    return r;
}

/*!
 * Template type definition
 */

enum
{
	ID_MODE_SIMPLE = 2000,
    ID_MODE_COMPACT,
	ID_LOAD_LAYOUT,
	ID_SAVE_LAYOUT,
    ID_NEW_CHILD,
    ID_EXIT
};

IMPLEMENT_CLASS( Template, wxMDIParentFrame )

/*!
 * Template event table definition
 */

BEGIN_EVENT_TABLE( Template, wxMDIParentFrame )
	EVT_SIZE( Template::OnSize )

    EVT_MENU( ID_EXIT, Template::OnExit )
    EVT_MENU( ID_MODE_SIMPLE, Template::OnMode )
    EVT_MENU( ID_MODE_COMPACT, Template::OnMode )
    EVT_MENU( ID_LOAD_LAYOUT,Template::OnLoadLayout )
    EVT_MENU( ID_SAVE_LAYOUT,Template::OnSaveLayout )

    EVT_MENU( ID_NEW_CHILD,Template::OnNewChild )
////@begin Template event table entries
////@end Template event table entries

END_EVENT_TABLE()

/*!
 * Template constructors
 */

wxLayoutManager * pLayoutManager = NULL;

Template::Template( )
{
}

Template::~Template() {
    delete pLayoutManager;
}

Template::Template( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * Template creator
 */

wxSlideBar * slideBar = 0;
wxMenu * fileMenu = 0;
wxMenu * mdiMenu = 0;

bool Template::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin Template member initialisation
////@end Template member initialisation

////@begin Template creation
    wxMDIParentFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end Template creation

    slideBar = new wxSlideBar( this, 0 );

    // create dockwindows
    wxDockWindow * pDockWindow1 = new wxDockWindow( this, 0, wxT("Dock Window #1"), wxPoint( 64, 64 ), wxSize( 128, 128 ) );
    wxTextCtrl * a = new wxTextCtrl( this, 0, wxT("TextBox in a normal dockwindow."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    pDockWindow1->SetClient( a );

    wxDockWindow * pDockWindow2 = new wxDockWindow( this, 0, wxT("Dock Window #2"), wxPoint( 96, 96 ), wxSize( 128, 128 ), wxT("d1") );
    wxWindow * p = pDockWindow2->GetClient();
    p->SetBackgroundColour( *wxRED );
    pDockWindow2->Show();
    
    wxDockWindow * pDockWindow3 = new wxDockWindow( this, 0, wxT("Dock Window #3"), wxPoint( 128, 128 ), wxSize( 128, 128 ), wxT("d2"), wxDWC_NO_CONTROLS );
    pDockWindow3->GetClient()->SetBackgroundColour( *wxGREEN );
    pDockWindow3->Show();
    
    wxDockWindow * pDockWindow4 = new wxDockWindow( this, 0, wxT("Dock Window #4"), wxPoint( 160, 160 ), wxSize( 128, 128 ), wxT("d3") );
    pDockWindow4->Show();
    wxPane * d = new wxPane( this, 0, wxT("Embedded Pane") );
    pDockWindow4->SetClient( d );
    wxTextCtrl * b = new wxTextCtrl( this, 0, wxT("TextBox in a pane."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    d->SetClient( b );
    
    // setup dockmanager
	pLayoutManager = new wxLayoutManager( this );
    pLayoutManager->AddDefaultHosts();
    pLayoutManager->AddDockWindow( pDockWindow1 );
    pLayoutManager->AddDockWindow( pDockWindow2 );
    pLayoutManager->AddDockWindow( pDockWindow3 );
    pLayoutManager->AddDockWindow( pDockWindow4 );

    // auto-dock some dockwindows
    wxHostInfo hi;
    hi = pLayoutManager->GetDockHost( wxDEFAULT_LEFT_HOST );
    pLayoutManager->DockWindow( pDockWindow1, hi );
    pLayoutManager->DockWindow( pDockWindow2, hi );
    hi.GetHost()->SetAreaSize( 128 );
    hi = pLayoutManager->GetDockHost( wxDEFAULT_BOTTOM_HOST );
    pLayoutManager->DockWindow( pDockWindow4, hi );
    hi.GetHost()->SetAreaSize( 128 );

    wxToolBar * toolBar = new wxToolBar( this, 0, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxTB_FLAT );
    wxBitmap toolBarBitmaps[8];
    toolBarBitmaps[0] = wxBitmap(new_xpm);
    toolBarBitmaps[1] = wxBitmap(open_xpm);
    toolBarBitmaps[2] = wxBitmap(save_xpm);
    toolBarBitmaps[3] = wxBitmap(copy_xpm);
    toolBarBitmaps[4] = wxBitmap(cut_xpm);
    toolBarBitmaps[5] = wxBitmap(paste_xpm);
    toolBarBitmaps[6] = wxBitmap(print_xpm);
    toolBarBitmaps[7] = wxBitmap(help_xpm);

    toolBar->AddTool(wxID_NEW, wxT("New"), toolBarBitmaps[0], wxT("New file"));
    toolBar->AddTool(wxID_OPEN, wxT("Open"), toolBarBitmaps[1], wxT("Open file"));
    toolBar->AddTool(wxID_SAVE, wxT("Save"), toolBarBitmaps[2], wxT("Toggle button 1"), wxITEM_CHECK);
    
    toolBar->AddTool(wxID_COPY, wxT("Copy"), toolBarBitmaps[3], wxT("Toggle button 2"), wxITEM_CHECK);
    toolBar->AddTool(wxID_CUT, wxT("Cut"), toolBarBitmaps[4], wxT("Toggle/Untoggle help button"));
    toolBar->AddTool(wxID_PASTE, wxT("Paste"), toolBarBitmaps[5], wxT("Paste"));
    toolBar->AddTool(wxID_PRINT, wxT("Print"), toolBarBitmaps[6], wxT("Delete this tool"));
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_HELP, wxT("Help"), toolBarBitmaps[7], wxT("Help button"), wxITEM_CHECK);

    // after adding the buttons to the toolbar, must call Realize() to reflect
    // the changes
    toolBar->Realize();

	fileMenu = new wxMenu;
    fileMenu->AppendRadioItem(ID_MODE_SIMPLE, wxT("Slidebar: &Simple Mode"));
    fileMenu->AppendRadioItem(ID_MODE_COMPACT, wxT("Slidebar: &Compact Mode"));
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_LOAD_LAYOUT, wxT("L&oad Layout"));
    fileMenu->Append(ID_SAVE_LAYOUT, wxT("S&ave Layout"));
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_EXIT, wxT("E&xit"));

    mdiMenu = new wxMenu;
    mdiMenu->Append(ID_NEW_CHILD, wxT("&New MDI Child"));

    wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, wxT("&File"));
	menuBar->Append(mdiMenu, wxT("&Mdi"));
	SetMenuBar(menuBar);

    wxPanel * w2 = new wxPanel( this, 0, wxDefaultPosition, wxSize( 200, 32 ) );
    wxButton * bu = new wxButton( w2, 1, wxT("&Find"), wxDefaultPosition, wxSize( 64, 24 ) );
    wxComboBox * cb = new wxComboBox( w2, 0, wxT("ComboBox"), wxDefaultPosition, wxSize( 128, 24 ) );
    wxBoxSizer * sz = new wxBoxSizer( wxHORIZONTAL );
    sz->Add( bu, 0, wxTOP | wxBOTTOM | wxGROW, 2 );
    sz->Add( cb, 1, wxTOP | wxBOTTOM | wxLEFT | wxALIGN_CENTER_VERTICAL , 2 );
    w2->SetSizer( sz );

    wxTextCtrl * text = new wxTextCtrl( this, 0, wxT("TextBox"), wxDefaultPosition, wxSize(64,32), wxTE_MULTILINE );

    slideBar->AddWindow( toolBar, wxT("Tools") );
    slideBar->AddWindow( w2, wxT("Mixed"), wxBF_EXPAND_X );
    slideBar->AddWindow( text, wxT("Text"), wxBF_EXPAND_X );
    slideBar->SetMode( wxSLIDE_MODE_SIMPLE );

    return TRUE;
}

/*!
 * Control creation for Template
 */

void Template::CreateControls()
{    
////@begin Template content construction

//    Template* item1 = this;
////@end Template content construction
}

/*!
 * Should we show tooltips?
 */

bool Template::ShowToolTips()
{
    return TRUE;
}

void Template::OnExit( wxCommandEvent & WXUNUSED(event) ) {
    Close();
}

void Template::OnNewChild( wxCommandEvent & WXUNUSED(event) ) {
    // create MDI child window
    static int num = 1;
    wxString caption;
    caption.Printf(wxT("Child %d"), num++);
    Child * pChild = new Child(this, -1, caption);
    pChild->Show();
}

void Template::OnMode( wxCommandEvent & event ) {
	// change mode
	slideBar->SetMode( (wxSlideBarMode)(event.GetId()-ID_MODE_SIMPLE) );
}

void Template::OnLoadLayout( wxCommandEvent & WXUNUSED(event) ) {
    wxFileInputStream fi( wxT("layout.bin") );
    wxUtil::ReadWindowLayout( fi, this );
    pLayoutManager->LoadFromStream( fi );
    slideBar->LoadFromStream( fi );

    // update UI
    wxSlideBarMode slideMode = slideBar->GetMode();
    if( slideMode == wxSLIDE_MODE_SIMPLE ) {
        fileMenu->Check( ID_MODE_SIMPLE, true );
    }
    else {
        fileMenu->Check( ID_MODE_COMPACT, true );
    }
}

void Template::OnSaveLayout( wxCommandEvent & WXUNUSED(event) ) {
    wxFileOutputStream fo( wxT("layout.bin") );
    wxUtil::WriteWindowLayout( fo, this );
    pLayoutManager->SaveToStream( fo );
    slideBar->SaveToStream( fo );
}

void Template::OnSize(wxSizeEvent& event) {
	// let the docking system override the placement of the client window
	if(!pLayoutManager) {
		event.Skip();
	}
}
