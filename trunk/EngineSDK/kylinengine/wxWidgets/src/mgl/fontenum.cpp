/////////////////////////////////////////////////////////////////////////////
// Name:        src/unix/fontenum.cpp
// Purpose:     wxFontEnumerator class for MGL
// Author:      Vaclav Slavik
// RCS-ID:      $Id: fontenum.cpp,v 1.9 2004/05/23 20:52:45 JS Exp $
// Copyright:   (c) 2001-2002 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "fontenum.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/defs.h"
#include "wx/dynarray.h"
#include "wx/string.h"
#include "wx/utils.h"

#include "wx/fontenum.h"
#include "wx/encinfo.h"
#include "wx/fontutil.h"

#include <mgraph.h>

// ============================================================================
// implementation
// ============================================================================


// ----------------------------------------------------------------------------
// wxFontEnumerator
// ----------------------------------------------------------------------------

bool wxFontEnumerator::EnumerateFacenames(wxFontEncoding encoding,
                                          bool fixedWidthOnly)
{
    bool found = FALSE;
    wxMGLFontFamilyList *list = wxTheFontsManager->GetFamilyList();
    wxMGLFontFamilyList::Node *node;
    wxMGLFontFamily *f = NULL;
    wxNativeEncodingInfo info;

    if ( encoding != wxFONTENCODING_SYSTEM )
        wxGetNativeFontEncoding(encoding, &info);
     
    for (node = list->GetFirst(); node; node = node->GetNext())
    {
        f = node->GetData();
        info.facename = f->GetName();
        if ( (!fixedWidthOnly || f->GetInfo()->isFixed) &&
             (encoding == wxFONTENCODING_SYSTEM || wxTestFontEncoding(info)) )
        {
            found = TRUE;
            if ( !OnFacename(f->GetName()) )
                return TRUE;
        }
    }

    return found;
}

bool wxFontEnumerator::EnumerateEncodings(const wxString& family)
{
    static wxFontEncoding encodings[] = 
    {
        wxFONTENCODING_ISO8859_1,
        wxFONTENCODING_ISO8859_2,
        wxFONTENCODING_ISO8859_3,
        wxFONTENCODING_ISO8859_4,
        wxFONTENCODING_ISO8859_5,
        wxFONTENCODING_ISO8859_6,
        wxFONTENCODING_ISO8859_7,
        wxFONTENCODING_ISO8859_8,
        wxFONTENCODING_ISO8859_9,
        wxFONTENCODING_ISO8859_10,
        //wxFONTENCODING_ISO8859_11,
        //wxFONTENCODING_ISO8859_12,
        wxFONTENCODING_ISO8859_13,
        wxFONTENCODING_ISO8859_14,
        wxFONTENCODING_ISO8859_15,
        wxFONTENCODING_CP1250,
        wxFONTENCODING_CP1251,
        wxFONTENCODING_CP1252,
        wxFONTENCODING_CP1253,
        wxFONTENCODING_CP1254,
        wxFONTENCODING_CP1255,
        wxFONTENCODING_CP1256,
        wxFONTENCODING_CP1257,
        wxFONTENCODING_KOI8,
        
        wxFONTENCODING_SYSTEM
    };
    
    static const char *encodingNames[] = 
    {
        "iso88590-1",
        "iso88590-2",
        "iso88590-3",
        "iso88590-4",
        "iso88590-5",
        "iso88590-6",
        "iso88590-7",
        "iso88590-8",
        "iso88590-9",
        "iso88590-10",
        "iso88590-13",
        "iso88590-14",
        "iso88590-15",
        "windows-1250",
        "windows-1251",
        "windows-1252",
        "windows-1253",
        "windows-1254",
        "windows-1255",
        "windows-1256",
        "windows-1257",
        "koi-8",
        NULL
    };
    
    wxNativeEncodingInfo info;
    info.facename = family;
    
    for (size_t i = 0; encodings[i] != wxFONTENCODING_SYSTEM; i++)
    {
        if ( !wxGetNativeFontEncoding(encodings[i], &info) ||
             !wxTestFontEncoding(info) ) 
            continue;
        if ( !OnFontEncoding(family, encodingNames[i]) )
            break;
    }

    return TRUE;
}
