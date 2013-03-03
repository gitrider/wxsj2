/////////////////////////////////////////////////////////////////////////////
// Name:        bitmap.h
// Purpose:     wxBitmap class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: pngdemo.h,v 1.7 2004/10/06 20:54:09 ABX Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#if defined( __GNUG__) && !defined(__APPLE__)
#pragma interface
#endif

#include "wx/wx.h"

// Define a new application
class MyApp: public wxApp
{
  public:
    MyApp(void){};
    bool OnInit(void);
};

// Define a new frame
class MyCanvas;

class MyFrame: public wxFrame
{
  public:
    MyCanvas *canvas;
    MyFrame(wxFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size);
    virtual ~MyFrame();

    void OnActivate(bool) {}
    void OnLoadFile(wxCommandEvent& event);
    void OnSaveFile(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
DECLARE_EVENT_TABLE()
};

// Define a new canvas which can receive some events
class MyCanvas: public wxScrolledWindow
{
  public:
    MyCanvas(wxWindow *parent, const wxPoint& pos, const wxSize& size);
    ~MyCanvas(void){};

    void OnPaint(wxPaintEvent& event);
DECLARE_EVENT_TABLE()
};

#define PNGDEMO_QUIT       100
#define PNGDEMO_ABOUT      101
#define PNGDEMO_LOAD_FILE  102
#define PNGDEMO_SAVE_FILE  103

