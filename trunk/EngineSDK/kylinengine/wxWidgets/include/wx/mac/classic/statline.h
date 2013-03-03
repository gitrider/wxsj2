/////////////////////////////////////////////////////////////////////////////
// Name:        mac/statline.h
// Purpose:     a generic wxStaticLine class used for mac before adaptation
// Author:      Vadim Zeitlin
// Created:     28.06.99
// Version:     $Id: statline.h,v 1.4 2005/03/11 02:13:13 VZ Exp $
// Copyright:   (c) 1998 Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GENERIC_STATLINE_H_
#define _WX_GENERIC_STATLINE_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "statline.h"
#endif

class wxStaticBox;

// ----------------------------------------------------------------------------
// wxStaticLine
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxStaticLine : public wxStaticLineBase
{
public:
    // constructors and pseudo-constructors
    wxStaticLine() : m_statbox(NULL) { }

    wxStaticLine( wxWindow *parent,
                  wxWindowID id = wxID_ANY,
                  const wxPoint &pos = wxDefaultPosition,
                  const wxSize &size = wxDefaultSize,
                  long style = wxLI_HORIZONTAL,
                  const wxString &name = wxStaticTextNameStr )
        : m_statbox(NULL)
    {
        Create(parent, id, pos, size, style, name);
    }

    bool Create( wxWindow *parent,
                 wxWindowID id = wxID_ANY,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 long style = wxLI_HORIZONTAL,
                 const wxString &name = wxStaticTextNameStr );

    // it's necessary to override this wxWindow function because we
    // will want to return the main widget for m_statbox
    //
    WXWidget GetMainWidget() const;

protected:
    // we implement the static line using a static box
    wxStaticBox *m_statbox;

    DECLARE_DYNAMIC_CLASS(wxStaticLine)
};

#endif // _WX_GENERIC_STATLINE_H_

