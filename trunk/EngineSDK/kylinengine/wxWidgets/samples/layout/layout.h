/////////////////////////////////////////////////////////////////////////////
// Name:        layout.h
// Purpose:     Layout sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: layout.h,v 1.16 2005/06/02 13:27:44 MW Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// Define a new application
class MyApp: public wxApp
{
public:
    MyApp(){};
    bool OnInit();
};

// the main frame class
class MyFrame : public wxFrame
{
public:
    MyFrame();

    void TestFlexSizers(wxCommandEvent& event);
    void TestNotebookSizers(wxCommandEvent& event);
    void TestGridBagSizer(wxCommandEvent& event);

    void OnAbout(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);

private:
    DECLARE_EVENT_TABLE()
};

// a frame using flex sizers for layout
class MyFlexSizerFrame : public wxFrame
{
public:
    MyFlexSizerFrame(const wxChar *title, int x, int y );

private:
    void InitFlexSizer(wxFlexGridSizer *sizer, wxWindow* parent);
};


// a dialog using notebook sizer for layout
class MySizerDialog : public wxDialog
{
public:
    MySizerDialog(wxWindow *parent, const wxChar *title);
};


// a frame using wxGridBagSizer for layout
class MyGridBagSizerFrame : public wxFrame
{
public:
    MyGridBagSizerFrame(const wxChar *title, int x, int y );

    void OnHideBtn(wxCommandEvent&);
    void OnShowBtn(wxCommandEvent&);
    void OnMoveBtn(wxCommandEvent&);

private:
    wxGridBagSizer*     m_gbs;
    wxPanel*            m_panel;
    wxButton*           m_hideBtn;
    wxButton*           m_showBtn;
    wxTextCtrl*         m_hideTxt;

    wxButton*           m_moveBtn1;
    wxButton*           m_moveBtn2;
    wxGBPosition        m_lastPos;

    DECLARE_EVENT_TABLE()
};





// controls and menu constants
enum
{
    LAYOUT_QUIT = wxID_EXIT,
    LAYOUT_ABOUT = wxID_ABOUT,
    LAYOUT_TEST_SIZER = 101,
    LAYOUT_TEST_NB_SIZER = 102,
    LAYOUT_TEST_GB_SIZER = 103
};

