/////////////////////////////////////////////////////////////////////////////
// Name:        prntbase.cpp
// Purpose:     Printing framework base class implementation
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: prntbase.cpp,v 1.88 2005/04/01 16:31:44 SC Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
  #pragma implementation "prntbase.h"
  #pragma implementation "printdlg.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wx/defs.h"

#if wxUSE_PRINTING_ARCHITECTURE

#ifndef WX_PRECOMP
#include "wx/utils.h"
#include "wx/dc.h"
#include "wx/app.h"
#include "wx/msgdlg.h"
#include "wx/layout.h"
#include "wx/choice.h"
#include "wx/button.h"
#include "wx/settings.h"
#include "wx/dcmemory.h"
#include "wx/stattext.h"
#include "wx/intl.h"
#include "wx/textdlg.h"
#include "wx/sizer.h"
#endif // !WX_PRECOMP

#include "wx/prntbase.h"
#include "wx/dcprint.h"
#include "wx/printdlg.h"
#include "wx/print.h"
#include "wx/module.h"

#include <stdlib.h>
#include <string.h>

#if defined(__WXMSW__) && !defined(__WXUNIVERSAL__)
#include "wx/msw/printdlg.h"
#elif defined(__WXMAC__)
#include "wx/mac/printdlg.h"
#include "wx/mac/private/print.h"
#else
#include "wx/generic/prntdlgg.h"
#endif

#ifdef __WXMSW__
    #include "wx/msw/wrapcdlg.h"
    #ifndef __WIN32__
        #include <print.h>
    #endif
#endif // __WXMSW__

//----------------------------------------------------------------------------
// wxPrintFactory
//----------------------------------------------------------------------------

wxPrintFactory *wxPrintFactory::m_factory = NULL;

void wxPrintFactory::SetPrintFactory( wxPrintFactory *factory )
{
    if (wxPrintFactory::m_factory)
        delete wxPrintFactory::m_factory;

    wxPrintFactory::m_factory = factory;
}

wxPrintFactory *wxPrintFactory::GetFactory()
{
    if (!wxPrintFactory::m_factory)
        wxPrintFactory::m_factory = new wxNativePrintFactory;

    return wxPrintFactory::m_factory;
}

//----------------------------------------------------------------------------
// wxNativePrintFactory
//----------------------------------------------------------------------------

wxPrinterBase *wxNativePrintFactory::CreatePrinter( wxPrintDialogData *data )
{
#if defined(__WXMSW__) && !defined(__WXUNIVERSAL__)
    return new wxWindowsPrinter( data );
#elif defined(__WXMAC__)
    return new wxMacPrinter( data );
#elif defined(__WXPM__)
    return new wxOS2Printer( data );
#else
    return new wxPostScriptPrinter( data );
#endif
};

wxPrintPreviewBase *wxNativePrintFactory::CreatePrintPreview( wxPrintout *preview,
    wxPrintout *printout, wxPrintDialogData *data )
{
#if defined(__WXMSW__) && !defined(__WXUNIVERSAL__)
    return new wxWindowsPrintPreview( preview, printout, data );
#elif defined(__WXMAC__)
    return new wxMacPrintPreview( preview, printout, data );
#elif defined(__WXPM__)
    return new wxOS2PrintPreview( preview, printout, data );
#else
    return new wxPostScriptPrintPreview( preview, printout, data );
#endif
}

wxPrintPreviewBase *wxNativePrintFactory::CreatePrintPreview( wxPrintout *preview,
    wxPrintout *printout, wxPrintData *data )
{
#if defined(__WXMSW__) && !defined(__WXUNIVERSAL__)
    return new wxWindowsPrintPreview( preview, printout, data );
#elif defined(__WXMAC__)
    return new wxMacPrintPreview( preview, printout, data );
#elif defined(__WXPM__)
    return new wxOS2PrintPreview( preview, printout, data );
#else
    return new wxPostScriptPrintPreview( preview, printout, data );
#endif
}

wxPrintDialogBase *wxNativePrintFactory::CreatePrintDialog( wxWindow *parent,
                                                  wxPrintDialogData *data )
{
#if defined(__WXMSW__) && !defined(__WXUNIVERSAL__)
    return new wxWindowsPrintDialog( parent, data );
#elif defined(__WXMAC__)
    return new wxMacPrintDialog( parent, data );
#else
    return new wxGenericPrintDialog( parent, data );
#endif
}

wxPrintDialogBase *wxNativePrintFactory::CreatePrintDialog( wxWindow *parent,
                                                  wxPrintData *data )
{
#if defined(__WXMSW__) && !defined(__WXUNIVERSAL__)
    return new wxWindowsPrintDialog( parent, data );
#elif defined(__WXMAC__)
    return new wxMacPrintDialog( parent, data );
#else
    return new wxGenericPrintDialog( parent, data );
#endif
}

wxPageSetupDialogBase *wxNativePrintFactory::CreatePageSetupDialog( wxWindow *parent,
                                                  wxPageSetupDialogData *data )
{
#if defined(__WXMSW__) && !defined(__WXUNIVERSAL__)
    return new wxWindowsPageSetupDialog( parent, data );
#elif defined(__WXMAC__)
    return new wxMacPageSetupDialog( parent, data );
#else
    return new wxGenericPageSetupDialog( parent, data );
#endif
}

bool wxNativePrintFactory::HasPrintSetupDialog()
{
#if defined(__WXMSW__) && !defined(__WXUNIVERSAL__)
    return false;
#elif defined(__WXMAC__)
    return false;
#else
    // Only here do we need to provide the print setup
    // dialog ourselves, the other platforms either have
    // none, don't make it accessible or let you configure
    // the printer from the wxPrintDialog anyway.
    return true;
#endif

}

wxDialog *wxNativePrintFactory::CreatePrintSetupDialog( wxWindow *parent,
                                                        wxPrintData *data )
{
#if defined(__WXMSW__) && !defined(__WXUNIVERSAL__)
    wxUnusedVar(parent);
    wxUnusedVar(data);
    return NULL;
#elif defined(__WXMAC__)
    wxUnusedVar(parent);
    wxUnusedVar(data);
    return NULL;
#else
    // Only here do we need to provide the print setup
    // dialog ourselves, the other platforms either have
    // none, don't make it accessible or let you configure
    // the printer from the wxPrintDialog anyway.
    return new wxGenericPrintSetupDialog( parent, data );
#endif
}

bool wxNativePrintFactory::HasOwnPrintToFile()
{
    // Only relevant for PostScript and here the
    // setup dialog provides no "print to file"
    // option. In the GNOME setup dialog, the
    // setup dialog has its own print to file.
    return false;
}

bool wxNativePrintFactory::HasPrinterLine()
{
    // Only relevant for PostScript for now
    return true;
}

wxString wxNativePrintFactory::CreatePrinterLine()
{
    // Only relevant for PostScript for now

    // We should query "lpstat -d" here
    return _("Generic PostScript");
}

bool wxNativePrintFactory::HasStatusLine()
{
    // Only relevant for PostScript for now
    return true;
}

wxString wxNativePrintFactory::CreateStatusLine()
{
    // Only relevant for PostScript for now

    // We should query "lpstat -r" or "lpstat -p" here
    return _("Ready");
}

wxPrintNativeDataBase *wxNativePrintFactory::CreatePrintNativeData()
{
#if defined(__WXMSW__) && !defined(__WXUNIVERSAL__)
    return new wxWindowsPrintNativeData;
#elif defined(__WXMAC__)
    return new wxMacCarbonPrintData;
#else
    return new wxPostScriptPrintNativeData;
#endif
}

//----------------------------------------------------------------------------
// wxPrintNativeDataBase
//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxPrintNativeDataBase, wxObject)

wxPrintNativeDataBase::wxPrintNativeDataBase()
{
    m_ref = 1;
}

//----------------------------------------------------------------------------
// wxPrintFactoryModule
//----------------------------------------------------------------------------

class wxPrintFactoryModule: public wxModule
{
public:
    wxPrintFactoryModule() {}
    bool OnInit() { return true; }
    void OnExit() { wxPrintFactory::SetPrintFactory( NULL ); }

private:
    DECLARE_DYNAMIC_CLASS(wxPrintFactoryModule)
};

IMPLEMENT_DYNAMIC_CLASS(wxPrintFactoryModule, wxModule)

//----------------------------------------------------------------------------
// wxPrinterBase
//----------------------------------------------------------------------------

IMPLEMENT_CLASS(wxPrinterBase, wxObject)

wxPrinterBase::wxPrinterBase(wxPrintDialogData *data)
{
    m_currentPrintout = (wxPrintout *) NULL;
    sm_abortWindow = (wxWindow *) NULL;
    sm_abortIt = false;
    if (data)
        m_printDialogData = (*data);
    sm_lastError = wxPRINTER_NO_ERROR;
}

wxWindow *wxPrinterBase::sm_abortWindow = (wxWindow *) NULL;
bool wxPrinterBase::sm_abortIt = false;
wxPrinterError wxPrinterBase::sm_lastError = wxPRINTER_NO_ERROR;

wxPrinterBase::~wxPrinterBase()
{
}

wxWindow *wxPrinterBase::CreateAbortWindow(wxWindow *parent, wxPrintout * printout)
{
    wxPrintAbortDialog *dialog = new wxPrintAbortDialog(parent, _("Printing ") , wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);

    wxBoxSizer *button_sizer = new wxBoxSizer( wxVERTICAL );
    button_sizer->Add( new wxStaticText(dialog, wxID_ANY, _("Please wait while printing\n") + printout->GetTitle() ), 0, wxALL, 10 );
    button_sizer->Add( new wxButton( dialog, wxID_CANCEL, wxT("Cancel") ), 0, wxALL | wxALIGN_CENTER, 10 );

    dialog->SetAutoLayout( true );
    dialog->SetSizer( button_sizer );

    button_sizer->Fit(dialog);
    button_sizer->SetSizeHints (dialog) ;

    return dialog;
}

void wxPrinterBase::ReportError(wxWindow *parent, wxPrintout *WXUNUSED(printout), const wxString& message)
{
    wxMessageBox(message, _("Printing Error"), wxOK, parent);
}

wxPrintDialogData& wxPrinterBase::GetPrintDialogData() const
{
    return (wxPrintDialogData&) m_printDialogData;
}

//----------------------------------------------------------------------------
// wxPrinter
//----------------------------------------------------------------------------

IMPLEMENT_CLASS(wxPrinter, wxPrinterBase)

wxPrinter::wxPrinter(wxPrintDialogData *data)
{
    m_pimpl = wxPrintFactory::GetFactory()->CreatePrinter( data );
}

wxPrinter::~wxPrinter()
{
    delete m_pimpl;
}

wxWindow *wxPrinter::CreateAbortWindow(wxWindow *parent, wxPrintout *printout)
{
    return m_pimpl->CreateAbortWindow( parent, printout );
}

void wxPrinter::ReportError(wxWindow *parent, wxPrintout *printout, const wxString& message)
{
    m_pimpl->ReportError( parent, printout, message );
}

bool wxPrinter::Setup(wxWindow *parent)
{
    return m_pimpl->Setup( parent );
}

bool wxPrinter::Print(wxWindow *parent, wxPrintout *printout, bool prompt)
{
    return m_pimpl->Print( parent, printout, prompt );
}

wxDC* wxPrinter::PrintDialog(wxWindow *parent)
{
    return m_pimpl->PrintDialog( parent );
}

wxPrintDialogData& wxPrinter::GetPrintDialogData() const
{
    return m_pimpl->GetPrintDialogData();
}

// ---------------------------------------------------------------------------
// wxPrintDialogBase: the dialog for printing.
// ---------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxPrintDialogBase, wxDialog)

wxPrintDialogBase::wxPrintDialogBase(wxWindow *parent,
                                     wxWindowID id,
                                     const wxString &title,
                                     const wxPoint &pos,
                                     const wxSize &size,
                                     long style)
    : wxDialog( parent, id, title.empty() ? wxString(_("Print")) : title,
                pos, size, style )
{
}

// ---------------------------------------------------------------------------
// wxPrintDialog: the dialog for printing
// ---------------------------------------------------------------------------

IMPLEMENT_CLASS(wxPrintDialog, wxObject)

wxPrintDialog::wxPrintDialog(wxWindow *parent, wxPrintDialogData* data)
{
    m_pimpl = wxPrintFactory::GetFactory()->CreatePrintDialog( parent, data );
}

wxPrintDialog::wxPrintDialog(wxWindow *parent, wxPrintData* data)
{
    m_pimpl = wxPrintFactory::GetFactory()->CreatePrintDialog( parent, data );
}

wxPrintDialog::~wxPrintDialog()
{
    delete m_pimpl;
}

int wxPrintDialog::ShowModal()
{
    return m_pimpl->ShowModal();
}

wxPrintDialogData& wxPrintDialog::GetPrintDialogData()
{
    return m_pimpl->GetPrintDialogData();
}

wxPrintData& wxPrintDialog::GetPrintData()
{
    return m_pimpl->GetPrintData();
}

wxDC *wxPrintDialog::GetPrintDC()
{
    return m_pimpl->GetPrintDC();
}

// ---------------------------------------------------------------------------
// wxPageSetupDialogBase: the page setup dialog
// ---------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxPageSetupDialogBase, wxDialog)

wxPageSetupDialogBase::wxPageSetupDialogBase(wxWindow *parent,
                                     wxWindowID id,
                                     const wxString &title,
                                     const wxPoint &pos,
                                     const wxSize &size,
                                     long style)
    : wxDialog( parent, id, title.empty() ? wxString(_("Page setup")) : title,
                pos, size, style )
{
}

// ---------------------------------------------------------------------------
// wxPageSetupDialog: the page setup dialog
// ---------------------------------------------------------------------------

IMPLEMENT_CLASS(wxPageSetupDialog, wxObject)

wxPageSetupDialog::wxPageSetupDialog(wxWindow *parent, wxPageSetupDialogData *data )
{
    m_pimpl = wxPrintFactory::GetFactory()->CreatePageSetupDialog( parent, data );
}

wxPageSetupDialog::~wxPageSetupDialog()
{
    delete m_pimpl;
}

int wxPageSetupDialog::ShowModal()
{
    return m_pimpl->ShowModal();
}

wxPageSetupDialogData& wxPageSetupDialog::GetPageSetupDialogData()
{
    return m_pimpl->GetPageSetupDialogData();
}

// old name
wxPageSetupDialogData& wxPageSetupDialog::GetPageSetupData()
{
    return m_pimpl->GetPageSetupDialogData();
}

//----------------------------------------------------------------------------
// wxPrintAbortDialog
//----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxPrintAbortDialog, wxDialog)
    EVT_BUTTON(wxID_CANCEL, wxPrintAbortDialog::OnCancel)
END_EVENT_TABLE()

void wxPrintAbortDialog::OnCancel(wxCommandEvent& WXUNUSED(event))
{
    wxPrinterBase::sm_abortIt = true;
    wxPrinterBase::sm_abortWindow->Show(false);
    wxPrinterBase::sm_abortWindow->Close(true);
    wxPrinterBase::sm_abortWindow = (wxWindow *) NULL;
}

//----------------------------------------------------------------------------
// wxPrintout
//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxPrintout, wxObject)

wxPrintout::wxPrintout(const wxString& title)
{
    m_printoutTitle = title ;
    m_printoutDC = (wxDC *) NULL;
    m_pageWidthMM = 0;
    m_pageHeightMM = 0;
    m_pageWidthPixels = 0;
    m_pageHeightPixels = 0;
    m_PPIScreenX = 0;
    m_PPIScreenY = 0;
    m_PPIPrinterX = 0;
    m_PPIPrinterY = 0;
    m_isPreview = false;
}

wxPrintout::~wxPrintout()
{
}

bool wxPrintout::OnBeginDocument(int WXUNUSED(startPage), int WXUNUSED(endPage))
{
    return GetDC()->StartDoc(_("Printing ") + m_printoutTitle);
}

void wxPrintout::OnEndDocument()
{
    GetDC()->EndDoc();
}

void wxPrintout::OnBeginPrinting()
{
}

void wxPrintout::OnEndPrinting()
{
}

bool wxPrintout::HasPage(int page)
{
    return (page == 1);
}

void wxPrintout::GetPageInfo(int *minPage, int *maxPage, int *fromPage, int *toPage)
{
    *minPage = 1;
    *maxPage = 32000;
    *fromPage = 1;
    *toPage = 1;
}

//----------------------------------------------------------------------------
// wxPreviewCanvas
//----------------------------------------------------------------------------

IMPLEMENT_CLASS(wxPreviewCanvas, wxWindow)

BEGIN_EVENT_TABLE(wxPreviewCanvas, wxScrolledWindow)
    EVT_PAINT(wxPreviewCanvas::OnPaint)
    EVT_CHAR(wxPreviewCanvas::OnChar)
    EVT_SYS_COLOUR_CHANGED(wxPreviewCanvas::OnSysColourChanged)
END_EVENT_TABLE()

// VZ: the current code doesn't refresh properly without
//     wxFULL_REPAINT_ON_RESIZE, this must be fixed as otherwise we have
//     really horrible flicker when resizing the preview frame, but without
//     this style it simply doesn't work correctly at all...
wxPreviewCanvas::wxPreviewCanvas(wxPrintPreviewBase *preview, wxWindow *parent,
                                 const wxPoint& pos, const wxSize& size, long style, const wxString& name):
wxScrolledWindow(parent, wxID_ANY, pos, size, style | wxFULL_REPAINT_ON_RESIZE, name)
{
    m_printPreview = preview;
#ifdef __WXMAC__
    // The app workspace colour is always white, but we should have
    // a contrast with the page.
    wxSystemColour colourIndex = wxSYS_COLOUR_3DDKSHADOW;
#else
    wxSystemColour colourIndex = wxSYS_COLOUR_APPWORKSPACE;
#endif
    SetBackgroundColour(wxSystemSettings::GetColour(colourIndex));

    SetScrollbars(10, 10, 100, 100);
}

wxPreviewCanvas::~wxPreviewCanvas()
{
}

void wxPreviewCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    PrepareDC( dc );

/*
#ifdef __WXGTK__
    if (!GetUpdateRegion().IsEmpty())
        dc.SetClippingRegion( GetUpdateRegion() );
#endif
*/

    if (m_printPreview)
    {
        m_printPreview->PaintPage(this, dc);
    }
}

// Responds to colour changes, and passes event on to children.
void wxPreviewCanvas::OnSysColourChanged(wxSysColourChangedEvent& event)
{
#ifdef __WXMAC__
    // The app workspace colour is always white, but we should have
    // a contrast with the page.
    wxSystemColour colourIndex = wxSYS_COLOUR_3DDKSHADOW;
#else
    wxSystemColour colourIndex = wxSYS_COLOUR_APPWORKSPACE;
#endif
    SetBackgroundColour(wxSystemSettings::GetColour(colourIndex));
    Refresh();

    // Propagate the event to the non-top-level children
    wxWindow::OnSysColourChanged(event);
}

void wxPreviewCanvas::OnChar(wxKeyEvent &event)
{
    wxPreviewControlBar* controlBar = ((wxPreviewFrame*) GetParent())->GetControlBar();
    if (event.GetKeyCode() == WXK_ESCAPE)
    {
        ((wxPreviewFrame*) GetParent())->Close(true);
        return;
    }
    else if (event.GetKeyCode() == WXK_TAB)
    {
        controlBar->OnGoto();
        return;
    }
    else if (event.GetKeyCode() == WXK_RETURN)
    {
        controlBar->OnPrint();
        return;
    }

    if (!event.ControlDown())
    {
        event.Skip();
        return;
    }

    switch(event.GetKeyCode())
    {
        case WXK_NEXT:
            controlBar->OnNext(); break;
        case WXK_PRIOR:
            controlBar->OnPrevious(); break;
        case WXK_HOME:
            controlBar->OnFirst(); break;
        case WXK_END:
            controlBar->OnLast(); break;
        default:
            event.Skip();
    }
}

//----------------------------------------------------------------------------
// wxPreviewControlBar
//----------------------------------------------------------------------------

IMPLEMENT_CLASS(wxPreviewControlBar, wxWindow)

BEGIN_EVENT_TABLE(wxPreviewControlBar, wxPanel)
    EVT_BUTTON(wxID_PREVIEW_CLOSE,    wxPreviewControlBar::OnWindowClose)
    EVT_BUTTON(wxID_PREVIEW_PRINT,    wxPreviewControlBar::OnPrintButton)
    EVT_BUTTON(wxID_PREVIEW_PREVIOUS, wxPreviewControlBar::OnPreviousButton)
    EVT_BUTTON(wxID_PREVIEW_NEXT,     wxPreviewControlBar::OnNextButton)
    EVT_BUTTON(wxID_PREVIEW_FIRST,    wxPreviewControlBar::OnFirstButton)
    EVT_BUTTON(wxID_PREVIEW_LAST,     wxPreviewControlBar::OnLastButton)
    EVT_BUTTON(wxID_PREVIEW_GOTO,     wxPreviewControlBar::OnGotoButton)
    EVT_CHOICE(wxID_PREVIEW_ZOOM,     wxPreviewControlBar::OnZoom)
    EVT_PAINT(wxPreviewControlBar::OnPaint)
END_EVENT_TABLE()

wxPreviewControlBar::wxPreviewControlBar(wxPrintPreviewBase *preview, long buttons,
                                         wxWindow *parent, const wxPoint& pos, const wxSize& size,
                                         long style, const wxString& name):
wxPanel(parent, wxID_ANY, pos, size, style, name)
{
    m_printPreview = preview;
    m_closeButton = (wxButton *) NULL;
    m_nextPageButton = (wxButton *) NULL;
    m_previousPageButton = (wxButton *) NULL;
    m_printButton = (wxButton *) NULL;
    m_zoomControl = (wxChoice *) NULL;
    m_buttonFlags = buttons;
}

wxPreviewControlBar::~wxPreviewControlBar()
{
}

void wxPreviewControlBar::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);

    int w, h;
    GetSize(&w, &h);
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawLine( 0, h-1, w, h-1 );
}

void wxPreviewControlBar::OnWindowClose(wxCommandEvent& WXUNUSED(event))
{
    wxPreviewFrame *frame = (wxPreviewFrame *)GetParent();
    frame->Close(true);
}

void wxPreviewControlBar::OnPrint(void)
{
    wxPrintPreviewBase *preview = GetPrintPreview();
    preview->Print(true);
}

void wxPreviewControlBar::OnNext(void)
{
    wxPrintPreviewBase *preview = GetPrintPreview();
    if (preview)
    {
        int currentPage = preview->GetCurrentPage();
        if ((preview->GetMaxPage() > 0) &&
            (currentPage < preview->GetMaxPage()) &&
            preview->GetPrintout()->HasPage(currentPage + 1))
        {
            preview->SetCurrentPage(currentPage + 1);
        }
    }
}

void wxPreviewControlBar::OnPrevious(void)
{
    wxPrintPreviewBase *preview = GetPrintPreview();
    if (preview)
    {
        int currentPage = preview->GetCurrentPage();
        if ((preview->GetMinPage() > 0) &&
            (currentPage > preview->GetMinPage()) &&
            preview->GetPrintout()->HasPage(currentPage - 1))
        {
            preview->SetCurrentPage(currentPage - 1);
        }
    }
}

void wxPreviewControlBar::OnFirst(void)
{
    wxPrintPreviewBase *preview = GetPrintPreview();
    if (preview)
    {
        int currentPage = preview->GetMinPage();
        if (preview->GetPrintout()->HasPage(currentPage))
        {
            preview->SetCurrentPage(currentPage);
        }
    }
}

void wxPreviewControlBar::OnLast(void)
{
    wxPrintPreviewBase *preview = GetPrintPreview();
    if (preview)
    {
        int currentPage = preview->GetMaxPage();
        if (preview->GetPrintout()->HasPage(currentPage))
        {
            preview->SetCurrentPage(currentPage);
        }
    }
}

void wxPreviewControlBar::OnGoto(void)
{
    wxPrintPreviewBase *preview = GetPrintPreview();
    if (preview)
    {
        long currentPage;

        if (preview->GetMinPage() > 0)
        {
            wxString strPrompt;
            wxString strPage;

            strPrompt.Printf( _("Enter a page number between %d and %d:"),
                preview->GetMinPage(), preview->GetMaxPage());
            strPage.Printf( wxT("%d"), preview->GetCurrentPage() );

            strPage =
                wxGetTextFromUser( strPrompt, _("Goto Page"), strPage, GetParent());

            if ( strPage.ToLong( &currentPage ) )
                if (preview->GetPrintout()->HasPage(currentPage))
                {
                    preview->SetCurrentPage(currentPage);
                }
        }
    }
}

void wxPreviewControlBar::OnZoom(wxCommandEvent& WXUNUSED(event))
{
    int zoom = GetZoomControl();
    if (GetPrintPreview())
        GetPrintPreview()->SetZoom(zoom);
}

void wxPreviewControlBar::CreateButtons()
{
    SetSize(0, 0, 400, 40);

    wxBoxSizer *item0 = new wxBoxSizer( wxHORIZONTAL );

    m_closeButton = new wxButton( this, wxID_PREVIEW_CLOSE, _("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    item0->Add( m_closeButton, 0, wxALIGN_CENTRE|wxALL, 5 );

    if (m_buttonFlags & wxPREVIEW_PRINT)
    {
        m_printButton = new wxButton( this, wxID_PREVIEW_PRINT, _("&Print..."), wxDefaultPosition, wxDefaultSize, 0 );
        item0->Add( m_printButton, 0, wxALIGN_CENTRE|wxALL, 5 );
    }

    if (m_buttonFlags & wxPREVIEW_FIRST)
    {
        m_firstPageButton = new wxButton( this, wxID_PREVIEW_FIRST, _("|<<"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
        item0->Add( m_firstPageButton, 0, wxALIGN_CENTRE|wxALL, 5 );
    }

    if (m_buttonFlags & wxPREVIEW_PREVIOUS)
    {
        m_previousPageButton = new wxButton( this, wxID_PREVIEW_PREVIOUS, _("<<"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
        item0->Add( m_previousPageButton, 0, wxALIGN_CENTRE|wxRIGHT|wxTOP|wxBOTTOM, 5 );
    }

    if (m_buttonFlags & wxPREVIEW_NEXT)
    {
        m_nextPageButton = new wxButton( this, wxID_PREVIEW_NEXT, _(">>"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
        item0->Add( m_nextPageButton, 0, wxALIGN_CENTRE|wxRIGHT|wxTOP|wxBOTTOM, 5 );
    }

    if (m_buttonFlags & wxPREVIEW_LAST)
    {
        m_lastPageButton = new wxButton( this, wxID_PREVIEW_LAST, _(">>|"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
        item0->Add( m_lastPageButton, 0, wxALIGN_CENTRE|wxRIGHT|wxTOP|wxBOTTOM, 5 );
    }

    if (m_buttonFlags & wxPREVIEW_GOTO)
    {
        m_gotoPageButton = new wxButton( this, wxID_PREVIEW_GOTO, _("&Goto..."), wxDefaultPosition, wxDefaultSize, 0 );
        item0->Add( m_gotoPageButton, 0, wxALIGN_CENTRE|wxALL, 5 );
    }

    if (m_buttonFlags & wxPREVIEW_ZOOM)
    {
        wxString choices[] =
        {
            wxT("10%"), wxT("15%"), wxT("20%"), wxT("25%"), wxT("30%"), wxT("35%"), wxT("40%"), wxT("45%"), wxT("50%"), wxT("55%"),
                wxT("60%"), wxT("65%"), wxT("70%"), wxT("75%"), wxT("80%"), wxT("85%"), wxT("90%"), wxT("95%"), wxT("100%"), wxT("110%"),
                wxT("120%"), wxT("150%"), wxT("200%")
        };
        int n = WXSIZEOF(choices);

        m_zoomControl = new wxChoice( this, wxID_PREVIEW_ZOOM, wxDefaultPosition, wxSize(70,wxDefaultCoord), n, choices, 0 );
        item0->Add( m_zoomControl, 0, wxALIGN_CENTRE|wxALL, 5 );
        SetZoomControl(m_printPreview->GetZoom());
    }

    SetSizer(item0);
    item0->Fit(this);
}

void wxPreviewControlBar::SetZoomControl(int zoom)
{
    if (m_zoomControl)
    {
        int n, count = m_zoomControl->GetCount();
        long val;
        for (n=0; n<count; n++)
        {
            if (m_zoomControl->GetString(n).BeforeFirst(wxT('%')).ToLong(&val) &&
                (val >= long(zoom)))
            {
                m_zoomControl->SetSelection(n);
                return;
            }
        }

        m_zoomControl->SetSelection(count-1);
    }
}

int wxPreviewControlBar::GetZoomControl()
{
    if (m_zoomControl && (m_zoomControl->GetStringSelection() != wxEmptyString))
    {
        long val;
        if (m_zoomControl->GetStringSelection().BeforeFirst(wxT('%')).ToLong(&val))
            return int(val);
    }

    return 0;
}


/*
* Preview frame
*/

IMPLEMENT_CLASS(wxPreviewFrame, wxFrame)

BEGIN_EVENT_TABLE(wxPreviewFrame, wxFrame)
    EVT_CLOSE(wxPreviewFrame::OnCloseWindow)
END_EVENT_TABLE()

wxPreviewFrame::wxPreviewFrame(wxPrintPreviewBase *preview, wxWindow *parent, const wxString& title,
                               const wxPoint& pos, const wxSize& size, long style, const wxString& name):
wxFrame(parent, wxID_ANY, title, pos, size, style, name)
{
    m_printPreview = preview;
    m_controlBar = NULL;
    m_previewCanvas = NULL;
    m_windowDisabler = NULL;

    // Give the application icon
#ifdef __WXMSW__
    wxFrame* topFrame = wxDynamicCast(wxTheApp->GetTopWindow(), wxFrame);
    if (topFrame)
        SetIcon(topFrame->GetIcon());
#endif
}

wxPreviewFrame::~wxPreviewFrame()
{
}

void wxPreviewFrame::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
    if (m_windowDisabler)
        delete m_windowDisabler;

    // Need to delete the printout and the print preview
    wxPrintout *printout = m_printPreview->GetPrintout();
    if (printout)
    {
        delete printout;
        m_printPreview->SetPrintout(NULL);
        m_printPreview->SetCanvas(NULL);
        m_printPreview->SetFrame(NULL);
    }
    delete m_printPreview;

    Destroy();
}

void wxPreviewFrame::Initialize()
{
#if wxUSE_STATUSBAR
    CreateStatusBar();
#endif
    CreateCanvas();
    CreateControlBar();

    m_printPreview->SetCanvas(m_previewCanvas);
    m_printPreview->SetFrame(this);

    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    item0->Add( m_controlBar, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5 );
    item0->Add( m_previewCanvas, 1, wxGROW|wxALIGN_CENTER_VERTICAL, 5 );

    SetAutoLayout( true );
    SetSizer( item0 );

    m_windowDisabler = new wxWindowDisabler(this);

    Layout();

    m_printPreview->AdjustScrollbars(m_previewCanvas);
    m_previewCanvas->SetFocus();
    m_controlBar->SetFocus();
}

void wxPreviewFrame::CreateCanvas()
{
    m_previewCanvas = new wxPreviewCanvas(m_printPreview, this);
}

void wxPreviewFrame::CreateControlBar()
{
    long buttons = wxPREVIEW_DEFAULT;
    if (m_printPreview->GetPrintoutForPrinting())
        buttons |= wxPREVIEW_PRINT;

    m_controlBar = new wxPreviewControlBar(m_printPreview, buttons, this, wxPoint(0,0), wxSize(400, 40));
    m_controlBar->CreateButtons();
}

/*
* Print preview
*/

IMPLEMENT_CLASS(wxPrintPreviewBase, wxObject)

wxPrintPreviewBase::wxPrintPreviewBase(wxPrintout *printout,
                                       wxPrintout *printoutForPrinting,
                                       wxPrintData *data)
{
    if (data)
        m_printDialogData = (*data);

    Init(printout, printoutForPrinting);
}

wxPrintPreviewBase::wxPrintPreviewBase(wxPrintout *printout,
                                       wxPrintout *printoutForPrinting,
                                       wxPrintDialogData *data)
{
    if (data)
        m_printDialogData = (*data);

    Init(printout, printoutForPrinting);
}

void wxPrintPreviewBase::Init(wxPrintout *printout,
                              wxPrintout *printoutForPrinting)
{
    m_isOk = true;
    m_previewPrintout = printout;
    if (m_previewPrintout)
        m_previewPrintout->SetIsPreview(true);

    m_printPrintout = printoutForPrinting;

    m_previewCanvas = NULL;
    m_previewFrame = NULL;
    m_previewBitmap = NULL;
    m_currentPage = 1;
    m_currentZoom = 70;
    m_topMargin = 40;
    m_leftMargin = 40;
    m_pageWidth = 0;
    m_pageHeight = 0;
    m_printingPrepared = false;
    m_minPage = 1;
    m_maxPage = 1;
}

wxPrintPreviewBase::~wxPrintPreviewBase()
{
    if (m_previewPrintout)
        delete m_previewPrintout;
    if (m_previewBitmap)
        delete m_previewBitmap;
    if (m_printPrintout)
        delete m_printPrintout;
}

bool wxPrintPreviewBase::SetCurrentPage(int pageNum)
{
    if (m_currentPage == pageNum)
        return true;

    m_currentPage = pageNum;
    if (m_previewBitmap)
    {
        delete m_previewBitmap;
        m_previewBitmap = NULL;
    }

    if (m_previewCanvas)
    {
        AdjustScrollbars(m_previewCanvas);

        if (!RenderPage(pageNum))
            return false;
        m_previewCanvas->Refresh();
        m_previewCanvas->SetFocus();
    }
    return true;
}

int wxPrintPreviewBase::GetCurrentPage() const
    { return m_currentPage; };
void wxPrintPreviewBase::SetPrintout(wxPrintout *printout)
    { m_previewPrintout = printout; };
wxPrintout *wxPrintPreviewBase::GetPrintout() const
    { return m_previewPrintout; };
wxPrintout *wxPrintPreviewBase::GetPrintoutForPrinting() const
    { return m_printPrintout; };
void wxPrintPreviewBase::SetFrame(wxFrame *frame)
    { m_previewFrame = frame; };
void wxPrintPreviewBase::SetCanvas(wxPreviewCanvas *canvas)
    { m_previewCanvas = canvas; };
wxFrame *wxPrintPreviewBase::GetFrame() const
    { return m_previewFrame; }
wxPreviewCanvas *wxPrintPreviewBase::GetCanvas() const
    { return m_previewCanvas; }

bool wxPrintPreviewBase::PaintPage(wxPreviewCanvas *canvas, wxDC& dc)
{
    DrawBlankPage(canvas, dc);

    if (!m_previewBitmap)
        if (!RenderPage(m_currentPage))
            return false;

    if (!m_previewBitmap)
        return false;

    if (!canvas)
        return false;

    int canvasWidth, canvasHeight;
    canvas->GetSize(&canvasWidth, &canvasHeight);

    double zoomScale = ((float)m_currentZoom/(float)100);
    double actualWidth = (zoomScale*m_pageWidth*m_previewScale);
    //  float actualHeight = (float)(zoomScale*m_pageHeight*m_previewScale);

    int x = (int) ((canvasWidth - actualWidth)/2.0);
    if (x < m_leftMargin)
        x = m_leftMargin;
    int y = m_topMargin;

    wxMemoryDC temp_dc;
    temp_dc.SelectObject(*m_previewBitmap);

    dc.Blit(x, y, m_previewBitmap->GetWidth(), m_previewBitmap->GetHeight(), &temp_dc, 0, 0);

    temp_dc.SelectObject(wxNullBitmap);

    return true;
}

// Adjusts the scrollbars for the current scale
void wxPrintPreviewBase::AdjustScrollbars(wxPreviewCanvas *canvas)
{
    if (!canvas)
        return ;

    int canvasWidth, canvasHeight;
    canvas->GetSize(&canvasWidth, &canvasHeight);

    double zoomScale = ((float)m_currentZoom/(float)100);
    double actualWidth = (zoomScale*m_pageWidth*m_previewScale);
    double actualHeight = (zoomScale*m_pageHeight*m_previewScale);

    // Set the scrollbars appropriately
    int totalWidth = (int)(actualWidth + 2*m_leftMargin);
    int totalHeight = (int)(actualHeight + 2*m_topMargin);
    int scrollUnitsX = totalWidth/10;
    int scrollUnitsY = totalHeight/10;
    wxSize virtualSize = canvas->GetVirtualSize();
    if (virtualSize.GetWidth() != totalWidth || virtualSize.GetHeight() != totalHeight)
        canvas->SetScrollbars(10, 10, scrollUnitsX, scrollUnitsY, 0, 0, true);
}

bool wxPrintPreviewBase::RenderPage(int pageNum)
{
    wxBusyCursor busy;

    int canvasWidth, canvasHeight;

    if (!m_previewCanvas)
    {
        wxFAIL_MSG(_T("wxPrintPreviewBase::RenderPage: must use wxPrintPreviewBase::SetCanvas to let me know about the canvas!"));

        return false;
    }
    m_previewCanvas->GetSize(&canvasWidth, &canvasHeight);

    double zoomScale = (m_currentZoom/100.0);
    int actualWidth = (int)(zoomScale*m_pageWidth*m_previewScale);
    int actualHeight = (int)(zoomScale*m_pageHeight*m_previewScale);

    if (!m_previewBitmap)
    {
        m_previewBitmap = new wxBitmap((int)actualWidth, (int)actualHeight);
        if (!m_previewBitmap || !m_previewBitmap->Ok())
        {
            if (m_previewBitmap) {
                delete m_previewBitmap;
                m_previewBitmap = NULL;
            }
            wxMessageBox(_("Sorry, not enough memory to create a preview."), _("Print Preview Failure"), wxOK);
            return false;
        }
    }

    wxMemoryDC memoryDC;
    memoryDC.SelectObject(*m_previewBitmap);

    memoryDC.Clear();

    m_previewPrintout->SetDC(&memoryDC);
    m_previewPrintout->SetPageSizePixels(m_pageWidth, m_pageHeight);

    // Need to delay OnPreparePrinting until here, so we have enough information.
    if (!m_printingPrepared)
    {
        m_previewPrintout->OnPreparePrinting();
        int selFrom, selTo;
        m_previewPrintout->GetPageInfo(&m_minPage, &m_maxPage, &selFrom, &selTo);
        m_printingPrepared = true;
    }

    m_previewPrintout->OnBeginPrinting();

    if (!m_previewPrintout->OnBeginDocument(m_printDialogData.GetFromPage(), m_printDialogData.GetToPage()))
    {
        wxMessageBox(_("Could not start document preview."), _("Print Preview Failure"), wxOK);

        memoryDC.SelectObject(wxNullBitmap);

        delete m_previewBitmap;
        m_previewBitmap = NULL;
        return false;
    }

    m_previewPrintout->OnPrintPage(pageNum);
    m_previewPrintout->OnEndDocument();
    m_previewPrintout->OnEndPrinting();

    m_previewPrintout->SetDC(NULL);

    memoryDC.SelectObject(wxNullBitmap);

#if wxUSE_STATUSBAR
    wxString status;
    if (m_maxPage != 0)
        status = wxString::Format(_("Page %d of %d"), pageNum, m_maxPage);
    else
        status = wxString::Format(_("Page %d"), pageNum);

    if (m_previewFrame)
        m_previewFrame->SetStatusText(status);
#endif

    return true;
}


bool wxPrintPreviewBase::DrawBlankPage(wxPreviewCanvas *canvas, wxDC& dc)
{
    int canvasWidth, canvasHeight;
    canvas->GetSize(&canvasWidth, &canvasHeight);

    float zoomScale = (float)((float)m_currentZoom/(float)100);
    float actualWidth = zoomScale*m_pageWidth*m_previewScale;
    float actualHeight = zoomScale*m_pageHeight*m_previewScale;

    float x = (float)((canvasWidth - actualWidth)/2.0);
    if (x < m_leftMargin)
        x = (float)m_leftMargin;
    float y = (float)m_topMargin;

    // Draw shadow, allowing for 1-pixel border AROUND the actual page
    int shadowOffset = 4;
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxBLACK_BRUSH);
    /*
    dc.DrawRectangle((int)(x-1 + shadowOffset), (int)(y-1 + shadowOffset), (int)(actualWidth+2), (int)(actualHeight+2));
    */
    dc.DrawRectangle((int)(x + shadowOffset), (int)(y + actualHeight+1), (int)(actualWidth), shadowOffset);
    dc.DrawRectangle((int)(x + actualWidth), (int)(y + shadowOffset), shadowOffset, (int)(actualHeight));

    // Draw blank page allowing for 1-pixel border AROUND the actual page
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxWHITE_BRUSH);

    /*
    wxRegion update_region = canvas->GetUpdateRegion();
    wxRect r = update_region.GetBox();

      printf( "x: %d y: %d w: %d h: %d.\n", (int)r.x, (int)r.y, (int)r.width, (int)r.height );
    */

    dc.DrawRectangle((int)(x-2), (int)(y-1), (int)(actualWidth+3), (int)(actualHeight+2));

    return true;
}

void wxPrintPreviewBase::SetZoom(int percent)
{
    if (m_currentZoom == percent)
        return;

    m_currentZoom = percent;
    if (m_previewBitmap)
    {
        delete m_previewBitmap;
        m_previewBitmap = NULL;
    }

    if (m_previewCanvas)
    {
        AdjustScrollbars(m_previewCanvas);
        RenderPage(m_currentPage);
        ((wxScrolledWindow *) m_previewCanvas)->Scroll(0, 0);
        m_previewCanvas->ClearBackground();
        m_previewCanvas->Refresh();
        m_previewCanvas->SetFocus();
    }
}

wxPrintDialogData& wxPrintPreviewBase::GetPrintDialogData()
{
    return m_printDialogData;
}

int wxPrintPreviewBase::GetZoom() const
{ return m_currentZoom; }
int wxPrintPreviewBase::GetMaxPage() const
{ return m_maxPage; }
int wxPrintPreviewBase::GetMinPage() const
{ return m_minPage; }
bool wxPrintPreviewBase::Ok() const
{ return m_isOk; }
void wxPrintPreviewBase::SetOk(bool ok)
{ m_isOk = ok; }
//----------------------------------------------------------------------------
// wxPrintPreview
//----------------------------------------------------------------------------

IMPLEMENT_CLASS(wxPrintPreview, wxPrintPreviewBase)

wxPrintPreview::wxPrintPreview(wxPrintout *printout,
                   wxPrintout *printoutForPrinting,
                   wxPrintDialogData *data) :
    wxPrintPreviewBase( printout, printoutForPrinting, data )
{
    m_pimpl = wxPrintFactory::GetFactory()->
        CreatePrintPreview( printout, printoutForPrinting, data );
}

wxPrintPreview::wxPrintPreview(wxPrintout *printout,
                   wxPrintout *printoutForPrinting,
                   wxPrintData *data ) :
    wxPrintPreviewBase( printout, printoutForPrinting, data )
{
    m_pimpl = wxPrintFactory::GetFactory()->
        CreatePrintPreview( printout, printoutForPrinting, data );
}

wxPrintPreview::~wxPrintPreview()
{
    delete m_pimpl;

    // don't delete twice
    m_printPrintout = NULL;
    m_previewPrintout = NULL;
    m_previewBitmap = NULL;
}

bool wxPrintPreview::SetCurrentPage(int pageNum)
{
    return m_pimpl->SetCurrentPage( pageNum );
}

int wxPrintPreview::GetCurrentPage() const
{
    return m_pimpl->GetCurrentPage();
}

void wxPrintPreview::SetPrintout(wxPrintout *printout)
{
    m_pimpl->SetPrintout( printout );
}

wxPrintout *wxPrintPreview::GetPrintout() const
{
    return m_pimpl->GetPrintout();
}

wxPrintout *wxPrintPreview::GetPrintoutForPrinting() const
{
    return m_pimpl->GetPrintoutForPrinting();
}

void wxPrintPreview::SetFrame(wxFrame *frame)
{
    m_pimpl->SetFrame( frame );
}

void wxPrintPreview::SetCanvas(wxPreviewCanvas *canvas)
{
    m_pimpl->SetCanvas( canvas );
}

wxFrame *wxPrintPreview::GetFrame() const
{
    return m_pimpl->GetFrame();
}

wxPreviewCanvas *wxPrintPreview::GetCanvas() const
{
    return m_pimpl->GetCanvas();
}

bool wxPrintPreview::PaintPage(wxPreviewCanvas *canvas, wxDC& dc)
{
    return m_pimpl->PaintPage( canvas, dc );
}

bool wxPrintPreview::DrawBlankPage(wxPreviewCanvas *canvas, wxDC& dc)
{
    return m_pimpl->DrawBlankPage( canvas, dc );
}

void wxPrintPreview::AdjustScrollbars(wxPreviewCanvas *canvas)
{
    m_pimpl->AdjustScrollbars( canvas );
}

bool wxPrintPreview::RenderPage(int pageNum)
{
    return m_pimpl->RenderPage( pageNum );
}

void wxPrintPreview::SetZoom(int percent)
{
    m_pimpl->SetZoom( percent );
}

wxPrintDialogData& wxPrintPreview::GetPrintDialogData()
{
    return m_pimpl->GetPrintDialogData();
}

int wxPrintPreview::GetMaxPage() const
{
    return m_pimpl->GetMaxPage();
}

int wxPrintPreview::GetMinPage() const
{
    return m_pimpl->GetMinPage();
}

bool wxPrintPreview::Ok() const
{
    return m_pimpl->Ok();
}

void wxPrintPreview::SetOk(bool ok)
{
    m_pimpl->SetOk( ok );
}

bool wxPrintPreview::Print(bool interactive)
{
    return m_pimpl->Print( interactive );
}

void wxPrintPreview::DetermineScaling()
{
    m_pimpl->DetermineScaling();
}


#endif // wxUSE_PRINTING_ARCHITECTURE
