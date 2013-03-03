/////////////////////////////////////////////////////////////////////////////
// Name:        imaggif.h
// Purpose:     wxImage GIF handler
// Author:      Vaclav Slavik & Guillermo Rodriguez Garcia
// RCS-ID:      $Id: imaggif.h,v 1.11 2004/09/16 18:13:25 ABX Exp $
// Copyright:   (c) Guillermo Rodriguez Garcia
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_IMAGGIF_H_
#define _WX_IMAGGIF_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "imaggif.h"
#endif

#include "wx/image.h"


//-----------------------------------------------------------------------------
// wxGIFHandler
//-----------------------------------------------------------------------------

#if wxUSE_GIF

class WXDLLEXPORT wxGIFHandler : public wxImageHandler
{
public:
    inline wxGIFHandler()
    {
        m_name = wxT("GIF file");
        m_extension = wxT("gif");
        m_type = wxBITMAP_TYPE_GIF;
        m_mime = wxT("image/gif");
    }

#if wxUSE_STREAMS
    virtual bool LoadFile( wxImage *image, wxInputStream& stream, bool verbose=true, int index=-1 );
    virtual bool SaveFile( wxImage *image, wxOutputStream& stream, bool verbose=true );
    virtual bool DoCanRead( wxInputStream& stream );
#endif

private:
    DECLARE_DYNAMIC_CLASS(wxGIFHandler)
};
#endif


#endif
  // _WX_IMAGGIF_H_

