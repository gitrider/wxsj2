/////////////////////////////////////////////////////////////////////////////
// Name:        layout.cpp
// Purpose:     Layout sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: layout.cpp,v 1.41 2005/01/18 21:21:57 RD Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/sizer.h"
#include "wx/gbsizer.h"
#include "wx/statline.h"
#include "wx/notebook.h"

#include "layout.h"

// ----------------------------------------------------------------------------
// MyApp
// ----------------------------------------------------------------------------

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
  // Create the main frame window
  MyFrame *frame = new MyFrame;

  frame->Show(true);

  return true;
}

// ----------------------------------------------------------------------------
// MyFrame
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
  EVT_MENU(LAYOUT_ABOUT, MyFrame::OnAbout)
  EVT_MENU(LAYOUT_QUIT, MyFrame::OnQuit)

  EVT_MENU(LAYOUT_TEST_SIZER, MyFrame::TestFlexSizers)
  EVT_MENU(LAYOUT_TEST_NB_SIZER, MyFrame::TestNotebookSizers)
  EVT_MENU(LAYOUT_TEST_GB_SIZER, MyFrame::TestGridBagSizer)
END_EVENT_TABLE()

// Define my frame constructor
MyFrame::MyFrame()
       : wxFrame(NULL, wxID_ANY, _T("wxWidgets Layout Demo"),
                 wxDefaultPosition, wxDefaultSize,
                 wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE)
{
  // Make a menubar
  wxMenu *file_menu = new wxMenu;

  file_menu->Append(LAYOUT_TEST_SIZER, _T("Test wx&FlexSizer"));
  file_menu->Append(LAYOUT_TEST_NB_SIZER, _T("&Test notebook sizers"));
  file_menu->Append(LAYOUT_TEST_GB_SIZER, _T("Test &gridbag sizer"));

  file_menu->AppendSeparator();
  file_menu->Append(LAYOUT_QUIT, _T("E&xit"), _T("Quit program"));

  wxMenu *help_menu = new wxMenu;
  help_menu->Append(LAYOUT_ABOUT, _T("&About"), _T("About layout demo"));

  wxMenuBar *menu_bar = new wxMenuBar;

  menu_bar->Append(file_menu, _T("&File"));
  menu_bar->Append(help_menu, _T("&Help"));

  // Associate the menu bar with the frame
  SetMenuBar(menu_bar);

#if wxUSE_STATUSBAR
  CreateStatusBar(2);
  SetStatusText(_T("wxWidgets layout demo"));
#endif // wxUSE_STATUSBAR

  wxPanel* p = new wxPanel(this, -1);
  
  // we want to get a dialog that is stretchable because it
  // has a text ctrl in the middle. at the bottom, we have
  // two buttons which.

  wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );

  // 1) top: create wxStaticText with minimum size equal to its default size
  topsizer->Add(
    new wxStaticText( p, wxID_ANY, _T("An explanation (wxALIGN_RIGHT).") ),
    wxSizerFlags().Align(wxALIGN_RIGHT).Border(wxALL & ~wxBOTTOM, 5));

  // 2) top: create wxTextCtrl with minimum size (100x60)
  topsizer->Add(
    new wxTextCtrl( p, wxID_ANY, _T("My text (wxEXPAND)."), wxDefaultPosition, wxSize(100,60), wxTE_MULTILINE),
    wxSizerFlags(1).Expand().Border(wxALL, 5));

  // 2.5) Gratuitous test of wxStaticBoxSizers
  wxBoxSizer *statsizer = new wxStaticBoxSizer(
    new wxStaticBox(p, wxID_ANY, _T("A wxStaticBoxSizer")), wxVERTICAL );
  statsizer->Add(
    new wxStaticText(p, wxID_ANY, _T("And some TEXT inside it")),
    wxSizerFlags().Center().Border(wxALL, 30));
  topsizer->Add(
    statsizer,
    wxSizerFlags(1).Expand().Border(wxALL, 10));

    // 2.7) And a test of wxGridSizer
    wxGridSizer *gridsizer = new wxGridSizer(2, 5, 5);
    gridsizer->Add(new wxStaticText(p, wxID_ANY, _T("Label")),
                   wxSizerFlags().Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL));
    gridsizer->Add(new wxTextCtrl(p, wxID_ANY, _T("Grid sizer demo")),
                   wxSizerFlags(1).Align(wxGROW | wxALIGN_CENTER_VERTICAL));
    gridsizer->Add(new wxStaticText(p, wxID_ANY, _T("Another label")),
                   wxSizerFlags().Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL));
    gridsizer->Add(new wxTextCtrl(p, wxID_ANY, _T("More text")),
                   wxSizerFlags(1).Align(wxGROW | wxALIGN_CENTER_VERTICAL));
    gridsizer->Add(new wxStaticText(p, wxID_ANY, _T("Final label")),
                   wxSizerFlags().Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL));
    gridsizer->Add(new wxTextCtrl(p, wxID_ANY, _T("And yet more text")),
                   wxSizerFlags().Align(wxGROW | wxALIGN_CENTER_VERTICAL));
    topsizer->Add(
        gridsizer,
        wxSizerFlags().Proportion(1).Expand().Border(wxALL, 10));


#if wxUSE_STATLINE
  // 3) middle: create wxStaticLine with minimum size (3x3)
  topsizer->Add(
     new wxStaticLine( p, wxID_ANY, wxDefaultPosition, wxSize(3,3), wxHORIZONTAL),
     wxSizerFlags().Expand());
#endif // wxUSE_STATLINE


  // 4) bottom: create two centred wxButtons
  wxBoxSizer *button_box = new wxBoxSizer( wxHORIZONTAL );
  button_box->Add(
     new wxButton( p, wxID_ANY, _T("Two buttons in a box") ),
     wxSizerFlags().Border(wxALL, 7));
  button_box->Add(
     new wxButton( p, wxID_ANY, _T("(wxCENTER)") ),
     wxSizerFlags().Border(wxALL, 7));

  topsizer->Add(button_box, wxSizerFlags().Center());

  p->SetSizer( topsizer );
  
  // don't allow frame to get smaller than what the sizers tell it and also set
  // the initial size as calculated by the sizers
  topsizer->SetSizeHints( this );
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event) )
{
    Close(true);
}

void MyFrame::TestFlexSizers(wxCommandEvent& WXUNUSED(event) )
{
    MyFlexSizerFrame *newFrame = new MyFlexSizerFrame(_T("Flex Sizer Test Frame"), 50, 50);
    newFrame->Show(true);
}

void MyFrame::TestNotebookSizers(wxCommandEvent& WXUNUSED(event) )
{
    MySizerDialog dialog( this, _T("Notebook Sizer Test Dialog") );

    dialog.ShowModal();
}


void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event) )
{
    (void)wxMessageBox(_T("wxWidgets GUI library layout demo\n"),
            _T("About Layout Demo"), wxOK|wxICON_INFORMATION);
}

void MyFrame::TestGridBagSizer(wxCommandEvent& WXUNUSED(event) )
{
    MyGridBagSizerFrame *newFrame = new
        MyGridBagSizerFrame(_T("wxGridBagSizer Test Frame"), 50, 50);
    newFrame->Show(true);
}


// ----------------------------------------------------------------------------
//  MyFlexSizerFrame
// ----------------------------------------------------------------------------

void MyFlexSizerFrame::InitFlexSizer(wxFlexGridSizer *sizer, wxWindow* parent)
{
    for ( int i = 0; i < 3; i++ )
    {
        for ( int j = 0; j < 3; j++ )
        {
            sizer->Add(new wxStaticText
                           (
                            parent,
                            wxID_ANY,
                            wxString::Format(_T("(%d, %d)"), i + 1, j + 1),
                            wxDefaultPosition,
                            wxDefaultSize,
                            wxALIGN_CENTER
                           ),
                       0, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALL, 3);
        }
    }
}

MyFlexSizerFrame::MyFlexSizerFrame(const wxChar *title, int x, int y )
            : wxFrame(NULL, wxID_ANY, title, wxPoint(x, y) )
{
    wxFlexGridSizer *sizerFlex;
    wxPanel* p = new wxPanel(this, -1);

    // consttuct the first column
    wxSizer *sizerCol1 = new wxBoxSizer(wxVERTICAL);
    sizerCol1->Add(new wxStaticText(p, wxID_ANY, _T("Ungrowable:")), 0, wxCENTER | wxTOP, 20);
    sizerFlex = new wxFlexGridSizer(3, 3);
    InitFlexSizer(sizerFlex, p);
    sizerCol1->Add(sizerFlex, 1, wxALL | wxEXPAND, 10);

    sizerCol1->Add(new wxStaticText(p, wxID_ANY, _T("Growable middle column:")), 0, wxCENTER | wxTOP, 20);
    sizerFlex = new wxFlexGridSizer(3, 3);
    InitFlexSizer(sizerFlex, p);
    sizerFlex->AddGrowableCol(1);
    sizerCol1->Add(sizerFlex, 1, wxALL | wxEXPAND, 10);

    sizerCol1->Add(new wxStaticText(p, wxID_ANY, _T("Growable middle row:")), 0, wxCENTER | wxTOP, 20);
    sizerFlex = new wxFlexGridSizer(3, 3);
    InitFlexSizer(sizerFlex, p);
    sizerFlex->AddGrowableRow(1);
    sizerCol1->Add(sizerFlex, 1, wxALL | wxEXPAND, 10);

    sizerCol1->Add(new wxStaticText(p, wxID_ANY, _T("All growable columns:")), 0, wxCENTER | wxTOP, 20);
    sizerFlex = new wxFlexGridSizer(3, 3);
    InitFlexSizer(sizerFlex, p);
    sizerFlex->AddGrowableCol(0, 1);
    sizerFlex->AddGrowableCol(1, 2);
    sizerFlex->AddGrowableCol(2, 3);
    sizerCol1->Add(sizerFlex, 1, wxALL | wxEXPAND, 10);

    // the second one
    wxSizer *sizerCol2 = new wxBoxSizer(wxVERTICAL);
    sizerCol2->Add(new wxStaticText(p, wxID_ANY, _T("Growable middle row and column:")), 0, wxCENTER | wxTOP, 20);
    sizerFlex = new wxFlexGridSizer(3, 3);
    InitFlexSizer(sizerFlex, p);
    sizerFlex->AddGrowableCol(1);
    sizerFlex->AddGrowableRow(1);
    sizerCol2->Add(sizerFlex, 1, wxALL | wxEXPAND, 10);

    sizerCol2->Add(new wxStaticText(p, wxID_ANY, _T("Same with horz flex direction")), 0, wxCENTER | wxTOP, 20);
    sizerFlex = new wxFlexGridSizer(3, 3);
    InitFlexSizer(sizerFlex, p);
    sizerFlex->AddGrowableCol(1);
    sizerFlex->AddGrowableRow(1);
    sizerFlex->SetFlexibleDirection(wxHORIZONTAL);
    sizerCol2->Add(sizerFlex, 1, wxALL | wxEXPAND, 10);

    sizerCol2->Add(new wxStaticText(p, wxID_ANY, _T("Same with grow mode == \"none\"")), 0, wxCENTER | wxTOP, 20);
    sizerFlex = new wxFlexGridSizer(3, 3);
    InitFlexSizer(sizerFlex, p);
    sizerFlex->AddGrowableCol(1);
    sizerFlex->AddGrowableRow(1);
    sizerFlex->SetFlexibleDirection(wxHORIZONTAL);
    sizerFlex->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_NONE);
    sizerCol2->Add(sizerFlex, 1, wxALL | wxEXPAND, 10);

    sizerCol2->Add(new wxStaticText(p, wxID_ANY, _T("Same with grow mode == \"all\"")), 0, wxCENTER | wxTOP, 20);
    sizerFlex = new wxFlexGridSizer(3, 3);
    InitFlexSizer(sizerFlex, p);
    sizerFlex->AddGrowableCol(1);
    sizerFlex->AddGrowableRow(1);
    sizerFlex->SetFlexibleDirection(wxHORIZONTAL);
    sizerFlex->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_ALL);
    sizerCol2->Add(sizerFlex, 1, wxALL | wxEXPAND, 10);

    // add both columns to grid sizer
    wxGridSizer *sizerTop = new wxGridSizer(2, 0, 20);
    sizerTop->Add(sizerCol1, 1, wxEXPAND);
    sizerTop->Add(sizerCol2, 1, wxEXPAND);

    p->SetSizer(sizerTop);
    sizerTop->SetSizeHints(this);
}

// ----------------------------------------------------------------------------
// MySizerDialog
// ----------------------------------------------------------------------------

MySizerDialog::MySizerDialog(wxWindow *parent, const wxChar *title)
             : wxDialog(parent, wxID_ANY, wxString(title))
{
    // Begin with first hierarchy: a notebook at the top and
    // and OK button at the bottom.

    wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );

    wxNotebook *notebook = new wxNotebook( this, wxID_ANY );
    topsizer->Add( notebook, 1, wxGROW );

    wxButton *button = new wxButton( this, wxID_OK, _T("OK") );
    topsizer->Add( button, 0, wxALIGN_RIGHT | wxALL, 10 );

    // First page: one big text ctrl
    wxTextCtrl *multi = new wxTextCtrl( notebook, wxID_ANY, _T("TextCtrl."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    notebook->AddPage( multi, _T("Page One") );

    // Second page: a text ctrl and a button
    wxPanel *panel = new wxPanel( notebook, wxID_ANY );
    notebook->AddPage( panel, _T("Page Two") );

    wxSizer *panelsizer = new wxBoxSizer( wxVERTICAL );

    wxTextCtrl *text = new wxTextCtrl( panel, wxID_ANY, _T("TextLine 1."), wxDefaultPosition, wxSize(250,-1) );
    panelsizer->Add( text, 0, wxGROW|wxALL, 30 );
    text = new wxTextCtrl( panel, wxID_ANY, _T("TextLine 2."), wxDefaultPosition, wxSize(250,-1) );
    panelsizer->Add( text, 0, wxGROW|wxALL, 30 );
    wxButton *button2 = new wxButton( panel, wxID_ANY, _T("Hallo") );
    panelsizer->Add( button2, 0, wxALIGN_RIGHT | wxLEFT|wxRIGHT|wxBOTTOM, 30 );

    panel->SetAutoLayout( true );
    panel->SetSizer( panelsizer );

    // Tell dialog to use sizer
    SetSizer( topsizer );
    topsizer->SetSizeHints( this );
}

// ----------------------------------------------------------------------------
// MyGridBagSizerFrame
// ----------------------------------------------------------------------------

// some simple macros to help make the sample code below more clear
#define TEXTCTRL(text)   new wxTextCtrl(p, wxID_ANY, _T(text))
#define MLTEXTCTRL(text) new wxTextCtrl(p, wxID_ANY, _T(text), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE)
#define POS(r, c)        wxGBPosition(r,c)
#define SPAN(r, c)       wxGBSpan(r,c)

wxChar* gbsDescription =_T("\
The wxGridBagSizer is similar to the wxFlexGridSizer except the items are explicitly positioned\n\
in a virtual cell of the layout grid, and column or row spanning is allowed.  For example, this\n\
static text is positioned at (0,0) and it spans 7 columns.");


// Some IDs
enum {
    GBS_HIDE_BTN = 1212,
    GBS_SHOW_BTN,
    GBS_MOVE_BTN1,
    GBS_MOVE_BTN2,

    GBS_MAX,
};


BEGIN_EVENT_TABLE(MyGridBagSizerFrame, wxFrame)
    EVT_BUTTON( GBS_HIDE_BTN,  MyGridBagSizerFrame::OnHideBtn)
    EVT_BUTTON( GBS_SHOW_BTN,  MyGridBagSizerFrame::OnShowBtn)
    EVT_BUTTON( GBS_MOVE_BTN1, MyGridBagSizerFrame::OnMoveBtn)
    EVT_BUTTON( GBS_MOVE_BTN2, MyGridBagSizerFrame::OnMoveBtn)
END_EVENT_TABLE()


MyGridBagSizerFrame::MyGridBagSizerFrame(const wxChar *title, int x, int y )
    : wxFrame( NULL, wxID_ANY, title, wxPoint(x, y) )
{
    wxPanel* p = new wxPanel(this, wxID_ANY);
    m_panel = p;
    m_gbs = new wxGridBagSizer();


    m_gbs->Add( new wxStaticText(p, wxID_ANY, gbsDescription),
                POS(0,0), SPAN(1, 7),
                wxALIGN_CENTER | wxALL, 5);

    m_gbs->Add( TEXTCTRL("pos(1,0)"),   POS(1,0) );
    m_gbs->Add( TEXTCTRL("pos(1,1)"),   POS(1,1) );
    m_gbs->Add( TEXTCTRL("pos(2,0)"),   POS(2,0) );
    m_gbs->Add( TEXTCTRL("pos(2,1)"),   POS(2,1) );
    m_gbs->Add( MLTEXTCTRL("pos(3,2), span(1,2)\nthis row and col are growable"),
              POS(3,2), SPAN(1,2), wxEXPAND );
    m_gbs->Add( MLTEXTCTRL("pos(4,3)\nspan(3,1)"),
              POS(4,3), SPAN(3,1), wxEXPAND );
    m_gbs->Add( TEXTCTRL("pos(5,4)"),   POS(5,4), wxDefaultSpan, wxEXPAND );
    m_gbs->Add( TEXTCTRL("pos(6,5)"),   POS(6,5), wxDefaultSpan, wxEXPAND );
    m_gbs->Add( TEXTCTRL("pos(7,6)"),   POS(7,6) );

    //m_gbs->Add( TEXTCTRL("bad position"), POS(4,3) );  // Test for assert
    //m_gbs->Add( TEXTCTRL("bad position"), POS(5,3) );  // Test for assert


    m_moveBtn1 = new wxButton(p, GBS_MOVE_BTN1, _T("Move this to (3,6)"));
    m_moveBtn2 = new wxButton(p, GBS_MOVE_BTN2, _T("Move this to (3,6)"));
    m_gbs->Add( m_moveBtn1, POS(10,2) );
    m_gbs->Add( m_moveBtn2, POS(10,3) );

    m_hideBtn = new wxButton(p, GBS_HIDE_BTN, _T("Hide this item -->"));
    m_gbs->Add(m_hideBtn, POS(12, 3));

    m_hideTxt = new wxTextCtrl(p, wxID_ANY, _T("pos(12,4), size(150, -1)"),
                                wxDefaultPosition, wxSize(150,-1));
    m_gbs->Add( m_hideTxt, POS(12,4) );

    m_showBtn = new wxButton(p, GBS_SHOW_BTN, _T("<-- Show it again"));
    m_gbs->Add(m_showBtn, POS(12, 5));
    m_showBtn->Disable();

    m_gbs->Add(10,10, POS(14,0));

    m_gbs->AddGrowableRow(3);
    m_gbs->AddGrowableCol(2);

    p->SetSizerAndFit(m_gbs);
    SetClientSize(p->GetSize());
}


void MyGridBagSizerFrame::OnHideBtn(wxCommandEvent&)
{
    m_gbs->Hide(m_hideTxt);
    m_hideBtn->Disable();
    m_showBtn->Enable();
    m_gbs->Layout();
}

void MyGridBagSizerFrame::OnShowBtn(wxCommandEvent&)
{
    m_gbs->Show(m_hideTxt);
    m_hideBtn->Enable();
    m_showBtn->Disable();
    m_gbs->Layout();
}


void MyGridBagSizerFrame::OnMoveBtn(wxCommandEvent& event)
{
    wxButton* btn = (wxButton*)event.GetEventObject();
    wxGBPosition curPos = m_gbs->GetItemPosition(btn);

    // if it's already at the "other" spot then move it back
    if (curPos == wxGBPosition(3,6))
    {
        m_gbs->SetItemPosition(btn, m_lastPos);
        btn->SetLabel(_T("Move this to (3,6)"));
    }
    else
    {
        if ( m_gbs->CheckForIntersection(wxGBPosition(3,6), wxGBSpan(1,1)) )
            wxMessageBox(
_T("wxGridBagSizer will not allow items to be in the same cell as\n\
another item, so this operation will fail.  You will also get an assert\n\
when compiled in debug mode."), _T("Warning"), wxOK | wxICON_INFORMATION);

        if ( m_gbs->SetItemPosition(btn, wxGBPosition(3,6)) )
        {
            m_lastPos = curPos;
            btn->SetLabel(_T("Move it back"));
        }
    }
    m_gbs->Layout();
}
