/////////////////////////////////////////////////////////////////////////////
// Name:        wx/palmos/minifram.h
// Purpose:     wxMiniFrame class
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: minifram.h,v 1.2 2005/01/18 21:14:26 ABX Exp $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MINIFRAM_H_
#define _WX_MINIFRAM_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "minifram.h"
#endif

#include "wx/frame.h"

class WXDLLEXPORT wxMiniFrame : public wxFrame
{
public:
  wxMiniFrame() { }

  bool Create(wxWindow *parent,
              wxWindowID id,
              const wxString& title,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = wxCAPTION | wxCLIP_CHILDREN | wxRESIZE_BORDER,
              const wxString& name = wxFrameNameStr)
  {
      return wxFrame::Create(parent, id, title, pos, size,
                             style |
                             wxFRAME_TOOL_WINDOW |
                             (parent ? wxFRAME_FLOAT_ON_PARENT : 0), name);
  }

  wxMiniFrame(wxWindow *parent,
              wxWindowID id,
              const wxString& title,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = wxCAPTION | wxCLIP_CHILDREN | wxRESIZE_BORDER,
              const wxString& name = wxFrameNameStr)
  {
      Create(parent, id, title, pos, size, style, name);
  }

protected:
  DECLARE_DYNAMIC_CLASS_NO_COPY(wxMiniFrame)
};

#endif
    // _WX_MINIFRAM_H_
