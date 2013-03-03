/////////////////////////////////////////////////////////////////////////////
// Name:        imagiff.h
// Purpose:     wxImage handler for Amiga IFF images
// Author:      Steffen Gutmann
// RCS-ID:      $Id: imagiff.h,v 1.8 2004/09/16 18:13:25 ABX Exp $
// Copyright:   (c) Steffen Gutmann, 2002
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_IMAGE_IFF_H_
#define _WX_IMAGE_IFF_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "imagiff.h"
#endif

#include "wx/image.h"

//-----------------------------------------------------------------------------
// wxIFFHandler
//-----------------------------------------------------------------------------

#if wxUSE_IMAGE && wxUSE_IFF

class WXDLLEXPORT wxIFFHandler : public wxImageHandler
{
public:
    wxIFFHandler()
    {
        m_name = wxT("IFF file");
        m_extension = wxT("iff");
        m_type = wxBITMAP_TYPE_IFF;
        m_mime = wxT("image/x-iff");
    }

#if wxUSE_STREAMS
    virtual bool LoadFile(wxImage *image, wxInputStream& stream, bool verbose=true, int index=-1);
    virtual bool SaveFile(wxImage *image, wxOutputStream& stream, bool verbose=true);
    virtual bool DoCanRead(wxInputStream& stream);
#endif

private:
    DECLARE_DYNAMIC_CLASS(wxIFFHandler)
};

#endif // wxUSE_IMAGE && wxUSE_IFF

#endif // _WX_IMAGE_IFF_H_
