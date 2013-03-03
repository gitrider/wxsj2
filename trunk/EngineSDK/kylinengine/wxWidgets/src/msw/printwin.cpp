/////////////////////////////////////////////////////////////////////////////
// Name:        printwin.cpp
// Purpose:     wxWindowsPrinter framework
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: printwin.cpp,v 1.42 2005/03/22 19:39:54 ABX Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "printwin.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/defs.h"

// Don't use the Windows printer if we're in wxUniv mode and using
// the PostScript architecture
#if wxUSE_PRINTING_ARCHITECTURE && (!defined(__WXUNIVERSAL__) || !wxUSE_POSTSCRIPT_ARCHITECTURE_IN_MSW)

#ifndef WX_PRECOMP
    #include "wx/window.h"
    #include "wx/msw/private.h"
    #include "wx/utils.h"
    #include "wx/dc.h"
    #include "wx/app.h"
    #include "wx/msgdlg.h"
    #include "wx/intl.h"
#endif

#include "wx/msw/printwin.h"
#include "wx/dcprint.h"
#include "wx/printdlg.h"
#include "wx/log.h"
#include "wx/msw/private.h"

#include <stdlib.h>

#include "wx/msw/wrapcdlg.h"
#ifndef __WIN32__
    #include <print.h>
#endif

// ---------------------------------------------------------------------------
// private functions
// ---------------------------------------------------------------------------

LONG APIENTRY _EXPORT wxAbortProc(HDC hPr, int Code);

// ---------------------------------------------------------------------------
// wxWin macros
// ---------------------------------------------------------------------------

    IMPLEMENT_DYNAMIC_CLASS(wxWindowsPrinter, wxPrinterBase)
    IMPLEMENT_CLASS(wxWindowsPrintPreview, wxPrintPreviewBase)

// ===========================================================================
// implementation
// ===========================================================================

// ---------------------------------------------------------------------------
// Printer
// ---------------------------------------------------------------------------

wxWindowsPrinter::wxWindowsPrinter(wxPrintDialogData *data)
                : wxPrinterBase(data)
{
    m_lpAbortProc = (WXFARPROC) MakeProcInstance((FARPROC) wxAbortProc, wxGetInstance());
}

wxWindowsPrinter::~wxWindowsPrinter()
{
    // avoids warning about statement with no effect (FreeProcInstance
    // doesn't do anything under Win32)
#if !defined(__WIN32__) && !defined(__NT__)
    FreeProcInstance((FARPROC) m_lpAbortProc);
#endif
}

bool wxWindowsPrinter::Print(wxWindow *parent, wxPrintout *printout, bool prompt)
{
    sm_abortIt = false;
    sm_abortWindow = NULL;

    if (!printout)
    {
        sm_lastError = wxPRINTER_ERROR;
        return false;
    }

    printout->SetIsPreview(false);

    if (m_printDialogData.GetMinPage() < 1)
        m_printDialogData.SetMinPage(1);
    if (m_printDialogData.GetMaxPage() < 1)
        m_printDialogData.SetMaxPage(9999);

    // Create a suitable device context
    wxDC *dc wxDUMMY_INITIALIZE(NULL);
    if (prompt)
    {
        dc = PrintDialog(parent);
        if (!dc)
            return false;
    }
    else
    {
        dc = new wxPrinterDC(m_printDialogData.GetPrintData());
    }

    // May have pressed cancel.
    if (!dc || !dc->Ok())
    {
        if (dc) delete dc;
        return false;
    }

    HDC hdc = ::GetDC(NULL);
    int logPPIScreenX = ::GetDeviceCaps(hdc, LOGPIXELSX);
    int logPPIScreenY = ::GetDeviceCaps(hdc, LOGPIXELSY);
    ::ReleaseDC(NULL, hdc);

    int logPPIPrinterX = ::GetDeviceCaps((HDC) dc->GetHDC(), LOGPIXELSX);
    int logPPIPrinterY = ::GetDeviceCaps((HDC) dc->GetHDC(), LOGPIXELSY);
    if (logPPIPrinterX == 0 || logPPIPrinterY == 0)
    {
        delete dc;
        sm_lastError = wxPRINTER_ERROR;
        return false;
    }

    printout->SetPPIScreen(logPPIScreenX, logPPIScreenY);
    printout->SetPPIPrinter(logPPIPrinterX, logPPIPrinterY);

    // Set printout parameters
    printout->SetDC(dc);

    int w, h;
    dc->GetSize(&w, &h);
    printout->SetPageSizePixels((int)w, (int)h);

    dc->GetSizeMM(&w, &h);
    printout->SetPageSizeMM((int)w, (int)h);

    // Create an abort window
    wxBusyCursor busyCursor;

    printout->OnPreparePrinting();

    // Get some parameters from the printout, if defined
    int fromPage, toPage;
    int minPage, maxPage;
    printout->GetPageInfo(&minPage, &maxPage, &fromPage, &toPage);

    if (maxPage == 0)
    {
        sm_lastError = wxPRINTER_ERROR;
        return false;
    }

    // Only set min and max, because from and to have been
    // set by the user
    m_printDialogData.SetMinPage(minPage);
    m_printDialogData.SetMaxPage(maxPage);

    wxWindow *win = CreateAbortWindow(parent, printout);
    wxYield();

#if defined(__WATCOMC__) || defined(__BORLANDC__) || defined(__GNUWIN32__) || defined(__SALFORDC__) || !defined(__WIN32__)
#ifdef STRICT
    ::SetAbortProc((HDC) dc->GetHDC(), (ABORTPROC) m_lpAbortProc);
#else
    ::SetAbortProc((HDC) dc->GetHDC(), (FARPROC) m_lpAbortProc);
#endif
#else
    ::SetAbortProc((HDC) dc->GetHDC(), (int (_stdcall *)
        // cast it to right type only if required
        // FIXME it's really cdecl and we're casting it to stdcall - either there is
        //       something I don't understand or it will crash at first usage
#ifdef STRICT
        (HDC, int)
#else
        ()
#endif
        )m_lpAbortProc);
#endif

    if (!win)
    {
        wxLogDebug(wxT("Could not create an abort dialog."));
        sm_lastError = wxPRINTER_ERROR;

        delete dc;
        return false;
    }
    sm_abortWindow = win;
    sm_abortWindow->Show();
    wxSafeYield();

    printout->OnBeginPrinting();

    sm_lastError = wxPRINTER_NO_ERROR;

    int minPageNum = minPage, maxPageNum = maxPage;

    if ( !m_printDialogData.GetAllPages() )
    {
        minPageNum = m_printDialogData.GetFromPage();
        maxPageNum = m_printDialogData.GetToPage();
    }

    int copyCount;
    for ( copyCount = 1;
          copyCount <= m_printDialogData.GetNoCopies();
          copyCount++ )
    {
        if ( !printout->OnBeginDocument(minPageNum, maxPageNum) )
        {
            wxLogError(_("Could not start printing."));
            sm_lastError = wxPRINTER_ERROR;
            break;
        }
        if (sm_abortIt)
        {
            sm_lastError = wxPRINTER_CANCELLED;
            break;
        }

        int pn;

        for ( pn = minPageNum;
              pn <= maxPageNum && printout->HasPage(pn);
              pn++ )
        {
            if ( sm_abortIt )
            {
                sm_lastError = wxPRINTER_CANCELLED;
                break;
            }

            dc->StartPage();
            bool cont = printout->OnPrintPage(pn);
            dc->EndPage();

            if ( !cont )
            {
                sm_lastError = wxPRINTER_CANCELLED;
                break;
            }
        }

        printout->OnEndDocument();
    }

    printout->OnEndPrinting();

    if (sm_abortWindow)
    {
        sm_abortWindow->Show(false);
        delete sm_abortWindow;
        sm_abortWindow = NULL;
    }

    delete dc;

    return sm_lastError == wxPRINTER_NO_ERROR;
}

wxDC* wxWindowsPrinter::PrintDialog(wxWindow *parent)
{
    wxDC* dc = (wxDC*) NULL;

    wxPrintDialog dialog(parent, & m_printDialogData);
    int ret = dialog.ShowModal();

    if (ret == wxID_OK)
    {
        dc = dialog.GetPrintDC();
        m_printDialogData = dialog.GetPrintDialogData();
        if (dc == NULL)
            sm_lastError = wxPRINTER_ERROR;
        else
            sm_lastError = wxPRINTER_NO_ERROR;
    }
    else
        sm_lastError = wxPRINTER_CANCELLED;

    return dc;
}

bool wxWindowsPrinter::Setup(wxWindow *WXUNUSED(parent))
{
#if 0
    // We no longer expose that dialog
    wxPrintDialog dialog(parent, & m_printDialogData);
    dialog.GetPrintDialogData().SetSetupDialog(true);

    int ret = dialog.ShowModal();

    if (ret == wxID_OK)
    {
        m_printDialogData = dialog.GetPrintDialogData();
    }

    return (ret == wxID_OK);
#else
    return false;
#endif
}

/*
* Print preview
*/

wxWindowsPrintPreview::wxWindowsPrintPreview(wxPrintout *printout,
                                             wxPrintout *printoutForPrinting,
                                             wxPrintDialogData *data)
                     : wxPrintPreviewBase(printout, printoutForPrinting, data)
{
    DetermineScaling();
}

wxWindowsPrintPreview::wxWindowsPrintPreview(wxPrintout *printout,
                                             wxPrintout *printoutForPrinting,
                                             wxPrintData *data)
                     : wxPrintPreviewBase(printout, printoutForPrinting, data)
{
    DetermineScaling();
}

wxWindowsPrintPreview::~wxWindowsPrintPreview()
{
}

bool wxWindowsPrintPreview::Print(bool interactive)
{
    if (!m_printPrintout)
        return false;
    wxWindowsPrinter printer(&m_printDialogData);
    return printer.Print(m_previewFrame, m_printPrintout, interactive);
}

void wxWindowsPrintPreview::DetermineScaling()
{
    HDC dc = ::GetDC(NULL);
    int screenWidth = ::GetDeviceCaps(dc, HORZSIZE);
    int screenYRes = ::GetDeviceCaps(dc, VERTRES);
    int logPPIScreenX = ::GetDeviceCaps(dc, LOGPIXELSX);
    int logPPIScreenY = ::GetDeviceCaps(dc, LOGPIXELSY);
    m_previewPrintout->SetPPIScreen(logPPIScreenX, logPPIScreenY);

    ::ReleaseDC(NULL, dc);

    // Get a device context for the currently selected printer
    wxPrinterDC printerDC(m_printDialogData.GetPrintData());

    int printerWidth = 150;
    int printerHeight wxDUMMY_INITIALIZE(250);
    int printerXRes = 1500;
    int printerYRes = 2500;

    dc = GetHdcOf(printerDC);
    if ( dc )
    {
        printerWidth = ::GetDeviceCaps(dc, HORZSIZE);
        printerHeight = ::GetDeviceCaps(dc, VERTSIZE);
        printerXRes = ::GetDeviceCaps(dc, HORZRES);
        printerYRes = ::GetDeviceCaps(dc, VERTRES);

        int logPPIPrinterX = ::GetDeviceCaps(dc, LOGPIXELSX);
        int logPPIPrinterY = ::GetDeviceCaps(dc, LOGPIXELSY);

        m_previewPrintout->SetPPIPrinter(logPPIPrinterX, logPPIPrinterY);
        m_previewPrintout->SetPageSizeMM(printerWidth, printerHeight);

        if (logPPIPrinterX == 0 || logPPIPrinterY == 0 || printerWidth == 0 || printerHeight == 0)
            m_isOk = false;
    }
    else
        m_isOk = false;

    m_pageWidth = printerXRes;
    m_pageHeight = printerYRes;

    // At 100%, the page should look about page-size on the screen.
    m_previewScale = (float)((float)screenWidth/(float)printerWidth);
    m_previewScale = m_previewScale * (float)((float)screenYRes/(float)printerYRes);
}

/****************************************************************************

  FUNCTION: wxAbortProc()

    PURPOSE:  Processes messages for the Abort Dialog box

****************************************************************************/

LONG APIENTRY _EXPORT wxAbortProc(HDC WXUNUSED(hPr), int WXUNUSED(Code))
{
    MSG msg;

    if (!wxPrinterBase::sm_abortWindow)              /* If the abort dialog isn't up yet */
        return(TRUE);

    /* Process messages intended for the abort dialog box */

    while (!wxPrinterBase::sm_abortIt && ::PeekMessage(&msg, 0, 0, 0, TRUE))
        if (!IsDialogMessage((HWND) wxPrinterBase::sm_abortWindow->GetHWND(), &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

    /* bAbort is TRUE (return is FALSE) if the user has aborted */

    return !wxPrinterBase::sm_abortIt;
}

#endif
    // wxUSE_PRINTING_ARCHITECTURE
