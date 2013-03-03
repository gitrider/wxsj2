/////////////////////////////////////////////////////////////////////////////
// Name:        wx/palmos/statbox.h
// Purpose:     wxStaticBox class
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: statbox.h,v 1.2 2005/01/18 21:14:27 ABX Exp $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_STATBOX_H_
#define _WX_STATBOX_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "statbox.h"
#endif

// Group box
class WXDLLEXPORT wxStaticBox : public wxStaticBoxBase
{
public:
    wxStaticBox() { }

    wxStaticBox(wxWindow *parent, wxWindowID id,
                const wxString& label,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxStaticBoxNameStr)
    {
        Create(parent, id, label, pos, size, style, name);
    }

    bool Create(wxWindow *parent, wxWindowID id,
                const wxString& label,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxStaticBoxNameStr);

    // implementation from now on
    // --------------------------

    virtual WXLRESULT MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

    // overriden base class virtuals
    virtual bool AcceptsFocus() const { return FALSE; }

protected:
    virtual wxSize DoGetBestSize() const;

private:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxStaticBox)
};

#endif
    // _WX_STATBOX_H_
