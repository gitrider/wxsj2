/////////////////////////////////////////////////////////////////////////////
// Name:        dcclient.h
// Purpose:
// Author:      Vaclav Slavik
// Id:          $Id: dcclient.h,v 1.9 2004/05/23 20:51:18 JS Exp $
// Copyright:   (c) 2001-2002 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_DCCLIENT_H__
#define __WX_DCCLIENT_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "dcclient.h"
#endif

#include "wx/dc.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxWindowDC;
class WXDLLEXPORT wxPaintDC;
class WXDLLEXPORT wxClientDC;
class WXDLLEXPORT wxWindowMGL;

//-----------------------------------------------------------------------------
// wxWindowDC
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxWindowDC : public wxDC
{
public:
    wxWindowDC() {}
    virtual ~wxWindowDC();
    wxWindowDC(wxWindow *win);

protected:
    wxWindow *m_wnd;
    bool      m_inPaintHandler;

private:
    DECLARE_DYNAMIC_CLASS(wxWindowDC)
};

//-----------------------------------------------------------------------------
// wxClientDC
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxClientDC : public wxWindowDC
{
public:
    wxClientDC() : wxWindowDC() {}
    wxClientDC(wxWindow *win);

private:
    DECLARE_DYNAMIC_CLASS(wxClientDC)
};

//-----------------------------------------------------------------------------
// wxPaintDC
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxPaintDC : public wxClientDC
{
public:
    wxPaintDC() : wxClientDC() {}
    wxPaintDC(wxWindow *win) : wxClientDC(win) {}

private:
    DECLARE_DYNAMIC_CLASS(wxPaintDC)
};

#endif // __WX_DCCLIENT_H__
