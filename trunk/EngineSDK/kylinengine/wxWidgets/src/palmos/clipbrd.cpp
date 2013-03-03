/////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/clipbrd.cpp
// Purpose:     Clipboard functionality
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: clipbrd.cpp,v 1.2 2005/01/21 21:04:00 ABX Exp $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "clipbrd.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/setup.h"
#endif

#if wxUSE_CLIPBOARD

#ifndef WX_PRECOMP
    #include "wx/object.h"
    #include "wx/event.h"
    #include "wx/app.h"
    #include "wx/frame.h"
    #include "wx/bitmap.h"
    #include "wx/utils.h"
    #include "wx/intl.h"
#endif

#if wxUSE_METAFILE
    #include "wx/metafile.h"
#endif

#include "wx/log.h"
#include "wx/clipbrd.h"

#include <string.h>

#include "wx/palmos/private.h"

#if wxUSE_WXDIB
#include "wx/palmos/dib.h"
#endif

#if wxUSE_DATAOBJ
    #include "wx/dataobj.h"
#endif

#if wxUSE_OLE && !defined(__WXWINCE__)
    // use OLE clipboard
    #define wxUSE_OLE_CLIPBOARD 1
#else // !wxUSE_DATAOBJ
    // use Win clipboard API
    #define wxUSE_OLE_CLIPBOARD 0
#endif

#if wxUSE_OLE_CLIPBOARD
    #include <ole2.h>
#endif // wxUSE_OLE_CLIPBOARD

// ===========================================================================
// implementation
// ===========================================================================

// ---------------------------------------------------------------------------
// old-style clipboard functions
// ---------------------------------------------------------------------------

static bool gs_wxClipboardIsOpen = FALSE;

bool wxOpenClipboard()
{
    return false;
}

bool wxCloseClipboard()
{
    return false;
}

bool wxEmptyClipboard()
{
    return false;
}

bool wxIsClipboardOpened()
{
    return false;
}

bool wxIsClipboardFormatAvailable(wxDataFormat dataFormat)
{
    return false;
}


bool wxSetClipboardData(wxDataFormat dataFormat,
                        const void *data,
                        int width, int height)
{
    return false;
}

void *wxGetClipboardData(wxDataFormat dataFormat, long *len)
{
    void *retval = NULL;

    return retval;
}

wxDataFormat wxEnumClipboardFormats(wxDataFormat dataFormat)
{
  return (wxDataFormat::NativeFormat)::EnumClipboardFormats(dataFormat);
}

int wxRegisterClipboardFormat(wxChar *formatName)
{
  return ::RegisterClipboardFormat(formatName);
}

bool wxGetClipboardFormatName(wxDataFormat dataFormat,
                              wxChar *formatName,
                              int maxCount)
{
  return false;
}

// ---------------------------------------------------------------------------
// wxClipboard
// ---------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxClipboard, wxObject)

wxClipboard::wxClipboard()
{
    m_clearOnExit = FALSE;
    m_isOpened = FALSE;
}

wxClipboard::~wxClipboard()
{
    if ( m_clearOnExit )
    {
        Clear();
    }
}

void wxClipboard::Clear()
{
}

bool wxClipboard::Flush()
{
    return false;
}

bool wxClipboard::Open()
{
    return wxOpenClipboard();
}

bool wxClipboard::IsOpened() const
{
    return wxIsClipboardOpened();
}

bool wxClipboard::SetData( wxDataObject *data )
{
    return false;
}

bool wxClipboard::AddData( wxDataObject *data )
{
    return false;
}

void wxClipboard::Close()
{
    wxCloseClipboard();
}

bool wxClipboard::IsSupported( wxDataFormat format )
{
    return wxIsClipboardFormatAvailable(format);
}

bool wxClipboard::GetData( wxDataObject& data )
{
    return false;
}

#endif // wxUSE_CLIPBOARD

