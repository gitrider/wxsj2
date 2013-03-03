/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/dcmemory.h
// Purpose:     wxMemoryDC class
// Author:      David Elliott
// Modified by:
// Created:     2003/03/16
// RCS-ID:      $Id: dcmemory.h,v 1.8 2005/01/17 21:30:52 DE Exp $
// Copyright:   (c) 2003 David Elliott
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_COCOA_DCMEMORY_H__
#define __WX_COCOA_DCMEMORY_H__

#include "wx/dc.h"

class WXDLLEXPORT wxMemoryDC: public wxDC
{
    DECLARE_DYNAMIC_CLASS(wxMemoryDC)
public:
    wxMemoryDC(void);
    wxMemoryDC( wxDC *dc ); // Create compatible DC
    ~wxMemoryDC(void);
    virtual void SelectObject(const wxBitmap& bitmap);
    virtual void DoGetSize(int *width, int *height) const;
protected:
    wxBitmap m_selectedBitmap;
    WX_NSImage m_cocoaNSImage;
// DC stack
    virtual bool CocoaLockFocus();
    virtual bool CocoaUnlockFocus();
    virtual bool CocoaGetBounds(void *rectData);
// Blitting
    virtual bool CocoaDoBlitOnFocusedDC(wxCoord xdest, wxCoord ydest,
        wxCoord width, wxCoord height, wxCoord xsrc, wxCoord ysrc,
        int logicalFunc, bool useMask, wxCoord xsrcMask, wxCoord ysrcMask);
};

#endif // __WX_COCOA_DCMEMORY_H__
