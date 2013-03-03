/////////////////////////////////////////////////////////////////////////////
// Name:        src/msw/dcprint.cpp
// Purpose:     wxPrinterDC class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: dcprint.cpp,v 1.58 2005/05/31 09:20:30 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "dcprint.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/string.h"
    #include "wx/log.h"
    #include "wx/window.h"
    #include "wx/dcmemory.h"
#endif

#if wxUSE_PRINTING_ARCHITECTURE

#include "wx/msw/private.h"

#if wxUSE_WXDIB
#include "wx/msw/dib.h"
#endif

#include "wx/dcprint.h"
#include "wx/printdlg.h"
#include "wx/msw/printdlg.h"
#include "wx/math.h"

#include "wx/msw/wrapcdlg.h"
#ifndef __WIN32__
    #include <print.h>
#endif

// mingw32 defines GDI_ERROR incorrectly
#if defined(__GNUWIN32__) || !defined(GDI_ERROR)
    #undef GDI_ERROR
    #define GDI_ERROR ((int)-1)
#endif

// ----------------------------------------------------------------------------
// wxWin macros
// ----------------------------------------------------------------------------

IMPLEMENT_CLASS(wxPrinterDC, wxDC)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxPrinterDC construction
// ----------------------------------------------------------------------------

// This form is deprecated
wxPrinterDC::wxPrinterDC(const wxString& driver_name,
                         const wxString& device_name,
                         const wxString& file,
                         bool interactive,
                         int orientation)
{
    m_isInteractive = interactive;

    if ( !file.empty() )
        m_printData.SetFilename(file);

#if wxUSE_COMMON_DIALOGS
    if ( interactive )
    {
        PRINTDLG pd;

        pd.lStructSize = sizeof( PRINTDLG );
        pd.hwndOwner = (HWND) NULL;
        pd.hDevMode = (HANDLE)NULL;
        pd.hDevNames = (HANDLE)NULL;
        pd.Flags = PD_RETURNDC | PD_NOSELECTION | PD_NOPAGENUMS;
        pd.nFromPage = 0;
        pd.nToPage = 0;
        pd.nMinPage = 0;
        pd.nMaxPage = 0;
        pd.nCopies = 1;
        pd.hInstance = (HINSTANCE)NULL;

        m_ok = PrintDlg( &pd ) != 0;
        if ( m_ok )
        {
            m_hDC = (WXHDC) pd.hDC;
        }
    }
    else
#endif // wxUSE_COMMON_DIALOGS
    {
        if ( !driver_name.empty() && !device_name.empty() && !file.empty() )
        {
            m_hDC = (WXHDC) CreateDC(driver_name, device_name, file, NULL);
        }
        else // we don't have all parameters, ask the user
        {
            wxPrintData printData;
            printData.SetOrientation(orientation);
            m_hDC = wxGetPrinterDC(printData);
        }

        m_ok = m_hDC ? true: false;

        // as we created it, we must delete it as well
        m_bOwnsDC = true;
    }

    Init();
}

wxPrinterDC::wxPrinterDC(const wxPrintData& printData)
{
    m_printData = printData;

    m_isInteractive = false;

    m_hDC = wxGetPrinterDC(printData);
    m_ok = m_hDC != 0;
    m_bOwnsDC = true;

    Init();
}


wxPrinterDC::wxPrinterDC(WXHDC dc)
{
    m_isInteractive = false;

    m_hDC = dc;
    m_bOwnsDC = true;
    m_ok = true;
}

void wxPrinterDC::Init()
{
    if ( m_hDC )
    {
        //     int width = GetDeviceCaps(m_hDC, VERTRES);
        //     int height = GetDeviceCaps(m_hDC, HORZRES);
        SetMapMode(wxMM_TEXT);

        SetBrush(*wxBLACK_BRUSH);
        SetPen(*wxBLACK_PEN);
    }
}

// ----------------------------------------------------------------------------
// wxPrinterDC {Start/End}{Page/Doc} methods
// ----------------------------------------------------------------------------

bool wxPrinterDC::StartDoc(const wxString& message)
{
    DOCINFO docinfo;
    docinfo.cbSize = sizeof(DOCINFO);
    docinfo.lpszDocName = (const wxChar*)message;

    wxString filename(m_printData.GetFilename());

    if (filename.empty())
        docinfo.lpszOutput = NULL;
    else
        docinfo.lpszOutput = (const wxChar *) filename;

#if defined(__WIN95__)
    docinfo.lpszDatatype = NULL;
    docinfo.fwType = 0;
#endif

    if (!m_hDC)
        return false;

    int ret = ::StartDoc(GetHdc(), &docinfo);

    if (ret <= 0)
    {
        DWORD lastError = GetLastError();
        wxLogDebug(wxT("wxDC::StartDoc failed with error: %ld\n"), lastError);
    }

    return (ret > 0);
}

void wxPrinterDC::EndDoc()
{
    if (m_hDC) ::EndDoc((HDC) m_hDC);
}

void wxPrinterDC::StartPage()
{
    if (m_hDC)
        ::StartPage((HDC) m_hDC);
}

void wxPrinterDC::EndPage()
{
    if (m_hDC)
        ::EndPage((HDC) m_hDC);
}

// Returns default device and port names
static bool wxGetDefaultDeviceName(wxString& deviceName, wxString& portName)
{
    deviceName.clear();

    LPDEVNAMES  lpDevNames;
    LPTSTR      lpszDeviceName;
    LPTSTR      lpszPortName;

    PRINTDLG    pd;

    // Cygwin has trouble believing PRINTDLG is 66 bytes - thinks it is 68
#ifdef __GNUWIN32__
    memset(&pd, 0, 66);
    pd.lStructSize    = 66; // sizeof(PRINTDLG);
#else
    memset(&pd, 0, sizeof(PRINTDLG));
    pd.lStructSize    = sizeof(PRINTDLG);
#endif

    pd.hwndOwner      = (HWND)NULL;
    pd.hDevMode       = NULL; // Will be created by PrintDlg
    pd.hDevNames      = NULL; // Ditto
    pd.Flags          = PD_RETURNDEFAULT;
    pd.nCopies        = 1;

    if (!PrintDlg((LPPRINTDLG)&pd))
    {
        if ( pd.hDevMode )
            GlobalFree(pd.hDevMode);
        if (pd.hDevNames)
            GlobalFree(pd.hDevNames);

        return false;
    }

    if (pd.hDevNames)
    {
        lpDevNames = (LPDEVNAMES)GlobalLock(pd.hDevNames);
        lpszDeviceName = (LPTSTR)lpDevNames + lpDevNames->wDeviceOffset;
        lpszPortName   = (LPTSTR)lpDevNames + lpDevNames->wOutputOffset;

        deviceName = lpszDeviceName;
        portName = lpszPortName;

        GlobalUnlock(pd.hDevNames);
        GlobalFree(pd.hDevNames);
        pd.hDevNames=NULL;
    }

    if (pd.hDevMode)
    {
        GlobalFree(pd.hDevMode);
        pd.hDevMode=NULL;
    }
    return ( !deviceName.empty() );
}

// Gets an HDC for the specified printer configuration
WXHDC WXDLLEXPORT wxGetPrinterDC(const wxPrintData& printDataConst)
{
#if defined(__WXUNIVERSAL__) && (!defined(__WXMSW__) || wxUSE_POSTSCRIPT_ARCHITECTURE_IN_MSW)

#if 0
    wxPostScriptPrintNativeData *data =
        (wxPostScriptPrintNativeData *) printDataConst.GetNativeData();
    // FIXME: how further ???
#else
    return 0;
#endif

#else // Postscript vs. native Windows

    wxWindowsPrintNativeData *data =
        (wxWindowsPrintNativeData *) printDataConst.GetNativeData();

    data->TransferFrom( printDataConst );

    wxChar* driverName = (wxChar*) NULL;

    wxString devNameStr = printDataConst.GetPrinterName();
    wxChar* portName = (wxChar*) NULL; // Obsolete in WIN32

    const wxChar* deviceName;
    if ( !devNameStr )
        deviceName = (wxChar*) NULL;
    else
        deviceName = devNameStr.c_str();

    LPDEVMODE lpDevMode = (LPDEVMODE) NULL;

    HGLOBAL hDevMode = (HGLOBAL)(DWORD) data->GetDevMode();

    if ( hDevMode )
        lpDevMode = (DEVMODE*) GlobalLock(hDevMode);

    if ( !devNameStr )
    {
        // Retrieve the default device name
        wxString portName;
        if ( !wxGetDefaultDeviceName(devNameStr, portName) )
        {
            return 0; // Could not get default device name
        }
        deviceName = devNameStr.c_str();
    }

#ifdef __WIN32__
    HDC hDC = CreateDC(driverName, deviceName, portName, (DEVMODE *) lpDevMode);
#else
    HDC hDC = CreateDC(driverName, deviceName, portName, (LPSTR) lpDevMode);
#endif

    if (hDevMode && lpDevMode)
        GlobalUnlock(hDevMode);

    return (WXHDC) hDC;
#endif
}

// ----------------------------------------------------------------------------
// wxPrinterDC bit blitting/bitmap drawing
// ----------------------------------------------------------------------------

// helper of DoDrawBitmap() and DoBlit()
static
bool DrawBitmapUsingStretchDIBits(HDC hdc,
                                  const wxBitmap& bmp,
                                  wxCoord x, wxCoord y)
{
#if wxUSE_WXDIB
    wxDIB dib(bmp);
    bool ok = dib.IsOk();
    if ( !ok )
        return false;

    DIBSECTION ds;
    if ( !::GetObject(dib.GetHandle(), sizeof(ds), &ds) )
    {
        wxLogLastError(_T("GetObject(DIBSECTION)"));

        return false;
    }

    // ok, we've got all data we need, do blit it
    if ( ::StretchDIBits
            (
                hdc,
                x, y,
                ds.dsBmih.biWidth, ds.dsBmih.biHeight,
                0, 0,
                ds.dsBmih.biWidth, ds.dsBmih.biHeight,
                ds.dsBm.bmBits,
                (LPBITMAPINFO)&ds.dsBmih,
                DIB_RGB_COLORS,
                SRCCOPY
            ) == GDI_ERROR )
    {
        wxLogLastError(wxT("StretchDIBits"));

        return false;
    }

    return true;
#else
    return false;
#endif
}

void wxPrinterDC::DoDrawBitmap(const wxBitmap& bmp,
                               wxCoord x, wxCoord y,
                               bool useMask)
{
    wxCHECK_RET( bmp.Ok(), _T("invalid bitmap in wxPrinterDC::DrawBitmap") );

    int width = bmp.GetWidth(),
        height = bmp.GetHeight();

    if ( !(::GetDeviceCaps(GetHdc(), RASTERCAPS) & RC_STRETCHDIB) ||
            !DrawBitmapUsingStretchDIBits(GetHdc(), bmp, x, y) )
    {
        // no support for StretchDIBits() or an error occurred if we got here
        wxMemoryDC memDC;
        memDC.SelectObject(bmp);

        Blit(x, y, width, height, &memDC, 0, 0, wxCOPY, useMask);

        memDC.SelectObject(wxNullBitmap);
    }
}

bool wxPrinterDC::DoBlit(wxCoord xdest, wxCoord ydest,
                         wxCoord width, wxCoord height,
                         wxDC *source,
                         wxCoord WXUNUSED(xsrc), wxCoord WXUNUSED(ysrc),
                         int WXUNUSED(rop), bool useMask,
                         wxCoord WXUNUSED(xsrcMask), wxCoord WXUNUSED(ysrcMask))
{
    wxBitmap& bmp = source->GetSelectedBitmap();
    wxMask *mask = useMask ? bmp.GetMask() : NULL;
    if ( mask )
    {
        // If we are printing source colours are screen colours not printer
        // colours and so we need copy the bitmap pixel by pixel.
        RECT rect;
        HDC dcSrc = GetHdcOf(*source);
        MemoryHDC dcMask(dcSrc);
        SelectInHDC selectMask(dcMask, (HBITMAP)mask->GetMaskBitmap());

        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                COLORREF cref = ::GetPixel(dcMask, x, y);
                if (cref)
                {
                    HBRUSH brush = ::CreateSolidBrush(::GetPixel(dcSrc, x, y));
                    rect.left = xdest + x;
                    rect.right = rect.left + 1;
                    rect.top = ydest + y;
                    rect.bottom = rect.top + 1;
                    ::FillRect(GetHdc(), &rect, brush);
                    ::DeleteObject(brush);
                }
            }
        }
    }
    else // no mask
    {
        if ( !(::GetDeviceCaps(GetHdc(), RASTERCAPS) & RC_STRETCHDIB) ||
                !DrawBitmapUsingStretchDIBits(GetHdc(), bmp, xdest, ydest) )
        {
            // no support for StretchDIBits

            // as we are printing, source colours are screen colours not
            // printer colours and so we need copy the bitmap pixel by pixel.
            HDC dcSrc = GetHdcOf(*source);
            RECT rect;
            for (int y = 0; y < height; y++)
            {
                // optimization: draw identical adjacent pixels together.
                for (int x = 0; x < width; x++)
                {
                    COLORREF col = ::GetPixel(dcSrc, x, y);
                    HBRUSH brush = ::CreateSolidBrush( col );

                    rect.left = xdest + x;
                    rect.top = ydest + y;
                    while( (x + 1 < width) &&
                                (::GetPixel(dcSrc, x + 1, y) == col ) )
                    {
                        ++x;
                    }
                    rect.right = xdest + x + 1;
                    rect.bottom = rect.top + 1;
                    ::FillRect((HDC) m_hDC, &rect, brush);
                    ::DeleteObject(brush);
                }
            }
        }
    }

    return true;
}

#endif
    // wxUSE_PRINTING_ARCHITECTURE
