/////////////////////////////////////////////////////////////////////////////
// Name:        bmpmotif.h
// Purpose:     Motif-specific bitmap routines
// Author:      Julian Smart, originally in bitmap.h
// Modified by:
// Created:     25/03/2003
// RCS-ID:      $Id: bmpmotif.h,v 1.5 2004/05/23 20:51:21 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_BMPMOTIF_H_
#define _WX_BMPMOTIF_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "bmpmotif.h"
#endif

#include "wx/defs.h"
#include "wx/bitmap.h"

class WXDLLEXPORT wxBitmapCache
{
public:
    wxBitmapCache()
    {
        m_labelPixmap = (WXPixmap)NULL;
        m_armPixmap = (WXPixmap)NULL;
        m_insensPixmap = (WXPixmap)NULL;
        m_image = (WXImage)NULL;
        m_display = (WXDisplay*)NULL;
        SetColoursChanged();
    }

    ~wxBitmapCache();

    void SetColoursChanged();
    void SetBitmap( const wxBitmap& bitmap );

    WXPixmap GetLabelPixmap( WXWidget w );
    WXPixmap GetInsensPixmap( WXWidget w = (WXWidget)NULL );
    WXPixmap GetArmPixmap( WXWidget w );
private:
    void InvalidateCache();
    void CreateImageIfNeeded( WXWidget w );

    struct
    {
        bool label  : 1;
        bool arm    : 1;
        bool insens : 1;
    } m_recalcPixmaps;
    wxBitmap m_bitmap;
    WXDisplay* m_display;
    WXPixmap m_labelPixmap, m_armPixmap, m_insensPixmap;
    WXImage m_image;
};

#endif // _WX_BMPMOTIF_H_
