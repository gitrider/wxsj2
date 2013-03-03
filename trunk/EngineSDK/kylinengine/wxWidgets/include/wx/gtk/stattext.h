/////////////////////////////////////////////////////////////////////////////
// Name:        stattext.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id: stattext.h,v 1.17 2005/04/07 22:52:32 RR Exp $
// Copyright:   (c) 1998 Robert Roebling
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __GTKSTATICTEXTH__
#define __GTKSTATICTEXTH__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface
#endif

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/list.h"
#include "wx/control.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxStaticText;

//-----------------------------------------------------------------------------
// global data
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// wxStaticText
//-----------------------------------------------------------------------------

class wxStaticText : public wxControl
{
public:
    wxStaticText();
    wxStaticText(wxWindow *parent,
                 wxWindowID id,
                 const wxString &label,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize, 
                 long style = 0,
                 const wxString &name = wxStaticTextNameStr );

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString &label,
                const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize, 
                long style = 0,
                const wxString &name = wxStaticTextNameStr );

    wxString GetLabel() const;
    void SetLabel( const wxString &label );

    bool SetFont( const wxFont &font );
    bool SetForegroundColour( const wxColour& colour );

    static wxVisualAttributes
    GetClassDefaultAttributes(wxWindowVariant variant = wxWINDOW_VARIANT_NORMAL);
    
    // implementation
    // --------------

protected:
    virtual void DoSetSize(int x, int y,
                           int width, int height,
                           int sizeFlags = wxSIZE_AUTO);
                           
    virtual wxSize DoGetBestSize() const;

    DECLARE_DYNAMIC_CLASS(wxStaticText)
};

#endif // __GTKSTATICTEXTH__
