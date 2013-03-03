/////////////////////////////////////////////////////////////////////////////
// Name:        imagpcx.h
// Purpose:     wxImage PCX handler
// Author:      Guillermo Rodriguez Garcia <guille@iies.es>
// RCS-ID:      $Id: imagpcx.h,v 1.8 2004/09/16 18:13:25 ABX Exp $
// Copyright:   (c) 1999 Guillermo Rodriguez Garcia
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_IMAGPCX_H_
#define _WX_IMAGPCX_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "imagpcx.h"
#endif

#include "wx/image.h"


//-----------------------------------------------------------------------------
// wxPCXHandler
//-----------------------------------------------------------------------------

#if wxUSE_PCX
class WXDLLEXPORT wxPCXHandler : public wxImageHandler
{
public:
    inline wxPCXHandler()
    {
        m_name = wxT("PCX file");
        m_extension = wxT("pcx");
        m_type = wxBITMAP_TYPE_PCX;
        m_mime = wxT("image/pcx");
    }

#if wxUSE_STREAMS
    virtual bool LoadFile( wxImage *image, wxInputStream& stream, bool verbose=true, int index=-1 );
    virtual bool SaveFile( wxImage *image, wxOutputStream& stream, bool verbose=true );
    virtual bool DoCanRead( wxInputStream& stream );
#endif // wxUSE_STREAMS

private:
    DECLARE_DYNAMIC_CLASS(wxPCXHandler)
};
#endif // wxUSE_PCX


#endif
  // _WX_IMAGPCX_H_

