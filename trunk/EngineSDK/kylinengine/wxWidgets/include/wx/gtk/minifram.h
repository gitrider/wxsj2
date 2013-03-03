/////////////////////////////////////////////////////////////////////////////
// Name:        minifram.h
// Purpose:     wxMiniFrame class
// Author:      Robert Roebling
// RCS-ID:      $Id: minifram.h,v 1.9 2004/11/03 20:47:05 RR Exp $
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GTKMINIFRAMEH__
#define __GTKMINIFRAMEH__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface
#endif

#include "wx/defs.h"

#if wxUSE_MINIFRAME

#include "wx/object.h"
#include "wx/frame.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxMiniFrame;

//-----------------------------------------------------------------------------
// wxMiniFrame
//-----------------------------------------------------------------------------

class wxMiniFrame: public wxFrame
{
    DECLARE_DYNAMIC_CLASS(wxMiniFrame)

public:
    wxMiniFrame() {}
    wxMiniFrame(wxWindow *parent,
            wxWindowID id,
            const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE | wxTINY_CAPTION_HORIZ,
            const wxString& name = wxFrameNameStr)
    {
        Create(parent, id, title, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
            wxWindowID id,
            const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE | wxTINY_CAPTION_HORIZ,
            const wxString& name = wxFrameNameStr);

    virtual void SetTitle( const wxString &title );
 // implementation
 
    bool   m_isDragging;
    int    m_oldX,m_oldY;
    int    m_diffX,m_diffY;
};

#endif

#endif
  //  __GTKMINIFRAMEH__
