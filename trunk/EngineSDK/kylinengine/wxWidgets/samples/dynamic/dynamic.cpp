/////////////////////////////////////////////////////////////////////////////
// Name:        dynamic.cpp
// Purpose:     Dynamic events wxWidgets sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: dynamic.cpp,v 1.17 2005/06/02 12:04:09 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation
#pragma interface
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if defined(__WXGTK__) || defined(__WXX11__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXCOCOA__)
#include "mondrian.xpm"
#endif

// Define a new application type
class MyApp: public wxApp
{ public:
    bool OnInit(void);
};

// Define a new frame type
class MyFrame: public wxFrame
{ public:
    MyFrame(wxFrame *frame, wxChar *title, int x, int y, int w, int h);

 public:
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};

// ID for the menu commands
#define DYNAMIC_QUIT   wxID_EXIT
#define DYNAMIC_TEXT   101
#define DYNAMIC_ABOUT  wxID_ABOUT

// Create a new application object
IMPLEMENT_APP  (MyApp)

// `Main program' equivalent, creating windows and returning main app frame
bool MyApp::OnInit(void)
{
  // Create the main frame window
  MyFrame *frame = new MyFrame(NULL, _T("Dynamic wxWidgets App"), 50, 50, 450, 340);

  // You used to have to do some casting for param 4, but now there are type-safe handlers
  frame->Connect( DYNAMIC_QUIT,  wxID_ANY,
                    wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame::OnQuit) );
  frame->Connect( DYNAMIC_ABOUT, wxID_ANY,
                    wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame::OnAbout) );

  // Give it an icon
#ifdef __WXMSW__
  frame->SetIcon(wxIcon(_T("mondrian")));
#else
  frame->SetIcon(wxIcon(mondrian_xpm));
#endif

  // Make a menubar
  wxMenu *file_menu = new wxMenu;

  file_menu->Append(DYNAMIC_ABOUT, _T("&About"));
  file_menu->Append(DYNAMIC_QUIT, _T("E&xit"));
  wxMenuBar *menu_bar = new wxMenuBar;
  menu_bar->Append(file_menu, _T("&File"));
  frame->SetMenuBar(menu_bar);

  // Make a panel with a message
  wxPanel *panel = new wxPanel(frame, wxID_ANY, wxPoint(0, 0), wxSize(400, 200), wxTAB_TRAVERSAL);

  (void)new wxStaticText(panel, 311, _T("Hello!"), wxPoint(10, 10), wxDefaultSize, 0);

  // Show the frame
  frame->Show(true);

  SetTopWindow(frame);

  return true;
}

// My frame constructor
MyFrame::MyFrame(wxFrame *frame, wxChar *title, int x, int y, int w, int h):
  wxFrame(frame, wxID_ANY, title, wxPoint(x, y), wxSize(w, h))
{}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event) )
{
  Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event) )
{
  wxMessageDialog dialog(this, _T("This demonstrates dynamic event handling"),
    _T("About Dynamic"), wxYES_NO|wxCANCEL);

  dialog.ShowModal();
}


