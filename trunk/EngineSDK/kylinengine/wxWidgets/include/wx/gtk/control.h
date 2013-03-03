/////////////////////////////////////////////////////////////////////////////
// Name:        control.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id: control.h,v 1.28 2005/03/21 23:42:11 VZ Exp $
// Copyright:   (c) 1998 Robert Roebling, Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GTKCONTROLH__
#define __GTKCONTROLH__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface
#endif

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/list.h"
#include "wx/window.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxControl;

//-----------------------------------------------------------------------------
// wxControl
//-----------------------------------------------------------------------------

// C-linkage function pointer types for GetDefaultAttributesFromGTKWidget
extern "C" {
    typedef GtkWidget* (*wxGtkWidgetNew_t)(void);
    typedef GtkWidget* (*wxGtkWidgetNewFromStr_t)(const gchar*);
    typedef GtkWidget* (*wxGtkWidgetNewFromAdj_t)(GtkAdjustment*);
}

class wxControl : public wxControlBase
{
public:
    wxControl();
    wxControl(wxWindow *parent, wxWindowID id,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize, long style = 0,
             const wxValidator& validator = wxDefaultValidator,
             const wxString& name = wxControlNameStr)
    {
        Create(parent, id, pos, size, style, validator, name);
    }

    bool Create(wxWindow *parent, wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxControlNameStr);

    // this function will filter out '&' characters and will put the accelerator
    // char (the one immediately after '&') into m_chAccel (TODO not yet)
    virtual void SetLabel( const wxString &label );
    virtual wxString GetLabel() const;
    
    virtual wxVisualAttributes GetDefaultAttributes() const;

protected:
    virtual wxSize DoGetBestSize() const;
    void PostCreation(const wxSize& size);

#ifdef __WXGTK20__
    wxString PrepareLabelMnemonics( const wxString &label ) const;
#endif

    // These are used by GetDefaultAttributes
    static wxVisualAttributes
        GetDefaultAttributesFromGTKWidget(GtkWidget* widget,
                                          bool useBase = false,
                                          int state = -1);
    static wxVisualAttributes
        GetDefaultAttributesFromGTKWidget(wxGtkWidgetNew_t,
                                          bool useBase = false,
                                          int state = -1);
    static wxVisualAttributes
        GetDefaultAttributesFromGTKWidget(wxGtkWidgetNewFromStr_t,
                                          bool useBase = false,
                                          int state = -1);

    static wxVisualAttributes
        GetDefaultAttributesFromGTKWidget(wxGtkWidgetNewFromAdj_t,
                                          bool useBase = false,
                                          int state = -1);

    // Widgets that use the style->base colour for the BG colour should
    // override this and return true.
    virtual bool UseGTKStyleBase() const { return false; }

    wxString   m_label;
    char       m_chAccel;  // enabled to avoid breaking binary compatibility later on

private:
    DECLARE_DYNAMIC_CLASS(wxControl)
};

#endif // __GTKCONTROLH__
