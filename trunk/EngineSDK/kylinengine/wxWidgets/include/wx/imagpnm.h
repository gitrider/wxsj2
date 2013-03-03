/////////////////////////////////////////////////////////////////////////////
// Name:        imagpnm.h
// Purpose:     wxImage PNM handler
// Author:      Sylvain Bougnoux
// RCS-ID:      $Id: imagpnm.h,v 1.8 2004/09/16 18:13:25 ABX Exp $
// Copyright:   (c) Sylvain Bougnoux
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_IMAGPNM_H_
#define _WX_IMAGPNM_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "imagpnm.h"
#endif

#include "wx/image.h"

//-----------------------------------------------------------------------------
// wxPNMHandler
//-----------------------------------------------------------------------------

#if wxUSE_PNM
class WXDLLEXPORT wxPNMHandler : public wxImageHandler
{
public:
    inline wxPNMHandler()
    {
        m_name = wxT("PNM file");
        m_extension = wxT("pnm");
        m_type = wxBITMAP_TYPE_PNM;
        m_mime = wxT("image/pnm");
    }

#if wxUSE_STREAMS
    virtual bool LoadFile( wxImage *image, wxInputStream& stream, bool verbose=true, int index=-1 );
    virtual bool SaveFile( wxImage *image, wxOutputStream& stream, bool verbose=true );
    virtual bool DoCanRead( wxInputStream& stream );
#endif

private:
    DECLARE_DYNAMIC_CLASS(wxPNMHandler)
};
#endif


#endif
  // _WX_IMAGPNM_H_

