/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/checkbox.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id: checkbox.h,v 1.30 2004/06/26 15:53:06 VS Exp $
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GTKCHECKBOXH__
#define __GTKCHECKBOXH__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface
#endif

// ----------------------------------------------------------------------------
// wxCheckBox
// ----------------------------------------------------------------------------

class wxCheckBox : public wxCheckBoxBase
{
public:
    wxCheckBox();
    wxCheckBox( wxWindow *parent, wxWindowID id, const wxString& label,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxCheckBoxNameStr)
    {
        Create(parent, id, label, pos, size, style, validator, name);
    }
    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& label,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxCheckBoxNameStr );

    void SetValue( bool state );
    bool GetValue() const;

    virtual void SetLabel( const wxString& label );
    virtual bool Enable( bool enable = TRUE );

    static wxVisualAttributes
    GetClassDefaultAttributes(wxWindowVariant variant = wxWINDOW_VARIANT_NORMAL);
    
    // implementation
    // --------------

    void DoApplyWidgetStyle(GtkRcStyle *style);
    bool IsOwnGtkWindow( GdkWindow *window );
    void OnInternalIdle();

    GtkWidget *m_widgetCheckbox;
    GtkWidget *m_widgetLabel;

    bool       m_blockEvent;

protected:
    virtual wxSize DoGetBestSize() const;

#ifdef __WXGTK20__
    void DoSet3StateValue(wxCheckBoxState state);
    wxCheckBoxState DoGet3StateValue() const;
#endif

private:
    DECLARE_DYNAMIC_CLASS(wxCheckBox)
};

#endif // __GTKCHECKBOXH__
