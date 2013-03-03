/////////////////////////////////////////////////////////////////////////////
// Program:     wxWidgets Widgets Sample
// Name:        widgets.cpp
// Purpose:     Sample showing most of the simple wxWidgets widgets
// Author:      Vadim Zeitlin
// Created:     27.03.01
// Id:          $Id: widgets.cpp,v 1.33 2005/05/18 23:19:47 VZ Exp $
// Copyright:   (c) 2001 Vadim Zeitlin
// License:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/log.h"
    #include "wx/frame.h"
    #include "wx/menu.h"

    #include "wx/button.h"
    #include "wx/checkbox.h"
    #include "wx/listbox.h"
    #include "wx/statbox.h"
    #include "wx/stattext.h"
    #include "wx/textctrl.h"
#endif

#include "wx/sysopt.h"
#include "wx/bookctrl.h"
#include "wx/sizer.h"
#include "wx/colordlg.h"
#include "wx/fontdlg.h"
#include "wx/textdlg.h"

#include "widgets.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// control ids
enum
{
    Widgets_ClearLog = 100,
    Widgets_Quit,
#if wxUSE_TOOLTIPS
    Widgets_SetTooltip,
#endif // wxUSE_TOOLTIPS
    Widgets_SetFgColour,
    Widgets_SetBgColour,
    Widgets_SetFont,
    Widgets_Enable
};

// ----------------------------------------------------------------------------
// our classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class WidgetsApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
};

// Define a new frame type: this is going to be our main frame
class WidgetsFrame : public wxFrame
{
public:
    // ctor(s) and dtor
    WidgetsFrame(const wxString& title);
    virtual ~WidgetsFrame();

protected:
    // event handlers
#if USE_LOG
    void OnButtonClearLog(wxCommandEvent& event);
#endif // USE_LOG
    void OnExit(wxCommandEvent& event);

#if wxUSE_MENUS
#if wxUSE_TOOLTIPS
    void OnSetTooltip(wxCommandEvent& event);
#endif // wxUSE_TOOLTIPS
    void OnSetFgCol(wxCommandEvent& event);
    void OnSetBgCol(wxCommandEvent& event);
    void OnSetFont(wxCommandEvent& event);
    void OnEnable(wxCommandEvent& event);
#endif // wxUSE_MENUS

    // initialize the book: add all pages to it
    void InitBook();

private:
    // the panel containing everything
    wxPanel *m_panel;

#if USE_LOG
    // the listbox for logging messages
    wxListBox *m_lboxLog;

    // the log target we use to redirect messages to the listbox
    wxLog *m_logTarget;
#endif // USE_LOG

    // the book containing the test pages
    wxBookCtrl *m_book;

    // and the image list for it
    wxImageList *m_imaglist;

#if wxUSE_MENUS
    // last chosen fg/bg colours and font
    wxColour m_colFg,
             m_colBg;
    wxFont   m_font;
#endif // wxUSE_MENUS

    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

#if USE_LOG
// A log target which just redirects the messages to a listbox
class LboxLogger : public wxLog
{
public:
    LboxLogger(wxListBox *lbox, wxLog *logOld)
    {
        m_lbox = lbox;
        //m_lbox->Disable(); -- looks ugly under MSW
        m_logOld = logOld;
    }

    virtual ~LboxLogger()
    {
        wxLog::SetActiveTarget(m_logOld);
    }

private:
    // implement sink functions
    virtual void DoLog(wxLogLevel level, const wxChar *szString, time_t t)
    {
        // don't put trace messages into listbox or we can get into infinite
        // recursion
        if ( level == wxLOG_Trace )
        {
            if ( m_logOld )
            {
                // cast is needed to call protected method
                ((LboxLogger *)m_logOld)->DoLog(level, szString, t);
            }
        }
        else
        {
            wxLog::DoLog(level, szString, t);
        }
    }

    virtual void DoLogString(const wxChar *szString, time_t WXUNUSED(t))
    {
        wxString msg;
        TimeStamp(&msg);
        msg += szString;

        #ifdef __WXUNIVERSAL__
            m_lbox->AppendAndEnsureVisible(msg);
        #else // other ports don't have this method yet
            m_lbox->Append(msg);
            m_lbox->SetFirstItem(m_lbox->GetCount() - 1);
        #endif
    }

    // the control we use
    wxListBox *m_lbox;

    // the old log target
    wxLog *m_logOld;
};
#endif // USE_LOG

// array of pages
WX_DEFINE_ARRAY_PTR(WidgetsPage *, ArrayWidgetsPage);

// ----------------------------------------------------------------------------
// misc macros
// ----------------------------------------------------------------------------

IMPLEMENT_APP(WidgetsApp)

// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(WidgetsFrame, wxFrame)
#if USE_LOG
    EVT_BUTTON(Widgets_ClearLog, WidgetsFrame::OnButtonClearLog)
#endif // USE_LOG
    EVT_BUTTON(Widgets_Quit, WidgetsFrame::OnExit)

#if wxUSE_TOOLTIPS
    EVT_MENU(Widgets_SetTooltip, WidgetsFrame::OnSetTooltip)
#endif // wxUSE_TOOLTIPS

    EVT_MENU(Widgets_SetFgColour, WidgetsFrame::OnSetFgCol)
    EVT_MENU(Widgets_SetBgColour, WidgetsFrame::OnSetBgCol)
    EVT_MENU(Widgets_SetFont,     WidgetsFrame::OnSetFont)
    EVT_MENU(Widgets_Enable,      WidgetsFrame::OnEnable)

    EVT_MENU(wxID_EXIT, WidgetsFrame::OnExit)
END_EVENT_TABLE()

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// app class
// ----------------------------------------------------------------------------

bool WidgetsApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    // the reason for having these ifdef's is that I often run two copies of
    // this sample side by side and it is useful to see which one is which
    wxString title;
#if defined(__WXUNIVERSAL__)
    title = _T("wxUniv/");
#endif

#if defined(__WXMSW__)
    title += _T("wxMSW");
#elif defined(__WXGTK__)
    title += _T("wxGTK");
#elif defined(__WXMAC__)
    title += _T("wxMAC");
#elif defined(__WXMOTIF__)
    title += _T("wxMOTIF");
#else
    title += _T("wxWidgets");
#endif

    wxFrame *frame = new WidgetsFrame(title + _T(" widgets demo"));
    frame->Show();

    //wxLog::AddTraceMask(_T("listbox"));
    //wxLog::AddTraceMask(_T("scrollbar"));
    //wxLog::AddTraceMask(_T("focus"));

    return true;
}

// ----------------------------------------------------------------------------
// WidgetsFrame construction
// ----------------------------------------------------------------------------

WidgetsFrame::WidgetsFrame(const wxString& title)
            : wxFrame(NULL, wxID_ANY, title,
                      wxPoint(0, 50), wxDefaultSize,
                      wxDEFAULT_FRAME_STYLE |
                      wxNO_FULL_REPAINT_ON_RESIZE |
                      wxCLIP_CHILDREN |
                      wxTAB_TRAVERSAL)
{
    // init everything
#if USE_LOG
    m_lboxLog = (wxListBox *)NULL;
    m_logTarget = (wxLog *)NULL;
#endif // USE_LOG
    m_book = (wxBookCtrl *)NULL;
    m_imaglist = (wxImageList *)NULL;

#if wxUSE_MENUS
    // create the menubar
    wxMenuBar *mbar = new wxMenuBar;
    wxMenu *menuWidget = new wxMenu;
#if wxUSE_TOOLTIPS
    menuWidget->Append(Widgets_SetTooltip, _T("Set &tooltip...\tCtrl-T"));
    menuWidget->AppendSeparator();
#endif // wxUSE_TOOLTIPS
    menuWidget->Append(Widgets_SetFgColour, _T("Set &foreground...\tCtrl-F"));
    menuWidget->Append(Widgets_SetBgColour, _T("Set &background...\tCtrl-B"));
    menuWidget->Append(Widgets_SetFont,     _T("Set f&ont...\tCtrl-O"));
    menuWidget->AppendCheckItem(Widgets_Enable,  _T("&Enable/disable\tCtrl-E"));
    menuWidget->AppendSeparator();
    menuWidget->Append(wxID_EXIT, _T("&Quit\tCtrl-Q"));
    mbar->Append(menuWidget, _T("&Widget"));
    SetMenuBar(mbar);

    mbar->Check(Widgets_Enable, true);
#endif // wxUSE_MENUS

    // create controls
    m_panel = new wxPanel(this, wxID_ANY,
        wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN);

    wxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);

    // we have 2 panes: book with pages demonstrating the controls in the
    // upper one and the log window with some buttons in the lower

    int style = wxNO_FULL_REPAINT_ON_RESIZE|wxCLIP_CHILDREN|wxBC_DEFAULT;
    // Uncomment to suppress page theme (draw in solid colour)
    //style |= wxNB_NOPAGETHEME;

    m_book = new wxBookCtrl(m_panel, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, style);
    InitBook();

#ifndef __SMARTPHONE__
    // the lower one only has the log listbox and a button to clear it
#if USE_LOG
    wxSizer *sizerDown = new wxStaticBoxSizer(
        new wxStaticBox( m_panel, wxID_ANY, _T("&Log window") ),
        wxVERTICAL);

    m_lboxLog = new wxListBox(m_panel, wxID_ANY);
    sizerDown->Add(m_lboxLog, 1, wxGROW | wxALL, 5);
    sizerDown->SetMinSize(100, 150);
#else
    wxSizer *sizerDown = new wxBoxSizer(wxVERTICAL);
#endif // USE_LOG

    wxBoxSizer *sizerBtns = new wxBoxSizer(wxHORIZONTAL);
    wxButton *btn;
#if USE_LOG
    btn = new wxButton(m_panel, Widgets_ClearLog, _T("Clear &log"));
    sizerBtns->Add(btn);
    sizerBtns->Add(10, 0); // spacer
#endif // USE_LOG
    btn = new wxButton(m_panel, Widgets_Quit, _T("E&xit"));
    sizerBtns->Add(btn);
    sizerDown->Add(sizerBtns, 0, wxALL | wxALIGN_RIGHT, 5);

    // put everything together
    sizerTop->Add(m_book, 1, wxGROW | (wxALL & ~(wxTOP | wxBOTTOM)), 10);
    sizerTop->Add(0, 5, 0, wxGROW); // spacer in between
    sizerTop->Add(sizerDown, 0,  wxGROW | (wxALL & ~wxTOP), 10);

#else // !__SMARTPHONE__/__SMARTPHONE__

    sizerTop->Add(m_book, 1, wxGROW | wxALL );

#endif // __SMARTPHONE__

    m_panel->SetSizer(sizerTop);

    sizerTop->Fit(this);
    sizerTop->SetSizeHints(this);

#if USE_LOG && !defined(__WXCOCOA__)
    // wxCocoa's listbox is too flakey to use for logging right now
    // now that everything is created we can redirect the log messages to the
    // listbox
    m_logTarget = new LboxLogger(m_lboxLog, wxLog::GetActiveTarget());
    wxLog::SetActiveTarget(m_logTarget);
#endif
}

void WidgetsFrame::InitBook()
{
    m_imaglist = new wxImageList(32, 32);

    ArrayWidgetsPage pages;
    wxArrayString labels;

    // we need to first create all pages and only then add them to the book
    // as we need the image list first
    WidgetsPageInfo *info = WidgetsPage::ms_widgetPages;
    while ( info )
    {
        WidgetsPage *page = (*info->GetCtor())(m_book, m_imaglist);
        pages.Add(page);

        labels.Add(info->GetLabel());

        info = info->GetNext();
    }

    m_book->SetImageList(m_imaglist);

    // now do add them
    size_t count = pages.GetCount();
    for ( size_t n = 0; n < count; n++ )
    {
        m_book->AddPage(
                        pages[n],
                        labels[n],
                        false, // don't select
                        n // image id
                       );

/*
        wxColour colour = m_book->MSWGetBgColourForChild(pages[n]);
        pages[n]->SetBackgroundColour(colour);
*/
    }
}

WidgetsFrame::~WidgetsFrame()
{
#if USE_LOG
    delete m_logTarget;
#endif // USE_LOG
    delete m_imaglist;
}

// ----------------------------------------------------------------------------
// WidgetsFrame event handlers
// ----------------------------------------------------------------------------

void WidgetsFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

#if USE_LOG
void WidgetsFrame::OnButtonClearLog(wxCommandEvent& WXUNUSED(event))
{
    m_lboxLog->Clear();
}
#endif // USE_LOG

#if wxUSE_MENUS

#if wxUSE_TOOLTIPS

void WidgetsFrame::OnSetTooltip(wxCommandEvent& WXUNUSED(event))
{
    static wxString s_tip = _T("This is a tooltip");

    wxString s = wxGetTextFromUser
                 (
                    _T("Tooltip text: "),
                    _T("Widgets sample"),
                    s_tip,
                    this
                 );

    if ( s.empty() )
        return;

    WidgetsPage *page = wxStaticCast(m_book->GetCurrentPage(), WidgetsPage);
    page->GetWidget()->SetToolTip(s_tip = s);

    wxControl *ctrl2 = page->GetWidget2();
    if ( ctrl2 )
        ctrl2->SetToolTip(s);
}

#endif // wxUSE_TOOLTIPS

void WidgetsFrame::OnSetFgCol(wxCommandEvent& WXUNUSED(event))
{
#if wxUSE_COLOURDLG
    // allow for debugging the default colour the first time this is called
    WidgetsPage *page = wxStaticCast(m_book->GetCurrentPage(), WidgetsPage);
    if (!m_colFg.Ok())
        m_colFg = page->GetForegroundColour();

    wxColour col = wxGetColourFromUser(this, m_colFg);
    if ( !col.Ok() )
        return;

    m_colFg = col;

    page->GetWidget()->SetForegroundColour(m_colFg);
    page->GetWidget()->Refresh();

    wxControl *ctrl2 = page->GetWidget2();
    if ( ctrl2 )
    {
        ctrl2->SetForegroundColour(m_colFg);
        ctrl2->Refresh();
    }
#else
    wxLogMessage(_T("Colour selection dialog not available in current build."));
#endif
}

void WidgetsFrame::OnSetBgCol(wxCommandEvent& WXUNUSED(event))
{
#if wxUSE_COLOURDLG
    WidgetsPage *page = wxStaticCast(m_book->GetCurrentPage(), WidgetsPage);
    if ( !m_colBg.Ok() )
        m_colBg = page->GetBackgroundColour();

    wxColour col = wxGetColourFromUser(this, m_colBg);
    if ( !col.Ok() )
        return;

    m_colBg = col;

    page->GetWidget()->SetBackgroundColour(m_colBg);
    page->GetWidget()->Refresh();

    wxControl *ctrl2 = page->GetWidget2();
    if ( ctrl2 )
    {
        ctrl2->SetBackgroundColour(m_colFg);
        ctrl2->Refresh();
    }
#else
    wxLogMessage(_T("Colour selection dialog not available in current build."));
#endif
}

void WidgetsFrame::OnSetFont(wxCommandEvent& WXUNUSED(event))
{
#if wxUSE_FONTDLG
    WidgetsPage *page = wxStaticCast(m_book->GetCurrentPage(), WidgetsPage);
    if (!m_font.Ok())
        m_font = page->GetFont();

    wxFont font = wxGetFontFromUser(this, m_font);
    if ( !font.Ok() )
        return;

    m_font = font;

    page->GetWidget()->SetFont(m_font);
    page->GetWidget()->Refresh();

    wxControl *ctrl2 = page->GetWidget2();
    if ( ctrl2 )
    {
        ctrl2->SetFont(m_font);
        ctrl2->Refresh();
    }
#else
    wxLogMessage(_T("Font selection dialog not available in current build."));
#endif
}

void WidgetsFrame::OnEnable(wxCommandEvent& event)
{
    WidgetsPage *page = wxStaticCast(m_book->GetCurrentPage(), WidgetsPage);
    page->GetWidget()->Enable(event.IsChecked());
}

#endif // wxUSE_MENUS

// ----------------------------------------------------------------------------
// WidgetsPageInfo
// ----------------------------------------------------------------------------

WidgetsPageInfo *WidgetsPage::ms_widgetPages = NULL;

WidgetsPageInfo::WidgetsPageInfo(Constructor ctor, const wxChar *label)
               : m_label(label)
{
    m_ctor = ctor;

    m_next = NULL;

    // dummy sorting: add and immediately sort on list according to label

    if(WidgetsPage::ms_widgetPages)
    {
        WidgetsPageInfo *node_prev = WidgetsPage::ms_widgetPages;
        if(wxStrcmp(label,node_prev->GetLabel().c_str())<0)
        {
            // add as first
            m_next = node_prev;
            WidgetsPage::ms_widgetPages = this;
        }
        else
        {
            WidgetsPageInfo *node_next;
            do
            {
                node_next = node_prev->GetNext();
                if(node_next)
                {
                    // add if between two
                    if(wxStrcmp(label,node_next->GetLabel().c_str())<0)
                    {
                        node_prev->SetNext(this);
                        m_next = node_next;
                        // force to break loop
                        node_next = NULL;
                    }
                }
                else
                {
                    // add as last
                    node_prev->SetNext(this);
                    m_next = node_next;
                }
                node_prev = node_next;
            }while(node_next);
        }
    }
    else
    {
        // add when first

        WidgetsPage::ms_widgetPages = this;

    }

}

// ----------------------------------------------------------------------------
// WidgetsPage
// ----------------------------------------------------------------------------

WidgetsPage::WidgetsPage(wxBookCtrl *book)
           : wxPanel(book, wxID_ANY,
                     wxDefaultPosition, wxDefaultSize,
                     wxNO_FULL_REPAINT_ON_RESIZE |
                     wxCLIP_CHILDREN |
                     wxTAB_TRAVERSAL)
{
}

wxSizer *WidgetsPage::CreateSizerWithText(wxControl *control,
                                          wxWindowID id,
                                          wxTextCtrl **ppText)
{
    wxSizer *sizerRow = new wxBoxSizer(wxHORIZONTAL);
    wxTextCtrl *text = new wxTextCtrl(this, id, wxEmptyString,
        wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

    sizerRow->Add(control, 0, wxRIGHT | wxALIGN_CENTRE_VERTICAL, 5);
    sizerRow->Add(text, 1, wxLEFT | wxALIGN_CENTRE_VERTICAL, 5);

    if ( ppText )
        *ppText = text;

    return sizerRow;
}

// create a sizer containing a label and a text ctrl
wxSizer *WidgetsPage::CreateSizerWithTextAndLabel(const wxString& label,
                                                  wxWindowID id,
                                                  wxTextCtrl **ppText)
{
    return CreateSizerWithText(new wxStaticText(this, wxID_ANY, label),
        id, ppText);
}

// create a sizer containing a button and a text ctrl
wxSizer *WidgetsPage::CreateSizerWithTextAndButton(wxWindowID idBtn,
                                                   const wxString& label,
                                                   wxWindowID id,
                                                   wxTextCtrl **ppText)
{
    return CreateSizerWithText(new wxButton(this, idBtn, label), id, ppText);
}

wxCheckBox *WidgetsPage::CreateCheckBoxAndAddToSizer(wxSizer *sizer,
                                                     const wxString& label,
                                                     wxWindowID id)
{
    wxCheckBox *checkbox = new wxCheckBox(this, id, label);
    sizer->Add(checkbox, 0, wxLEFT | wxRIGHT, 5);
    sizer->Add(0, 2, 0, wxGROW); // spacer

    return checkbox;
}

