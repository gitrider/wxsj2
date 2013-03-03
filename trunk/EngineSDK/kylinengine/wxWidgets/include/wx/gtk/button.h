/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/button.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id: button.h,v 1.30 2004/08/30 14:38:55 VS Exp $
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GTKBUTTONH__
#define __GTKBUTTONH__

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

class wxButton;

//-----------------------------------------------------------------------------
// global data
//-----------------------------------------------------------------------------

extern const wxChar *wxButtonNameStr;

//-----------------------------------------------------------------------------
// wxButton
//-----------------------------------------------------------------------------

class wxButton: public wxButtonBase
{
public:
    wxButton();
    wxButton(wxWindow *parent, wxWindowID id,
           const wxString& label = wxEmptyString,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxButtonNameStr)
    {
        Create(parent, id, label, pos, size, style, validator, name);
    }

    virtual ~wxButton();

    bool Create(wxWindow *parent, wxWindowID id,
           const wxString& label = wxEmptyString,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxButtonNameStr);

    virtual void SetDefault();
    virtual void SetLabel( const wxString &label );
    virtual bool Enable( bool enable = TRUE );

    // implementation
    // --------------

    void DoApplyWidgetStyle(GtkRcStyle *style);
    bool IsOwnGtkWindow( GdkWindow *window );

    // Since this wxButton doesn't derive from wxButtonBase (why?) we need
    // to override this here too...
    virtual bool ShouldInheritColours() const { return false; }
    
    static wxVisualAttributes
    GetClassDefaultAttributes(wxWindowVariant variant = wxWINDOW_VARIANT_NORMAL);

protected:
    virtual wxSize DoGetBestSize() const;

private:
    DECLARE_DYNAMIC_CLASS(wxButton)
};

#endif // __GTKBUTTONH__
